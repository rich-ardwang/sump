/*
 * This program neads the hard ulimit for real time priority to be at
 * least PRIORITY (defined below), the capability CAP_SYS_RESOURCE, or
 * root priviledge.  Note the file capabilities and SUID bits are
 * *not* effective in a partition mounted with the nosuid option.
 *
 * You can set the hard limit by adding a line to the
 * /etc/security/limits.conf like this:
 *
 * jdriscoll  hard  rtprio 99
 *
 * You can grant the capability via
 *
 * sudo setcap cap_sys_resource=p <executable>
 *
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/capability.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

#define PRIORITY (50)
#define LISTEN_BACKLOG (5000)
#define PAYLOAD_LEN (1400)
#define DELAY_SEC (0)
#define DELAY_NSEC (950000000)
#define MAX_CONNECTIONS (4000)
#define NOFILES (4004)


static int verbose = 1;

struct timespec diff(struct timespec start, struct timespec end) {
  struct timespec temp;
  if ((end.tv_nsec - start.tv_nsec) < 0) {
    temp.tv_sec = end.tv_sec - start.tv_sec - 1;
    temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec - start.tv_sec;
    temp.tv_nsec = end.tv_nsec - start.tv_nsec;
  }
  return temp;
}

struct timespec add(struct timespec start, struct timespec delay) {
  struct timespec temp;
  if ((delay.tv_nsec+start.tv_nsec) > 1000000000) {
    temp.tv_sec = delay.tv_sec + start.tv_sec + 1;
    temp.tv_nsec = delay.tv_nsec + start.tv_nsec - 1000000000;
  } else {
    temp.tv_sec = delay.tv_sec + start.tv_sec;
    temp.tv_nsec = delay.tv_nsec + start.tv_nsec;
  }
  return temp;
}

/*
 * Set_policy_priority
 *
 * This is used to set the priority and policy for the real-time
 * scheduler.  This normally requires some form of privilege, as the
 * default hard ulimit of 0 will prevent an unprivileged program from
 * doing so.
 *
 * The most secure thing to do is grant the executable the potential
 * to enable CAP_SYS_RESOURCE (with sudo setcap cap_sys_resource=p
 * <executable>).  If it is needed, this routine will enable the
 * capability, raise the hard limit, and then irrevocably drop the
 * privilege.
 *
 */

int set_policy_priority(int policy, int priority, int nofiles) {
  const cap_value_t cap_vector[1] = { CAP_SYS_RESOURCE };
  cap_t privilege_dropped = cap_init();
  cap_t privilege_off = cap_dup(privilege_dropped);
  cap_set_flag(privilege_off, CAP_PERMITTED, 1, cap_vector, CAP_SET);
  cap_t privilege_on = cap_dup(privilege_off);
  cap_set_flag(privilege_on, CAP_EFFECTIVE, 1, cap_vector, CAP_SET);

  struct sched_param param;
  struct rlimit rl;
  int e, min, max;

  // See if priority we want is in the range offered by SCHED_FIFO

  min = sched_get_priority_min(policy);
  max = sched_get_priority_max(policy);
  if (verbose) {
    fprintf(stderr, "For policy SCHED_FIFO min priority is %d, max is %d.\n", min, max);
  }

  if ((min>priority)||(max<priority)) {
    fprintf(stderr, "Desired priority of %d is out of range.\n", priority);
    return 1;
  }

  // See if the RTPRIO limits allows the priority we want

  if (getrlimit(RLIMIT_RTPRIO, &rl) != 0) {
    e = errno;
    fprintf(stderr, "Failed to getrlimit(): %s.\n", strerror(e));
    return 1;
  }

  if (verbose) {
    fprintf(stderr, "RTPRIO soft limit is %d, hard is %d.\n", 
	    (int) rl.rlim_cur, (int) rl.rlim_max);
  }

  // Adjust hard limit if necessary

  if (rl.rlim_max < priority) {
    if (cap_set_proc(privilege_on) != 0) {
      fprintf(stderr, "Need to raise RTPRIO hard limit, but can't enable CAP_SYS_RESOURCE.\n");
      return 1;
    }
    rl.rlim_max = priority;
    if (setrlimit(RLIMIT_RTPRIO, &rl) != 0) {
      e = errno;
      fprintf(stderr, "Failed to raise hard limit for RTPRIO to %d: %s.\n", 
	     (int) rl.rlim_max, strerror(e));
      return 1;
    }
    if (cap_set_proc(privilege_off)) {
      fprintf(stderr, "Failed to turn off privileges.\n");
      return 1;
    }
    if (verbose) {
      printf("Raised hard limit for RTPRIO to %d.\n", (int) rl.rlim_max);
    }
  }

  // Adjust soft limit if necessary

  if (rl.rlim_cur < priority) {
    rl.rlim_cur = priority;
    if (setrlimit(RLIMIT_RTPRIO, &rl) != 0) {
      e = errno;
      fprintf(stderr, "Failed to raise soft limit for RTPRIO to %d: %s.\n", 
	     (int) rl.rlim_cur, strerror(e));
      return 1;
    }
    if (verbose) {
      printf("Raised soft limit for RTPRIO to %d.\n", (int) rl.rlim_cur);
    }
  }

  // Set desired priority with class SCHED_FIFO

  param.sched_priority = priority;
  if (sched_setscheduler(0, policy, &param) != 0) {
    e = errno;
    fprintf(stderr, "Setting policy failed: %s.\n", strerror(e));
    return 1;
  } else if (verbose) {
    printf("Set policy SCHED_FIFO, priority %d.\n", param.sched_priority);
  }
  
  // See if the NOFILE limits allows the number of fds we want

  if (getrlimit(RLIMIT_NOFILE, &rl) != 0) {
    e = errno;
    fprintf(stderr, "Failed to getrlimit(): %s.\n", strerror(e));
    return 1;
  }

  if (verbose) {
    fprintf(stderr, "NOFILE soft limit is %d, hard is %d.\n", 
	    (int) rl.rlim_cur, (int) rl.rlim_max);
  }

  // Adjust hard limit if necessary

  if (rl.rlim_max < nofiles) {
    if (cap_set_proc(privilege_on) != 0) {
      fprintf(stderr, "Need to raise NOFILE hard limit, but can't enable CAP_SYS_RESOURCE.\n");
      return 1;
    }
    rl.rlim_max = nofiles;
    if (setrlimit(RLIMIT_NOFILE, &rl) != 0) {
      e = errno;
      fprintf(stderr, "Failed to raise hard limit for NOFILE to %d: %s.\n", 
	     (int) rl.rlim_max, strerror(e));
      return 1;
    }
    if (cap_set_proc(privilege_off)) {
      fprintf(stderr, "Failed to turn off privileges.\n");
      return 1;
    }
    if (verbose) {
      printf("Raised hard limit for NOFILE to %d.\n", (int) rl.rlim_max);
    }
  }

  // Adjust soft limit if necessary

  if (rl.rlim_cur < nofiles) {
    rl.rlim_cur = nofiles;
    if (setrlimit(RLIMIT_NOFILE, &rl) != 0) {
      e = errno;
      fprintf(stderr, "Failed to raise soft limit for NOFILE to %d: %s.\n", 
	     (int) rl.rlim_cur, strerror(e));
      return 1;
    }
    if (verbose) {
      printf("Raised soft limit for NOFILE to %d.\n", (int) rl.rlim_cur);
    }
  }
  
  if (cap_set_proc(privilege_dropped)) {
    fprintf(stderr, "Failed to turn irrevocably drop privileges.\n");
    return 1;
  }

  return 0;
}

int bind_listener(char *port, int backlog, int *returned_listener) {
  int listener; // listening socket descriptor
  int rv, yes=1, e;
  struct addrinfo ai_hints, *ai_result, *p;
  
  memset(&ai_hints, 0, sizeof(ai_hints));
  ai_hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
  ai_hints.ai_socktype = SOCK_STREAM;
  ai_hints.ai_flags = AI_PASSIVE;
  if ((rv = getaddrinfo(NULL, port, &ai_hints, &ai_result)) != 0) {
    fprintf(stderr, "Getaddrinfo failed: %s.\n", gai_strerror(rv));
    return 1;
  }
    
  for (p = ai_result; p != NULL; p = p->ai_next) {
    listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (listener < 0) {
      if (verbose) {
	e = errno;
	fprintf(stderr, "Socket failed: %s, trying nex\n", strerror(e));
      }
      continue;
    }
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) != 0) {
      if (verbose) {
	e = errno;
	fprintf(stderr, "Setsockopt (SO_REUSEADDR) failed: %s, trying next\n", strerror(e));
      }
      continue;
    }
    if (setsockopt(listener, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(int)) != 0) {
      if (verbose) {
	e = errno;
	fprintf(stderr, "Setsockopt (TCP_NODELAY) failed: %s, trying next\n", strerror(e));
      }
      continue;
    }
    if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
      e = errno;
      close(listener);
      if (verbose) {
	fprintf(stderr, "Bind failed: %s, trying next\n", strerror(errno));
      }
      continue;
    }
    break;
  }

  if (p == NULL) {
    fprintf(stderr, "Failed to bind to listener on port %s.\n", port);
    return 1;
  } else if (verbose) {
    fprintf(stderr, "Listener bound on port %s.\n", port);
  }

  if (listen(listener, backlog) != 0) {
    e = errno;
    fprintf(stderr, "Listen failed: %s.\n", strerror(e));
  }

  freeaddrinfo(ai_result);
  *returned_listener = listener;
  return 0;
}

void accept_connection(int listener_index, struct pollfd *fds, int *p_num_fds) {
  int e;
  struct sockaddr_storage client_addr;
  int client_fd;
  socklen_t  addr_len;

  addr_len = sizeof(client_addr);
  client_fd = accept(fds[listener_index].fd, (struct sockaddr *) &client_addr, &addr_len);
  if (client_fd == -1) {
    e = errno;
    fprintf(stderr, "Error from accept: %s.\n", strerror(e));
  } else {
    if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1) {
      e = errno;
      fprintf(stderr, "Error setting nonblocking: %s.\n", strerror(e));
    }
    if (*p_num_fds >= MAX_CONNECTIONS) {
      close(client_fd);
      if (verbose) {
	fprintf(stderr, "Open connections more than max of %d, closing connection request.\n",
		MAX_CONNECTIONS);
      }
      return;
    }
    fds[*p_num_fds].fd = client_fd;
    fds[*p_num_fds].events = POLLIN | POLLOUT;
    *p_num_fds += 1;
    if (verbose) {
      fprintf(stderr, "New connection on socket %d.\n", client_fd);
    }
  }
}

int read_client_data(int index, char *buf, size_t len, struct pollfd *fds, int *p_num_fds) {
  int rbytes, e;
  rbytes = recv(fds[index].fd, buf, len, 0);
  e = errno;
  if ((rbytes == 0) || 
      (rbytes == -1 && (e != EAGAIN && e != EWOULDBLOCK))) {
    if (rbytes == -1) {
      fprintf(stderr, "Error on recv: %s, closing connection %d.\n",
	      strerror(e), fds[index].fd);
    }
    if (verbose) {
      fprintf(stderr, "Closing connection on socket %d.\n", fds[index].fd);
    }
    close(fds[index].fd);
    *p_num_fds -= 1;
    if (*p_num_fds > 0) {
      fds[index] = fds[*p_num_fds];
    }
  } else if ((rbytes > 0) && verbose) {
    fprintf(stderr, "Read %d bytes from socket %d.\n", rbytes, fds[index].fd);
  }
  if (rbytes < 0) return 0;
  return rbytes;
}

int simulate_gateway(char *listen_port) {
  int listener_fd, listener_index, rv, num_fds = 0, e, i, seq = 0;
  char rbuf[1024], wbuf[PAYLOAD_LEN];
  struct timespec now, wake, delay, timeout;
  struct pollfd fds[MAX_CONNECTIONS];

  for (i = 0; i < PAYLOAD_LEN; i++) { 
    wbuf[i] = '.'; 
  }

  if ((rv = bind_listener(listen_port, LISTEN_BACKLOG, &listener_fd)) != 0) {
    return rv;
  }

  listener_index = num_fds;
  fds[num_fds].fd = listener_fd;
  fds[num_fds].events = POLLIN;
  num_fds++;

  delay.tv_sec = DELAY_SEC;
  delay.tv_nsec = DELAY_NSEC;

  while (1) {

    clock_gettime(CLOCK_MONOTONIC_RAW, &now);
    wake = add(now, delay);
    timeout = diff(now, wake);

    // while we are waiting for the wakeup time, accept connections and
    // read data sent from connected clients

    for (i = 0; i < num_fds; i++) {
      fds[i].events = POLLIN;
    }

    while (timeout.tv_sec >= 0) {
      if (ppoll(fds, num_fds, &timeout, NULL) == -1) {
	e = errno;
	fprintf(stderr, "Error in read poll: %s.\n", strerror(e));
	return 1;
      }
      for (i = 0; i < num_fds; i++) {
	if (fds[i].revents & POLLIN) {
	  if (i == listener_index) {
	    accept_connection(listener_index, fds, &num_fds);
	  } else {
	    read_client_data(i, rbuf, sizeof(rbuf), fds, &num_fds);
	  }
	}
      }
      clock_gettime(CLOCK_MONOTONIC_RAW, &now);
      timeout = diff(now, wake);
    } 

    // once the wakeup time has passed, send data to every client
    // ready to write

    for (i = 0; i < num_fds; i++) {
      if (i == listener_index) {
	fds[i].events = 0;
      } else {
	fds[i].events = POLLOUT;
      }
    }

    timeout.tv_sec = 0;
    timeout.tv_nsec = 0;
    if ((rv = ppoll(fds, num_fds, &timeout, NULL)) == -1) {
      e = errno;
      fprintf(stderr, "Error in write poll: %s.\n", strerror(e));
      return 1;
    }

    if (verbose) {
      fprintf(stderr, "There are %d sockets ready to write.\n", rv);
    }

    snprintf(wbuf, sizeof(wbuf), "%05d", seq++);

    for (i = 0; i < num_fds; i++) {
      if (fds[i].revents & POLLOUT) {
	if (send(fds[i].fd, wbuf, sizeof(wbuf), MSG_NOSIGNAL) == -1) {
	  e = errno;
	  if ((e != EWOULDBLOCK) && (e != EAGAIN)) {
	    fprintf(stderr, "Error writing to socket %d: %s, closing\n", fds[i].fd, strerror(e));
	    close(fds[i].fd);
	    num_fds--;
	    if (num_fds > 0) {
	      fds[i] = fds[num_fds];
	      i--;
	    }
	  }
	}
      }
    }

  } // while (1)

}

int main(int argc, char **argv) {
  char *listen_port;

  if (argc != 2) {
    fprintf(stderr,"Usage: %s <port>\n", argv[0]);
    return 1;
  }
  listen_port = argv[1];

  if (set_policy_priority(SCHED_FIFO, PRIORITY, NOFILES)) {
    fprintf(stderr, "Failed to set scheduling policy, priority, or nofile.\n");
    return 1;
  }

  return simulate_gateway(listen_port);
}
