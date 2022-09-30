#include "utility.h"

void run_monte_carlo_sfw_example();
void run_monte_carlo_cdonet_example();

void run_monte_carlo_example()
{
    run_monte_carlo_sfw_example();
    run_monte_carlo_cdonet_example();
}

void run_monte_carlo_sfw_example()
{
    std::cout << "\n=================" << __FUNCTION__ << "=================" << std::endl;

    void *pDeal = NULL;
    CMO_STRUCT cmo;
    CMO_STRUCT *pCmo = &cmo;

    memset(pCmo, 0, sizeof(*pCmo));
    strncpy(pCmo->dealid, "CMBS_BOA00002", 20 - 1);

    do {
        // Set the preferred engine for overlapping deals.
        set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);

        // Create deal_scenario object.
        create_deal_scenario_object(&pDeal, NULL, NULL, NULL);

        // Open the specified deal.
        CHECK_API_RETURN_VALUE(pDeal, 0, open_deal_ex(pDeal, pCmo));

        // Show deal info.
        printDealInfo(pDeal);

        //Get global currency
        char global_currency[4];
        CHECK_API_RETURN_VALUE(pDeal, 0, get_global_currency(pDeal, global_currency));

        //Get currencies
        const int nCurrencies = get_currencies(pDeal, NULL);
        std::vector<char> curr_buf(nCurrencies*4); 
        std::vector<char*> currencies(nCurrencies);
        for(int i = 0; i < nCurrencies; ++i )
        {
            currencies[i] = &curr_buf[i*4];
        }
        CHECK_API_RETURN_VALUE(pDeal, nCurrencies, get_currencies(pDeal, &currencies.front()));

        //Set exchange rates and interest rates
        double exchange_rate_against_global_currency = 1.0;
        double pIndexRates[MAX_PERIODS] = {0};
        int RatesStatus[MAX_INDEX_TYPES_EX] = {0};
        for (int i = 0; i < nCurrencies; ++i)
        {
            CHECK_API_RETURN_VALUE(pDeal, 0, set_exchange_rate(pDeal, currencies[i], exchange_rate_against_global_currency));

            const int nRates=get_required_index_codes(pDeal, currencies[i], RatesStatus, MAX_INDEX_TYPES_EX);

            for (int j=0; j<nRates; ++j)
            {
                short idx(RatesStatus[j]);
                CHECK_API_RETURN_VALUE(pDeal, 0, set_index_rate(pDeal, currencies[i], &idx, MAX_PERIODS, pIndexRates));
            }
        }
        
        // Set edf and lgd
        double edfBase[] = {0.01, 0.02, 0.03, 0.04, 0.05};
        double lgdRate[] = {0.5, 0.5, 0.5, 0.5, 0.5};
        for (int i = 1; i <= pCmo->num_colls; ++i)
        {
            double edf[5] = {.0};
            for(int j = 0; j < 5; ++j)
            {
                edf[j] = edfBase[j] + (i / 10000.0);
            }
            CHECK_API_RETURN_VALUE(pDeal, 0, set_loan_edf(pDeal, NULL, i, edf, 5));
            CHECK_API_RETURN_VALUE(pDeal, 0, set_loan_lgd(pDeal, NULL, i, lgdRate, 5));
        }

        CHECK_API_RETURN_VALUE(pDeal, 0, set_recovery_lag_ex(pDeal, 18, -1, true));

        // Set simulation engine
        CHECK_API_RETURN_VALUE(pDeal, 0, set_simulation_engine(pDeal, SIMULATION_MONTE_CARLO));
        // Set monte carlo assumptions
        MONTE_CARLO_ASSUMPTION assumption;
        memset(&assumption, 0 , sizeof(MONTE_CARLO_ASSUMPTION));
        assumption.mode = 0;
        assumption.num_paths = 100;
        assumption.optimization_type = MC_OPTIMIZATION_NONE;
        assumption.optimization_pct = 0.0;
        MONTE_CARLO_DEF_PPY_REC_ASSUMPTION def_ppy_rec_assumption;
        memset(&def_ppy_rec_assumption, 0, sizeof(MONTE_CARLO_DEF_PPY_REC_ASSUMPTION));
        def_ppy_rec_assumption.correlation_type = 2;
        def_ppy_rec_assumption.global_correlation = 0.3;
        def_ppy_rec_assumption.ppy_type = PREPAY_CURVE_CPR;
        def_ppy_rec_assumption.ppy_pct = 0.2;
        def_ppy_rec_assumption.rec_vol = 0.2;
        def_ppy_rec_assumption.global_rec_correlation = 0.1;
        def_ppy_rec_assumption.asset_def_rec_correlation = 0.2;
        def_ppy_rec_assumption.copula = 0;
        def_ppy_rec_assumption.use_random_seed = true;
        def_ppy_rec_assumption.random_number_seed = 2;
        CHECK_API_RETURN_VALUE(pDeal, 0, set_monte_carlo_assumption(pDeal, &assumption, &def_ppy_rec_assumption));

        // Run monte carlo simulation
        CHECK_API_RETURN_VALUE(pDeal, 0, run_monte_carlo_simulation(pDeal));

        // Show average collateral cashflow
        printAverageCollatFlow(pDeal);
        // Show average bond cashflow
        printAverageBondFlow(pDeal);

        // Close deal.
        close_deal_ex(pDeal, pCmo);

        // Release a deal_scenario object.
        release_deal_scenario_object(&pDeal);

        std::cout << __FUNCTION__ << " succeed." << std::endl;
        return;

    } while(0);

    std::cout << __FUNCTION__ << " failed." << std::endl;
}

void run_monte_carlo_cdonet_example()
{
    std::cout << "\n=================" << __FUNCTION__ << "=================" << std::endl;

    void *pDeal = NULL;
    CMO_STRUCT cmo;
    CMO_STRUCT *pCmo = &cmo;

    memset(pCmo, 0, sizeof(*pCmo));
    strncpy(pCmo->dealid, "JUBILEE4", 20 - 1);

    do {
        // Set the preferred engine for overlapping deals.
        set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);

        // Create deal_scenario object.
        create_deal_scenario_object(&pDeal, NULL, NULL, NULL);

        // Open the specified deal.
        CHECK_API_RETURN_VALUE(pDeal, 0, open_deal_ex(pDeal, pCmo));

        // Show deal info.
        printDealInfo(pDeal);

        //Get global currency
        char global_currency[4];
        CHECK_API_RETURN_VALUE(pDeal, 0, get_global_currency(pDeal, global_currency));

        //Get currencies
        const int nCurrencies = get_currencies(pDeal, NULL);
        std::vector<char> curr_buf(nCurrencies*4); 
        std::vector<char*> currencies(nCurrencies);
        for(int i = 0; i < nCurrencies; ++i )
        {
            currencies[i] = &curr_buf[i*4];
        }
        CHECK_API_RETURN_VALUE(pDeal, nCurrencies, get_currencies(pDeal, &currencies.front()));

        //Set exchange rates and interest rates
        double exchange_rate_against_global_currency = 1.0;
        double pIndexRates[MAX_PERIODS] = {0};
        int RatesStatus[MAX_INDEX_TYPES_EX] = {0};
        for (int i = 0; i < nCurrencies; ++i)
        {
            CHECK_API_RETURN_VALUE(pDeal, 0, set_exchange_rate(pDeal, currencies[i], exchange_rate_against_global_currency));

            const int nRates=get_required_index_codes(pDeal, currencies[i], RatesStatus, MAX_INDEX_TYPES_EX);

            for (int j=0; j<nRates; ++j)
            {
                short idx(RatesStatus[j]);
                CHECK_API_RETURN_VALUE(pDeal, 0, set_index_rate(pDeal, currencies[i], &idx, MAX_PERIODS, pIndexRates));
            }
        }
        
        // Set edf and lgd
        double edfBase[] = {0.01, 0.02, 0.03, 0.04, 0.05};
        double recovery = 0.3;
        for (int i = 1; i <= pCmo->num_colls; ++i)
        {
            double edf[5] = {.0};
            for(int j = 0; j < 5; ++j)
            {
                edf[j] = edfBase[j] + (i / 10000.0);
            }
            
            CHECK_API_RETURN_VALUE(pDeal, 0, set_loan_edf(pDeal, NULL, i, edf, 5));
            CHECK_API_RETURN_VALUE(pDeal, 0, set_recoveries_ex(pDeal, 0, &recovery, i, true));
        }

        // Set simulation engine
        CHECK_API_RETURN_VALUE(pDeal, 0, set_simulation_engine(pDeal, SIMULATION_MONTE_CARLO));
        // Set monte carlo assumptions
        MONTE_CARLO_ASSUMPTION assumption;
        memset(&assumption, 0 , sizeof(MONTE_CARLO_ASSUMPTION));
        assumption.mode = 0;
        assumption.num_paths = 100;
        assumption.optimization_type = MC_OPTIMIZATION_NONE;
        assumption.optimization_pct = 0.0;
        assumption.reinv_pool = 1;
        assumption.default_to_reinv = 1;
        MONTE_CARLO_DEF_PPY_REC_ASSUMPTION def_ppy_rec_assumption;
        memset(&def_ppy_rec_assumption, 0, sizeof(MONTE_CARLO_DEF_PPY_REC_ASSUMPTION));
        def_ppy_rec_assumption.correlation_type = 0;
        def_ppy_rec_assumption.intra_industry_correlation = 0.3;
        def_ppy_rec_assumption.inter_industry_correlation = 0.3;
        def_ppy_rec_assumption.default_probability_source = ASSET_DEFAULT_PROBABILITY;
        def_ppy_rec_assumption.default_probability_multiplier = 1.0;
        def_ppy_rec_assumption.ppy_type = PREPAY_CURVE_CPR;
        def_ppy_rec_assumption.ppy_pct = 0.2;
        def_ppy_rec_assumption.rec_vol = 0.2;
        def_ppy_rec_assumption.global_rec_correlation = 0.1;
        def_ppy_rec_assumption.asset_def_rec_correlation = 0.2;
        def_ppy_rec_assumption.copula = 0;
        def_ppy_rec_assumption.use_random_seed = true;
        def_ppy_rec_assumption.random_number_seed = 2;
        CHECK_API_RETURN_VALUE(pDeal, 0, set_monte_carlo_assumption(pDeal, &assumption, &def_ppy_rec_assumption));

        // Run monte carlo simulation
        CHECK_API_RETURN_VALUE(pDeal, 0, run_monte_carlo_simulation(pDeal));

        // Show average collateral cashflow
        printAverageCollatFlow(pDeal);
        // Show average bond cashflow
        printAverageBondFlow(pDeal);

        // Close deal.
        close_deal_ex(pDeal, pCmo);

        // Release a deal_scenario object.
        release_deal_scenario_object(&pDeal);

        std::cout << __FUNCTION__ << " succeed." << std::endl;
        return;

    } while(0);

    std::cout << __FUNCTION__ << " failed." << std::endl;
}
