// BoostTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <string>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/asio.hpp>


//#include <set>
//#include <objbase.h>

/*
#include <sstream>
#include "busmap.h"
*/

#include "BlockingQueue.h"
using namespace std;


/*
boost::mutex io_mutex;

struct count {
    count(int id) : id(id) { }
    void operator()() {
        for (int i = 0; i < 50; i++) {
            //boost::mutex::scoped_lock lock(io_mutex);
            std::cout << id << ": " << i << std::endl;
            boost::this_thread::sleep(boost::posix_time::milliseconds(3000));
        }
    }
    int id;
};
*/

/*
void print(const boost::system::error_code& err)
{
    if (err)
    {
        std::cout << "timer is canceled\n";
        return;
    }
    std::cout << "Hello, world!\n";
}

void waitStop(int b)
{
    boost::asio::io_service io;
    boost::asio::deadline_timer timer(io, boost::posix_time::millisec(b));
    timer.async_wait(&print);
    io.run();

    std::cout << "d=" << b << std::endl;
}

*/

/*
busmap::map<std::string, std::string> g_testMap;

void t1() {
    for (int i = 0; i < 100; i++) {
        stringstream ss;
        stringstream ss2;
        ss << i;
        ss2 << i;
        string k;
        string v;
        ss >> k;
        ss2 >> v;
        k += "_key";
        v += "_value";
        g_testMap.insert_s(k, v);
        Sleep(2000);
    }
}

void t2() {
    for (int i = 100; i < 201; i++) {
        stringstream ss;
        stringstream ss2;
        ss << i;
        ss2 << i;
        string k;
        string v;
        ss >> k;
        ss2 >> v;
        k += "_key";
        v += "_value";
        g_testMap.insert_s(k, v);
        Sleep(2000);
    }
}

void t3() {
    int i = 0;
    while (true) {
        stringstream ss;
        ss << i;
        string key;
        ss >> key;
        key += "_key";
        busmap::map<std::string, std::string>::iterator iter = g_testMap.find_s(key);
        if (iter != g_testMap.end_s())
        {
            cout << "I'm t3. I find: Key=[" << iter->first << "], Value=[" << iter->second << "]." << endl;
            i++;
        }
        Sleep(500);
    }
}

void t4() {
    int i = 100;
    while (true) {
        stringstream ss;
        ss << i;
        string key;
        ss >> key;
        key += "_key";
        busmap::map<std::string, std::string>::iterator iter = g_testMap.find_s(key);
        if (iter != g_testMap.end_s())
        {
            cout << "I'm t4. I find: Key=[" << iter->first << "], Value=[" << iter->second << "]." << endl;
            i++;
        }
        Sleep(500);
    }
}
*/


/*
std::string GetUUID()
{
    string strUUID;
    GUID guid;
    if (!CoCreateGuid(&guid))
    {
        char buffer[64] = { 0 };
        _snprintf_s(buffer, sizeof(buffer),
            "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",    //大写
            //"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",  //小写
            guid.Data1, guid.Data2, guid.Data3,
            guid.Data4[0], guid.Data4[1], guid.Data4[2],
            guid.Data4[3], guid.Data4[4], guid.Data4[5],
            guid.Data4[6], guid.Data4[7]);
        strUUID = buffer;
    }
    return strUUID;
}
*/


boost::mutex g_mutex;
bool t1Stop = false;
bool t2Stop = false;
BlockingQueue<string>   g_bq;


void print(string id) {
    boost::mutex::scoped_lock lock(g_mutex);
    cout << "hello, I'm thread" << id << endl;
}

void t1() {
    //try
    //{
        while (true) {
            string s = "t1";
            g_bq.put(s);
            boost::this_thread::sleep(boost::posix_time::seconds(2));//休眠5s
            if (t1Stop) {
                break;
            }
        }
    //}
    //catch (boost::thread_interrupted&)//捕获线程中断异常
    //{
    //    boost::mutex::scoped_lock lock(g_mutex);
    //    std::cout << "thread1 interrupted!" << std::endl;
    //}
}

void t2() {
    //try
    //{
    while (true) {
        string s = g_bq.take();
        print(s);
        boost::mutex::scoped_lock lock(g_mutex);
        cout << "I'm thread2, i take " << s << endl;
        //boost::this_thread::sleep(boost::posix_time::seconds(1));//休眠1s
        if (t2Stop) {
            break;
        }
    }
    //}
    //catch (boost::thread_interrupted&)//捕获线程中断异常
    //{
    //    boost::mutex::scoped_lock lock(g_mutex);
    //   std::cout << "thread2 interrupted!" << std::endl;
    //}
}

void t3() {
    //try
    //{
    while (true) {
        string s = "t3";
        g_bq.put(s);
        boost::this_thread::sleep(boost::posix_time::seconds(3));//休眠5s
        if (t1Stop) {
            break;
        }
    }
    //}
    //catch (boost::thread_interrupted&)//捕获线程中断异常
    //{
    //    boost::mutex::scoped_lock lock(g_mutex);
    //    std::cout << "thread1 interrupted!" << std::endl;
    //}
}

void t4() {
    //try
    //{
    while (true) {
        string s = g_bq.take();
        print(s);
        boost::mutex::scoped_lock lock(g_mutex);
        cout << "I'm thread4, i take " << s << endl;
        //boost::this_thread::sleep(boost::posix_time::seconds(1));//休眠1s
        if (t2Stop) {
            break;
        }
    }
    //}
    //catch (boost::thread_interrupted&)//捕获线程中断异常
    //{
    //    boost::mutex::scoped_lock lock(g_mutex);
    //   std::cout << "thread2 interrupted!" << std::endl;
    //}
}

void t5() {
    //try
    //{
    while (true) {
        string s = g_bq.take();
        print(s);
        boost::mutex::scoped_lock lock(g_mutex);
        cout << "I'm thread5, i take " << s << endl;
        //boost::this_thread::sleep(boost::posix_time::seconds(1));//休眠1s
        if (t2Stop) {
            break;
        }
    }
    //}
    //catch (boost::thread_interrupted&)//捕获线程中断异常
    //{
    //    boost::mutex::scoped_lock lock(g_mutex);
    //    std::cout << "thread2 interrupted!" << std::endl;
    //}
}

class A
{
public:
    A();
    ~A();
    boost::thread *CreateThread(void (A::*fn)(int), int id);
    void task1(int id);
    void run(int id);
    void stop();
    void join();

    vector<boost::thread *> m_p1;
    bool bStop;
};

void A::join()
{
    if (!m_p1.empty()) {
        vector<boost::thread *>::iterator iter = m_p1.begin();
        for (; iter != m_p1.end(); iter++) {
            (*iter)->join();
        }
    }
}

A::A()
{
    bStop = false;
}

A::~A()
{
    if (!m_p1.empty()) {
        vector<boost::thread *>::iterator iter = m_p1.begin();
        for (; iter != m_p1.end(); iter++) {
            delete (*iter);
            (*iter) = nullptr;
        }
        m_p1.clear();
    }
}

void A::run(int id)
{
    m_p1.push_back(CreateThread(&A::task1, id));
}

void A::stop()
{
    bStop = true;
}

boost::thread *A::CreateThread(void (A::*fn)(int), int id)
{
    boost::thread *p = new boost::thread(boost::bind(fn, this, id));
    return p;
}

void A::task1(int id)
{
    while (true) {
        if (bStop) {
            cout << "I'm A::task finish. id=" << id << endl;
            return;
        }
        cout << "I'm A::task1. id=" << id << endl;
        boost::this_thread::sleep(boost::posix_time::seconds(1));
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    /*
    boost::thread thrd1(&t1);
    boost::thread thrd2(&t2);
    boost::thread thrd3(&t3);
    boost::thread thrd4(&t4);
    thrd1.join();
    thrd2.join();
    thrd3.join();
    thrd4.join();
    */
    /*
    string tmp_uuid = GetUUID();
    cout << "uuid=[" << tmp_uuid << "]." << endl;
    */

    /*
    boost::thread t1(&t1);
    boost::thread t2(&t2);
    boost::thread t3(&t3);
    boost::thread t4(&t4);
    boost::thread t5(&t5);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    */

    /*
    A a;
    a.run(1);
    a.run(2);
    a.run(3);
    a.run(4);
    a.run(5);

    while (1) {
        Sleep(1000 * 5);
        a.stop();
        break;
    }
    a.join();
    */

    string inTpoic = "acodf22dodjwq";
    string sp = ".";
    std::string sRet;
    int pos = 0;
    if (std::string::npos != (pos = inTpoic.find_first_of(sp, 0))) {
        sRet = inTpoic.substr(0, pos);
        cout << sRet << endl;
    }

    system("pause");
	return 0;
}

