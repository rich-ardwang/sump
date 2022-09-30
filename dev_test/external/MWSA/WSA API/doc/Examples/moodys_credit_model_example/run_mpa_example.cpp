#include "utility.h"

void run_mpa_single_path_example();
void run_mpa_cust_single_path_example();
void run_mpa_loss_simulation_example();
void run_mpa_loss_simulation_custom_scenario_example();


void run_mpa_example()
{
    run_mpa_single_path_example();
    run_mpa_cust_single_path_example();
    run_mpa_loss_simulation_example();
    run_mpa_loss_simulation_custom_scenario_example();
}

void run_mpa_single_path_example()
{
    std::cout << "\n============ Run mpa single path example ===========" << std::endl;

    void *pDeal = NULL;
    CMO_STRUCT cmo;
    CMO_STRUCT *pCmo = &cmo;

    memset(pCmo, 0, sizeof(*pCmo));
    strncpy(pCmo->dealid, "ACE06NC1", 20 - 1);

    do {
        // Set the preferred engine for overlapping deals.
        set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);

        // Create deal_scenario object.
        create_deal_scenario_object(&pDeal, NULL, NULL, NULL);

        // Open the specified deal.
        CHECK_API_RETURN_VALUE(pDeal, 0, open_deal_ex(pDeal, pCmo));

        // Show deal info.
        printDealInfo(pDeal);

        // Set credit model type.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, true));

        // Set MPA analysis type.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_analysis_type(pDeal, MPA_MEDC_SINGLE_PATH));

        // Get MPA scenarios.
        int scenario_count = get_mpa_scenarios(pDeal, NULL);
        assert(scenario_count > 0);

        std::vector<char *> scenario_list(scenario_count);
        std::vector<char> scenario_strs(scenario_count*100);    // each scenario name may contains 100 characters(include '\0')

        for (int i = 0; i < scenario_count; ++i)
            scenario_list[i] = &scenario_strs[i*100];
        
        CHECK_API_RETURN_VALUE(pDeal, scenario_count, get_mpa_scenarios(pDeal, &scenario_list[0]));

        // Show MPA scenarios.
        std::cout << "\nMPA scenarios: (" << scenario_count << " scenarios in total)" << std::endl;
        for (int i = 0; i < scenario_count; ++i)
        {
            std::cout << "Scenario " << i << ": " << scenario_list[i] << std::endl;
        }

        // Set MPA scenario, scenario index is start from 0.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_current_mpa_scenario(pDeal, 3));
        CHECK_API_RETURN_VALUE(pDeal, 3, get_current_mpa_scenario(pDeal));

        // Set MPA haircut.
        double haircut_vector[] = {3.0, 4.0};
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_haircut(pDeal, sizeof(haircut_vector)/sizeof(haircut_vector[0]), haircut_vector, 0));

        // Set recovery lag by state(judicial/non-judicial).
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_recovery_lag_by_state(pDeal, 30, 35));

        // Set recovery lag on loan 1.
        int recoveryLag = 10;
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_recovery_lag(pDeal, 0, &recoveryLag, 1));

        // Set MPA multipliers.
        double ppy_multiplier = 2.0;
        double def_multiplier = 3.0;
        double sev_multiplier = 4.0;
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_multiplier(pDeal, MPA_MULTIPLIER_PREPAY, 0, &ppy_multiplier, -1));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_multiplier(pDeal, MPA_MULTIPLIER_DEFAULT, 0, &def_multiplier, -1));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_multiplier(pDeal, MPA_MULTIPLIER_SEVERITY, 0, &sev_multiplier, -1));

        // Set MPA simulation length.
        CHECK_API_RETURN_VALUE(pDeal, 200, set_mpa_simulation_length(pDeal, 200));

        // Set use mid course adjustment data.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_mid_course_adj(pDeal, true));

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

        std::cout << "run_mpa_single_path_example() succeed." << std::endl;
        return;

    } while(0);

    std::cout << "run_mpa_single_path_example() failed." << std::endl;
}

void run_mpa_cust_single_path_example()
{
    std::cout << "\n============ Run mpa custom single path example ===========" << std::endl;

    void *pDeal = NULL;
    CMO_STRUCT cmo;
    CMO_STRUCT *pCmo = &cmo;

    memset(pCmo, 0, sizeof(*pCmo));
    strncpy(pCmo->dealid, "ACE06NC1", 20 - 1);

    do {
        // Set the preferred engine for overlapping deals.
        set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);

        // Create deal_scenario object.
        create_deal_scenario_object(&pDeal, NULL, NULL, NULL);

        // Open the specified deal.
        CHECK_API_RETURN_VALUE(pDeal, 0, open_deal_ex(pDeal, pCmo));

        // Show deal info.
        printDealInfo(pDeal);

        // Set credit model type.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, true));

        // Set MPA analysis type.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_analysis_type(pDeal, MPA_CUST_MEDC_SINGLE_PATH));


        // Set MPA custom scenario data.
        int year[] = {2014,2014,2014,2014,2015,2015,2015,2015,2016,2016,2016,2016,2017,2017,2017,2017,2018,2018,2018,2018};
        int quarter[] = {1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4};

        double UsUemployment_value[] = {7.89,7.85,7.77,7.56,7.67,7.42,7.39,7.31,7.26,7.29,7.33,7.28,7.25,7.17,7.21,7.19,7.14,7.03,7.01,6.88};
        double UsHPI_value[] = {2.34,2.98,3.59,4.12,4.46,4.02,4.13,4.19,4.35,4.67,4.95,4.97,5.23,5.69,6.19,8.43,10.13,13.11,14.01,18.63};
        double UsGDP_value[] = {0.99,1.34,1.77,1.98,2.09,2.43,2.53,2.69,3.16,3.87,4.05,5.11,6.19,9.67,13.42,15.55,16.95,18.34,23.39,29.66};
        double UsT1yRate_value[] = {0.23,0.27,0.39,0.46,0.55,0.69,0.79,0.92,1.23,1.34,1.56,1.77,1.95,2.03,2.15,2.34,2.39,2.41,2.68,2.71};
        double UsT10yRate_value[] = {2.25,2.34,2.39,2.44,2.59,2.48,2.41,2.37,2.43,2.57,2.64,2.83,2.96,3.24,3.35,3.81,3.49,3.67,4.22,4.53};
        double Libor6mSpread_value[] = {0.62,0.61,0.63,0.78,0.96,1.26,1.35,1.67,1.69,1.75,1.86,1.92,2.11,2.23,2.42,2.53,2.67,2.92,3.32,3.36};
        double STATE_GDP_value[] = {1.7,2.3,2.5,3.4,3.9,5.1,5.9,6.7,8.3,9.2,9.8,10.1,10.6,13.2,15.7,18.4,19.2,23.1,30.3,35.23};
        double FREDMAC_value[] = {4.15,4.28,4.55,4.45,5.02,5.11,5.13,5.26,5.27,5.39,5.66,6.03,6.17,6.52,6.77,7.13,7.22,7.52,7.53,7.55};

        double REG_UsHPI_value_MABI[] = {-1.03,-0.59,1.15,2.57,4.68,9.16};
        double REG_UsHPI_value_AK[] = {2.18,2.34,2.97,3.15,3.23,3.56,4.67,4.92,5.12,6.19,8.33,10.53,13.52,16.41,18.96,23.11,29.1,33.15,35.15,39.11};

        double REG_UsUemployment_value_AK[] = {6.54,6.38,6.24,6.11,6.07,5.99};
        double REG_UsUemployment_value_MNEY[] = {8.36,8.23,8.21,8.19,8.1,8.06,7.95,7.91,7.83,7.61,7.53,7.39,7.34,7.27,7.13,7.16,7.26,7.33,7.18,7.02};

        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "UNEMPLOYMENT", "US", year, quarter, UsUemployment_value, 20));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "HPI", "US", year, quarter, UsHPI_value, 20));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "GDP", "US", year, quarter, UsGDP_value, 20));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "TSY1Y", "US", year, quarter, UsT1yRate_value, 20));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "TSY10Y", "US", year, quarter, UsT10yRate_value, 20));

        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "REG_UNEMPLOYMENT", "AK", year, quarter, REG_UsUemployment_value_AK, 6));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "REG_UNEMPLOYMENT", "MNEY", year, quarter, REG_UsUemployment_value_MNEY, 20));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "REG_HPI", "MABI", year, quarter, REG_UsHPI_value_MABI, 6));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "REG_HPI", "AK", year, quarter, REG_UsHPI_value_AK, 20));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "STATE_GDP" , "NY", year, quarter, STATE_GDP_value, 20));

        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "LIBOR6MSPREAD", "US", year, quarter, Libor6mSpread_value, 20));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "FREDMAC", "US", year, quarter, FREDMAC_value, 20));

        // Set MPA haircut.
        double haircut_vector[] = {3.0, 4.0};
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_haircut(pDeal, sizeof(haircut_vector)/sizeof(haircut_vector[0]), haircut_vector, 0));

        // Set recovery lag by state(judicial/non-judicial).
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_recovery_lag_by_state(pDeal, 30, 35));

        // Set recovery lag on loan 1.
        int recoveryLag = 10;
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_recovery_lag(pDeal, 0, &recoveryLag, 1));

        // Set MPA multipliers.
        double ppy_multiplier = 2.0;
        double def_multiplier = 3.0;
        double sev_multiplier = 4.0;
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_multiplier(pDeal, MPA_MULTIPLIER_PREPAY, 0, &ppy_multiplier, -1));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_multiplier(pDeal, MPA_MULTIPLIER_DEFAULT, 0, &def_multiplier, -1));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_multiplier(pDeal, MPA_MULTIPLIER_SEVERITY, 0, &sev_multiplier, -1));

        // Set MPA simulation length.
        CHECK_API_RETURN_VALUE(pDeal, 200, set_mpa_simulation_length(pDeal, 200));

        // Set use mid course adjustment data.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_mid_course_adj(pDeal, true));

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

        std::cout << "run_mpa_cust_single_path_example() succeed." << std::endl;
        return;

    } while(0);

    std::cout << "run_mpa_cust_single_path_example() failed." << std::endl;
}

void run_mpa_loss_simulation_example()
{
    std::cout << "\n============ Run mpa loss simulation(pre-defined scenario) example ===========" << std::endl;

    void *pDeal = NULL;
    CMO_STRUCT cmo;
    CMO_STRUCT *pCmo = &cmo;

    memset(pCmo, 0, sizeof(*pCmo));
    strncpy(pCmo->dealid, "ACE06NC1", 20 - 1);

    do {
        // Set the preferred engine for overlapping deals.
        set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);

        // Create deal_scenario object.
        create_deal_scenario_object(&pDeal, NULL, NULL, NULL);

        // Open the specified deal.
        CHECK_API_RETURN_VALUE(pDeal, 0, open_deal_ex(pDeal, pCmo));

        // Show deal info.
        printDealInfo(pDeal);

        // Set credit model type.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, true));

        // Set MPA analysis type.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_analysis_type(pDeal, MPA_LOSS_SIMULATION));

        // Set MPA simulation path number.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_simulation_path_num(pDeal, 5));    // 5 path, default path number is 10000

        // Set MPA haircut.
        double haircut_vector[] = {3.0, 4.0};
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_haircut(pDeal, sizeof(haircut_vector)/sizeof(haircut_vector[0]), haircut_vector, 0));

        // Set recovery lag by state(judicial/non-judicial).
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_recovery_lag_by_state(pDeal, 30, 35));

        // Set recovery lag on loan 1.
        int recoveryLag = 10;
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_recovery_lag(pDeal, 0, &recoveryLag, 1));

        // Set MPA multipliers.
        double ppy_multiplier = 2.0;
        double def_multiplier = 3.0;
        double sev_multiplier = 4.0;
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_multiplier(pDeal, MPA_MULTIPLIER_PREPAY, 0, &ppy_multiplier, -1));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_multiplier(pDeal, MPA_MULTIPLIER_DEFAULT, 0, &def_multiplier, -1));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_multiplier(pDeal, MPA_MULTIPLIER_SEVERITY, 0, &sev_multiplier, -1));

        // Set MPA simulation length.
        CHECK_API_RETURN_VALUE(pDeal, 200, set_mpa_simulation_length(pDeal, 200));

        // Set use mid course adjustment data.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_mid_course_adj(pDeal, true));

        // Run deal.
        std::cout << "Running deal......" << std::endl;
        CHECK_API_RETURN_VALUE(pDeal, 0, run_deal_ex(pDeal, pCmo));

        // Show average collateral cashflow(the first 10 periods).
        printCollatFlow(pDeal, 10);
        // Show average bond cashflow(the first 5 periods for each bond).
        printBondFlow(pDeal, 5);

        // Close deal.
        close_deal_ex(pDeal, pCmo);

        // Release a deal_scenario object.
        release_deal_scenario_object(&pDeal);

        std::cout << __FUNCTION__ << "() succeed." << std::endl;
        return;

    } while(0);

    std::cout << __FUNCTION__ << "() failed." << std::endl;
}

void run_mpa_loss_simulation_custom_scenario_example()
{
    std::cout << "\n============ Run mpa loss simulation(customized scenario) example ===========" << std::endl;

    void *pDeal = NULL;
    CMO_STRUCT cmo;
    CMO_STRUCT *pCmo = &cmo;

    memset(pCmo, 0, sizeof(*pCmo));
    strncpy(pCmo->dealid, "ACE06NC1", 20 - 1);

    do {
        // Set the preferred engine for overlapping deals.
        set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);

        // Create deal_scenario object.
        create_deal_scenario_object(&pDeal, NULL, NULL, NULL);

        // Open the specified deal.
        CHECK_API_RETURN_VALUE(pDeal, 0, open_deal_ex(pDeal, pCmo));

        // Show deal info.
        printDealInfo(pDeal);

        // Set credit model type.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, true));

        // Set MPA analysis type.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_analysis_type(pDeal, MPA_CUST_MEDC_SIMULATION));

        // Set MPA simulation path number.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_simulation_path_num(pDeal, 5));    // 5 path, default path number is 10000

        // Set MPA custom scenario data.
        int year[] = {2014,2014,2014,2014,2015,2015,2015,2015,2016,2016,2016,2016,2017,2017,2017,2017,2018,2018,2018,2018};
        int quarter[] = {1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4};

        double UsUemployment_value[] = {7.89,7.85,7.77,7.56,7.67,7.42,7.39,7.31,7.26,7.29,7.33,7.28,7.25,7.17,7.21,7.19,7.14,7.03,7.01,6.88};
        double UsHPI_value[] = {2.34,2.98,3.59,4.12,4.46,4.02,4.13,4.19,4.35,4.67,4.95,4.97,5.23,5.69,6.19,8.43,10.13,13.11,14.01,18.63};
        double UsGDP_value[] = {0.99,1.34,1.77,1.98,2.09,2.43,2.53,2.69,3.16,3.87,4.05,5.11,6.19,9.67,13.42,15.55,16.95,18.34,23.39,29.66};
        double UsT1yRate_value[] = {0.23,0.27,0.39,0.46,0.55,0.69,0.79,0.92,1.23,1.34,1.56,1.77,1.95,2.03,2.15,2.34,2.39,2.41,2.68,2.71};
        double UsT10yRate_value[] = {2.25,2.34,2.39,2.44,2.59,2.48,2.41,2.37,2.43,2.57,2.64,2.83,2.96,3.24,3.35,3.81,3.49,3.67,4.22,4.53};
        double Libor6mSpread_value[] = {0.62,0.61,0.63,0.78,0.96,1.26,1.35,1.67,1.69,1.75,1.86,1.92,2.11,2.23,2.42,2.53,2.67,2.92,3.32,3.36};
        double STATE_GDP_value[] = {1.7,2.3,2.5,3.4,3.9,5.1,5.9,6.7,8.3,9.2,9.8,10.1,10.6,13.2,15.7,18.4,19.2,23.1,30.3,35.23};
        double FREDMAC_value[] = {4.15,4.28,4.55,4.45,5.02,5.11,5.13,5.26,5.27,5.39,5.66,6.03,6.17,6.52,6.77,7.13,7.22,7.52,7.53,7.55};

        double REG_UsHPI_value_MABI[] = {-1.03,-0.59,1.15,2.57,4.68,9.16};
        double REG_UsHPI_value_AK[] = {2.18,2.34,2.97,3.15,3.23,3.56,4.67,4.92,5.12,6.19,8.33,10.53,13.52,16.41,18.96,23.11,29.1,33.15,35.15,39.11};

        double REG_UsUemployment_value_AK[] = {6.54,6.38,6.24,6.11,6.07,5.99};
        double REG_UsUemployment_value_MNEY[] = {8.36,8.23,8.21,8.19,8.1,8.06,7.95,7.91,7.83,7.61,7.53,7.39,7.34,7.27,7.13,7.16,7.26,7.33,7.18,7.02};

        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "UNEMPLOYMENT", "US", year, quarter, UsUemployment_value, 20));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "HPI", "US", year, quarter, UsHPI_value, 20));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "GDP", "US", year, quarter, UsGDP_value, 20));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "TSY1Y", "US", year, quarter, UsT1yRate_value, 20));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "TSY10Y", "US", year, quarter, UsT10yRate_value, 20));

        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "REG_UNEMPLOYMENT", "AK", year, quarter, REG_UsUemployment_value_AK, 6));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "REG_UNEMPLOYMENT", "MNEY", year, quarter, REG_UsUemployment_value_MNEY, 20));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "REG_HPI", "MABI", year, quarter, REG_UsHPI_value_MABI, 6));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "REG_HPI", "AK", year, quarter, REG_UsHPI_value_AK, 20));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "STATE_GDP" , "NY", year, quarter, STATE_GDP_value, 20));

        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "LIBOR6MSPREAD", "US", year, quarter, Libor6mSpread_value, 20));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_custom_scenario(pDeal, "FREDMAC", "US", year, quarter, FREDMAC_value, 20));

        // Set MPA haircut.
        double haircut_vector[] = {3.0, 4.0};
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_haircut(pDeal, sizeof(haircut_vector)/sizeof(haircut_vector[0]), haircut_vector, 0));

        // Set recovery lag by state(judicial/non-judicial).
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_recovery_lag_by_state(pDeal, 30, 35));

        // Set recovery lag on loan 1.
        int recoveryLag = 10;
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_recovery_lag(pDeal, 0, &recoveryLag, 1));

        // Set MPA multipliers.
        double ppy_multiplier = 2.0;
        double def_multiplier = 3.0;
        double sev_multiplier = 4.0;
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_multiplier(pDeal, MPA_MULTIPLIER_PREPAY, 0, &ppy_multiplier, -1));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_multiplier(pDeal, MPA_MULTIPLIER_DEFAULT, 0, &def_multiplier, -1));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_multiplier(pDeal, MPA_MULTIPLIER_SEVERITY, 0, &sev_multiplier, -1));

        // Set MPA simulation length.
        CHECK_API_RETURN_VALUE(pDeal, 200, set_mpa_simulation_length(pDeal, 200));

        // Set use mid course adjustment data.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_mpa_mid_course_adj(pDeal, true));

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

        std::cout << __FUNCTION__ << "() succeed." << std::endl;
        return;

    } while(0);

    std::cout << __FUNCTION__ << "() failed." << std::endl;
}
