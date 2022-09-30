#include <stdio.h>
#include <string>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/timer.hpp>
#include "cJSON.h"
#include <sstream>
#include "wsa.h"

using namespace std;
#define THREAD_CNT  5       //5�߳�Ч�����
#define CNT     (100/THREAD_CNT)


struct CBondCalc {
    string      deal_id;
    string      settlement_date;
    double      prepay_rate;
    double      default_rate;
    double      recoveries_rate;
    int         recovery_lag;
    int         anchor_type;
    double      anchor_value;
};

bool query_bond_calc(const CBondCalc &bondParam, PRICING_RESULTS &calc_ret) {
    /*Set the path where deals located, make sure there is "WSA_API.DB" database in this folder as well. */
    const char input_path[] = "D:\\moodySFW\\MoodysData";
    set_input_path(input_path);
    //cout << "�Ѿ�����Ŀ¼��D:\\moodySFW\\MoodysData����Ŀ¼����GONGYUANZHIYUAN181.SFW��DB���ļ���" << endl;
    //cout << "����������GONGYUANZHIYUAN181���dealΪ������Ԫ��Զ181������ѯbondidΪ1889210��ȯ��" << endl;

    //cout << endl;
    void* tid(NULL);
    //cout << "����ʼִ�� create_deal_scenario_object������tid����" << endl;
    int tid_ret = create_deal_scenario_object(&tid, NULL, NULL, false);
    if (0 == tid_ret) {
        //cout << "��ִ�н����tid_ret=" << tid_ret << " ,����tid�ɹ���" << endl;
    }
    else {
        return false;
    }

    //cout << endl;
    CMO_STRUCT *pCmo = new CMO_STRUCT();
    //char deal_id[] = "GONGYUANZHIYUAN181";    // ���룺 SFW ID��Ҳ���� *.SFW �е� *�� ����HUAYU176 
    //int len = sizeof(pCmo->dealid) / sizeof(pCmo->dealid[0]);
    strncpy(pCmo->dealid, bondParam.deal_id.c_str(), bondParam.deal_id.length()); // ��id ��ֵ��CMO_STRUCT�����deal_id.API��ȥ��ȡ����ֶΣ���ɶ� *.SFW �ļ��ļ��ء�
    //cout << "����pCmo->dealid=" << pCmo->dealid << endl;
    strncpy(pCmo->settlement_date, bondParam.settlement_date.c_str(), bondParam.settlement_date.length()); // ���룺 settlement date��������
    //cout << "����settlement_date=" << pCmo->settlement_date << endl;
    //cout << "����ʼִ�� open_deal_ex��" << endl;
    long openFlag = open_deal_ex(tid, pCmo); // ִ�д�deal �ļ������0 ��ʾ�ɹ������һ��Զ����CMO_STRUCT �����ֶΡ�
    if (0 == openFlag) {
        //cout << "��ִ�н����openFlag=" << openFlag << " ,ִ��open_deal_ex�ɹ���" << endl;
    }
    else {
        return false;
    }

    //��������� get_china_bond_info_by_tranche() , ���ṹ�� �����Ի�õ�ǰ��deal��ĳ��ծȯ��ծȯ����

    //cout << endl;
    MARKIT_DEAL_INFO deal_info;
    //cout << "����ʼִ�� get_deal_info��" << endl;
    int ret = get_deal_info(tid, NULL, &deal_info);
    if (ret < 0) {
        //printf("%s", get_deal_error_msg(tid));
        return false;
    }
    else {
        //cout << "��ִ�н����ȡ��deal_info�ɹ�����Ϣ���£�" << endl;
        //printf("deal %s opened , it has %d bonds and %d collaterals \n", deal_info.deal_name, deal_info.num_bonds, deal_info.num_colls);
    }

    //��� �������ʣ�౾��
    //ͨ������ get_bond_info_by_index_ex() �����index��Χ����0~x, x = deal_info.num_bonds -1
    //��Ҫ��ȡȫ�� bond �Ļ�����Ϣ(MARKIT_BOND_INFO) ����ṹ���������� balance��Ϣ
    MOODYS_BOND_INFO result;
    for (int i = 1; i <= deal_info.num_bonds; i++) {
        int bond_info_ret = get_bond_info_by_index_ex(tid, NULL, i, &result);
        cout << "bond_info_ret=" << bond_info_ret << endl;
        if (0 != bond_info_ret) {
            printf("%s", get_deal_error_msg(tid));
        }

        
    }
    MARKIT_BOND_INFO bondInfo;
    get_bond_info_by_tranche(tid, NULL, "A1", &bondInfo);
    std::cout << bondInfo.floater_index << std::endl;
    INDEX_TYPE_EX;

    double rate = 0.0475;
    int index_rate_ret = set_index_rate(tid, "CNY", &bondInfo.floater_index, 0, &rate);
    if (0 != index_rate_ret) {
        // error handle
    }


    /* Input: asset pool assumption */
    //cout << endl;
    //cout << "���濪ʼ���ø���������" << endl;
    //set prepayments
    double dCPR = bondParam.prepay_rate;
    if (set_prepayments_ex(tid, PREPAY_CURVE_CPR, 0, &dCPR, -1, false)) {
        std::cout << "Set set_prepayments_ex fail! Deal ID:" << pCmo->dealid << " Message:" << get_deal_error_msg(tid) << std::endl;
        return false;
    }
    //cout << "���� prepayments=12.78% �ɹ���" << endl;

    //Set defaults
    double dCDR = bondParam.default_rate;
    if (set_defaults_ex(tid, DEFAULT_CURVE_CDR, 0, &dCDR, -1, false)) {
        std::cout << "Set set_defaults_ex fail! Deal ID:" << pCmo->dealid << " Message:" << get_deal_error_msg(tid) << std::endl;
        return false;
    }
    //cout << "���� defaults_rate=0.03% �ɹ���" << endl;

    //Set recoveries
    double dRcvs = bondParam.recoveries_rate;
    if (set_recoveries_ex(tid, 0, &dRcvs, -1, false) < 0) {
        std::cout << "Set set_recoveries_ex fail! Deal ID:" << pCmo->dealid << " Message:" << get_deal_error_msg(tid) << std::endl;
        return false;
    }
    //cout << "���� recoveries_rate=20% �ɹ���" << endl;

    //Set months from default to liquidation.
    if (set_recovery_lag_ex(tid, static_cast<short>(bondParam.recovery_lag), -1, false)) {
        std::cout << "Set set_recovery_lag_ex fail! Deal ID:" << pCmo->dealid << " Message:" << get_deal_error_msg(tid) << std::endl;
        return false;
    }
    //cout << "���� recoveries_lag=6 month �ɹ���" << endl;

    // �������������run deal�� ��̨�ͻ�����ծȯ�˵��ֽ���
    //cout << "����ʼִ�� run_deal_ex��" << endl;
    long run_ret = run_deal_ex(tid, pCmo);
    if (0 == run_ret) {
        //cout << "��ִ�н����run_deal_ret=" << run_ret << " ,run_deal_exִ�гɹ���" << endl;
    }
    else {
        return false;
    }

    //cout << endl;
    /*
    price_bond() acquire bond's analysis params
    PRICING_RESULTS contains kinds of params which we need
    */
    //Get the price of bonds.
    double dAnchor = bondParam.anchor_value;
    //cout << "����dAnchor=3.8%" << endl;
    PRICING_ANCHORS anchorType = PRICE;
    if (0 == bondParam.anchor_type) {
        anchorType = PRICE;
    }
    else if (1 == bondParam.anchor_type) {
        anchorType = YIELD;
    }
    else if (2 == bondParam.anchor_type) {
        anchorType = DM;
    }

    
    //cout << "����anchorType=YIELD�����㵽�������ʡ�" << endl;
    memset(&calc_ret, 0x00, sizeof(PRICING_RESULTS));
    //cout << "����ʼִ�� run price_bond��" << endl;
    long price_ret = price_bond(tid, "A1", anchorType, dAnchor, &calc_ret);
    if (0 == price_ret) {
        //cout << "��ִ�н����price_ret=" << price_ret << ", price_bondִ�гɹ���" << endl;
    }
    else {
        //cout << "��ִ�н����price_ret=" << price_ret << ", price_bondִ��ʧ��!!!" << endl;
        return false;
    }

    //����̯����ֹ��
    //�����Ҫ��MARKIT_BOND_CASHFLOW �ṹ���� balance �Ͷ�Ӧdates�ķ�Χ��á�
    // new MARKIT_BOND_CASHFLOW to hold bond cashflow result
    MARKIT_BOND_CASHFLOW * pMBC = new MARKIT_BOND_CASHFLOW(); // MARKIT_BOND_CASHFLOW �ṹ������ڱ����ֽ���
    // return pointer to MARKIT_BOND_CASHFLOW structure with bond cashflow populated
    pMBC = get_bond_flow_ex1(tid, NULL, "A"); // ���ﴫ��ծȯ��id��Ҳ����bondid����ȡ���ծȯ���ֽ���
    if (!pMBC) {
        std::cout << "Fail to call get_bond_flow_ex1:" << std::endl;
    }
    // inspect the bond cashflow
    int stIndex = 0;
    int endIndex = 0;
    int swFlg = 0;
    const double eps = 1e-6;
    for (int i = 0; i < pMBC->size; i++) {
        std::cout << "dates:" << pMBC->dates[i] << ", principal:" << pMBC->principal[i] << ", balance:" << pMBC->balance[i] << std::endl;
        if ((0 == swFlg) && (fabs(pMBC->principal[i]) > eps)) {
            stIndex = i;
            swFlg++;
        }
        if ((1 == swFlg) && (fabs(pMBC->balance[i]) < eps)) {
            endIndex = i;
            break;
        }
    }
    std::cout << "startdate:" << pMBC->dates[stIndex] << ", enddate:" << pMBC->dates[endIndex] << std::endl;

    /* close deal file */
    close_deal_ex(tid, pCmo);

    /* release deal scenario obj */
    release_deal_scenario_object(&tid);

    return true;
}

void t1() {
    for (int i = 0; i < CNT; i++) {
        CBondCalc bdParam;
        bdParam.deal_id = "GONGYUANANJU192";
        bdParam.settlement_date = "04/19/19";
        bdParam.prepay_rate = 0.1278;
        bdParam.default_rate = 0.0003;
        bdParam.recoveries_rate = 0.2;
        bdParam.recovery_lag = 6;
        bdParam.anchor_type = 2;
        bdParam.anchor_value = 1.82;
        PRICING_RESULTS calcret;
        bool ret = query_bond_calc(bdParam, calcret);
        cout << "t1, i=" << (i + 1) << "," << calcret.AccruedInterest << "," << calcret.AverageLife << "," << calcret.DiscountMargin << endl;
    }
}

void t2() {
    for (int i = 0; i < CNT; i++) {
        CBondCalc bdParam;
        bdParam.deal_id = "GONGYUANZHIYUAN181";
        bdParam.settlement_date = "04/19/19";
        bdParam.prepay_rate = 0.1278;
        bdParam.default_rate = 0.0003;
        bdParam.recoveries_rate = 0.2;
        bdParam.recovery_lag = 6;
        bdParam.anchor_type = 1;
        bdParam.anchor_value = 0.038;
        PRICING_RESULTS calcret;
        bool ret = query_bond_calc(bdParam, calcret);
        cout << "t2, i=" << (i + 1) << "," << calcret.AccruedInterest << "," << calcret.AverageLife << "," << calcret.DiscountMargin << endl;
    }
}

void t3() {
    for (int i = 0; i < CNT; i++) {
        CBondCalc bdParam;
        bdParam.deal_id = "GONGYUANZHIYUAN181";
        bdParam.settlement_date = "04/19/19";
        bdParam.prepay_rate = 0.1278;
        bdParam.default_rate = 0.0003;
        bdParam.recoveries_rate = 0.2;
        bdParam.recovery_lag = 6;
        bdParam.anchor_type = 1;
        bdParam.anchor_value = 0.038;
        PRICING_RESULTS calcret;
        bool ret = query_bond_calc(bdParam, calcret);
        cout << "t3, i=" << (i + 1) << "," << calcret.AccruedInterest << "," << calcret.AverageLife << "," << calcret.DiscountMargin << endl;
    }
}

void t4() {
    for (int i = 0; i < CNT; i++) {
        CBondCalc bdParam;
        bdParam.deal_id = "GONGYUANZHIYUAN181";
        bdParam.settlement_date = "04/19/19";
        bdParam.prepay_rate = 0.1278;
        bdParam.default_rate = 0.0003;
        bdParam.recoveries_rate = 0.2;
        bdParam.recovery_lag = 6;
        bdParam.anchor_type = 1;
        bdParam.anchor_value = 0.038;
        PRICING_RESULTS calcret;
        bool ret = query_bond_calc(bdParam, calcret);
        cout << "t4, i=" << (i + 1) << "," << calcret.AccruedInterest << "," << calcret.AverageLife << "," << calcret.DiscountMargin << endl;
    }
}


int main(void)
{
    boost::timer t;
    boost::thread *pd[THREAD_CNT];
    for (int i = 0; i < THREAD_CNT; i++) {
        pd[i] = new boost::thread(&t1);
    }
    for (int b = 0; b < THREAD_CNT; b++) {
        pd[b]->join();
        delete pd[b];
    }

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
    cout << t.elapsed() << " �롣" << endl;

    system("pause");
    return 0;
}
