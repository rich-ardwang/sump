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

template <typename T>
T safe_value(T* array, int i) 
{
    return array ? array[i] : 0;
}

void printBondFlow(void* tid, int periods)
{
    MARKIT_DEAL_INFO deal_info = {NON};
    get_deal_info(tid, 0, &deal_info);

    for(int i = 1; i <= deal_info.num_bonds; ++i)
    {
        MARKIT_BOND_INFO bi;
        get_bond_info_by_index(tid, 0, i, &bi);
        MARKIT_BOND_CASHFLOW* mb_cf =  get_bond_flow_ex1(tid, 0, bi.tranche_name);
        std::cout << deal_info.deal_name << "," << bi.tranche_name<<std::endl;
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
            << "WRITE_DOWN" ;

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

        for(int j = mb_cf->start_index_as_per_settle_date; j < mb_cf->size; ++j)
        {
            if (periods != -1 && j - mb_cf->start_index_as_per_settle_date >= periods)   // only show the first a few periods of cashflow for example
                break;

            std::cout << j << sp
                << mb_cf->dates[j] << sp
                << mb_cf->accrual_begin_dates[j] << sp
                << mb_cf->balance[j] << sp
                << mb_cf->interest[j] << sp
                << mb_cf->principal[j] << sp
                << mb_cf->rate[j] << sp
                << safe_value(mb_cf->cash, j) << sp
                << safe_value(mb_cf->principal_writedown, j) << sp;
            if(add_info)
            {
                std::cout << safe_value(mb_cf->interest_due,j) << sp
                    << safe_value(mb_cf->interest_shortfall,j) << sp
                    << safe_value(mb_cf->accrual_days,j) << sp
                    << safe_value(mb_cf->uncapped_rate,j) << sp
                    << mb_cf->hist_flag[j] << sp;
            }
            std::cout << std::endl;
        }
    }
}

void printCollatFlow(void* tid, int periods)
{
    MARKIT_DEAL_INFO deal_info = {NON};
    get_deal_info(tid, 0, &deal_info);

    const int MAX_GROUPS = 20;
    std::vector<int> groups(MAX_GROUPS);
    int total_groups =  0;
    const int grps_returned = view_coll_groups(tid, &groups[0], MAX_GROUPS , &total_groups);
    groups.resize(grps_returned>= 0?grps_returned:0);
    for(size_t i= 0; i < groups.size(); ++i)
    {
        int grp = groups[i];
        std::cout << deal_info.deal_name << "," << "Collat Group " << grp << ",Cashflow" << std::endl;						
        MARKIT_COLLAT_CASHFLOW mccf;
        get_collateral_flow_ex1(tid,grp,0,&mccf );

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
            << "SCHED P&I,"
            << "NEG_AM" ;

        std::cout << std::endl; 

        for(int i= 0; i < mccf.size; ++i)
        {
            if (periods != -1 && i >= periods)   // only show the first a few periods of cashflow for example
                break;

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
                << mccf.cash[i] << sp
                << mccf.gross_interest[i] << sp
                << mccf.sched_p_and_i[i] << sp
                << mccf.negative_amortization[i] << sp;

            std::cout << std::endl;
        }
    }
}
