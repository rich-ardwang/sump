#include "utility.h"

void printDealInfo(void *tid)
{
    // Get deal information.
    MARKIT_DEAL_INFO deal_info={NON};
    get_deal_info(tid, 0, &deal_info);

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

void printAverageBondFlow(void* tid)
{
    MARKIT_DEAL_INFO deal_info = {NON};
    get_deal_info(tid, 0, &deal_info);

    for(int i = 1; i <= deal_info.num_bonds; ++i)
    {
        MARKIT_BOND_INFO bi;
        get_bond_info_by_index(tid, 0, i, &bi);
        MARKIT_BOND_CASHFLOW* mb_cf =  get_bond_flow_ex1(tid, 0, bi.tranche_name);
        std::cout << deal_info.deal_name << "," << bi.tranche_name << ", Size=" << mb_cf->size <<std::endl;

        if (mb_cf->size > 0)
        {
            const char sp = ',';
            std::cout.setf(std::ios::fixed);

            std::cout << "PERIOD,"
                << "DATE,"
                << "BALANCE,"
                << "INTEREST,"
                << "PRINCIPAL,"
                << "RATE,"
                << "WRITE_DOWN"
                << std::endl;

            if(mb_cf->start_index_as_per_settle_date > 0)
            {
                mb_cf->interest[mb_cf->start_index_as_per_settle_date] = 0;
                mb_cf->principal[mb_cf->start_index_as_per_settle_date] = 0;
                mb_cf->rate[mb_cf->start_index_as_per_settle_date] = 0;
                mb_cf->principal_writedown[mb_cf->start_index_as_per_settle_date] = 0;
            }

            for(int j = mb_cf->start_index_as_per_settle_date; j < mb_cf->size; ++j)
            {
                std::cout << j << sp
                    << mb_cf->dates[j] << sp
                    << mb_cf->balance[j] << sp
                    << mb_cf->interest[j] << sp
                    << mb_cf->principal[j] << sp
                    << mb_cf->rate[j] << sp
                    << mb_cf->principal_writedown[j] << sp
                    << std::endl;
            }
        }
    }
}

void printAverageCollatFlow(void* tid)
{
    MARKIT_DEAL_INFO deal_info = {NON};
    get_deal_info(tid, 0, &deal_info);

    MARKIT_COLLAT_CASHFLOW mccf;
    get_collateral_flow_ex1(tid, 0, 0, &mccf );
    double* interest_of_bought = get_collateral_flow_ex(tid, 0, FLOW_COLLATERAL_INTEREST_OF_BUYS);
    std::cout << deal_info.deal_name << ", Cashflow, Size=" << mccf.size << std::endl;	

    if (mccf.size > 0)
    {
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
            << "INTEREST_OF_BUYS,"
            << "COLLATERAL_CASH"
            << std::endl; 

        for(int i= 0; i < mccf.size; ++i)
        {
            std::cout << i << sp
                << mccf.dates[i] << sp
                << mccf.balance[i] << sp
                << mccf.sched_principal[i] << sp
                << mccf.prepayments[i] << sp
                << mccf.defaults[i] << sp
                << mccf.losses[i] << sp
                << mccf.prin_recoveries[i] << sp
                << mccf.interest[i] << sp
                << mccf.reinvestment[i] << sp
                << interest_of_bought[i] << sp
                << mccf.cash[i] << sp   // 'cash' is not available for DPD(always 0.0)
                << std::endl;
        }
    }
}