// =======================================================================
//  Copyright (C) 2014 Moody's Analytics, Inc., and/or its licensors and
//  affiliates. All rights reserved.
// =======================================================================
#pragma warning(disable : 4996)

#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <list>
#include <map>

#include "wsa.h"

// input path
#ifdef _WIN32
const char* input_path="..\\DATA";
#else
const char* input_path="../DATA";
#endif

// utils
void printBondFlow(void* tid, const MARKIT_DEAL_INFO& deal_info);
void printCollatFlow(void* tid, const MARKIT_DEAL_INFO& deal_info);
void printDealInfo(const MARKIT_DEAL_INFO& deal_info);

#define CHECK_API_ERROR_BREAK(tid, expect_value, value) \
{ \
    int ret = value; \
    if (ret != expect_value) \
    { \
        std::cerr << "Error:" << get_deal_error_msg(tid) << std::endl; \
        break; \
    } \
}

#define CHECK_API_ERROR_RETURN(tid, expect_value, value) \
{ \
    int ret = value; \
    if (ret != expect_value) \
    { \
        std::cerr << "Error:" << get_deal_error_msg(tid) << std::endl; \
        return; \
    } \
}

void run_chs_deal_example();
void run_sfw_deal_example();
void run_cdonet_deal_example();

int main()
{
    // Set the path to deals.
    set_input_path(input_path);

    run_chs_deal_example();
    run_sfw_deal_example();
    run_cdonet_deal_example();

    return 0;
}

void run_chs_deal_example()
{
    std::cout << "\n=================" << __FUNCTION__ << "=================" << std::endl;
    // Set the preferred engine for overlapping deals.
    set_engine_preference(PICK_CHS_ENGINE_FOR_MAPPED_DEALS);

    do {
        // Create deal_scenario object (tid).
        void* tid = NULL;
        if (create_deal_scenario_object(&tid, NULL, NULL, NULL) < 0)
        {
            std::cerr << "Can't create deal scenario object:" << get_deal_error_msg(NULL) << std::endl;
            break;
        }

        // CMO_STRUCT for open deal
        CMO_STRUCT cmo = { 0 };
        strcpy(cmo.dealid, "AB06HE5");              // Set deal name.
        strcpy(cmo.settlement_date, "10/31/13");    // Set settlement date.
        cmo.actual_coll = 1;                        // Set to run actual collateral. set 0 for running with repline data

        // Open the specified deal.
        int ret = open_deal_ex(tid, &cmo);
        if(ret != 0)
        {
            std::cerr << "Open deal fail! Deal ID:" << cmo.dealid << ", Message:" << get_deal_error_msg(tid) << std::endl;
            break;
        }
        else
        {
            std::cout << "Open deal success! Deal ID:" << cmo.dealid << std::endl;
        }

        // Get deal information.
        MARKIT_DEAL_INFO deal_info = { NON };
        CHECK_API_ERROR_BREAK(tid, 0, get_deal_info(tid, 0, &deal_info));
        printDealInfo(deal_info);

        // Set required interest rates.
        double pRates[MAX_PERIODS] = { 0 };
        double ZERO_RATE[MAX_PERIODS] = { 0 };
        int Rates[MAX_INDEX_TYPES_EX] = { 0 };
        const int nRates = get_required_rate_codes(tid, Rates, MAX_INDEX_TYPES_EX);
        for (int i = 0; i < nRates; ++i)
        {
            short idx(Rates[i]);
            if (WAC == idx)
            {
                CHECK_API_ERROR_BREAK(tid, 0, set_rate_ex(tid, &idx, MAX_PERIODS, ZERO_RATE));
            }
            else if (idx)
            {
                CHECK_API_ERROR_BREAK(tid, 0, set_rate_ex(tid, &idx, MAX_PERIODS, pRates));
            }
        }

        // Set prepayments
        double dCPRs[200] = {0, 0.0018378,0.0008946,0.0005118,0.0007898,0.0009523,0.0006757,0.000754,0.0004566,0.0002117,0.0002227,
            0.0000984,0.0000761,0.000125,0.0000664,0.0000402,0.0001042,0.0001595,0.0001149,0.0001582,0.0001113,
            0.000073,0.0001416,0.0000835,0.0000571,0.0000739,0.0000205,0.00000654,0.0000159,0.0000224,0.0000162,
            0.0000303,0.0000215,0.0000102,0.0000188,0.00000678,0.00000349,0.00000466,0.000000606,0.000000063,
            0.000000405,0.000000741,0.000000304,0.000000745,0.000000295,0.0000000619,0.000000234,0.0000000231,
            0.00000000274,0.00000000665,0.0017694,0.0018378,0.0008946,0.0005118,0.0007898,0.0009523,0.0006757,
            0.000754,0.0004566,0.0002117,0.0002227,0.0000984,0.0000761,0.000125,0.0000664,0.0000402,0.0001042,
            0.0001595,0.0001149,0.0001582,0.0001113,0.000073,0.0001416,0.0000835,0.0000571,0.0000739,0.0000205,
            0,0,0.0017694,0.0018378,0.0008946,0.0005118,0.0007898,0.0009523,0.0006757,0.000754,0.0004566,0.0002117,
            0.0002227,0.0000984,0.0000761,0.000125,0.0000664,0.0000402,0.0001042,0.0001595,0.0001149,0.0001582,0.0001113,
            0.000073,0.0001416,0.0000835,0.0000571,0.0000739,0.0000205,0,0,0,0.0017694,0.0018378,0.0008946,0.0005118,
            0.0007898,0.0009523,0.0006757,0.000754,0.0004566,0.0002117,0.0002227,0.0000984,0.0000761,0.000125,0.0000664,
            0.0000402,0.0001042,0.0001595,0.0001149,0.0001582,0.0001113,0.000073,0.0001416,0.0000835,0.0000571,0.0000739,
            0.0000205,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

        CHECK_API_ERROR_BREAK(tid, 0, set_prepayments_ex(tid, PREPAY_CURVE_CPR, 200, dCPRs, -1, false));

        // Set defaults
        double dMDRs = 0.0051;
        CHECK_API_ERROR_BREAK(tid, 0, set_defaults_ex(tid, DEFAULT_CURVE_MDR, 0, &dMDRs, -1, false));

        // Set months from default to liquidation.
        CHECK_API_ERROR_BREAK(tid, 0, set_recovery_lag_ex(tid, 3, -1, false)); // 3 month for all loans

        // Set servicer advances.
        set_service_advances_ex(tid, SERVICER_ADVANCES_BOTH, false); // SERVICER_ADVANCES_INTEREST, SERVICER_ADVANCES_NOTHING

        // Run deal.
        if (run_deal_ex(tid, &cmo) < 0)
        {
            std::cerr << "Run deal fail! Deal ID:" << cmo.dealid << ", Message:" << get_deal_error_msg(tid) << std::endl;
            break;
        }
        else
        {
            std::cout << "Run deal success! Deal ID:" << cmo.dealid << std::endl;
        }

        // Print bond cashflow.
        printBondFlow(tid, deal_info);
        // Print Collateral cashflow.
        printCollatFlow(tid, deal_info);

        // Close deal.
        CHECK_API_ERROR_BREAK(tid, 0, close_deal_ex(tid, &cmo));

        // Release a deal_scenario object.
        CHECK_API_ERROR_BREAK(tid, 0, release_deal_scenario_object(&tid));
        tid = NULL;

        std::cout << "Run " << __FUNCTION__ << "() succeed." << std::endl;
        return;
    } while(0);

    std::cerr << "Run " << __FUNCTION__ << "() failed." << std::endl;
}

void run_sfw_deal_example()
{
    std::cout << "\n=================" << __FUNCTION__ << "=================" << std::endl;
    // Set the preferred engine for overlapping deals.
    set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);

    do {
        // Create deal_scenario object (tid).
        void* tid = NULL;
        if (create_deal_scenario_object(&tid, NULL, NULL, NULL) < 0)
        {
            std::cerr << "Can't create deal scenario object:" << get_deal_error_msg(NULL) << std::endl;
            break;
        }

        // CMO_STRUCT for open deal
        CMO_STRUCT cmo = { 0 };
        strcpy(cmo.dealid, "ACE06NC1");             // Set deal name.
        strcpy(cmo.settlement_date, "10/31/13");    // Set settlement date.
        cmo.actual_coll = 1;                        // Set to run actual collateral. set 0 for running with repline data

        // Open the specified deal.
        int ret = open_deal_ex(tid, &cmo);
        if(ret != 0)
        {
            std::cerr << "Open deal fail! Deal ID:" << cmo.dealid << ", Message:" << get_deal_error_msg(tid) << std::endl;
            break;
        }
        else
        {
            std::cout << "Open deal success! Deal ID:" << cmo.dealid << std::endl;
        }

        // Get deal information.
        MARKIT_DEAL_INFO deal_info = { NON };
        CHECK_API_ERROR_BREAK(tid, 0, get_deal_info(tid, 0, &deal_info));
        printDealInfo(deal_info);

        // Set required interest rates.
        double pRates[MAX_PERIODS] = { 0 };
        double ZERO_RATE[MAX_PERIODS] = { 0 };
        int Rates[MAX_INDEX_TYPES_EX] = { 0 };
        const int nRates = get_required_rate_codes(tid, Rates, MAX_INDEX_TYPES_EX);
        for (int i = 0; i < nRates; ++i)
        {
            short idx(Rates[i]);
            if (WAC == idx)
            {
                CHECK_API_ERROR_BREAK(tid, 0, set_rate_ex(tid, &idx, MAX_PERIODS, ZERO_RATE));
            }
            else if (idx)
            {
                CHECK_API_ERROR_BREAK(tid, 0, set_rate_ex(tid, &idx, MAX_PERIODS, pRates));
            }
        }

        // Set prepayments
        double dCPRs[200] = {0, 0.0018378,0.0008946,0.0005118,0.0007898,0.0009523,0.0006757,0.000754,0.0004566,0.0002117,0.0002227,
            0.0000984,0.0000761,0.000125,0.0000664,0.0000402,0.0001042,0.0001595,0.0001149,0.0001582,0.0001113,
            0.000073,0.0001416,0.0000835,0.0000571,0.0000739,0.0000205,0.00000654,0.0000159,0.0000224,0.0000162,
            0.0000303,0.0000215,0.0000102,0.0000188,0.00000678,0.00000349,0.00000466,0.000000606,0.000000063,
            0.000000405,0.000000741,0.000000304,0.000000745,0.000000295,0.0000000619,0.000000234,0.0000000231,
            0.00000000274,0.00000000665,0.0017694,0.0018378,0.0008946,0.0005118,0.0007898,0.0009523,0.0006757,
            0.000754,0.0004566,0.0002117,0.0002227,0.0000984,0.0000761,0.000125,0.0000664,0.0000402,0.0001042,
            0.0001595,0.0001149,0.0001582,0.0001113,0.000073,0.0001416,0.0000835,0.0000571,0.0000739,0.0000205,
            0,0,0.0017694,0.0018378,0.0008946,0.0005118,0.0007898,0.0009523,0.0006757,0.000754,0.0004566,0.0002117,
            0.0002227,0.0000984,0.0000761,0.000125,0.0000664,0.0000402,0.0001042,0.0001595,0.0001149,0.0001582,0.0001113,
            0.000073,0.0001416,0.0000835,0.0000571,0.0000739,0.0000205,0,0,0,0.0017694,0.0018378,0.0008946,0.0005118,
            0.0007898,0.0009523,0.0006757,0.000754,0.0004566,0.0002117,0.0002227,0.0000984,0.0000761,0.000125,0.0000664,
            0.0000402,0.0001042,0.0001595,0.0001149,0.0001582,0.0001113,0.000073,0.0001416,0.0000835,0.0000571,0.0000739,
            0.0000205,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

        CHECK_API_ERROR_BREAK(tid, 0, set_prepayments_ex(tid, PREPAY_CURVE_CPR, 200, dCPRs, -1, false));

        // Set defaults
        double dMDRs = 0.0051;
        CHECK_API_ERROR_BREAK(tid, 0, set_defaults_ex(tid, DEFAULT_CURVE_MDR, 0, &dMDRs, -1, false));

        // Set months from default to liquidation.
        CHECK_API_ERROR_BREAK(tid, 0, set_recovery_lag_ex(tid, 3, -1, false)); // 3 month for all loans

        // Set servicer advances.
        set_service_advances_ex(tid, SERVICER_ADVANCES_BOTH, false); // SERVICER_ADVANCES_INTEREST, SERVICER_ADVANCES_NOTHING

        // Run deal.
        if (run_deal_ex(tid, &cmo) < 0)
        {
            std::cerr << "Run deal fail! Deal ID:" << cmo.dealid << ", Message:" << get_deal_error_msg(tid) << std::endl;
            break;
        }
        else
        {
            std::cout << "Run deal success! Deal ID:" << cmo.dealid << std::endl;
        }

        // Print bond cashflow.
        printBondFlow(tid, deal_info);
        // Print Collateral cashflow.
        printCollatFlow(tid, deal_info);

        // Close deal.
        CHECK_API_ERROR_BREAK(tid, 0, close_deal_ex(tid, &cmo));

        // Release a deal_scenario object.
        CHECK_API_ERROR_BREAK(tid, 0, release_deal_scenario_object(&tid));
        tid = NULL;

        std::cout << "Run " << __FUNCTION__ << "() succeed." << std::endl;
        return;
    } while(0);

    std::cerr << "Run " << __FUNCTION__ << "() failed." << std::endl;
}

void run_cdonet_deal_example()
{
    std::cout << "\n=================" << __FUNCTION__ << "=================" << std::endl;
    // Set the preferred engine for overlapping deals.
    set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);

    do {
        // Create deal_scenario object (tid).
        void* tid = NULL;
        if (create_deal_scenario_object(&tid, NULL, NULL, NULL) < 0)
        {
            std::cerr << "Can't create deal scenario object:" << get_deal_error_msg(NULL) << std::endl;
            break;
        }

        // CMO_STRUCT for open deal
        CMO_STRUCT cmo = { 0 };
        strcpy(cmo.dealid, "STATICLO");             // Set deal name.
        strcpy(cmo.settlement_date, "08/01/13");    // Set settlement date.
        cmo.actual_coll = 1;                        // Set to run actual collateral. set 0 for running with repline data

        // Open the specified deal.
        int ret = open_deal_ex(tid, &cmo);
        if(ret != 0)
        {
            std::cerr << "Open CDO deal fail! Deal ID:" << cmo.dealid << " Message:" << get_deal_error_msg(tid) << std::endl;
            break;
        }
        else
        {
            std::cout << "Open CDO deal success! Deal ID:" << cmo.dealid << std::endl;
        }

        // Get deal information.
        MARKIT_DEAL_INFO deal_info = { NON };
        CHECK_API_ERROR_BREAK(tid, 0, get_deal_info(tid, 0, &deal_info));
        printDealInfo(deal_info);

        // Get CDO deal currencies
        const int nCurrencies = get_currencies(tid, NULL);
        if (nCurrencies < 0)
        {
            std::cerr << "Can't get currencies:" << get_deal_error_msg(tid) << std::endl;
            break;
        }
        std::vector<char> curr_buf(nCurrencies * 4);
        std::vector<char*> currencies(nCurrencies);
        for(int i = 0; i < nCurrencies; ++i )
        {
            currencies[i] = &curr_buf[i * 4];
        }
        get_currencies(tid, &currencies.front());

        double ZERO_RATE[MAX_PERIODS] = { 0 };
        // Set rates and exchange rates
        double exchangeRateForUSD = 1.0;
        for(int i = 0; i < nCurrencies; ++i)
        {
            short Rates[MAX_INDEX_TYPES_EX];
            const int iRates = get_indices(tid, currencies[i], Rates);
            for (int j = 0; j < iRates; ++j)
            {
                short idx(Rates[j]);
                if (idx)
                {
                    CHECK_API_ERROR_BREAK(tid, 0, set_index_rate(tid, currencies[i], &idx, MAX_PERIODS, ZERO_RATE));
                }
            }
            // set exchange rate against USD
            if(strcmp(currencies[i], "USD") != 0)
            {
                CHECK_API_ERROR_BREAK(tid, 0, set_exchange_rate(tid, currencies[i], exchangeRateForUSD));
            }
        }

        // Set prepayments
        double dCPRs[200] = {0, 0.0018378,0.0008946,0.0005118,0.0007898,0.0009523,0.0006757,0.000754,0.0004566,0.0002117,0.0002227,
            0.0000984,0.0000761,0.000125,0.0000664,0.0000402,0.0001042,0.0001595,0.0001149,0.0001582,0.0001113,
            0.000073,0.0001416,0.0000835,0.0000571,0.0000739,0.0000205,0.00000654,0.0000159,0.0000224,0.0000162,
            0.0000303,0.0000215,0.0000102,0.0000188,0.00000678,0.00000349,0.00000466,0.000000606,0.000000063,
            0.000000405,0.000000741,0.000000304,0.000000745,0.000000295,0.0000000619,0.000000234,0.0000000231,
            0.00000000274,0.00000000665,0.0017694,0.0018378,0.0008946,0.0005118,0.0007898,0.0009523,0.0006757,
            0.000754,0.0004566,0.0002117,0.0002227,0.0000984,0.0000761,0.000125,0.0000664,0.0000402,0.0001042,
            0.0001595,0.0001149,0.0001582,0.0001113,0.000073,0.0001416,0.0000835,0.0000571,0.0000739,0.0000205,
            0,0,0.0017694,0.0018378,0.0008946,0.0005118,0.0007898,0.0009523,0.0006757,0.000754,0.0004566,0.0002117,
            0.0002227,0.0000984,0.0000761,0.000125,0.0000664,0.0000402,0.0001042,0.0001595,0.0001149,0.0001582,0.0001113,
            0.000073,0.0001416,0.0000835,0.0000571,0.0000739,0.0000205,0,0,0,0.0017694,0.0018378,0.0008946,0.0005118,
            0.0007898,0.0009523,0.0006757,0.000754,0.0004566,0.0002117,0.0002227,0.0000984,0.0000761,0.000125,0.0000664,
            0.0000402,0.0001042,0.0001595,0.0001149,0.0001582,0.0001113,0.000073,0.0001416,0.0000835,0.0000571,0.0000739,
            0.0000205,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

        CHECK_API_ERROR_BREAK(tid, 0, set_prepayments_ex(tid, PREPAY_CURVE_CPR, 200, dCPRs, -1, false));

        // Set defaults
        double dMDRs = 0.0051;
        CHECK_API_ERROR_BREAK(tid, 0, set_defaults_ex(tid, DEFAULT_CURVE_MDR, 0, &dMDRs, -1, false));

        // Set months from default to liquidation.
        CHECK_API_ERROR_BREAK(tid, 0, set_recovery_lag_ex(tid, 3, -1, false)); // 3 month for all loans

        // Run deal.
        if (run_deal_ex(tid, &cmo) < 0)
        {
            std::cerr << "Run CDO deal fail! Deal ID:" << cmo.dealid << " Message:" << get_deal_error_msg(tid) << std::endl;
            break;
        }
        else
        {
            std::cout << "Run CDO deal success! Deal ID:" << cmo.dealid << std::endl;
        }

        // Print bond cashflow.
        printBondFlow(tid, deal_info);
        // Print Collateral cashflow.
        printCollatFlow(tid, deal_info);

        // Close deal.
        CHECK_API_ERROR_BREAK(tid, 0, close_deal_ex(tid, &cmo));

        // Release a deal_scenario object.
        CHECK_API_ERROR_BREAK(tid, 0, release_deal_scenario_object(&tid));
        tid = NULL;

        std::cout << "Run " << __FUNCTION__ << "() succeed." << std::endl;
        return;
    } while(0);

    std::cerr << "Run " << __FUNCTION__ << "() failed." << std::endl;
}

void printDealInfo(const MARKIT_DEAL_INFO& deal_info)
{
    std::cout << "Deal Info:" << std::endl;
    std::cout << "Deal name:" << deal_info.deal_name << std::endl;
    std::cout << "Issuer:" << deal_info.issuer << std::endl;
    std::cout << "First pay date:" << deal_info.first_pay_date << std::endl;
    std::cout << "Settlement date:" << deal_info.deal_settlement_date << std::endl;
    std::cout << "Next pay date:" << deal_info.next_pay_date << std::endl;
    std::cout << "Age:" <<deal_info.age << std::endl;
    std::cout << "Number of bonds:" << deal_info.num_bonds << std::endl;
    std::cout << "Number of collaterals:" << deal_info.num_colls << std::endl;
}

template <typename T>
T safe_value(T* array, int i)
{
    return array ? array[i] : 0;
}

void printBondFlow(void* tid, const MARKIT_DEAL_INFO& deal_info)
{
    for(int b_i = 1; b_i <= deal_info.num_bonds; ++b_i)
    {
        std::string compof="";
        MARKIT_BOND_INFO bi;

        CHECK_API_ERROR_RETURN(tid, 0, get_bond_info_by_index(tid, 0, b_i, &bi));

        if(bi.component>0)
        {
            MARKIT_BOND_INFO bicompof;
            CHECK_API_ERROR_RETURN(tid, 0, get_bond_info_by_index(tid, 0, bi.component, &bicompof));
            compof = bicompof.tranche_name;
        }

        MARKIT_BOND_CASHFLOW* mb_cf =  get_bond_flow_ex1(tid, 0, bi.tranche_name);
        std::cout << deal_info.deal_name << "," << bi.tranche_name << ","<<compof<<std::endl;

        bool add_info = get_deal_calc_level(tid) == CALC_LEVEL_FULL;
        const char sp = ',';
        std::cout.setf(std::ios::fixed);
        std::cout << "PERIOD,"
            << "DATE,"
            << "ACCRUAL_BEGIN_DATE,"
            << "BALANCE,"
            << "INTEREST,"
            << "PRINCIPAL,"
            << "RATE,"
            << "CASH,"
            << "WRITE_DOWN";
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
            mb_cf->interest[mb_cf->start_index_as_per_settle_date] = 0;
            mb_cf->principal[mb_cf->start_index_as_per_settle_date] = 0;
            mb_cf->cash[mb_cf->start_index_as_per_settle_date] = 0;
            mb_cf->accrual_begin_dates[mb_cf->start_index_as_per_settle_date] = 0;
            mb_cf->rate[mb_cf->start_index_as_per_settle_date] = 0;
            mb_cf->principal_writedown[mb_cf->start_index_as_per_settle_date] = 0;
            if(mb_cf->interest_due)
                mb_cf->interest_due[mb_cf->start_index_as_per_settle_date] = 0;
            if(mb_cf->interest_shortfall)
                mb_cf->interest_shortfall[mb_cf->start_index_as_per_settle_date] = 0;
            if(mb_cf->accrual_days)
                mb_cf->accrual_days[mb_cf->start_index_as_per_settle_date] = 0;
            if(mb_cf->uncapped_rate)
                mb_cf->uncapped_rate[mb_cf->start_index_as_per_settle_date] = 0;

        }
        for(int per = mb_cf->start_index_as_per_settle_date; per< mb_cf->size; ++per)
        {
            std::cout << per << sp
                << mb_cf->dates[per] << sp
                << mb_cf->accrual_begin_dates[per] << sp
                << mb_cf->balance[per] << sp
                << mb_cf->interest[per] << sp
                << mb_cf->principal[per] << sp
                << mb_cf->rate[per] << sp
                << mb_cf->cash[per] << sp
                << mb_cf->principal_writedown[per] << sp;
            if(add_info)
            {
                std::cout << safe_value(mb_cf->interest_due,per) << sp
                    << safe_value(mb_cf->interest_shortfall,per) << sp
                    << safe_value(mb_cf->accrual_days,per) << sp
                    << safe_value(mb_cf->uncapped_rate,per) << sp
                    << mb_cf->hist_flag[per] << sp;
            }
            std::cout << std::endl;
        }
    }
}

void printCollatFlow(void* tid, const MARKIT_DEAL_INFO& deal_info)
{
    const int MAX_GROUPS = 20;
    std::vector<int> groups(MAX_GROUPS);
    int total_groups = 0;
    const int grps_returned = view_coll_groups(tid, &groups[0], MAX_GROUPS, &total_groups);
    groups.resize(grps_returned>= 0 ? grps_returned : 0);
    for(size_t i = 0; i < groups.size(); ++i)
    {
        int grp = groups[i];
        double* c_cf = get_collateral_flow_ex(tid, grp, FLOW_COLLATERAL_CASH );
        std::cout << deal_info.deal_name << ", Collat Group " << grp << ", Cashflow:" << std::endl;
        MARKIT_COLLAT_CASHFLOW mccf;
        CHECK_API_ERROR_RETURN(tid, 0, get_collateral_flow_ex1(tid, grp, 0, &mccf));

        const char sp = ',';
        std::cout.setf(std::ios::fixed);
        std::cout << "PERIOD,"
            << "DATE,"
            << "BALANCE,"
            << "SCHED_PRINCIPAL,"
            << "PREPAYMENTS,"
            << "DEFAULTS,"
            << "LOSSES,"
            << "PRIN RECOVERIES,"
            << "INTEREST,"
            << "REINVESTMENT,"
            << "COLLATERAL_CASH,"
            << "GROSS_INTEREST,"
            << "SCHED P&I ,"
            << "NEG_AM "
            << std::endl;

        for(int j = 0; j < mccf.size; ++j)
        {
            std::cout << j << sp
                << mccf.dates[j] << sp
                << mccf.balance[j] << sp
                << mccf.sched_principal[j] << sp
                << mccf.prepayments[j] << sp
                << mccf.defaults[j] << sp
                << mccf.losses[j] << sp
                << mccf.prin_recoveries[j] << sp
                << mccf.interest[j] << sp
                << mccf.reinvestment[j] << sp
                << mccf.cash[j] << sp
                << mccf.gross_interest[j] << sp
                << mccf.sched_p_and_i[j] << sp
                << mccf.negative_amortization[j] << sp
                << std::endl;
        }
    }
}
