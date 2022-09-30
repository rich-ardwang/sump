/***************************************************************************
*                                  _   _ ____  _
*  Project                     ___| | | |  _ \| |
*                             / __| | | | |_) | |
*                            | (__| |_| |  _ <| |___
*                             \___|\___/|_| \_\_____|
*
* Copyright (C) 1998 - 2015, Daniel Stenberg, <daniel@haxx.se>, et al.
*
* This software is licensed as described in the file COPYING, which
* you should have received as part of this distribution. The terms
* are also available at https://curl.haxx.se/docs/copyright.html.
*
* You may opt to use, copy, modify, merge, publish, distribute and/or sell
* copies of the Software, and permit persons to whom the Software is
* furnished to do so, under the terms of the COPYING file.
*
* This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
* KIND, either express or implied.
*
***************************************************************************/
/* <DESC>
* simple HTTP POST using the easy interface
* </DESC>
*/
#include <stdio.h>
#include <string>
#include <iostream>
#include <curl/curl.h>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "cJSON.h"
#include <sstream>

using namespace std;

string double2string(double src, int bit) {
    stringstream ss;
    ss << setprecision(bit) << fixed << src;
    string ret;
    ss >> ret;
    ss.clear();
    return ret;
}

string int2string(int src) {
    stringstream ss;
    ss << src;
    string ret;
    ss >> ret;
    ss.clear();
    return ret;
}

double year2month(double src) {
    return src * 12;
}

double month2year(const double src) {
    return src / 12.0;
}

string cut_double(const string &src, const int bit) {
    string ret = src;
    int pos = ret.find(".");
    if (-1 == pos) {
        return ret;
    }
    string s_int = ret.substr(0, pos+1);
    string s_dig = ret.substr(pos+1);
    int len = s_dig.length();
    if (len > bit) {
        s_dig = s_dig.substr(0, bit);
        ret = s_int + s_dig;
    }
    return ret;
}

size_t post_resp_data_writer(void* data, size_t size, size_t nmemb, void* content)
{
    long totalSize = size*nmemb;
    std::string* symbolBuffer = (std::string*)content;
    if (symbolBuffer)
    {
        symbolBuffer->append((char *)data, ((char*)data) + totalSize);
    }
    return totalSize;
}

struct CDfParams {
    string security_code;
    int pricing_status;
    string default_rate;
    string prepay_rate;
    double recovery_rate;
    double recovery_lag;
};

bool parseJsonDefaultParams(const string &content, CDfParams &params) {
    cJSON *root = cJSON_Parse(content.c_str());
    if (!root) {
        std::cout << "parse json root failed!" << endl;
        return false;
    }

    cJSON *data = cJSON_GetObjectItem(root, "data");
    if (!data) {
        std::cout << "parse json data failed!" << endl;
        cJSON_Delete(root);
        return false;
    }

    cJSON *item = cJSON_GetObjectItem(data, "security_code");
    if (!item) {
        std::cout << "parse json security_code failed!" << endl;
        cJSON_Delete(root);
        return false;
    }
    params.security_code = item->valuestring;

    item = cJSON_GetObjectItem(data, "pricing_status");
    if (!item) {
        std::cout << "parse json pricing_status failed!" << endl;
        cJSON_Delete(root);
        return false;
    }
    params.pricing_status = item->valueint;

    item = cJSON_GetObjectItem(data, "default_rate");
    if (!item) {
        std::cout << "parse json default_rate failed!" << endl;
        cJSON_Delete(root);
        return false;
    }
    params.default_rate = item->valuestring;

    item = cJSON_GetObjectItem(data, "prepay_rate");
    if (!item) {
        std::cout << "parse json prepay_rate failed!" << endl;
        cJSON_Delete(root);
        return false;
    }
    params.prepay_rate = item->valuestring;

    item = cJSON_GetObjectItem(data, "recovery_rate");
    if (!item) {
        std::cout << "parse json recovery_rate failed!" << endl;
        cJSON_Delete(root);
        return false;
    }
    params.recovery_rate = item->valuedouble;

    item = cJSON_GetObjectItem(data, "recovery_lag");
    if (!item) {
        std::cout << "parse json recovery_lag failed!" << endl;
        cJSON_Delete(root);
        return false;
    }
    params.recovery_lag = item->valuedouble;

    cJSON_Delete(root);
    return true;
}

string getParams() {
    CURL *curl;
    CURLcode res;
    string ret;

    /* get a curl handle */
    curl = curl_easy_init();
    if (curl) {
        /* First set the URL that is about to receive our POST. This URL can
        just as well be a https:// URL if that is what should receive the
        data. */
        string s_url;
        s_url = "https://www.cn-abs.com/apigateway/service/security-pricing/default-params";
        curl_easy_setopt(curl, CURLOPT_URL, s_url.c_str());

        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 2000);   //接收数据时超时设置，如果超时未接收完，直接退出

        struct curl_slist *headers = NULL; /* init to NULL is important */
        headers = curl_slist_append(headers, "Authorization:Basic UUI6UUJAYWIx");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        /* Now specify the POST data */
        string bodyList; /* init to NULL is important */
        bodyList += "security_code=1889210.IB";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, bodyList.c_str());

        // 设置回调函数
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, post_resp_data_writer);

        std::string strData;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&strData);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
        }
        else {
            long responseCode = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
            if (responseCode < 200 || responseCode >= 300 || strData.empty())
            {
                cout << "Error! responseCode:[" << responseCode << "]!" << endl;
            }
            else {
                //下面可以对应答的数据进行处理了
                cout << "responseCode:[" << responseCode << "]." << endl;
                ret = strData;
                CDfParams pa;
                bool ret = parseJsonDefaultParams(strData, pa);
                if (ret) {
                    cout << "security_code=" << pa.security_code << endl;
                    cout << "pricing_status_=" << pa.pricing_status << endl;
                    cout << "prepay_rate_=" << pa.prepay_rate << endl;
                    cout << "default_rate_=" << pa.default_rate << endl;
                    cout << "recovery_rate_=" << pa.recovery_rate << endl;
                    cout << "recovery_lag_=" << pa.recovery_lag << endl;
                }
            }
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    return ret;
}

string getCalcRet() {
    CURL *curl;
    CURLcode res;
    string ret;

    /* get a curl handle */
    curl = curl_easy_init();
    if (curl) {
        /* First set the URL that is about to receive our POST. This URL can
        just as well be a https:// URL if that is what should receive the
        data. */
        string s_url;
        s_url = "https://www.cn-abs.com/apigateway/service/security-pricing/calc-result";
        curl_easy_setopt(curl, CURLOPT_URL, s_url.c_str());

        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 3000);    //接收数据时超时设置，如果超时未接收完，直接退出

        struct curl_slist *headers = NULL; /* init to NULL is important */
        headers = curl_slist_append(headers, "Authorization:Basic UUI6UUJAYWIx");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        /* Now specify the POST data */
        string bodyList; /* init to NULL is important */
        bodyList += "security_code=1889210.IB";
        bodyList += "&";
        bodyList += "settlement_date=2019-04-19";
        bodyList += "&";
        bodyList += "param_type=0";
        bodyList += "&";
        bodyList += "param_value=3.8";
        bodyList += "&";
        bodyList += "default_rate=0.03";
        bodyList += "&";
        bodyList += "prepay_rate=12.78";
        bodyList += "&";
        bodyList += "recovery_lag=0.5";
        bodyList += "&";
        bodyList += "recovery_rate=20";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, bodyList.c_str());

        // 设置回调函数
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, post_resp_data_writer);

        std::string strData;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&strData);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        }
        else {
            long responseCode = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
            if (responseCode < 200 || responseCode >= 300 || strData.empty())
            {
                cout << "Error! responseCode:[" << responseCode << "]!" << endl;
            }
            else {
                //下面可以对应答的数据进行处理了
                cout << "responseCode:[" << responseCode << "]." << endl;
                ret = strData;
            }
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    return ret;
}

boost::mutex io_mutex;
void printParams(string id, int cnt) {
    //boost::mutex::scoped_lock lock(io_mutex);
    cout << "cnt:" << cnt << ", " << id << ":" << getParams() << endl;
}

void printCalcRet(string id, int cnt) {
    //boost::mutex::scoped_lock lock(io_mutex);
    cout << "cnt:" << cnt << ", " << id << ":" << getCalcRet() << endl;
}

void t1() {
    printParams("t1", 1);
    /*
    while (true) {
        for (int i = 0; i < 5; i++) {
            printParams("t1", i);
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
    }
    */
}

void t2() {
    printCalcRet("t2", 1);
    /*
    while (true) {
        for (int i = 0; i < 5; i++) {
            printCalcRet("t2", i);
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
    }
    */
}

void t3() {
    while (true) {
        /*
        for (int i = 0; i < 5; i++) {
            printParams("t3", i);
        }
        */
        boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
    }
}

void t4() {
    while (true) {
        /*
        for (int i = 0; i < 5; i++) {
            printCalcRet("t4", i);
        }
        */
        boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
    }
}

double RoundTo(const double N, const int Digit) {
    if ((Digit < -1) || (Digit > 1))
        return N;
    double Factor = pow(10.0, Digit);
    if (N < 0)
        return floor(N / Factor - 0.5) * Factor;
    else
        return floor(N / Factor + 0.5) * Factor;
}

int main(void)
{
    double df = 15960.29481953256500049;
    string sf;
    sf = double2string(df, 4);
    string sub = cut_double(sf, 6);

    double month = 35.5;
    double year = month2year(month);

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    boost::thread thrd1(&t1);
    boost::thread thrd2(&t2);
    boost::thread thrd3(&t3);
    boost::thread thrd4(&t4);
    thrd1.join();
    thrd2.join();
    thrd3.join();
    thrd4.join();


    curl_global_cleanup();


    system("pause");
    return 0;
}
