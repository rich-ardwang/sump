#include "utility.h"

void run_cmm_with_economy_settings_example();
void run_cmm_predefined_scenario_example();

void run_cmm_example()
{
    run_cmm_with_economy_settings_example();
    run_cmm_predefined_scenario_example();
}


void run_cmm_with_economy_settings_example()
{
    std::cout << "\n============ Run cmm with economy settings example ===========" << std::endl;

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

        // Set cmm credit model type.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_moodys_credit_model_settings(pDeal, MOODYS_CMM_SETTINGS, true));
      
        // Set required interest rates.
        double pRates[MAX_PERIODS] = { 0 };
        double ZERO_RATE[MAX_PERIODS] = { 0 };
        int Rates[MAX_INDEX_TYPES_EX] = { 0 };
        const int nRates = get_required_rate_codes(pDeal, Rates, MAX_INDEX_TYPES_EX);
        for (int i = 0; i < nRates; ++i)
        {
            short idx(Rates[i]);
            if (WAC == idx)
            {
                CHECK_API_RETURN_VALUE(pDeal, 0, set_rate_ex(pDeal, &idx, MAX_PERIODS, ZERO_RATE));
            }
            else if (idx)
            {
                CHECK_API_RETURN_VALUE(pDeal, 0, set_rate_ex(pDeal, &idx, MAX_PERIODS, pRates));
            }
        }
        // Set prepayments.
        double dCPRs[] = {0, 0.0018378,0.0008946,0.0005118,0.0007898,0.0009523,0.0006757,0.000754,0.0004566,0.0002117};
        CHECK_API_RETURN_VALUE(pDeal, 0, set_prepayments_ex(pDeal, PREPAY_CURVE_CPR, 10, dCPRs, -1, true));

        // Set defaults.
        double dMDRs = 0.005;
        CHECK_API_RETURN_VALUE(pDeal, 0, set_defaults_ex(pDeal, DEFAULT_CURVE_MDR, 0, &dMDRs, -1, true));
        
        // Set months from default to liquidation.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_recovery_lag_ex(pDeal, 3, -1, true)); // 3 month for all loans

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

        std::cout << "run_cmm_with_economy_settings_example() succeed." << std::endl;
        return;

    } while(0);
    std::cout << "run_cmm_with_economy_settings_example() failed." << std::endl;
} 

void run_cmm_predefined_scenario_example()
{
    std::cout << "\n============ Run cmm pre-defined scenario example ===========" << std::endl;

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

        // Set cmm credit model type.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_moodys_credit_model_settings(pDeal, MOODYS_CMM_SETTINGS, false));
       
        // Get cmm scenarios.
        int scenario_count = get_moodys_cmm_scenarios(pDeal, NULL, NULL);
        assert(scenario_count > 0);

        std::vector<char *> scenario_list(scenario_count);
        std::vector<char> scenario_strs(scenario_count*20);

        for (int i = 0; i < scenario_count; ++i)
            scenario_list[i] = &scenario_strs[i*20];

        CHECK_API_RETURN_VALUE(pDeal, scenario_count, get_moodys_cmm_scenarios(pDeal, NULL, &scenario_list[0]));

        // Show all cmm scenarios.
        std::cout << "\nCMM scenarios: (" << scenario_count << " scenarios in total)" << std::endl;
        for (int i = 0; i < scenario_count; ++i)
        {
            std::cout << "Scenario " << i << ": " << scenario_list[i] << std::endl;
        }

        //Get cmm default current scenario."MEDC_Base"
        char current_scenario[20] = {0};
        CHECK_API_RETURN_VALUE(pDeal,0, get_current_moodys_cmm_scenario(pDeal, NULL, current_scenario));
        std::cout << "CMM default current scenario:" << current_scenario << std::endl;

        //Set current cmm scenario "MEDC_S3" to run.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_current_moodys_cmm_scenario(pDeal, NULL, scenario_list[3]));
        CHECK_API_RETURN_VALUE(pDeal,0, get_current_moodys_cmm_scenario(pDeal, NULL, current_scenario));
        std::cout << "Set CMM current scenario:" << current_scenario << std::endl;

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

        std::cout << "run_cmm_predefined_scenario_example() succeed." << std::endl;
        return;

    } while(0);
    std::cout << "run_cmm_predefined_scenario_example() failed." << std::endl;
}


