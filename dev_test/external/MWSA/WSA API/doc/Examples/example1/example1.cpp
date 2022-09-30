//=======================================================================
// Copyright (C) 2012 Moody’s Analytics, Inc., and/or its licensors and
// affiliates. All rights reserved.
//
//=======================================================================

#pragma warning(disable : 4996)


#include "wsa.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <list>
#include <map>


// input path
#ifdef _WIN32
const char* input_path="..\\DATA";
#else
const char* input_path="../DATA";
#endif

// utils
void printBondFlow(void* tid, MARKIT_DEAL_INFO& deal_info);
void printCollatFlow(void* tid, MARKIT_DEAL_INFO& deal_info);

//call back functions
int fCOLLAT_ASSUMP_CB(void* tid, char* first_period_date, int max_periods, PAY_POOL_INFO* pool_info, CCMO_COLLAT_ASSUMPTIONS* assumptions, void* user_data, char* error_message,int max_size_of_error_message);

int main()
{
    //Set the path to deals.
    set_input_path(input_path);
    //Set the error handling level.
    set_error_handling_level(ERROR_HANDLING_LEVEL_LOG_IT); // ERROR_HANDLING_LEVEL_STOP_CALCULATION
    //Set the rule to handle missing interst rates.
    set_missing_interest_rates_handling(MISSING_INTEREST_RATES_USE_ZERO); // MISSING_INTEREST_RATES_TREAT_AS_ERROR
    //Set the preferred engine for overlapping deals.
    set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS); //PICK_CHS_ENGINE_FOR_MAPPED_DEALS

    //Create deal_scenario object (tid).
    void* tid(NULL);
    create_deal_scenario_object(&tid,NULL,NULL,NULL);

    //CMO_STRUCT for open deal
    CMO_STRUCT cmo={};
    //Set deal name.
    strcpy(cmo.dealid, "ACE06NC1");
    //Set settlement date.
    strcpy(cmo.settlement_date, "08/31/11");
    //Set to run actual collateral.
    cmo.actual_coll=1; // 0 for repline

    int ret = open_deal_ex(tid,&cmo);
    if (ret!=0)
    {
        std::cout <<"Open deal fail! Deal ID:"<<cmo.dealid <<" Message:"<< get_deal_error_msg(tid) << std::endl;
        return ret;
    }
    else
    {
        std::cout << "Opea deal success! Deal ID:"<<cmo.dealid<< std::endl;
    }

    //Get current deal engine info
    ENGINE_TYPE et=get_current_deal_engine(tid);
    switch (et)
    {
    case CHS_ENGINE:
        std::cout<<"Current deal engine is CHS_ENGINE"<<std::endl;
        break;
    case SFW_ENGINE:
        std::cout<<"Current deal engine is SFW_ENGINE"<<std::endl;
        break;
    case UNKNOWN_ENGINE:
        std::cout<<"Current deal engine is unspecified"<<std::endl;
        break;
    default:
        std::cout<<"Error, undefined behavior"<<std::endl;
        return -1;
    }

    //Get available update dates
    std::vector<int> UpdDates(MAX_PERIODS);
    const int n_updates=get_dates_from_upd_ex(tid,cmo.dealid,&UpdDates.front());
    if (n_updates>0)
    {
        std::cout << "There are "<<n_updates<<" updates:"<<std::endl;
        for (int i=0; i<n_updates; ++i)
        {
            std::cout<<UpdDates[i]<<std::endl;
        }
    }

    //Get current update id (valid for SFW deal only, will return 0 for CHS deal)
    const int update_id_len(20);
    char update_id[update_id_len]={0};
    get_deal_update_id(tid,update_id, update_id_len);
    std::cout<<"Update id for settlement date "<<cmo.settlement_date<<" is "<<update_id<<std::endl;

    //Get deal information.
    MARKIT_DEAL_INFO deal_info={};
    get_deal_info(tid,0,&deal_info);
    std::cout << "Deal Info:"<< std::endl;
    std::cout << "Deal name:"<<deal_info.deal_name<<std::endl;
    std::cout << "Issuer:"<<deal_info.issuer<<std::endl;
    std::cout << "First pay date:"<<deal_info.first_pay_date<<std::endl;
    std::cout << "Next pay date:"<<deal_info.next_pay_date<<std::endl;
    std::cout << "Deal update date:"<<deal_info.update_date<<std::endl;

    //Indicates whether the specified deal has an underlying deal(s).
    ret = deal_has_underlying_deal(tid);
    if (ret==0)
    {
        std::cout << "Regular collateral only."<< std::endl;
    }
    else if (ret==1)
    {
        std::cout << "Underlying deal(s), no regular collateral."<<std::endl;
    }
    else if (ret==2)
    {
        std::cout << "Underlying deal(s) and regular collateral."<<std::endl;
    }

    //Get the issuer type.
    char issuer[80]={0};
    char type[26]={0};
    get_deal_issuer_type(tid, issuer,type);
    std::cout << "Issuer:"<<issuer<< std::endl;
    std::cout << "Issuer type:"<<type<< std::endl;

    //Get the longest period of all bonds.
    long longest = get_longest_ex(tid);
    std::cout << "The longest period:"<<longest<< std::endl;

    //Get surveillance data as a string for a specific month.
    int YYYYMM = 201007;
    const int SURV_DATA_SIZE = 4096;
    char surv_buf_s[SURV_DATA_SIZE]={0};
    long actSize=0;
    ret = get_surveillance_data( tid, YYYYMM, surv_buf_s,SURV_DATA_SIZE,&actSize);
    if (ret==0)
    {
        std::cout << "Surveillance data:"<< std::endl;
        std::cout <<surv_buf_s<< std::endl;
    }

    //Get the deal surveillance data as of the month and year provided in the format YYYYMM.
    MARKIT_DEAL_SURVEILLANCE_DATA survData;
    memset(&survData,0,sizeof(MARKIT_DEAL_SURVEILLANCE_DATA));
    ret=get_deal_surv_data(tid, &survData, YYYYMM);
    if (ret==0)
    {
        std::cout << "Deal level Surveillance data:"<< std::endl;
        std::cout << "Report Date:"<<survData.reportDate<<std::endl;
        std::cout << "Requird OCAMT:"<<survData.reqOCAmt<<std::endl;
        std::cout << "Actual OCAMT:"<<survData.actOCAmt<<std::endl;
        std::cout << "Wala:"<<survData.wala<<std::endl;
        std::cout <<std::endl;

        std::cout << "Group level Surveillance data:"<< std::endl;
        for (int i=0; i<MAX_COLL_GROUPS; ++i)
        {
            std::cout << "Group ID:"<<survData.groupLevelData[i].groupId<<std::endl;
            std::cout << "Report date:"<<survData.groupLevelData[i].reportDate<<std::endl;
            std::cout << "End scheduled balance:"<<survData.groupLevelData[i].endSchedBalance<<std::endl;
            std::cout << "Requird OCAMT:"<<survData.groupLevelData[i].reqOCAmt<<std::endl;
            std::cout << "Actual OCAMT:"<<survData.groupLevelData[i].actOCAmt<<std::endl;
            std::cout << "Wala:"<<survData.groupLevelData[i].wala<<std::endl;
        }
    }

    // get pool group historical data
    int yyyymms[100]={};
    int num_avail=0;
    ret = get_moodys_pool_history_avail_YYYYMMs(tid, 1, yyyymms, 100, &num_avail);
    if (ret>=0)
    {
        MOODYS_POOL_HISTORY ph[1]={};
        ret = get_moodys_pool_history(tid, 1, ph, 1, yyyymms[0]);
        if (ret>=0)
        {
            std::cout<<ph[0].CDR_1M<<std::endl;
        }
    }

    //Get underlying deals information, and do some settings for underlying deals
    const int n_under_deal = view_reremic_deals(tid, NULL, NULL);
    std::vector<CMO_STRUCT> remics(n_under_deal>0?n_under_deal:1);
    view_reremic_deals(tid, NULL, &remics[0]);
    if (n_under_deal > 0)
    {
        std::cout << "This are "<<n_under_deal<<" underlying deals in "<<cmo.dealid<<std::endl;
        for (int i=0; i<n_under_deal; ++i)
        {
            MARKIT_DEAL_INFO uderlying_deal_info={};
            //Get deal information of underlying deal
            get_deal_info(tid, remics[i].dealid, &uderlying_deal_info);
            std::cout << "Underlying Deal " <<uderlying_deal_info.deal_name<< " Info:"<< std::endl;
            std::cout << "Underlying Deal Settlement date:"<<remics[i].settlement_date<<std::endl;
            std::cout << "Underlying Deal Issuer:"<<uderlying_deal_info.issuer<<std::endl;
            std::cout << "Underlying Deal First pay date:"<<uderlying_deal_info.first_pay_date<<std::endl;
            std::cout << "Underlying Deal Next pay date:"<<uderlying_deal_info.next_pay_date<<std::endl;
        }

        //Get collaterals info for the first underlying deal 
        void* coll_iter = obtain_collat_iterator(tid, remics[0].dealid);
        std::vector<MARKIT_POOL_INFO> collat_vector;
        MARKIT_POOL_INFO* coll_info = NULL;
        while (coll_info = get_next_collat(tid, coll_iter))
        {
            collat_vector.push_back(*coll_info);
        }
        std::vector<MARKIT_POOL_INFO*> replace_coll_vec;
        std::cout<<"Underlying Deal "<<remics[0].dealid<<" have " <<collat_vector.size()<<"collaterals"<<std::endl;
        for (size_t i=0; i < collat_vector.size(); ++i)
        {
            MARKIT_POOL_INFO& pool_item = collat_vector[i];
            std::cout<<"Collateral "<<i+1<<" in underlying deal "<<remics[0].dealid<<std::endl;
            std::cout<<"Current balance:"<<pool_item.current_balance<<std::endl;
            std::cout<<"Factor:"<<pool_item.factor<<std::endl;
            std::cout<<"Wac:"<<pool_item.wac<<std::endl;
            std::cout<<"Wala:"<<pool_item.wala<<std::endl;
            std::cout<<"LTV:"<<pool_item.ltv<<std::endl;
            replace_coll_vec.push_back(&pool_item);
        }
        //Modify first collateral balance for first underlying deal and replace collaterals of the first underlying deal
        if (replace_coll_vec.size() > 0)
        {
            //modify  balance + 1000.0
            replace_coll_vec[0]->current_balance += replace_coll_vec[0]->current_balance;
            replace_coll_vec[0]->avg_loan_bal += replace_coll_vec[0]->avg_loan_bal; //current_balance and avg_loan_bal are always the same
            replace_collateral(tid, remics[0].dealid, &replace_coll_vec[0], replace_coll_vec.size());
        }

        //Get deal accounts info for the first underlying deal, if deal is a reremic deal
        int underlying_account_num = get_deal_account_avail(tid, remics[0].dealid, NULL, NULL, 0);
        if (underlying_account_num > 0)
        {
            std::vector<char> account_name_buf(underlying_account_num * 11);
            std::vector<char*> account_names(underlying_account_num);
            for (int i=0; i<underlying_account_num; ++i)
            {
                account_names[i] = &account_name_buf[i*11];
            }
            std::vector<DEAL_ACCOUNT_INFO> account_info(underlying_account_num);
            get_deal_account_avail(tid, remics[0].dealid, &account_names.front(), &account_info.front(), underlying_account_num);

            //Set the first underlying deal account default.
            for (int i=0; i<underlying_account_num; ++i)
            {
                if (account_info[i].acct_type == DEAL_INSURANCE_ACCOUNT)
                {
                    set_deal_account_default(tid, remics[0].dealid, account_names[i], true);
                }
            }
        }
         //Get additional information(segment, MACR or PO) of bond A1 for the first underlying deal 
        BOOLYAN u_isSeg = 0;
        BOOLYAN u_isMacr = 0;
        BOOLYAN u_isPo = 0;
        get_reremic_bond_misc(tid, remics[0].dealid, "A1", &u_isSeg, &u_isMacr, &u_isPo);
        std::cout<<"Additional information on bond A1 of underlying deal "<<remics[0].dealid<<std::endl;
        std::cout<<"Segment:"<<u_isSeg<<std::endl;
        std::cout<<"MACR:"<<u_isMacr<<std::endl;
        std::cout<<"PO:"<<u_isPo<<std::endl;

        //Retrieves the band information of the bond A1 for the first underlying deal 
        double u_pricing_wal = 0.0;
        double u_low = 0.0;
        double u_high = 0.0;
        get_reremic_bond_band(tid, remics[0].dealid, "A1", &u_pricing_wal, &u_low, &u_high);
        std::cout << "Retrieves the band information Bond Name A1 underlying of deal "<<remics[0].dealid<<std::endl;
        std::cout << "Pricing Wal:"<<u_pricing_wal<<std::endl;
        std::cout << "Low collar speed:"<<u_low<<std::endl;
        std::cout << "High collar speed:"<<u_high<<std::endl;
    }

    //Get the average collateral for this deal.
    void* coll_it;
    MARKIT_POOL_INFO* avg_coll;
    coll_it =obtain_collat_iterator(tid, 0);
    avg_coll=get_average_collat(tid,coll_it,0);
    std::cout<<"Average collateral for this deal:"<<std::endl;
    std::cout<<"Original balance:"<<avg_coll->original_balance<<std::endl;
    std::cout<<"Factor:"<<avg_coll->factor<<std::endl;
    std::cout<<"Wac:"<<avg_coll->wac<<std::endl;
    std::cout<<"Wala:"<<avg_coll->wala<<std::endl;
    std::cout<<"LTV:"<<avg_coll->ltv<<std::endl;

    //Get all the deal accounts info,if the deal accounts are available.
    int account_num = get_deal_account_avail(tid, NULL, NULL, NULL, 0);
    if (account_num > 0)
    {
        std::vector<char> name_buf(account_num*11);
        std::vector<char*> names(account_num);
        for(int i = 0; i<account_num; i++ )
        {
            names[i]=&name_buf[i*11];
        }
        std::vector<DEAL_ACCOUNT_INFO> info(account_num);
        get_deal_account_avail( tid, NULL, &names.front(), &info.front(), account_num );

        //Set account default.
        for (int i=0; i<account_num; ++i)
        {
            if(info[i].acct_type==DEAL_INSURANCE_ACCOUNT)
            {
                set_deal_account_default(tid, NULL, names[i], true);
            }
        }
    }

    //Loop through all bonds.
    std::vector<CCMO_BONDS_S> bonds(deal_info.num_bonds);
	view_all_bonds_ex(tid,&bonds[0]);
    for (size_t b_i=0; b_i<bonds.size(); ++b_i)
    {
        CCMO_BONDS_S bi=bonds[b_i];
        //Returns the deal and bond id for an industry-standard bond identifier
        char err_buff[80];
        int err_len = 80;
        char deal_name[11]={0};
        char bond_id[11]={0};
        get_moodys_id(bi.cusip, deal_name, bond_id, err_buff, err_len);

        //Get additional information on a bond (segment, MACR or PO).
        BOOLYAN isSeg = 0;
        BOOLYAN isMacr = 0;
        BOOLYAN isPo = 0;
        get_bond_misc_ex(tid,bond_id, &isSeg, &isMacr, &isPo);
        std::cout<<"Additional information on bond:"<<bond_id<<std::endl;
        std::cout<<"Segment:"<<isSeg<<std::endl;
        std::cout<<"MACR:"<<isMacr<<std::endl;
        std::cout<<"PO:"<<isPo<<std::endl;

        //Calculates the cashflow offset, days of accrued interest, and days the first period is long or short for a bond transaction.
        int monthsOffset=0;
        int daysAccrued=0;
        int daysOffset=0;
        char settdate[9]={0};
        MARKIT_BOND_INFO markit_bond_info;
        get_bond_info_by_tranche(tid, NULL, bi.stripped_id, &markit_bond_info);
        dayt_to_str_with_day_count(bi.date, settdate, markit_bond_info.day_count);
        ret=calc_cashflow_offset_ex(tid, bond_id, settdate, &monthsOffset, &daysAccrued, &daysOffset);
        if (ret==0)
        {
            std::cout << "Calculates the cashflow offset Bond Name:"<<bond_id<<std::endl;
            std::cout << "Offset months:"<<monthsOffset<<std::endl;
            std::cout << "Offset days of accrued interest:"<<daysAccrued<<std::endl;
            std::cout << "Offset days the first period:"<<daysOffset<<std::endl;
            std::cout << std::endl;
        }

        //Retrieves the band information for the bond
        double pricing_wal = 0.0;
        double low = 0.0;
        double high = 0.0;
        get_bond_band(tid,bond_id, &pricing_wal, &low, &high);
        std::cout << "Retrieves the band information Bond Name:"<<bond_id<<std::endl;
        std::cout << "Pricing Wal:"<<pricing_wal<<std::endl;
        std::cout << "Low collar speed:"<<low<<std::endl;
        std::cout << "High collar speed:"<<high<<std::endl;
        std::cout << std::endl;

        //Retrieves the day count convention, business day/calendar rules.
        char dayCount[26]={0};
        char busRules[26]={0};
        char currency[26]={0};
        ret=get_bond_day_cal_cur_ex(tid, bond_id, 1, dayCount, busRules, currency);
        if (ret==0)
        {
            std::cout << "Retrieves the day count convention Bond Name:"<<bond_id<<std::endl;
            std::cout << "Day count rule:"<<dayCount<<std::endl;
            std::cout << "Payment date rule:"<<busRules<<std::endl;
            std::cout << "Currency:"<<currency<<std::endl;
            std::cout << std::endl;
        }
        //Get history data.
        double hist_factor[MAX_PERIODS] = {0};
        double hist_coupon[MAX_PERIODS] = {0};
        int period=0;
        period=get_hist_data_ex(tid,&cmo,bond_id,hist_factor,hist_coupon);
        if (period)
        {
            std::cout << "Get history data for bond:"<<bond_id<<std::endl;
            for(int i=0;i<period;i++)
            {
                std::cout << "History factor:"<<hist_factor[i]<<std::endl;
                std::cout << "History coupon:"<<hist_coupon[i]<<std::endl;
            }
        }
    }

    //Set required intrest rates.
    double pRates[MAX_PERIODS] = {0};
    double ZERO_RATE[MAX_PERIODS] = {0};
    int Rates[MAX_INDEX_TYPES_EX]={0};
    const int nRates=get_required_rate_codes(tid, Rates, MAX_INDEX_TYPES_EX);
    for (int i=0; i<nRates; ++i)
    {
        short idx(Rates[i]);
        if (WAC == idx)
        {
            set_rate_ex(tid, &idx, MAX_PERIODS, ZERO_RATE);
        }
        else
        {
            set_rate_ex(tid, &idx, MAX_PERIODS, pRates);
        }
    }

    //Set prepayments
    double dCPRs[MAX_PERIODS]={0};
    set_prepayments_ex(tid, PREPAY_CURVE_CPR, MAX_PERIODS, dCPRs, -1, false);

    //Set defaults
    double dMDRs[MAX_PERIODS]={0};
    set_defaults_ex(tid, DEFAULT_CURVE_MDR, MAX_PERIODS, dMDRs, -1, false);

    //Set which balance defaults are calculated from.
    set_default_from_ex(tid, DEFLT_FROM_CURBAL, false); //DEFLT_FROM_ORIGBAL, DEFLT_FROM_ZERO_CPR

    //Set months from default to liquidation.
    set_recovery_lag_ex(tid, 3, -1, false);

    //Set servicer advances.
    set_service_advances_ex(tid, SERVICER_ADVANCES_BOTH, false); // SERVICER_ADVANCES_INTEREST, SERVICER_ADVANCES_NOTHING
    
    //Set ppy,defaults...settings for the first underlying deal, if deal is a reremic deal
    if (n_under_deal > 0)
    {
        //Set prepayments for the first underlying deal
        set_reremic_prepayments(tid, remics[0].dealid, PREPAY_CURVE_CPR, MAX_PERIODS, dCPRs, -1);

        //Set defaults for the first underlying deal
        set_reremic_defaults(tid, remics[0].dealid, DEFAULT_CURVE_MDR, MAX_PERIODS, dMDRs, -1);

        //Set which balance defaults are calculated from for the first underlying deal
        set_reremic_default_from(tid, remics[0].dealid, DEFLT_FROM_ORIGBAL); //DEFLT_FROM_ORIGBAL, DEFLT_FROM_ZERO_CPR

        //Set 3 months from default to liquidation for the first underlying deal
        set_reremic_recovery_lag(tid, remics[0].dealid, 3, -1);

        //Set servicer advances for the first underlying deal
        set_reremic_service_advances(tid, remics[0].dealid, SERVICER_ADVANCES_BOTH); // SERVICER_ADVANCES_INTEREST, SERVICER_ADVANCES_NOTHING
    }

    //Set the bond cash flow mode and payment dates type.
    //                    BOND_CF_MODE_TRADING  BOND_PAYMENT_DATES_THEORETICAL
    set_bond_cf_mode(tid, BOND_CF_MODE_HOLDING, BOND_PAYMENT_DATES_BUS_RULES, false);

    //Set the deal calculation level.
    set_deal_calc_level(tid, CALC_LEVEL_FULL, false); // CALC_LEVEL_BASIC
    //Set call state
    clean_up_call_ex(tid, 1, -1, false); // 1(On), 0(Off)

    double sample_vector[MAX_PERIODS] = {0};

    //Set user data for callback function
    if(set_user_data_for_cb(tid,(void*)&sample_vector[0])!=0)
    {
        std::cout <<"Set user data for callback function fail! Message:"<< get_deal_error_msg(tid) << std::endl;
    }
    //Install callback function
    if(install_collat_assump_cb(tid, fCOLLAT_ASSUMP_CB)!=0)
    {
        std::cout <<"Install collateral assumption callback function fail! Message:"<< get_deal_error_msg(tid) << std::endl;
    }

    //Run deal.
    if (run_deal_ex(tid,&cmo)<0)
    {
        std::cout <<"Run deal fail! Deal ID:"<<cmo.dealid <<" Message:"<< get_deal_error_msg(tid) << std::endl;
        return -1;
    }

    //Uninstall callback function
    install_collat_assump_cb(tid,NULL);

    //Print bond cashflow.
    printBondFlow(tid, deal_info);

    //Print Collateral cashflow.
    printCollatFlow(tid, deal_info);


    //Retrieves information about the deal call.
    char call_date[11]    =  {0};
    double call_percent   =  0.0;
    int call_percent_calc =  0;
    ret = get_cleanup_call_ex(tid, call_date, &call_percent, &call_percent_calc);
    if (ret==CLEANUP_CALL_DATE)
    {
        std::cout << "Deal call by date:"<<call_date<<std::endl;
    }
    else if (ret==CLEANUP_CALL_PERCENT)
    {
        std::cout << "Deal call by percent:"<<call_percent<<std::endl;
    }
    else if (ret==CLEANUP_CALL_EITHER)
    {
        std::cout << "Deal call by date:"<<call_date<<" or percent:"<<call_percent<<std::endl;
    }
    else if (ret==CLEANUP_CALL_BOTH)
    {
        std::cout << "Deal call by date:"<<call_date<<" and percent:"<<call_percent<<std::endl;
    }


    //Get all available triggers.
    int trg_num = get_triggers_avail(tid, NULL, NULL);
    if (trg_num>0)
    {
        std::vector<char> name_buf(trg_num*21);
        std::vector<char*> names(trg_num);
        std::vector<signed char> status_buf(trg_num*MAX_PERIODS);
        std::vector<signed char*> status(trg_num);
        for (int i=0; i<trg_num; ++i)
        {
            names[i] = &name_buf[i*21];
            status[i] = &status_buf[i*MAX_PERIODS];
        }
        get_triggers_avail(tid, &names.front(), NULL);
        //Get all triggers' status.
        std::cout<<"All triggers' status:"<<std::endl;
        for (int i=0; i<trg_num; ++i)
        {
            int statusnum=get_trigger_status(tid, names[i], status[i]);
            std::cout<<"Trigger name:"<<names[i]<<std::endl;
            for (int j=0; j<MAX_PERIODS; ++j)
            {
                std::cout<<"Per:"<<j<<" Trigger status:"<<(int)status[i][j]<<std::endl;
            }
        }

        //Override all triggers' status.
        std::cout<<"Overwrite All triggers' status:"<<std::endl;
        SBYTE TriggerStatus[MAX_PERIODS] = {0};
        for (int i=0; i<trg_num; ++i)
        {
            set_trigger_override(tid, names[i], -1, &TriggerStatus[0]);
            std::cout<<"Overwrite trigger"<<names[i]<<" to "<<(int)TriggerStatus[0]<<std::endl;
        }
    }

    // set trigger override
    const int TRG_NAME_BUF_SIZE = 21;
    const int TRG_DESC_BUF_SIZE = 1025;
    const int OVERRIDE_CODE = 2; //0 for no override, 1 for Always "NO" and 2 for Always "YES"
    const int OVERRIDE_DATE = 20141125;

    int masterTriggersNum = get_trigger_avail_ex(tid, NULL, NULL, NULL, NULL, 0);
    if(masterTriggersNum > 0)
    {
        std::vector<char> vecNameBuf(masterTriggersNum * TRG_NAME_BUF_SIZE), vecDescBuf(masterTriggersNum * TRG_DESC_BUF_SIZE); 
        std::vector<char*> vecNames(masterTriggersNum), vecDescs(masterTriggersNum);
        std::vector<int> vecNumSubTriggers(masterTriggersNum);
        for(int i = 0; i < masterTriggersNum; i++)
        {
            vecNames[i] = &vecNameBuf[i * TRG_NAME_BUF_SIZE];
            vecDescs[i] = &vecDescBuf[i * TRG_DESC_BUF_SIZE];
        }
        get_trigger_avail_ex(tid, NULL, &vecNames.front(), &vecDescs.front(), &vecNumSubTriggers.front(), masterTriggersNum);

        // loop all high level triggers
        for(int highTrigIdx = 0; highTrigIdx < masterTriggersNum; ++highTrigIdx)
        {
            std::string masterTrigName = vecNames[highTrigIdx];
            int subTriggersNum = vecNumSubTriggers[highTrigIdx];

            // get master trigger info
            std::vector<char> vecSubNameBuf(subTriggersNum * TRG_NAME_BUF_SIZE), vecSubDescBuf(subTriggersNum * TRG_DESC_BUF_SIZE); 
            std::vector<char*> vecSubNames(subTriggersNum), vecSubDescs(subTriggersNum);
            for(int i = 0; i<subTriggersNum; i++)
            {
                vecSubNames[i] = &vecSubNameBuf[i * TRG_NAME_BUF_SIZE];
                vecSubDescs[i] = &vecSubDescBuf[i * TRG_DESC_BUF_SIZE];
            }
            signed char status[MAX_PERIODS] = {0};
            char subTriggerLogic[TRG_NAME_BUF_SIZE] = {0};
            get_master_trigger_info(tid, NULL, masterTrigName.c_str(), status, subTriggerLogic, &vecSubNames.front(), &vecSubDescs.front(), subTriggersNum);
            
            std::cout<<"Master trigger: "<<masterTrigName<<", Desc: "<<vecDescs[highTrigIdx]<<", Logic: "<<subTriggerLogic<<", Sub trigger num: "<<vecNumSubTriggers[highTrigIdx]<<std::endl;

            // loop all sub triggers, set overide
            for(int subTrigIdx = 0; subTrigIdx < subTriggersNum; ++subTrigIdx)
            {
                std::cout<<"Sub trigger: "<<vecSubNames[subTrigIdx]<<", Desc: "<<vecSubDescs[subTrigIdx]<<std::endl;
                set_trigger_override_ex(tid, NULL, vecSubNames[subTrigIdx], OVERRIDE_CODE, OVERRIDE_DATE);
            }
        }
    }

    // set call option
    set_call_option(tid, STEPUP_CALL, false);

    //Get colls info and override triggers for the first underlying deal, if deal is a reremic deal
    if (n_under_deal > 0)
    {
         //Get all colls info for the first underlying deal after replaced
        const size_t num_colls = remics[0].num_colls>0?remics[0].num_colls:1;
        std::vector<CCMO_POOL_INFO> cpi(num_colls);
        std::vector<CCMO_POOL_INFO_EX> cpi_ex(num_colls);
        std::vector<CCMO_ARM_INFO> cpi_arm_info(num_colls);
        for (size_t i=0; i<num_colls; ++i)
        {
            cpi_ex[i].arm = &cpi_arm_info[i];
        }
        view_reremic_colls_ex(tid, remics[0].dealid, -1, &cpi.front(), &cpi_ex.front(),sizeof(CCMO_POOL_INFO),sizeof(CCMO_POOL_INFO_EX),sizeof(CCMO_ARM_INFO));
        for (size_t i=0; i<cpi.size(); ++i)
        {
            std::cout<<"Underlying Deal "<<remics[0].dealid<<" Collateral: "<<i+1<<std::endl;
            std::cout<<"Current balance:"<<cpi[i].current_balance<<std::endl;
        }

        //Get triggers for the first underlying deal
        int trigger_num = get_reremic_triggers_avail(tid, remics[0].dealid, NULL, NULL);
        if (trigger_num>0)
        {
            std::vector<char> trigger_name_buf(trigger_num*21);
            std::vector<char*> trigger_names(trigger_num);
            std::vector<signed char> trigger_status_buf(trigger_num*MAX_PERIODS);
            std::vector<signed char*> trigger_status(trigger_num);
            for (int i=0; i<trigger_num; ++i)
            {
                trigger_names[i] = &trigger_name_buf[i*21];
                trigger_status[i] = &trigger_status_buf[i*MAX_PERIODS];
            }
            get_reremic_triggers_avail(tid, remics[0].dealid, &trigger_names.front(), NULL);

            //Get all triggers' status for the first underlying deal
            std::cout<<"Underlying deal"<<remics[0].dealid<<"all triggers' status:"<<std::endl;
            for (int i=0; i<trigger_num; ++i)
            {
                int statusnum = get_reremic_trigger_status(tid, remics[0].dealid, trigger_names[i], trigger_status[i]);
                std::cout<<"Trigger name:"<<trigger_names[i]<<std::endl;
                for (int j=0; j<MAX_PERIODS; ++j)
                {
                    std::cout<<"Per:"<<j<<" Trigger status:"<<(int)trigger_status[i][j]<<std::endl;
                }
            }
            //Overwrite all triggers' status for the first underlying deal
            std::cout<<"Overwrite "<<remics[0].dealid<<" all triggers' status:"<<std::endl;
            SBYTE TriggerStatus[MAX_PERIODS] = {0};
            for (int i=0; i<trigger_num; ++i)
            {
                set_reremic_trigger_override(tid, remics[0].dealid, trigger_names[i], -1, &TriggerStatus[0]);
                std::cout<<"Overwrite trigger"<<trigger_names[i]<<" to "<<(int)TriggerStatus[0]<<std::endl;
            }
        }
    }

    for (size_t b_i=0; b_i<bonds.size(); ++b_i)
    {
        //Get the price of bonds.
        double dAnchor             = 2.0000;
        PRICING_ANCHORS anchorType = YIELD;
        PRICING_RESULTS results;
        memset(&results, 0x00, sizeof(PRICING_RESULTS));
        ret = price_bond(tid, bonds[b_i].stripped_id, anchorType, dAnchor, &results);
        std::cout<<"Bond ID:"<<bonds[b_i].stripped_id<<" Price:"<<results.Price<<" Yield:"<<results.Yield<<std::endl;
    }

    //Close deal.
    close_deal_ex(tid,&cmo);
    
    //Open and run CDOnet Deal
    //Set the rule to handle missing exchange rates.
    set_missing_exchange_rates_handling(MISSING_EXCHANGE_RATES_USE_ONE); // MISSING_EXCHANGE_RATES_TREAT_AS_ERROR
    //Set the preferred engine for overlapping deals.
    set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS); 
    //CMO_STRUCT for open deal
    memset(&cmo, 0, sizeof(CMO_STRUCT));
    //Set deal name.
    strcpy(cmo.dealid, "STATICLO");
    //Set settlement date.
    strcpy(cmo.settlement_date, "08/01/13");
    //Set to run actual collateral.
    cmo.actual_coll = 1; // 0 for repline
    //Open CDOnet deal
    ret = open_deal_ex(tid,&cmo);
    if (ret != 0)
    {
        std::cout <<"Open deal fail! Deal ID:"<<cmo.dealid <<" Message:"<< get_deal_error_msg(tid) << std::endl;
        return ret;
    }
    else
    {
        std::cout << "Opea deal success! Deal ID:"<<cmo.dealid<< std::endl;
    }
    
    //Get deal information.
    memset(&deal_info, 0, sizeof(MARKIT_DEAL_INFO));
    if (get_deal_info(tid,0,&deal_info) < 0)
    {
        std::cout <<"Get deal info fail! Deal ID:"<<cmo.dealid <<" Message:"<< get_deal_error_msg(tid) << std::endl;
        return -1;
    }
    std::cout << "Deal Info:"<< std::endl;
    std::cout << "Deal name:"<<deal_info.deal_name<<std::endl;
    std::cout << "Issuer:"<<deal_info.issuer<<std::endl;
    std::cout << "First pay date:"<<deal_info.first_pay_date<<std::endl;
    std::cout << "Settlement date:"<<deal_info.deal_settlement_date<<std::endl;
    std::cout << "Next pay date:"<<deal_info.next_pay_date<<std::endl;
    std::cout << "Age:"<<deal_info.age<<std::endl;
    std::cout << "Number of bonds:"<<deal_info.num_bonds<<std::endl;
    std::cout << "Number of collaterals:"<<deal_info.num_colls<<std::endl;
    
    //Set reinvestment assumptions
    if (set_reinvestment_type(tid, GLOBAL_REINV) < 0)
    {
        std::cout <<"Set reinvestment type fail! Deal ID:"<<cmo.dealid <<" Message:"<< get_deal_error_msg(tid) << std::endl;
        return -1;
    }
    //Reinvestment settings  
    GLOBAL_REINVESTMENT_INFO reinvInfo;
    memset(&reinvInfo, 0, sizeof(GLOBAL_REINVESTMENT_INFO));
    reinvInfo.auto_reinv_end_date_override = 1;
    reinvInfo.term_setting = TERM_WAL_CURRENT;
    reinvInfo.ppy_pct_in_rp = 100.0;
    reinvInfo.ppy_pct_after_rp = 100.0;
    reinvInfo.sched_pct_prin_in_rp = 100.0;
    reinvInfo.sched_pct_prin_after_rp = 100.0;
    reinvInfo.rec_pct_in_rp = 100.0;
    reinvInfo.rec_pct_after_rp = 100.0;
    reinvInfo.curr_pct_prin_cash_in_rp = 100.0;
    reinvInfo.curr_pct_prin_cash_after_rp = 100.0;
    reinvInfo.num_global_libor_floor = 1;
    reinvInfo.global_libor_floor[0] = 0.0025;
    reinvInfo.use_global_reinvestment_override = 1;
    //Reinvestment assets
    const int nAssets = 2;
    std::vector<GLOBAL_REINVESTMENT_ASSET_INFO> assetInfos(nAssets);
    
    //Asset of fixed rate
    assetInfos[0].num_allocation_ratio = 1;
    assetInfos[0].allocation_ratio[0] = 0.05;
    assetInfos[0].adjustable = 0;
    strcpy(assetInfos[0].currency, "EUR");
    assetInfos[0].term = 60;
    assetInfos[0].market_price = 95;
    assetInfos[0].recovery_rate = 0.65;
    assetInfos[0].pay_freq = PAY_FREQ_QUARTERLY;
    assetInfos[0].day_count = DAYCOUNT_30_360;
    assetInfos[0].coupon = 0.07;
    assetInfos[0].bank_loans = 0;
    strcpy(assetInfos[0].first_reset, "20131001");
    strcpy(assetInfos[0].country, "CAN");
    strcpy(assetInfos[0].moodys_rating, "Aa1");
    //Asset of adjustable rate
    assetInfos[1].num_allocation_ratio = 1;
    assetInfos[1].allocation_ratio[0] = 0.95;
    assetInfos[1].adjustable = 1;
    strcpy(assetInfos[1].currency, "EUR");
    assetInfos[1].term = 60;
    assetInfos[1].market_price = 98;
    assetInfos[1].recovery_rate = 0.65;
    assetInfos[1].pay_freq = PAY_FREQ_QUARTERLY;
    assetInfos[1].day_count = DAYCOUNT_30_360;
    assetInfos[1].coupon = 0;
    assetInfos[1].leverage = 1.0;
    assetInfos[1].index = LIBOR_3;
    assetInfos[1].num_margin = 0;
    assetInfos[1].margin[0] = 0.03;
    assetInfos[1].reset_freq = 3;
    assetInfos[1].bank_loans = 1;
    strcpy(assetInfos[1].first_reset, "20131001");
    strcpy(assetInfos[1].country, "CAN");
    strcpy(assetInfos[1].moodys_rating, "Aaa");
    if (set_global_reinvestment(tid, reinvInfo, nAssets, &assetInfos.front()) < 0)
    {
        std::cout <<"Set global reinvestment fail! Deal ID:"<<cmo.dealid <<" Message:"<< get_deal_error_msg(tid) << std::endl;
        return -1;
    }
    
    //Set buy price override 
    double price = 95.0;
    if (set_buy_price_override(tid, BUY_PRICE_OVERRIDE_INPUT, &price, 0) < 0)
    {
        std::cout <<"Set buy price override fail! Deal ID:"<<cmo.dealid <<" Message:"<< get_deal_error_msg(tid) << std::endl;
        return -1;
    }

    //Set prepayments
    double dSMMs[MAX_PERIODS] = {0};
    if (set_prepayments_ex(tid, PREPAY_CURVE_SMM, MAX_PERIODS, dSMMs, -1, false) < 0)
    {
        std::cout <<"Set prepayments fail! Deal ID:"<<cmo.dealid <<" Message:"<< get_deal_error_msg(tid) << std::endl;
        return -1;
    }

    //Set defaults
    double dCDRs[MAX_PERIODS] = {0};
    if (set_defaults_ex(tid, DEFAULT_CURVE_CDR, MAX_PERIODS, dCDRs, -1, false) < 0)
    {
        std::cout <<"Set defaults fail! Deal ID:"<<cmo.dealid <<" Message:"<< get_deal_error_msg(tid) << std::endl;
        return -1;
    }

    //Set recoveries
    double dRcvs[MAX_PERIODS] = {0};
    if (set_recoveries_ex(tid, MAX_PERIODS, dRcvs, -1, false) < 0)
    {
        std::cout <<"Set recoveries fail! Deal ID:"<<cmo.dealid <<" Message:"<< get_deal_error_msg(tid) << std::endl;
        return -1;
    }

    //Set months from default to liquidation.
    if (set_recovery_lag_ex(tid, 3, -1, false) < 0)
    {
        std::cout <<"Set recovery lag fail! Deal ID:"<<cmo.dealid <<" Message:"<< get_deal_error_msg(tid) << std::endl;
        return -1;
    }

    //Get global currency
    char global_currency[4];
    if (get_global_currency(tid, global_currency) < 0)
    {
        std::cout <<"Get global currency fail! Deal ID:"<<cmo.dealid <<" Message:"<< get_deal_error_msg(tid) << std::endl;
        return -1;
    }
    //Get currencies
    const int nCurrencies = get_currencies(tid, NULL);
    if (nCurrencies < 0)
    {
        std::cout <<"Get currencies fail! Deal ID:"<<cmo.dealid <<" Message:"<< get_deal_error_msg(tid) << std::endl;
        return -1;
    }
    std::vector<char> curr_buf(nCurrencies*4); 
    std::vector<char*> currencies(nCurrencies);
    for(int i = 0; i < nCurrencies; ++i )
    {
        currencies[i] = &curr_buf[i*4];
    }
    get_currencies(tid, &currencies.front());
    //Set exchange rates and interest rates
    double exchange_rate_against_global_currency = 1.0;
    double pIndexRates[MAX_PERIODS] = {0};
    int RatesStatus[MAX_INDEX_TYPES_EX] = {0};
    for (int i = 0; i < nCurrencies; ++i)
    {
        if (set_exchange_rate(tid, currencies[i], exchange_rate_against_global_currency) < 0)
        {
            std::cout <<"Set exchange rate fail! Deal ID:"<<cmo.dealid <<" Message:"<< get_deal_error_msg(tid) << std::endl;
            return -1;
        }
        const int nRates=get_required_index_codes(tid, currencies[i], RatesStatus, MAX_INDEX_TYPES_EX);
        if (nRates < 0)
        {
            std::cout <<"Get required index codes fail! Deal ID:"<<cmo.dealid <<" Message:"<< get_deal_error_msg(tid) << std::endl;
            return -1;
        }
        for (int j=0; j<nRates; ++j)
        {
            short idx(RatesStatus[j]);
            if (set_index_rate(tid, currencies[i], &idx, MAX_PERIODS, pIndexRates) < 0)
            {
                std::cout <<"Set index rate fail! Deal ID:"<<cmo.dealid <<" Message:"<< get_deal_error_msg(tid) << std::endl;
                return -1;
            }
        }
    }

    //Run deal.
    if (run_deal_ex(tid,&cmo) < 0)
    {
        std::cout <<"Run deal fail! Deal ID:"<<cmo.dealid <<" Message:"<< get_deal_error_msg(tid) << std::endl;
        return -1;
    }

    //Print bond cashflow.
    printBondFlow(tid, deal_info);

    //Print Collateral cashflow.
    printCollatFlow(tid, deal_info);

    //Loop through all bonds.
    std::vector<CCMO_BONDS_S> vecBonds(deal_info.num_bonds);
	if (view_all_bonds_ex(tid, &vecBonds[0]) < 0)
    {
        std::cout <<"View all bonds fail! Deal ID:"<<cmo.dealid <<" Message:"<< get_deal_error_msg(tid) << std::endl;
        return -1;
    }
    //Price bonds
    for (size_t b_i=0; b_i<vecBonds.size(); ++b_i)
    {
        //Get the price of bonds.
        double dAnchor             = 2.0000;
        PRICING_ANCHORS anchorType = YIELD;
        PRICING_RESULTS results;
        memset(&results, 0x00, sizeof(PRICING_RESULTS));
        if (price_bond(tid, vecBonds[b_i].stripped_id, anchorType, dAnchor, &results) < 0)
        {
            std::cout <<"Price bond fail! Deal ID:"<<cmo.dealid <<" Message:"<< get_deal_error_msg(tid) << std::endl;
            return -1;
        }
        std::cout<<"Bond ID:"<<vecBonds[b_i].stripped_id<<" Price:"<<results.Price<<" Yield:"<<results.Yield<<std::endl;
    } 

    //Close deal.
    if (close_deal_ex(tid,&cmo) < 0)
    {
        std::cout <<"Close deal fail! Deal ID:"<<cmo.dealid <<" Message:"<< get_deal_error_msg(tid) << std::endl;
        return -1;
    }

    //Release a deal_scenario object.
    release_deal_scenario_object(&tid);
    return 0;
}


template <typename T>
T safe_value(T* array, int i) 
{
	return array ? array[i] : 0;
}
void printBondFlow(void* tid, MARKIT_DEAL_INFO& deal_info)
{
    for(int b_i=1; b_i<= deal_info.num_bonds; b_i++)
	{
		MARKIT_BOND_INFO bi;
		get_bond_info_by_index(tid,0,b_i,&bi);
        MARKIT_BOND_CASHFLOW* mb_cf =  get_bond_flow_ex1(tid, 0,bi.tranche_name);
        std::cout << deal_info.deal_name << "," << bi.tranche_name<<std::endl;
        bool add_info = get_deal_calc_level(tid) == CALC_LEVEL_FULL;
        const char sp = ',';
        std::cout << 
			"PERIOD," <<
			"DATE," <<
			"ACCRUAL_BEGIN_DATE,"
			"BALANCE," << 
			"INTEREST," <<
			"PRINCIPAL," <<
			"RATE," <<
			"CASH," << 
			"WRITE_DOWN" ;
		if(add_info)
		{
			std::cout <<
			",INTEREST_DUE," 
			"INTEREST_SHORTFALL," 
			"ACCRUAL_DAYS," 
			"UNCAPPED_RATE," 
			"HIST_FLAG" ;
		}
		std::cout <<std::endl;
        if(mb_cf->start_index_as_per_settle_date > 0)
		{
			mb_cf->interest[mb_cf->start_index_as_per_settle_date] =0;
			mb_cf->principal[mb_cf->start_index_as_per_settle_date] =0;
			mb_cf->cash[mb_cf->start_index_as_per_settle_date] =0;
			mb_cf->accrual_begin_dates[mb_cf->start_index_as_per_settle_date] =0;
			mb_cf->rate[mb_cf->start_index_as_per_settle_date] =0;
			mb_cf->principal_writedown[mb_cf->start_index_as_per_settle_date] =0;
			if(mb_cf->interest_due)
				mb_cf->interest_due[mb_cf->start_index_as_per_settle_date] =0;
			if(mb_cf->interest_shortfall)
				mb_cf->interest_shortfall[mb_cf->start_index_as_per_settle_date] =0;
			if(mb_cf->accrual_days)
				mb_cf->accrual_days[mb_cf->start_index_as_per_settle_date] =0;
			if(mb_cf->uncapped_rate)
				mb_cf->uncapped_rate[mb_cf->start_index_as_per_settle_date] =0;

		}
		for(int per=mb_cf->start_index_as_per_settle_date; per< mb_cf->size; per++)
		{
			std::cout << per << sp; 
			std::cout << mb_cf->dates[per] << sp;
			std::cout << mb_cf->accrual_begin_dates[per] << sp;
			std::cout << mb_cf->balance[per] << sp;
			std::cout << mb_cf->interest[per] << sp;
			std::cout << mb_cf->principal[per] << sp;
			std::cout << mb_cf->rate[per] << sp;
			std::cout << mb_cf->cash[per] << sp;
			std::cout << mb_cf->principal_writedown[per] << sp;
			if(add_info)
			{
				std::cout << safe_value(mb_cf->interest_due,per) << sp;
				std::cout << safe_value(mb_cf->interest_shortfall,per) << sp;
				std::cout << safe_value(mb_cf->accrual_days,per) << sp;
				std::cout << safe_value(mb_cf->uncapped_rate,per) << sp;
				std::cout << mb_cf->hist_flag[per] << sp;
			}
			std::cout << std::endl;
		}
    }
}
void printCollatFlow(void* tid, MARKIT_DEAL_INFO& deal_info)
{
    const int MAX_GROUPS = 20;
    std::vector<int> groups(MAX_GROUPS);
    int total_groups =  0;
    const int grps_returned = view_coll_groups(tid, &groups[0], MAX_GROUPS , &total_groups);
    groups.resize(grps_returned>=0?grps_returned:0);
    for(size_t i=0; i<groups.size(); ++i)
    {
        int grp = groups[i];
        double * c_cf = get_collateral_flow_ex(tid, grp, FLOW_COLLATERAL_CASH );
        std::cout << deal_info.deal_name << "," << "Collat Group " << grp << ",Cashflow" << std::endl;						
        MARKIT_COLLAT_CASHFLOW mccf;
        get_collateral_flow_ex1(tid,grp,0,&mccf );

        const char sp = ',';
        std::cout << 
            "PERIOD,"
            "DATE,"
            "BALANCE," << 
            "SCHED_PRINCIPAL," <<
            "PREPAYMENTS," <<
            "DEFAULTS," << 
            "LOSSES," << 
            "PRIN RECOVERIES," <<
            "INTEREST," <<
            "REINVESTMENT," << 
            "COLLATERAL_CASH," <<
            "GROSS_INTEREST," <<
            "SCHED P&I ," <<
            "NEG_AM " ;

        std::cout << std::endl; 

        for(int i=0; i<mccf.size; ++i)
        {
            std::cout << i << sp; 
            std::cout << mccf.dates[i] << sp;
            std::cout << mccf.balance[i] << sp;
            std::cout << mccf.sched_principal[i] ;
            std::cout << sp;
            std::cout << mccf.prepayments[i] ;
            std::cout << sp;
            std::cout << mccf.defaults[i] ;
            std::cout << sp;
            std::cout << mccf.losses[i];
            std::cout << sp;
            std::cout << mccf.prin_recoveries[i] ;
            std::cout << sp;
            std::cout << mccf.interest[i] ;
            std::cout << sp;
            std::cout << mccf.reinvestment[i] ;
            std::cout << sp;
            std::cout << mccf.cash[i] ;
            std::cout << sp;
            std::cout << mccf.gross_interest[i] ;
            std::cout << sp;
            std::cout << mccf.sched_p_and_i[i] ;
            std::cout << sp;
            std::cout << mccf.negative_amortization[i] ;
            std::cout << sp;

            std::cout << std::endl;
        }
    }
}

int fCOLLAT_ASSUMP_CB(void* tid, 
                      char* first_period_date,
                      int max_periods, 
                      PAY_POOL_INFO* pool_info,
                      CCMO_COLLAT_ASSUMPTIONS* assumptions,
                      void* user_data,
                      char* error_message,
                      int max_size_of_error_message
                      )
{
    if(tid==NULL)
    {
        strncpy(error_message,"tid is null",max_size_of_error_message);
        return -1;
    }
    if(first_period_date==NULL)
    {
        strncpy(error_message,"first_period_date is null",max_size_of_error_message);
        return -2;
    }
    if(max_periods<=0)
    {
        strncpy(error_message,"max_periods is less than 0",max_size_of_error_message);
        return -3;
    }
    if(pool_info==NULL)
    {
        strncpy(error_message,"pool_info is null",max_size_of_error_message);
        return -4;
    }
    if(assumptions==NULL)
    {
        strncpy(error_message,"assumptions is null",max_size_of_error_message);
        return -5;
    }
    if(user_data==NULL)
    {
        strncpy(error_message,"user_data is null",max_size_of_error_message);
        return -6;
    }

    std::cout<<"Set prepay vector in call back function. Deal:"<<pool_info->deal_name<<" LoanID:"<<pool_info->pool_info.id<<std::endl;

    double *sample_vector = (double*)user_data;
    assumptions->prepay_type = PREPAY_CURVE_CPR;
    assumptions->n_prepays = MAX_PERIODS;
    assumptions->prepay_vector = sample_vector;

    return 0;
}