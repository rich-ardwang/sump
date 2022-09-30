//=======================================================================
// Copyright (C) 2013 Moody's Analytics, Inc., and/or its licensors and
// affiliates. All rights reserved.
//
//=======================================================================

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Runtime.InteropServices;

using MWSA;

namespace DotNetExample
{
    class Program
    {
        static void Main(string[] args)
        {
            TestExample.initialize();

            int testNumber = 0;
            string titleLine = "-------------------";

            System.Console.WriteLine(titleLine + "Test #" + ++testNumber + ":Run deal with simple scenario" + titleLine);
            TestExample.RunDealSimple();

            System.Console.WriteLine(titleLine + "Test #" + ++testNumber + ":Run deal with trigger setting" + titleLine);
            TestExample.RunDealTrigger();

            System.Console.WriteLine(titleLine + "Test #" + ++testNumber + ":Run deal with call option" + titleLine);
            TestExample.RunDealCall();

            System.Console.WriteLine("\n" + titleLine + "Test #" + ++testNumber + ":Run deal multi-threaded" + titleLine);
            TestExample.RunDealWithMutipleThread();

            System.Console.WriteLine("\n" + titleLine + "Test #" + ++testNumber + ":Run deal with callbacks" + titleLine);
            TestExample.RunDealWithCallbacks();

            System.Console.WriteLine("\n" + titleLine + "Test #" + ++testNumber + ":Run deal with dplc model" + titleLine);
            TestExample.RunDealWithDplcModel();

            System.Console.WriteLine("\n" + titleLine + "Test #" + ++testNumber + ":Run deal with cmm model" + titleLine);
            TestExample.RunDealWithCMMModel();

#if __ENABLE_MPA_SINGLE_PATH__
            System.Console.WriteLine("\n" + titleLine + "Test #" + ++testNumber + ":Run deal with mpa single path" + titleLine);
            TestExample.RunDealWithMPASinglePath();
#endif

#if __ENABLE_MPA_CUSTOM_SINGLE_PATH__
            System.Console.WriteLine("\n" + titleLine + "Test #" + ++testNumber + ":Run deal with mpa custom single path" + titleLine);
            TestExample.RunDealWithMPACustomSinglePath();
#endif

#if __ENABLE_MPA_LOSS_SIMULATION__
            System.Console.WriteLine("\n" + titleLine + "Test #" + ++testNumber + ":Run deal with mpa loss simulation" + titleLine);
            TestExample.RunDealWithMPALossSimulation();
#endif

#if __ENABLE_MPA_LOSS_SIMULATION_CUSTOM_SCENARIO__
            System.Console.WriteLine("\n" + titleLine + "Test #" + ++testNumber + ":Run deal with mpa loss simulation with custom scenario" + titleLine);
            TestExample.RunDealWithMPALossSimulationCustomScenario();
#endif

#if __ENABLE_PA_SINGLE_PATH__
            System.Console.WriteLine("\n" + titleLine + "Test #" + ++testNumber + ":Run deal with pa single path" + titleLine);
            TestExample.RunDealWithPASinglePath();
#endif

#if __ENABLE_PA_CUSTOM_SINGLE_PATH__
            System.Console.WriteLine("\n" + titleLine + "Test #" + ++testNumber + ":Run deal with pa custom single path" + titleLine);
            TestExample.RunDealWithPACustomSinglePath();
#endif

            System.Console.WriteLine("\n" + titleLine + "Test #" + ++testNumber + ":Run CDOnet deal" + titleLine);
            TestExample.RunCDOnetDeal();
            System.Console.WriteLine("\n" + titleLine + "Test #" + ++testNumber + ":Run monte carlo with SFW deal" + titleLine);
            TestExample.RunMonteCarloSFW();
            System.Console.WriteLine("\n" + titleLine + "Test #" + ++testNumber + ":Run monte carlo with CDOnet deal" + titleLine);
            TestExample.RunMonteCarloCDOnet();
            System.Console.WriteLine("\n" + titleLine + "Test #" + ++testNumber + ":Run DPD(default probability distribution)" + titleLine);
            TestExample.RunDPDSimulation();
            System.Console.WriteLine("\n" + titleLine + "Test #" + ++testNumber + "::Run deal with sedf model" + titleLine);
            TestExample.RunDealWithSedfModel();
            System.Console.WriteLine("\n\n" + titleLine + "All test finished." + titleLine);
        }
    }

    class TestExample
    {
        public static void RunDealSimple()
        {
            // Create deal_scenario object.
            Deal dso = SDK.CreateDeal();

            try
            {
                dso.OpenDeal("ACE06NC1", new DateTime(2011, 8, 31), true);
                int propagate_to_remics = 1;
                dso.SetDealCalcLevel(CALC_LEVEL.CALC_LEVEL_FULL, propagate_to_remics);

                Double[] ppy_rate = { 0.0 };
                dso.SetPrepaymentsOnAllLoans(PrepaymentCurves.SMM, ppy_rate);

                dso.RunDeal();

                int threadId = Thread.CurrentThread.GetHashCode();
                ReportCollateralFlow(dso, "RunDealSimple_coll_" + threadId + ".csv");
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            finally
            {
                String sdkErrorMsg = dso.GetDealErrorMessage();
                if (sdkErrorMsg != null)
                {
                    Console.WriteLine(sdkErrorMsg);
                }
                // Close deal
                dso.CloseDeal();
                // Release deal scenario object
                dso.Dispose();
            }
        }

        public static void RunDealTrigger()
        {
            // Create deal_scenario object.
            Deal dso = SDK.CreateDeal();

            try
            {
                dso.OpenDeal("MECENA2007", new DateTime(2011, 8, 31), true);
                int propagate_to_remics = 1;
                dso.SetDealCalcLevel(CALC_LEVEL.CALC_LEVEL_FULL, propagate_to_remics);

                Double[] ppy_rate = { 0.0 };
                dso.SetPrepaymentsOnAllLoans(PrepaymentCurves.SMM, ppy_rate);

                // set trigger override
                const int TRG_NAME_BUF_SIZE = 21;
                const int TRG_DESC_BUF_SIZE = 1025;
                const int OVERRIDE_CODE = 2; //0 for no override, 1 for Always "NO" and 2 for Always "YES"
                const int OVERRIDE_DATE = 20141125;

                string[] vecMasterNames;
                string[] vecMasterDescs;
                int[] vecNumSubTriggers;

                int masterTriggersNum = dso.ListMasterTriggers(null, out vecMasterNames, out vecMasterDescs, out vecNumSubTriggers);

                if (masterTriggersNum > 0)
                {
                    // loop all high level triggers
                    for (int highTrigIdx = 0; highTrigIdx < masterTriggersNum; ++highTrigIdx)
                    {
                        string masterTrigName = vecMasterNames[highTrigIdx];
                        int subTriggersNum = vecNumSubTriggers[highTrigIdx];

                        // get master trigger info
                        string[] vecSubNames;
                        string[] vecSubDescs;
                        sbyte[] status;
                        string subTriggerLogic;

                        subTriggersNum = dso.GetMasterTriggerInfo(null, masterTrigName, out status, out subTriggerLogic, out vecSubNames, out vecSubDescs);

                        // loop all sub triggers, set overide
                        for (int subTrigIdx = 0; subTrigIdx < subTriggersNum; ++subTrigIdx)
                        {
                            int ret = dso.SetTriggerOverride(null, vecSubNames[subTrigIdx], OVERRIDE_CODE, OVERRIDE_DATE);
                        }
                    }
                }
                dso.RunDeal();

                int threadId = Thread.CurrentThread.GetHashCode();
                ReportCollateralFlow(dso, "RunDealTrigger_coll_" + threadId + ".csv");
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            finally
            {
                String sdkErrorMsg = dso.GetDealErrorMessage();
                if (sdkErrorMsg != null)
                {
                    Console.WriteLine(sdkErrorMsg);
                }
                // Close deal
                dso.CloseDeal();
                // Release deal scenario object
                dso.Dispose();
            }
        }

        public static void RunDealCall()
        {
            // Create deal_scenario object.
            Deal dso = SDK.CreateDeal();

            try
            {
                dso.OpenDeal("ACE06NC1", new DateTime(2011, 8, 31), true);
                int propagate_to_remics = 1;
                dso.SetDealCalcLevel(CALC_LEVEL.CALC_LEVEL_FULL, propagate_to_remics);

                Double[] ppy_rate = { 0.05 };
                Double[] def_rate = { 0.3 };
                Double[] rec_rate = { 0.6 };
                // Set custom vectors
                dso.SetPrepaymentsOnAllLoans(PrepaymentCurves.CPR, ppy_rate);
                dso.SetDefaultsOnAllLoans(DefaultCurves.CDR, def_rate);
                dso.SetRecoveriesOnAllLoans(rec_rate);

                // clean-up call
                dso.SetCallOption(CallOption.CLEANUP_CALL, 0);

                dso.RunDeal();

                int threadId = Thread.CurrentThread.GetHashCode();
                ReportCollateralFlow(dso, "RunDealCall_coll_" + threadId + ".csv");
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            finally
            {
                String sdkErrorMsg = dso.GetDealErrorMessage();
                if (sdkErrorMsg != null)
                {
                    Console.WriteLine(sdkErrorMsg);
                }
                // Close deal
                dso.CloseDeal();
                // Release deal scenario object
                dso.Dispose();
            }
        }

        public static void RunDealWithCallbacks()
        {
            // Create deal_scenario object.
            Deal dso = SDK.CreateDeal();

            try
            {
                dso.OpenDeal("AB06HE5", new DateTime(2011, 8, 31), false);

                // Period callback example
                Deal.PerPeriodCallBackFunc ppCallBack = new Deal.PerPeriodCallBackFunc(UserDefinedCallbacks.MyPerPeriodAssumpCallBack);
                // Prepare usr data for period callback
                UserDefinedCallbacks.PeriodAssumptionData per_assump_data = new UserDefinedCallbacks.PeriodAssumptionData();
                IntPtr pAssump = Marshal.AllocHGlobal(Marshal.SizeOf(per_assump_data));
                Marshal.StructureToPtr(per_assump_data, pAssump, true);
                // Set usr data for period callback
                dso.SetUserDataForCallBack(pAssump);
                // Install period callback for deal
                dso.InstallPerPeriodAssumptionCallBack(ppCallBack);

                // Collateral callback example
                Deal.CollatAssumptionCallBackFunc caCallback = new Deal.CollatAssumptionCallBackFunc(UserDefinedCallbacks.MyCollatAssumpCallBack);
                // Install collateral callback
                dso.InstallCollatAssumptionCallback(caCallback);

                // Run deal
                dso.RunDeal();

                // Print Collateral flows
                ReportCollateralFlow(dso, "RunDealWithCallbacks_coll.csv");
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            finally
            {
                String sdkErrorMsg = dso.GetDealErrorMessage();
                if (sdkErrorMsg != null)
                {
                    Console.WriteLine(sdkErrorMsg);
                }
                // Close deal
                dso.CloseDeal();
                // Release deal scenario object
                dso.Dispose();
            }
        }

        private class ThreadData
        {
            public string Deal;
            public Deal dso;
        };
        public static void RunDealWithMutipleThread()
        {
            try
            {
                // Enable same deal multithreading
                Deal.EnableSameDealMultithread(1);

                int numThreads = 3;
                List<ThreadData> tdList = new List<ThreadData>(numThreads);
                for (int i = 0; i < numThreads; ++i)
                {
                    ThreadData td = new ThreadData();
                    td.Deal = "ACE06NC1";
                    td.dso = SDK.CreateDeal();
                    tdList.Add(td);
                }

                List<Thread> allThreads = new List<Thread>();
                for (int i = 0; i < numThreads; i++)
                {
                    Thread t = new Thread(new ParameterizedThreadStart(TestExample.RunOneDeal));
                    allThreads.Add(t);
                    t.Start(tdList[i]);
                }

                for (int i = 0; i < numThreads; ++i)
                {
                    allThreads[i].Join();
                }

                // Output and close deal
                for (int i = 0; i < numThreads; ++i)
                {
                    ReportCollateralFlow(tdList[i].dso, "Multi-thread_Coll_" + i + ".csv");
                    tdList[i].dso.Dispose();
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }
        private static void RunOneDeal(Object runData)
        {
            ThreadData data = (ThreadData)runData;

            Deal dso = data.dso;

            dso.OpenDeal(data.Deal, new DateTime(2011, 8, 31), true);
            int propagate_to_remics = 1;
            dso.SetDealCalcLevel(CALC_LEVEL.CALC_LEVEL_FULL, propagate_to_remics);

            Double[] ppy_rate = { 0.0 };
            dso.SetPrepaymentsOnAllLoans(PrepaymentCurves.SMM, ppy_rate);

            dso.RunDeal();
        }
        public static void RunDealWithDplcModel()
        {
            // Create deal_scenario object.
            Deal dso = SDK.CreateDeal();

            try
            {
                dso.OpenDeal("ACE06NC1", new DateTime(2011, 8, 31), true);
                int propagate_to_remics = 1;
                dso.SetDealCalcLevel(CALC_LEVEL.CALC_LEVEL_FULL, propagate_to_remics);

                Double[] ppy_rate = { 0.0 };
                Double[] def_rate = { 0.0 };
                Double[] rec_rate = { 0.0 };
                // Set custom vectors
                dso.SetPrepaymentsOnAllLoans(PrepaymentCurves.CPR, ppy_rate);
                dso.SetDefaultsOnAllLoans(DefaultCurves.CDR, def_rate);
                dso.SetRecoveriesOnAllLoans(rec_rate);

                double[] libor_rates = { 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 6.5 };
                double[] rates = new double[640];
                double shift = 1.5;
                // Set rates
                for (int i = 0; i < 12; ++i)
                {
                    for (int j = 0; j < 640; ++j)
                    {
                        rates[j] = (libor_rates[i] + shift) / 100;
                    }
                    IndexTypes idx = IndexTypes.LIBOR_1 + i;
                    dso.SetRates(idx, rates);
                }


                dso.SetMoodysCreditModelSettings(MOODYS_CREDIT_MODEL_SETTINGS.MOODYS_DPLC_SETTINGS, 1);
                dso.SetServicerAdvances(ServicerAdvances.None);

                dso.RunDeal();

                ReportBondResults(dso, "RunDealWithDplcModel_bond.csv");
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            finally
            {
                String sdkErrorMsg = dso.GetDealErrorMessage();
                if (sdkErrorMsg != null)
                {
                    Console.WriteLine(sdkErrorMsg);
                }
                // Close deal
                dso.CloseDeal();
                // Release deal scenario object
                dso.Dispose();
            }
        }

        public static void RunDealWithCMMModel()
        {
            // Create deal_scenario object.
            Deal dso = SDK.CreateDeal();

            try
            {
                dso.OpenDeal("CMBS_BOA00002", null, true);
                int propagate_to_remics = 1;
                dso.SetDealCalcLevel(CALC_LEVEL.CALC_LEVEL_FULL, propagate_to_remics);

                Double[] ppy_rate = { 0.0 };
                Double[] def_rate = { 0.005 };
                // Set prepayments
                dso.SetPrepaymentsOnAllLoans(PrepaymentCurves.CPR, ppy_rate);
                //Set defaults
                dso.SetDefaultsOnAllLoans(DefaultCurves.CDR, def_rate);
                // Set months from default to liquidation.
                dso.SetRecoveryLagOnAllLoans(3, 1);

                // Set required interest rates.
                double[] libor_rates = { 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 6.5 };
                double[] rates = new double[640];
                double shift = 1.5;
                IndexTypes[] IndexRates = dso.ListIndexes();
                for (int i = 0; i < IndexRates.Length; ++i)
                {
                    for (int j = 0; j < 640; ++j)
                    {
                        rates[j] = ((short)IndexRates[i] + shift) / 100;
                    }
                    dso.SetRates(IndexRates[i], rates);
                }


                dso.SetMoodysCreditModelSettings(MOODYS_CREDIT_MODEL_SETTINGS.MOODYS_CMM_SETTINGS, 1);

                dso.RunDeal();

                ReportBondResults(dso, "RunDealWithCMMSettings_bond.csv");
                dso.CloseDeal();

                //Run CMM pre-defined scenarios
                dso.OpenDeal("CMBS_BOA00002", null, true);
                dso.SetMoodysCreditModelSettings(MOODYS_CREDIT_MODEL_SETTINGS.MOODYS_CMM_SETTINGS, 0);

                //Get all CMM pre-defined scenarios
                List<string> scenarios = dso.GetMoodysCMMScenarios(null);
                for (int i = 0; i < scenarios.Count(); ++i)
                {
                    System.Console.WriteLine("\n Scenario:" + scenarios[i]);

                }
                //Get default cmm current scenario
                string currentScenario = dso.GetCurrentMoodysCMMScenario(null);
                System.Console.WriteLine("\n Default CMM current scenario:" + currentScenario);

                //Set current scenario and run
                dso.SetCurrentMoodysCMMScenario(null, scenarios[3]);
                currentScenario = dso.GetCurrentMoodysCMMScenario(null);
                System.Console.WriteLine("\n Set CMM current scenario:" + currentScenario);
                dso.RunDeal();
                ReportBondResults(dso, "RunDealWithPredefined_bond.csv");
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            finally
            {
                String sdkErrorMsg = dso.GetDealErrorMessage();
                if (sdkErrorMsg != null)
                {
                    Console.WriteLine(sdkErrorMsg);
                }
                // Close deal
                dso.CloseDeal();
                // Release deal scenario object
                dso.Dispose();
            }
        }

        public static void RunCDOnetDeal()
        {
            // Create deal_scenario object.
            Deal dso = SDK.CreateDeal();

            try
            {
                // Set the preferred engine for overlapping deals.
                SDK.SetEnginePreference(ENGINE_PREFERENCE.PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
                Deal.SetMissingExchangeRatesHandling(MISSING_EXCHANGE_RATES_HANDLING.MISSING_EXCHANGE_RATES_USE_ONE);
                Deal.SetMissingInterestRatesHandling(MISSING_INTEREST_RATES_HANDLING.MISSING_INTEREST_RATES_USE_ZERO);

                // Open deal
                dso.OpenDeal("STATICLO", new DateTime(2013, 8, 1), true);
                int propagate_to_remics = 1;
                dso.SetDealCalcLevel(CALC_LEVEL.CALC_LEVEL_FULL, propagate_to_remics);
                
                // Set reinvestment assumptions
                dso.SetReinvestmentType(REINV_TYPE.GLOBAL_REINV);
                // Reinvestment settings  
                GLOBAL_REINVESTMENT_INFO reinvInfo = new GLOBAL_REINVESTMENT_INFO();
                reinvInfo.auto_reinv_end_date_override = 1;
                reinvInfo.term_setting = REINV_TERM_SETTING_TYPE.TERM_WAL_CURRENT;
                reinvInfo.ppy_pct_in_rp = 100.0;
                reinvInfo.ppy_pct_after_rp = 100.0;
                reinvInfo.sched_pct_prin_in_rp = 100.0;
                reinvInfo.sched_pct_prin_after_rp = 100.0;
                reinvInfo.rec_pct_in_rp = 100.0;
                reinvInfo.rec_pct_after_rp = 100.0;
                reinvInfo.curr_pct_prin_cash_in_rp = 100.0;
                reinvInfo.curr_pct_prin_cash_after_rp = 100.0;
                reinvInfo.use_global_reinvestment_override = 1;
                reinvInfo.global_libor_floor = new double[Constants.MaxPeriods];
                reinvInfo.num_global_libor_floor = 1;
                reinvInfo.global_libor_floor[0] = 0.0025;
                // Reinvestment assets
                GLOBAL_REINVESTMENT_ASSET_INFO[] assetInfos = new GLOBAL_REINVESTMENT_ASSET_INFO[2];
                // Asset of fixed rate
                assetInfos[0].allocation_ratio = new double[Constants.MaxPeriods];
                assetInfos[0].num_allocation_ratio = 1;
                assetInfos[0].allocation_ratio[0] = 0.05;
                assetInfos[0].adjustable = 0;
                assetInfos[0].currency = "EUR";
                assetInfos[0].term = 60;
                assetInfos[0].market_price = 95;
                assetInfos[0].recovery_rate = 0.65;
                assetInfos[0].pay_freq = PAYMENT_FREQUENCY.PAY_FREQ_QUARTERLY;
                assetInfos[0].day_count = DAY_COUNTS.DAY_COUNTS_30_360;
                assetInfos[0].coupon = 0.07;
                assetInfos[0].bank_loans = 0;
                assetInfos[0].first_reset = "20131001";
                assetInfos[0].country = "CAN";
                assetInfos[0].moodys_rating = "Aa1";
                // Asset of adjustable rate
                assetInfos[1].allocation_ratio = new double[Constants.MaxPeriods];
                assetInfos[1].num_allocation_ratio = 1;
                assetInfos[1].allocation_ratio[0] = 0.95;
                assetInfos[1].adjustable = 1;
                assetInfos[1].currency = "EUR";
                assetInfos[1].term = 60;
                assetInfos[1].market_price = 98;
                assetInfos[1].recovery_rate = 0.65;
                assetInfos[1].pay_freq = PAYMENT_FREQUENCY.PAY_FREQ_QUARTERLY;
                assetInfos[1].day_count = DAY_COUNTS.DAY_COUNTS_30_360;
                assetInfos[1].coupon = 0;
                assetInfos[1].leverage = 1.0;
                assetInfos[1].index = (short)IndexTypes.LIBOR_3;
                assetInfos[1].num_margin = 10;
                double margin = 0.011;
                assetInfos[1].margin = new double[Constants.MaxPeriods];
                for (int i = 0; i < 10; ++i)
                {
                    assetInfos[1].margin[i] = margin + i * 0.001;
                }
                assetInfos[1].reset_freq = 3;
                assetInfos[1].bank_loans = 1;
                assetInfos[1].first_reset = "20131001";
                assetInfos[1].country = "CAN";
                assetInfos[1].moodys_rating = "Aaa";
                dso.SetGlobalReinvestment(reinvInfo, assetInfos);
                
                // Set buy price override 
                double[] price = { 120.0, 115.0, 110.0, 105.0, 100.0 };
                dso.SetBuyPriceOverride(BUY_PRICE_OVERRIDE_TYPE.BUY_PRICE_OVERRIDE_INPUT, price);

                // Set custom vectors
                Double[] ppy_rate = { 0.0 };
                Double[] def_rate = { 0.0 };
                Double[] rec_rate = { 0.0 };
                dso.SetPrepaymentsOnAllLoans(PrepaymentCurves.CPR, ppy_rate);
                dso.SetDefaultsOnAllLoans(DefaultCurves.CDR, def_rate);
                dso.SetRecoveriesOnAllLoans(rec_rate);
                dso.SetRecoveryLagOnAllLoans(3);

                // Get global currency
                string global_currency = dso.GetGlobalCurrency();
                // Get currencies
                String[] currencies = dso.GetCurrencies();
                // Set exchange rate
                dso.SetExchangeRate(currencies[0], 1.0);
                // Set index ratre
                double[] index_rate = { 0.0 };
                dso.SetIndexRates(currencies[0], IndexTypes.LIBOR_1, index_rate);

                // Run deal
                dso.RunDeal();

                // Get bond cashflows
                ReportBondResults(dso, "RunCDOnetDeal_bond.csv");

                // Price bond
                PRICING_RESULTS results = dso.PriceBond("A", PRICING_ANCHORS.PRICE, 95.0000);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            finally
            {
                String sdkErrorMsg = dso.GetDealErrorMessage();
                if (sdkErrorMsg != null)
                {
                    Console.WriteLine(sdkErrorMsg);
                }
                // Close deal
                dso.CloseDeal();
                // Release deal scenario object
                dso.Dispose();
            }
        }

        public static void RunMonteCarloSFW()
        {
            // Create deal_scenario object.
            Deal dso = SDK.CreateDeal();

            try
            {
                // Open deal
                dso.OpenDeal("CMBS_BOA00002", null, true);
                // Set edf and lgd
                MARKIT_DEAL_INFO deal_info = dso.GetDealInfo(null);
                double[] edfRate = {0.01, 0.02, 0.03, 0.04, 0.05};
                double[] lgdRate = {0.5, 0.5, 0.5, 0.5, 0.5};
                
                for (int i = 1; i <= deal_info.num_colls; ++i)
                {
                    for(int j = 0; j < 5; ++j)
                    {
                        edfRate[j] += (i / 10000.0);
                    }
                    dso.SetLoanEDF(null, i, edfRate);
                    dso.SetLoanLGD(null, i, lgdRate);
                }
                // Set simulation engine
                dso.SetSimulationEngine(SIMULATION_TYPE.SIMULATION_MONTE_CARLO);
                // Set assumption
                MONTE_CARLO_ASSUMPTION assumption = new MONTE_CARLO_ASSUMPTION();
                assumption.mode = 0;
                assumption.num_paths = 100;
                assumption.optimization_type = MONTE_CARLO_OPTIMIZATION.MC_OPTIMIZATION_NONE;
                assumption.optimization_pct = 0.0;
                MONTE_CARLO_DEF_PPY_REC_ASSUMPTION def_ppy_rec_assumption = new MONTE_CARLO_DEF_PPY_REC_ASSUMPTION();
                def_ppy_rec_assumption.correlation_type = 2;
                def_ppy_rec_assumption.global_correlation = 0.3;
                def_ppy_rec_assumption.ppy_type = PrepaymentCurves.CPR;
                def_ppy_rec_assumption.ppy_pct = 0.2;
                def_ppy_rec_assumption.rec_vol = 0.0;
                def_ppy_rec_assumption.copula = 0;
                def_ppy_rec_assumption.use_random_seed = true;
                def_ppy_rec_assumption.random_number_seed = 2;
                dso.SetMonteCarloAssumption(assumption, def_ppy_rec_assumption);
                // Run monte carlo simulation
                dso.RunMonteCarloSimulation();

                int threadId = Thread.CurrentThread.GetHashCode();
                ReportAvgCollFlow(dso, "RunMonteCarloSFW_coll_" + threadId + ".csv");
                ReportAvgBondFlow(dso, "RunMonteCarloSFW_bond_" + threadId + ".csv");
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            finally
            {
                String sdkErrorMsg = dso.GetDealErrorMessage();
                if (sdkErrorMsg != null)
                {
                    Console.WriteLine(sdkErrorMsg);
                }
                // Close deal
                dso.CloseDeal();
                // Release deal scenario object
                dso.Dispose();
            }
        }

        public static void RunMonteCarloCDOnet()
        {
            // Create deal_scenario object.
            Deal dso = SDK.CreateDeal();

            try
            {
                // Set the preferred engine for overlapping deals.
                SDK.SetEnginePreference(ENGINE_PREFERENCE.PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
                Deal.SetMissingExchangeRatesHandling(MISSING_EXCHANGE_RATES_HANDLING.MISSING_EXCHANGE_RATES_USE_ONE);
                Deal.SetMissingInterestRatesHandling(MISSING_INTEREST_RATES_HANDLING.MISSING_INTEREST_RATES_USE_ZERO);
                // Open deal
                dso.OpenDeal("JUBILEE4", null, true);
                // Set edf 
                MARKIT_DEAL_INFO deal_info = dso.GetDealInfo(null);
                double[] edfRate = { 0.01, 0.02, 0.03, 0.04, 0.05 };
                for (int i = 1; i <= deal_info.num_colls; ++i)
                {
                    for (int j = 0; j < 5; ++j)
                    {
                        edfRate[j] += (i / 10000.0);
                    }
                    dso.SetLoanEDF(null, i, edfRate);
                }
                // Set recovery
                double[] rcv = { 0.4 };
                dso.SetRecoveriesOnAllLoans(rcv);
                // Set simulation engine
                dso.SetSimulationEngine(SIMULATION_TYPE.SIMULATION_MONTE_CARLO);
                // Set assumption
                MONTE_CARLO_ASSUMPTION assumption = new MONTE_CARLO_ASSUMPTION();
                assumption.mode = 0;
                assumption.num_paths = 100;
                assumption.optimization_type = MONTE_CARLO_OPTIMIZATION.MC_OPTIMIZATION_NONE;
                assumption.optimization_pct = 0.0;
                assumption.reinv_pool = 0;
                assumption.default_to_reinv = 0;
                MONTE_CARLO_DEF_PPY_REC_ASSUMPTION def_ppy_rec_assumption = new MONTE_CARLO_DEF_PPY_REC_ASSUMPTION();
                def_ppy_rec_assumption.correlation_type = 1;
                def_ppy_rec_assumption.intra_industry_correlation = 0.3;
                def_ppy_rec_assumption.inter_industry_correlation = 0.2;
                def_ppy_rec_assumption.default_probability_source = MONTE_CARLO_DEFAULT_TYPE.ASSET_DEFAULT_PROBABILITY;
                def_ppy_rec_assumption.default_probability_multiplier = 1.0;
                def_ppy_rec_assumption.ppy_type = PrepaymentCurves.CPR;
                def_ppy_rec_assumption.ppy_pct = 0.4;
                def_ppy_rec_assumption.rec_vol = 0.0;
                def_ppy_rec_assumption.copula = 0;
                def_ppy_rec_assumption.use_random_seed = true;
                def_ppy_rec_assumption.random_number_seed = 2;
                dso.SetMonteCarloAssumption(assumption, def_ppy_rec_assumption);
                // run monte carlo simulation
                dso.RunMonteCarloSimulation();

                int threadId = Thread.CurrentThread.GetHashCode();
                ReportAvgCollFlow(dso, "RunMonteCarloCDOnet_coll_" + threadId + ".csv");
                ReportAvgBondFlow(dso, "RunMonteCarloCDOnet_bond_" + threadId + ".csv");
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            finally
            {
                String sdkErrorMsg = dso.GetDealErrorMessage();
                if (sdkErrorMsg != null)
                {
                    Console.WriteLine(sdkErrorMsg);
                }
                // Close deal
                dso.CloseDeal();
                // Release deal scenario object
                dso.Dispose();
            }
        }
        public static void RunDPDSimulation()
        {
            // Create deal_scenario object.
            Deal dso = SDK.CreateDeal();

            try
            {
                dso.OpenDeal("ACE06NC1", new DateTime(2011, 8, 31), true);
                int propagate_to_remics = 1;
                dso.SetDealCalcLevel(CALC_LEVEL.CALC_LEVEL_FULL, propagate_to_remics);

                // Set simulation engine to DPD
                dso.SetSimulationEngine(SIMULATION_TYPE.SIMULATION_DEFAULT_PROBABILITY_DISTRIBUTION);

                // Set economy settings as usual, e.g. exchange/interest rates, prepay, default, losses and recovery lag and so on.
                // e.g. set prepay:
                Double[] ppy_rate = { 0.10 };
                dso.SetPrepaymentsOnAllLoans(PrepaymentCurves.CPR, ppy_rate);

                // Set dpd assumptions
                DPD_ASSUMPTION dpdAssumption = new DPD_ASSUMPTION();
                dpdAssumption.distribution = DPD_DISTRIBUTION_TYPE.DPD_DISTRIBUTION_LOGNORMAL;
                dpdAssumption.scenario_type = 1;
                dpdAssumption.mean = 10.0;
                dpdAssumption.use_milan_aaa_ce = 0;
                dpdAssumption.milan_aaa_ce = 0.0;
                dpdAssumption.standard_deviation = 1.0;
                dpdAssumption.discounted_recoveries = 0;
                dpdAssumption.revolving_default_factors = new double[Constants.MaxPeriods];
                for (short i = 0; i < Constants.MaxPeriods; ++i)
                {
                    dpdAssumption.revolving_default_factors[i] = 1.0;
                }
                dpdAssumption.num_scenarios = 3;
                dpdAssumption.use_revolving_def_timing = 1;
                dpdAssumption.rating_cap_primary = 3;
                dpdAssumption.rating_cap_surveillance = 5;
                dso.SetDpdAssumption(dpdAssumption);

                // Set current default timing vector(required)
                double[] currentDefaultTimingVector = {0.01};
                dso.SetDpdCurrentDefaultTiming(currentDefaultTimingVector, false);
                // Set revolving default timing vector if DPD_ASSUMPTION.use_revolving_def_timing has been set to '1'
                double[] revolvingDefaultTimingVector = {0.02};
                dso.SetDpdRevolvingDefaultTiming(revolvingDefaultTimingVector, false);

                // Run dpd simulation
                dso.RunDefaultProbabilityDistribution();

                int threadId = Thread.CurrentThread.GetHashCode();
                ReportAvgCollFlow(dso, "RunDPDSimulation_coll_" + threadId + ".csv");
                ReportAvgBondFlow(dso, "RunDPDSimulation_bond_" + threadId + ".csv");
            }
            catch (Exception e)
            {
                Console.WriteLine("[ERROR] " + e.Message);
            }
            finally
            {
                String sdkErrorMsg = dso.GetDealErrorMessage();
                if (sdkErrorMsg != null)
                {
                    Console.WriteLine(sdkErrorMsg);
                }
                // Close deal
                dso.CloseDeal();
                // Release deal scenario object
                dso.Dispose();
            }
        }

        public static void RunDealWithSedfModel()
        {
            // Create deal_scenario object.
            Deal dso = SDK.CreateDeal();

            try
            {
                // Set the preferred engine for overlapping deals.
                SDK.SetEnginePreference(ENGINE_PREFERENCE.PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);

                // Open the specified deal.
                dso.OpenDeal("ARIEL", null, true);

                // Set credit model type.
                dso.SetMoodysCreditModelSettings(MOODYS_CREDIT_MODEL_SETTINGS.MOODYS_SEDF_SETTINGS, 1);

                // Get SEDF scenarios.
                List<string> scenario_list = dso.GetEDFScenarios();

                // Show SEDF scenarios.
                System.Console.WriteLine("\nSEDF scenarios: (" + scenario_list.Count() + " scenarios in total)");
                for (int i = 0; i < scenario_list.Count(); ++i)
                {
                    System.Console.WriteLine("Scenario " + i + ": " + scenario_list[i]);
                }

                // Set SEDF scenario, scenario index is start from 0.
                dso.SetCurrentEDFScenario(1);
                int currScenario = dso.GetCurrentEDFScenario();

                // Set user edf data for loan 2.
                double[] pdSet = { 0.1, 0.2, 0.3, 0.4, 0.5 };
                dso.SetLoanEDF(null, 2, pdSet);

                // get edf data of loan 2
                double[] pdGet = dso.GetLoanEDF(null, 2);

                Double[] ppy_rate = { 0.18 };
                // Set prepayments
                dso.SetPrepaymentsOnAllLoans(PrepaymentCurves.CPR, ppy_rate);
                // Set recovery lag with 8 for all loans
                dso.SetRecoveryLagOnAllLoans(8, 1);
                // Run deal
                dso.RunDeal();

                ReportBondResults(dso, "RunDealWithSedfSettings_bond.csv");
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            finally
            {
                String sdkErrorMsg = dso.GetDealErrorMessage();
                if (sdkErrorMsg != null)
                {
                    Console.WriteLine(sdkErrorMsg);
                }
                // Close deal
                dso.CloseDeal();
                // Release deal scenario object
                dso.Dispose();
            }
        }

        public static void initialize()
        {
            String input_path = "..\\..\\..\\..\\DATA";

            // Set input path
            SDK.SetInputPath(input_path);
            SDK.SetMPADataPath(input_path + "\\MPA\\data");
            SDK.SetPADataPath(input_path + "\\PA\\data");

            //Set the preferred engine for overlapping deals.
            SDK.SetEnginePreference(ENGINE_PREFERENCE.PICK_SFW_ENGINE_FOR_MAPPED_DEALS); //PICK_CHS_ENGINE_FOR_MAPPED_DEALS

            // Set the error handling level.
            Deal.SetErrorHandlingLevel(ERROR_HANDLING_LEVEL.ERROR_HANDLING_LEVEL_LOG_IT); //ERROR_HANDLING_LEVEL_STOP_CALCULATION
        }

        private static void ReportCollateralFlow(Deal dso, string fileName)
        {
            string text = "";

            int[] collateralGroups;
            int totalGroups = dso.GetCollateralGroups(out collateralGroups);

            for (int i = 0; i < totalGroups; i++)
            {
                int group = collateralGroups[i];
                text += dso.GetDealId() + " Collat Group " + group + ",Cashflow:" + "\n";
                MARKIT_COLLAT_CASHFLOW collCF = dso.GetCollGroupCashFlowAllTypes(group, null);
                text += "PERIOD,DATE,BALANCE,SCHED_PRINCIPAL,PREPAYMENTS,DEFAULTS," +
                        "LOSSES,LIQUIDATIONS,INTEREST,REINVESTMENT,COLLATERAL_CASH,GROSS_INTEREST,SCHED P&I" +
                        "\n";

                for (int ii = 0; ii < collCF.size; ii++)
                {
                    text += ii + "," + collCF.dates[ii] + "," +
                                    collCF.balance[ii] + "," +
                                    collCF.sched_principal[ii] + "," +
                                    collCF.prepayments[ii] + "," +
                                    collCF.defaults[ii] + "," +
                                    collCF.losses[ii] + "," +
                                    collCF.liquidations[ii] + "," +
                                    collCF.interest[ii] + "," +
                                    collCF.reinvestment[ii] + "," +
                                    collCF.cash[i] + "," +
                                    collCF.gross_interest[ii] + "," +
                                    collCF.sched_p_and_i[ii] +
                                    "\n";
                }
            }

            System.IO.File.WriteAllText(@fileName, text);

            System.Console.WriteLine("-----Coll ashflow saved to " + fileName);
        }

        private static void ReportBondResults(Deal dso, string fileName)
        {
            string text = "";

            CCMO_BONDS_S[] bonds = dso.GetBondInfo();


            for (int i = 0; i < bonds.Length; i++)
            {
                text += "bond,period,date,balance,principal,interest" + "\n";
                string bondId = bonds[i].StrippedId;

                MARKIT_BOND_CASHFLOW bondCashFlow = dso.GetBondCashFlowAllTypes(null, bondId);

                if (bondCashFlow.start_index_as_per_settle_date > 0)
                {
                    bondCashFlow.interest[bondCashFlow.start_index_as_per_settle_date] = 0;
                    bondCashFlow.principal[bondCashFlow.start_index_as_per_settle_date] = 0;
                    bondCashFlow.cash[bondCashFlow.start_index_as_per_settle_date] = 0;
                    bondCashFlow.accrual_begin_dates[bondCashFlow.start_index_as_per_settle_date] = 0;
                    bondCashFlow.rate[bondCashFlow.start_index_as_per_settle_date] = 0;
                }

                for (int per = bondCashFlow.start_index_as_per_settle_date; per < bondCashFlow.size; ++per)
                {
                    text += bondId + "," + per + "," + bondCashFlow.dates[per] + "," + bondCashFlow.balance[per] + "," + bondCashFlow.principal[per] + "," + bondCashFlow.interest[per] + "\n";
                }
            }
            System.IO.File.WriteAllText(@fileName, text);

            System.Console.WriteLine("-----Bond cashflow saved to " + fileName);
        }

        private static void ReportAvgCollFlow(Deal dso, string fileName)
        {
            string text = "";

            MARKIT_COLLAT_CASHFLOW collCF = dso.GetCollGroupCashFlowAllTypes(0, null);
            text += dso.GetDealId() + ", Cashflow, Size=" + collCF.size + "\n";

            if (collCF.size > 0)
            {
                text += "PERIOD,DATE,BALANCE,SCHED_PRINCIPAL,PREPAYMENTS,DEFAULTS," +
                        "LOSSES,LIQUIDATIONS,INTEREST,REINVESTMENT,COLLATERAL_CASH" +
                        "\n";

                for (int ii = 0; ii < collCF.size; ii++)
                {
                    text += ii + "," + collCF.dates[ii] + "," +
                                    collCF.balance[ii] + "," +
                                    collCF.sched_principal[ii] + "," +
                                    collCF.prepayments[ii] + "," +
                                    collCF.defaults[ii] + "," +
                                    collCF.losses[ii] + "," +
                                    collCF.liquidations[ii] + "," +
                                    collCF.interest[ii] + "," +
                                    collCF.reinvestment[ii] + "," +
                                    collCF.cash[ii] + "," + // 'cash' is not available for DPD(always 0.0)
                                    "\n";
                }
            }

            System.IO.File.WriteAllText(@fileName, text);

            System.Console.WriteLine("-----Average coll cashflow saved to " + fileName);
        }

        private static void ReportAvgBondFlow(Deal dso, string fileName)
        {
            string text = "";

            CCMO_BONDS_S[] bonds = dso.GetBondInfo();


            for (int i = 0; i < bonds.Length; i++)
            {
                string bondId = bonds[i].StrippedId;
                MARKIT_BOND_CASHFLOW bondCashFlow = dso.GetBondCashFlowAllTypes(null, bondId);
                if (bondCashFlow.size > 0)
                {
                    text += "bond,period,date,balance,principal,interest,rate,principal_writedown" + "\n";
                    if (bondCashFlow.start_index_as_per_settle_date > 0)
                    {
                        bondCashFlow.interest[bondCashFlow.start_index_as_per_settle_date] = 0;
                        bondCashFlow.principal[bondCashFlow.start_index_as_per_settle_date] = 0;
                        bondCashFlow.rate[bondCashFlow.start_index_as_per_settle_date] = 0;
                        bondCashFlow.principal_writedown[bondCashFlow.start_index_as_per_settle_date] = 0;
                    }

                    for (int per = bondCashFlow.start_index_as_per_settle_date; per < bondCashFlow.size; ++per)
                    {
                        text += bondId + "," + per + "," + bondCashFlow.dates[per] + "," + bondCashFlow.balance[per] + "," + bondCashFlow.principal[per] + "," + bondCashFlow.interest[per] + "," + bondCashFlow.rate[per] + "," + bondCashFlow.principal_writedown[per] + "\n";
                    }
                }
            }
            System.IO.File.WriteAllText(@fileName, text);

            System.Console.WriteLine("-----Average bond cashflow saved to " + fileName);
        }

        public static void RunDealWithMPASinglePath()
        {
            // Create deal_scenario object.
            Deal dso = SDK.CreateDeal();

            try
            {
                dso.OpenDeal("ACE06NC1", new DateTime(2011, 8, 31), true);
                int propagate_to_remics = 1;
                dso.SetDealCalcLevel(CALC_LEVEL.CALC_LEVEL_FULL, propagate_to_remics);

                Double[] ppy_rate = { 0.0 };
                dso.SetPrepaymentsOnAllLoans(PrepaymentCurves.SMM, ppy_rate);

                // Set credit model type.
                dso.SetMoodysCreditModelSettings(MOODYS_CREDIT_MODEL_SETTINGS.MOODYS_MPA_SETTINGS, 0);

                // Set MPA analysis type.
                dso.SetMPAAnalysisType(MPA_ANALYSIS_TYPE.MPA_MEDC_SINGLE_PATH);

                // Get MPA scenarios.
                List<String> scenarios = dso.GetMPAScenarios();
                Console.WriteLine("\nMPA scenarios: (" + scenarios.Count + " scenarios in total)");
                foreach (String scenario in scenarios)
                    Console.WriteLine(scenario);

                // Set MPA scenario, scenario index is start from 0.
                dso.SetCurrentMPAScenario(3);

                // Set recovery lag by state(judicial/non-judicial).
                dso.SetMPARecoveryLagByState(30, 35);

                // Set recovery lag on loan 1.
                int[] recoveryLags = { 10, 12 };
                dso.SetMPARecoveryLagOnSingleLoan(1, recoveryLags);

                // Set MPA multipliers.
                double[] ppy_multiplier = { 2.0 };
                double[] def_multiplier = { 3.0 };
                double[] sev_multiplier = { 4.0 };
                dso.SetMPAMultiplierOnAllLoans(MPA_MULTIPLIER_TYPE.MPA_MULTIPLIER_PREPAY, ppy_multiplier);
                dso.SetMPAMultiplierOnAllLoans(MPA_MULTIPLIER_TYPE.MPA_MULTIPLIER_DEFAULT, def_multiplier);
                dso.SetMPAMultiplierOnAllLoans(MPA_MULTIPLIER_TYPE.MPA_MULTIPLIER_SEVERITY, sev_multiplier);

                // Set MPA simulation length.
                dso.SetMPASimulationLength(200);

                // Set use mid course adjustment data.
                dso.SetMPAMidCourseAdj(true);

                // Set MPA haircut.
                double[] haircut_vector = new double[13];
                for (int i = 0; i < 12; ++i)
                    haircut_vector[i] = 7.0;
                haircut_vector[12] = 3.0;
                dso.SetMPAHaircut(haircut_vector, false);

                dso.RunDeal();

                int threadId = Thread.CurrentThread.GetHashCode();
                ReportCollateralFlow(dso, "RunDealWithMPASinglePathSample_coll_" + threadId + ".csv");
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            finally
            {
                String sdkErrorMsg = dso.GetDealErrorMessage();
                if (sdkErrorMsg != null)
                {
                    Console.WriteLine(sdkErrorMsg);
                }
                // Close deal
                dso.CloseDeal();
                // Release deal scenario object
                dso.Dispose();
            }
        }

        public static void RunDealWithMPACustomSinglePath()
        {
            // Create deal_scenario object.
            Deal dso = SDK.CreateDeal();

            try
            {
                dso.OpenDeal("ACE06NC1", new DateTime(2011, 8, 31), true);
                int propagate_to_remics = 1;
                dso.SetDealCalcLevel(CALC_LEVEL.CALC_LEVEL_FULL, propagate_to_remics);

                Double[] ppy_rate = { 0.0 };
                dso.SetPrepaymentsOnAllLoans(PrepaymentCurves.SMM, ppy_rate);

                // Set credit model type.
                dso.SetMoodysCreditModelSettings(MOODYS_CREDIT_MODEL_SETTINGS.MOODYS_MPA_SETTINGS, 0);

                // Set MPA analysis type.
                dso.SetMPAAnalysisType(MPA_ANALYSIS_TYPE.MPA_CUST_MEDC_SINGLE_PATH);

                // Set MPA custom scenario data.
                int[] year = { 2014, 2014, 2014, 2014, 2015, 2015, 2015, 2015, 2016, 2016, 2016, 2016, 2017, 2017, 2017, 2017, 2018, 2018, 2018, 2018 };
                int[] quarter = { 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4 };

                double[] UsUemployment_value = { 7.89, 7.85, 7.77, 7.56, 7.67, 7.42, 7.39, 7.31, 7.26, 7.29, 7.33, 7.28, 7.25, 7.17, 7.21, 7.19, 7.14, 7.03, 7.01, 6.88 };
                double[] UsHPI_value = { 2.34, 2.98, 3.59, 4.12, 4.46, 4.02, 4.13, 4.19, 4.35, 4.67, 4.95, 4.97, 5.23, 5.69, 6.19, 8.43, 10.13, 13.11, 14.01, 18.63 };
                double[] UsGDP_value = { 0.99, 1.34, 1.77, 1.98, 2.09, 2.43, 2.53, 2.69, 3.16, 3.87, 4.05, 5.11, 6.19, 9.67, 13.42, 15.55, 16.95, 18.34, 23.39, 29.66 };
                double[] UsT1yRate_value = { 0.23, 0.27, 0.39, 0.46, 0.55, 0.69, 0.79, 0.92, 1.23, 1.34, 1.56, 1.77, 1.95, 2.03, 2.15, 2.34, 2.39, 2.41, 2.68, 2.71 };
                double[] UsT10yRate_value = { 2.25, 2.34, 2.39, 2.44, 2.59, 2.48, 2.41, 2.37, 2.43, 2.57, 2.64, 2.83, 2.96, 3.24, 3.35, 3.81, 3.49, 3.67, 4.22, 4.53 };
                double[] Libor6mSpread_value = { 0.62, 0.61, 0.63, 0.78, 0.96, 1.26, 1.35, 1.67, 1.69, 1.75, 1.86, 1.92, 2.11, 2.23, 2.42, 2.53, 2.67, 2.92, 3.32, 3.36 };
                double[] STATE_GDP_value = { 1.7, 2.3, 2.5, 3.4, 3.9, 5.1, 5.9, 6.7, 8.3, 9.2, 9.8, 10.1, 10.6, 13.2, 15.7, 18.4, 19.2, 23.1, 30.3, 35.23 };
                double[] FREDMAC_value = { 4.15, 4.28, 4.55, 4.45, 5.02, 5.11, 5.13, 5.26, 5.27, 5.39, 5.66, 6.03, 6.17, 6.52, 6.77, 7.13, 7.22, 7.52, 7.53, 7.55 };

                int[] year2 = { 2014, 2014, 2014, 2014, 2015, 2015 };
                int[] quarter2 = { 1, 2, 3, 4, 1, 2 };
                double[] REG_UsHPI_value_MABI = { -1.03, -0.59, 1.15, 2.57, 4.68, 9.16 };
                double[] REG_UsHPI_value_AK = { 2.18, 2.34, 2.97, 3.15, 3.23, 3.56, 4.67, 4.92, 5.12, 6.19, 8.33, 10.53, 13.52, 16.41, 18.96, 23.11, 29.1, 33.15, 35.15, 39.11 };

                double[] REG_UsUemployment_value_AK = { 6.54, 6.38, 6.24, 6.11, 6.07, 5.99 };
                double[] REG_UsUemployment_value_MNEY = { 8.36, 8.23, 8.21, 8.19, 8.1, 8.06, 7.95, 7.91, 7.83, 7.61, 7.53, 7.39, 7.34, 7.27, 7.13, 7.16, 7.26, 7.33, 7.18, 7.02 };

                dso.SetMPACustomScenario("UNEMPLOYMENT", "US", year, quarter, UsUemployment_value);
                dso.SetMPACustomScenario("HPI", "US", year, quarter, UsHPI_value);
                dso.SetMPACustomScenario("GDP", "US", year, quarter, UsGDP_value);
                dso.SetMPACustomScenario("TSY1Y", "US", year, quarter, UsT1yRate_value);
                dso.SetMPACustomScenario("TSY10Y", "US", year, quarter, UsT10yRate_value);

                dso.SetMPACustomScenario("REG_UNEMPLOYMENT", "AK", year2, quarter2, REG_UsUemployment_value_AK);
                dso.SetMPACustomScenario("REG_UNEMPLOYMENT", "MNEY", year, quarter, REG_UsUemployment_value_MNEY);
                dso.SetMPACustomScenario("REG_HPI", "MABI", year2, quarter2, REG_UsHPI_value_MABI);
                dso.SetMPACustomScenario("REG_HPI", "AK", year, quarter, REG_UsHPI_value_AK);
                dso.SetMPACustomScenario("STATE_GDP", "NY", year, quarter, STATE_GDP_value);

                dso.SetMPACustomScenario("LIBOR6MSPREAD", "US", year, quarter, Libor6mSpread_value);
                dso.SetMPACustomScenario("FREDMAC", "US", year, quarter, FREDMAC_value);


                // Set recovery lag by state(judicial/non-judicial).
                dso.SetMPARecoveryLagByState(30, 35);

                // Set recovery lag on loan 1.
                int[] recoveryLags = { 10, 12 };
                dso.SetMPARecoveryLagOnSingleLoan(1, recoveryLags);

                // Set MPA multipliers.
                double[] ppy_multiplier = { 2.0 };
                double[] def_multiplier = { 3.0 };
                double[] sev_multiplier = { 4.0 };
                dso.SetMPAMultiplierOnAllLoans(MPA_MULTIPLIER_TYPE.MPA_MULTIPLIER_PREPAY, ppy_multiplier);
                dso.SetMPAMultiplierOnAllLoans(MPA_MULTIPLIER_TYPE.MPA_MULTIPLIER_DEFAULT, def_multiplier);
                dso.SetMPAMultiplierOnAllLoans(MPA_MULTIPLIER_TYPE.MPA_MULTIPLIER_SEVERITY, sev_multiplier);

                // Set MPA simulation length.
                dso.SetMPASimulationLength(200);

                // Set use mid course adjustment data.
                dso.SetMPAMidCourseAdj(true);

                // Set MPA haircut.
                double[] haircut_vector = new double[13];
                for (int i = 0; i < 12; ++i)
                    haircut_vector[i] = 7.0;
                haircut_vector[12] = 3.0;
                dso.SetMPAHaircut(haircut_vector, false);

                dso.RunDeal();

                int threadId = Thread.CurrentThread.GetHashCode();
                ReportCollateralFlow(dso, "RunDealWithMPACustomSinglePathSample_coll_" + threadId + ".csv");
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            finally
            {
                String sdkErrorMsg = dso.GetDealErrorMessage();
                if (sdkErrorMsg != null)
                {
                    Console.WriteLine(sdkErrorMsg);
                }
                // Close deal
                dso.CloseDeal();
                // Release deal scenario object
                dso.Dispose();
            }
        }

        public static void RunDealWithMPALossSimulation()
        {
            // Create deal_scenario object.
            Deal dso = SDK.CreateDeal();

            try
            {
                dso.OpenDeal("ACE06NC1", new DateTime(2011, 8, 31), true);
                int propagate_to_remics = 1;
                dso.SetDealCalcLevel(CALC_LEVEL.CALC_LEVEL_FULL, propagate_to_remics);

                Double[] ppy_rate = { 0.0 };
                dso.SetPrepaymentsOnAllLoans(PrepaymentCurves.SMM, ppy_rate);

                // Set credit model type.
                dso.SetMoodysCreditModelSettings(MOODYS_CREDIT_MODEL_SETTINGS.MOODYS_MPA_SETTINGS, 0);

                // Set MPA analysis type.
                dso.SetMPAAnalysisType(MPA_ANALYSIS_TYPE.MPA_LOSS_SIMULATION);

                // Set MPA simulation path number.
                dso.SetMPASimulationPathNum(5);    // 5 path, default path number is 10000

                // Set recovery lag by state(judicial/non-judicial).
                dso.SetMPARecoveryLagByState(30, 35);

                // Set recovery lag on loan 1.
                int[] recoveryLags = { 10, 12 };
                dso.SetMPARecoveryLagOnSingleLoan(1, recoveryLags);

                // Set MPA multipliers.
                double[] ppy_multiplier = { 2.0 };
                double[] def_multiplier = { 3.0 };
                double[] sev_multiplier = { 4.0 };
                dso.SetMPAMultiplierOnAllLoans(MPA_MULTIPLIER_TYPE.MPA_MULTIPLIER_PREPAY, ppy_multiplier);
                dso.SetMPAMultiplierOnAllLoans(MPA_MULTIPLIER_TYPE.MPA_MULTIPLIER_DEFAULT, def_multiplier);
                dso.SetMPAMultiplierOnAllLoans(MPA_MULTIPLIER_TYPE.MPA_MULTIPLIER_SEVERITY, sev_multiplier);

                // Set MPA simulation length.
                dso.SetMPASimulationLength(200);

                // Set use mid course adjustment data.
                dso.SetMPAMidCourseAdj(true);

                // Set MPA haircut.
                double[] haircut_vector = new double[13];
                for (int i = 0; i < 12; ++i)
                    haircut_vector[i] = 7.0;
                haircut_vector[12] = 3.0;
                dso.SetMPAHaircut(haircut_vector, false);

                dso.RunDeal();

                int threadId = Thread.CurrentThread.GetHashCode();
                ReportCollateralFlow(dso, "RunDealWithMPALossSimulation_coll_" + threadId + ".csv");
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            finally
            {
                String sdkErrorMsg = dso.GetDealErrorMessage();
                if (sdkErrorMsg != null)
                {
                    Console.WriteLine(sdkErrorMsg);
                }
                // Close deal
                dso.CloseDeal();
                // Release deal scenario object
                dso.Dispose();
            }
        }

        public static void RunDealWithMPALossSimulationCustomScenario()
        {
            // Create deal_scenario object.
            Deal dso = SDK.CreateDeal();

            try
            {
                dso.OpenDeal("ACE06NC1", new DateTime(2011, 8, 31), true);
                int propagate_to_remics = 1;
                dso.SetDealCalcLevel(CALC_LEVEL.CALC_LEVEL_FULL, propagate_to_remics);

                Double[] ppy_rate = { 0.0 };
                dso.SetPrepaymentsOnAllLoans(PrepaymentCurves.SMM, ppy_rate);

                // Set credit model type.
                dso.SetMoodysCreditModelSettings(MOODYS_CREDIT_MODEL_SETTINGS.MOODYS_MPA_SETTINGS, 0);

                // Set MPA analysis type.
                dso.SetMPAAnalysisType(MPA_ANALYSIS_TYPE.MPA_CUST_MEDC_SIMULATION);

                // Set MPA simulation path number.
                dso.SetMPASimulationPathNum(5);    // 5 path, default path number is 10000

                // Set MPA custom scenario data.
                int[] year = { 2014, 2014, 2014, 2014, 2015, 2015, 2015, 2015, 2016, 2016, 2016, 2016, 2017, 2017, 2017, 2017, 2018, 2018, 2018, 2018 };
                int[] quarter = { 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4 };

                double[] UsUemployment_value = { 7.89, 7.85, 7.77, 7.56, 7.67, 7.42, 7.39, 7.31, 7.26, 7.29, 7.33, 7.28, 7.25, 7.17, 7.21, 7.19, 7.14, 7.03, 7.01, 6.88 };
                double[] UsHPI_value = { 2.34, 2.98, 3.59, 4.12, 4.46, 4.02, 4.13, 4.19, 4.35, 4.67, 4.95, 4.97, 5.23, 5.69, 6.19, 8.43, 10.13, 13.11, 14.01, 18.63 };
                double[] UsGDP_value = { 0.99, 1.34, 1.77, 1.98, 2.09, 2.43, 2.53, 2.69, 3.16, 3.87, 4.05, 5.11, 6.19, 9.67, 13.42, 15.55, 16.95, 18.34, 23.39, 29.66 };
                double[] UsT1yRate_value = { 0.23, 0.27, 0.39, 0.46, 0.55, 0.69, 0.79, 0.92, 1.23, 1.34, 1.56, 1.77, 1.95, 2.03, 2.15, 2.34, 2.39, 2.41, 2.68, 2.71 };
                double[] UsT10yRate_value = { 2.25, 2.34, 2.39, 2.44, 2.59, 2.48, 2.41, 2.37, 2.43, 2.57, 2.64, 2.83, 2.96, 3.24, 3.35, 3.81, 3.49, 3.67, 4.22, 4.53 };
                double[] Libor6mSpread_value = { 0.62, 0.61, 0.63, 0.78, 0.96, 1.26, 1.35, 1.67, 1.69, 1.75, 1.86, 1.92, 2.11, 2.23, 2.42, 2.53, 2.67, 2.92, 3.32, 3.36 };
                double[] STATE_GDP_value = { 1.7, 2.3, 2.5, 3.4, 3.9, 5.1, 5.9, 6.7, 8.3, 9.2, 9.8, 10.1, 10.6, 13.2, 15.7, 18.4, 19.2, 23.1, 30.3, 35.23 };
                double[] FREDMAC_value = { 4.15, 4.28, 4.55, 4.45, 5.02, 5.11, 5.13, 5.26, 5.27, 5.39, 5.66, 6.03, 6.17, 6.52, 6.77, 7.13, 7.22, 7.52, 7.53, 7.55 };

                int[] year2 = { 2014, 2014, 2014, 2014, 2015, 2015 };
                int[] quarter2 = { 1, 2, 3, 4, 1, 2 };
                double[] REG_UsHPI_value_MABI = { -1.03, -0.59, 1.15, 2.57, 4.68, 9.16 };
                double[] REG_UsHPI_value_AK = { 2.18, 2.34, 2.97, 3.15, 3.23, 3.56, 4.67, 4.92, 5.12, 6.19, 8.33, 10.53, 13.52, 16.41, 18.96, 23.11, 29.1, 33.15, 35.15, 39.11 };

                double[] REG_UsUemployment_value_AK = { 6.54, 6.38, 6.24, 6.11, 6.07, 5.99 };
                double[] REG_UsUemployment_value_MNEY = { 8.36, 8.23, 8.21, 8.19, 8.1, 8.06, 7.95, 7.91, 7.83, 7.61, 7.53, 7.39, 7.34, 7.27, 7.13, 7.16, 7.26, 7.33, 7.18, 7.02 };

                dso.SetMPACustomScenario("UNEMPLOYMENT", "US", year, quarter, UsUemployment_value);
                dso.SetMPACustomScenario("HPI", "US", year, quarter, UsHPI_value);
                dso.SetMPACustomScenario("GDP", "US", year, quarter, UsGDP_value);
                dso.SetMPACustomScenario("TSY1Y", "US", year, quarter, UsT1yRate_value);
                dso.SetMPACustomScenario("TSY10Y", "US", year, quarter, UsT10yRate_value);

                dso.SetMPACustomScenario("REG_UNEMPLOYMENT", "AK", year2, quarter2, REG_UsUemployment_value_AK);
                dso.SetMPACustomScenario("REG_UNEMPLOYMENT", "MNEY", year, quarter, REG_UsUemployment_value_MNEY);
                dso.SetMPACustomScenario("REG_HPI", "MABI", year2, quarter2, REG_UsHPI_value_MABI);
                dso.SetMPACustomScenario("REG_HPI", "AK", year, quarter, REG_UsHPI_value_AK);
                dso.SetMPACustomScenario("STATE_GDP", "NY", year, quarter, STATE_GDP_value);

                dso.SetMPACustomScenario("LIBOR6MSPREAD", "US", year, quarter, Libor6mSpread_value);
                dso.SetMPACustomScenario("FREDMAC", "US", year, quarter, FREDMAC_value);


                // Set recovery lag by state(judicial/non-judicial).
                dso.SetMPARecoveryLagByState(30, 35);

                // Set recovery lag on loan 1.
                int[] recoveryLags = { 10, 12 };
                dso.SetMPARecoveryLagOnSingleLoan(1, recoveryLags);

                // Set MPA multipliers.
                double[] ppy_multiplier = { 2.0 };
                double[] def_multiplier = { 3.0 };
                double[] sev_multiplier = { 4.0 };
                dso.SetMPAMultiplierOnAllLoans(MPA_MULTIPLIER_TYPE.MPA_MULTIPLIER_PREPAY, ppy_multiplier);
                dso.SetMPAMultiplierOnAllLoans(MPA_MULTIPLIER_TYPE.MPA_MULTIPLIER_DEFAULT, def_multiplier);
                dso.SetMPAMultiplierOnAllLoans(MPA_MULTIPLIER_TYPE.MPA_MULTIPLIER_SEVERITY, sev_multiplier);

                // Set MPA simulation length.
                dso.SetMPASimulationLength(200);

                // Set use mid course adjustment data.
                dso.SetMPAMidCourseAdj(true);

                // Set MPA haircut.
                double[] haircut_vector = new double[13];
                for (int i = 0; i < 12; ++i)
                    haircut_vector[i] = 7.0;
                haircut_vector[12] = 3.0;
                dso.SetMPAHaircut(haircut_vector, false);

                dso.RunDeal();

                int threadId = Thread.CurrentThread.GetHashCode();
                ReportCollateralFlow(dso, "RunDealWithMPACustomLossSimulation_coll_" + threadId + ".csv");
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            finally
            {
                String sdkErrorMsg = dso.GetDealErrorMessage();
                if (sdkErrorMsg != null)
                {
                    Console.WriteLine(sdkErrorMsg);
                }
                // Close deal
                dso.CloseDeal();
                // Release deal scenario object
                dso.Dispose();
            }
        }

        public static void RunDealWithPASinglePath()
        {
            // Create deal_scenario object.
            Deal dso = SDK.CreateDeal();

            try
            {
                dso.OpenDeal("AMEXCAMT", null, true);
                int propagate_to_remics = 1;
                dso.SetDealCalcLevel(CALC_LEVEL.CALC_LEVEL_FULL, propagate_to_remics);

                // Set credit model type.
                dso.SetMoodysCreditModelSettings(MOODYS_CREDIT_MODEL_SETTINGS.MOODYS_PA_SETTINGS, 0);

                // Set PA analysis type.
                dso.SetPAAnalysisType(PA_ANALYSIS_TYPE.PA_MEDC_SINGLE_PATH);

                // Get PA scenarios.
                List<String> scenarios = dso.GetPAScenarios();
                Console.WriteLine("\nPA scenarios: (" + scenarios.Count + " scenarios in total)");
                foreach (String scenario in scenarios)
                    Console.WriteLine(scenario);

                // Set PA scenario, scenario index is start from 0.
                dso.SetCurrentPAScenario(3);

                // Default pool data for SFW deal.
                dso.SetPADefaultPoolData("Rate30", "10");
                dso.SetPADefaultPoolData("Rate60", "10");
                dso.SetPADefaultPoolData("Rate90", "10");
                dso.SetPADefaultPoolData("CPR", "10");
                dso.SetPADefaultPoolData("CDR", "10");
                dso.SetPADefaultPoolData("Severity", "10");
                dso.SetPADefaultPoolData("PrincipalPayment", "10");
                dso.SetPADefaultPoolData("SellerPercentage", "10");
                dso.SetPADefaultPoolData("Yield", "10");
                dso.SetPADefaultPoolData("ChargeOff", "10");
                dso.SetPADefaultPoolData("RepaymentRate", "10");
                dso.SetPADefaultPoolData("DefermentRate", "10");
                dso.SetPADefaultPoolData("ForbearanceRate", "10");

                /*
                        // Use the following settings for CHS deal, the pool data fields are different from SFW deal.
                        // Default pool data for CHS deal.
                        dso.SetPADefaultPoolData("PurposePurchase", "0");
                        dso.SetPADefaultPoolData("PurposeRefi", "0");
                        dso.SetPADefaultPoolData("OccupancyOwner", "0");
                        dso.SetPADefaultPoolData("OccupancySecondHome", "0");
                        dso.SetPADefaultPoolData("OccupancyInvestor", "0");
                        dso.SetPADefaultPoolData("Property1Unit", "0");
                        dso.SetPADefaultPoolData("Property24Unit", "0");
                        dso.SetPADefaultPoolData("OriginatorThirdParty", "0");
                        dso.SetPADefaultPoolData("OriginatorRetail", "0");
                        dso.SetPADefaultPoolData("HARP1", "0");
                        dso.SetPADefaultPoolData("HARP2", "0");
                        dso.SetPADefaultPoolData("FHA", "0");
                */

                dso.RunDeal();

                int threadId = Thread.CurrentThread.GetHashCode();
                ReportCollateralFlow(dso, "RunDealWithPASinglePathSample_coll_" + threadId + ".csv");
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            finally
            {
                String sdkErrorMsg = dso.GetDealErrorMessage();
                if (sdkErrorMsg != null)
                {
                    Console.WriteLine(sdkErrorMsg);
                }
                // Close deal
                dso.CloseDeal();
                // Release deal scenario object
                dso.Dispose();
            }
        }

        public static void RunDealWithPACustomSinglePath()
        {
            // Create deal_scenario object.
            Deal dso = SDK.CreateDeal();

            try
            {
                dso.OpenDeal("AMEXCAMT", null, true);
                int propagate_to_remics = 1;
                dso.SetDealCalcLevel(CALC_LEVEL.CALC_LEVEL_FULL, propagate_to_remics);

                // Set credit model type.
                dso.SetMoodysCreditModelSettings(MOODYS_CREDIT_MODEL_SETTINGS.MOODYS_PA_SETTINGS, 0);

                // Set PA analysis type.
                dso.SetPAAnalysisType(PA_ANALYSIS_TYPE.PA_CUST_MEDC_SINGLE_PATH);

                // Set PA default pool data.
                // Default pool data for SFW deal(so we need to provide the default settings, in case some pool data are missing).
                dso.SetPADefaultPoolData("Rate30", "10");
                dso.SetPADefaultPoolData("Rate60", "10");
                dso.SetPADefaultPoolData("Rate90", "10");
                dso.SetPADefaultPoolData("CPR", "10");
                dso.SetPADefaultPoolData("CDR", "10");
                dso.SetPADefaultPoolData("Severity", "10");
                dso.SetPADefaultPoolData("PrincipalPayment", "10");
                dso.SetPADefaultPoolData("SellerPercentage", "10");
                dso.SetPADefaultPoolData("Yield", "10");
                dso.SetPADefaultPoolData("ChargeOff", "10");
                dso.SetPADefaultPoolData("RepaymentRate", "10");
                dso.SetPADefaultPoolData("DefermentRate", "10");
                dso.SetPADefaultPoolData("ForbearanceRate", "10");

        /*
                // Use the following settings for CHS deal, the pool data fields are different from SFW deal.
                // Default pool data for CHS deal(so we need to provide the default settings, in case some pool data are missing).
                dso.SetPADefaultPoolData("PurposePurchase", "0");
                dso.SetPADefaultPoolData("PurposeRefi", "0");
                dso.SetPADefaultPoolData("OccupancyOwner", "0");
                dso.SetPADefaultPoolData("OccupancySecondHome", "0");
                dso.SetPADefaultPoolData("OccupancyInvestor", "0");
                dso.SetPADefaultPoolData("Property1Unit", "0");
                dso.SetPADefaultPoolData("Property24Unit", "0");
                dso.SetPADefaultPoolData("OriginatorThirdParty", "0");
                dso.SetPADefaultPoolData("OriginatorRetail", "0");
                dso.SetPADefaultPoolData("HARP1", "0");
                dso.SetPADefaultPoolData("HARP2", "0");
                dso.SetPADefaultPoolData("FHA", "0");
        */

                // Set pa custom scenario.
                int[] year = { 2014, 2014, 2014, 2014, 2015, 2015, 2015, 2015, 2016, 2016, 2016, 2016, 2017, 2017, 2017, 2017, 2018, 2018, 2018 };
                int[] quarter = { 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3 };
                double[] unemploy = { 7.85, 7.77, 7.56, 7.67, 7.42, 7.39, 7.31, 7.26, 7.29, 7.33, 7.28, 7.25, 7.17, 7.21, 7.19, 7.14, 7.03, 7.01, 6.88 };
                double[] hpi = { 2.98, 3.59, 4.12, 4.46, 4.02, 4.13, 4.19, 4.35, 4.67, 4.95, 4.97, 5.23, 5.69, 6.19, 8.43, 10.13, 13.11, 14.01, 18.63 };
                double[] gdp = { 1.34, 1.77, 1.98, 2.09, 2.43, 2.53, 2.69, 3.16, 3.87, 4.05, 5.11, 6.19, 9.67, 13.42, 15.55, 16.95, 18.34, 23.39, 29.66 };
                double[] try1y = { 0.27, 0.39, 0.46, 0.55, 0.69, 0.79, 0.92, 1.23, 1.34, 1.56, 1.77, 1.95, 2.03, 2.15, 2.34, 2.39, 2.41, 2.68, 2.71 };
                double[] try10y = { 2.34, 2.39, 2.44, 2.59, 2.48, 2.41, 2.37, 2.43, 2.57, 2.64, 2.83, 2.96, 3.24, 3.35, 3.81, 3.49, 3.67, 4.22, 4.53 };
                double[] libor6m = { 7.85, 7.77, 7.56, 7.67, 7.42, 7.39, 7.31, 7.26, 7.29, 7.33, 7.28, 7.25, 7.17, 7.21, 7.19, 7.14, 7.03, 7.01, 6.88 };

                dso.SetPACustomScenario("UNEMPLOYMENT", year, quarter, unemploy);
                dso.SetPACustomScenario("HPI", year, quarter, hpi);
                dso.SetPACustomScenario("GDP", year, quarter, gdp);
                dso.SetPACustomScenario("TSY1Y", year, quarter, try1y);
                dso.SetPACustomScenario("TSY10Y", year, quarter, try10y);
                dso.SetPACustomScenario("LIBOR6MSPREAD", year, quarter, libor6m);

                dso.RunDeal();

                int threadId = Thread.CurrentThread.GetHashCode();
                ReportCollateralFlow(dso, "RunDealWithPACustomSinglePathSample_coll_" + threadId + ".csv");
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            finally
            {
                String sdkErrorMsg = dso.GetDealErrorMessage();
                if (sdkErrorMsg != null)
                {
                    Console.WriteLine(sdkErrorMsg);
                }
                // Close deal
                dso.CloseDeal();
                // Release deal scenario object
                dso.Dispose();
            }
        }
    }


    class UserDefinedCallbacks
    {
        /**
        * Given a pool age (wala) and pool factor, project the default, prepayment, 
        * recovery, deliquency, recovery lag and service advance flag.
        **/
        public static void ProjectAssump(int age, double pool_factor, int length,
                            double[] cpr, double[] cdr,
                            double[] recovery, double[] del,
                            out short recoveryLag, out ServicerAdvances serviceAdvanceFlag)
        {
            double modifier = pool_factor * age / length;

            double p_step = 0.1 / length * modifier;
            double d_step = 0.2 / length * modifier;
            double r_step = 0.6 / length * modifier;
            double del_step = 0.8 / length;

            for (int i = 1; i <= length; i++)
            {
                cpr[i] = p_step * i;
                cdr[i] = d_step * i;
                recovery[i] = r_step * i;
                del[i] = del_step * i;
            }
            recoveryLag = 12;
            serviceAdvanceFlag = ServicerAdvances.Both;
        }

        public struct PeriodAssumptionData
        {
            public void SetCollatInfo(int period, int max_periods, PAY_POOL_INFO pool)
            {
                if (period == 1 ||
                    dealName != pool.deal_name ||
                    collatId != pool.pool_info.Id ||
                    pool.parent_deal_name != null &&
                    parentDeal != pool.parent_deal_name)
                {
                    dealName = pool.deal_name;
                    collatId = pool.pool_info.Id;
                    parentDeal = pool.parent_deal_name;
                    cpr = new double[max_periods + 1];
                    cdr = new double[max_periods + 1];
                    recovery = new double[max_periods + 1];
                    del = new double[max_periods + 1];

                    ProjectAssump(pool.pool_info.Wala, pool.pool_info.Factor, max_periods, cpr, cdr,
                                      recovery, del, out recoveryLag, out servicerAdvanceFlag);

                    last_cpr = cpr[max_periods - 1];
                    last_cdr = cdr[max_periods - 1];
                    last_recovery = recovery[max_periods - 1];
                    last_del = del[max_periods - 1];
                }
            }

            public double get_cpr(int per, int max_periods)
            {
                return per < max_periods ? cpr[per] : last_cpr;
            }

            public double get_cdr(int per, int max_periods)
            {
                return per < max_periods ? cdr[per] : last_cdr;
            }

            public double get_recovery(int per, int max_periods)
            {
                return per < max_periods ? recovery[per] : last_recovery;
            }

            public double get_del(int per, int max_periods)
            {
                return per < max_periods ? del[per] : last_del;
            }

            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 80)]
            public string parentDeal;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 80)]
            public string dealName;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 20)]
            public string collatId;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = Constants.MaxPeriods + 1)]
            public double[] cpr;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = Constants.MaxPeriods + 1)]
            public double[] cdr;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = Constants.MaxPeriods + 1)]
            public double[] recovery;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = Constants.MaxPeriods + 1)]
            public double[] del;
            public double last_cpr;
            public double last_cdr;
            public double last_recovery;
            public double last_del;
            public short recoveryLag;
            public ServicerAdvances servicerAdvanceFlag;
        }

        /**
        * Example for a collateral assumption call back function
        **/
        public unsafe static int MyCollatAssumpCallBack(Deal dso,
                          String first_period_date, int max_periods,
                          PAY_POOL_INFO pool_info,
                          ref CCMO_COLLAT_ASSUMPTIONS assumptions,
                          IntPtr user_data, StringBuilder error_message)
        {
            double[] cpr = new double[Constants.MaxPeriods + 1];
            double[] cdr = new double[Constants.MaxPeriods + 1];
            double[] recovery = new double[Constants.MaxPeriods + 1];
            double[] del = new double[Constants.MaxPeriods + 1];

            short recoveryLag = 0;
            ServicerAdvances servicerAdvanceFlag = 0;
            ProjectAssump(pool_info.pool_info.Wala, pool_info.pool_info.Factor, max_periods,
                cpr, cdr, recovery, del, out recoveryLag, out servicerAdvanceFlag);

            assumptions.default_type = DefaultCurves.CDR;
            assumptions.n_defaults = max_periods;
            assumptions.prepay_type = PrepaymentCurves.CPR;
            assumptions.n_prepays = max_periods;
            assumptions.n_recoveries = max_periods;

            for (int i = 1; i <= max_periods; i++)
            {
                assumptions.prepay_vector[i] = cpr[i];
                assumptions.default_vector[i] = cdr[i];
                assumptions.recovery_vector[i] = recovery[i];
            }
            Marshal.Copy(del, 1, assumptions.dlnq[(int)GroupDelinqStats.GROUP_DELINQ_30], max_periods);
            Marshal.Copy(del, 1, assumptions.dlnq[(int)GroupDelinqStats.GROUP_DELINQ_60], max_periods);
            Marshal.Copy(del, 1, assumptions.dlnq[(int)GroupDelinqStats.GROUP_DELINQ_90], max_periods);
            assumptions.n_recovery_lags = 1;
            assumptions.recovery_lag_vec[0] = assumptions.recovery_lag_vec[1] = recoveryLag;
            assumptions.servicer_advancing = servicerAdvanceFlag;

            return 0;
        }

        /**
         * Example for a per period assumption call back function
         **/
        public unsafe static int MyPerPeriodAssumpCallBack(Deal dso, int period, int max_periods,
                                             PAY_POOL_INFO pool,
                                             PAY_POOL_STATE pool_state,
                                             ref CCMO_PERIOD_ASSUMPTIONS assumptions,
                                             IntPtr user_data, StringBuilder error_message)
        {
            if (max_periods < 1)
            {
                return 0;
            }

            // Get assumption data
            PeriodAssumptionData per_assump;
            per_assump = (PeriodAssumptionData)Marshal.PtrToStructure(user_data, typeof(PeriodAssumptionData));
            per_assump.SetCollatInfo(period, max_periods, pool);

            // Set assumption data per period
            double smm = 1 - Math.Pow(1 - per_assump.get_cpr(period, max_periods), 1.0 / 12);
            double mdr = 1 - Math.Pow(1 - per_assump.get_cdr(period, max_periods), 1.0 / 12);
            assumptions.smm_vector[period] = smm;
            assumptions.mdr_vector[period] = mdr;
            assumptions.recovery_vector[period] = per_assump.get_recovery(period, max_periods);
            assumptions.recovery_lag_vec[period] = (short)per_assump.recoveryLag;
            assumptions.servicer_advancing = per_assump.servicerAdvanceFlag;
            return 0;
        }
    }
}
