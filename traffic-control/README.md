This project is used for regulating outbound QB gateway traffic.

Two scripts (htb_on and htb_off) are used to configure and remove a heirarchical token bucket filter to limit QB gateway traffic in the production environment.  Please see the scripts themselves for more information about how to configure and use these scripts.

If you need to "back out" the configuration change, it is safe to run htb_off on a live production system under load.  While making changes to a live production machine is not generally desirable, it is also safe to change configuration parameters and alter an htb configuration by re-running htb_on or to configure a fresh htb on an interface with a default qdisc on live machines under load.

For a general overview of queueing disciplines, please see [this](http://lartc.org/howto/lartc.qdisc.html).  You can find a Chinese translation of this as Chapter 9 of [this](http://lartc.org/LARTC-zh_CN.GB2312.pdf).  Note that these documents are somewhat old.  A good place to look for news about linux kernel updates is lwn.net.  For example a google search for "site:lwn.net qdisc" will turn up a lot of qdiscs not documented in the lartc.org writeups.

Please note that the configurations of qdiscs and filters on an interface is not persistent.  It will be necessary to reconfigure them after reboot, e.g. via something like /etc/rc.local.

The programs gwsim.c and clients are used to test/validate the configuration scripts.

* gwsim.c simulates a QB gateway.  It listens on a port specified on the command line, accepts connections, and writes 1400 bytes to each socket connection every second.  In order to facilitate doing this as fast as possible, it runs under the real time scheduler.  It also might need to accept a lot of connections.  So it eather needs to run as root, be granted the capability CAP_SYS_RESOURCE, or limits.conf must specify a high enough limit for the real time scheduler priority and number of socket connections.  You can build it with make and the supplied Makefile.  The makefile statically links so that the binary built on Ubuntu will run on Centos.

* clients simulates a large number of QB clients, taking as a command line argument the number of clients to simulate.  It simply fires up the requested number of netcat (nc) instances that connect to the simulated server and read data from the connection into /dev/null.
