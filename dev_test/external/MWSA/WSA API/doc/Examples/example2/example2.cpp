//=======================================================================
// Copyright (C) 2012 Moody’s Analytics, Inc., and/or its licensors and
// affiliates. All rights reserved.
//
//=======================================================================

#pragma warning(disable : 4996)

#include "wsa.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <list>
#include <map>
#include <fstream>


// input path
#ifdef _WIN32
const char* input_path="..\\DATA";
#else
const char* input_path="../DATA";
#endif

//Worker thread function
#ifdef _WIN32
DWORD WINAPI Worker(void* tid);
#else
void* Worker(void* tid);
#endif

//Util functions
std::string reportError(void* tid);
void PrintCollateralFlow(std::ofstream &fout,void* tid);


int main()
{

    //Set the path to deals.
    set_input_path(input_path);

    //Set the preferred engine for overlapping deals.
    set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS); //PICK_CHS_ENGINE_FOR_MAPPED_DEALS

    //Enable same deal multithreading (for CHS deals)
    enable_same_deal_multithreading(1);

    //Set up thread data
    const int numThreads=3;
    std::vector<void*> threadData(numThreads);
    //Create dso for each thread.
    for (int i=0; i<numThreads; ++i)
    {
        short LogAction = 2; //0
        char log[128]={0};
        sprintf(log,"thread_%d.log",i);
        BOOLYAN Debug = 1; //0
        create_deal_scenario_object(&threadData[i], &LogAction, log, &Debug);
    }

#ifdef WIN32
    std::vector<HANDLE > ths(numThreads);
    for(int i=0; i< numThreads; i++)
    {
        DWORD ti;
        ths[i] = CreateThread(0,0,Worker,threadData[i],0,&ti);
    }

    while(true)
    {
        DWORD ret = WaitForMultipleObjects(numThreads,&ths[0],TRUE,60000);
        if(ret == WAIT_TIMEOUT)
        {
        }
        else
        {
            break;
        }
    }
#else
    std::vector<pthread_t > ths(numThreads);
    for(int i=0; i< numThreads; i++)
    {
        pthread_create(&ths[i],NULL,Worker,threadData[i]);
    }
    for(int i=0; i< numThreads; i++)
	{
		pthread_join(ths[i],NULL);

	}
#endif

    //Release dso
    for(int i=0; i< numThreads; ++i)
    {
        release_deal_scenario_object(&threadData[i]);
    }
    return 0;
}


#ifdef WIN32
DWORD WINAPI Worker(void* tid)
#else
void* Worker(void* tid)
#endif
{
    static int iThreadid=0;
    int iMyThreadid=0;
    char resultfile[64]={0};
    int ret=0;
    CMO_STRUCT deal;
    
    iMyThreadid=iThreadid++;

    sprintf(resultfile,"%d.txt",iThreadid);
    std::ofstream fout(resultfile);
    if(!fout.good())
    {
        std::cerr << "Failed to open log file " << resultfile << std::endl;
        return 0;
    }
    memset(&deal, 0, sizeof(CMO_STRUCT));

    //Set deal name
    strcpy(deal.dealid, "ACE06NC1");
    //Set settlement date
    strcpy(deal.settlement_date, "08/31/11");
    //Set run actual collateral
    deal.actual_coll = 1;

    //Open deal
    ret = open_deal_ex(tid,&deal);

    if(ret < 0)
    {
        std::cerr <<"Thread:"<<iMyThreadid<< " Open deal fail:" << reportError(tid) << std::endl;
        return 0;
    }
    //Run deal
    ret=run_deal_ex(tid, &deal);
    if(ret < 0)
    {
        std::cerr <<"Thread:"<<iMyThreadid<< "Run deal fail:" << reportError(tid) << std::endl;
        close_deal_ex(tid,&deal);
        return 0;
    }
    //Print collateral cashflow to file.
    PrintCollateralFlow(fout,tid);

    //Close deal
    close_deal_ex(tid,&deal);
    return 0;
}

void PrintCollateralFlow(std::ofstream &fout,void* tid)
{
    bool add_info = get_deal_calc_level(tid) == CALC_LEVEL_FULL;
    MARKIT_COLLAT_CASHFLOW mccf;
    get_collateral_flow_ex1(tid,0,0,&mccf )	;

    const char sp = ',';
    fout << 
        "PERIOD,"
        "DATE,"
        "BALANCE," << 
        "SCHED_PRINCIPAL," <<
        "PREPAYMENTS," <<
        "DEFAULTS," << 
        "LOSSES," << 
        "PRIN RECOVERIES," <<
        "INTEREST," <<
        "REINVESTMENT," << 
        "COLLATERAL_CASH," ;
    if(add_info)
    {
        fout << 	
            "GROSS_INTEREST," <<
            "SCHED P&I ," 
            "NEG_AM " ;
    }
    fout << std::endl; 
    int start = mccf.start_index_as_per_settle_date;
    int end = mccf.size;
    for(int i=start; i<  end; i++)
    {
        fout << i << sp; 
        bool firstPoint = i == start;

        fout << mccf.dates[i] << sp;
        fout << mccf.balance[i] << sp;

        if(!firstPoint) fout << mccf.sched_principal[i] ;
        fout << sp;
        if(!firstPoint)fout << mccf.prepayments[i] ;
        fout << sp;
        if(!firstPoint)fout << mccf.defaults[i] ;
        fout << sp;
        if(!firstPoint)fout << mccf.losses[i];
        fout << sp;
        if(!firstPoint)fout << mccf.prin_recoveries[i] ;
        fout << sp;
        if(!firstPoint)fout << mccf.interest[i] ;
        fout << sp;
        if(!firstPoint)fout << mccf.reinvestment[i] ;
        fout << sp;
        if(!firstPoint) fout << mccf.cash[i] ;
        fout << sp;
        if(add_info)
        {
            if(!firstPoint) fout << mccf.gross_interest[i] ;
            fout << sp;
            if(!firstPoint) fout << mccf.sched_p_and_i[i] ;
            fout << sp;
            if(!firstPoint) fout << mccf.negative_amortization[i] ;
            fout << sp;
        }


        fout << std::endl;
    }
}
std::string reportError(void* tid)
{
	const char* error  = get_deal_error_msg(tid);
	return error ? std::string(error) : "";
}
