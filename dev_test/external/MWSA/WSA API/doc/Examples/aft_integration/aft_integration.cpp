//=======================================================================
// Copyright (C) 2012 Moody’s Analytics, Inc., and/or its licensors and
// affiliates. All rights reserved.
//
//=======================================================================

#pragma warning(disable : 4996)


#include "wsa.h"
#if defined(_WIN32)
#include <WSAAftProviderApi.h>
#endif

#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <list>
#include <map>


// input path
#ifdef _WIN32
const char* input_path="..\\DATA";
const char* aft_path="..\\DATA\\AFT\\data";
#else
const char* input_path="../DATA";
const char* aft_path="../DATA/AFT/data";
#endif

// utils
int process_cb(void* userData,int pool_processed, int pools_succeeded,int pools_in_deal,char* error_message,int max_size_of_error_message);
void loan_tunning_cb(void* pool_info, void* tid, void* userData);

void printBondFlow(void* tid);

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

#if defined(_WIN32)
    //Set tuning of AFT model.
    std::vector<double> HPI(360,100);
    for(int i=1; i<360; i++)
    {
        HPI[i]=HPI[i-1] * 1;
    }
    MarkitAftTuningParam aftTuning;
    aftTuning.prepay_magnitude = 1;
    aftTuning.default_magnitude = 1;
    aftTuning.spread_30Y_Mtg_to_30Y_Index = 0.05;	
    aftTuning.spread_15Y_Mtg_to_10Y_Index=0.05;	// Spread from 15 years mortgage to 10Y index
    aftTuning.spread_7Y_Mtg_to_7Y_Index=0.05;		// Spread from 7 years mortgage to 7Y index
    aftTuning.spread_5Y_Mtg_to_5Y_Index=0.05;
    strcpy(aftTuning.AftDataPath,aft_path);
    aftTuning.modelType = MarkitAftTuningParam::MODEL_PREPAY_ONLY;
    aftTuning.errorHandling = MarkitAftTuningParam::ON_ERROR_CONTINUE;
    aftTuning.smmForFailedLoans = 5;
    aftTuning.mdrForFailedLoans = 10;
    aftTuning.recoveryForFailedLoans = 40;
    aftTuning.ltvToUseIfNotAvailable = 99;
    aftTuning.HPI_vector = &HPI[0];
    aftTuning.HPI_vector_size = static_cast<int>(HPI.size());
    aftTuning.ficoToUseIfNotAvailable = 500;
    aftTuning.zipCodeToUseIfNotAvailable = 11235;
    aftTuning.userProvidedPerformanceInfo = 1;
    aftTuning.checkResultsFor_NAN_Values = 1;
    MarkitAftDefaultModelDials dftDials;
    dftDials.dDefaultAgeMultiplier = 0.8;
    dftDials.dDefaultRateMultiplier = 0.7;
    dftDials.dLossSeverityMultiplier = 0.5;
    aftTuning.defaultModelDials = &dftDials;

    MarkitAftPrepayModelDials pDials;
    pDials.rfMultiplier = 2.5;
    pDials.htMultiplier = 2.5;

    aftTuning.prepayModelDials = &pDials;
    aftTuning.loanTunningCb = loan_tunning_cb;

    //Setup AFT model.
    SetupAFTModel(pDeal, &aftTuning);
#endif
    //Run deal
    ret=run_deal_ex(pDeal, pCmo);
    if(ret<0)
    {
        std::cout << " ERROR: "<< get_deal_error_msg(pDeal);
        return ret;
    }

    //Output bond cashflows.
    printBondFlow(pDeal);

#if defined(_WIN32)
    //Remove ADCo model.
    RemoveAFTModel(pDeal);
#endif

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
#if defined(_WIN32)
MarkitAftPrepayModelDials aftPrepDials;
#endif
void loan_tunning_cb(
                     void* pool_info, // cast to  PAY_POOL_INFO*
                     void* tid, // use for calling SetTuningParam
                     void* userData // userData memeber of  MarkitAdcoTuningParam structure
                     )
{
    PAY_POOL_INFO *pPoolInfo=(PAY_POOL_INFO *)pool_info;
    std::cout<<"call user function before calculation pool:"<<pPoolInfo->unique_pool_id<<std::endl;
    // SetPrepayModelDials(tid,&aftPrepDials);
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
