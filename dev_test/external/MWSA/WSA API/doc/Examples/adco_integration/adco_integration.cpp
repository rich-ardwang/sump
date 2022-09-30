//=======================================================================
// Copyright (C) 2012 Moody’s Analytics, Inc., and/or its licensors and
// affiliates. All rights reserved.
//
//=======================================================================

#pragma warning(disable : 4996)


#include "wsa.h"
#include "WSAAdcoProviderApi.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <list>
#include <map>


// input path
#ifdef _WIN32
const char* input_path="..\\DATA";
const char* adco_path="..\\DATA\\ADCO\\data";
#else
const char* input_path="../DATA";
const char* adco_path="../DATA/ADCO/data";
#endif

// utils
int process_cb(void* userData,int pool_processed, int pools_succeeded,int pools_in_deal,char* error_message,int max_size_of_error_message);
void loan_tunning_cb(void* pool_info, void* tid, void* userData);

void printBondFlow(void* tid);

int main()
{
    //Set the path to deals.
    set_input_path(input_path);
    //Set the preferred engine for overlapping deals.
    set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS); // PICK_CHS_ENGINE_FOR_MAPPED_DEALS
    //Use default tid
    void* pDeal(NULL);
    //Create CMO_STRUCT for open deal
    CMO_STRUCT* pCmo = new CMO_STRUCT();
    memset(pCmo, 0, sizeof(CMO_STRUCT));
    //Set the name of the Deal.
    strcpy(pCmo->dealid, "ACE06NC1");
    //Set to run actual collateral.
    pCmo->actual_coll=1; // 0 for repline
    //Open the specified deal.
    int ret = open_deal_ex(pDeal,pCmo);
    if(ret != 0)
    {
        std::cout << pCmo->dealid << ", failed to get deal info , " << get_deal_error_msg(pDeal) << std::endl;
        return ret;
    }
    else
    {
        std::cout << pCmo->dealid << " open." << std::endl;
    }

    //Set required intrest rates.
    double dRate[] = {0};
    double ZERO_RATE[] = {0};
    int Rates[MAX_INDEX_TYPES_EX]={0};
    const int nRates=get_required_rate_codes(pDeal, Rates, MAX_INDEX_TYPES_EX);
    for (int i=0; i<nRates; ++i)
    {
        short idx(Rates[i]);
        if (WAC == idx)
        {
            set_rate_ex(pDeal, &idx, 0, ZERO_RATE);
        }
        else
        {
            set_rate_ex(pDeal, &idx, 0, dRate);
        }
    }

    //Set interest rates required by ADCo model.
    short sIndexLibor24(LIBOR_24);
    short sIndexLibor120=LIBOR_120;
    double dRate24[]={0.007905};
    double dRate120[]={0.03462};
    set_rate_ex(pDeal, &sIndexLibor24, 0, dRate24);
    set_rate_ex(pDeal, &sIndexLibor120, 0, dRate120);

    //Set tuning of ADCo model.
    MarkitAdcoTuningParam AdcoParam;
    double dhpi=100;
    AdcoParam.prepay_magnitude = 1;                                            //Set the magnitude of SMM.
    AdcoParam.default_magnitude = 1;                                           //Set the magnitude of MDR.
    AdcoParam.recovery_magnitude = 1;                                          //Set the magnitude of LossSeverity.
    AdcoParam.recovery_lag = 0;                                                //Set the recovery month.
    AdcoParam.servicer_advancing = SERVICER_ADVANCES_INTEREST;                 //Set the type of servicer advancing.
    strcpy(AdcoParam.AdcoDataPath,adco_path);                                  //Set the data path to ADCo model. 
    AdcoParam.modelType = MarkitAdcoTuningParam::MODEL_DEFAULT_AND_PREPAY;     //Set which type of ADCo model to be used.
    AdcoParam.errorHandling = MarkitAdcoTuningParam::ON_ERROR_STOP;            //Set handling method when error happens.
    AdcoParam.smmForFailedLoans = 1;                                           //When error happens in ADCo model, this setting of SMM will be applied to Deal. 
    AdcoParam.mdrForFailedLoans = 5;                                           //When error happens in ADCo model, this setting of MDR will be applied to Deal.
    AdcoParam.recoveryForFailedLoans = 30;                                     //When error happens in ADCo model, this setting of LossSeverity will be applied to Deal.
    AdcoParam.ficoToUseIfNotAvailable = 640;                                   //Set default fico of one loan which not contains fico information.
    AdcoParam.userProvidedPerformanceInfo = 0;                                 //Set this flag to 1 if you replaced collateral, and new collareral has loan performance info.
    AdcoParam.HPI_vector_size=1;                                               //Set HPI forcast.
    AdcoParam.HPI_vector=&dhpi;
    AdcoParam.ficoSubprimeThreashold = 600;                                    //Fico below this value is considered as subprime.
    AdcoParam.progressCb=process_cb;                                           //Set call back function for each loan processing.
    AdcoParam.loanTunningCb=loan_tunning_cb;                                   //Set call back function for changing each loan's tunning parameter.
   
    //Setup ADCo model.
    SetupADCOModel(pDeal,&AdcoParam);

    //Run deal
    ret=run_deal_ex(pDeal, pCmo);
    if(ret<0)
    {
        std::cout << " ERROR: "<< get_deal_error_msg(pDeal);
        return ret;
    }

    //Output bond cashflows.
    printBondFlow(pDeal);

    //Remove ADCo model.
    RemoveADCOModel(pDeal);

    //close deal, clean all resources
    close_deal_ex(pDeal,pCmo);
    if (NULL != pCmo)
    {
        delete pCmo;
    }

    return 0;
}


int process_cb(void* userData,int pool_processed, int pools_succeeded,
               int pools_in_deal,
               char* error_message,
               int max_size_of_error_message)
{
    std::cout<<"pools processed:"<<pool_processed<<" pools succeeded:"<<pools_succeeded<<" pools in deal:"<<pools_in_deal<<std::endl;
    return 1;
}
void loan_tunning_cb(
                     void* pool_info, // cast to  PAY_POOL_INFO*
                     void* tid, // use for calling SetTuningParam
                     void* userData // userData memeber of  MarkitAdcoTuningParam structure
                     )
{
    PAY_POOL_INFO *pPoolInfo=(PAY_POOL_INFO *)pool_info;
    std::cout<<"call user function before calculation pool:"<<pPoolInfo->unique_pool_id<<std::endl;
    //Change tuning param before calculating each loan.
    // char szMessage[100];
    // SetTuningParam(tid,"TuneCD",0.0,szMessage,sizeof(szMessage));
}

template <typename T>
T safe_value(T* array, int i) 
{
	return array ? array[i] : 0;
}
void printBondFlow(void* tid)
{
    MARKIT_DEAL_INFO deal_info={};
    get_deal_info(tid,0,&deal_info);
    for(int b_i=1; b_i<= deal_info.num_bonds; b_i++)
	{
		MARKIT_BOND_INFO bi;
		get_bond_info_by_index(tid,0,b_i,&bi);
        MARKIT_BOND_CASHFLOW* mb_cf =  get_bond_flow_ex1(tid, 0,bi.tranche_name);
        std::cout << deal_info.deal_name << "," << bi.tranche_name<<std::endl;
        bool add_info = get_deal_calc_level(tid) == CALC_LEVEL_FULL;
        const char sp = ',';
        std::cout.setf(std::ios::fixed);
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
