#include "utility.h"

void printAverageBondFlow(void* tid);
void printAverageCollatFlow(void* tid);

void run_dpd_example()
{
    std::cout << "\n=================" << __FUNCTION__ << "=================" << std::endl;

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

        // Set economy settings as usual, e.g. exchange/interest rates, prepay, default, losses and recovery lag and so on.
        // e.g. set prepay:
        double CPR = .10;
        CHECK_API_RETURN_VALUE(pDeal, 0, set_prepayments_ex(pDeal, PREPAY_CURVE_CPR, 0, &CPR, -1, FALSE));
        
        // Set simulation engine
        CHECK_API_RETURN_VALUE(pDeal, 0, set_simulation_engine(pDeal, SIMULATION_DEFAULT_PROBABILITY_DISTRIBUTION));
        // Set dpd assumptions
        DPD_ASSUMPTION dpdAssumption;
        memset(&dpdAssumption, 0, sizeof(dpdAssumption));
        dpdAssumption.distribution = DPD_DISTRIBUTION_LOGNORMAL;
        dpdAssumption.scenario_type = 1;
        dpdAssumption.mean = 10.0;
        dpdAssumption.use_milan_aaa_ce = 0;
        dpdAssumption.milan_aaa_ce = 0.0;
        dpdAssumption.standard_deviation = 1.0;
        dpdAssumption.discounted_recoveries = 0;
        std::fill(dpdAssumption.revolving_default_factors, dpdAssumption.revolving_default_factors+500, 1.0);
        dpdAssumption.num_scenarios = 3;
        dpdAssumption.use_revolving_def_timing = 1;
        dpdAssumption.rating_cap_primary = 3;
        dpdAssumption.rating_cap_surveillance = 5;
        CHECK_API_RETURN_VALUE(pDeal, 0, set_dpd_assumption(pDeal, &dpdAssumption));

        // Set current default timing vector(required)
        double currentDefaultTimingVector[] = {0.01};
        CHECK_API_RETURN_VALUE(pDeal, 0, set_dpd_current_default_timing(pDeal, currentDefaultTimingVector, 1, 0));
        // Set revolving default timing vector if DPD_ASSUMPTION.use_revolving_def_timing has been set to '1'
        double revolvingDefaultTimingVector[] = {0.02};
        CHECK_API_RETURN_VALUE(pDeal, 0, set_dpd_revolving_default_timing(pDeal, revolvingDefaultTimingVector, 1, 0));

        // Run dpd simulation
        CHECK_API_RETURN_VALUE(pDeal, 0, run_default_probability_distribution(pDeal));

        DPD_RESULT result;
        CHECK_API_RETURN_VALUE(pDeal, 0, get_dpd_results(pDeal, "A1", &result));
        // assert(strcmp(result.tranche, "A1") == 0);
        // assert(strcmp(result.currency, "USD") == 0);
        // ...

        int scenariosNum = get_dpd_scenarios(pDeal, 0, 0);
        std::vector<DPD_SCENARIO> scenarios(scenariosNum);  // Generally, the total number of generated scenarios is equal to 'dpdAssumption.num_scenarios+1'
        CHECK_API_RETURN_VALUE(pDeal, scenariosNum, get_dpd_scenarios(pDeal, &scenarios[0], scenariosNum));
        // assert(fabs(0.13 - scenarios[2].default_loss) < 1e-5);
        // assert(fabs(0.01 - scenarios[2].scenario_probability) < 1e-5);
        // ...


        // Get path specified(0 for average, you can use get_collateral_flow_ex() and get_bond_flow_ex1() to get average cashflow as well) cashflow through api get_collateral_flow_sim() and get_bond_flow_sim().
        // get_dpd_scenarios(pDeal, 0, 0) will return the total path number.
        const int bondcfIds[] = {FLOW_BOND_BALANCE, FLOW_BOND_INTEREST, FLOW_BOND_PRINCIPAL, FLOW_BOND_INTEREST_DUE, FLOW_BOND_RATE, FLOW_BOND_PRINCIPAL_WRITEDOWN};
        const int collcfIds[] = {
            FLOW_COLLATERAL_BALANCE, FLOW_COLLATERAL_INTEREST, FLOW_COLLATERAL_PRINCIPAL, FLOW_COLLATERAL_SCHED_PRINCIPAL, FLOW_COLLATERAL_PREPAYMENTS,
            FLOW_COLLATERAL_DEFAULTS, FLOW_COLLATERAL_LOSSES, FLOW_COLLATERAL_LIQUIDATIONS, FLOW_COLLATERAL_REINVESTMENT, FLOW_COLLATERAL_INTEREST_OF_BUYS
        };
        const double *bondcf[sizeof(bondcfIds)/sizeof(bondcfIds[0])];
        const double *collcf[sizeof(collcfIds)/sizeof(collcfIds[0])];

        // Get average(path == 0) cashflow
        for (int i = 0; i < sizeof(collcfIds)/sizeof(collcfIds[0]); ++i)
        {
            collcf[i] = get_collateral_flow_sim(pDeal, 0, collcfIds[i]);
            assert(collcf[i] != NULL);
        }
        for (int i = 0; i < sizeof(bondcfIds)/sizeof(bondcfIds[0]); ++i)
        {
            bondcf[i] = get_bond_flow_sim(pDeal, 0, "A1", bondcfIds[i]);
            assert(bondcf[i] != NULL);
        }

        // Show average collateral cashflow(using get_collateral_flow_ex())
        printAverageCollatFlow(pDeal);
        // Show average bond cashflow(using get_bond_flow_ex1())
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
