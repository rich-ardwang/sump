#include "utility.h"

void run_sedf_use_predefined_edf_example();
void run_sedf_use_custom_edf_example();


void run_sedf_example()
{
    run_sedf_use_predefined_edf_example();
    run_sedf_use_custom_edf_example();
}

void run_sedf_use_predefined_edf_example()
{
    std::cout << "\n============ Run sedf with predefined edf data example ===========" << std::endl;

    void *pDeal = NULL;
    CMO_STRUCT cmo;
    CMO_STRUCT *pCmo = &cmo;

    memset(pCmo, 0, sizeof(*pCmo));
    strncpy(pCmo->dealid, "ARIEL", 20 - 1);

    do {
        // Set the preferred engine for overlapping deals.
        set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);

        // Create deal_scenario object.
        create_deal_scenario_object(&pDeal, NULL, NULL, NULL);

        // Open the specified deal.
        CHECK_API_RETURN_VALUE(pDeal, 0, open_deal_ex(pDeal, pCmo));

        // Show deal info.
        printDealInfo(pDeal);

        // Set credit model type.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_moodys_credit_model_settings(pDeal, MOODYS_SEDF_SETTINGS, true));

        // Get SEDF scenarios.
        int scenario_count = get_edf_scenarios(pDeal, NULL);
        assert(scenario_count > 0);

        std::vector<char *> scenario_list(scenario_count);
        std::vector<char> scenario_strs(scenario_count*20);    // each scenario name may contains 20 characters(include '\0')

        for (int i = 0; i < scenario_count; ++i)
            scenario_list[i] = &scenario_strs[i*20];

        CHECK_API_RETURN_VALUE(pDeal, scenario_count, get_edf_scenarios(pDeal, &scenario_list[0]));

        // Show SEDF scenarios.
        std::cout << "\nSEDF scenarios: (" << scenario_count << " scenarios in total)" << std::endl;
        for (int i = 0; i < scenario_count; ++i)
        {
            std::cout << "Scenario " << i << ": " << scenario_list[i] << std::endl;
        }

        // Set SEDF scenario, scenario index is start from 0.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_current_edf_scenario(pDeal, 1));
        CHECK_API_RETURN_VALUE(pDeal, 1, get_current_edf_scenario(pDeal));

        // Set prepayment on deal level
        double ppy = 0.18 ;
        CHECK_API_RETURN_VALUE(pDeal, 0, set_prepayments_ex(pDeal, PREPAY_CURVE_CPR, 0, &ppy, -1, 0));
        // Set recovery lag with 8 for all loans
        CHECK_API_RETURN_VALUE(pDeal, 0, set_recovery_lag_ex(pDeal, 8, -1, NULL));

        // Run deal.
        std::cout << "Running deal......" << std::endl;
        CHECK_API_RETURN_VALUE(pDeal, 0, run_deal_ex(pDeal, pCmo));

        // Show collateral cashflow(the first 10 periods).
        printCollatFlow(pDeal, 10);
        // Show bond cashflow(the first 5 periods for each bond).
        printBondFlow(pDeal, 5);

        // Close deal.
        close_deal_ex(pDeal, pCmo);

        // Release a deal_scenario object.
        release_deal_scenario_object(&pDeal);

        std::cout << "run_sedf_use_predefined_edf_example() succeed." << std::endl;
        return;

    } while(0);

    std::cout << "run_sedf_use_predefined_edf_example() failed." << std::endl;
}

void run_sedf_use_custom_edf_example()
{
    std::cout << "\n============ Run sedf with custom edf data example ===========" << std::endl;

    void *pDeal = NULL;
    CMO_STRUCT cmo;
    CMO_STRUCT *pCmo = &cmo;

    memset(pCmo, 0, sizeof(*pCmo));
    strncpy(pCmo->dealid, "ARIEL", 20 - 1);

    do {
        // Set the preferred engine for overlapping deals.
        set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);

        // Create deal_scenario object.
        create_deal_scenario_object(&pDeal, NULL, NULL, NULL);

        // Open the specified deal.
        CHECK_API_RETURN_VALUE(pDeal, 0, open_deal_ex(pDeal, pCmo));

        // Show deal info.
        printDealInfo(pDeal);

        // Set credit model type.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_moodys_credit_model_settings(pDeal, MOODYS_SEDF_SETTINGS, true));

        // low default
        double lDefaults[] = {0.01,0.015,0.012,0.014,0.018};
        // hight default
        double hDefaults[] = {0.08,0.1,0.11,0.14,0.15};
        // Create pointer to custom EDF data
        double * edf_to_set;
        
        // Set custom EDF data by looping over all available collateral under the deal opened
        MARKIT_POOL_INFO* mpi = NULL;
        void* coll_it = obtain_collat_iterator(pDeal, 0);
        while (mpi = get_next_collat(pDeal, coll_it))
        {
            if ((mpi->ltv_cur) > 0.85)
                edf_to_set = hDefaults;
            else
                edf_to_set = lDefaults;

            CHECK_API_RETURN_VALUE(pDeal, 0, set_loan_edf(pDeal, NULL, mpi->loan_number, edf_to_set, 5));
        }

        // Set prepayment on deal level
        double ppy = 0.18 ;
        CHECK_API_RETURN_VALUE(pDeal, 0, set_prepayments_ex(pDeal, PREPAY_CURVE_CPR, 0, &ppy, -1, 0));
        // Set recovery lag with 8 for all loans
        CHECK_API_RETURN_VALUE(pDeal, 0, set_recovery_lag_ex(pDeal, 8, -1, NULL));

        // Run deal.
        std::cout << "Running deal......" << std::endl;
        CHECK_API_RETURN_VALUE(pDeal, 0, run_deal_ex(pDeal, pCmo));

        // Show collateral cashflow(the first 10 periods).
        printCollatFlow(pDeal, 10);
        // Show bond cashflow(the first 5 periods for each bond).
        printBondFlow(pDeal, 5);

        // Close deal.
        close_deal_ex(pDeal, pCmo);

        // Release a deal_scenario object.
        release_deal_scenario_object(&pDeal);

        std::cout << "run_sedf_use_custom_edf_example() succeed." << std::endl;
        return;

    } while(0);

    std::cout << "run_sedf_use_custom_edf_example() failed." << std::endl;
}
