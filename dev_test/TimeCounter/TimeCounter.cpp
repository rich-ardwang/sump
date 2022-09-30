// TimeCounter.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "time_counter.h"
#include <iostream>
#include <fstream>
#include <string>
#include <thread>


using namespace std;
#define FILE_CPY_CNT    25000
#define THREDA_CNT      50
#define AVG     (FILE_CPY_CNT / THREDA_CNT)



void copy(char* src, char* dst) {
    using namespace std;
    ifstream in(src, ios::binary);
    ofstream out(dst, ios::binary);
    if (!in.is_open()) {
        cout << "error open file " << src << endl;
        exit(EXIT_FAILURE);
    }
    if (!out.is_open()) {
        cout << "error open file " << dst << endl;
        exit(EXIT_FAILURE);
    }
    if (src == dst) {
        cout << "the src file can't be same with dst file" << endl;
        exit(EXIT_FAILURE);
    }
    char buf[2048];
    long long totalBytes = 0;
    while (in) {
        /*
            read��in���ж�ȡ2048�ֽڣ�����buf�����У�ͬʱ�ļ�ָ������ƶ�2048�ֽ�,
            ������2048�ֽ������ļ���β������ʵ����ȡ�ֽڶ�ȡ��
        */
        in.read(buf, 2048);
        /* gcount()������ȡ��ȡ���ֽ�����write��buf�е�����д��out���� */
        out.write(buf, in.gcount());
        totalBytes += in.gcount();
    }
    in.close();
    out.close();
}

void mutiCpy(int threadid)
{
    char src[50] = "D:\\copyTest\\cn_abs_calc_srv_a.zip";
    char dst[50] = "D:\\copyTest\\cn_abs_calc_srv_a.zip_";

    for (int i = 0; i < AVG; ++i)
    {
        char td[5];
        _itoa(threadid, td, 10);
        char id[5];
        _itoa(i, id, 10);
        char dt[100];
        strcpy(dt, dst);
        strcat(dt, td);
        strcat(dt, "_");
        strcat(dt, id);
        copy(src, dt);
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    stop_watch watch;
    watch.start();

    //To-do
    thread workerThreads[THREDA_CNT];
    for (int i = 0; i < THREDA_CNT; ++i) {
        workerThreads[i] = thread(mutiCpy, i);
    }
    for (auto& workerThread : workerThreads) {
        workerThread.join();
    }

    watch.stop();
    cout << "Timespan: [" << watch.elapsed_ms() << " ms]." << endl;

    system("pause");
	return 0;
}

