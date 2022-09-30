#include "utility.h"

void run_pa_single_path_example();
void run_pa_cust_single_path_example();


void run_pa_example()
{
    run_pa_single_path_example();
    run_pa_cust_single_path_example();
}

void run_pa_single_path_example()
{
    std::cout << "\n============ Run pa single path example ===========" << std::endl;

    void *pDeal = NULL;
    CMO_STRUCT cmo;
    CMO_STRUCT *pCmo = &cmo;

    memset(pCmo, 0, sizeof(*pCmo));
    strncpy(pCmo->dealid, "AMEXCAMT", 20 - 1);

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
        CHECK_API_RETURN_VALUE(pDeal, 0, set_moodys_credit_model_settings(pDeal, MOODYS_PA_SETTINGS, true));

        // Set PA analysis type.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_analysis_type(pDeal, PA_MEDC_SINGLE_PATH));

        // Get PA scenarios.
        int scenario_count = get_pa_scenarios(pDeal, NULL);
        assert(scenario_count > 0);

        std::vector<char *> scenario_list(scenario_count);
        std::vector<char> scenario_strs(scenario_count*100);    // each scenario name may contains 100 characters(include '\0')

        for (int i = 0; i < scenario_count; ++i)
            scenario_list[i] = &scenario_strs[i*100];

        CHECK_API_RETURN_VALUE(pDeal, scenario_count, get_pa_scenarios(pDeal, &scenario_list[0]));

        // Show PA scenarios.
        std::cout << "\nPA scenarios: (" << scenario_count << " scenarios in total)" << std::endl;
        for (int i = 0; i < scenario_count; ++i)
        {
            std::cout << "Scenario " << i << ": " << scenario_list[i] << std::endl;
        }

        // Set PA scenario, scenario index is start from 0.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_current_pa_scenario(pDeal, 3));
        CHECK_API_RETURN_VALUE(pDeal, 3, get_current_pa_scenario(pDeal));

        // Set PA default pool data.
        
        // Default pool data for SFW deal.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "Rate30", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "Rate60", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "Rate90", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "CPR", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "CDR", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "Severity", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "PrincipalPayment", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "SellerPercentage", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "Yield", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "ChargeOff", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "RepaymentRate", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "DefermentRate", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "ForbearanceRate", "10"));

/*
        // Use the following settings for CHS deal, the pool data fields are different from SFW deal.
        // Default pool data for CHS deal.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "PurposePurchase", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "PurposeRefi", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "OccupancyOwner", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "OccupancySecondHome", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "OccupancyInvestor", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "Property1Unit", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "Property24Unit", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "OriginatorThirdParty", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "OriginatorRetail", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "HARP1", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "HARP2", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "FHA", "0"));
*/

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

        std::cout << "run_pa_single_path_example() succeed." << std::endl;
        return;

    } while(0);

    std::cout << "run_pa_single_path_example() failed." << std::endl;
}

void run_pa_cust_single_path_example()
{
    std::cout << "\n============ Run pa custom single path example ===========" << std::endl;

    void *pDeal = NULL;
    CMO_STRUCT cmo;
    CMO_STRUCT *pCmo = &cmo;

    memset(pCmo, 0, sizeof(*pCmo));
    strncpy(pCmo->dealid, "AMEXCAMT", 20 - 1);

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
        CHECK_API_RETURN_VALUE(pDeal, 0, set_moodys_credit_model_settings(pDeal, MOODYS_PA_SETTINGS, true));

        // Set PA analysis type.
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_analysis_type(pDeal, PA_CUST_MEDC_SINGLE_PATH));


        // Set PA default pool data.
        // Default pool data for SFW deal(so we need to provide the default settings, in case some pool data are missing).
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "Rate30", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "Rate60", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "Rate90", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "CPR", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "CDR", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "Severity", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "PrincipalPayment", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "SellerPercentage", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "Yield", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "ChargeOff", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "RepaymentRate", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "DefermentRate", "10"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "ForbearanceRate", "10"));

/*
        // Use the following settings for CHS deal, the pool data fields are different from SFW deal.
        // Default pool data for CHS deal(so we need to provide the default settings, in case some pool data are missing).
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "PurposePurchase", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "PurposeRefi", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "OccupancyOwner", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "OccupancySecondHome", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "OccupancyInvestor", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "Property1Unit", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "Property24Unit", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "OriginatorThirdParty", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "OriginatorRetail", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "HARP1", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "HARP2", "0"));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_default_pool_data(pDeal, "FHA", "0"));
*/

        // Set pa custom scenario.
        int year[]={2014,2014,2014,2014,2015,2015,2015,2015,2016,2016,2016,2016,2017,2017,2017,2017,2018,2018,2018};
        int quarter[]={1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3};
        double unemploy[] = {7.85,7.77,7.56,7.67,7.42,7.39,7.31,7.26,7.29,7.33,7.28,7.25,7.17,7.21,7.19,7.14,7.03,7.01,6.88};
        double hpi[] = {2.98,3.59,4.12,4.46,4.02,4.13,4.19,4.35,4.67,4.95,4.97,5.23,5.69,6.19,8.43,10.13,13.11,14.01,18.63};
        double gdp[] = {1.34,1.77,1.98,2.09,2.43,2.53,2.69,3.16,3.87,4.05,5.11,6.19,9.67,13.42,15.55,16.95,18.34,23.39,29.66};
        double try1y[] = {0.27,0.39,0.46,0.55,0.69,0.79,0.92,1.23,1.34,1.56,1.77,1.95,2.03,2.15,2.34,2.39,2.41,2.68,2.71};
        double try10y[] = {2.34,2.39,2.44,2.59,2.48,2.41,2.37,2.43,2.57,2.64,2.83,2.96,3.24,3.35,3.81,3.49,3.67,4.22,4.53};
        double libor6m[] = {7.85,7.77,7.56,7.67,7.42,7.39,7.31,7.26,7.29,7.33,7.28,7.25,7.17,7.21,7.19,7.14,7.03,7.01,6.88};

        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_custom_scenario(pDeal, "UNEMPLOYMENT", year, quarter, unemploy, 19));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_custom_scenario(pDeal, "HPI", year, quarter, hpi, 19));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_custom_scenario(pDeal, "GDP", year, quarter, gdp, 19));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_custom_scenario(pDeal, "TSY1Y", year, quarter, try1y, 19));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_custom_scenario(pDeal, "TSY10Y", year, quarter, try10y, 19));
        CHECK_API_RETURN_VALUE(pDeal, 0, set_pa_custom_scenario(pDeal, "LIBOR6MSPREAD", year, quarter, libor6m, 19));

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

        std::cout << "run_pa_cust_single_path_example() succeed." << std::endl;
        return;

    } while(0);

    std::cout << "run_pa_cust_single_path_example() failed." << std::endl;
}
