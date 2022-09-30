#include <cstdio>
//#include <sys/stat.h>
#include <iostream>
#include "thread_pool.h"

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

std::mutex      g_mutex_lock_;

void fun1(int slp) {
	printf(" hello, fun1 ! %d\n", std::this_thread::get_id());
	if (slp > 0) {
		printf(" ======= fun1 sleep %d ========= %d\n", slp, std::this_thread::get_id());
		std::this_thread::sleep_for(std::chrono::milliseconds(slp));
	}
}

struct gfun {
	int operator()(int n) {
		printf("%d hello, gfun ! %d\n", n, std::this_thread::get_id());
		return 42;
	}
};

void test_msg_order(int number) {
	std::lock_guard<std::mutex> lock(g_mutex_lock_);
	std::cout << "thread id : " << std::this_thread::get_id()
		<< "msg number : " << number << std::endl;
}

class A {
public:
	A() : m_int(119) {}

public:
	static int Afun(int n = 0) {  //函数必须是 static 的才能直接使用线程池
		std::cout << n << " hello, Afun ! " << std::this_thread::get_id() << std::endl;
		return n;
	}

	static std::string Bfun(int n, std::string str, char c) {
		std::cout << n << " hello, Bfun ! " << str.c_str() << " " << (int)c << " " << std::this_thread::get_id() << std::endl;
		//std::cout << m_int << std::endl;
		return str;
	}

private:
	int		m_int;
};

int main()
{
	/*
	struct stat stat_buf;
	int result;
	//获得文件状态信息
	std::string path = "./setting.ini";
	result = stat(path.c_str(), &stat_buf);

	//显示文件状态信息
	if (result != 0)
		std::cout << "error!" << std::endl;
	else
	{
		printf("file size: %d\n", stat_buf.st_size);
		printf("create time: %s", ctime(&stat_buf.st_ctime));
		printf("access time: %s", ctime(&stat_buf.st_atime));
		printf("last update time: %s", ctime(&stat_buf.st_mtime));
		long upd_time = stat_buf.st_mtime;
		std::cout << upd_time << std::endl;
	}
	*/

	/*
	std::threadpool *pool_ = new std::threadpool(4); //use 10 threads in the pool
	int i = 0;
	for (; i < 100; i++) {
		std::future<void> ret = pool_->commit(test_msg_order, i);
		//ret.get();
	}
	test_msg_order(i);
	*/
	/*
	std::future<std::string> gh = pool_->commit(A::Bfun, 9998, "mult args", 123);
	gh.get();
	*/
	/*
	if (pool_) {
		delete pool_;
		pool_ = nullptr;
	}
	*/

	int nPid = (int)getpid();
	std::cout << "The process id is: " << nPid << std::endl;

	std::cout << "enter to quit." << std::endl;
	std::cin.get();
    return 0;
}