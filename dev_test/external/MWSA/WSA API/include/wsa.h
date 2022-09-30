/// 
/// @file    wsa.h
/// @version 4.0.1.0
/// @date    2011-2019
/// 
#ifndef __WSA_H_83D2CD9D_09B0_48D2_9DA4_349B28ABD854
#define __WSA_H_83D2CD9D_09B0_48D2_9DA4_349B28ABD854

#include "ccmo.h"

/*********************************************************************/
/// WSA API Version
#define WSA_API_VERSION "4.0.1.0"
/*********************************************************************/

///
/// This enumeration can be used to indicate preferred engine for deals supported
/// by both CHS and SFW. By default, SFW is the preferred engine.
/// @see
///     set_engine_preference()
typedef enum
{
    PICK_CHS_ENGINE_FOR_MAPPED_DEALS,   ///< Choose CHS as preferred engine to run deals
    PICK_SFW_ENGINE_FOR_MAPPED_DEALS,   ///< Choose SFW as preferred engine to run deals
    PICK_CDONET_ENGINE_FOR_MAPPED_DEALS ///< Choose CDONET as preferred engine to run deals
} ENGINE_PREFERENCE;


/// This enumeration can be used to indicate the engine opening current deal.
/// @see
///     get_current_deal_engine()
typedef enum
{
    ENGINE_TYPE_ERROR = -99,    ///< Obsolete, use UNKNOWN_ENGINE
    UNKNOWN_ENGINE=-99,         ///< Unknown engine (no deal open yet)

    CHS_ENGINE = 0,             ///< Current deal opened by CHS engine
    SFW_ENGINE = 1,             ///< Current deal opened by SFW engine
    CDONET_ENGINE = 2           ///< Current deal opened by CDONET engine
} ENGINE_TYPE;


/// INDEX_TYPE_EX is the extension to INDEX_TYPE (In indextypes.h).
///
/// To get required index rate type codes (including extended ones), use get_required_rate_codes().
typedef enum
{
    INDEX_TYPE_EX_BASE=MAX_INDEX_TYPES-1, 	///< Place holder
    GNMA_MORT, 				///< Ginnie Mae Mortgage Rate
    FNMA_MORT, 				///< Fannie Mae Mortgage Rate
    FED_FUNDS, 				///< Federal funds rate
    LARGE_CD, 				///< Large CD rate (Certificates of Deposit)
    EURO_CD, 				///< Euro CD rate (Certificates of Deposit)
    COMM_PP, 				///< Commercial Paper Rates
    CORP_AAA, 				///< Yield on AAA Corporate bond rate
    COMM_LOAN, 				///< Commercial loan rate
    NATCOFI, 				///< COFI rate (Cost of Funds Index)
    CD_6, 					///< 6 months CD rate (Certificates of Deposit)
    AUTO_LOAN, 				///< Auto loan rate
    MTA, 					///< Monthly Treasury Average, aka Credit Card Index
    XRATE1, 				///< Extra Rate 1
    XRATE2, 				///< Extra Rate 2
    XRATE3, 				///< Extra Rate 3
    XRATE4, 				///< Extra Rate 4
    XRATE5, 				///< Extra Rate 5
    XRATE6, 				///< Extra Rate 6
    XRATE7, 				///< Extra Rate 7
    XRATE8, 				///< Extra Rate 8
    INFLATION, 				///< Inflation rate
    SVR,                    ///< Standard Variable Rate
    BBR,                    ///< Bank base rate
    COFI_6,                 ///< 6 months COFI rate (Cost of Funds Index)
    NAT_MORT,               ///< National Mortgage rate
    LIBOR_2,                ///< 2 months Libor rate
    LIBOR_1W,               ///< 1 week Libor rate
    LIBOR_2W,               ///< 2 week Libor rate
    LIBOR_4,                ///< 4 months Libor rate
    BLR_12,                 ///< China 1 year lending rate, SFW only.
    BLR_12_60,              ///< China 1 - 5 year lending rate, SFW only.
    BLR_60_PLUS,            ///< China 5+ year lending rate, SFW only.
    BDR_3,                  ///< China 3 months deposit rate, SFW only.
    BDR_6,                  ///< China 6 months deposit rate, SFW only.
    BDR_12,                 ///< China 1 year deposit rate, SFW only.
    BDR_24,                 ///< China 2 year deposit rate, SFW only.
    BDR_36,                 ///< China 3 year deposit rate, SFW only.
    BDR_60,                 ///< China 5 year deposit rate, SFW only.
    HPF_0_60,               ///< China 0 - 5 year home provident fund rate, SFW only.
    HPF_60_PLUS,            ///< China 5+ year home provident fund rate, SFW only.
    MAX_INDEX_TYPES_EX,  	///< Number of index type(include both basic and extra index type)
} INDEX_TYPE_EX;

/// Missing exchange rates handling type
/// @exptl Subject to change
/// @see set_missing_exchange_rates_handling
typedef enum
{
    MISSING_EXCHANGE_RATES_USE_ONE,        ///< Use 1.0 as the exchange rate  
    MISSING_EXCHANGE_RATES_TREAT_AS_ERROR, ///< Report as an error.   
    NUM_MISSING_EXCHANGE_RATES_TYPE        ///< Enumeration number of the type.
} MISSING_EXCHANGE_RATES_HANDLING;

/// Resec Exception Handling
/// @exptl Subject to change
/// @see set_resec_exceptions_handling
typedef enum
{
    RESEC_EXCEPTIONS_HANDLING_TREAT_AS_NONRESEC, ///< Treat missing underlying deals or circularly referenced deals as non-resec 
    RESEC_EXCEPTIONS_HANDLING_TREAT_AS_ERROR,    ///< Treat missing underlying deals or circularly referenced deals as errors
    
    NUM_RESEC_EXCEPTIONS_HANDLING_TYPE           ///< Number of resec exceptions handling types
} RESEC_EXCEPTIONS_HANDLING;

/// Additional pool property types
///
/// POOL_PROPERTY_TYPES_EX is the extension to POOL_PROPERTY_TYPES (In indextypes.h).
typedef enum
{
    POOL_PROPERTY_TYPE_EX_BASE=POOL_PROPERTY_NOT_AVAILABLE,	///< Place holder 
    POOL_PROPERTY_MULTI_FAMILY_TWO, 						///< Property Type: Two Family
    POOL_PROPERTY_MULTI_FAMILY_THREE, 						///< Property Type: Three Family
    POOL_PROPERTY_MULTI_FAMILY_FOUR, 						///< Property Type: Four Family
    POOL_PROPERTY_MULTI_FAMILY_FIVEPLUS, 					///< Property Type: Five or more Family
} POOL_PROPERTY_TYPES_EX;


/// Additional delinquent states
///
/// POOL_DELINQ_STATES_EX is the extension to POOL_DELINQ_STATES (In indextypes.h).
typedef enum
{
    POOL_DELINQ_STATE_EX_BASE=POOL_DELINQ_STATES_SIZE-1,	///< Place holder 
    POOL_DELINQ_120P,  										///< Delinquent >= 120 days
    POOL_DELINQ_150P, 										///< Delinquent >= 150 days
    POOL_DELINQ_180P, 										///< Delinquent >= 180 days
    POOL_DEFEASED,  										///< Defeasance status
    POOL_NON_PERFORMING_MATURED_BALLOON, 					///< Non performing matured balloon
    POOL_DELINQ_0P, 										///< Delinquent >= 0 days
    POOL_BANKRUPT,                                          ///< Bankrupt
    POOL_PAID_OFF,                                          ///< Paid off
    POOL_REPURCHASED,                                       ///< Repurchased
    POOL_LIQUIDATED,                                        ///< Liquidated
    POOL_CLOSED,                                            ///< Closed

    POOL_DELINQ_STATES_EX_SIZE, 							///< Number of extra delinquency states
} POOL_DELINQ_STATES_EX;


/// Additional doc types
///
/// POOL_DOCUM_TYPES_EX is the extension to POOL_DOCUM_TYPES (In indextypes.h).
typedef enum
{
    POOL_DOCUM_TYPES_EX_BASE=POOL_DOCUM_UNKNOWN, 	///< Place holder
    POOL_DOCUM_FULL_ASSETS_PARTIAL_INCOME, 			///< Documentation type: Full Assets - Partial Income
    POOL_DOCUM_FULL_INCOME_NO_ASSETS, 				///< Documentation type: Full Income - No Assets
    POOL_DOCUM_NO_INCOME_PARTIAL_ASSETS, 			///< Documentation type: No Income - Partial Assets
    POOL_DOCUM_NO_INCOME_STATED_ASSETS 				///< Documentation type: No Income - Stated Assets
} POOL_DOCUM_TYPES_EX;

/// Prepayment and default calculation methods
/// @see
///     * set_calculation_method()
///     * get_calculation_method()
/// @note
/// - Def[i] means defaults at period i of cashflow 
/// - Ppy[i] means prepayments at period i of cashflow
/// - SchdPrin[i] means scheduled principal at period i of cashflow
/// - OrigAmort[i] means performing balance at period i of the original amortization schedule with no prepayments and defaults
/// - SMM[i] means single monthly mortality default rate at period i
typedef enum
{
    PREPAY_DEFAULT_BEFORE_SCHED_PRIN_PPYDEF,    ///< Default & Prepay before Scheduled Principal\n
	                                            ///< Default, Prepayment and scheduled principal are calculated by the following formula:\n
												///< - Def[i] = PerfBal[i-1] * SMM[i]\n
												///< - Ppy[i] = PerfBal[i-1] * OrigAmort[i] / OrigAmort[i-1] * SMM[i]\n
												///< - SchdPrin[i] = ( PerfBal[i-1] - Def[i] ) * ( 1 - OrigAmort[i] / OrigAmort[i-1] )\n
												///< 
    SCHED_PRIN_PREPAY_BEFORE_DEFAULT_PPYDEF,    ///< Scheduled Principal & Prepay before Default\n
												///< Default, Prepayment and scheduled principal are calculated by the following formula:\n
												///< - SchdPrin[i] = PerfBal[i-1] * ( 1 - OrigAmort[i] / OrigAmort[i-1] )\n
												///< - Ppy[i] = PerfBal[i-1] * OrigAmort[i] / OrigAmort[i-1] * SMM[i]\n
												///< - Def[i] = ( PerfBal[i-1] - SchdPrin[i] - Ppy[i] ) * SMM[i]\n
												///< 
    DEFAULT_BEFORE_SCHED_PRIN_PREPAY_PPYDEF,    ///< Default before Scheduled Principal & Prepay\n
												///< Default, Prepayment and scheduled principal are calculated by the following formula:\n
												///< - Def[i] = PerfBal[i-1] * SMM[i]\n
												///< - SchdPrin[i] = ( PerfBal[i-1] - Def[i] ) * ( 1 - OrigAmort[i] / OrigAmort[i-1] )\n
												///< - Ppy[i] = ( PerfBal[i-1] - Def[i] - SchdPrin[i] ) * SMM[i]\n
												///< 
    DEFAULT_PREPAY_SCHED_PRIN_PPYDEF,           ///< Default, Prepay, Scheduled Principal\n
												///< Default, Prepayment and scheduled principal are calculated by the following formula:\n
												///< - Def[i] = PerfBal[i-1] * SMM[i]\n
												///< - Ppy[i] = ( PerfBal[i-1] - Def[i] ) * SMM[i]\n
												///< - SchdPrin[i] = ( PerfBal[i-1] - Def[i] - Ppy[i] ) * ( 1 - OrigAmort[i] / OrigAmort[i-1] )\n
												///< 
    JAPANESE_PREPAY_DEFAULT_PPYDEF,             ///< Japanese default and prepay convention\n
												///< Default, Prepayment and scheduled principal are calculated by the following formula:\n
												///< - Def[i] = PerfBal[i-1] * SMM[i]\n
												///< - Ppy[i] = PerfBal[i-1] * OrigAmort[i] / OrigAmort[i-1] * SMM[i]\n
												///< - SchdPrin[i] = ( PerfBal[i-1] - Def[i] ) * ( 1 - OrigAmort[i] / OrigAmort[i-1] )\n
												///< @note For student loan deals, "JAPANESE_PREPAY_DEFAULT_PPYDEF" is an invalid input.
												///< 
    NUM_PREPAY_DEFAULT_METHODS                  ///< Max methods num
}PREPAY_DEFAULT_CALC_METHOD_TYPE;

/// Type of draw rate
/// @exptl Subject to change
/// @see
///     set_draw_rates()
typedef enum
{
    DRAW_CURVE_SMM=0,          ///< Draw rate expressed in SMM
    DRAW_CURVE_CPR,            ///< Draw rate expressed in CPR
    DRAW_CURVE_SEASONED_SMM,   ///< Draw rate expressed in SMM with seasoned
    DRAW_CURVE_SEASONED_CPR,   ///< Draw rate expressed in CPR with seasoned
    NUM_DRAW_RATE_TYPE  ///< Size of DRAW_RATE_TYPE
}DRAW_RATE_TYPE;

/// Deal level miscellaneous cashflow identifiers
/// @exptl Subject to change
/// @see
///     get_misc_flow()
typedef enum
{
    FLOW_MISC_FEE_TOTAL = 0,               ///< Total fee (SFW and CDOnet)
    FLOW_MISC_FEE_AE,                      ///< Administrative Fee (CDOnet only)
    FLOW_MISC_FEE_DEF_AE,                  ///< Deferred Administrative Fee (CDOnet only) 
    FLOW_MISC_FEE_SCMF,                    ///< Senior Collateral Management Fees (CDOnet only)
    FLOW_MISC_FEE_DEF_SCMF,                ///< Deferred Senior Collateral Management Fees (CDOnet only)
    FLOW_MISC_FEE_SUCMF,                   ///< Subordinate Collateral Management Fees (CDOnet only)
    FLOW_MISC_FEE_DEF_SUCMF,               ///< Deferred Subordinate Collateral Management Fees (CDOnet only)
    FLOW_MISC_FEE_ICMF,                    ///< Incentive Collateral Management Fee (CDOnet only)  
    FLOW_MISC_FEE_DEF_ICMF,                ///< Deferred Incentive Collateral Management Fee (CDOnet only)
    FLOW_MISC_FEE_ICMF2,                   ///< Incentive Collateral Management Fee 2 (CDOnet only)
    FLOW_MISC_FEE_TAXES,                   ///< Taxes Fee (CDOnet only)   
    FLOW_MISC_FEE_TRUSTEE,                 ///< Trustee Fee (CDOnet only)
    FLOW_MISC_FEE_DEF_TRUSTEE              ///< Deferred Trustee Fee (CDOnet only)
} FLOW_MISC_INDENTIFIER;

/// Deal account types.
/// @see
///     DEAL_ACCOUNT_INFO
typedef enum
{
    DEAL_RESERVE_ACCOUNT,           ///< Reserve Account
    DEAL_LIQFAC_ACCOUNT,            ///< Liquidation Facility
    DEAL_INSURANCE_ACCOUNT,         ///< Insurance Account

    NUM_DEAL_ACCOUNT_TYPES          ///< Number of account types
} DEAL_ACCOUNT_TYPES;


/// CDO test type
/// @exptl Subject to change
typedef enum
{
    TEST_IC,    ///< Interest coverage test
    TEST_PV,    ///< Par value test
    TEST_UD     ///< User defined test
} TEST_TYPE;

/// CDO haircut type.
/// @exptl Subject to change
typedef enum
{
    HAIRCUT_1,     ///< Haircut 1.
    HAIRCUT_2,     ///< Haircut 2.
    HAIRCUT_UD     ///< User defined haircut.
} CDO_HAIRCUT_TYPE;

/// Buy price override types.
/// @exptl Subject to change
/// @see set_buy_price_override()
typedef enum
{ 
    BUY_PRICE_OVERRIDE_NO,          ///< No:     use the deal level assumptions.
    BUY_PRICE_OVERRIDE_MODEL,       ///< MODEL:  use 100.
    BUY_PRICE_OVERRIDE_MARKET,      ///< MARKET: use weighted average market price of the asset pool.
    BUY_PRICE_OVERRIDE_INPUT,       ///< INPUT:  use the price vector provided by user

    NUM_BUY_PRICE_OVERRIDE_TYPE     ///< Number of buy price override types 
} BUY_PRICE_OVERRIDE_TYPE;

/// Call date override types.
/// @exptl Subject to change
/// @see set_call_date_override()
typedef enum
{
    CALL_DATE_OVERRIDE_NO,              ///< NO:            use the deal level assumptions 
    CALL_DATE_OVERRIDE_MATURITY,        ///< MATURITY:      use deal maturity date as call date 
    CALL_DATE_OVERRIDE_NEXT_CALLABLE,   ///< NEXT_CALLABLE: use next callable date as call date 
    CALL_DATE_OVERRIDE_AUCTION,         ///< AUCTION:       use auction call date as call date 
    CALL_DATE_OVERRIDE_CLEAN_UP,        ///< CLEAN_UP:      use clean up call date as call date 
    CALL_DATE_OVERRIDE_INPUT,           ///< INPUT:         use user input Date as call date 

    NUM_CALL_DATE_OVERRIDE_TYPE         ///< Number of buy price override types 
} CALL_DATE_OVERRIDE_TYPE;

/// Call price override types.
/// @exptl Subject to change
/// @see set_call_price_override()
typedef enum
{ 
    CALL_PRICE_OVERRIDE_NO,          ///< No:     use the deal level assumptions.
    CALL_PRICE_OVERRIDE_MODEL,       ///< MODEL:  use 100.
    CALL_PRICE_OVERRIDE_MARKET,      ///< MARKET: use weighted average market price of the asset pool.
    CALL_PRICE_OVERRIDE_INPUT,       ///< INPUT:  use the price vector provided by user

    NUM_CALL_PRICE_OVERRIDE_TYPE     ///< Number of call price override types 
} CALL_PRICE_OVERRIDE_TYPE;

/// Reinvestment types.
/// @exptl Subject to change
/// @see set_reinvestment_type()
typedef enum
{ 
    DEAL_REINV,                      ///< Use the deal level reinvestment settings.
    GLOBAL_REINV,                    ///< Use global reinvestment settings. 

    NUM_REINV_TYPE                   ///< Number of reinvestment types
} REINV_TYPE;

/// Reinvestment term setting types
/// @exptl Subject to change
/// @see GLOBAL_REINVESTMENT_INFO
typedef enum
{
    TERM_NON_SEASONED_INPUT,               ///< The value to the term of an asset will not be adjusted by the age of the deal.  
    TERM_SEASONED_INPUT,                   ///< The value to the term of an asset will be adjusted by the age of the deal.  
    TERM_WAL_CURRENT,                      ///< The value to the term of an asset will be overriden by the weighted average life of current collateral pool.  
    TERM_WAL_TRIGGER,                      ///< The value to the term of an asset will be overriden by the value of the trigger of the weighted average life quality test.     

    NUM_TERM_SETTING_TYPE                  ///< Number of term setting types   
} REINV_TERM_SETTING_TYPE;

/// Reinvestment override types
/// @exptl Subject to change
/// @see set_pv_reinvest_override
typedef enum
{ 
    REINV_OVERRIDE_ALWAYS,                 ///< Test is not considered for Auto Reinvestment. This is the default
    REINV_OVERRIDE_NEVER,                  ///< If any test labeled 'Never' is failing, Auto Reinvestment is not active until all 'Never' tests pass again. 
    REINV_OVERRIDE_REINV_PER,              ///< This is treated as 'Always' during the RP, 'Never' during After RP.

    NUM_REINV_OVERRIDE_TYPE                ///< Number of reinvestment override types 
} REINV_OVERRIDE_TYPE;

/// Seniority of the asset
/// @exptl Subject to change
/// @see GLOBAL_REINVESTMENT_ASSET_INFO
typedef enum
{ 
    SENIORITY_BLANK,                       ///< Blank
	SENIORITY_SUB,                         ///< Sub 
	SENIORITY_MEZZ,                        ///< Mezzanine
    SENIORITY_JUNIOR,                      ///< Junior 
    SENIORITY_SENIOR,                      ///< Senior
    SENIORITY_SR_SEC,                      ///< Senior secured
    SENIORITY_SR_SUB,                      ///< Senior subordinated
	SENIORITY_JR_MEZZ,					   ///< Junior Mezzanine
	SENIORITY_SR_MEZZ,					   ///< Senior Mezzanine
    SENIORITY_SR_UNSEC,                    ///< Senior unsecured
	SENIORITY_SOVEREIGN,				   ///< Sovereign
    SENIORITY_SECOND_LIEN,                 ///< Second lien loan
	SENIORITY_SUB_SEC,					   ///< Subordinated secured
	SENIORITY_SUB_UNSEC,				   ///< Subordinated unsecured
    NUM_ASSET_SENIORITY_TYPE               ///< Number of asset seniority types 
} ASSET_SENIORITY;

/// Payment frequency types
/// @exptl Subject to change
/// @see GLOBAL_REINVESTMENT_ASSET_INFO
typedef enum
{ 
    PAY_FREQ_MONTHLY,                      ///< Pay every month.  
    PAY_FREQ_SANNUALLY,                    ///< Pay every 6 months.  
    PAY_FREQ_QUARTERLY,                    ///< Pay every 3 months.
    PAY_FREQ_ANNUALLY,                     ///< Pay every year.
    PAY_FREQ_BIMONTHLY,                    ///< Pay every 2 months.     

    NUM_PAYMENT_FREQUENCY                  ///< Number of payment frequency.   
} PAYMENT_FREQUENCY;

/// Credit model settings
/// @see
///     set_moodys_credit_model_settings()
typedef enum
{
    MOODYS_STANDARD_SETTINGS,        ///< Standard initial settings.
    MOODYS_DPLC_SETTINGS,            ///< DPLC model settings. Note that these settings (months to liquidation, delinquency rates, default non-performing loans flag, etc.) vary from asset class to asset class.
    MOODYS_CMM_SETTINGS,             ///< CMM model settings.
    MOODYS_MPA_SETTINGS,             ///< MPA model settings.
    MOODYS_PA_SETTINGS,              ///< PA model settings.
    MOODYS_SEDF_SETTINGS,            ///< SEDF model settings.
	MOODYS_UK_MPA_SETTINGS,			 ///< MPA model settings for UK.

    NUM_MOODYS_CREDIT_MODEL_SETTINGS ///< number of credit model settings.
} MOODYS_CREDIT_MODEL_SETTINGS;

/// Analysis type for MPA.
/// @exptl Subject to change
/// @see set_mpa_analysis_type()
enum MPA_ANALYSIS_TYPE
{
    MPA_LOSS_SIMULATION = 0,        ///< Runs MPA credit model with loss simulation analysis. Typically runs 10000+ economic paths.
    MPA_MEDC_SINGLE_PATH,           ///< Runs MPA credit model with single path. It is run of a given MEDC scenario. MEDC scenario is predefined and in-build economic scenario and can be specified using a scenario number. The scenario number should be specified using API set_current_mpa_scenario.
    MPA_MEDC_SIMULATION,            ///< Reserved for future use.
    MPA_CUST_MEDC_SINGLE_PATH,      ///< Runs MPA credit model with single path of customized scenario. It is single path run of a user defined economic scenario. The User can provide an economic scenario path using custom economy API set_mpa_custom_scenario.
    MPA_CUST_MEDC_SIMULATION,       ///< Runs MPA credit model with MEDC simulation of customized scenario. It is simulation type analysis running 10000+ economic paths anchored to a user provided economic scenario. A user must create the economic scenario and can provide the scenario using API set_mpa_custom_scenario.
    MPA_FIXED_ECO_SIMULATION,       ///< Runs MPA credit model with fixed economy predefined simulation scenario. This analysis type executes a predefined and in-build MEDC scenario path and simulate behavior of individual loan for 10000+ times. The selected economic scenario remains fix and only behavior of each loan in the provided pool is simulated each time.The scenario number should be specified using API set_current_mpa_scenario.
    MPA_FIXED_CUST_SIMULATION,      ///< Runs MPA credit model with fixed user defined simulation of customized scenario. This analysis type executes user defined scenario path and simulate behavior of individual loan for 10000+ times. The user defined scenario remains fix and only behavior of each loan in the provided pool is simulated each time. The User can provide an economic scenario path using API set_mpa_custom_scenario.

    NUM_MPA_ANALYSIS_TYPE           ///< Number of MPA analysis type.
};

/// Multiplier type for MPA
/// @exptl Subject to change
/// @see set_mpa_multiplier()
enum MPA_MULTIPLIER_TYPE
{
    MPA_MULTIPLIER_PREPAY = 0,      ///< Prepay multiplier.
    MPA_MULTIPLIER_DEFAULT,         ///< Default multiplier.
    MPA_MULTIPLIER_SEVERITY,        ///< Severity multiplier.

    NUM_MPA_MULTIPLIER_TYPE,        ///< Number of MPA multiplier type.
};

/// The stress range type for MPA
/// @exptl Subject to change
/// @see set_mpa_stress_range()
enum MPA_ANALYSIS_PARAM {
    MPA_ANALYSIS_PARAM_PREPAY,		///< Indicating set cap and floor for prepay.
    MPA_ANALYSIS_PARAM_DEFAULT,		///< Indicating set cap and floor for default.
    MPA_ANALYSIS_PARAM_SEVERITY,	///< Indicating set cap and floor for severity.
};

/// The offset type(LTV or FICO) for MPA.
/// @exptl Subject to change
/// @see set_mpa_offset()
enum MPA_ANALYSIS_PARAM_OFFSET {
    MPA_ANALYSIS_PARAM_OFFSET_LTV,  ///< Indicating set LTV offset.
    MPA_ANALYSIS_PARAM_OFFSET_FICO, ///< Indicating set FICO offset.
};

/// Analysis type for PA.
/// @exptl Subject to change
/// @see set_pa_analysis_type()
enum PA_ANALYSIS_TYPE
{
    PA_LOSS_SIMULATION = 0,         ///< Standard Loss Simulation
    PA_MEDC_SINGLE_PATH,            ///< Runs PA credit model with single path.It is run of a given MEDC scenario.MEDC scenario is predefined and in-build economic scenario and can be specified using a scenario number. The scenario number should be specified using API set_current_pa_scenario.
    PA_CUST_MEDC_SIMULATION,        ///< Runs PA credit model with MEDC simulation of customized scenario. It is simulation type analysis running 10000+ economic paths anchored to a user provided economic scenario. A user must create the economic scenario and can provide the scenario using API set_pa_custom_scenario.
    PA_CUST_MEDC_SINGLE_PATH,       ///< Runs PA credit model with single path of customized scenario.It is single path run of a user defined economic scenario. The User can provide an economic scenario path using custom economy API set_pa_custom_scenario.
    PA_FIXED_ECO_SIMULATION,        ///< Runs PA credit model with fixed economy predefined simulation scenario. The scenario number should be specified using API set_current_pa_scenario.
    PA_FIXED_CUST_SIMULATION,       ///< Runs PA credit model with fixed user defined simulation of customized scenario. The User can provide an fixed economic scenario using API set_pa_custom_scenario.
    NUM_PA_ANALYSIS_TYPE            ///< Number of PA analysis type.
};

/// Non performing loans status array index.
/// @exptl Subject to change
/// @see 
///     set_default_non_performing_loans
typedef enum
{
    NON_PERFORMING_DELINQUENT,
    NON_PERFORMING_BANKRUPTED,
    NON_PERFORMING_REO,
    NON_PERFORMING_FORECLOSED,
    NON_PERFORMING_SIZE
} NON_PERFORMING_STATUS;

/// @brief This structure stores information about tranche nullified type.
enum TRANCHE_NULLIFICATION_TYPE {
    NULL_TYPE_NO,
    NULL_TYPE_PAY,
    NULL_TYPE_FULL
};

/// @brief This structure stores information about deal accounts (reserve, insurance, liquidity) in SFW deals.
/// @see
///     get_deal_account_avail()
typedef struct
{
    char id[11];			///< Account ID
    short acct_type;		///< One of the enums from ::DEAL_ACCOUNT_TYPES
    char currency;			///< Currency code, is one of:
                            ///< ASCII | Value | Meaning
                            ///< ------|-------|-------
                            ///<  69   |  'E'  | EUR, Euros
                            ///<  70   |  'F'  | CHF, Swiss Francs
                            ///<  75   |  'K'  | SEK, Swedish Kronors
                            ///<  76   |  'L'  | GBP, British Pounds
                            ///<  85   |  'U'  | USD, US Dollars
                            ///<  82   |  'R'  | RUB, Russian Rubles
                            ///<  78   |  'N'  | NOK, Norwegian krone
                            ///<  68   |  'D'  | DKK, Danish krone
                            ///<  74   |  'J'  | JPY, Japanese Yen
                            ///<  65   |  'A'  | AUD, Australian Dollar
                            ///<  72   |  'H'  | HKD, Hong Kong Dollar
                            ///<  89   |  'Y'  | CNY, Chinese Yuan
                            ///<  67   |  'C'  | CAD, Canadian Dollar
                            ///<  90   |  'Z'  | Other,for non-mapped currency
                            ///<
    double orig_balance;	///< Original balance
    double orig_max;		///< Original max balance
    double current_balance;	///< Current balance
    double current_max;		///< Current max balance
    short day_count;		///< Calendar convention code 
    short index;			///< Code for reference rate
    double margin;			///< Margin rate adding to reference rate
    short fee_type;			///< Index of fee type
    BOOLYAN is_default;		///< If current account is default
    int default_date;		///< Default rate if the account is default
    char insurer[49];		///< Name of the insurer
    char label[31];			///< Reserve for future usage
} DEAL_ACCOUNT_INFO;


/// @brief Simplified Supervisory Formula Approach (SSFA) calculation.
/// @exptl Subject to change
/// @see
///     get_moodys_ssfa_calc()
typedef struct
{
    double Kg;      ///< Weighted average (unpaid principal for weighting) total capital requirement of the underlying exposures
                    ///< Call get_bond_flow_ex() with identifier ::FLOW_BOND_SSFA_KG to get projections.
                    ///<  - must be a decimal between 0 and 1
    double W;       ///< Ratio of sum of dollar amounts of any underlying exposures default or 90+ days delinquent over the balance of underlying exposures
                    ///< Call get_bond_flow_ex() with identifier ::FLOW_BOND_SSFA_W to get projections.
                    ///<  - must be a decimal between 0 and 1
    double A;       ///< Attachment point of exposure, the threshold at which credit losses will first be allocated to the position.
                    ///< Call get_bond_flow_ex() with identifier ::FLOW_BOND_ATTACHMENT to get projections.
                    ///<  - must be a decimal between 0 and 1
    double D;       ///< Detachment point of exposure, the threshold at which credit losses of principal allocated to the position would result in total loss of principal.
                    ///< Call get_bond_flow_ex() with identifier ::FLOW_BOND_DETACHMENT to get projections.
                    ///<  - must be a decimal between 0 and 1
    double Ps;      ///< Percentage of structured assets in the underlying exposures
                    ///<  - must be a decimal between 0 and 1
    double Ks;      ///< Weighted average capital charge on the underlying structured securities
                    ///<  - must be a decimal between 0 and 1
    double Ka;      ///< Consolidated total capital requirement of the underlying exposures
                    ///< Call get_bond_flow_ex() with identifier ::FLOW_BOND_SSFA_KA to get projections.
                    ///<
                    ///<        Ka = (1-W)*Kg + 0.5*W
                    ///<
    double Kssfa;   ///< The SSFA formula used for capital charge calculation (see RW).
                    ///< Call get_bond_flow_ex() with identifier ::FLOW_BOND_SSFA_KSSFA to get projections.
                    ///<
                    ///<        Kssfa = [e^(a*u)-e^(a*l)]/[a*(u-l)]
                    ///<
                    ///< Where
                    ///<
                    ///<        a = -1/(p*Ka)
                    ///<        u = D - Ka
                    ///<        l = max(A - Ka, 0)
                    ///<
                    ///< Where p is the resecuritization factor:
                    ///<
                    ///<        p = 0.5 for securitization positions, or for resecuritization positions that have one single asset
                    ///<        p = 1.5 for all other resecuritization positions
                    ///<
    double RW;      ///< Risk weight based on SSFA calculation methodology.
                    ///< Call get_bond_flow_ex() with identifier ::FLOW_BOND_SSFA_RW to get projections.
                    ///<
                    ///<        If  [Ka] >= [D] then
                    ///<            [RWCap] '1250%
                    ///<        Elseif  [A] >= [Ka] then
                    ///<            If [Kssfa_Cap] < [RWFloor] then
                    ///<                [RWFloor] '20%
                    ///<            Else
                    ///<                [Kssfa_Cap]
                    ///<            End If
                    ///<        Elseif [Opt3] < [RWFloor] Then
                    ///<            [RWFloor]
                    ///<        Else
                    ///<            [Opt3]
                    ///<        End If
                    ///<
                    ///< Where
                    ///<
                    ///<        [Kssfa_Cap] = Kssfa*RWCap
                    ///<        [Opt3] = ((Ka-A)/(D-A)*RWCap)+((D-Ka)/(D-A)*Kssfa*RWCap)
                    ///<
                    ///< CapCharge, the percentage used to determine the capital charge for the security, is
                    ///<
                    ///<        CapCharge = RW/1250%
                    ///<
} MOODYS_SSFA_CALC;

/// @brief This structure stores historical information of a given bond.
/// @see
///     * get_moodys_bond_history()
///     * get_moodys_bond_history_avail_YYYYMMs()
typedef struct
{
    int month; 					///< Month since origination
    double ending_balance; 		///< Ending balance
    double coupon; 				///< Coupon rate
    double bond_factor; 		///< Bond factor 
    double principal_losses;	///< Principal losses
    double cumu_prin_losses;	///< Cumulative principal losses
    double paid_interest; 		///< Interest paid out 
    double sub_amount; 			///< Subordination amount
    double sub_percentage; 		///< Subordination percentage 
    double interest_loss;       ///< Interest shortfall
    double beginning_balance;   ///< Beginning balance
    double deferred_interest;   ///< Deferred interest
    double cumu_interest_losses;///< Cumulative interest losses
} MOODYS_BOND_HISTORY;

/// @brief This structure is passed to get coupon step up date and trigger.
typedef struct
{
	char trigger_name[21];		/// This is trigger name.
	short date_flag;            /// If TRUE, set_up_date is used, else set_up_date is not used.
	int set_up_date;			/// This is the date determine when the step executes.
	short reset_freq;			/// The number of months between interest rate resets. 
	double coupon;				/// This is coupon. 
	char int_type[20];			/// This is description of the interest type. 
	double floater_spread;      /// This is added to the index rate* multiplier. 
	short  index_type;			/// The market index used to calculate the coupon. Available values: INDEX_TYPE_EX and INDEX_TYPE. 
	double floater_multiplier;	///	This is multiplied by the index rate to calculate the bond coupon at reset. 
	double swap_spread;         /// This is added to the swap index rate* multiplier. 
	short  swap_index_type;     /// The swap index used to calculate the coupon. Available values: INDEX_TYPE_EX and INDEX_TYPE. 
	double swap_multiplier;	    /// This is multiplied by the swap index rate to calculate the bond coupon at reset.
	double floater_cap;			/// Floaters Only: The maximum interest rate for this bond
	double floater_floor;		///	Floaters Only: The minimum interest rate for this bond.
}BOND_STEP_UP_COUPON;

/// @brief Additional bond information.
/// @exptl Subject to change
/// @see
///     get_bond_info_by_tranche_ex()
///     get_bond_info_by_index_ex()
typedef struct
{
	char  tranche_name[20];
    int   vintage_year;             ///< vintage year.
	double subordinate_pct;         ///< percentage of subordination. Available in SFW and CDONET engine.
    double reserve_account_pct;     ///< percentage of reserve account support. Only available in SFW engine.
    char CUSIP[6][10];              ///< CUSIP.
    char ISIN[6][16];               ///< ISIN.
    int first_coupon_date;          ///< First payment after origination, format "YYYYMMDD".
    int issue_date;                 ///< issue date.
    double issue_price;             ///< issue price.
    char bond_type[20];				///< principal type.
    char payment_date_code[11];					///< payment date code
	TRANCHE_NULLIFICATION_TYPE nullified;		///< Tranche nullified value.
	char moodys_bond_id[11];				///< Moodys Tranche ID
} MOODYS_BOND_INFO;

/// @brief This structure stores historical information of a given pool group.
/// @see
///     * get_moodys_pool_history()
///     * get_moodys_pool_history_avail_YYYYMMs()
typedef struct
{
    int month; 								///< Month since origination
    int loans; 								///< Number of loans in the pool group
    double ending_balance; 					///< Ending balance
    double CDR_1M;                          ///< Weighted average of the historical 1 month CDR rates
    double CDR_3M;                          ///< Weighted average of the historical 3 months CDR rates
    double CDR_6M;                          ///< Weighted average of the historical 6 months CDR rates
    double CDR_12M;                         ///< Weighted average of the historical 12 months CDR rates
    double end_del_bal_2M;                  ///< Ending delinquency balance 30-59 days
    double end_del_bal_3M;                  ///< Ending delinquency balance 60-89 days
    double end_del_bal_4M;                  ///< Ending delinquency balance 90-119 days
    double end_del_bal_3M_plus;             ///< Ending delinquency balance 90+ days
    double serious_del_bal;                 ///< Serious delinquencies balance
    double serious_del_bal_pct;             ///< Serious delinquencies balance percentage
    double REO_bal;                         ///< REO Balance
    double begin_WAC;                       ///< Beginning weighted average coupon
    double cumu_prin_losses;                ///< Cumulative principal losses
    double WAM;                             ///< Weighted average maturity
    double CPR_1M;                          ///< Weighted average of the historical 1 month CPR rates
    double CPR_3M;                          ///< Weighted average of the historical 3 months CPR rates
    double CPR_6M;                          ///< Weighted average of the historical 6 months CPR rates
    double CPR_12M;                         ///< Weighted average of the historical 12 months CPR rates
    double chargeoff_severity;              ///< Charge off severity
    double chargeoff_severity_3M;           ///< Charge off severity over 3 months
    double WALA;                            ///< Weighted average loan age
    double bankruptcy_bal;                  ///< Balance in bankruptcy
    double foreclosed_bal;                  ///< Balance in foreclosure
    double fee_by_servicer;                 ///< Service fees rate by servicer
    double chargeoff_proceeds;              ///< Charge off proceeds
    int delinq_trigger_breached;            ///< Number of delinquency trigger breached
    int cumu_delinq_trigger_breached;       ///< Number of cumulative delinquency trigger breached
    int delinq_cumu_loss_trigger_breached;  ///< Number of cumulative delinquency loss trigger breached
    double vol_CPR_1M;                      ///< Voluntary weighted average of the historical 1 month CPR rates
    double vol_CPR_3M;                      ///< Voluntary weighted average of the historical 3 months CPR rates
    double vol_CPR_6M;                      ///< Voluntary weighted average of the historical 6 months CPR rates
    double vol_CPR_12M;                     ///< Voluntary weighted average of the historical 12 months CPR rates
    double invol_CPR_1M;                    ///< Involuntary weighted average of the historical 1 month CPR rates
    double invol_CPR_3M;                    ///< Involuntary weighted average of the historical 3 months CPR rates
    double invol_CPR_6M;                    ///< Involuntary weighted average of the historical 6 months CPR rates
    double invol_CPR_12M;                   ///< Involuntary weighted average of the historical 12 months CPR rates
    double CDR_lifetime;                    ///< CDR rate since issuance 
    double delinq_trigger_threshold_pct;    ///< Threshold percentage of delinquency trigger 
    double delinq_trigger_curr_level_pct;   ///< Current level percentage of delinquency trigger
    double cumu_loss_trigger_threshold_pct; ///< Threshold percentage of cumulative loss trigger
    double cumu_loss_trigger_curr_level_pct;///< Current level percentage of cumulative loss trigger 
    double vol_CPR_lifetime;                ///< Voluntary CPR rate since issuance
    double invol_CPR_lifetime;              ///< Involuntary CPR rate since issuance
    double CPR_lifetime;                    ///< Reserve for future use
    double periodic_loss;                   ///< Reserve for future use
} MOODYS_POOL_HISTORY;

/// @brief This structure describes borrower benefit for a student loan.
/// @see
///     MOODYS_STUDENT_LOAN_INFO
typedef struct
{
    short benefit_type;                 ///<  Type of the borrower benefit:
                                        ///<  Value | Meaning
                                        ///<  ------|-----------
                                        ///<   0   | Interest benefit with ACH transfer;
                                        ///<   1   | Interest benefit with on time payment
                                        ///<   2   | Principal benefit
                                        ///< 
    double rate;                        ///< Benefit rate
    short month;                        ///< Number of months to be qualified for the benefit
    BOOLYAN int_reamort;                ///< Indicates if the interest benefit can be reamortized or not
    short prin_cal_from;                ///< Indicates which principal balance value should be taken for principal benefit calculation (0 for original balance; 1 for current balance), only available with benefit type=2(principal) 
} BORROWER_BENEFIT_ELIGIBILITY;

/// @brief This structure stores loan level information for a student loan.
/// @see
///     view_moodys_student_loan_info()
typedef struct
{
    short loan_type;        ///< Type of the loan:
                            ///< Value | Meaning
                            ///< ------|--------    
                            ///<   0  | FFELP
                            ///<   1  | Stafford
                            ///<   4  | Consolidation
                            ///<   6  | PLUS
                            ///<   7  | SLS
                            ///<   9  | HEAL
                            ///<   10 | MedLoans
                            ///<   11 | Law Access
                            ///<   12 | MBA Access
                            ///<   15 | Private
                            ///<   16 | Other
                            ///<   19 | TERI
                            ///<
    BOOLYAN subsidized;                                 ///< Indicates whether interest is subsidized by the government
    short deferment_code;                               ///< Deferment type of the loan:
                                                        ///< Value | Meaning
                                                        ///< ------|--------
                                                        ///<  1    | defer both interest and principal;
                                                        ///<  2    | defer principal only.
    short int_cap_period;                               ///< Indicates the periodicity of interest capitalization:
                                                        ///< Value | Meaning
                                                        ///< ------|--------
                                                        ///<   0   | monthly
                                                        ///<   3   | quarterly
                                                        ///<   6   | semi-annually
                                                        ///<   12  | annually
                                                        ///< 
    double min_payment;                                 ///< Minimum payment for the loan
    short input_payment_type;                           ///< Schedule payment type:
                                                        ///< Value | Meaning
                                                        ///< ------|--------
                                                        ///<  1    | the loan has a level payment for principal and interest;
                                                        ///<  2    | the loan has a payment schedule for principal;
                                                        ///<  3    | the loan has payment schedule for both principal and interest
                                                        ///<  
    short input_payment_from;                           ///< Indicates when schedule payment begins
    double input_payment_amount[MAX_PERIODS];           ///< Payment schedule
    BOOLYAN borrower_reduct_at_default;                 ///< If number of borrower will be reducted when loan defaults
    double repay_gross_margin;                          ///< Gross margin in repayment period
    double orig_accrued_int;                            ///< Original accrued interest
    double update_accrued_int;                          ///< Updated accrued interest
    short orig_nonpyament_term;                         ///< Original non-payment term
    short update_nonpayment_term;                       ///< Updated non-payment term
    short orig_repay_term;                              ///< Original repayment term
    short update_repay_term;                            ///< Updated repayment term
    STUDENT_LOAN_STATE orig_loan_state;                 ///< Original loan status
    short orig_months_in_repay;                         ///< Original months in repay
    short update_months_in_repay;                       ///< Updated months in repay
    short orig_borrower_count;                          ///< Original borrower number
    short update_borrower_count;                        ///< Updated borrower number
    BORROWER_BENEFIT_ELIGIBILITY int_benefit_info[6];   ///< Borrower interest benefit
    BORROWER_BENEFIT_ELIGIBILITY prin_benefit_info;     ///< Borrower principal benefit
    double prin_benefit_applied;                        ///< Amount of principal benefit applied
    BOOLYAN sap_eligible;                               ///< Indicates whether the loan is eligible for special allowance payment
    BOOLYAN sap_in_deferment;                           ///< Indicates whether sap is paid in deferment
    BOOLYAN excess_int_to_doe;                          ///< Indicates whether excess interest is paid to DOE (Department of Education)
    short sap_index;                                    ///< Index used for special allowance payment
    double defer_sap_margin;                            ///< Sap margin in deferment
    double repay_sap_margin;                            ///< Sap margin in repayment
    BOOLYAN only_pay_sap_for_loans_at_cap;              ///< Indicate if special allowance payment is only paid at cap
} MOODYS_STUDENT_LOAN_INFO;

/// @brief This struct stores CDO test information.
/// @exptl Subject to change
/// @see
///     get_cdo_test_info()
typedef struct
{
    char name[50];                     ///< Test name. For IC and PV test it is an tranche name. For user defined test, it is an defined name.
    short level;                       ///< Test level.
    TEST_TYPE test_type;               ///< Type of the test.
    double threshold_pct;              ///< Test threshold.
    double actual_pct;                 ///< Calculated percentage value.
    BOOLYAN result;                    ///< Test result.
    double tr_pct;                     ///< Trustee report percentage value.
    BOOLYAN tr_result;                 ///< Trustee report test result.
	char test_var[25];				   ///< The test script variable specified as part of the deal model
} CDO_TEST_INFO;

/// @brief This struct stores the test projection information.
/// @exptl Subject to change
/// @see
///     get_cdo_test_flow()
typedef struct
{
    int size;                              ///< Size of the projection.
    int date[MAX_PERIODS];                 ///< Date of the test.
    double threshold_pct[MAX_PERIODS];     ///< Test threshold.
    double actual_pct[MAX_PERIODS];        ///< Calculated percentage value.
    int result[MAX_PERIODS];               ///< Test result.
    double cure_amount[MAX_PERIODS];       ///< Test remaining cure amount.
    double idt_buy[MAX_PERIODS];           ///< Test Interest Diversion Test BUY.
} CDO_TEST_FLOW;

/// @brief This struct stores the date info of the CDOnet Deal.
/// @exptl Subject to change
/// @see
///     get_cdo_date_info()
typedef struct
{
    int closing_date;                       ///< Deal closing date, format "YYYYMMDD".
    int non_call_end_date;                  ///< Non-call end date, format "YYYYMMDD".
    int auction_call_date;                  ///< Auction call date, format "YYYYMMDD".
    int reinvestment_end_date;              ///< Reinvestment end date, format "YYYYMMDD".
    int maturity_date;                      ///< Deal maturity date, format "YYYYMMDD".
    int num_pay_periods;                    ///< Indicates how many valid determination/payment dates have been populated.
    int determination_dates[MAX_PERIODS];   ///< Determination dates, format "YYYYMMDD".
    int pay_dates[MAX_PERIODS];             ///< Pay dates, format "YYYYMMDD".
    int next_callable_date;                 ///< Next callable date, format "YYYYMMDD".
    int EOD_date;                           ///< EOD date, format "YYYYMMDD".
	int redemption_date;					///< redemption date, format "YYYYMMDD".
} CDO_DATE_INFO;

/// @brief This struct stores information of global reinvestment setting.
/// @exptl Subject to change
/// @see
///     set_global_reinvestment()
typedef struct
{
    BOOLYAN auto_reinv_end_date_override;  ///< If true, the reinvestment end date will be set to "6 months before maturity" regardless of the user input for auto_reinv_end_date 
    char auto_reinv_end_date[9];           ///< Auto reinvestment end date, of format "YYYYMMDD". 
    short term_setting;                    ///< Reinvestment term settings, refer to enum ::REINV_TERM_SETTING_TYPE 
    double ppy_pct_in_rp;                  ///< Percentage of prepayment that will be used for reinvestment during reinvestment period. 
    double ppy_pct_after_rp;               ///< Percentage of prepayment that will be used for reinvestment after reinvestment period. 
    double sched_pct_prin_in_rp;           ///< Percentage of scheduled principal that will be used for reinvestment during reinvestment period. 
    double sched_pct_prin_after_rp;        ///< Percentage of scheduled principal that will be used for reinvestment after reinvestment period. 
    double rec_pct_in_rp;                  ///< Percentage of recovery that will be used for reinvestment during reinvestment period. 
    double rec_pct_after_rp;               ///< Percentage of recovery that will be used for reinvestment after reinvestment period. 
    double curr_pct_prin_cash_in_rp;       ///< Percentage of money in current principal account that will be used for reinvestment during reinvestment period. 
    double curr_pct_prin_cash_after_rp;    ///< Percentage of money in current principal account that will be used for reinvestment after reinvestment period. 
    BOOLYAN use_global_reinvestment_override; ///< True if the user wants to use deal periodicity/currency/benchmark/reset period in the asset pool setting.
    short num_global_libor_floor;          ///< The length of the vector that has been passed by users to global_libor_floor, 0 means not use global libor floor for assets 
    double global_libor_floor[MAX_PERIODS];///< Libor floor vector for all assets 
} GLOBAL_REINVESTMENT_INFO;

/// @brief This struct stores asset information for global reinvestment.
/// @exptl Subject to change
/// @see
///     set_global_reinvestment()
typedef struct
{
    short num_allocation_ratio;            ///< The length of the vector that has been passed by users to allocation_ratio.
    double allocation_ratio[MAX_PERIODS];  ///< Allocation ratio of the asset in the reinvestment portfolio.
    char currency[4];                      ///< Standard currency code of the asset,refer to ISO 4217.
    short term;                            ///< Term of the asset.
    short pay_freq;                        ///< Payment frequency of the asset, refer to ::PAYMENT_FREQUENCY.
    short day_count;                       ///< Day count convention, must be one of:
                                           ///<  - DAYCOUNT_ACTUAL_360
                                           ///<  - DAYCOUNT_ACTUAL_365
                                           ///<  - DAYCOUNT_ACTUAL_ACTUAL
                                           ///<  - DAYCOUNT_30_360
                                           ///<  - DAYCOUNT_30_365

    BOOLYAN io;                            ///< Interest only.
    BOOLYAN po;                            ///< Principal only.
    BOOLYAN adjustable;                    ///< Indicates if the asset if floating or not.
    double coupon;                         ///< Coupon of the asset.
    double leverage;                       ///< Leverage for coupon calculation.
    short index;                           ///< Reference index for coupon.
    short num_margin;                      ///< This field indicates how many payment date margins have been input by the user. If the value is set to 0, API will use the value in margin[0] as the fixed margin for the asset.
    double margin[MAX_PERIODS];            ///< Coupon margins used for the corresponding payment dates of an asset. If the update date of the deal is 01/Jan/2013, and the asset pays quarterly, then margin[0] will be the margin used to calculate interest for 01/Apr/2013, margin[1] for 01/Jul/2013.
    double cap;                            ///< Coupon cap.
    double floor;                          ///< Coupon floor.
    double libor_floor;                    ///< Libor floor.
    char first_reset[9];                   ///< First reset date, of format "YYYYMMDD".
    short reset_freq;                      ///< Rate reset frequency.
    double market_price;                   ///< Market price of the asset, an input of 99 means the market price of the asset is 99.
    double recovery_rate;                  ///< Recovery rate of the asset, an input of 0.7 means the recovery rate is 70%.
    BOOLYAN bank_loans;                    ///< CBO Asset Check Bank Loans.
    short seniority;                       ///< Seniority of the asset, refer to enum ::ASSET_SENIORITY.
    char country[4];                       ///< The three-character country codes, please refer to ISO 3166-1 alpha-3.
    char moodys_rating[5];                 ///< Reported Moodys Rating, should be one of:
                                           ///<  - "Aaa",  "Aa1",  "Aa2",  "Aa3",   "A1",  "A2",  "A3",
                                           ///<  - "Baa1",  "Baa2", "Baa3",   "Ba1",  "Ba2",  "Ba3",   "B1",  "B2",  "B3",
                                           ///<  - "Caa1",  "Caa2", "Caa3",   "Ca",   "C",
                                           ///<  - "D",     "LD",   "WR",
    char company_name[50];                 ///< Company name.
} GLOBAL_REINVESTMENT_ASSET_INFO;

/// Call option Type
/// @exptl Subject to change
/// @see 
///     set_call_option
typedef enum
{
    RUN_TO_MATURITY = 0,                   ///< Run the deal to maturity
    FORCED_EARLIEST_CALL,                  ///< Call the deal if either the clean-up call or the step-up call conditions are met (even if the collateral available is not enough to cover the senior tranches)
    EARLIEST_CALL,                         ///< Call the deal if either the clean-up call or the step-up call conditions are met and the collateral available is enough to cover the senior tranches
    CLEANUP_CALL,                          ///< Call the deal if the clean-up call conditions are met and the collateral available is enough to cover the senior tranches
    FORCED_CLEANUP_CALL,                   ///< Call the deal if the clean-up call conditions are met (even if the collateral available is not enough to cover the senior tranches)
    STEPUP_CALL,                           ///< Call the deal if the step-up call conditions are met and the collateral available is enough to cover the senior tranches
    FORCED_STEPUP_CALL,                    ///< Call the deal if the step-up call conditions are met (even if the collateral available is not enough to cover the senior tranches)
    NUM_CALL_OPTION_TYPE                   ///< Number of call option types
} CALL_OPTION_TYPE;

/// Simulation engine type
/// @exptl Subject to change
/// @see 
///     set_simulation_engine
typedef enum
{
    SIMULATION_MONTE_CARLO = 0,                   ///< Monte carlo
    SIMULATION_DEFAULT_PROBABILITY_DISTRIBUTION,  ///< Default probability distribution
    NUM_SIMULATION_TYPE                           ///< Number of simulation types
} SIMULATION_TYPE;

/// Monte carlo optimization types
/// @exptl Subject to change
/// @see 
///     MONTE_CARLO_ASSUMPTION
typedef enum 
{
    MC_OPTIMIZATION_NONE = 0,      ///< No optimization
    MC_OPTIMIZATION_PATHS,         ///< Percentage of Paths
    MC_OPTIMIZATION_TAIL_RUN,      ///< Percentage of Tail Run
    NUM_MC_OPTIMIZATION            ///< Number of optimization
} MONTE_CARLO_OPTIMIZATION;

/// @brief This struct stores settings of monte carlo assumption.
/// @exptl Subject to change
/// @see
///     run_monte_carlo_simulation()
typedef struct
{
    short mode;                     ///< simulation mode, 0 for "Auto" and 1 for "Input".
    short num_paths;                ///< number of paths for the simulation to run 
    short optimization_type;        ///< options for the simulation to run faster. refer to enum ::MONTE_CARLO_OPTIMIZATION 
    double optimization_pct;        ///< percentage of paths or tail runs for the optimization
    short reinv_pool;               ///< pool used for reinvestment, 0 for "REINV" and 1 for "CURRENT" 
    BOOLYAN default_to_reinv;       ///< choose to whether apply default to "REINV" pool.
} MONTE_CARLO_ASSUMPTION;

/// Monte carlo default types
/// @exptl Subject to change
/// @see 
///     MONTE_CARLO_DEF_PPY_REC_ASSUMPTION
typedef enum 
{
    ASSET_DEFAULT_PROBABILITY = 0,     ///< Use loan level default probability set by set_loan_edf()
    MOODYS_RATING_DEFAULT_PROBABILITY, ///< Use loan level default probability base on Moodys rating 
    FITCH_RATING_DEFAULT_PROBABILITY,  ///< Use loan level default probability base on Fetch rating
    INDUSTRY_DEFAULT_PROBABIILTY,      ///< Reserve for future use
    NUM_MC_DEFAULT_TYPE                ///< Number of default probability source
} MONTE_CARLO_DEFAULT_TYPE;

/// @brief This struct stores settings of monte carlo economy assumption.
/// @exptl Subject to change
/// @see
///     run_monte_carlo_simulation()
typedef struct
{
    short correlation_type;                   ///< choose which correlation matrix to use, 0 for "Industry Correlation", 1 for "Portfolio Correlation" and 2 for "Global Correlation(SFW)".
    double intra_industry_correlation;        ///< correlation for assets who are in the same industry. 
    double inter_industry_correlation;        ///< correlation for assets whose industries are not in the industry matrix.
    double global_correlation;                ///< global correlation for SFW.
    short default_probability_source;         ///< choose what data source to use for the default probability, refer to enum ::MONTE_CARLO_DEFAULT_TYPE.  
    double default_probability_multiplier;    ///< multiplier for the default probability.
    int ppy_type;                             ///< type of prepayment curve.
                                              ///<         - PREPAY_CURVE_PSA - Standard prepayment curve measuring for prepayments in the residential mortgage market. 
                                              ///<         - PREPAY_CURVE_SMM - Monthly prepayment or default rate.
                                              ///<         - PREPAY_CURVE_CPR - Constant Prepayment Rate(CPR): Prepayment percentage expressed as an annual compounded rate.
                                              ///<         - PREPAY_CURVE_CPY - Constant Prepayment Yield(CPY): It is equivalent to the Constant Prepayment Rate(CPR) except that it assumes prepayment only happens after contractual lockout and yield maintenance period.
                                              ///<         - PREPAY_CURVE_HEP - Home Equity Prepayment: A measure of prepayments for closed-end, fixed rate HEL loans. This curve accounts for the faster seasoning ramp for home equity loans.
                                              ///<         - PREPAY_CURVE_ABS - Asset-Backed Securities(ABS): It is used in ABS markets, where prepayments differ significantly from standard mortgages. This model defines an increasing sequence of monthly prepayment rates, which correspond to a constant absolute level of loan prepayments in all future periods.
    double ppy_pct;                           ///< prepayment rate.
    double rec_vol;                           ///< volatility for recovery rate.
    double global_rec_correlation;            ///< global recovery correlation.
    double asset_def_rec_correlation;         ///< asset default-recovery correlation.
    short copula;                             ///< copula function, 0 for Gaussian and 1 for Student-T.
    short degrees_of_freedom;                 ///< degrees of freedom.
                                              ///<         - 0 for degree 3,  1 for degree 4,  2 for degree 5,   3 for degree 6
                                              ///<         - 4 for degree 7,  5 for degree 8,  6 for degree 9,   7 for degree 10
                                              ///<         - 8 for degree 12, 9 for degree 15, 10 for degree 30, 11 for degree 60
    bool use_random_seed;                      ///< use random seed or not.  
    int random_number_seed;                   ///< random number seed.
} MONTE_CARLO_DEF_PPY_REC_ASSUMPTION;

/// @brief This struct stores results of monte carlo run.
/// @exptl Subject to change
/// @see
///     get_monte_carlo_result()
typedef struct
{
    char tranche_name[20];	                ///< name of the tranche
    double total_cashflow;	                ///< total cashflow received by the tranche
    double total_principal;	                ///< principal received by the tranche
    double breakeven_probability;	        ///< break even probability for the tranche
    double tranche_delta;	                ///< tranche delta
    double irr_average;	                    ///< average internal rate of return
    double irr_sd;	                        ///< standard deviation of the internal rate of return
    double price_average;	                ///< average price
    double price_sd;	                    ///< standard deviation of the price
    double yield_average;	                ///< average yield
    double yield_sd;	                    ///< standard deviation of the yield
    double yield_dm_average;	            ///< average discounted margin
    double yield_dm_sd;	                    ///< standard deviation of the discounted margin
    double average_life_average;	        ///< average of the average life
    double average_life_sd;	                ///< standard deviation of the average life
    double expected_losses_average;	        ///< average expected losses
    double expected_losses_capped_average;	///< average expected capped losses
    double expected_losses_sd;	            ///< standard deviation of the expected losses
    double expected_losses_capped_sd;	    ///< standard deviation of the expected capped losses
    double accrued;                         ///< 
    double convexity_average;               ///< average of convexity
    double convexity_sd;                    ///< standard deviation of convexity
    char currency[4];                       ///< currency of tranche
                                            ///< Valid Output:
                                            ///< * "USD","GBP","CAD","DEM","UDI",
                                            ///< * "VSM","JPY","CHF","EUR","SDR",
                                            ///< * "ARS","AUD","ATS","BES","BEF",
                                            ///< * "BRL","CLP","CNY","DKK","EGP",
                                            ///< * "FIM","GRD","HKD","ISK","INR",
                                            ///< * "IDR","LUF","MXN","NZD","NOK",
                                            ///< * "PKR","PEN","PHP","RUB","SGD",
                                            ///< * "ZAR","KRW","ESP","SEK","TWD",
                                            ///< * "THB","TRL","UAH","FX1","FX2",
                                            ///< * "DFL","PLN"
    double duration_average;                ///< average of duration
    double duration_sd;                     ///< standard deviation of duration
    double modified_duration_average;       ///< average of modified duration 
    double modified_duration_sd;            ///< standard deviation of modified duration 
    double pv_at_coupon_average;            ///< average of pv at coupon
    double pv_at_coupon_sd;                 ///< standard deviation of pv at coupon
    double reimbursed_loss_average;         ///< average of reimbursed loss 
    double reimbursed_loss_sd;              ///< standard deviation of reimbursed loss  
    double total_interest;                  ///< total interest received by the tranche
} MONTE_CARLO_RESULT;


/// @brief Enum FFIEC test mode .
/// @see
///     get_bond_FFIEC_results()
typedef enum
{
    NO_BENCH_MODE = 0,                      ///< FFIEC test results base on none bench mode.
    INPUT_MODE,                             ///< FFIEC test results base on input tsy yield mode.
    BENCH_MODE,                             ///< FFIEC test results base on bench mode.
    AVG_MODE                                ///< FFIEC test results base on average life mode.
}FFIEC_TEST_MODE;

/// @brief This struct stores FFIEC tests input information.
/// @see
///     get_bond_FFIEC_results()
typedef struct
{
    FFIEC_TEST_MODE test_mode[7];           ///< FFIEC scenario test results base on what mode, must be one of the FFIEC_TEST_MODE.
    double input_tsy_yield[7];              ///< It is only required when FFIEC scenario test_mode is INPUT_MODE, FFIEC results base on input_tsy_yield.
    bool price_include_accrued;             ///< FFIEC results projected_price included accrued interest or not.
    double up_wal_limit;                    ///< Upward average life limit, default is 4.0 .
	double down_wal_limit;                  ///< Downward average life limit, default is 6.0.
    double price_change_limit;              ///< Price change limit, default is 17.0.
}FFIEC_INPUT_PARAMS;


/// @brief This struct stores results for FFIEC test.
/// @see
///     get_bond_FFIEC_results()
typedef struct
{
    bool test_result;                       ///< FFIEC test result, true is pass, false is failed.
    double average_life;                    ///< Weighted average life.
    double wal_change;                      ///< The difference between current mode wal and +0bps mode wal.
    double tsy_yield;                       ///< Treasury yield in current mode.
    double pricing_yield;                   ///< The yield used for pricing in current mode.
    double projected_price;                 ///< The market price based on pricing yield in current mode.
    double percent_change;                  ///< The percent change of current mode price based on +0bps mode price.
}FFIEC_RESULTS;

/// @brief Enum for monte carlo correlation types.
/// @exptl Subject to change
typedef enum
{
    MONTE_CARLO_CORRELATION_PORTFOLIO = 0,  ///< Correlation portfolio.
    MONTE_CARLO_CORRELATION_INDUSTRY,       ///< Correlation industry.
    NUM_MONTE_CARLO_CORRELATION_TYPE        ///< Number of monte carlo correlation type.
}MONTE_CARLO_CORRELATION_TYPE;

/// @brief Enum for Default Probability Distribution type.
/// @exptl Subject to change
typedef enum
{
    DPD_DISTRIBUTION_LOGNORMAL,             ///< Lognormal distribution.
    DPD_DISTRIBUTION_INVERSE_NORMAL,        ///< Inverse normal distribution.
    DPD_DISTRIBUTION_USER_DEFINED,          ///< User defined distribution.

    NUM_DPD_DISTRIBUTION_TYPE               ///< Number of supported distribution type.
} DPD_DISTRIBUTION_TYPE;

/// @brief Assumption for Default Probability Distribution.
/// @exptl Subject to change
typedef struct
{
    short distribution;                     ///< The default/loss probability distribution to be applied to the deal. Should be one of enum ::DPD_DISTRIBUTION_TYPE.
    short scenario_type;                    ///< Scenario type, 0 for defaults and 1 for losses.
    double mean;                            ///< Mean for the probability distribution, required when the distribution type is lognormal or inverse normal.
    double standard_deviation;              ///< Standard deviation for the probability distribution, required when the distribution type is lognormal or inverse normal.
	BOOLYAN use_milan_aaa_ce;				///< If TRUE then use the Milan Aaa CE input, use Standard Deviation if otherwise.
    double milan_aaa_ce;                    ///< Editable when the distribution type is lognormal and scenario type is losses.
    BOOLYAN discounted_recoveries;          ///< Editable when the scenario type is losses.
    double revolving_default_factors[MAX_PERIODS];  ///< Revolving default factor.
    short num_scenarios;                    ///< Number of scenarios for the simulation to run. The maximum of num_scenarios must not be greater than 1000.
	BOOLYAN use_revolving_def_timing;		///< TRUE if users want to use separate default timing curve for revolving assets. Call set_dpd_revolving_default_timing() to set the revolving default timing curve.
    short rating_cap_primary;				///< Primary proposed rating cap.
    short rating_cap_surveillance;			///< Surveillance proposed rating cap.
} DPD_ASSUMPTION;

/// @brief Scenario for Default Probability Distribution Simulation.
/// @exptl Subject to change
typedef struct
{
    short scenario_num;             ///< Index number for the scenario.
    double default_loss;            ///< Default/loss rate for the scenario.
    double scenario_probability;    ///< Probability for the scenario to occur.
    double cumulative_probability;  ///< Cumulative probability of the scenarios up to the underlying one.
} DPD_SCENARIO;


/// @brief Result for Default Probability Distribution Simulation.
/// @exptl Subject to change
typedef struct
{
    char    tranche[8];                      ///< Name of the tranche.
    double  accrued_int;                     ///< Accrued interest for the tranche.
    double  average_life_average;            ///< Average "average life" of the tranche across all scenario runs.
    double  average_life_sd;                 ///< Standard deviation the average life of the tranche across all scenario runs.
    double  base_proceeds;                   ///< Base proceeds of the tranche across all scenario runs.
    double  base_settlement_balance;         ///< Base settlement balance of the tranche across all scenario runs.
    double  convexity_average;               ///< Average convexity of the tranche across all scenario runs.
    double  convexity_sd;                    ///< Standard deviation of the convexity of the tranche across all scenario runs.
    double  duration_average;                ///< Average duration of the tranche across all scenario runs.
    double  duration_sd;                     ///< Standard deviation of the duration of the tranche across all scenario runs.
    double  modified_duration_average;       ///< Average modified duration of the tranche across all scenario runs.
    double  modified_duration_sd;            ///< Standard deviation of the modified duration of the tranche across all scenario runs.
    double  principal_loss_average;          ///< Average principal loss of the tranche across all scenario runs.
    double  principal_loss_sd;               ///< Standard deviation of the principal loss of the tranche across all scenario runs.
    double  pv_at_coupon_average;            ///< Average pv at coupon of the tranche across all scenario runs.
    double  pv_at_coupon_sd;                 ///< Standard deviation of pv at coupon of the tranche across all scenario runs.
    double  reimbursed_loss_average;         ///< Average reimbursed loss of the tranche across all scenario runs.
    double  reimbursed_loss_sd;              ///< Standard deviation of reimbursed loss of the tranche across all scenario runs.
    double  total_cashflow_average;          ///< Average total cashflow of the tranche across all scenario runs.
    double  total_cashflow_sd;               ///< Standard deviation of total cashflow of the tranche across all scenario runs.
    double  total_interest_average;          ///< Average total interest of the tranche across all scenario runs.
    double  total_interest_sd;               ///< Standard deviation of total interest of the tranche across all scenario runs.
    double  total_principal_average;         ///< Average total principal of the tranche across all scenario runs.
    double  total_principal_sd;              ///< Standard deviation of total principal of the tranche across all scenario runs.
    double  price_average;                   ///< Average price of the tranche across all scenario runs.
    double  price_sd;                        ///< Standard deviation of price of the tranche across all scenario runs.
    double  yield_average;                   ///< Average yield of the tranche across all scenario runs.
    double  yield_sd;                        ///< Standard deviation of yield of the tranche across all scenario runs.
    double  dm_average;                      ///< Average discounted margin of the tranche across all scenario runs.
    double  dm_sd;                           ///< Standard deviation of the discounted margin of the tranche across all scenario runs.
    char    currency[4];                     ///< Currency of the tranche.
                                             ///< Valid Output:
                                             ///< * "USD","GBP","CAD","DEM","UDI",
                                             ///< * "VSM","JPY","CHF","EUR","SDR",
                                             ///< * "ARS","AUD","ATS","BES","BEF",
                                             ///< * "BRL","CLP","CNY","DKK","EGP",
                                             ///< * "FIM","GRD","HKD","ISK","INR",
                                             ///< * "IDR","LUF","MXN","NZD","NOK",
                                             ///< * "PKR","PEN","PHP","RUB","SGD",
                                             ///< * "ZAR","KRW","ESP","SEK","TWD",
                                             ///< * "THB","TRL","UAH","FX1","FX2",
                                             ///< * "DFL","PLN"
                                             ///<
    double  balance;                         ///< Balance of the tranche.
    char    rating_target[5];                ///< Target rating of the tranche.
    double  wal;                             ///< Weighted average life of the tranche.
    double  expected_loss_el;                ///< Expected loss of the tranche in percentage.
    double  expected_loss_mm;                ///< Moody's metric for the given expected loss.
    char    expected_loss_rating[6];         ///< Corresponding rating for the given expected loss.
    double  principal_interest_dp;           ///< Probability of default on either principal or interest in percentage.
    double  principal_interest_mm;           ///< Moody's metric for the given probability of default.
    char    principal_interest_rating[6];    ///< Corresponding rating for the given probability of default.
    double  principal_loss_dp;               ///< Probability of default on principal only in percentage.
    double  principal_loss_mm;               ///< Moody's metric for the given probability of default.
    char    principal_loss_rating[6];        ///< Corresponding rating for the given probability of default.
    char    proposed_rating[6];              ///< Final proposed rating for the tranche.
    double  breakeven_probability;           ///< Breakeven probability.
} DPD_RESULT;

/// @brief Enum for the types of servicer advance rate.
/// @see set_service_advances_rates_type()
/// @exptl Subject to change
typedef enum
{
    SERVICER_ADVANCES_BASE_OFF = 0,          ///< The servicer advance projections are disable.
    SERVICER_ADVANCES_BASE_DELINQ = 1,       ///< Indicate vector/constant is projected against delinquent balance whose projection is set by set_addit_group_delinquencies().
    SERVICER_ADVANCES_BASE_DEFAULT           ///< Indicate vector/constant is projected against default balance.
} SERVICER_ADVANCES_BASE;

/// @brief Enum for the types of insurance rate.
/// @see set_insurance_coverage()
/// @exptl Subject to change
typedef enum
{
    INSURANCE_CLAIM_COVERAGE = 1,           ///< Indicate vector/constant is coverage claim.
    INSURANCE_CLAIM_ACCRUED                 ///< Indicate vector/constant is accrued claim.
} INSURANCE_CLAIM;

/// @brief Enum for the types of interest capitalization code override.
/// @see set_int_capital_code_override()
/// @exptl Subject to change
typedef enum
{
    INT_CAPITAL_CODE_OVERRIDE_NONE = 0,     ///< Turn off capitalization code override
    INT_CAPITAL_CODE_OVERRIDE_REPAYMENT,    ///< Change the capitalization code to repayment
    INT_CAPITAL_CODE_OVERRIDE_MONTHLY,      ///< Change the capitalization code to monthly
    INT_CAPITAL_CODE_OVERRIDE_QUARTERLY,    ///< Change the capitalization code to quarterly
    INT_CAPITAL_CODE_OVERRIDE_SEMIANNUALLY, ///< Change the capitalization code to semi-annually
    INT_CAPITAL_CODE_OVERRIDE_ANNUALLY      ///< Change the capitalization code to annually
} INT_CAPITAL_CODE_OVERRIDE;

/// @brief Enum for the types of liquidation periodicity.
/// @see set_liquidation_periodicity()
/// @exptl Subject to change
typedef enum
{
    LIQUIDATION_MONTHLY = 0,            	///< Liquidation periodicity is monthly
    LIQUIDATION_BIMONTHLY = 8,	            ///< Liquidation periodicity is bimonthly
    LIQUIDATION_QUARTERLY = 1,	            ///< Liquidation periodicity is quarterly
    LIQUIDATION_SEMI_ANNUALLY = 2,          ///< Liquidation periodicity is semi-annually
    LIQUIDATION_ANNUALLY = 3	            ///< Liquidation periodicity is annually
} LIQUIDATION_PERIODICITY_TYPE;

/// @brief Additional deal information.
/// @exptl Subject to change
/// @see
///     get_deal_info_ex()
typedef struct
{
    char full_name[100];                    ///< Full name of deal.
    char country[4];                        ///< Country code of deal.
    int period_begin_date;                  ///< Deal period begin date of format YYYYMMDD.
	int period_value;                       ///< Period value monthly = 1, quarterly = 3 etc.
    int collateral_update_date;             ///< Collateral update date of format YYYYMMDD. CDOnet engine only.
    char collateral_type[100];              ///< Collateral type.
    char placement_type[10];                ///< Placement type.
    char series[15];                        ///< Series.
	char currency[4];                       ///< Currency.
    char product_line[255];                 ///< Product line.
	double effective_date_target_par;       ///< Effective date target par.
	char manager[100];						///< Manager for CDOnet.
	char moodys_deal_id[11];				///< Moodys deal ID.
} MOODYS_DEAL_INFO;

/// @brief Enum for the types of state.
/// @see set_whole_loan()
/// @exptl Subject to change
typedef enum
{
    US_STATE_NONE,
    US_STATE_AL,  
    US_STATE_AK,  
    US_STATE_AZ,  
    US_STATE_AR,  
    US_STATE_CA,  
    US_STATE_CO,  
    US_STATE_CT,  
    US_STATE_DE,  
    US_STATE_FL,  
    US_STATE_GA,  
    US_STATE_HI,  
    US_STATE_ID,  
    US_STATE_IL,  
    US_STATE_IN,  
    US_STATE_IA,  
    US_STATE_KS,  
    US_STATE_KY,  
    US_STATE_LA,  
    US_STATE_ME,  
    US_STATE_MD,  
    US_STATE_MA,  
    US_STATE_MI,  
    US_STATE_MN,  
    US_STATE_MS,  
    US_STATE_MO,  
    US_STATE_MT,  
    US_STATE_NE,  
    US_STATE_NV,  
    US_STATE_NH,  
    US_STATE_NJ,  
    US_STATE_NM,  
    US_STATE_NY,  
    US_STATE_NC,  
    US_STATE_ND,  
    US_STATE_OH,  
    US_STATE_OK,  
    US_STATE_OR,  
    US_STATE_PA, 
    US_STATE_RI,
    US_STATE_SC,
    US_STATE_SD,
    US_STATE_TN,
    US_STATE_TX,
    US_STATE_UT,
    US_STATE_VT,
    US_STATE_VA,
    US_STATE_WA,
    US_STATE_WV,
    US_STATE_WI,
    US_STATE_WY,
    US_STATE_DC,
    US_STATE_PR
} US_STATE;

/// @brief Enum for the types of loan status.
/// @see set_whole_loan()
/// @exptl Subject to change
typedef enum
{
    LOAN_STATUS_NONE,
    LOAN_STATUS_CURRENT,
    LOAN_STATUS_UNKNOWN,
    LOAN_STATUS_PAIDOFF,
    LOAN_STATUS_DELINQUENT,
    LOAN_STATUS_FORECLOSED,
    LOAN_STATUS_BANKRUPT,
    LOAN_STATUS_REO,
    LOAN_STATUS_DEFAULTED,
    LOAN_STATUS_REPURCHASED,
    LOAN_STATUS_LIQUIDATED,
    LOAN_STATUS_CLOSED
} LOAN_STATUS;

/// @brief Enum for the whole loan default caculation method type.
/// @exptl Subject to change
typedef enum
{
	WL_NORMAL_DEFAULT_METHOD,
	DEFAULT_PATTERN_NONBINARY,
	DEFAULT_PATTERN_BINARY
} WHOLE_LOAN_DEFAULT_METHOD_TYPE;

/// @brief Enum for bank loan rating type.
/// @exptl Subject to change
typedef enum
{
	ESG_RATING_BBB,
	ESG_RATING_BB,
	ESG_RATING_B,
	ESG_RATING_CCC
} ESG_RATING_TYPE;

/// @brief Enum for bank loan term type.
/// @exptl Subject to change
typedef enum
{
	ESG_TERM_1M = 1,
	ESG_TERM_2M = 2,
	ESG_TERM_3M = 3,
	ESG_TERM_6M = 6,
	ESG_TERM_12M = 12
} ESG_RATING_TERM;

/// @brief coupon information.
/// @exptl Subject to change
typedef struct
{
    short coupon_type;             ///< Coupon type, must be one of ::WHOLE_LOAN_COUPON_TYPE
    double gross_coupon;           ///< Gross coupon of the loan
    double net_coupon;             ///< Net coupon of the loan
    int index;                     ///< The market index used to calculate the coupon. Available values: INDEX_TYPE_EX and INDEX_TYPE.        
    double margin;                 ///< Margin rate adding to reference rate.
    short first_rate_reset;        ///< Months to the first rate reset based on the date the deal is opened as of.
    short first_pay_reset;         ///< Months to the first payment reset based on the date the deal is opened as of.(SFW only)
    short reset_freq;              ///< Months between interest rate resets.
    short pay_reset_freq;          ///< Months between payment resets.(SFW only)
    double periodic_cap;           ///< Maximum absolute interest rate change per period. i.e., 2.0% means absolute rate change is at most 2.0%.(SFW only)
    double cap;                    ///< Maximum interest rate for this loan.
    double floor;                  ///< Minimum interest rate for this loan.
    short day_count;               ///< Calender must be one of
                                   ///<   - ::DAYCOUNT_ACTUAL_360
                                   ///<   - ::DAYCOUNT_ACTUAL_365
                                   ///<   - ::DAYCOUNT_ACTUAL_ACTUAL
                                   ///<   - ::DAYCOUNT_30_360
                                   ///<   - ::DAYCOUNT_30_365
                                   ///<   - ::DAYCOUNT_30_360E
    short pay_freq;                ///< Payment frequency, must be one of ::PAYMENT_FREQUENCY 
    int conversion_date_1;         ///< First conversion date after origination. format "YYYYMMDD"
    int conversion_date_2;         ///< Second conversion date after origination. format "YYYYMMDD"
	int conversion_date_3;         ///< Third conversion date after origination. format "YYYYMMDD"
	int conversion_date_final;     ///< Final conversion date after origination. format "YYYYMMDD"
    double conversion_coupon;      ///< Converted coupon of the loan
    double conversion_margin_1;    ///< First converted Margin rate adding to reference rate.
    double conversion_margin_2;    ///< Second converted Margin rate adding to reference rate. 
    double conversion_margin_3;    ///< Third converted Margin rate adding to reference rate.
    double conversion_margin_final;///< Final converted Margin rate adding to reference rate.
} COUPON_INFO;

typedef enum
{
    PAYMENT_PLAN_TENURE,           ///< Equal monthly payments as long as at least one borrower lives and continues to occupy the property as a principal residence.
    PAYMENT_PLAN_TERM,             ///< Equal monthly payments for a fixed period of months selected.
    PAYMENT_PLAN_LOC,              ///< Unscheduled payments or in installments, at times and in an amount of your choosing until the line of credit is exhausted.
    PAYMENT_PLAN_MOD_TENURE,       ///< Combination of line of credit and scheduled monthly payments for as long as you remain in the home.
    PAYMENT_PLAN_MOD_TERM          ///< Combination of line of credit plus monthly payments for a fixed period of months selected by the borrower.
} HECM_PAYMENT_PLAN;

typedef enum
{
    ANN,							///< Annuity amortization schedule.
    LIN,							///< Straight line amortization schedule.
    BUL,							///< Bullet amortization schedule.
	BULINV							///< Bullet and Investment
} WHOLE_LOAN_AMORTIZATION_TYPE;

typedef enum
{
	WL_OCCUPANCY_UNK,						///< Unknown						
    WL_OCCUPANCY_OWN,						///< Owner-occupied/Primary
	WL_OCCUPANCY_SEC, 						///< Holiday/second home
	WL_OCCUPANCY_INV						///< Non-owner-occupied/buy-to-let/Investment
} WHOLE_LOAN_OCCUPANCY_TYPE;

typedef struct
{
    short payment_plan;            ///< The payment plan (option) of HECM. Need to be one type of HECM_PAYMENT_PLAN.
    double loan_balance;           ///< HECM loan balance.
	double MIP;                    ///< Mortgage Insurance Premium: a cost for FHA mortgage insurance charged based on MCA.
    double servicing_amount;       ///< Fee charged by lenders or lenders' agents.
    short draw_term;               ///< Term in months for draw_amount.
    double draw_amount;            ///< Amount of additional money drawn during the draw period, which occurs at the beginning of the month.
    double avail_LOC;              ///< A credit limit that a borrower can draw.
    double max_claim_amount;       ///< The lesser of a home's appraised value or the maximum loan limit that can be insured by FHA.
} HECM_INFO;

/// @brief Enum for the types of coupon.
/// @see set_whole_loan()
/// @exptl Subject to change
typedef enum
{
    FIXED_COUPON,                  ///< Fixed coupon;
    FLOATING_COUPON,               ///< Floating coupon;
    FIXED_TO_FLOATING,             ///< Fixed to floating coupon;
    FLOATING_TO_FIXED,             ///< Floating to fixed coupon;
    FIXED_STEP,                    ///< Fixed step coupon;
    FLOATING_STEP,                 ///< Floating step coupon;
    FIXED_TO_FLOATING_STEP,        ///< Fixed to floating step coupon;
    FLOATING_TO_FLOATING_STEP      ///< Floating to floating step coupon;
} WHOLE_LOAN_COUPON_TYPE;

/// @brief Enum for the types of UK Region.
/// @see set_whole_loan()
/// @exptl Subject to change
typedef enum
{
	IGBR,             ///< UK
	EAMI,             ///< East Midlands
	EAST,             ///< Eastern
	LOND,             ///< London
	NEAS,             ///< North East
	NORW,             ///< North West (including Merseyside)
	NOIR,             ///< Northern Ireland
	SCTL,             ///< Scotland
	SOEA,             ///< South East
	SOWE,             ///< South West
	WALS,             ///< Wales
	WEMI,             ///< West Midlands
	YOHU              ///< Yorkshire and The Humber
} UK_REGION;

/// @brief Enum for the types of MOODYS RATING.
/// @see set_whole_loan()
/// @exptl Subject to change
typedef enum
{
	MOODYS_RATING_Aaa,             ///< Aaa
	MOODYS_RATING_Aa1,             ///< Aa1
	MOODYS_RATING_Aa2,             ///< Aa2
	MOODYS_RATING_Aa3,             ///< Aa3
	MOODYS_RATING_A1,              ///< A1
	MOODYS_RATING_A2,              ///< A2
	MOODYS_RATING_A3,              ///< A3
	MOODYS_RATING_Baa1,            ///< Baa1
	MOODYS_RATING_Baa2,            ///< Baa2
	MOODYS_RATING_Baa3,            ///< Baa3
	MOODYS_RATING_Ba1,             ///< Ba1
	MOODYS_RATING_Ba2,             ///< Ba2

	MOODYS_RATING_Ba3,             ///< Ba3
	MOODYS_RATING_B1,              ///< B1
	MOODYS_RATING_B2,              ///< B2
	MOODYS_RATING_B3,              ///< B3
	MOODYS_RATING_Caa1,            ///< Caa1
	MOODYS_RATING_Caa2,            ///< Caa2
	MOODYS_RATING_Caa3,            ///< Caa3
	MOODYS_RATING_Ca,              ///< Ca
	MOODYS_RATING_C,               ///< C
	MOODYS_RATING_D,               ///< D
	MOODYS_RATING_LD,              ///< LD
	MOODYS_RATING_WR,			   ///< WR
	MOODYS_RATING_NULL             ///< N/R
} MOODYS_RATING_TYPE;

/// @brief Enum for the types of bank loan junior-senior type.
/// @see set_whole_loan()
/// @exptl Subject to change
typedef enum
{
    NULL_JSTYPE,
    JUNIOR_JSTYPE,
    MEZZ_JSTYPE,
    SENIOR_JSTYPE,
    SRSUB_JSTYPE,
	SUB_JSTYPE,
    SR_SEC_JSTYPE,
	SR_UNSEC_JSTYPE,
    SECOND_LIEN_JSTYPE
} BANKLOAN_JUNIOR_SENIOR_TYPE;

typedef struct
{
    short occupancy;			  ///< Please review WHOLE_LOAN_OCCUPANCY_TYPE enum for types.
    short lien_type;			  ///< Line position.
	int borrower_birthday;        ///< The borrower's birthday, format "YYYYMMDD"
    double borrower_income;       ///< The borrower's household income.
    BOOLYAN first_time_buyer;     ///< The borrower is first time buyer or not.
	BOOLYAN structured_security;  ///< Whether the loan is structured security or not.
} WHOLE_LOAN_EXTEND_INFO;

/// @brief bankloan information.
/// @exptl Subject to change
typedef struct
{
	MOODYS_RATING_TYPE moodys_rating;				 ///< Moodys Rating
	ESG_RATING_TYPE esg_spotspread_rating;			 ///< The rating of the ESG spotspread path/trial, must be one of ::ESG_RATING_TYPE
    double alpha;									 ///< Adjustment for adjusting ESG_spotspread curve of different ratings in bank loan call logic
    double phi;										 ///< Adjustment for adjusting Discount Margin of loan of different ratings in bank loan call logic
}
BANKLOAN_CALL_ADJ_PARAM;

typedef struct
{
	short size;					  ///< sink fund schedule size
	int pdate[240];		  ///< sink fund payment date info
	double pprin[240];	  ///< sink fund principal amount
} WHOLE_LOAN_SINK_FUND;

typedef struct
{
	short Moodys_Rating;					///< Moody's rating for this loan. should be one of the ::MOODYS_RATING_TYPE
	short non_perfoming_flag;				///< 0 or 1. By default is 0. 1 is checked
	double recovery_rate;					///< Individual recovery rate for the loan
	double adjusted_spread;					///< adjusted spread for the loan
	char loanx_id[20];						///< loanx ID
	BANKLOAN_JUNIOR_SENIOR_TYPE seniority;	///< Seniority of the asset, refer to enum ::BANKLOAN_JUNIOR_SENIOR_TYPE
} BANKLOAN_EXTEND_INFO;

typedef struct
{
    short UK_region;			  ///< Please refer to enum UK_REGION. By default is IGBR.
	short UK_purpose;			  ///< loan purpose, only for UK loans, 1:Purchase, 2:Re-mortgage, 3:Equity Release, 4:Renovation, 5:Right to Buy, 6:Investment Mortgage, 7:Debt Consolidation.
	double fund_growth_rate;      ///< Annual growth rate of life fund or investment account. Required for BULINV mortgage,[0,25]
    double collateral_balance;    ///< The value of collateral fund for BULINV mortgage.
    double monthly_fund_deposit;  ///< Amount of money deposited for BULINV mortgage.
} UK_WHOLE_LOAN_INFO;

/// @brief Enum for the types of loan.
/// @see set_whole_loan()
/// @exptl Subject to change
typedef enum
{
    WL_RMBS,						  ///< RMBS loan type;
    WL_ABS_AUTO,					  ///< ABS Auto loan type;
    WL_ABS_STUDENT_LOAN,              ///< ABS student loan loan type;
    WL_ABS_CREDIT_CARD,               ///< ABS credit card loan type;
    WL_ABS_AUTO_LEASE,                ///< ABS auto lease loan type;
    WL_CMBS,						  ///< CMBS loan type;
    WL_CDO,						      ///< CDO loan type;
	WL_HECM,						  ///< HECM loan type;
	WL_REVERSE_MORTGAGE,			  ///< Reverse mortgae loan type;
    WL_TYPE_OTHER					  ///< other loan type;
} WHOLE_LOAN_TYPE;

/// @brief Enum for the issue type.
/// @see set_whole_loan()
/// @exptl Subject to change
typedef enum
{
    WL_FNMA = 1,							///< Issued by Fannie Mae;
    WL_FHLMC,								///< Issued by Freddie Mac;
    WL_GNMA,								///< Issued by Ginnie Mae;
    WL_NA_SUBPRIME,							///< Non-agency, Subprime underwriting;
    WL_NA_PRIME,							///< Non-agency, Prime underwriting;
    WL_ISSUER_OTHER							///< All others & unknown underwriting;
} WHOLE_LOAN_ISSUER_TYPE;

/// @brief whole loan information.
/// @exptl Subject to change
/// @see
///     set_whole_loan()
typedef struct
{
    char loan_id[10];							///< Loan id number
    short whole_loan_type;						///< whole_loan_type: type of whole loan; need to be one of "WHOLE_LOAN_TYPE"; regular mortgage cashflow projection would be applied unless being set "WL_HECM" and "WL_REVERSE_MORTGAGE", in which case HECM or Reverse Mortgage methodology would be applied.
    double current_balance;						///< Current unpaid balance (as of the date the deal is opened).       
    double original_balance;					///< Original balance (as of the date the loan is began).
    short term;									///< Maturity (in months) of the loan. This should be equal to the number of months between initial date and maturity date.
    short original_term;						///< The original term in months. This should be equal to the number of months between origination date and maturity date.
    int origination_date;						///< Origination date, format "YYYYMMDD".
    int first_pay_date;							///< First payment after origination. format "YYYYMMDD"
    int paid_through;							///< Paid through, format "YYYYMMDD".
    int maturity;								///< Loan maturity date, format "YYYYMMDD".
    double ltv;									///< Loan to value
    double original_ltv;						///< Original loan to value
    double appraisal_value;						///< Appraisal value
    short purpose;								///< Loan purpose
    char city[25];								///< City
    short state;								///< State
    char zip[6];								///< Zip
    char currency[4];							///< Standard currency code of the asset,refer to ISO 4217
    COUPON_INFO coupon_info;					///< Coupon information, see ::COUPON_INFO
    short status;								///< Loan status, must be one of ::LOAN_STATUS
    HECM_INFO hecm;								///< Hecm information, see ::HECM_INFO     
	short prin_lockout;							///< The remaining principal lockout in months (as of the date the deal is opened).
    short balloon_period;						///< The balloon period.
	char issuer_id[30];							///< The issuer ID number
	char country[4];							///< The three-character country codes, please refer to ISO 3166-1 alpha-3.
	short amortization_type;					///< Loan's amortization method. Need to be one of WHOLE_LOAN_AMORTIZATION_TYPE. By default, it is "ANN".
	short issuer_type;							///< The issuer type.
	double market_price;						///< The market price.
	char loan_id_ex[101];						///< Loan id number extension only for SFW
	BOOLYAN individual_pay_date_flag;			///< Whether enable loan level payment date or not.
	WHOLE_LOAN_EXTEND_INFO whole_loan_ex;		///< whole loan extend info
	UK_WHOLE_LOAN_INFO whole_loan_uk;			///< UK whole loan info
	BANKLOAN_EXTEND_INFO bankloan_ex;			///< bank loan info, see ::BANKLOAN_EXTEND_INFO
} WHOLE_LOAN_STRUCT;

/// @brief price_loan_ex input information.
/// @exptl Subject to change
/// @see
///     price_loan_ex()
typedef struct
{
	PRICING_ANCHORS anchorType;					///< Enum for the pricing anchors.
	double anchor_value;						///< Value for the related pricing type.
	short index;								///< This would be used when anchorType is PRICE and DM, can be null or one of INDEX_TYPE or INDEX_TYPE_EX.
	short day_count;							///< Calendar setting for discounting, need to be "DAYCOUNT", by default it is 30/360.
}
LOAN_PRICING_INPUT;

/// @brief distressed property recovery information.
/// @exptl Subject to change
/// @see
///     set_distressed_property_recovery()
typedef struct
{
	int recovery_lag;							///< The number of months from the default event on which the recovery payment is made.       
    int inflation_start_period;					///< The snapshot period (typically months from the start of the scenario) on which inflation starts.
    double inflation_rate;						///< The annual inflation rate commencing from the inflation start period.
    double distressed_property_value;			///< Specified at the loan level, this is the distressed value of the collateral for the loan.
    double variable_foreclosure_cost;			///< Specifies a portion of the property value which will comprise part of the foreclosure costs.
    double fixed_foreclosure_cost;				///< A fixed amount which will comprise part of the foreclosure costs.
}
DISTRESSED_PROPERTY_RECOVERY;

/// @brief This structure is used to provide user with loan cashflow. 
/// It includes number of cashflow points as well as loan cashflow dates.
/// @see get_loan_flow_ex()
typedef struct
{
    int     size;                                           ///< Size of the cash flow including element of 0 representing balance at the settlement date
    int     dates[MAX_PERIODS];                             ///< Array of CCYYMMDD dates. The first period stores the last payment date of the current deal update.
    double  balance[MAX_PERIODS];                           ///< Balance at the end of the period.
    double  sched_principal[MAX_PERIODS];                   ///< Scheduled principal paid.
    double  prepayments[MAX_PERIODS];                       ///< Prepayments per period
    double  defaults[MAX_PERIODS];                          ///< Defaults per period with lag.
    double  losses[MAX_PERIODS];                            ///< Losses per period
    double  prin_recoveries[MAX_PERIODS];                   ///< This is principal amount recovered when loan defaults.
    double  interest[MAX_PERIODS];                          ///< Interest paid per period
    double  cash[MAX_PERIODS];                              ///< Cash flow per period This field is the sum of interest, scheduled principal, prepayment and principal recoveries from the default loan.
    int     start_index_as_per_settle_date;                 ///< Start index as per settle date
                                                            ///<  - number of months between trade settlement date (MARKIT_DEAL_INFO::trade_settlement_date) and collateral accrual begin date (MARKIT_BOND_CASHFLOW::accrual_begin_dates[1])
                                                            ///<  - 0 if trade settlement date is the same or earlier than collateral accrual begin date
    double  draw_amount[MAX_PERIODS];                       ///< Amount of additional money drawn during the draw period, applicable for HELOCs only, ( Home Equity Line of Credit). This amount will be added into performing balance in the next period.
	double  performing_balance[MAX_PERIODS];				///< Performing balance per period.
} MOODYS_LOAN_CASHFLOW;

/// @brief This structure is used to provide user with loan cashflow. 
/// It includes number of cashflow points as well as loan cashflow dates.
/// @see get_loan_flow_ex()
typedef struct
{
    int     size;                                           ///< Size of the cash flow including element of 0 representing balance at the settlement date
    int     dates[MAX_PERIODS];                             ///< Array of CCYYMMDD dates. The first period stores the last payment date of the current deal update.
    double  balance[MAX_PERIODS];                           ///< Balance of account at end of period.
    double  withdrawal[MAX_PERIODS];						///< Withdrawal amount of account at end of period.
    double  deposit[MAX_PERIODS];							///< Deposit amount of account at end of period.
    double  target[MAX_PERIODS];                            ///< Target amount of account at end of period.
    double  interest[MAX_PERIODS];                          ///< Interest amount of account at end of period.
    double  deferred_interest[MAX_PERIODS];                 ///< Deferred Interest of account at end of period.
    double  commit_fee[MAX_PERIODS];                        ///< Commit_fee of account at end of period.
    double  deferred_fee[MAX_PERIODS];                      ///< Deferred_fee of account at end of period.
} MOODYS_ACCOUNT_CASHFLOW;

/// @brief Enum for swap notional code.
/// @see MOODYS_HEDGE_STRUCT
typedef enum 
{
    SWAP_FIXED_LOAN_NOTIONAL,        ///< Fixed swap
    SWAP_FLOATING_LOAN_NOTIONAL,     ///< Floating swap
    SWAP_ALL_ASSETS_NOTIONAL,        ///< All loans swap
    SWAP_CUSTOM_NOTIONAL,            ///< Custom swap
    SWAP_CAP_NOTIONAL,               ///< Caps swap
    SWAP_FLOOR_NOTIONAL,             ///< Floors swap
    NUM_MOODYS_SWAP_NOTIONAL_CODES
}MOODYS_SWAP_NOTIONAL_CODE;

/// @brief This structure stores hedge override information.
/// @see
///     * get_deal_hedge()
///     * set_deal_hedge_override()
typedef struct
{
    short counterparty_default_code;      ///< Counterparty default code, 0:no default code, 1: default from.
    int counterparty_default_from;        ///< The start date of counterparty defaults.
    int counterparty_default_to;          ///< The end date of counterparty defaults.
    bool use_paying_margin_override;      ///< Override paying margin or not.
    double override_paying_margin;        ///< Override paying/strike margin, a decimal value.
    int paying_margin_override_from;      ///< The override start date of paying margin.
    int paying_margin_override_to;        ///< The override end date of paying margin.
    bool use_receiving_margin_override;   ///< Override receiving margin or not.
    double override_receiving_margin;        ///< Override receiving margin, a decimal value.
    int receiving_margin_override_from;   ///< The override start date of receiving margin.
    int receiving_margin_override_to;     ///< The override end date of receiving margin.
} MOODYS_HEDGE_OVERRIDE;

/// @brief This structure stores hedge information.
/// @see
///     * get_deal_hedge()
///     * set_deal_hedge_override()
typedef struct
{
    char hedge_id[65]; 			     ///< Hedge name
    char hedge_desc[256];            ///< Hedge description.
    char counterparty_name[256]; 	 ///< Name of counterparty
    short swap_notional_code;        ///< Swap notional code, refer to enum ::MOODYS_SWAP_NOTIONAL_CODE .
    short swap_notional_index;       ///< Swap float index, refer to enum ::INDEX_TYPE and enum ::INDEX_TYPE_EX.
    double paying_margin;            ///< The paying/Strike margin or the fixed rate that is being applied to the swap, expressed as decimals.
    double receiving_margin;         ///< The receiving margin or the fixed rate that is being applied to the swap, expressed as decimals.
    MOODYS_HEDGE_OVERRIDE hedge_override_info;  ///< The hedge override information.
} MOODYS_HEDGE_STRUCT;

/// @brief Enum for fee type calculate code.
/// @see MOODYS_FEE_STRUCT
typedef enum 
{
    FEES_CALC_NONE,           ///< No fee calculation type will be applied
    FEES_TOTAL_POOL_CALC,     ///< Total pool fee
    FEES_TOTAL_BONDS_CALC,    ///< Total bond fee
    FEES_FIXED_AMOUNT_CALC,   ///< Fixed amount fee
    FEES_CUSTOM_CALC,         ///< Custom fee
    NUM_FEES_CALC,             
}MOODYS_FEE_CAL_CODE;

/// @brief This structure stores fee information.
/// @see
///     * get_deal_fee()
///     * set_deal_fee_override()
typedef struct
{
	int	  fee_id;			   ///< Fee id
    char  fee_name[256]; 	   ///< Fee name
    short fee_type;            ///< Fee type calculate, refer to enum ::MOODYS_FEE_CAL_CODE (SFW only)
    short day_count;           ///< Day count convention, must be one of: (SFW only)
							   ///<  - DAYCOUNT_DEFAULT
                               ///<  - DAYCOUNT_ACTUAL_360
                               ///<  - DAYCOUNT_ACTUAL_365
                               ///<  - DAYCOUNT_ACTUAL_ACTUAL
                               ///<  - DAYCOUNT_30_360
                               ///<  - DAYCOUNT_30_360E
    double fee_value;          ///< Value of fee (SFW only)
} MOODYS_FEE_STRUCT;

/// @brief Enum for metrics spread type
/// @see 
///     * get_bond_market_risk_metrics()
///     * get_bond_market_risk_metrics_ex()
typedef enum 
{
    APPLY_SPREAD_TO_TSY,          ///< Apply spread to Treasury 3 months Curves.
    APPLY_SPREAD_TO_LIBOR,        ///< Apply spread to Libor 1 month Curves.
    NUM_APPLY_SPREAD_TO,
}APPLY_SPREAD_TYPE;

/// @brief Enum for metrics calculation mode
/// @see 
///     * set_metrics_input_ex()
typedef enum 
{
    ENABLE_NONE,                   ///< Disable OAS calculation.
    OAS_ONLY,                      ///< Just enable calculation of OAS, but disable calculation of effective duration, effective convexity.
    ENABLE_ALL,                    ///< Enable calculation of OAS, effective duration, effective convexity.                       
    NUM_OAS_MODE
}OAS_CAL_MODE;

/// @brief Enum for the metrics input anchors type.
/// @see
///     * get_bond_market_risk_metrics_ex()
typedef enum
{ 
    OAS,           ///< Input type is OAS (in basis point).
    MARKET_PRICE   ///< Input type is market price.
} METRIC_ANCHORS;

/// @brief This structure stores OAS price array.
/// @see
///     * get_bond_market_risk_metrics_ex()
typedef struct
{
	int date[MAX_PERIODS];           ///< Monthly date array starting from user specified trade settlement date.
    double price[MAX_PERIODS];       ///< The corresponding price array calculated from OAS.    
} OAS_PRICE_STRUCT;

/// @brief This structure stores metircs results information.
/// @see
///     * get_bond_market_risk_metrics_ex()
typedef struct
{
    double OAS;                      ///< The constant spread which, when added to the LIBOR/Treasury curve associated with the security's currency, causes the present value of the expected option adjusted cash flows to equal the input price.
    double effective_duration;       ///< The average percentage change in a security's market value (incorporating the OAS) given a user specified shift (e.g., +-100bps) in the corresponding LIBOR/Treasury curve, while incorporating the optionality based on the yield curve shifts. The shift is performed on the spot curve.
	double effective_duration_par;   ///< The average percentage change in a security's market value (incorporating the OAS) given a user specified shift (e.g., +-100bps) in the corresponding government yield curve, while incorporating the optionality based on the yield curve shifts. The shift is performed on the par curve before forward curve construction.
    double effective_convexity;      ///< An option-adjusted measure of the curvature in the relationship between security prices and security yields that demonstrates how the effective duration of a security changes with interest rate shifts (e.g., +/-100 bps).
	double effective_convexity_par;  ///< An option-adjusted measure of the curvature in the relationship between security prices and security yields that demonstrates how the effective duration (par) of a security changes with interest rate shifts (e.g., +/-100 bps).
    double price;                    ///< In percentage of a security's current face, the average present value of all the future cash flow paths subtracted by accrued interest, incorporating the optionality based on the simulated economic scenarios. The discount factors are based on the LIBOR/Treasury, OAS and user-specified shift amount.
    double spread_duration;          ///< The average percentage change in a security's market value given user-specified shifts (e.g., +/- 100bps) in its OAS.
	OAS_PRICE_STRUCT price_array;    ///< Optional, only when anchor_type is OAS and METRIC_INPUT_STRUCT_EX.enable_price_array set to true returned. In percentage of a security's current face, the average present value of all the future cash flow paths subtracted by accrued interest, incorporating the optionality based on the simulated economic scenarios. The discount factors are based on the LIBOR/Treasury, OAS and user-specified shift amount and user input settlement date.
} METRIC_RESULTS_STRUCT_EX;


/// @brief This structure stores inputs information for metrics ex calculation.
/// @see
///     * get_bond_market_risk_metrics_ex()
typedef struct
{
    OAS_CAL_MODE oas_mode;                      ///< Flag to enable OAS-related metrics calculation or not.
    double shift_amt;                           ///< Rate shift amount, a decimal, whose default is 0.00001 (1bp).
    int num_paths;                              ///< Number of simulation paths for OAS.
    short shift_index_array[MAX_INDEX_TYPES];   ///< Array of index rates to shift. Please refer to INDEX_TYPE for available index types, reserve for future use.
    int enable_parallel_run;                    ///< Specify number of threads to run OAS simultaneously. Set 0 or 1 would disable parallel run OAS simultaneously.
    bool enable_CMM_custom_scenario_for_CMBS;   ///< Enable CMM custom scenario analysis for each interest rate path for OAS analysis. Credit model must also be set to CMM using set_moodys_credit_model_settings().
	bool disable_fixed_assumption;              ///< Disable the fixed credit assumption for bankloan's risk_metrics_ex calculation.
    bool enable_price_array;                    ///< Enable calculating price_array or not.
} METRIC_INPUT_STRUCT_EX;

/// @brief This structure stores inputs information for metrics calculation.
/// @see
///     * get_bond_market_risk_metrics()
typedef struct
{
    double clean_price; 			             ///< Given clean price which the metrics analysts base on.
    APPLY_SPREAD_TYPE apply_spread_to;           ///< Apply spread to Libor curves or Treasury curves, refer to enum ::APPLY_SPREAD_TYPE.
    bool calc_basic_metrics_only;                ///< If true, just calculate and return DV01, DV100 and CS01 only.
} METRIC_INPUT_STRUCT;

/// @brief This structure stores outputs results for metrics calculation.
/// @see
///     * get_bond_market_risk_metrics()
typedef struct
{
    double i_spread; 			        ///< Interpolated spread, the difference in basis point between its yield to maturity and the linearly interpolated yield for the same maturity on a user-specified reference curve (LIBOR/Treasury).
    double z_spread;                    ///< Zero-volatility spread in basis point that makes the security's price equal the present value of its cash flows along each point along the Treasury/Libor curve. 
    double macaulay_duration; 	        ///< The weighted average years to maturity of the cash flows from a security.
    double DV01;                        ///< The "dollar value of a 01" - i.e., the estimated change in the price of the instrument given a 1 bp change in the yield.
    double yield_to_worst;              ///< The lowest possible yield on a security.
    double effective_yield;             ///< The yield of a security which has its payments reinvested after they have been received.
    double spread_convexity;            ///< A measure (second derivative) of price sensitivity calculated by shifting z-spread +/- one BP but not the index and measuring average price change.
    double yield_value_of_32nd;         ///< The difference between the initial yield and the new yield given a price change of one tick (i.e., 32nd or 1/32).
    double annual_modified_duration;    ///< The standard macaulay duration formula, assuming an annual compounding frequency.
    double annual_duration_to_worst;    ///< Duration to Worst, based upon an annual compounding frequency.
    double annual_yield_to_maturity;    ///< Yield to Maturity, assuming an annual discounting of coupon payments.
    double annual_yield_to_worst;       ///< Yield to Worst, assuming an annual discounting of coupon payments.
	double CS01;                        ///< The "credit spread dollar value of a 01" - i.e., the estimated change in the price of the instrument given a 1 bp change in the discount margin.
	double DV100;                       ///< The "dollar value of a 01" - i.e., the estimated change in the price of the instrument given a 1 percent change in the yield.
} METRIC_RESULTS_STRUCT;

/// Types of rating agency.
/// @see get_bond_rating_by_tranche()
enum RATING_AGENCY 
{
    S_AND_P_CURRENT,
    S_AND_P_ORIGINAL,
    MOODYS_CURRENT,
    MOODYS_ORIGINAL,
    FITCH_CURRENT,
    FITCH_ORIGINAL,
    DCR_CURRENT,
    DCR_ORIGINAL,
    DBRS_CURRENT,
    DBRS_ORIGINAL,
    MAX_RATINGAGENCY
};

// Types of ESG rate output type.
/// @see ESG_MODEL_INPUTS
enum ESG_RATE_TYPE 
{
    SPOT_RATE,               ///< Only spot rate.
    SPOT_SPREAD_RATE         ///< Spot rate and credit model spread rate.
};
/// @brief This structure stores specified inputs to setup ESG specified currency simulation interest rates.
typedef struct
{
    char    Currency[4];							///< Standard currency code for the interest rate,refer to ISO 4217.
	double* OverrideLiborZCBP;                      ///< The array of libor override rates of zero coupon bond price.
    int     OverrideLiborZCBPSize;                  ///< The number of elements of OverrideLiborZCBP.
    double* OverrideTsyZCBP;                        ///< The array of treasury override rates of zero coupon bond price. 
    int     OverrideTsyZCBPSize;                    ///< The number of elements of OverrideTsyZCBP.
    bool    UpdateInitialCurveFlag;                 ///< Flag to enable or disable initializing ESG curve rates with spot rates.
} ESG_CURRENCY_RATE_INPUTS;

/// @brief This structure stores inputs to setup ESG model interest rates.
/// @see
///     * set_up_ESG_model_interest_rates()
typedef struct
{
    char    ESGbhmPath[1024];                       ///< Path of ESG installer bhm files and dlls.
    char    ESGLicenseFilePath[1024];               ///< ESG license file.
    char    ESGCalibrationFilePath[1024];           ///< ESG calibration file.
    char    ESGDataPath[1024];                      ///< Path of ESG installer data, optional
    char    ESGRatesOutputPath[1024];               ///< ESG rates output Path, optional, if set, just generates ESG output .CSV file under the output path;
                                                    ///< if not set, WSAAPI would load/set ESG rates automatically when set_metrics_input_ex with OAS enabled.
    ESG_RATE_TYPE  ESGRateType;                     ///< ESG rates output type.
    int     YYYYMMDD;                               ///< Simulation date, the format would be yyyymmdd.
    int     SimulationPaths;                        ///< Num of paths for simulation.
    int     Periods;                                ///< Periods for simulation.
} ESG_MODEL_INPUTS;

// Types of CMM macro economic rate output type.
/// @see set_macroeconomic_factor_ex
enum MACROECONOMIC_FACTOR_TYPE 
{
	REALGDPGROWTH,           ///< NIPA:Gross Domestic Product (growth).
    UNEMPRATE,	             ///< U3 - Unemployment Level.
	FEDFUNDSRATE,            ///< Federal funds rate.
	CPIINFRATE,              ///< Consumer Price Index (inflation).
	POPGROWTH,               ///< Population growth.
	NUMHOUSEHOLDSGROWTH	,    ///< Number of households (growth)
	RETAILSALESGROWTH,       ///< Retail sales (growth)
	TOTNONFARMEMPGROWTH,     ///< Nonfarm employment (growth)
	PERSONALINCGROWTH,       ///< Personal income (growth)
	HOMEPRICEGROWTH,         ///< HPI (growth)
	BAACORPYIELD,            ///< Moody's Intermediate-Term Bond Yield Average: Corporate - Rated Baa
	CREPXIDXGROWTH           ///< Moody's/RCA Commercial Property Price Index - All property
};

// Types of ECON rate settings.
/// @see set_rate_shift_setting
enum SCENARIO_RATE_SHIFT_TYPE
{
    SCENARIO_DEAL,
    SCENARIO_YES,
    SCENARIO_NO
};

/// @brief This structure stores inputs to setup ECON rate settings.
/// @see
///     * set_rate_shift_setting()
typedef struct
{
   SCENARIO_RATE_SHIFT_TYPE alwaysUseScenRateShift;     ///< Need to be one of SCENARIO_RATE_SHIFT_TYPE. By default, it is 'SCENARIO_DEAL'. CDOnet only.
   BOOLYAN rateShiftFromSettle;                         ///< Whether apply rate shift from settlement date or not. By default, it is false.
   BOOLYAN shiftRelativeToCurrentRates;                 ///< Field reserved.
} RATE_SHIFT_SETTING;

typedef enum
{ 
    FIRST_LOSS_THRESHOLD_INTEREST,	  ///< Interest
    FIRST_LOSS_THRESHOLD_PRINCIPAL,   ///< Principal
    FIRST_LOSS_THRESHOLD_EITHER       ///< either interest or principal
} FIRST_LOSS_THRESHOLD;

typedef enum
{ 
	// 0
    FIRST_LOSS_RUN_MODE_PRINCIPAL_PAYMENT_RATE,    ///< Principal payment rate
    FIRST_LOSS_RUN_MODE_MONTHLY_PURCHASE_RATE,     ///< Monthly purchase rate
    FIRST_LOSS_RUN_MODE_PORTFOLIO_YIELD,		   ///< Portfolio yield
    FIRST_LOSS_RUN_MODE_LOSS_RATE,				   ///< Loss rate
    FIRST_LOSS_RUN_MODE_DEFAULT,				   ///< Default rate
	FIRST_LOSS_RUN_MODE_RECOVERY_RATE,			   ///< Recovery rate
    // 6
	FIRST_LOSS_RUN_MODE_LOSS_SEVERITY,             ///< Loss severity rate 
	FIRST_LOSS_RUN_MODE_PREPAY,					   ///< Prepayment rate 
	FIRST_LOSS_RUN_MODE_FORBEARANCE,			   ///< Forbearance rate 
    FIRST_LOSS_RUN_MODE_DEFERMENT,				   ///< Deferment rate 
} FIRST_LOSS_RUN_MODE;

typedef struct
{
   FIRST_LOSS_RUN_MODE first_loss_run_mode;     ///< Run mode. Must be one of enum FIRST_LOSS_RUN_MODE.  SFW only. 
   BOOLYAN is_percentage;						///< If TRUE, the output FIRST_LOSS_RESULT.deal_collateral_losses is in percentage. If False, it is in dollar. SFW only.
   short prepayment_type;						///< Type of prepayment curve.  SFW only.  Must be one of:
												///<     - PREPAY_CURVE_PSA - The PSA Standard Prepayment Assumptions rate specifies an annual prepayment percentage as a function of the 
												///<     - PREPAY_CURVE_SMM - The Single Monthly Mortality rate is the percentage of the mortgages/loans outstanding at the beginning of the month 
												///<     - PREPAY_CURVE_CPR - The Constant Prepayment Rate is the percentage of the mortgages/loans outstanding at the beginning of a year assumed to terminate during the year.  
												///<     - PREPAY_CURVE_CPY - The Constant Prepayment Yield rate is equivalent to the Constant Prepayment Rate(CPR) except that it assumes prepayment only happens after contractual lockout and yield maintenance period.
												///<     - PREPAY_CURVE_HEP - The Home Equity Prepayment rate is a measure of prepayments for closed-end, fixed rate HEL loans.  
												///<     - PREPAY_CURVE_ABS - Asset-Backed Securities(ABS): 
    double  prepayment_rate;					///<  Prepayment rate.  SFW only. 
    short default_type;							///<  Type of default curve. Must be one of: 
												///<     - DEFAULT_CURVE_CDR - Constant Default Rate(CDR): The Constant Default Rate is the percentage of the mortgages/loans 
												///<     - DEFAULT_CURVE_SDA - Standard default curve: Measuring for defaults in the residential mortgage market
												///<     - DEFAULT_CURVE_SEASONED_CDR - The Constant Default Rate is the percentage of the mortgages/loans outstanding at the beginning of the year  
												///<     - DEFAULT_CURVE_SEASONED_MDR - The Monthly Default Rate is the percentage of the mortgages/loans outstanding at the beginning of the month 
												///<     - DEFAULT_CURVE_PCT - The PCT is similar to the MDR curve except that defaults are applied each month to the period 0 balance of the loan, rather than the 
    double  default_rate;						///<  Default rate.
    double forbearance_rate;					///<  Forbearance rate. Only available for Student Loan.  SFW only. 
    double deferment_rate;						///<  Deferment rate. Only available for Student Loan.  SFW only. 
    FIRST_LOSS_THRESHOLD first_loss_threshold;  ///< The first loss threshold. Must be one of FIRST_LOSS_THRESHOLD enum.
    double principal_payment_rate;				///< Principal payment rate. Only available for Credit Cards.  SFW only. 
    double monthly_purchase_rate;				///< Monthly purchase rate. Only available for Credit Cards.  SFW only. 
    double portfolio_yield;						///< Portfolio yield. Only available for Credit Cards.  SFW only. 
    double loss_rate;							///< Loss rate. Only available for Credit Cards.  SFW only. 
	double prin_loss_serverity;					///< prin_loss_serverity. Principal loss percentage. SFW only.
	short run_mode_cc;							///< SFW only. Must be one of:  CREDIT_RATE_MODES {
												///< CC_PRIN_PAY_MODE,
												///< CC_PURCHASE_MODE,
												///< CC_YIELD_MODE,
												///< CC_LOSS_MODE,
												///< CC_DEFAULT_MODE,
												///< CC_RECOVERY_MODE,
	BOOLYAN  is_true;							///< If TRUE, the calculation will take into account tranches that defer then pik. CDOnet only.
} FIRST_LOSS_INPUT;

typedef struct
{
    double run_mode_rate;						///< The rate of the input run mode.  SFW only. 
    double deal_collateral_losses;				///< If true it is mega. If false it is not.  SFW only. 
    int first_loss_date;						///< First loss date, format "YYYYMMDD".   SFW only. 
    double default_rate;						///< Default rate.   CDOnet only. 
} FIRST_LOSS_RESULT;

typedef struct
{
	char tranche_name[20];		///< The tranche name.
	char tranche_code[11];	    ///< The tranche code.
} CHINA_BOND_INFO;

typedef enum
{
    SEASONING_GLOBAL,   ///< Take into account the update month of the deal, then start all loans at that point of the vector. For example, if the deal is in its 12th payment month, everything will run at month 12 of the vector from today forward.
    SEASONING_NO,       ///< Do not apply any seasoning to the deal, so all loans will start at the first month of the vectors.
    SEASONING_YES,      ///< Apply loan level seasoning, so every loan backing the deal will start at the point of the vector corresponding to that loan's seasoning.
} SEASONING_TYPE;

typedef struct
{
    char prepay_type[4];            ///< Type of prepayment curve. Must be one of:
                                    ///< "PSA": Standard prepayment curve measuring for prepayments in the residential mortgage market.
                                    ///< "SMM": Monthly prepayment or default rate.
                                    ///< "CPR": Constant Prepayment Rate(CPR): Prepayment percentage expressed as an annual compounded rate.
                                    ///< "HEP": Home Equity Prepayment: A measure of prepayments for closed-end, fixed rate HEL loans. This curve accounts for the faster seasoning ramp for home equity loans.
                                    ///< "ABS": Asset-Backed Securities(ABS): It is used in ABS markets, where prepayments differ significantly from standard mortgages. This model defines an increasing sequence of monthly prepayment rates, which correspond to a constant absolute level of loan prepayments in all future periods.
                                    ///< "MHP",
                                    ///< "ADV": Prepayment method that applies immediate prepayment when its issue coupon is set to a specified percentage.
                                    ///< "MON": Prepays the entire performing balance of the loan in the month indicated as the rate (Example: 2 MON has a 100% SMM rate of prepayment or default in month 12 and a 0% rate of prepayment or default during all other months).
                                    ///< "PCT": Monthly prepayment rate as a percent of the original balance of the loan.
                                    ///< "SDA": Standard default curve measure for defaults in the residential mortgage market.
                                    ///< "NIS": Prepayment method for auto leases.
                                    ///< "MOD",
                                    ///< "RMP"
    double ppc_rates[MAX_PERIODS];  ///< Vector of prospectus prepayment curve.
    SEASONING_TYPE seasoning;       ///< Type of seasoning.
} PPC_STRUCT;

/// Prepayment compounding methods.
/// @see
///     * set_prepay_default_compounding_method()
typedef enum
{
    PREPAY_DEFAULT_COMPOUNDING_MONTHLY,     ///< Prepayment compounding bases on monthly.
    PREPAY_DEFAULT_COMPOUNDING_PERIODICITY  ///< Prepayment compounding bases on asset's periodicity.
} PREPAY_DEFAULT_COMPOUNDING_METHOD;

#if __cplusplus
extern "C"
{
#endif

    /// Allows the user to specify the processing engine to use in the event when a CUSIP is supported 
    /// by both: CHS and SFW engine. It is not thread-specific and should be called in the main 
    /// thread of the client application. The system default setting is to use SFW engine in the event of an overlap.
    ///
    /// @since 0.9.0
    /// @avail ALL
    ///
    /// @param[in] engine One of the enums from ::ENGINE_PREFERENCE
    ///
    /// @return None
    ///
    /// @note Call this function to specify which library to use in the event that a deal (or CUSIP) is supported by 
    /// both the CHS and SFW libraries.  It is not thread-specific and should be called in the main thread of the client
    /// application.  The system default setting is to use SFW engine in the event that a deal exists in both the CHS 
    /// and SFW libraries.
    ///
	/// @sample
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    /// @endsample
    ///
    void CHASAPI set_engine_preference(const ENGINE_PREFERENCE& engine);


    /// This is a convenience method provided in the wrapper. It can be called after the deal is opened to retrieve 
    /// the engine that is being used for processing the currently open deal. It will return ::UNKNOWN_ENGINE if no deal 
    /// is currently open.
    /// 
    /// @since 0.9.0
    /// @avail ALL
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using nonthread safe calls.
    ///
	/// @retval UNKNOWN_ENGINE Unknown engine (no deal open yet).
	/// @retval CHS_ENGINE Current deal opened by CHS engine.
	/// @retval SFW_ENGINE Current deal opened by SFW engine.
	/// @retval CDONET_ENGINE Current deal opened by CDOnet engine.
    ///
    /// @note Call this function after opening a deal to retrieve the library it's from.  Used for real-time processing
    ///     on the current open deal. It will return ::UNKNOWN_ENGINE if no deal is currently open.  See enums of ::ENGINE_TYPE
    ///     for the valid values this function returns.
    ///
    /// @sample
    ///     void* tid=NULL;
    ///     CMO_STRUCT cmo={};
    ///     strcpy(cmo.dealid, "AAM0401");
    ///
    ///     set_engine_preference(PICK_CHS_ENGINE_FORMAPPED_DEALS);
    ///     open_deal_ex(tid, &cmo);
    ///
    ///     // The expected engine type for Deal AAM0401 should be CHS_ENGINE.
    ///     int engine_type = get_current_deal_engine(tid);
    ///
    ///     close_deal_ex(tid, &cmo);
    /// @endsample
    ///
    ENGINE_TYPE CHASAPI get_current_deal_engine(void *tid);

    /// Retrieves the name and account info of the available accounts in current deal.
    ///
    /// This method gets all the deal accounts info, if the deal accounts are available.
    ///
    /// @since 1.1.0
    /// @avail CDOnet, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[in] account_size Fields size of account_names and account_info.
    /// @param[out] account_names A pointer to a client-allocated array of characters strings to store names of the account. At least 11 characters should be allocated for each string.
    /// @param[out] account_info Fields populated with account information.
    ///
    /// @return Number of accounts in deal.
    ///
    /// @retval >=0 Number of accounts in deal
    /// @retval  -1 Deal not open
    /// @retval  -2 Invalid account_names or account_info
    ///
    /// @sample
    ///     void * pDeal = NULL;
    ///     // Deal is already open.
    ///
    ///     int account_num = get_deal_account_avail(pDeal, NULL, NULL, NULL, 0);
    ///     std::vector<char> name_buf(account_num*11);
    ///     std::vector<char*> names(account_num);
    ///     for(int i = 0; i < account_num; ++i)
    ///     {
    ///         names[i] = &name_buf[i*11];
    ///     }
    ///     std::vector<DEAL_ACCOUNT_INFO> info(account_num);
    ///     assert(account_num == get_deal_account_avail(pDeal, NULL, &names.front(), NULL, account_num));
    ///     assert(account_num == get_deal_account_avail(pDeal, NULL, NULL, &info.front(), account_num));
    ///     assert(account_num == get_deal_account_avail(pDeal, NULL, &names.front(), &info.front(), account_num));
    /// @endsample
    int CHASAPI get_deal_account_avail(void* tid, const char* reremic_deal_id_or_null, char* account_names[], DEAL_ACCOUNT_INFO account_info[], unsigned int account_size);
    
    /// Turns the account on and off by setting the account default status.
    ///
    /// @since 1.1.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[in] account_name The name of account to be set.
    /// @param[in] account_default Set value of account default.
    ///
    /// @retval   0 SUCCESS
    /// @retval  -1 Deal not open
    /// @retval  -2 account_name not found
    /// @retval -99 Error, for details call get_deal_error_msg()
    ///
    /// @note This function is only available for Account Type = Liq Fac and Insurance.
    ///
	/// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SAS059XS"); 
    ///
    ///     open_deal_ex(tid,pCmo);
    ///
    ///     char modRemicDealName[] = "ABF00001";
    ///     set_deal_account_default(tid, modRemicDealName, "INSURANC-1", true);
    ///
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI set_deal_account_default(void* tid, const char* reremic_deal_id_or_null, const char* account_name, BOOLYAN account_default);

    /// Sets whether reimburse service advanced P&I for SFW and CDOnet deals.
    ///
    /// @since 1.6.0
    /// @avail SFW, CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[in] reimburse_advint TRUE/FALSE value to be set for reimbursing advanced P&I.
    ///
    /// @retval   0 SUCCESS
    /// @retval  -1 Error - Deal not opened
    /// @retval -99 Error - Invalid dso identifier (tid) and other errors
    ///
    /// @sample
    /// 	void* ptid = NULL;
    /// 	//deal has been opened
    ///
    /// 	int ret = set_service_reimburse_advint(ptid, null, TRUE);
    /// 	if(ret != 0)
    /// 	{
    /// 	    //error handling
    /// 	}
    /// @endsample
    ///
    /// @note if not set, the default reimburse advanced P&I flag is TRUE.
    ///
    /// @warning when the deal service advance is SERVICER_ADVANCES_NOTHING, set_service_reimburse_advint cannot set reimburse_advint=TRUE.
    int CHASAPI set_service_reimburse_advint(void* tid, const char* reremic_deal_id_or_null, BOOLYAN reimburse_advint); 

    /// Sets Prepays only on Pay Dates.
    /// Sets flag to indicate whether a loan can only prepay or default on a payment date.
    ///
    /// @since 1.6.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[in] only_on_paydate If TRUE, then collateral only prepays or defaults on the asset payment dates. If FALSE, then collateral can prepay and default on non-payment dates.
    ///
    /// @retval   0 SUCCESS
    /// @retval  -1 Error - Deal not opened
    /// @retval -99 Error - Invalid dso identifier (tid) and other errors
    ///
    /// @sample
    /// 	void* ptid = NULL;
    /// 	//deal has been opened
    ///
    /// 	int ret = set_ppydef_only_on_paydate(ptid, null, TRUE);
    /// 	if(ret != 0)
    /// 	{
    /// 	    //error handling
    /// 	}
    /// @endsample
    ///
    /// @note If this method is not called, then it will be set to TRUE by default (i.e. loans can only prepay and default on payment dates).
    int CHASAPI set_ppydef_only_on_paydate(void* tid, const char* reremic_deal_id_or_null, BOOLYAN only_on_paydate);

    /// This method can be used to specify whether to use the recovery rate at time of default or the recovery rate at time of recovery.
    /// It is only relevant when running a non-constant recovery rate vector and a non-zero recovery lag. 
    ///
    /// @since 1.5.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] type Recovery rate applying method:
    ///             - RECOVERY_RATE_AT_RECOVERY: Recovery rate at the time of recovery in case of recovery lag
    ///             - RECOVERY_RATE_AT_DEFAULT: Recovery rate at the time of default in case of recovery lag.
    /// @param[in] set_sup_remic Settings are applied to underlying deals if TRUE. Otherwise, it will not.
    ///
    /// @retval    0 No error
    /// @retval   -1 Deal not open
    /// @retval  -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @note 
    /// The default behavior for SFW deals is to use the recovery rate at the time of recovery, to be consistent with the CHS engine.
    /// One exception is when calling set_moodys_credit_model_settings(), which always uses recovery rate at the time of default (this method will not override that setting).
    ///
    /// @sample
    /// 	void* tid = NULL;
    /// 	CMO_STRUCT cmos;
    /// 	memset(&cmos, 0, sizeof(cmos));
    /// 	set_input_path("C:\\Deals");
    /// 	strcpy(cmos.dealid, "AL2010-A");
    /// 	
    /// 	// open_deal_ex() must be called before set_recovery_from().
    /// 	assert(-1 == set_recovery_from(tid, RECOVERY_RATE_AT_RECOVERY, true));
    /// 	assert(0 == open_deal_ex(tid, &cmos));
    ///	
    /// 	// recovery rate at recovery
    /// 	assert(0 == set_recovery_from(tid, RECOVERY_RATE_AT_RECOVERY, true));
    /// 	// run deal
    /// 	assert(0 == run_deal_ex(tid, &cmos));
    ///	
    /// 	// recovery rate at default
    /// 	assert(0 == set_recovery_from(tid, RECOVERY_RATE_AT_DEFAULT, true));
    /// 	// run deal
    /// 	assert(0 == run_deal_ex(tid, &cmos));
    ///
    /// 	close_deal_ex(tid, &cmos);
    /// @endsample
    ///
    long CHASAPI set_recovery_from(void* tid, short type, BOOLYAN set_sup_remic);


    /// This function enable user to specify if their input status vectors overwrite the asset level status. 
    ///
    /// If set a TRUE flag, API will use the status vector set by user and ignoring the asset level non-payment status.
    /// If set a FALSE flag, API will use both status vector and asset level non-payment status.
    ///
    /// @since 1.6.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] val 
    ///     - If set with TRUE, API will apply the status vector set by user and ignore the status from asset level.
    ///     - If set with FALSE, API will both use vectors set by user and non-payment status on asset level.
    ///
    /// @retval   0 Success
    /// @retval  -1	Error - Deal not opened
    /// @retval -99 Error - For details call get_deal_error_msg()
    ///
    /// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT;
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SMS2000-A");
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(tid, pCmo));
    ///     
    ///     // case 1: no ignore_asset_nonpayment_term(), not set status vectors
    ///     assert(0 == run_deal_ex(tid, pCmo));
    ///     
    ///     // case 2: ignore_asset_nonpayment_term(false), not set status vectors, equals to case 1
    ///     assert(0 == ignore_asset_nonpayment_term(tid, false));
    ///     assert(0 == run_deal_ex(tid, pCmo));
    ///     
    ///     // case 3: ignore_asset_nonpayment_term(false), set status vectors
    ///     
    ///     // set forbearance vector
    ///     #define MAX_MONTHS 500
    ///     double adRates[MAX_MONTHS];
    ///     
    ///     std::fill_n(adRates, MAX_MONTHS, 0.0);
    ///     std::fill_n(adRates+1, 30, 15.0/100);	// adRates[0] is not been used
    ///     
    ///     short is_vector = MAX_MONTHS;
    ///     double *pval = adRates;
    ///     long loan_num = -1;
    ///     BOOLYAN set_sup_remic = false;
    ///     assert(0 == set_forbearance_rates(tid, is_vector, pval, loan_num, set_sup_remic));
    ///     assert(0 == ignore_asset_nonpayment_term(tid, false));
    ///     assert(0 == run_deal_ex(tid, pCmo));
    ///     
    ///     // case 4: ignore_asset_nonpayment_term(true), set status vectors
    ///     std::fill_n(adRates, MAX_MONTHS, 0.0);
    ///     std::fill_n(adRates+1, 30, 15.0/100);	// adRates[0] is not been used
    ///     assert(0 == set_forbearance_rates(tid, is_vector, pval, loan_num, set_sup_remic));
    ///     assert(0 == ignore_asset_nonpayment_term(tid, true));
    ///     assert(0 == run_deal_ex(tid, pCmo));
    ///     
    ///     assert(0 == close_deal_ex(tid, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    /// 
    /// @sa
    /// - set_forbearance_rates()
    /// - set_deferment_rates()
    /// - set_grace_rates()
    /// 
    long CHASAPI ignore_asset_nonpayment_term(void *tid, bool val);

    /// Sets set the SLABS forbearance rate vector.
    ///
    /// Sets the constant or vectored forbearance rate for SLABS deals that will be used for the pool specified by loan_num or for all pools if loan_num = -1,
    /// with the ability to apply to underlying deals if a reremic.
    ///
    /// @since 1.4.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// 
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] is_vector The length of the vector pointed to by pval or 0 if pval is a constant.
    /// @param[in] pval A pointer to the prepayment speeds (or speed). Value for current period (0-indexed element) will not be applied.
    /// @param[in] loan_num The 1-based index of the loan or -1 to apply to all collateral in the deal.
    /// @param[in] set_sup_remic If TRUE this will replace any specific underlying deal settings. Otherwise it will not.
    ///
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Other error
    /// @retval  -3 Error - Invalid loan number
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @note
    ///     * Forbearance rates are expressed as decimals. e.g. 5.25% would be .0525.
    ///     * Value for current period (0-indexed element) will not be applied.
    ///
	/// @sample 
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SAS059XS");  
    ///
    ///     open_deal_ex(tid, pCmo);
    ///
    ///     short is_vector = 0;
    ///     double dValue = 0.001;
    ///     double * pval = &dValue;
    ///     long loan_num = 1;
    ///     BOOLYAN set_sup_remic = false;
    ///     set_forbearance_rates(tid, is_vector, pval, loan_num, set_sup_remic);
    ///
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    long CHASAPI set_forbearance_rates(void *tid, short is_vector, double * pval, long loan_num, BOOLYAN set_sup_remic);
    
    /// Sets deferment rates for SFW SLABS deals
    ///
    /// Sets the constant or vectored deferment rate for SLABS deals that will be used for the pool specified by loan_num or for all pools if loan_num = -1,
    /// with the ability to apply to underlying deals if a reremic.
    ///
    /// @since 1.4.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// 
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] is_vector The length of the vector pointed to by pval or 0 if pval is a constant.
    /// @param[in] pval A pointer to the prepayment speeds (or speed). Value for current period (0-indexed element) will not be applied.
    /// @param[in] loan_num The 1-based index of the loan or -1 to apply to all collateral in the deal.
    /// @param[in] set_sup_remic If TRUE this will replace any specific underlying deal settings. Otherwise it will not.
    ///
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Other error
    /// @retval  -3 Error - Invalid loan number
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @note
    ///     * Deferment rates are expressed as decimals. e.g. 6.23% would be .0623.
    ///     * Value for current period (0-indexed element) will not be applied.
    ///
	/// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SAS059XS");
    ///
    ///     open_deal_ex(tid,pCmo);
    ///
    ///     short is_vector = 0;
    ///     double dValue = 0.001;
    ///     double * pval = &dValue;
    ///     long loan_num = 1;
    ///     BOOLYAN set_sup_remic = false;
    ///     set_deferment_rates(tid, is_vector, pval, loan_num, set_sup_remic);
    ///
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
	///
    long CHASAPI set_deferment_rates(void *tid, short is_vector, double * pval, long loan_num, BOOLYAN set_sup_remic);

    /// Sets grace rates for SFW SLABS deals
    /// 
    /// This function allows users to set the percentage of student loans that are in grace period for SFW SLABS deals.
    /// 
    /// @since 1.6.0
    /// @avail SFW
    /// 
    /// @pre open_deal_ex() has been called.
    /// 
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] is_vector The length of the vector pointed to by pval or 0 if pval is a constant.
    /// @param[in] pval A pointer to the grace rates (or rate). Value for current period (0-indexed element) will not be applied.
    /// @param[in] loan_num The 1-based index of the loan or -1 to apply to all collateral in the deal.
    /// @param[in] set_sup_remic If TRUE this will replace any specific underlying deal settings. Otherwise it will not.
    ///   
    /// @retval   0 Success
    /// @retval  -1 Error - Deal not opened
    /// @retval -99 Error - For details call get_deal_error_msg()
    ///
	/// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SAS059XS");   
    ///
    ///     set_engine_preference(PICK_CHS_ENGINE_FOR_MAPPED_DEALS);
    ///     open_deal_ex(tid,pCmo);
    ///
    ///     short is_vector = 0;
    ///     double dValue = 0.001;
    ///     double * pval = &dValue;
    ///     long loan_num = 1;
    ///     BOOLYAN set_sup_remic = false;
    ///     set_grace_rates(tid, is_vector, pval, loan_num, set_sup_remic);
    ///
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @note
    ///     * Grace rates are expressed as decimals. e.g. 3.51% would be .0351.
    ///     * Value for current period (0-indexed element) will not be applied.
    long CHASAPI set_grace_rates(void *tid, short is_vector, double *pval, long loan_num, BOOLYAN set_sup_remic);

    /// Retrieves the list of borrower benefits for SLABS deals
    /// 
    /// This function allows users to retrieve the full list of borrower benefits available to SLABS deals.
    /// 
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail SFW
    /// 
    /// @pre open_deal_ex() has been called.
    /// 
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null Reremic deal id for a child deal, otherwise null.
    /// @param[out] benefit_list A pointer to an array which stores the information of the available borrower benefits of a deal.
    /// @param[in] size size of the benefit_list that user has passed.
    ///
    /// @retval >=0 The number of borrower benefits in deal.
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Invalid size
    /// @retval  -5 Error - Current deal is not SLABS
    /// @retval -99 Error - For details call get_deal_error_msg()
    ///
	/// @sample
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0 ,sizeof(*pCmo) );
    ///     strcpy(pCmo->dealid, "AB05HE3");
    ///     
    ///     open_deal_ex(tid,pCmo); 
    ///
    ///     get_available_borrower_benefits(tid, NULL, NULL, 0);
    ///
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    long CHASAPI get_available_borrower_benefits(void *tid, const char *reremic_deal_id_or_null, BORROWER_BENEFIT_ELIGIBILITY benefit_list[], int size);

    /// Sets borrower benefit discount rate for SLABS deals
    /// 
    /// This function allows users to set the discount rate applied to the borrower benefit for SLABS deals.
    /// 
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail SFW
    /// 
    /// @pre open_deal_ex() has been called.
    /// 
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null Reremic deal id for a child deal, otherwise null.
    /// @param[in] index The index of the borrower benefit. -1 if the given value/vector will be applied to all borrower benefits.
    /// @param[in] vector The length of the vector pointed to by pval or 0 if pval is a constant.
    /// @param[in] pval A pointer to the rates (or rate) to be applied to the specified borrower benefit.
    ///
    /// @retval   0 Success
    /// @retval  -1 Error - Deal not opened
    /// @retval  -3 Error - Invalid index
    /// @retval  -4 Error - No value passed or the value of vector is negative
    /// @retval  -5 Error - Current deal is not SLABS
    /// @retval -99 Error - For details call get_deal_error_msg()
    ///
    /// @note The rates are expressed as decimals. e.g. 65.8% would be 0.658.
    ///
    /// @sample
    ///     void* ptid = NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"WSLT2006-1");
    ///
    ///     //open deal
    ///     open_deal_ex(ptid,&deal);
    ///
    ///     short bbIdx(1);
    ///     short len(0);
    ///     double dRate(0.5);
    ///     int iret = set_borrower_benefits_rate(pDeal, NULL, bbIdx, len, &dRate);
    ///     if(iret < 0)
    ///         //Error handle
    ///
    ///     close_deal_ex(ptid,&deal);
    /// @endsample
    long CHASAPI set_borrower_benefits_rate(void *tid, const char *reremic_deal_id_or_null, short index, short vector, double *pval);

    /// Sets insurance percentage rate
    /// 
    /// @exptl Subject to change
    /// @since 2.4.0
    /// @avail SFW
    /// 
    /// @pre open_deal_ex() has been called.
    /// 
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] issuer The insurer name.
	/// @param[in] type The insurance claim type.
    /// @param[in] is_vector The length of the vector pointed to by pval or 0 if pval is a constant.
    /// @param[in] pval A pointer to the coverage rate.
    ///
    /// @retval   0 Success
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid pval pointer or pval length.
    /// @retval  -3 Error - Invalid insurer.
    /// @retval  -4 Error - The value of vector is negative.
    /// @retval -99 Error - For details call get_deal_error_msg().
    ///
    /// @note The rates are expressed as decimals. e.g. 65.8% would be 0.658.
    ///
    /// @sample
    ///     void* ptid = NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"SAS059XS");
    ///
    ///     //open deal
    ///     open_deal_ex(ptid,&deal);
    ///
    ///     double rate = 0.34; 
    ///     long ret = set_insurance_coverage(ptid, "Ambac Assurance Corporation", INSURANCE_CLAIM_COVERAGE, 0, &rate);
    ///
    ///     close_deal_ex(ptid,&deal);
    /// @endsample
    long CHASAPI set_insurance_coverage(void *tid, const char* issuer, INSURANCE_CLAIM type, short is_vector, double *pval);

    /// Enable bond insurance.
    ///
    /// @since 3.0.0
    /// @avail SFW
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid A pointer to the name of the bond.
    /// @param[in] is_enabled The flag indicate whether this bond is insured.
    ///
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Invalid bondid.
    /// @retval -99 Error - For details call get_deal_error_msg()
    ///
    /// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT;
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SAS059XS");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     int ret = open_deal_ex(tid, pCmo);
    ///     if(ret < 0)
    ///     {
    ///         //Error handling
    ///     }
    ///
    ///     const int len = 1024;
    ///     char input_csv[len]={0};
    ///     char output_csv[len]={0};
    ///     ret = get_cmm_input_files(tid,NULL,input_csv,len,output_csv,len);
    ///     if(ret < 0)
    ///     {
    ///       //Error handling
    ///     }
    ///
    ///     ret = enable_bond_insurance(tid, "1-A3A", true);
    ///     
    ///     ret = close_deal_ex(tid, pCmo);
    ///     if(ret < 0)
    ///     {
    ///         //Error handling
    ///     }
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    long CHASAPI enable_bond_insurance(void * tid, const char * bondid, BOOLYAN is_enabled);

    /// Sets CMM custom scenario data.
    ///
    /// @since 3.0.0
    /// @avail SFW
    /// @pre open_deal_ex() has been called.
	/// @pre The current credit model has been set to CMM with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] cmm_factor_type The type of CMM factor type. Must be one of CMM_FACTOR_TYPE.
    /// @param[in] factor The CMM factor. Must be one of CMM_FACTOR.
    /// @param[in] value A pointer to a double array, the array length must be 40.
    /// @param[in] length The length of value, the length must be 40.
    ///
    /// @retval   0 Success
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Invalid parameters.
    /// @retval -99 Error - For details call get_deal_error_msg()
    ///
    /// @sample
	///     #include "MarkitCMMProvider/MarkitCMMProvider.h"
    ///     #pragma comment(lib, "WSACMMProvider.lib")
	///
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT;
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SAS059XS");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     int ret = open_deal_ex(pDeal, pCmo);
    ///     if(ret < 0)
    ///     {
    ///         //Error handling
    ///     }
    ///
    ///     set_moodys_credit_model_settings(pDeal, MOODYS_CMM_SETTINGS, false);
	///     double value[] = {0.056999998,0.056258359,0.054541469,0.056365418,0.064670191,0.0782126,0.086426401,0.090757341,0.091721907,0.09174448,0.091839638,0.089244394,0.08650013,0.084163132,0.079800591,0.073180208,0.067184458,0.061098261,0.056628218,0.05455205,0.054580388,0.0547897323333333,0.0549990766666667,0.055208421,0.055474019,0.055556188,0.055709429,0.055890002,0.05600625,0.055872004,0.055737758,0.055525441,0.055487661,0.055262222,0.055346231,0.055127888,0.055127888,0.054981022,0.054845071,0.054779229};
    ///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_REALGDPGROWTH, value, 40);
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_UNEMPRATE, value, 40);
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_FEDFUNDSRATE, value, 40);
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_TSY10Y, value, 40);
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_CPIINFRATE, value, 40);
    ///
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_POPGROWTH, value, 40);
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_NUMHOUSEHOLDSGROWTH, value, 40);
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_RETAILSALESGROWTH, value, 40);
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_TOTNONFARMEMPGROWTH, value, 40);
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_NOMPERSONALINCGROWTH, value, 40);
    ///
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_HOMEPRICEGROWTH, value, 40);
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_BAACORPYIELD, value, 40);
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_CREPXIDXGROWTH, value, 40);
    ///
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_IR, CMM_IR_LIBOR1M, value, 40);
    ///
    ///     char* custom_scen_name = "scenario1";
    ///     
    ///     SetupCMMCustomModel(pDeal, "user", "123456");
    ///     set_current_moodys_cmm_scenario(pDeal, NULL, custom_scen_name);
    ///     generate_cmm_custom_result_output(pDeal, custom_scen_name);
    ///
    ///     run_deal_ex(pDeal,pCmo);
    ///     close_deal_ex(pDeal,pCmo);
	///     delete pCmo;
	///     pCmo = NULL;
    /// @endsample
    /// @note The CMM Macro factors(CMM_ME_REALGDPGROWTH to CMM_ME_CREPXIDXGROWTH) all need to input.
    ///       The CMM IR factors(CMM_IR_LIBOR1M to CMM_IR_MMOVINGAVGCMT) at least input one factor.
    int CHASAPI set_cmm_custom_scenario(void * tid,  CMM_FACTOR_TYPE cmm_factor_type, CMM_FACTOR factor,  const double * value, int length );

	/// Generates the CMM custom scenario output file, it would call the cmm custom callback function to generate the CMM custom scenario output file.
    ///
    /// @since 3.0.0
    /// @avail SFW
    /// @pre open_deal_ex() has been called.
	/// @pre The current credit model has been set to CMM with API set_moodys_credit_model_settings().
	/// @pre set_cmm_custom_scenario() has been called.
	/// @pre SetupCMMCustomModel() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] custom_scen_name A pointer to name of the CMM custom scenario. It is defined by user.
    ///
    /// @retval   0 Success
    /// @retval  -1 Error - Deal not opened
	/// @retval  -2 Error - ME or IR data not existed(set in set_cmm_custom_scenario())
	/// @retval  -3 Error - CMM Loan data not existed.
    /// @retval -99 Error - For details call get_deal_error_msg()
    ///
    /// @sample
    ///     #include "MarkitCMMProvider/MarkitCMMProvider.h"
    ///     #pragma comment(lib, "WSACMMProvider.lib")
    ///
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT;
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SAS059XS");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     int ret = open_deal_ex(pDeal, pCmo);
    ///     if(ret < 0)
    ///     {
    ///         //Error handling
    ///     }
	///
	///     set_moodys_credit_model_settings(pDeal, MOODYS_CMM_SETTINGS, false);
	///     double value[] = {0.056999998,0.056258359,0.054541469,0.056365418,0.064670191,0.0782126,0.086426401,0.090757341,0.091721907,0.09174448,0.091839638,0.089244394,0.08650013,0.084163132,0.079800591,0.073180208,0.067184458,0.061098261,0.056628218,0.05455205,0.054580388,0.0547897323333333,0.0549990766666667,0.055208421,0.055474019,0.055556188,0.055709429,0.055890002,0.05600625,0.055872004,0.055737758,0.055525441,0.055487661,0.055262222,0.055346231,0.055127888,0.055127888,0.054981022,0.054845071,0.054779229};
    ///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_REALGDPGROWTH, value, 40);
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_UNEMPRATE, value, 40);
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_FEDFUNDSRATE, value, 40);
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_TSY10Y, value, 40);
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_CPIINFRATE, value, 40);
    ///
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_POPGROWTH, value, 40);
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_NUMHOUSEHOLDSGROWTH, value, 40);
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_RETAILSALESGROWTH, value, 40);
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_TOTNONFARMEMPGROWTH, value, 40);
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_NOMPERSONALINCGROWTH, value, 40);
    ///
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_HOMEPRICEGROWTH, value, 40);
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_BAACORPYIELD, value, 40);
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_ME, CMM_ME_CREPXIDXGROWTH, value, 40);
    ///
	///     set_cmm_custom_scenario(pDeal, CMM_FACTOR_TYPE_IR, CMM_IR_LIBOR1M, value, 40);
    ///
	///     char* custom_scen_name = "scenario1";
	///     
	///     SetupCMMCustomModel(pDeal, "user", "123456");
	///     set_current_moodys_cmm_scenario(pDeal, NULL, custom_scen_name);
    ///
    ///     generate_cmm_custom_result_output(pDeal, custom_scen_name);
	///     RemoveCMMCustomModel(pDeal);
    ///
	///     run_deal_ex(pDeal,pCmo);
	///     close_deal_ex(pDeal,pCmo);
	///     delete pCmo;
	///     pCmo = NULL;
    /// @endsample
    /// @note Generating CMM custom scenario file need the cmm input csv file, please make sure cmm data file have been downloaded.
	int CHASAPI generate_cmm_custom_result_output(void *tid, char* custom_scen_name);

    /// Sets the constant or vectored yield, repayment, principal payment, default, recovery, and purchase rates to be used for the pool specified by loan_num or for all pools if loan_num = -1, with the ability to apply to underlying deals if a reremic.
    ///
    /// @since 1.4.0
    /// @avail SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The underlying deal id or NULL.
    /// @param[in] assump_type Type of credit card rate. Must be one of CREDIT_CARD_ASSUMP_TYPE(def in ccmo.h):
    ///         - CREDIT_CARD_ASSUMP_YIELD(Portfolio/Annual Yield)
    ///         - CREDIT_CARD_ASSUMP_REPAYMENT(Repayment Rate)
    ///         - CREDIT_CARD_ASSUMP_DEFAULT (only use for Markit engine)
    ///         - CREDIT_CARD_ASSUMP_RECOVERY(Loss Rate)
    ///         - CREDIT_CARD_ASSUMP_PURCHASE(Purchase Rate)
	///         - CREDIT_CARD_ASSUMP_PRINCIPAL_PAYMENT(Principal Payment Rate)
    /// @param[in] is_vector The length of the vector pointed to by pval or 0 if pval is a constant.
    /// @param[in] pval A pointer to the prepayment speeds (or speed). Value for current period (0-indexed element) will not be applied.
    /// @param[in] loan_num The 1-based index of the loan or -1 to apply to all collateral in the deal.
    ///
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Invalid input parameter
    /// @retval  -3 Error - Invalid loan number
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @note
    ///     * Yield, repayment, principal payment, default, recovery, and purchase rates are expressed as decimals. 5.25% would be .0525.
    ///     * value for current period (0-indexed element) will not be applied.
    ///
	/// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "AMEXCAMT");  
    /// 
    ///     open_deal_ex(tid,pCmo);
    ///
    ///     CMO_STRUCT remics[10] = {0};
    ///     int deal_num = view_reremic_deals(tid,NULL,remics);
    ///
    ///     double prepay_rate = 0.28;
    ///     double purchase_rate = 0.23;
    ///     double annual_yield = 0.20;
    ///     double loss_rate = 0.03;
    /// 
    ///     for(int i = 0; i < deal_num; i++)
    ///     {
    ///         set_credit_card_assump_ex(tid, remics[i].dealid, CREDIT_CARD_ASSUMP_REPAYMENT, 0, &prepay_rate, -1);
    ///         set_credit_card_assump_ex(tid, remics[i].dealid, CREDIT_CARD_ASSUMP_PURCHASE, 0, &purchase_rate, -1);
    ///         set_credit_card_assump_ex(tid, remics[i].dealid, CREDIT_CARD_ASSUMP_YIELD, 0, &annual_yield, -1);
    ///         set_credit_card_assump_ex(tid, remics[i].dealid, CREDIT_CARD_ASSUMP_RECOVERY, 0, &loss_rate, -1);
    ///     }
    ///
    ///     run_deal_ex(tid,pCmo);
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    long CHASAPI set_credit_card_assump_ex(void* tid, const char* reremic_deal_id_or_null, CREDIT_CARD_ASSUMP_TYPE assump_type, short is_vector, double* pval, long loan_num);
    
    /// Moody's has five credit models that provide prepayment/default/loss vectors:
    /// -   DPLC for RMBS, Autos, SLABS, and Credit Cards (pool-level vectors)
    /// -   CMM for CMBS (loan-level vectors)
    /// -   MPA for RMBS (loan-level vectors)
    /// -   PA for Student Loan, Auto Loan, Lease, Credit Card deal
    /// -   Stress EDF for CLOs
    ///
    /// Currently DPLC and CMM vectors are delivered as part of a separate data feed.
    /// In addition to the vectors provided by the credit models, Moody's also has its own assumptions (e.g., recovery lag, delinquency rate, etc.) 
    /// For each of the asset classes/credit models. This method allows you to run these Moody's assumptions along with the vectors that you would receive as part of the data feed. 
    ///
    /// @since 1.4.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] credit_model The credit model that will be used in sfw deal. See ::MOODYS_CREDIT_MODEL_SETTINGS enumerations for details.
    /// @param[in] sets_up_only Flag to indicate whether user-provided vectors will be set. Currently, it used in CMM, SEDF, MPA, PA credit model.
    ///		For MPA/PA model, sets_up_only dictates whether user-specified index rates will be used.
	///			If sets_up_only=1, index rates from MPA/PA model output will be overridden by the user-specified index rates. Index rates from MPA/PA model output will be used if there is no user input of those rates.
	///			If sets_up_only=0, index rates from MPA/PA model output will be used.
	///		For CMM model,
	///			If sets_up_only=1, default and recovery rates from CMM will be overridden by user assumptions.
	///			If sets_up_only=0, none of the default and recovery vectors from CMM will be overridden by user assumptions.
	///		For SEDF model,
	///			If sets_up_only=1, the embedded assumptions on prepayment speeds, recovery rates and recovery lag can be overridden by user assumptions.
	///			If sets_up_only=0, none of the SEDF embedded assumptions will be overridden by user assumptions.
    ///
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Invalid credit model
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///     void* ptid = NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"BAFC08R2");
    ///
    ///     //open deal
    ///     open_deal_ex(ptid,&deal);
    ///
    ///     long ret = set_moodys_credit_model_settings(ptid, MOODYS_CMM_SETTINGS, true));
    ///
    ///     close_deal_ex(ptid,&deal);
    /// @endsample
    ///
    long CHASAPI set_moodys_credit_model_settings(void * tid, const MOODYS_CREDIT_MODEL_SETTINGS credit_model, BOOLYAN sets_up_only);

    /// Clear sfw credit model
    ///
    /// @since 1.4.0
    /// @avail SFW, CHS
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    ///
    /// @retval    0 No error
    /// @retval   -1 Error - Deal not opened
    /// @retval  -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
	/// @sample 
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strncpy(pCmo->settlement_date,"04/01/14",9);
    ///     strcpy(pCmo->dealid, "AMEXCAMT");  
    ///
    ///     set_engine_preference(PICK_CHS_ENGINE_FOR_MAPPED_DEALS);
    ///     open_deal_ex(tid,pCmo);
    ///
    ///     set_moodys_credit_model_settings(tid, MOODYS_PA_SETTINGS, false);
    ///     set_pa_default_pool_data(tid,"PurposePurchase","100");
    ///     set_pa_default_pool_data(tid,"PurposeRefi","0");
    ///     set_pa_default_pool_data(tid,"OccupancyOwner","50");
    ///     set_pa_default_pool_data(tid,"OccupancySecondHome","50");
    ///     set_pa_default_pool_data(tid,"OccupancyInvestor","0");
    ///     set_pa_default_pool_data(tid,"Property1Unit","100");
    ///     set_pa_default_pool_data(tid,"Property24Unit","0");
    ///     set_pa_default_pool_data(tid,"OriginatorThirdParty","50");
    ///     set_pa_default_pool_data(tid,"OriginatorRetail","50");
    ///     set_pa_default_pool_data(tid,"HARP1","50");
    ///     set_pa_default_pool_data(tid,"HARP2","50");
    ///     set_pa_default_pool_data(tid,"FHA","100");
    ///     replace_pa_pool_data(tid,1,"WAFICO","300");
    ///
    ///     run_deal_ex(tid,pCmo);
    ///
    ///     MARKIT_COLLAT_CASHFLOW collCashflow;
    ///     memset(&collCashflow, 0, sizeof(MARKIT_COLLAT_CASHFLOW) );
    ///     get_collateral_flow_ex1(tid, 0, 0, &collCashflow);
    ///
    ///     for(int i=1;i<collCashflow.size-2;i++)
    ///     {
    ///         EXPECT_GT(collCashflow.prepayments[i],0.0);
    ///     }
    ///
    ///     clear_moodys_credit_model_setttings(tid);
    ///
    ///     double CPR = 0;
    ///     set_prepayments_ex(tid, PREPAY_CURVE_CPR, 0, &CPR, -1, true);   
    ///     // run standard mode.
    ///     run_deal_ex(tid,pCmo);
    ///
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    long CHASAPI clear_moodys_credit_model_setttings(void * tid);

    /// Retrieves Moody's poolgroup ID given a group number.
    ///
    /// @since 1.4.0
    /// @avail SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[in] group_number The group number.
    /// @param[out] group_id A pointer to the user allocated string for group id.
    ///
    /// @retval   0 Success
    /// @retval -99 Error - For details call get_deal_error_msg()
    ///
    /// @note For CHS deals, method throws exception "not implemented in CHS engine"
    ///
	/// @sample
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "NSC0301");  
    ///
    ///     open_deal_ex(tid,pCmo);
    ///
    ///     char grp_id[12];
    ///     get_moodys_pool_group_id(tid,NULL,1,grp_id);
    ///
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    long CHASAPI get_moodys_pool_group_id(void *tid, const char* reremic_deal_id_or_null, int group_number, char* group_id);

    /// refer to install_collat_assump_cb() and install_collat_assump_cb_ex(), install_collat_assump_cb_ex1() add a param MOODYS_POOL_INFO.
    ///
    /// @since 2.9.3
    /// @avail CDOnet, SFW, CHS
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] collat_assump_cb_ex1 The user provided call back function.
    ///
    /// @retval    0 Success.
    /// @retval  -99 Error - use get_deal_error_msg() function to obtain text of error.
    ///
	/// @sample
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "AMEXCAMT");   
    ///  
    ///     open_deal_ex(tid, pCmo);
    ///
    ///     COLLAT_ASSUMP_CB_EX1 collat_assump_cb_ex1;
    ///     install_collat_assump_cb_ex1(tid, collat_assump_cb_ex1);
    /// 
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI install_collat_assump_cb_ex1(void *tid, COLLAT_ASSUMP_CB_EX1 collat_assump_cb_ex1);

    /// Retrieves deal update ID.
    ///
    /// @since 1.4.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[out] update_id A pointer to a client-allocated character array.
    /// @param[in] len The size of update_id.
    ///
    /// @retval    0 No error
    /// @retval  -99 Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
	/// @sample 
    ///     char updateid[20]={0};
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "AL2010-A");   
    ///     strncpy(pCmo->settlement_date,"05/01/11",strlen("05/01/11"));
    ///
    ///     open_deal_ex(tid,pCmo);
    ///
    ///     get_deal_update_id(tid,updateid, 20);
    ///  
    ///     close_deal_ex(tid,pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI get_deal_update_id(void *tid,char * const update_id, const int len);

    /// Returns the SDK deal and bond ID for an industry-standard bond identifier.
    /// Comparing with get_markit_id1(), this function takes two additional parameters specifying
    /// output array sizes so arbitrarily sized deal and bond can be accommodated.
    ///
    /// @since 1.4.0
    /// @avail ALL
    ///
    /// @param[in] id The US or international bond identifier.
    /// @param[out] deal The deal ID. Should pre-allocate at least 20 bytes for this field.
    /// @param[in] deal_length The length of deal.
    /// @param[out] bond The bond ID. Should pre-allocate at least 20 bytes for this field.
    /// @param[in] bond_length The length of bond
    /// @param[out] err_buffer The error message.
    /// @param[in] err_length The length of the error message.
    ///
    /// @retval    0 No error, but deal is not found
    /// @retval    1 Found deal successfully
    /// @retval  -99 Please examine err_buffer for error, deal buffer for deal, bond buffer for bond.
    ///
	/// @sample
    ///     const char *cusip;
    ///     char deal_id[20];
    ///     char bond_id[20];
    ///     char error[100]={0};
    ///
    ///     cusip = "78443CCA0";
    ///     get_moodys_id(cusip, deal_id, 20, bond_id, 20, error,100);
    /// @endsample
    ///
    int CHASAPI get_moodys_id(const char * id, char * deal, int deal_length, char * bond, int bond_length, char* err_buffer, int err_length);

    /// Gets Moodys SSFA calculation for a bond.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail SFW, CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid A pointer to the name of the bond.
    /// @param[out] ssfa_calc A pointer to the user allocated structure holding bond ssfa calculation.
    /// 
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval -99 Error - For details call get_deal_error_msg()
    ///
    /// @note Currently supported asset classes (based on MARKIT_DEAL_INFO::asset_type):
    ///     Asset Type  | MARKIT_DEAL_INFO::asset_type string matches
    ///     ----------- | -------------------------------------------  
    ///     RMBS        | MBS*, HELOC, Home_Equity, UK_RMBS, Whole_Loan
    ///     CMBS        | CMBS*
    ///     SLABS       | Student_Loan*
    ///     ABS         | ABS*, Auto*, Equip, FloorPlan, Manufactured_Housing, PPLN, SBA, StructNote
    ///     CDO         | CDO deals
    ///
    /// @sample
    ///     void* ptid = NULL;
    ///     CMO_STRUCT deal={};
    ///     strcpy(deal.dealid,"CMBS_BOA06001");
    ///
    ///     //open deal
    ///     open_deal_ex(ptid, &deal);
    ///
    ///     // SSFA variable
    ///     MOODYS_SSFA_CALC ssfa_calc={};
    ///     // get ssfa calc for bond "X"
    ///     get_moodys_ssfa_calc(ptid, "X", &ssfa_calc);
    ///
    ///     // close deal
    ///     close_deal_ex(ptid, &deal);
    /// @endsample
    ///
    long CHASAPI get_moodys_ssfa_calc(void* tid, const char* bondid, MOODYS_SSFA_CALC* ssfa_calc);

    /// Get trustee loan ID.
    /// 
    /// @since 1.6.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[in] loan_number The 1-based index of the loan.
    /// @param[out] trustee_loan_id A pointer to the user allocated string for account id, this must be pre-allocated with at least 20 characters.
    /// 
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval -99 Error - For details call get_deal_error_msg()
    ///
    /// @sample
    ///     void* ptid = NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"CMBS_BOA06001");
    ///
    ///     //open deal
    ///     open_deal_ex(ptid, &deal);
    ///
    ///     char trustee_loan_id[20];
    ///     char *reremic_deal_id_or_null = NULL;
    ///
    ///     assert(0 == get_trustee_loan_id(ptid, reremic_deal_id_or_null, 1, trustee_loan_id));
    ///     assert("C10071038" == trustee_loan_id);
    ///
    ///     assert(0 == get_trustee_loan_id(ptid, reremic_deal_id_or_null, 2, trustee_loan_id));
    ///     assert("C10071071" == trustee_loan_id);
    ///
    ///     close_deal_ex(ptid, &deal);
    /// @endsample
    ///
    long CHASAPI get_trustee_loan_id(void* tid, const char* reremic_deal_id_or_null, int loan_number, char* trustee_loan_id);

    /// Sets assumption default_till_end of Mortgage/ABS, with the ability to apply to underlying deals if it is a reremic.
    ///
    /// It sets if default assumption will apply to the end of cash flow projection:
    /// - If val set to TRUE (valid for SFW only), the default assumption will apply till end of projection, which means the defaults will occur to each period till last period of Mortgage/ABS.
    /// - If val set to FALSE, the default assumption will NOT apply to the last N periods of Mortgage/ABS, where N is the recovery lag set by set_recovery_lag_ex().
    ///   This is the case if this api is not called.
    /// 
    /// @since 1.6.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    ///	@note For CHS engine, if val set to TRUE, error code -99 will return.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] val The flag indicates whether default assumption continue till end of payment periods.
    /// @param[in] set_sup_remic If TRUE this will replace any specified underlying deal settings. If FALSE, this will NOT replace any underlying deal settings.
    ///
    /// @retval    0 No error
    /// @retval   -1 Error: Deal not opened
    /// @retval  -99 Error: Invalid dso identifier (tid) or apply True in a CHS deal 
    ///
    /// @sample
    ///		void* ptid = NULL;
    ///		CMO_STRUCT deal;
    ///		memset(&deal, 0, sizeof(CMO_STRUCT));
    ///		strcpy(deal.dealid,"AL2010-A");
    ///
    ///		// open deal
    ///		open_deal_ex(ptid, &deal);
    ///
    ///		double MDR = .0025;
    ///		set_defaults_ex(ptid, DEFAULT_CURVE_MDR, 0, &MDR, -1, false);
    ///		// months to liquidation is 3
    ///		set_recovery_lag_ex(ptid, 3, -1, false); 
    ///
    ///		// set default till end to true
    ///		set_default_till_end(ptid, true, false);
    ///
    ///		run_deal_ex(ptid,pCmo);
    ///		close_deal_ex(ptid, &deal);
    /// @endsample
    ///
    /// @sa set_recovery_lag_ex()
    ///
    long CHASAPI set_default_till_end(void * tid, BOOLYAN val, BOOLYAN set_sup_remic);

	/// Sets assumption recover defaults at Maturity/Call of Mortgage/ABS, with the ability to apply to underlying deals if it is a reremic.
    ///
    /// @since 3.0.0
    /// @avail SFW, CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] is_enabled If TRUE this will enable recover defaults at Maturity/Call. Otherwise it will not.
    /// @param[in] set_sup_remic If TRUE this will replace any specific underlying deal settings. Otherwise it will not.
    ///
    /// @retval    0 No error
    /// @retval   -1 Error: Deal not opened
    /// @retval  -99 Error: other errors, for details call get_deal_error_msg() 
    ///
    /// @sample
    ///		void* tid = NULL;
    ///		CMO_STRUCT *pCmo = new CMO_STRUCT;
    ///		memset(pCmo, 0, sizeof(CMO_STRUCT));
	///		strcpy(pCmo->dealid,"AL2010-A");
	/// 
    ///     // open deal
    ///		open_deal_ex(tid, pCmo);
    ///
    ///		// set recover defaults at Maturity/Call to true
	///     double MDR = .0025;
    ///		set_defaults_ex(tid, DEFAULT_CURVE_MDR, 0, &MDR, -1, false);
	///     set_service_advances_ex(tid, SERVICER_ADVANCES_BOTH, true);
	///     set_recovery_lag_ex(tid, 3, -1, false); 
	///     double Recovery = .55;
    ///     set_recoveries_ex(tid, 0, &Recovery, -1, true);
    ///
    ///     set_recover_at_maturity_call(tid, true, false);
	///
    ///     run_deal_ex(tid, pCmo);
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    long CHASAPI set_recover_at_maturity_call(void * tid, BOOLYAN is_enabled, BOOLYAN set_sup_remic);

    /// Sets the constant liquidation period for SFW deals.  The liquidation period is the number of months over which recoveries (from defaulted principal) are realized.  The recoveries will be realized in even slices over the number of months indicated.  
    /// This method can be used to set the liquidation period for an individual loan specified by the loan_num or for all loans if loan_num = -1,with the ability to apply to underlying deals if a reremic. 
    ///
    /// @since 1.6.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] period The specified liquidation period.
    /// @param[in] loan_num The 1-based index of the loan or -1 to apply to all collateral in the deal.
    /// @param[in] set_sup_remic If TRUE this will replace any specific underlying deal settings. Otherwise it will not.
    ///
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Other error
    /// @retval  -3 Error - Invalid loan number
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    /// 	void* ptid = NULL;
    /// 	CMO_STRUCT deal;
    /// 	memset(&deal, 0, sizeof(CMO_STRUCT));
    /// 	strcpy(deal.dealid,"AL2010-A");
    ///
    /// 	// open deal
    /// 	open_deal_ex(ptid,&deal);
    ///
    /// 	long ret = set_liquidation_period(ptid, 3, 1, false);
    ///
    /// 	close_deal_ex(ptid,&deal);
    /// @endsample
    ///
    /// @note liquidation period should be >= 1.Default settings period=1
    ///
    /// @sa set_liquidation_schedule()
    ///
    long CHASAPI set_liquidation_period(void *tid, const int period, long loan_num, BOOLYAN set_sup_remic);

    /// Sets liquidation schedule for SFW deals
    ///
    /// Sets the vectored liquidation schedule for SFW deals.  The value entered in the schedule for any month is the percent of recoveries/liquidation proceeds that will be realized that month.  The schedule must sum to 100%.  
    /// This method can be used to set the liquidation schedule for an individual loan specified by the loan_num or for all loans if loan_num = -1,with the ability to apply to underlying deals if a reremic.
    ///
    /// @since 1.6.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] vector_length The length of liquidation schedule vector.
    /// @param[in] pval A pointer to the liquidation schedule vector.
    /// @param[in] loan_num The 1-based index of the loan or -1 to apply to all collateral in the deal.
    /// @param[in] set_sup_remic If TRUE this will replace any specific underlying deal settings. Otherwise it will not.
    ///
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Other error
    /// @retval  -3 Error - Invalid loan number
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///     void* ptid = NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"AL2010-A");
    ///
    ///     // open deal
    ///     open_deal_ex(ptid,&deal);
    ///
    ///     // double global_schedule[1] ={1};
    ///     const int vector_len = 4;
    ///     double schedule_vector[vector_len] = {0,0.5,0,0.5};
    ///     // long ret = set_liquidation_schedule(ptid, 1, global_schedule, -1, false);
    ///     long ret = set_liquidation_schedule(ptid, vector_len, schedule_vector, 1,false);
    ///
    ///     close_deal_ex(ptid,&deal);
    /// @endsample
    ///
    /// @note The liquidation schedule are expressed as decimals. e.g. 6.23% would be .0623, liquidation schedule vector max length is 60, and all items sum must equal to 100%.
    ///
    /// @warning  The default setting is to use liquidation period. If the user would like to use liquidation schedule instead of liquidation period, then the user must first call this method with loan_num = -1 to set the deal-level liquidation schedule before calling this method to set liquidation schedules for individual loans.
    ///
    /// @sa set_liquidation_period()
    ///
    long CHASAPI set_liquidation_schedule(void *tid, short vector_length, double* pval, long loan_num, BOOLYAN set_sup_remic);

    /// Sets the calculation method for a deal to run. SFW can let user to choose among several calculation methods to calculate default, prepayment and scheduled principal, when projecting cashflows of a deal.
    ///
    /// @since 1.6.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] method_index The specified method type, should be enums of ::PREPAY_DEFAULT_CALC_METHOD_TYPE.
    /// @param[in] set_sup_remic If TRUE this will replace any specific underlying deal settings. Otherwise it will not.
    ///
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Other error
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    /// 	void* ptid = NULL;
    /// 	//deal has been opened
    ///
    /// 	int ret = set_calculation_method(ptid, SCHED_PRIN_PREPAY_BEFORE_DEFAULT_PPYDEF, false);
    /// 	if(ret != 0)
    /// 	{
    /// 	    //error handling
    /// 	}
    /// @endsample
    ///
    /// @note method_index must be one of ::PREPAY_DEFAULT_CALC_METHOD_TYPE.
    ///     If not called, default method is ::PREPAY_DEFAULT_BEFORE_SCHED_PRIN_PPYDEF.
    ///
    /// @warning Student loan deal cannot set to ::JAPANESE_PREPAY_DEFAULT_PPYDEF.
    int CHASAPI set_calculation_method(void *tid, PREPAY_DEFAULT_CALC_METHOD_TYPE method_index, BOOLYAN set_sup_remic);

    /// Gets current calculation method of SFW deals
    ///
    /// The method retrieves the index number which corresponds to the calculation method of a deal.
    ///
    /// @since 1.6.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    ///
    /// @retval   >0 current method index
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Other error
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    /// 	void* ptid = NULL;
    /// 	//deal has been opened
    ///
    /// 	int ret = get_calculation_method(ptid, null);
    /// 	if(ret <= 0)
    /// 	{
    /// 	    //error handling
    /// 	}
    /// @endsample
    ///
    /// @note If successful returned, the value is one of ::PREPAY_DEFAULT_CALC_METHOD_TYPE.
    int CHASAPI get_calculation_method(void *tid, const char* reremic_deal_id_or_null);


    /// Sets when to realize a loss.
    ///
    /// @since 1.6.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] realized_at_liquidation flag when to realize a loss.
    ///            FALSE: loss will be realized when the default occurs;
    ////           TRUE: loss will be realized when liquidation occurs for Mortage/ABS collateral type deals or user-specified months prior liquidation for Student Loan collateral type deals.
    /// @param[in] months_prior_liquidation For Mortgage/ABS collateral type deals this input should be zero. For Student Loan deals, this input should be between 0 to recover lag of the asset to indicate in which month during default to liquidation to realize a the loss.
    /// @param[in] set_sup_remic If TRUE this will replace any specific underlying deal settings. Otherwise it will not.
    ///
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Other error
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///     void* ptid = NULL;
    ///     // deal has been opened
    ///
    ///     int ret = set_realized_losses_at_liquidation(ptid, true, 1, false);
    ///     if(ret != 0)
    ///     {
    ///         //error handling
    ///     }
    /// @endsample
    ///
    /// @note if not set, the default value of realized losses at liquidation would be TRUE.
    ///       months_prior_liquidation only work for student loan deals when realized losses at liquidation is TRUE, and the value should not be greater than the recovery delay of deal.
    ///
    /// @warning For Mortgage/ABS deals, regardless of the setting losses at liquidation, losses will always be realized at liquidation when the calculation method is JAPANESE_PREPAY_DEFAULT_PPYDEF or PREPAY_DEFAULT_BEFORE_SCHED_PRIN_PPYDEF
    int CHASAPI set_realized_losses_at_liquidation(void *tid, BOOLYAN realized_at_liquidation, int months_prior_liquidation, BOOLYAN set_sup_remic);
    
    /// Sets liquidation periodicity.
    ///
    /// @since 2.7.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] liquidation_periodicity_type Type to indicate which liquidation periodicity type to use, refer to enum ::LIQUIDATION_PERIODICITY_TYPE.
    /// @param[in] set_sup_remic If TRUE this will replace any specific underlying deal settings. Otherwise it will not.
    ///
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Invalid liquidation periodicity type.
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///     void* ptid = NULL;
    ///     // deal has been opened
    ///
    ///     int ret = set_liquidation_periodicity(ptid, LIQUIDATION_QUARTERLY, false);
    ///     if(ret != 0)
    ///     {
    ///         //error handling
    ///     }
    /// @endsample
    ///
    /// @note 
    /// By default, liquidation periodicity is monthly.
    /// The function is not available for SLABS deals.
    int CHASAPI set_liquidation_periodicity(void * tid, short liquidation_periodicity_type, BOOLYAN set_sup_remic);

    /// Gets the list of available CMM scenarios of SFW deal.
    ///
    /// @since 1.6.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[out] scenario_list A pointer to the user allocated string array to store the returned CMM scenarios, it need at least 20 characters for each scenario name. If scenario_list is NULL, this function will only return the total count of scenarios.
    ///
    /// @retval >=0 The total count of scenarios.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT;
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "CMBS_BOA00002");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(tid, pCmo));
    ///
    ///     int scenario_count = get_moodys_cmm_scenarios(tid, NULL, NULL);
    ///     assert(scenario_count > 0);
    ///
    ///     char **scenarios = new char*[scenario_count];
    ///     for (int i = 0; i < scenario_count; ++i)
    ///     	scenarios[i] = new char[20];
    ///     assert(scenario_count == get_moodys_cmm_scenarios(tid, NULL, scenarios));
    ///     
    ///     char current_scenario[20];
    ///     assert(0 == set_current_moodys_cmm_scenario(tid, NULL, scenarios[3]));
    ///     assert(0 == get_current_moodys_cmm_scenario(tid, NULL, current_scenario));
    ///     assert(0 == strcmp(scenarios[3], current_scenario));
    ///     
    ///     assert(0 == run_deal_ex(tid, pCmo));
    ///     assert(0 == close_deal_ex(tid, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @see
    /// - set_current_moodys_cmm_scenario()
    /// - get_current_moodys_cmm_scenario()
    ///
    int CHASAPI get_moodys_cmm_scenarios(void *tid, const char *reremic_deal_id_or_null, char *scenario_list[]);

    /// Set current CMM scenario of SFW deal.
    ///
    /// @since 1.6.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[in] cmm_scenario A pointer to a CMM scenario string.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Input cmm_scenario is not exist.
    /// @retval  -3 Error - Current credit model is not CMM.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @see
    /// - get_moodys_cmm_scenarios()
    /// - get_current_moodys_cmm_scenario()
    ///
	/// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT;
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "CMBS_BOA00002");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     open_deal_ex(tid, pCmo);
    ///
    ///     int scenario_count = get_moodys_cmm_scenarios(tid, NULL, NULL);
    ///     assert(scenario_count > 0);
    ///
    ///     char **scenarios = new char*[scenario_count];
    ///     for (int i = 0; i < scenario_count; ++i)
    ///     	scenarios[i] = new char[20];
    ///     assert(scenario_count == get_moodys_cmm_scenarios(tid, NULL, scenarios));  
    ///
    ///     set_current_moodys_cmm_scenario(tid, NULL, scenarios[3]);
    ///
    ///     run_deal_ex(tid, pCmo);
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;   
    /// @endsample
    ///
    int CHASAPI set_current_moodys_cmm_scenario(void *tid, const char *reremic_deal_id_or_null, const char *cmm_scenario);

    /// Gets current CMM scenario of SFW deal.
    ///
    /// @since 1.6.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[out] cmm_scenario A pointer to the user allocated string to store the returned CMM scenario, it need at least 20 characters for the scenario name.
    ///
    /// @retval   0 Success.
    /// @retval  -3 Error - Current credit model is not CMM.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @see
    /// - get_moodys_cmm_scenarios()
    /// - set_current_moodys_cmm_scenario()
    ///
	/// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT;
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "CMBS_BOA00002");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     open_deal_ex(tid, pCmo);
    ///
    ///     int scenario_count = get_moodys_cmm_scenarios(tid, NULL, NULL);
    ///     assert(scenario_count > 0);
    ///
    ///     char **scenarios = new char*[scenario_count];
    ///     for (int i = 0; i < scenario_count; ++i)
    ///     	scenarios[i] = new char[20];
    ///     assert(scenario_count == get_moodys_cmm_scenarios(tid, NULL, scenarios));
    ///     
    ///     set_current_moodys_cmm_scenario(tid, NULL, scenarios[3]);
    ///
    ///     char current_scenario[20];  
    ///     get_current_moodys_cmm_scenario(tid, NULL, current_scenario);
    ///     assert(0 == strcmp(scenarios[3], current_scenario));
    ///     
    ///     run_deal_ex(tid, pCmo);
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;   
    /// @endsample
    ///
    int CHASAPI get_current_moodys_cmm_scenario(void *tid, const char *reremic_deal_id_or_null, char *cmm_scenario);

    /// This function will set location of support data for MPA analysis explicitly, or default location will be used(deal_input_path/MPA/data/).
    /// Note that Support data DB file has been discontinued since MPA version 4.0.45 and the data has been embedded in the MPA API libraries.
    ///
    /// Since 2.9.6, this function can also set location of MPA(Mortgage Portfolio Analyzer) API library for MPA analysis explicitly,
    /// or default location of MPA(Mortgage Portfolio Analyzer) API library will be same as the application folder for wsa.dll.
    ///
    /// @since 2.0.0
    /// @avail SFW
    ///
    /// @pre NULL.
    ///
    /// @param[in] path Pointing to the location(folder) of "SupportData.db".
    ///
    /// @retval   0 Success.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @warning  This function must be called prior to call set_moodys_credit_model_settings() function to set mpa model.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///
    ///     assert(0 == set_mpa_data_path("./mpadata/"));
    ///
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, false));
    ///     assert(0 == set_mpa_analysis_type(pDeal, MPA_MEDC_SINGLE_PATH));
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_mpa_data_path(const char* path);

    /// Set the analysis type for MPA model.
    ///
    /// @since 2.0.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to MPA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] type MPA Analysis type.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Invalid MPA analysis type.
    /// @retval  -3 Error - Current credit model is not MPA.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, false));
    ///     
    ///     int ret = set_mpa_analysis_type(pDeal, MPA_MEDC_SINGLE_PATH);
    ///     if(ret < 0)
    ///     {
    ///         //Error handling
    ///         return;
    ///     }
    ///     
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_mpa_analysis_type(void* tid, MPA_ANALYSIS_TYPE type);

    /// This function will get array of scenarios names available in current MPA module.
    ///
    /// @since 2.0.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to MPA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[out] scenario_list A array of char pointers that points to current names of MPA scenarios, it need at least 100 characters for each scenario name. Pass NULL for scenario_list to get just the number of scenarios.
    ///
    /// @retval  >0 Number of scenarios found.
    /// @retval  0  None scenarios found.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Invalid MPA analysis type.
    /// @retval  -3 Error - Current credit model is not MPA.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);  
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, false));
    ///     assert(0 == set_mpa_analysis_type(pDeal, MPA_MEDC_SINGLE_PATH));
    ///
    ///     int scenario_count = get_mpa_scenarios(pDeal, NULL);
    ///     assert(scenario_count > 0);
    ///     
    ///     std::vector<char *> scenario_list(scenario_count);
    ///     std::vector<char> scenario_strs(scenario_count*100);
    ///
    ///     for (int i = 0; i < scenario_count; ++i)
    ///         scenario_list[i] = &scenario_strs[i*100];
    ///
    ///     assert(scenario_count == get_mpa_scenarios(pDeal, &scenario_list[0]));
    /// 
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @see
    /// - set_current_mpa_scenario()
    /// - get_current_mpa_scenario()
    ///
    int CHASAPI get_mpa_scenarios(void *tid, char *scenario_list[]);

    /// This function will set a credit scenario for current MPA module. idx will be index of array scenarios obtains from function get_mpa_scenarios().
    ///
    /// @since 2.0.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to MPA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] idx An index indicating which scenario to be set for MPA.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Invalid idx.
    /// @retval  -3 Error - Current credit model is not MPA.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);   
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, false));
    ///     assert(0 == set_mpa_analysis_type(pDeal, MPA_MEDC_SINGLE_PATH));
    ///      
    ///     int scenario_count = get_mpa_scenarios(pDeal, NULL);
    ///     assert(scenario_count > 0);
    ///     
    ///     assert(0 == set_current_mpa_scenario(pDeal, scenario_count-1));
    ///     
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @see
    /// - get_mpa_scenarios()
    /// - get_current_mpa_scenario()
    int CHASAPI set_current_mpa_scenario(void *tid, int idx);

    /// This function will return the index of the MPA credit scenario that is using under current opened deal.
    ///
    /// @since 2.0.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to MPA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    ///
    /// @retval  >0 Index of scenario which applying current deal .
    /// @retval  -1 Error - Deal not open.
    /// @retval  -3 Error - Current credit model is not MPA.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, false));
    ///     assert(0 == set_mpa_analysis_type(pDeal, MPA_MEDC_SINGLE_PATH));
    ///     
    ///     int scenario_count = get_mpa_scenarios(pDeal, NULL);
    ///     assert(scenario_count > 0);
    ///     
    ///     assert(0 == set_current_mpa_scenario(pDeal, scenario_count-1));
    ///
    ///     assert(scenario_count-1 == get_current_mpa_scenario(pDeal));
    ///     
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @see
    /// - get_mpa_scenarios()
    /// - set_current_mpa_scenario()
    int CHASAPI get_current_mpa_scenario(void *tid);

    /// This function will set recovery lag (judicial and non-judicial) for MPA analysis.
    ///
    /// @since 2.0.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to MPA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] judicial_lag Input value for judicial lag which applies to all loan in judicial state.
    /// @param[in] non_judicial_lag Input value for non judicial lag which applies to all loan in non-judicial state.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Invalid judicial/non-judicial lags.
    /// @retval  -3 Error - Current credit model is not MPA.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, false));
    ///     assert(0 == set_mpa_analysis_type(pDeal, MPA_MEDC_SINGLE_PATH));
    ///     assert(0 == set_current_mpa_scenario(pDeal, 1));
    ///
    ///     assert(0 == set_mpa_recovery_lag_by_state(pDeal, 20, 25));
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_mpa_recovery_lag_by_state(void* tid, int judicial_lag, int non_judicial_lag);

    /// This function will set recovery lag on loan level for MPA analysis. User are able to set a constant recovery lag number or vector of recovery lag for MPA analysis.
    ///
    /// @since 2.0.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to MPA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] is_vector The length of the vector pointed to by pval or 0 if pval is a constant. 
    /// @param[in] pval A pointer to recovery lag vector or a constant recovery lag value.
    /// @param[in] loan_num Loan number, indicating which loan will be applied by this function, -1 for all collateral loans.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Invalid loan number or pval pointer.
    /// @retval  -3 Error - Current credit model is not MPA.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, false));
    ///     assert(0 == set_mpa_analysis_type(pDeal, MPA_MEDC_SINGLE_PATH));
    ///     assert(0 == set_current_mpa_scenario(pDeal, 1));
    ///
    ///     int recoveryLag = 20;
    ///     assert(0 == set_mpa_recovery_lag(pDeal, 0, &recoveryLag, -1)); // set for all loans
    ///     
    ///     recoveryLag = 10;
    ///     assert(0 == set_mpa_recovery_lag(pDeal, 0, &recoveryLag, 1));  // loan 1
    ///     
    ///     int recoveries[200];
    ///     int *precv = recoveries;
    ///     std::fill(recoveries, recoveries+sizeof(recoveries)/sizeof(recoveries[0]), 0);
    ///     std::fill(precv, precv+12, 1); precv+=12;
    ///     std::fill(precv, precv+12, 2); precv+=12;
    ///     std::fill(precv, precv+12, 3); precv+=12;
    ///     std::fill(precv, precv+12, 4); precv+=12;
    ///     ASSERT_EQ(0, set_mpa_recovery_lag(pDeal, sizeof(recoveries)/sizeof(recoveries[0]), recoveries, 2)); // loan 2
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    /// @note 
	/// For deal level input(loan_num = -1), it will use the first value of inputted vectors.
    /// For individual loan input, it supports vectors.
    int CHASAPI set_mpa_recovery_lag(void* tid, short is_vector, int *pval, long loan_num);

    /// This function will set multipliers for MPA analysis, including prepay, default and severity. By default, MPA runs as all multiplier with value 1.00.
    ///
    /// @since 2.0.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to MPA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] type The type of MPA multiplier.
    /// @param[in] is_vector The length of the vector pointed to by pval or 0 if pval is a constant.
    /// @param[in] pval A pointer to the multiplier vector to be set.
    /// @param[in] loan_num Loan number, -1 for all collateral loans.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Invalid MPA multiplier or invalid loan number.
    /// @retval  -3 Error - Current credit model is not MPA.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, false));
    ///     assert(0 == set_mpa_analysis_type(pDeal, MPA_MEDC_SINGLE_PATH));
    ///
    ///     assert(0 == set_current_mpa_scenario(pDeal, 1));
    ///     assert(0 == set_mpa_recovery_lag_by_state(pDeal, 30, 35));
    ///
    ///     // deal level settings
    ///     double ppy_multiplier = 2.0;
    ///     double def_multiplier = 3.0;
    ///     double sev_multiplier = 4.0;
    ///     assert(0 == set_mpa_multiplier(pDeal, MPA_MULTIPLIER_PREPAY, 0, &ppy_multiplier, -1));
    ///     assert(0 == set_mpa_multiplier(pDeal, MPA_MULTIPLIER_DEFAULT, 0, &def_multiplier, -1));
    ///     assert(0 == set_mpa_multiplier(pDeal, MPA_MULTIPLIER_SEVERITY, 0, &sev_multiplier, -1));
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @note 
	/// For deal level input(loan_num = -1), it will use the first value of inputted vectors.
    /// For individual loan input, it supports vectors.
    int CHASAPI set_mpa_multiplier(void *tid, MPA_MULTIPLIER_TYPE type, short is_vector, double *pval, long loan_num);

    /// This function will set the haircut for MPA analysis with option to apply seasoning or not.
    ///
    /// @since 2.0.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to MPA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] is_vector The length of the vector pointed to by pval or 0 if pval is a constant.
    /// @param[in] pval A pointer to the haircut vector to be set.
    /// @param[in] seasoning Flag for applying seasoning or not.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Invalid haircut vector or invalid seasoning flag.
    /// @retval  -3 Error - Current credit model is not MPA.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, false));
    ///     assert(0 == set_mpa_analysis_type(pDeal, MPA_MEDC_SINGLE_PATH));
    ///     assert(0 == set_current_mpa_scenario(pDeal, 1));
    ///
    ///     double haircut_vector[500];
    ///     std::fill(haircut_vector, haircut_vector + 500, 3.0);
    ///     std::fill(haircut_vector, haircut_vector + 12, 7.0);
    ///     assert(0 == set_mpa_haircut(pDeal, 500, haircut_vector, 0));
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_mpa_haircut(void* tid, short is_vector, double *pval, BOOLYAN seasoning);

    /// This function will set length of MPA simulation.
    ///
    /// @since 2.0.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to MPA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] length The MPA simulation length in months. The valid input is [1,499]; if input is 1 or 2, the function will automatically change it to 3, so the return length is [3, 499].
    ///
    /// @retval  >0 The simulation length that actually used in MPA analysis.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Invalid length.
    /// @retval  -3 Error - Current credit model is not MPA.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, false));
    ///     assert(0 == set_mpa_analysis_type(pDeal, MPA_MEDC_SINGLE_PATH));
    ///     assert(0 == set_current_mpa_scenario(pDeal, 1));
    ///
    ///     assert(200, set_mpa_simulation_length(pDeal, 200));
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_mpa_simulation_length(void* tid, int length);

    /// When running MPA, this function will set default attribute for the loan which missing certain data field.
    ///
    /// @since 2.0.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to MPA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] loan_data_field The name of the loan data field, the "Default Loan Data" paragraph shows more details.
    /// @param[in] value Value for the data field to set.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - invalid parameter loan_data_field or value.
    /// @retval  -3 Error - Current credit model is not MPA.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @par Default Loan Data
    ///     | loan_data_filed | Default Value | valid input | Note |
    ///     | ------------- | ------------- | ------------- | ------------- |
    ///     | "Loan ID" | "111000" | | |
    ///     | "Mortgage Type" | "Unknown" | "Unknown", "10 yr. CMT balloons","15 yr. CMT balloons","3 yr. Balloon","5 yr. Balloon","7 yr. Balloon","10 yr. Balloon","15 yr. Balloon", "FIXED RATE","Fixed rate simple interest","Fixed rate fully amortizing","EOMs","Treasury 1/5 caps","Treasury 2/6 caps","Treasury 3/1 yr.","Treasury 5/1 yr.","Treasury 7/1 yr.","Treasury 10/1 yr.","Treasury 10/1 yr (40 yr loans)","Treasury negative amortization", "Treasury no periodic & life caps","Treasury neg am no periodic nor life caps","COFI","COFI 1/5 caps","COFI 2/6 caps, FHLB","COFI negative amortization", "COFI neg am (No caps)","Fixed 5 yrs/COFI neg am","Step loans","6 month LIBOR no neg am","6 month LIBOR neg am","1 mo. LIBOR no neg am", "1 mo. LIBOR neg am","3 mo. LIBOR no neg am","All other neg am","5/25","7/23","7/23 or 5/25","Simple interest","GEM","GPM", "GEM w/ buydown","CD based","ARM Neg Am CMT balloon","Neg Am LIBOR balloon","Neg Am COFI balloon","Fixed 1 yrs/LIBOR","Fixed 2 yrs/LIBOR", "Fixed 3 yrs/LIBOR","Fixed 5 yrs/LIBOR","Fixed 7 yrs/LIBOR","Fixed 7 yrs/COFI no neg am","15 yr LIBOR balloon","1 yr LIBOR","Prime based","Prime based neg am", "ARM 6 Mo. CMT Neg Am","ARM 3 yr CMT Neg Am","ARM 1 yr CMT Neg Am","Arms 1 yr CMT","Arms 3 yr CMT","Arms 5 yr CMT","ARM 10 yr CMT","ARM 2 yr CMT", "ARM 4 yr CMT","ARM 6 mo. CMT","ARM 7 yr CMT","Others"| |
    ///     | "Property Type" | "S: Single family" | "", "C: Condominium", "D: Deminimus PUD", "F: 2-4 Family", "J: Co-op", "K: Land, parking lot", "L: Leasehold", "M: Manufacturing, moho", "N: New contruction", "O: Condotel", "P: Planned unit development","S: Single family", "T: Townhouse, Row house", "Z: Mixed used" | |
    ///     | "Occupancy Type" | "P: Owner-occupied" | " ","P: Owner-occupied", "S: Second homes","I: Investor", "R: Rentor" | |
    ///     | "Purpose Type" | "P: Purpose Money", | " ","P: Purpose Money", "R: Rate/term refinance", "C: Cash-out refinance", "D: Debt consolidated", "H: Home improvement" | |
    ///     | "Lien Position" | 1 | Max value is 2 | |
    ///     | "Documentation" | "Full Income - No Assets" | "Unknown","Full Income - Full Assets", "Full Assets - Partial Income etc.", "Full Income - No Assets","Partial Income - Stated Assets etc.", "Stated Income - Partial Assets etc.", "No Income - Partial Assets", "No Income - Stated Assets", "No Income - No Assets - VOE", "No Income - No Assets - No VOE" | |
    ///     | "LTV" | 80 | | |
    ///     | "Junior LTV" | 20 | | |
    ///     | "FICO" | 670 | | |
    ///     | "Original Amount" | 24000.0 | | |
    ///     | "Securitized Amount" | 20000.0 | | |
    ///     | "Current Amount" | 24000.0 |  | |
    ///     | "Senior Balance" | 20000.0 |  | |
    ///     | "HELOC Max Draw Amount" | 1.0 | | 1.0 * Initial draw amount |
    ///     | "Gross Coupon" | 6.0 | | |
    ///     | "Gross Margin" | 2.5 | | |
    ///     | "Original Term" | 360 | | |
    ///     | "Amortization Term" | 360 | | |
    ///     | "Cutoff Age" | 3 | | |
    ///     | "Loan Status" | "Current" | " ","Current", "Paid Off", "Delinquent", "Foreclosed", "Bankrupt", "REO","Repurchased", "Liquidated", "Closed", | if set with "Delinquent", usermust input "Delinquent:x", x stands for months in delinquency |
    ///     | "State" | "CA" | "N/A","AL", "AK", "AZ", "AR", "CA", "CO", "CT", "DE","FL", "GA", "HI", "ID", "IL", "IN", "IA", "KS","KY", "LA", "ME", "MD", "MA", "MI", "MN", "MS","MO", "MT", "NE", "NV", "NH", "NJ", "NM", "NY","NC", "ND", "OH", "OK", "OR", "PA", "RI", "SC","SD", "TN", "TX", "UT", "VT", "VA", "WA", "WV","WI", "WY", "DC", "AB", "BC", "MB", "NB","NL", "NS", "NT", "NU", "ON", "PE", "QC", "SK","YT", "PR" | |
    ///     | "Zip Code" | "94945" | | |
    /// 
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, false));
    ///     assert(0 == set_mpa_analysis_type(pDeal, MPA_MEDC_SINGLE_PATH));
    ///     assert(0 == set_current_mpa_scenario(pDeal, 1));
    ///
    ///     assert(0 == set_mpa_default_loan_data(pDeal, "FICO", "500"));
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI set_mpa_default_loan_data(void* tid, const char* loan_data_field, const char* value);

    /// This function will enable api to use historical data to generate default/recovery/prepay vectors.
    ///
    /// @since 2.0.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to MPA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] use Flag to indicate whether use historical data to generate performance vectors.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
	/// @retval  -2 Error - No need to apply mid course adjustment data as the MPA version is 5.0 or above.
    /// @retval  -3 Error - Current credit model is not MPA.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, false));
    ///     assert(0 == set_mpa_analysis_type(pDeal, MPA_MEDC_SINGLE_PATH));
    ///     assert(0 == set_current_mpa_scenario(pDeal, 1));
    ///
    ///     assert(0 == set_mpa_mid_course_adj(pDeal, true));
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_mpa_mid_course_adj(void* tid, BOOLYAN use);

    /// This function will set a user defined scenario, which contains customized forecast number for several economic indicators.
    ///
    /// @since 2.0.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to MPA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] factor Identifier to set, should be one of factor name. The "Factor And Scope" paragraph shows more detail about this field.
    /// @param[in] scope Scope of the indicator, apply with "US" or the value in table "Region" and "State".
    /// @param[in] year A pointer to a year array.
    /// @param[in] quarter A pointer to a quarter array.
    /// @param[in] value A pointer to a value array.
    /// @param[in] length The length of year/quarter/value array.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Invalid factor, or invalid scope on economic factor, use get_deal_error_msg() to see details.
    /// @retval  -3 Error - Current credit model is not MPA.
    /// @retval  -4 Error - Invalid MPA analysis type for customized scenario.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @par Factor And Scope
    ///     | Factor Name | Description |
    ///     | ----------- | ----------- |
    ///     | "UNEMPLOYMENT" | U.S. Unemployment Rate , apply scope with "US". | 
    ///     | "HPI" | U.S. HPI, House Price Index , apply scope with "US". | 
    ///     | "GDP" | U.S. HP,Gross Domestic Product , apply scope with "US". | 
    ///     | "TSY1Y" | Treasury 1 Year , apply scope with "US". | 
    ///     | "TSY10Y" | Treasury 10 Year , apply scope with "US". | 
    ///     | "LIBOR6MSPREAD" | 6 Month LIBOR Spread , apply scope with "US". | 
    ///     | "FREDMAC" | Freddie Mac Rate , apply scope with "US". | 
    ///     | "REG_HPI" | Region House Price Index, apply scope with the value in the "Region" table. | 
    ///     | "REG_UNEMPLOYMENT" | Region Unemployment Rate, apply scope with the value in the "Region" table. | 
    ///     | "STATE_GDP" | State GDP, apply scope with the value in the "State" table. | 
    ///     | "TREASURY3M" | Treasury 3 Month, full economy. | 
    ///     | "TREASURY6M" | Treasury 6 Month, full economy. | 
    ///     | "TREASURY1YR" | Treasury 1 Year, full economy. | 
    ///     | "TREASURY5YR" | Treasury 5 Year, full economy. | 
    ///     | "TREASURY10YR" | Treasury 10 Year, full economy. | 
    ///     | "LIBOR1M" | 1 Month LIBOR, full economy. |     
    ///     | "LIBOR3M" | 3 Month LIBOR, full economy. |  
    ///     | "LIBOR6M" | 6 Month LIBOR, full economy. |  
    ///     | "LIBOR12M" | 12 Month LIBOR, full economy. |  
    ///     | "PRIME" | Prime, full economy. |  
    ///     | "FREDDIEMAC30YR" | Freddie Mac 30 Year, full economy. |  
	///     | "FREDDIEMAC5YR" | Freddie Mac 5 Year, full economy. | 
	///     | "FREDDIEMAC15YR" | Freddie Mac 15 Year, full economy. | 
    ///
    /// @par State
    ///     | Scope Name | Description | Scope Name | Description |
    ///     | ----------- | ----------- | ----------- | ----------- |
    ///     | "AK" | Alaska | "MT" | Montana |
    ///     | "AL" | Alabama | "NC" | North Carolina |
    ///     | "AR" | Arkansas | "ND" | North Dakota |
    ///     | "AZ" | Arizona | "NE" | Nebraska |
    ///     | "CA" | California | "NH" | New Hampshire |
    ///     | "CO" | Colorado | "NJ" | New Jersey |
    ///     | "CT" | Connecticut | "NM" | New Mexico |
    ///     | "DC" | District Of Columbia | "NV" | Nevada |
    ///     | "DE" | Delaware | "NY" | New York |
    ///     | "FL" | Florida | "OH" | Ohio |
    ///     | "GA" | Georgia | "OK" | Oklahoma |
    ///     | "HI" | Hawaii | "OR" | Oregon |
    ///     | "IA" | Iowa | "PA" | Pennsylvania |
    ///     | "ID" | Idaho | "RI" | Rhode Island |
    ///     | "IL" | Illinois | "SC" | South Carolina |
    ///     | "IN" | Indiana | "SD" | South Dakota |
    ///     | "KS" | Kansas | "TN" | Tennessee |
    ///     | "KY" | Kentucky | "TX" | Texas |
    ///     | "LA" | Louisiana | "UT" | Utah |
    ///     | "MA" | Massachusetts | "VA" | Virginia |
    ///     | "MD" | Maryland | "VT" | Vermont |
    ///     | "ME" | Maine | "WA" | Washington |
    ///     | "MI" | Michigan | "WI" | Wisconsin |
    ///     | "MN" | Minnesota | "WV" | West Virginia |
    ///     | "MO" | Missouri | "WY" | Wyoming |
    ///     | "MS" | Mississippi | | |
    ///
    /// @par Region
    ///     | Scope Name | Description | Scope Name | Description |
    ///     | ----------- | ----------- | ----------- | ----------- |
    ///     | "MABI" | Abilene, TX | "MLAN" | Lansing-East Lansing, MI |
    ///     | "MAKR" | Akron, OH | "MLAR" | La Crosse, WI-MN |
    ///     | "MALA" | Albany-Schenectady-Troy, NY | "MLAS" | Las Vegas-Paradise, NV |
    ///     | "MALB" | Albuquerque, NM | "MLAT" | Lawton, OK |
    ///     | "MALE" | Alexandria, LA | "MLEB" | Lebanon, PA |
    ///     | "MALL" | Allentown-Bethlehem-Easton, PA-NJ | "MLET" | Lewiston, ID-WA |
    ///     | "MALN" | Albany, GA | "MLEW" | Lewiston-Auburn, ME |
    ///     | "MALT" | Altoona, PA | "MLEX" | Lexington-Fayette, KY |
    ///     | "MAMA" | Amarillo, TX | "MLIM" | Lima, OH |
    ///     | "MAME" | Ames, IA | "MLIN" | Lincoln, NE |
    ///     | "MANC" | Anchorage, AK | "MLIT" | Little Rock-North Little Rock-Conway, AR |
    ///     | "MAND" | Anderson, IN | "MLOA" | Logan, UT-ID |
    ///     | "MANE" | Anderson, SC | "MLOG" | Longview, TX |
    ///     | "MANI" | Anniston-Oxford, AL | "MLON" | Longview, WA |
    ///     | "MANN" | Ann Arbor, MI | "MLOS" | Los Angeles-Long Beach-Santa Ana, CA |
    ///     | "MAPP" | Appleton, WI | "MLOU" | Louisville-Jefferson County, KY-IN |
    ///     | "MASH" | Asheville, NC | "MLSC" | Las Cruces, NM |
    ///     | "MATA" | Atlantic City-Hammonton, NJ | "MLUB" | Lubbock, TX |
    ///     | "MATH" | Athens-Clarke County, GA | "MLWR" | Lawrence, KS |
    ///     | "MATL" | Atlanta-Sandy Springs-Marietta, GA | "MLYN" | Lynchburg, VA |
    ///     | "MAUB" | Auburn-Opelika, AL | "MMAC" | Macon, GA |
    ///     | "MAUG" | Augusta-Richmond County, GA-SC | "MMAD" | Madison, WI |
    ///     | "MAUS" | Austin-Round Rock-San Marcos, TX | "MMAM" | Mankato-North Mankato, MN |
    ///     | "MBAK" | Bakersfield-Delano, CA | "MMAN" | Manhattan, KS |
    ///     | "MBAL" | Baltimore-Towson, MD | "MMAR" | Madera-Chowchilla, CA |
    ///     | "MBAN" | Bangor, ME | "MMAS" | Mansfield, OH |
    ///     | "MBAR" | Barnstable Town, MA | "MMCA" | McAllen-Edinburg-Mission, TX |
    ///     | "MBAT" | Baton Rouge, LA | "MMCD" | Merced, CA |
    ///     | "MBCR" | Battle Creek, MI | "MMED" | Medford, OR |
    ///     | "MBCY" | Bay City, MI | "MMEM" | Memphis, TN-MS-AR |
    ///     | "MBEA" | Beaumont-Port Arthur, TX | "MMIA" | Miami-Fort Lauderdale-Pompano Beach, FL |
    ///     | "MBEL" | Bellingham, WA | "MMIC" | Michigan City-La Porte, IN |
    ///     | "MBIL" | Billings, MT | "MMID" | Midland, TX |
    ///     | "MBIN" | Binghamton, NY | "MMIL" | Milwaukee-Waukesha-West Allis, WI |
    ///     | "MBIR" | Birmingham-Hoover, AL | "MMIN" | Minneapolis-St. Paul-Bloomington, MN-WI |
    ///     | "MBLC" | Blacksburg-Christiansburg-Radford, VA | "MMIS" | Missoula, MT |
    ///     | "MBLD" | Boulder, CO | "MMNC" | Manchester-Nashua, NH |
    ///     | "MBLM" | Bloomington, IN | "MMOB" | Mobile, AL |
    ///     | "MBLO" | Bloomington-Normal, IL | "MMOD" | Modesto, CA |
    ///     | "MBND" | Bend, OR | "MMOE" | Monroe, MI |
    ///     | "MBOI" | Boise City-Nampa, ID | "MMOG" | Morgantown, WV |
    ///     | "MBOS" | Boston-Cambridge-Quincy, MA-NH | "MMON" | Montgomery, AL |
    ///     | "MBOW" | Bowling Green, KY | "MMOR" | Monroe, LA |
    ///     | "MBRE" | Bremerton-Silverdale, WA | "MMOV" | Mount Vernon-Anacortes, WA |
    ///     | "MBRP" | Bridgeport-Stamford-Norwalk, CT | "MMOW" | Morristown, TN |
    ///     | "MBRW" | Brownsville-Harlingen, TX | "MMUN" | Muncie, IN |
    ///     | "MBSM" | Bismarck, ND | "MMUS" | Muskegon-Norton Shores, MI |
    ///     | "MBSW" | Brunswick, GA | "MMYB" | Myrtle Beach-North Myrtle Beach-Conway, SC |
    ///     | "MBUF" | Buffalo-Niagara Falls, NY | "MNAA" | Napa, CA |
    ///     | "MBUN" | Burlington, NC | "MNAH" | Nashville-Davidson--Murfreesboro--Franklin, TN |
    ///     | "MBUR" | Burlington-South Burlington, VT | "MNAP" | Naples-Marco Island, FL |
    ///     | "MCAJ" | Cape Girardeau-Jackson, MO-IL | "MNEH" | New Haven-Milford, CT |
    ///     | "MCAN" | Canton-Massillon, OH | "MNEO" | New Orleans-Metairie-Kenner, LA |
    ///     | "MCAR" | Carson City, NV | "MNEY" | New York-Northern New Jersey-Long Island, NY-NJ-PA |
    ///     | "MCAS" | Casper, WY | "MNIL" | Niles-Benton Harbor, MI |
    ///     | "MCCF" | Cape Coral-Fort Myers, FL | "MNOW" | Norwich-New London, CT |
    ///     | "MCED" | Cedar Rapids, IA | "MNPT" | North Port-Bradenton-Sarasota, FL |
    ///     | "MCHA" | Champaign-Urbana, IL | "MOCA" | Ocala, FL |
    ///     | "MCHE" | Cheyenne, WY | "MOCE" | Ocean City, NJ |
    ///     | "MCHI" | Chicago-Joliet-Naperville, IL-IN-WI | "MODE" | Odessa, TX |
    ///     | "MCHO" | Chico, CA | "MOGD" | Ogden-Clearfield, UT |
    ///     | "MCHR" | Charlotte-Gastonia-Rock Hill, NC-SC | "MOKL" | Oklahoma City, OK |
    ///     | "MCHS" | Charleston-North Charleston-Summerville, SC | "MOLY" | Olympia, WA |
    ///     | "MCHT" | Chattanooga, TN-GA | "MOMA" | Omaha-Council Bluffs, NE-IA |
    ///     | "MCHV" | Charlottesville, VA | "MORL" | Orlando-Kissimmee-Sanford, FL |
    ///     | "MCHW" | Charleston, WV | "MOSH" | Oshkosh-Neenah, WI |
    ///     | "MCIN" | Cincinnati-Middletown, OH-KY-IN | "MOWE" | Owensboro, KY |
    ///     | "MCLA" | Clarksville, TN-KY | "MOXN" | Oxnard-Thousand Oaks-Ventura, CA |
    ///     | "MCLD" | Cleveland, TN | "MPAL" | Palm Bay-Melbourne-Titusville, FL |
    ///     | "MCLE" | Cleveland-Elyria-Mentor, OH | "MPAN" | Panama City-Lynn Haven-Panama City Beach, FL |
    ///     | "MCOE" | Coeur d'Alene, ID | "MPAR" | Parkersburg-Marietta-Vienna, WV-OH |
    ///     | "MCOL" | Columbus, GA-AL | "MPAS" | Pascagoula, MS |
    ///     | "MCOM" | Columbia, MO | "MPEN" | Pensacola-Ferry Pass-Brent, FL |
    ///     | "MCON" | Columbus, IN | "MPEO" | Peoria, IL |
    ///     | "MCOO" | Colorado Springs, CO | "MPHI" | Philadelphia-Camden-Wilmington, PA-NJ-DE-MD |
    ///     | "MCOR" | Corpus Christi, TX | "MPHO" | Phoenix-Mesa-Glendale, AZ |
    ///     | "MCOS" | Columbia, SC | "MPIN" | Pine Bluff, AR |
    ///     | "MCOU" | Columbus, OH | "MPIS" | Pittsfield, MA |
    ///     | "MCOV" | Corvallis, OR | "MPIT" | Pittsburgh, PA |
    ///     | "MCRE" | Crestview-Fort Walton Beach-Destin, FL | "MPLM" | Palm Coast, FL |
    ///     | "MCSB" | College Station-Bryan, TX | "MPOC" | Pocatello, ID |
    ///     | "MCUM" | Cumberland, MD-WV | "MPOR" | Portland-South Portland-Biddeford, ME |
    ///     | "MDAG" | Dalton, GA | "MPOT" | Portland-Vancouver-Hillsboro, OR-WA |
    ///     | "MDAI" | Danville, IL | "MPOU" | Poughkeepsie-Newburgh-Middletown, NY |
    ///     | "MDAL" | Dallas-Fort Worth-Arlington, TX | "MPRE" | Prescott, AZ |
    ///     | "MDAV" | Davenport-Moline-Rock Island, IA-IL | "MPRO" | Providence-New Bedford-Fall River, RI-MA |
    ///     | "MDAY" | Dayton, OH | "MPRV" | Provo-Orem, UT |
    ///     | "MDEC" | Decatur, IL | "MPSL" | Port St. Lucie, FL |
    ///     | "MDEL" | Deltona-Daytona Beach-Ormond Beach, FL | "MPUE" | Pueblo, CO |
    ///     | "MDEN" | Denver-Aurora-Broomfield, CO | "MPUG" | Punta Gorda, FL |
    ///     | "MDES" | Des Moines-West Des Moines, IA | "MRAC" | Racine, WI |
    ///     | "MDET" | Detroit-Warren-Livonia, MI | "MRAL" | Raleigh-Cary, NC |
    ///     | "MDEZ" | Decatur, AL | "MRAP" | Rapid City, SD |
    ///     | "MDNV" | Danville, VA | "MREA" | Reading, PA |
    ///     | "MDOT" | Dothan, AL | "MRED" | Redding, CA |
    ///     | "MDOV" | Dover, DE | "MREN" | Reno-Sparks, NV |
    ///     | "MDUB" | Dubuque, IA | "MRIC" | Richmond, VA |
    ///     | "MDUL" | Duluth, MN-WI | "MRIV" | Riverside-San Bernardino-Ontario, CA |
    ///     | "MDUR" | Durham-Chapel Hill, NC | "MROA" | Roanoke, VA |
    ///     | "MEAU" | Eau Claire, WI | "MROC" | Rockford, IL |
    ///     | "MELC" | El Centro, CA | "MROE" | Rochester, MN |
    ///     | "MELI" | Elizabethtown, KY | "MROH" | Rochester, NY |
    ///     | "MELK" | Elkhart-Goshen, IN | "MROM" | Rocky Mount, NC |
    ///     | "MELM" | Elmira, NY | "MROR" | Rome, GA |
    ///     | "MELP" | El Paso, TX | "MSAA" | Santa Rosa-Petaluma, CA |
    ///     | "MERI" | Erie, PA | "MSAC" | Sacramento--Arden-Arcade--Roseville, CA |
    ///     | "MEUG" | Eugene-Springfield, OR | "MSAD" | Sandusky, OH |
    ///     | "MEVA" | Evansville, IN-KY | "MSAE" | Salem, OR |
    ///     | "MFAE" | Fayetteville, NC | "MSAF" | San Francisco-Oakland-Fremont, CA |
    ///     | "MFAI" | Fairbanks, AK | "MSAG" | Saginaw-Saginaw Township North, MI |
    ///     | "MFAM" | Farmington, NM | "MSAJ" | San Jose-Sunnyvale-Santa Clara, CA |
    ///     | "MFAR" | Fargo, ND-MN | "MSAL" | Salinas, CA |
    ///     | "MFAY" | Fayetteville-Springdale-Rogers, AR-MO | "MSAN" | San Diego-Carlsbad-San Marcos, CA |
    ///     | "MFLA" | Flagstaff, AZ | "MSAO" | San Angelo, TX |
    ///     | "MFLI" | Flint, MI | "MSAS" | Salisbury, MD |
    ///     | "MFLO" | Florence, SC | "MSAT" | Santa Barbara-Santa Maria-Goleta, CA |
    ///     | "MFLR" | Florence-Muscle Shoals, AL | "MSAU" | St. George, UT |
    ///     | "MFLW" | Fond du Lac, WI | "MSAV" | Savannah, GA |
    ///     | "MFOC" | Fort Collins-Loveland, CO | "MSAX" | Santa Cruz-Watsonville, CA |
    ///     | "MFOR" | Fort Smith, AR-OK | "MSAY" | Salt Lake City, UT |
    ///     | "MFOW" | Fort Wayne, IN | "MSAZ" | San Antonio-New Braunfels, TX |
    ///     | "MFRE" | Fresno, CA | "MSEA" | Seattle-Tacoma-Bellevue, WA |
    ///     | "MGAD" | Gadsden, AL | "MSFE" | Santa Fe, NM |
    ///     | "MGAG" | Gainesville, GA | "MSHB" | Sheboygan, WI |
    ///     | "MGAI" | Gainesville, FL | "MSHE" | Sherman-Denison, TX |
    ///     | "MGLF" | Glens Falls, NY | "MSHR" | Shreveport-Bossier City, LA |
    ///     | "MGOL" | Goldsboro, NC | "MSIO" | Sioux City, IA-NE-SD |
    ///     | "MGRA" | Grand Rapids-Wyoming, MI | "MSIU" | Sioux Falls, SD |
    ///     | "MGRB" | Green Bay, WI | "MSLO" | San Luis Obispo-Paso Robles, CA |
    ///     | "MGRE" | Great Falls, MT | "MSOU" | South Bend-Mishawaka, IN-MI |
    ///     | "MGRF" | Grand Forks, ND-MN | "MSPA" | Spartanburg, SC |
    ///     | "MGRJ" | Grand Junction, CO | "MSPF" | Springfield, OH |
    ///     | "MGRL" | Greeley, CO | "MSPI" | Springfield, MA |
    ///     | "MGRN" | Greensboro-High Point, NC | "MSPM" | Springfield, MO |
    ///     | "MGRV" | Greenville-Mauldin-Easley, SC | "MSPO" | Spokane, WA |
    ///     | "MGUL" | Gulfport-Biloxi, MS | "MSPR" | Springfield, IL |
    ///     | "MGVL" | Greenville, NC | "MSTC" | St. Cloud, MN |
    ///     | "MHAF" | Hanford-Corcoran, CA | "MSTE" | Steubenville-Weirton, OH-WV |
    ///     | "MHAI" | Harrisburg-Carlisle, PA | "MSTG" | State College, PA |
    ///     | "MHAN" | Harrisonburg, VA | "MSTJ" | St. Joseph, MO-KS |
    ///     | "MHAR" | Hartford-West Hartford-East Hartford, CT | "MSTL" | St. Louis, MO-IL |
    ///     | "MHAS" | Hagerstown-Martinsburg, MD-WV | "MSTO" | Stockton, CA |
    ///     | "MHAT" | Hattiesburg, MS | "MSUT" | Sumter, SC |
    ///     | "MHIC" | Hickory-Lenoir-Morganton, NC | "MSWB" | Scranton--Wilkes-Barre, PA |
    ///     | "MHIN" | Hinesville-Fort Stewart, GA | "MSYR" | Syracuse, NY |
    ///     | "MHMT" | Houma-Bayou Cane-Thibodaux, LA | "MTAL" | Tallahassee, FL |
    ///     | "MHOL" | Holland-Grand Haven, MI | "MTAM" | Tampa-St. Petersburg-Clearwater, FL |
    ///     | "MHON" | Honolulu, HI | "MTER" | Terre Haute, IN |
    ///     | "MHOT" | Hot Springs, AR | "MTEX" | Texarkana, TX-Texarkana, AR |
    ///     | "MHOU" | Houston-Sugar Land-Baytown, TX | "MTOL" | Toledo, OH |
    ///     | "MHUN" | Huntsville, AL | "MTOP" | Topeka, KS |
    ///     | "MHUT" | Huntington-Ashland, WV-KY-OH | "MTRE" | Trenton-Ewing, NJ |
    ///     | "MIDA" | Idaho Falls, ID | "MTUC" | Tucson, AZ |
    ///     | "MIND" | Indianapolis-Carmel, IN | "MTUL" | Tulsa, OK |
    ///     | "MIOW" | Iowa City, IA | "MTUS" | Tuscaloosa, AL |
    ///     | "MITH" | Ithaca, NY | "MTYL" | Tyler, TX |
    ///     | "MJAC" | Jacksonville, FL | "MUTI" | Utica-Rome, NY |
    ///     | "MJAK" | Jackson, MI | "MVAD" | Valdosta, GA |
    ///     | "MJAM" | Jackson, MS | "MVAL" | Vallejo-Fairfield, CA |
    ///     | "MJAN" | Janesville, WI | "MVER" | Sebastian-Vero Beach, FL |
    ///     | "MJAS" | Jacksonville, NC | "MVIC" | Victoria, TX |
    ///     | "MJAT" | Jackson, TN | "MVIN" | Vineland-Millville-Bridgeton, NJ |
    ///     | "MJEF" | Jefferson City, MO | "MVIR" | Virginia Beach-Norfolk-Newport News, VA-NC |
    ///     | "MJOB" | Jonesboro, AR | "MVIS" | Visalia-Porterville, CA |
    ///     | "MJOH" | Johnstown, PA | "MWAC" | Waco, TX |
    ///     | "MJON" | Johnson City, TN | "MWAE" | Waterloo-Cedar Falls, IA |
    ///     | "MJOP" | Joplin, MO | "MWAR" | Warner Robins, GA |
    ///     | "MKAK" | Kankakee-Bradley, IL | "MWAS" | Washington-Arlington-Alexandria, DC-VA-MD-WV |
    ///     | "MKAL" | Kalamazoo-Portage, MI | "MWAU" | Wausau, WI |
    ///     | "MKAN" | Kansas City, MO-KS | "MWEN" | Wenatchee-East Wenatchee, WA |
    ///     | "MKIL" | Killeen-Temple-Fort Hood, TX | "MWHE" | Wheeling, WV-OH |
    ///     | "MKIN" | Kingsport-Bristol-Bristol, TN-VA | "MWIC" | Wichita, KS |
    ///     | "MKIS" | Kingston, NY | "MWIH" | Wichita Falls, TX |
    ///     | "MKNE" | Kennewick-Pasco-Richland, WA | "MWII" | Williamsport, PA |
    ///     | "MKNO" | Knoxville, TN | "MWIM" | Wilmington, NC |
    ///     | "MKOK" | Kokomo, IN | "MWIN" | Winchester, VA-WV |
    ///     | "MLAA" | Lafayette, LA | "MWIS" | Winston-Salem, NC |
    ///     | "MLAC" | Lancaster, PA | "MWOR" | Worcester, MA |
    ///     | "MLAD" | Laredo, TX | "MYAK" | Yakima, WA |
    ///     | "MLAE" | Lakeland-Winter Haven, FL | "MYOR" | York-Hanover, PA |
    ///     | "MLAF" | Lafayette, IN | "MYOU" | Youngstown-Warren-Boardman, OH-PA |
    ///     | "MLAH" | Lake Havasu City-Kingman, AZ | "MYUB" | Yuba City, CA |
    ///     | "MLAK" | Lake Charles, LA | "MYUM" | Yuma, AZ |
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, false));
    ///     assert(0, set_mpa_analysis_type(pDeal, MPA_CUST_MEDC_SINGLE_PATH));
    ///
    ///     int UsUemployment_year[] = {2013,2014,2014,2014,2014,2015,2015,2015,2015,2016,2016,2016,2016,2017,2017,2017,2017,2018,2018,2018};
    ///     int UsUemployment_quarter[] = {4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3};
    ///     double UsUemployment_value[] = {7.89,7.85,7.77,7.56,7.67,7.42,7.39,7.31,7.26,7.29,7.33,7.28,7.25,7.17,7.21,7.19,7.14,7.03,7.01,6.88};
    ///     assert(0 == set_mpa_custom_scenario(pDeal, "UNEMPLOYMENT", "US", UsUemployment_year, UsUemployment_quarter, UsUemployment_value, 20));
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI set_mpa_custom_scenario(void * tid, const char* factor, const char* scope, const int* year, const int* quarter, const double* value, int length);

    /// This function will return the MPA model economy date(year and quarter).
    ///
    /// @since 2.0.0
    /// @avail All
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[out] year The year of the MPA economy date, e.g. 2013.
    /// @param[out] quarter The quarter of the MPA economy date, the value of quarter should be an integer in the range of [1, 4].
    ///
    /// @retval   0 Success.
    /// @retval  -2 Error - Invalid year or quarter pointers.
    /// @retval  -3 Error - Current credit model is not MPA.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///
    ///     int year = 0, quarter = 0;
    ///     assert(0 == get_mpa_economy_date(pDeal, &year, &quarter)); // e.g. year = 2013, quarter = 4
    ///
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, false));
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI get_mpa_economy_date(void *tid, int *year, int *quarter);

    /// This function will set the number of path for MPA simulation.
    ///
    /// @since 2.0.1
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called and set MPA mode to simulation.
    /// @pre The current credit model has been set to MPA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] number The number of path for MPA simulation, the value should be in [1, 10000]. The default path number is 10000.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Invalid input simulation number.
    /// @retval  -3 Error - Current credit model is not MPA or not in MPA simulation mode.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, false));
    ///     assert(0 == set_mpa_analysis_type(pDeal, MPA_LOSS_SIMULATION));
    ///
    ///     assert(0 == set_mpa_simulation_path_num(pDeal, 5));
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_mpa_simulation_path_num(void *tid, int number);

    /// This function will set mpa the probability of non-payment setting.
    ///
    /// @since 2.0.1
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called and set MPA mode to simulation.
    /// @pre The current credit model has been set to MPA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] probability The probability of non-payment, the value should be in [0,1.0].
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Invalid input probability value.
    /// @retval  -3 Error - Current credit model is not MPA or not in MPA simulation mode.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, false));
    ///
    ///     assert(0 == set_mpa_insurance_non_payment(pDeal, 0.5));
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_mpa_insurance_non_payment(void *tid, double probability);

    /// This function will save computation time by only running waterfalls for subset of paths generated by MPA.
    /// - For all paths in the tail section, each of them will be run with waterfall.
    /// - For paths in the non-tails section , only opt_percent * 100% of non-tail paths will be run with waterfall.
    ///
    /// @exptl Subject to change
    /// @since 2.2.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to MPA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] toggle 1 for open the optimization, 0(default value) for turning off the optimization.
    /// @param[in] tail_percent The tail percentage(in decimals, pass 0.1 indicate 10%) of all path generated.
    /// @param[in] opt_percent  The optimization percentage(in decimals, pass 0.1 indicate 10%) for non-tail paths.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid tail percentage or optimization percentage input.
    /// @retval  -3 Current credit model is not MPA.
    /// @retval  -4 Invalid analysis type, this function should be only called with simulation run.
    /// @retval -99 Other error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SAS059XS");   
    /// 
    ///     open_deal_ex(tid, pCmo);
    ///     set_moodys_credit_model_settings(tid, MOODYS_MPA_SETTINGS, true);
    ///
    ///     set_mpa_optimization(tid, true, 0.05, 0.05);
    /// 
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
	///
    /// @note By default, API won't use any optimization.
    int CHASAPI set_mpa_optimization(void * tid, BOOLYAN toggle, double tail_percent, double opt_percent);

    /// This function will set floor and cap value for Prepay/Default/Severity when performing MPA analysis in WSA API.
    ///
    /// @exptl Subject to change
    /// @since 2.2.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to MPA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] param_type Indicating which type to set (either prepay, default, or severity ).
    /// @param[in] floor Floor value for current param_type.
    /// @param[in] cap Cap value for current param_type.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid floor or cap value.
    /// @retval  -3 Current credit model is not MPA.
    /// @retval -99 Other error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SAS059XS");   
    /// 
    ///     open_deal_ex(tid, pCmo);
    ///     set_moodys_credit_model_settings(tid, MOODYS_MPA_SETTINGS, true);
    ///
    ///     set_mpa_stress_range(tid, MPA_ANALYSIS_PARAM_PREPAY, 0.1, 0.3);
    /// 
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;  
    /// @endsample
	///
    int CHASAPI set_mpa_stress_range(void * tid, MPA_ANALYSIS_PARAM param_type, double floor, double cap);

    /// This function overrides the probabilities of default for loans with 30/60 days delinquent.
    ///
    /// @exptl Subject to change
    /// @since 2.2.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to MPA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] deq_30days Default probability for loans with 30 days delinquent, the value should be in [0.0, 1.0].
    /// @param[in] deq_60days Default probability for loans with 60 days delinquent, the value should be in [0.0, 1.0].
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid deq_30days or deq_60days value, should be in [0.0, 1.0].
    /// @retval  -3 Current credit model is not MPA.
    /// @retval -99 Other error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
	///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SAS059XS");   
    /// 
    ///     open_deal_ex(tid, pCmo);
    ///     set_moodys_credit_model_settings(tid, MOODYS_MPA_SETTINGS, true);
    ///
    ///     set_mpa_delinquent_pd(tid, 0.1, 0.1);
    /// 
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
	///
    int CHASAPI set_mpa_delinquent_pd(void * tid, double deq_30days , double deq_60days);

    /// This function will set offset value and offset unit of LTV and FICO when performing MPA analysis in WSAAPI.
    ///
    /// @exptl Subject to change
    /// @since 2.2.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to MPA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] type Indicating which field to set(either LTV or FICO), should be one of ::MPA_ANALYSIS_PARAM_OFFSET.
    /// @param[in] unit 0(default value) means disable the offset type; 1 means the offset unit is "%"; 2 means the offset unit is "+/-".
    /// @param[in] offset Offset value for current offset type.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid offset type, or unit is not 1 or 2.
    /// @retval  -3 Current credit model is not MPA.
    /// @retval -99 Other error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SAS059XS");   
    /// 
    ///     open_deal_ex(tid, pCmo);
    ///     set_moodys_credit_model_settings(tid, MOODYS_MPA_SETTINGS, true);
    ///
    ///     set_mpa_offset(tid, MPA_ANALYSIS_PARAM_OFFSET_LTV, 1, 2.00);
    /// 
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;  
    /// @endsample
	///
    int CHASAPI set_mpa_offset(void * tid, MPA_ANALYSIS_PARAM_OFFSET type, int unit, double offset);

    /// This function is to set confidence level for MPA simulation.
    ///
    /// @exptl Subject to change
    /// @since 2.2.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to MPA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] confidence_level Confidence level to be set, the value should be in [0, 1.00].
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid confidence level, the value should be in [0, 1.00].
    /// @retval  -3 Current credit model is not MPA.
    /// @retval  -4 Invalid analysis type, this function should be only called with simulation run.
    /// @retval -99 Other error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SAS059XS");   
    /// 
    ///     open_deal_ex(tid, pCmo);
    ///     set_moodys_credit_model_settings(tid, MOODYS_MPA_SETTINGS, true);
    ///
    ///     set_mpa_confidence_level(tid, 1.00);
    /// 
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;  
    /// @endsample
	///
    int CHASAPI set_mpa_confidence_level(void * tid, double confidence_level);

    /// This method gets the issue date of a collateral group by group number.
    ///
    /// @since 1.6.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] group_number The 1-based group number of the reqeusted pool group.
    /// @param[out] date A pointer to a null-terminated string (of format YYYYMMDD). This parameter must be pre-allocated with at least 11 characters.
    ///
    /// @retval   0 The issued date of the requested pool group has been obtained successfully.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid pool group number.
    /// @retval -99 Error - Other error, call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///    void* tid = NULL;
    ///    CMO_STRUCT *pCmo = new CMO_STRUCT;
    ///    memset(pCmo, 0, sizeof(*pCmo));
    ///    strcpy(pCmo->dealid, "CMBS_BOA06006");
    ///
    ///    set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///    assert(0 == open_deal_ex(tid, pCmo));
    ///
    ///    int group_number = 1;
    ///    char date[11];
    ///
    ///    assert(0 == get_group_issue_date(tid, group_number, date));
    ///    assert(0 == strcmp("20061101", date));
    ///
    ///    assert(0 == run_deal_ex(tid, pCmo));
    ///    assert(0 == close_deal_ex(tid, pCmo));
    ///    delete pCmo;
    ///    pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI get_group_issue_date(void *tid, int group_number, char *date);

    /// This method gets the available pool history dates for the specified pool group, the format is in YYYYMM.
    ///
    /// @since 1.6.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] groupNumber The 1-based group number of the reqeusted pool group.
    /// @param[in] sizeOfYYYYMMs The size of array YYYYMMs provided.
    /// @param[out] YYYYMMs The list of available pool history dates (of format YYYYMMDD). This parameter must be pre-allocated before call this function.
    /// @param[out] numAvailable Total number of available YYYYMMs.
    ///
    /// @retval   >=0 Actual number of dates returned.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid pool group number.
    /// @retval -99 Error - Other error, call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* ptid = NULL;
    ///     // deal has been opened
    ///
    ///     int yyyymm[1] = {0};
    ///     int avail_num=0;
    ///     int ret = get_moodys_pool_history_avail_YYYYMMs(ptid, 1, yyyymm, 1, &avail_num);
    ///     if(ret < 0)
    ///     {
    ///         //error handling
    ///     }
    /// @endsample
    ///
    int CHASAPI get_moodys_pool_history_avail_YYYYMMs(void *tid, int groupNumber, int YYYYMMs[], int sizeOfYYYYMMs, int* numAvailable);

    /// This method gets the historical data of YYYYMM for the specified pool group, if YYYYMM=0 would return all historical data for the specified pool group.
    ///
    /// @since 1.6.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] groupNumber The 1-based group number of the reqeusted pool group.
    /// @param[in] sizeOfHistoryArray The size of array pool_history provided.
    /// @param[in] YYYYMM The specified date (of format YYYYMM),if YYYYMM=0 would return all historical data for the specified pool group.
    /// @param[out] poolHistory The list of pool history. This parameter must be pre-allocated before call this function.
    ///
    /// @retval   >=0 Actual size of history array returned.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid pool group number.
    /// @retval -99 Error - Other error, call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* ptid = NULL;
    ///     // deal has been opened
    ///
    ///     int yyyymm[1] = {0};
    ///     int avail_num=0;
    ///     int ret = get_moodys_pool_history_avail_YYYYMMs(ptid, 1, yyyymm, 1, &avail_num);
    ///     if(ret < 0)
    ///     {
    ///         // error handling
    ///     }
    ///
    ///     MOODYS_POOL_HISTORY poolhistory[1];
    ///     int actual_size=0;
    ///     ret = get_moodys_pool_history(ptid, 1, poolhistory, 1, yyyymm[0],&actual_size);//get history of specified YYYYMM
    ///     if(ret < 0)
    ///     {
    ///         // error handling
    ///     }
    ///
    ///     MOODYS_POOL_HISTORY* poolhistoryArray = new MOODYS_POOL_HISTORY[avail_num];   
    ///     int actual_size=0;
    ///     ret = get_moodys_pool_history(ptid, 1, poolhistoryArray, 1, 0,&actual_size);//get all history for the group 1
    ///     if(ret < 0)
    ///     {
    ///         // error handling
    ///     }
    ///     delete [] poolhistoryArray;
    ///     poolhistoryArray = NULL;
    /// @endsample
    ///
    int CHASAPI get_moodys_pool_history(void* tid, int groupNumber, MOODYS_POOL_HISTORY poolHistory[], int sizeOfHistoryArray, int YYYYMM);

    /// This method gets the available bond history dates for the specified bondid, the format is in YYYYMM.
    ///
    /// @since 1.6.0
    /// @avail SFW CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondId The bond name.
    /// @param[in] sizeOfYYYYMMs The size of array YYYYMMs provided.
    /// @param[out] YYYYMMs The list of available bond history dates (of format YYYYMMDD). This parameter must be pre-allocated before call this function.
    /// @param[out] numAvailable Total number of available YYYYMMs.
    ///
    /// @retval   >=0 Actual number of dates returned.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid bond id.
    /// @retval -99 Error - Other error, call get_deal_error_msg() for detail.
    ///
    /// @sample
    /// 	void* ptid = NULL;
    /// 	// deal has been opened
    ///
    /// 	int yyyymm[1] = {0};
    /// 	int avail_num=0;
    /// 	int ret = get_moodys_bond_history_avail_YYYYMMs(ptid, "A1", yyyymm, 1, &avail_num);
    /// 	if(ret < 0)
    /// 	{
    /// 	    //error handling
    /// 	}
    /// @endsample
    ///
    int CHASAPI get_moodys_bond_history_avail_YYYYMMs(void *tid, const char* bondId, int YYYYMMs[], int sizeOfYYYYMMs, int* numAvailable);

    /// This method gets the historical data of YYYYMM for the specified bondid, if YYYYMM=0 would return all historical data for the specified bondid.
    ///
    /// @since 1.6.0
    /// @avail SFW CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondId The bond name.
    /// @param[in] sizeOfHistoryArray The size of array pool_history provided.
    /// @param[in] YYYYMM The specified date (of format YYYYMMDD),if YYYYMM=0 would return an array of all historical data for the specified bondid, else it would return a historical data structure of specified YYYYMM.
    /// @param[out] bondHistory The list of bond history. This parameter must be pre-allocated before call this function.
    ///
    /// @retval   >=0 Actual size of history array returned.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid bond id.
    /// @retval -99 Error - Other error, call get_deal_error_msg() for detail.
    ///
    /// @sample
    /// 	void* ptid = NULL;
    /// 	// deal has been opened
    ///
    /// 	int yyyymm[1] = {0};
    /// 	int avail_num = 0;
    /// 	int ret = get_moodys_bond_history_avail_YYYYMMs(ptid, "A1", yyyymm, 1, &avail_num);
    /// 	if(ret < 0)
    /// 	{
    /// 	    // error handling
    /// 	}
    ///
    /// 	MOODYS_BOND_HISTORY bondhistory[1];
    /// 	ret = get_moodys_bond_history(ptid, "A1", bondhistory, 1, yyyymm[0]); // get history of specified YYYYMM for "A1"
    /// 	if(ret < 0)
    /// 	{
    /// 	    // error handling
    /// 	}
    ///
    /// 	MOODYS_BOND_HISTORY* bondhistoryArray = new MOODYS_BOND_HISTORY[avail_num];   
    /// 	ret = get_moodys_bond_history(ptid, "A1", bondhistoryArray, 1, 0); // get all history for "A1"
    /// 	if(ret < 0)
    /// 	{
    /// 	    // error handling
    /// 	}
    /// 	delete [] bondhistoryArray;
    /// 	bondhistoryArray = NULL;
    /// @endsample
    ///
    int CHASAPI get_moodys_bond_history(void* tid, const char* bondId, MOODYS_BOND_HISTORY bondHistory[], int sizeOfHistoryArray, int YYYYMM);
 
    /// This method retrieves the additional descriptive student loan information of a piece of collateral specified by index.
    ///
    /// @since 1.6.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] index The 0-based index of a piece of collateral (-1 for all collateral).
    /// @param[in] length The size of the MOODYS_STUDENT_LOAN_INFO structure.
    /// @param[out] all_colls[] A client-allocated array of MOODYS_STUDENT_LOAN_INFO structures.
    ///
    /// @retval  >=0 No error.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -3 Error - Invalid loan index.
    /// @retval  -4 Error - Invalid size.
    /// @retval  -5 Error - No output vector passed.
    /// @retval -99 Error - Other error, call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* ptid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT;
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "WSLT2006-1");
    ///
    ///     open_deal_ex(ptid, pCmo);
    ///
    ///     MOODYS_STUDENT_LOAN_INFO coll[1] = {0};
    ///     int ret = view_moodys_student_loan_info(ptid, 1, coll, sizeof(MOODYS_STUDENT_LOAN_INFO));
    ///     if(ret < 0)
    ///     {
    ///         // error handling
    ///     }
    ///
    ///     MOODYS_STUDENT_LOAN_INFO *all_coll = new MOODYS_STUDENT_LOAN_INFO[pCmo->num_colls];   
    ///     ret = view_moodys_student_loan_info(ptid, -1, all_coll, sizeof(MOODYS_STUDENT_LOAN_INFO));
    ///     if(ret < 0)
    ///     {
    ///         // error handling
    ///     }
    ///
    ///     delete [] all_coll;
    ///     all_coll = NULL;
    ///     close_deal_ex(ptid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @note If all collateral is requested the arrays all_colls[] must be allocated to be at least as long as the value CMO_STRUCT.num_colls returned by open_deal().
    long CHASAPI view_moodys_student_loan_info(void *tid, short index, MOODYS_STUDENT_LOAN_INFO all_colls[], short length);

    /// Sets the draw rates for the specified collateral.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// 
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] type The type of draw rate curve. Must be one of ::DRAW_RATE_TYPE.
    /// @param[in] is_vector The length of the vector pointed to by pval or 0 if pval is a constant.
    /// @param[in] pval A pointer to the prepayment speeds (or speed).
    /// @param[in] loan_num The 1-based index of the loan or -1 to apply to all collateral in the deal.
    /// @param[in] set_sup_remic If TRUE this will replace any specific underlying deal settings. Otherwise it will not.
    ///
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Other error
    /// @retval  -3 Error - Invalid loan number
    /// @retval  -4 Error - Deal type not support draw curve
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @note Draw rates are expressed as decimals. e.g. 5.25% would be .0525.
    ///       This method is just for reverse mortgages currently.
    /// @sample
    ///     void* pDeal = NULL;
    ///     //deal has been opened
    ///
    ///     double dRate = 0.08;
    ///     int ret = set_draw_rates(pDeal, DRAW_CURVE_CPR, 0, &dRate, -1, true);
    ///     if(ret < 0)
    ///     {
    ///         //error handling
    ///     }
    ///
    ///     double dRateVec[5]={0.0,0.05,0.05,0.05,0.1};
    ///     ret = set_draw_rates(pDeal, DRAW_CURVE_SMM, 5, dRateVec,1, true);
    ///     if(ret < 0)
    ///     {
    ///         //error handling
    ///     }
    /// @endsample
    long CHASAPI set_draw_rates(void *tid, short type, short is_vector, double *pval, long loan_num, BOOLYAN set_sup_remic);

    /// Returns a pointer to a vector of doubles containing deal level miscellaneous cashflow, the vector size is MAX_PERIODS.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail SFW CDOnet
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] flow_identifier The identifier of cashflow. Must be one of ::FLOW_MISC_INDENTIFIER.
    /// @retval NULL  The deal does not contain this cashflow field.
    /// @retval OTHER Pointer to the vector of cashflows
    ///
    /// @note 
    /// Call set_deal_calc_level() with parameter CALC_LEVEL_FULL after open_deal_ex() but before run_deal_ex().
    /// @sample
    /// 	void*ptid=NULL;
    /// 	CMO_STRUCT cmo;
    /// 	memset(&cmo, 0, sizeof(CMO_STRUCT));
    /// 	strcpy(cmo.dealid,"ACE06NC1");
    ///
    /// 	open_deal_ex(ptid,&cmo);
    /// 	set_deal_calc_level(ptid, CALC_LEVEL_FULL, 1); 
    /// 	run_deal_ex(ptid,&cmo);
    /// 	// Deal is already opened successfully.
    /// 	// Deal is already run successfully.
    ///
    /// 	// Get misc cashflow.
    /// 	double* misc_flow=get_misc_flow(ptid, FLOW_MISC_FEE_TOTAL);
    /// 	if(NULL==misc_flow)
    /// 	{
    /// 	    std::cout<<"The deal does not contain this cashflow field."<<std::endl;
    /// 	}
    ///
    /// 	close_deal_ex(ptid,&cmo);
    /// @endsample
    double * CHASAPI get_misc_flow(void *tid, int flow_identifier);

    /// @since 3.1.0
    /// @avail CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in]  tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in]  bondid The bond name.
    /// @param[out] value Authorized integral amount for specific bondid.
    ///
    /// @retval    0 No error
    /// @retval   -1 Error - Deal not opened
    /// @retval  -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    /// 	void *pDeal = NULL;
    /// 	CMO_STRUCT cmo;
    /// 	memset(&cmo, 0, sizeof(CMO_STRUCT));
    /// 	strcpy(cmo.dealid, "STATICLO");
    ///
    /// 	set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    /// 	open_deal_ex(pDeal, &cmo);
    ///
    /// 	double value;
    /// 	int ret = get_bond_authorized_integral_amount(pDeal, "A", &value);
    /// 	if (0 != ret)
    /// 	{
    ///		    // error handle
    /// 	}
    ///
    /// 	close_deal_ex(pDeal, &cmo);	
    /// @endsample
    ///
    int CHASAPI get_bond_authorized_integral_amount(void * tid,char * bondid, double * value);

    /// Retrieves global currency of deal.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail SFW, CDOnet, CHS
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[out] currency_index a pointer to the global currency index.
    ///
    /// @retval    0 No error
    /// @retval   -1 Error - Deal not opened
    /// @retval  -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    //deal has been opened
    ///
    ///    char global_currency[4];
    ///    int ret = get_global_currency(pDeal, global_currency);
    ///    if (0 != ret)
    ///    {
    ///        // error handle
    ///    }
    /// @endsample
    ///
    int CHASAPI get_global_currency(void *tid, char *currency_index);

    /// Retrieves currencies used in the deal.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail SFW, CDOnet, CHS
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[out] currencies a pointer to the currency index. If it's NULL, the number of currencies used in the deal will be returned. If it's not NULL, the pointers will be updated so that they points to the addresses where store the names of the currencies used in the deal and the number of currencies used will be returned by the method.
    ///
    /// @retval   >=0 Number of currencies used in the deal
    /// @retval   -1  Error - Deal not opened
    /// @retval   -2  Error - Invalid currencies pointer
    /// @retval  -99  Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    // deal is open
    ///
    ///    int curr_num = get_currencies(pDeal, NULL);
    ///    std::vector<char> curr_buf(curr_num*4); 
    ///    std::vector<char*> currencies(curr_num);
    ///    for(int i = 0; i<curr_num; i++ )
    ///    {
    ///        currencies[i] = &curr_buf[i*4];
    ///    }
    ///    int ret = get_currencies(pDeal, &currencies.front()));
    ///    if (ret < 0)
    ///    {
    ///        // error handle
    ///    }
    /// @endsample
    ///
    int CHASAPI get_currencies(void *tid, char* currencies[]);

    /// Set specified currency exchange rate per global currency.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail SFW CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] currency The ISO name of the specified currency.
    /// @param[in] val The exchange rate of the specified currency to the global currency. For example, if the required currency is GBP, the global currency of the deal is USD and pval is 0.6461, then it means that the exchange rate is 0.6461 GBP per USD.
    ///
    /// @retval    0 No error
    /// @retval   -1 Error - Deal not opened
    /// @retval   -2 Error - Invalid currency ISO name
    /// @retval  -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    // deal is open
    ///
    ///    int ret = set_exchange_rate(pDeal, "USD", 1.3247);
    ///    if (0 != ret)
    ///    {
    ///        // error handle
    ///    }
    /// @endsample
    ///
    int CHASAPI set_exchange_rate(void *tid, const char* currency, double val);

    /// Get specified currency exchange rate per global currency.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail SFW CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] currency The specified currency name.
    /// @param[out] pval Exchange rate value pointer.
    ///
    /// @retval    0 No error
    /// @retval   -1 Error - Deal not opened
    /// @retval   -2 Error - Invalid currency name
    /// @retval   -3 Error - Invalid exchange rate value pointer
    /// @retval  -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    // deal is open
    ///
    ///    double rate = 0.0;
    ///    int ret = get_exchange_rate(pDeal, "USD", &rate);
    ///    if (0 != ret)
    ///    {
    ///        // error handle
    ///    }
    /// @endsample
    ///
    int CHASAPI get_exchange_rate(void *tid, const char* currency, double* pval);

    /// Sets the constant or vector interest rate that will be used for the specified currency and index.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail ALL
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] currency The ISO name of the currency of the requested market index.
    /// @param[in] idx A pointer to the index to set. *idx must be one of: 
    ///         - enums of ::INDEX_TYPE (In indextypes.h).
    ///         - enums of ::INDEX_TYPE_EX (SFW and CDOnet deals).
    /// @param[in] vector The length of the vector pointed to by pval, or 0 if pval points to a constant.
    /// @param[in] pval A pointer to the new rate value or values.
    ///
    /// @retval   0 No error.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Invalid currency ISO name.
    /// @retval  -4 Error - Invalid index to market index.
    /// @retval  -5 Error - Invalid rate pointer.
	/// @retval  -6 Error - Currency 'CNY' only support indices from BLR_12 to HPF_60_PLUS, and from XRATE1 to XRATE8.
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    // deal is open
    ///
    ///    short index = LIBOR_3;
    ///    double rate = 0.028;
    ///    int ret = set_index_rate(pDeal, "USD", &index, 0, &rate);
    ///    if (0 != ret)
    ///    {
    ///        // error handle
    ///    }
    /// @endsample
    ///
    /// @note
    ///     - The rates are expressed as a decimal: 5.25% would be .0525.
    ///     - The required rates for a deal can be determined by calling get_required_index_codes().
    ///     - value for current period (0-indexed element) will not be applied.
    ///     - Index rates vector apply from the latest update date closest/relative to the settlement date. A floater bond will use period 1 rate assumption at first reset date since deal update date.
    ///     - Index rates vector is a monthly rate vector, regardless payment frequency of deal.
	///     - For indices from BLR_12 to HPF_60_PLUS, only effect when currency is 'CNY'.
	///     - Currency 'CNY' only support indices from BLR_12 to HPF_60_PLUS, and from XRATE1 to XRATE8.
    ///
    long CHASAPI set_index_rate(void* tid, const char* currency, short *idx, short vector, double * pval);

    /// This method gets the MWSA rates for the specified date yyyymmdd and sets rates to deal. The function looks back as many as 7 calendar days to avoid data availability gaps from weekends and holidays.
    ///
    /// @since 3.0.0
    /// @avail SFW, CHS, CDONET
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] yyyymmdd The specified date (of format YYYYMMDD).
    /// @param[in] load_forward_curves If true, load the forward curves of applicable rate types.
    ///
    /// @retval   >0 Actual date to be loaded.
    /// @retval  -1 Error - Deal not opened or Invalid date input.
    /// @retval  -2 Error - MWSA Rates for yyyymmdd not found.
    /// @retval -99 Error - Other error, call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* ptid = NULL;
    ///     // deal has been opened
    ///
    ///     int ret = load_MWSA_rates(ptid, 20161225, true);
    ///     if(ret < 0)
    ///     {
    ///         // error handling
    ///     }
    /// @endsample
    ///
    int CHASAPI load_MWSA_rates(void* tid, int yyyymmdd, BOOLYAN load_forward_curves);

	/// This method gets the available MA rate shifts scenarios list for the specified date yyyymmdd. The function looks back as many as 7 calendar days to avoid data availability gaps from weekends and holidays.
    ///
    /// @since 3.7.0
    /// @avail SFW, CHS, CDONET
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] yyyymmdd The specified date (of format YYYYMMDD).
    /// @param[in] ma_scenario the scenario type indicating which scenario to be set for MA rate shifts corresponding to get_MA_rate_shifts_scenarios().
    ///
    /// @retval   >0 Actual date to be loaded.
    /// @retval  -1 Error - Deal not opened or Invalid date input.
    /// @retval  -2 Error - MA/MEDC Rates for yyyymmdd not found.
    /// @retval -99 Error - Other error, call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* ptid = NULL;
    ///     // deal has been opened
    ///
    ///     int ret = load_MA_rates(ptid, 20161225, "BL");
    ///     if(ret < 0)
    ///     {
    ///         // error handling
    ///     }
    /// @endsample
    ///
    int CHASAPI load_MA_rates(void* tid, int yyyymmdd, const char * ma_scenario);

    /// Returns number of market rate indices per specified currency used by currently open deal 
    /// (includes bonds, collateral, hedges, accounts, and waterfall script) and populates ps_rates vector to indicate which market rate indices are used.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail ALL
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] currency The specified currency code. It should be the alphabetic code from ISO 4217, e.g. "EUR", "USD". Please refer to [current currency & funds code list](http://www.currency-iso.org/en/home/tables/table-a1.html) for all the available currency codes.
    /// @param[out] ps_rates Returns number of market rate indices used by currently open deal (includes bonds, collateral, hedges, accounts, and waterfall script) and populates ps_rates vector to indicate which market rate indices are used.
    ///
    /// @retval  >=0 return The number of rates used.
    /// @retval  -1  Error - Deal not open
    /// @retval  -2  Error - Invalid currency name
    /// @retval  -3  Error - Invalid ps_rates pointer
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    // deal is open
    ///
    ///    short idx_rates[MAX_INDEX_TYPES] = {0};
    ///    int ret = get_indices(pDeal, "EUR", idx_rates);
    ///    if (ret < 0)
    ///    {
    ///        // error handle
    ///    }
    /// @endsample
    ///
    /// @note For CHS engine, the currency just support the "USD".
    long CHASAPI get_indices(void *tid, const char* currency, short * ps_rates);

    /// Gets the rates for the given index and currency. 
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail ALL
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] currency The specified currency name.
    /// @param[in] idx the rate index type. *idx must be one of: 
    ///         - enums of ::INDEX_TYPE (In indextypes.h).
    ///         - enums of ::INDEX_TYPE_EX (SFW and CDOnet deals).
    ///
    /// @retval  NULL  The requested index rate was not retrieved successfully.
    /// @retval  Other A pointer to an array which stores the rates used for the index rate,the array length is MAX_PERIODS(=612).
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    // deal is open
    ///
    ///    short index = LIBOR_3;
    ///    double* pRate = get_index_rate(pDeal, "USD", &index)
    ///    if (NULL == pRate)
    ///    {
    ///        // error handle
    ///    }
    /// @endsample
    ///
    /// @note For CHS engine, the currency just support the "USD".
    double* CHASAPI get_index_rate(void *tid, const char *currency, short *idx);

    /// Returns number of market rate indices under the specified currency used by currently open deal (includes bonds, collateral, hedges, accounts, and waterfall script) 
    /// and populates rate_codes array with the list of index rate codes that are used.
    /// 
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail ALL
    ///
    /// @pre open_deal_ex() was called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] currency The specified currency name.
    /// @param[out] rate_codes The list of index rates codes used in the deal. 
    /// @param[in] size_of_array_codes The size of the user allocated array rate_codes.
    /// 
    /// @retval  >=0 No error
    /// @retval -1 Error - Deal not opened 
    /// @retval -2 Error - Invalid ISO currency name 
    /// @retval -3 Error - Invalid rate_codes pointer 
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    /// 
    /// @sample
    ///    void* ptid=NULL;
    ///    // Deal is open
    ///
    ///    int required_rates[MAX_INDEX_TYPES_EX]={0};
    ///    //the codes of index rates will return in 'required_rates'.
    ///    int actual_count = get_required_index_codes(ptid, "EUR", required_rates, MAX_INDEX_TYPES_EX);
    ///    if (actual_count < 0)
    ///    {
    ///        // error handle
    ///    }
    /// @endsample
    /// 
    /// @note For CHS engine, the currency just support the "USD".
    int CHASAPI get_required_index_codes(void *tid, const char *currency, int *rate_codes, int size_of_array_codes);

    /// Sets the handling of missing exchange rates.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail ALL
    ///
    /// @pre None.
    ///
    /// @param[in] handling The type of handle missing exchange rates, refer to enum MISSING_EXCHANGE_RATES_HANDLING.
    ///
    /// @retval void
    ///
    /// @sample
    ///     set_missing_exchange_rates_handling(MISSING_EXCHANGE_RATES_TREAT_AS_ERROR);
    /// @endsample
    ///
    void CHASAPI set_missing_exchange_rates_handling(MISSING_EXCHANGE_RATES_HANDLING handling);

    /// Sets the timing when default is calculated. If input parameter def_bef_amort is TRUE, default is calculated based on the outstanding balance before amortization. 
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] def_bef_amort If input parameter def_bef_amort is TRUE, default is calculated based on the outstanding balance before amortization.
    /// @param[in] set_sup_remic If TRUE, this setting will be applied to underlying deals; otherwise, it will not. 
    ///
    /// @retval  0 No error
    /// @retval -1 Error - Deal not opened  
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    // deal is open
    ///
    ///    set_default_before_amortization(pDeal, TRUE, TRUE);
    /// @endsample
    ///
    int CHASAPI set_default_before_amortization(void *tid, BOOLYAN def_bef_amort, BOOLYAN set_sup_remic);

    /// Overrides the buy price assumption for current deal.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] override_type Type to indicate what buy price assumptions to use, refer to enum ::BUY_PRICE_OVERRIDE_TYPE.
    /// @param[in] price A pointer to the vector of price. Need to input when the override_type equal to ::BUY_PRICE_OVERRIDE_INPUT.
    /// @param[in] size  Size of price vector, or 0 if pval points to a constant. Need to input when the override_type equal to ::BUY_PRICE_OVERRIDE_INPUT.
    ///
    /// @retval  0  No error
    /// @retval -1  Error - Deal not opened 
    /// @retval -2  Error - Invalid pointer of price
    /// @retval -3  Error - Invalid size
    /// @retval -4  Error - Invalid override type
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    // deal is open
    ///
    ///    double price[] = {100.0, 98.5, 102.5};
    ///    set_buy_price_override(pDeal, BUY_PRICE_OVERRIDE_INPUT, price, 3);
    /// @endsample
    ///
    int CHASAPI set_buy_price_override(void *tid, short override_type, double *price, int size);

    /// Overrides the call date assumption for current deal.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] override_type Type to indicate what call date assumptions to use, refer to enum ::CALL_DATE_OVERRIDE_TYPE.
    /// @param[in] override_date A pointer to the user input date. Format "YYYYMM" or "YYYYMMDD". Need to input when the override_type equal to ::CALL_DATE_OVERRIDE_INPUT.
    ///
    /// @retval  0  No error
    /// @retval -1  Error - Deal not opened 
    /// @retval -2  Error - Invalid value or pointer of date
    /// @retval -3  Error - Invalid override type
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    //deal has been opened
    ///    
    ///    set_call_date_override(pDeal, CALL_DATE_OVERRIDE_INPUT, "20160815");
    /// @endsample
    ///
    int CHASAPI set_call_date_override(void *tid, short override_type, char *override_date);
    
    /// Overrides the call price assumption for current deal.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] override_type Type to indicate what call price assumptions to use, refer to enum ::CALL_PRICE_OVERRIDE_TYPE.
    /// @param[in] price A pointer to the vector of price. Need to input when the override_type equal to ::CALL_PRICE_OVERRIDE_INPUT.
    /// @param[in] size Size of price vector, or 0 if pval points to a constant. Need to input when the override_type equal to ::CALL_PRICE_OVERRIDE_INPUT.
    ///
    /// @retval  0  No error
    /// @retval -1  Error - Deal not opened 
    /// @retval -2  Error - Invalid pointer of price
    /// @retval -3  Error - Invalid size
    /// @retval -4  Error - Invalid override type
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    // deal is open
    ///
    ///    double price[] = {100.0, 98.5, 102.5};
    ///    set_call_price_override(pDeal, CALL_PRICE_OVERRIDE_INPUT, price, 3);
    /// @endsample
    ///
    int CHASAPI set_call_price_override(void *tid, short override_type, double *price, int size);

    /// Sets the reinvestment type that is used for reinvestment. 
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reinv_type Type to indicate which reinvestment settings to use, refer to enum ::REINV_TYPE.
    ///
    /// @retval  0  No error
    /// @retval -1  Error - Deal not opened 
    /// @retval -2  Error - Invalid reinvestment type
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg() 
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    // deal is open
    ///
    ///    set_reinvestment_type(pDeal, GLOBAL_REINV);
    /// @endsample
    ///
    int CHASAPI set_reinvestment_type(void *tid, short reinv_type);

    /// Retreives test information for current deal.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail CDOnet
    ///
    /// @pre run_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] test_size Size of test_info vector.
    /// @param[out] test_info A pointer to the vector of test_info.
    ///
    /// @retval >=0 Number of tests that have been returned
    /// @retval -1  Error - Deal not opened 
    /// @retval -2  Error - Invalid input for test_size
    /// @retval -3  Error - Invalid pointer for test_info
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    // deal has been opened and run
    ///
    ///    CDO_TEST_INFO testinfos[10]={0};
    ///    int num_test = 10;
    ///    int ret = get_cdo_test_info(pDeal, &num_test, &testinfos[0]);
    /// @endsample
    ///
    int CHASAPI get_cdo_test_info(void *tid, short *test_size, CDO_TEST_INFO *test_info);

    /// Retreives test projection for current deal.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail CDOnet
    ///
    /// @pre run_deal_ex() has been called.
    ///
    /// @param[in]  tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in]  test_type Test type.
    /// @param[in]  test_name The name the test.
    /// @param[out] flow_test The test projection result.
    ///
    /// @retval  0  Success.
    /// @retval -1  Error - Deal not opened and run.
    /// @retval -2  Error - Invalid test type.
    /// @retval -3  Error - Specified test not found.
    /// @retval -3  Error - Invalid pointer for flow_test.
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg() 
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    // deal is open and run
    ///
    ///    CDO_TEST_FLOW testflow;
    ///    int ret = get_cdo_test_flow(pDeal, TEST_IC, "A1", &testflow);
    /// @endsample
    ///
    int CHASAPI get_cdo_test_flow(void *tid, TEST_TYPE test_type, const char *test_name, CDO_TEST_FLOW *flow_test);

    /// Retreives haircut projection for current deal.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail CDOnet
    ///
    /// @pre run_deal_ex() has been called.
    ///
    /// @param[in]  tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in]  haircut_type Haircut type.
    ///
    /// @retval  NULL  Invalide haircut type.
    /// @retval Other  A pointer to the specified haircut vector, the length of vector is MAX_PERIODS
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    // deal has been opened and run
    ///
    ///    double *ret = get_haircut_flow(pDeal, HAIRCUT_1);
    /// @endsample
    ///
    double * CHASAPI get_haircut_flow(void * tid, CDO_HAIRCUT_TYPE haircut_type);

    /// Retrieves date information of current deal.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null NULL for parent deal or name of the underlying deal.
    /// @param[out] date_info A pointer to a structure CDO_DATE_INFO which contains the date information of a CDOnet deal.
    ///
    /// @retval  0  Success.
    /// @retval -1  Error - Deal not opened.
    /// @retval -2  Error - Invalid pointer to structure CDO_DATE_INFO.
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    // deal is open
    ///
    ///    CDO_DATE_INFO date_info;
    ///    int ret = get_cdo_date_info(pDeal, NULL, &date_info);
    /// @endsample
    ///
    int CHASAPI get_cdo_date_info(void *tid, const char *reremic_deal_id_or_null, CDO_DATE_INFO *date_info);

    /// Sets the global reinvestment setting.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reinv_info Global reinvestment info, refer to GLOBAL_REINVESTMENT_INFO.
    /// @param[in] pool_size Number of assets for global reinvestment.
    /// @param[in] pool_info A pointer to the vector of assets for global reinvestment, refer to GLOBAL_REINVESTMENT_ASSET_INFO.
    ///
    /// @retval  0  No error
    /// @retval -1  Error - Deal not opened 
    /// @retval -2  Error - Invalid input for pool_size
    /// @retval -3  Error - Invalid pointer for pool_info
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    // deal is open
    ///
	///    set_reinvestment_type(pDeal, GLOBAL_REINV);
    ///
    ///    GLOBAL_REINVESTMENT_INFO reinv_info;
    ///    //set values of reinv_info members
    ///    const int nAssets = 5;
    ///    std::vector<GLOBAL_REINVESTMENT_ASSET_INFO> assetInfos(nAssets);
    ///    //set values of each asset members in vecter assetInfos
    ///
    ///    set_global_reinvestment(pDeal, reinv_info, nAssets, &assetInfos.front());
    /// @endsample
    /// @note The global reinvestment assets and settings only effect when reinvestment type is set to GLOBAL_REINV by calling set_reinvestment_type.
    int CHASAPI set_global_reinvestment(void * tid, GLOBAL_REINVESTMENT_INFO reinv_info, short pool_size, const GLOBAL_REINVESTMENT_ASSET_INFO *pool_info);

    /// Gets the global reinvestment setting information.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[out] reinv_info A pointer to the global reinvestment info, refer to GLOBAL_REINVESTMENT_INFO.
    /// @param[in] pool_size The length of the vector of global reinvestment portfolio.
    /// @param[out] pool_info A pointer to a vector containing the global reinvestment portfolio, refer to GLOBAL_REINVESTMENT_ASSET_INFO.
    ///
    /// @retval >=  Actual number of global reinv assets 
    /// @retval -1  Error - Deal not opened 
    /// @retval -2  Error - Invalid input for pool_size
    /// @retval -3  Error - Invalid pointer for pool_info
    /// @retval -4  Error - invalid pointer for reinv_info
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg() 
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    // deal is open
    ///
    ///    GLOBAL_REINVESTMENT_INFO reinv_info;
    ///    const int nAssets = 5;
    ///    std::vector<GLOBAL_REINVESTMENT_ASSET_INFO> vecAssets(nAssets);
    ///    int actual_num = get_global_reinvestment(pDeal, reinv_info, nAssets, &vecAssets.front());
    ///    if (actual_num > nAssets)
    ///    {
    ///        vecAssets.clear(); 
    ///        vecAssets.resize(actual_num);
    ///        get_global_reinvestment(pDeal, reinv_info, actual_num, &vecAssets.front());
    ///    }
    /// @endsample
    ///
    int CHASAPI get_global_reinvestment(void * tid, GLOBAL_REINVESTMENT_INFO * reinv_info, short pool_size, GLOBAL_REINVESTMENT_ASSET_INFO pool_info[]);

    /// Sets the reinvestment override value of a pv test for the specified tranche. 
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid Name of the tranche of which the reinvestment override type is requested.
    /// @param[in] override_type The override types, refer to enum ::REINV_OVERRIDE_TYPE.
    ///
    /// @retval  0  No error
    /// @retval -1  Error - Deal not opened 
    /// @retval -2  Error - The specified tranche does not have a PV test
    /// @retval -3  Error - The specified tranche does not exist
    /// @retval -4  Error - Invalid override type
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    // deal is open
    ///    
    ///    set_pv_reinvest_override(pDeal, "A1", REINV_OVERRIDE_REINV_PER);
    /// @endsample
    ///
    int CHASAPI set_pv_reinvest_override(void *tid, const char * bondid, short override_type);

    /// Sets the handling of resec exceptions.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail CDOnet
    ///
    /// @pre None.
    ///
    /// @param[in] handling The type of handle resec exceptions, refer to enum RESEC_EXCEPTIONS_HANDLING.
    ///
    /// @retval void
    ///
    /// @sample
    ///     set_resec_exceptions_handling(RESEC_EXCEPTIONS_HANDLING_TREAT_AS_NONRESEC);
    /// @endsample
    ///
    void CHASAPI set_resec_exceptions_handling(RESEC_EXCEPTIONS_HANDLING handling);

    /// This function will set location of support data for PA analysis explicitly, or default location will be used(deal_input_path/PA/data/).
    /// Note that Support data DB file has been discontinued since PA version 1.9.45 and the data has been embedded in the PA API libraries.
    ///
    /// Since 2.9.6, this function can also set location of PA(Portfolio Analyzer) API library for PA analysis explicitly,
    /// or default location of PA(Portfolio Analyzer) API library will be same as the application folder for wsa.dll.
    ///
    /// @since 2.0.0
    /// @avail CHS, SFW
    ///
    /// @pre NULL.
    ///
    /// @param[in] path Pointing to the location(folder) of "SupportData.db".
    ///
    /// @retval     0 Success.
    /// @retval   -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @warning  This function must be called prior to call set_moodys_credit_model_settings() function to set pa model.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "AMEXCAMT");
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///
    ///     assert(0 == set_pa_data_path("./padata/"));
    ///
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_PA_SETTINGS, false));
    ///     
    ///     assert(0 == set_pa_analysis_type(pDeal, PA_MEDC_SINGLE_PATH));
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI set_pa_data_path(const char* path);


    /// set the default pool data and other settings for PA model.
    ///
    /// @since 2.0.0
    /// @avail CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to PA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] paraName The name of the parameter:
    ///            - "WALoanAge"
    ///            - "WARemainingTerm"
    ///            - "WAFICO"
    ///            - "WACoupon"
    ///            - "WAPrepayPenaltyTerm"
    ///            - "AverageOriginalLoanAmount"
    ///            - "WAOriginalLTV"
    ///            - "PurposePurchase"
    ///            - "PurposeRefi"
    ///            - "OccupancyOwner"
    ///            - "OccupancySecondHome"
    ///            - "OccupancyInvestor"
    ///            - "Property1Unit"
    ///            - "Property24Unit"
    ///            - "OriginatorThirdParty"
    ///            - "OriginatorRetail"
    ///            - "HARP1"
    ///            - "HARP2"
    ///            - "FHA"
    ///            - "WACAtIssuance"
    ///            - "WAFixedRatePeriod"
    ///            - "ArmIndex"
    ///            - "WAResetInterval"
    ///            - "WALifetimeCap"
    ///            - "WALifetimeFloor"
    ///            - "WAPeriodicCap"
    ///            - "WAInitialCap"
    ///            - "WAMargin"
    ///            - "HistoricalPrepaymentPeriod"
    ///            - "HistoricalPrepaymentRate"
    ///            - "Factor"
    ///            - "Rate30"
    ///            - "Rate60"
    ///            - "Rate90"
    ///            - "CPR"
    ///            - "CDR"
    ///            - "Severity"
    ///            - "PrincipalPayment"
    ///            - "SellerPercentage"
    ///            - "Yield"
    ///            - "ChargeOff"
    ///            - "RepaymentRate"
    ///            - "DefermentRate"
    ///            - "ForbearanceRate"
    ///            - "DefermentReduceMonth"
    ///            - "DefermentReduceTo"
    ///            - "ForbearanceReduceMonth"
    ///            - "ForbearanceReduceTo"
    ///            - "GovtPct"
	///            - "MasterTrust"
    ///            - "DefermentReduceDuration"
    ///            - "DefermentReduceMonthsToZero"
    ///            - "ForbearanceReduceDuration"
    ///            - "ForbearanceReduceMonthsToZero"
    /// @param[in] value The value of the parameter.
    ///
    /// @retval     0 Success.
    /// @retval     -1 Deal not opened.
    /// @retval     -2 Parameter is NULL.
    /// @retval     -3 PA model is not setup.
    /// @retval     -4 Setting is not supported.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "AMEXCAMT");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_PA_SETTINGS, false));
    ///
    ///     assert(0 == set_pa_default_pool_data(pDeal,"WAFICO","600.0");
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_pa_default_pool_data(void* tid,const char* paraName, const char* value);

    /// get the default pool data and other settings set by calling setPAModelDefaultSettings.
    ///
    /// @since 2.0.0
    /// @avail CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to PA with API set_moodys_credit_model_settings().
    /// @pre set_pa_default_pool_data has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] paraName The name of the parameter:
    ///            - "WALoanAge"
    ///            - "WARemainingTerm"
    ///            - "WAFICO"
    ///            - "WACoupon"
    ///            - "WAPrepayPenaltyTerm"
    ///            - "AverageOriginalLoanAmount"
    ///            - "WAOriginalLTV"
    ///            - "PurposePurchase"
    ///            - "PurposeRefi"
    ///            - "OccupancyOwner"
    ///            - "OccupancySecondHome"
    ///            - "OccupancyInvestor"
    ///            - "Property1Unit"
    ///            - "Property24Unit"
    ///            - "OriginatorThirdParty"
    ///            - "OriginatorRetail"
    ///            - "HARP1"
    ///            - "HARP2"
    ///            - "FHA"
    ///            - "WACAtIssuance"
    ///            - "WAFixedRatePeriod"
    ///            - "ArmIndex"
    ///            - "WAResetInterval"
    ///            - "WALifetimeCap"
    ///            - "WALifetimeFloor"
    ///            - "WAPeriodicCap"
    ///            - "WAInitialCap"
    ///            - "WAMargin"
    ///            - "HistoricalPrepaymentPeriod"
    ///            - "HistoricalPrepaymentRate"
    ///            - "Factor"
    ///            - "Rate30"
    ///            - "Rate60"
    ///            - "Rate90"
    ///            - "CPR"
    ///            - "CDR"
    ///            - "Severity"
    ///            - "PrincipalPayment"
    ///            - "SellerPercentage"
    ///            - "Yield"
    ///            - "ChargeOff"
    ///            - "RepaymentRate"
    ///            - "DefermentRate"
    ///            - "ForbearanceRate"
    ///            - "DefermentReduceMonth"
    ///            - "DefermentReduceTo"
    ///            - "ForbearanceReduceMonth"
    ///            - "ForbearanceReduceTo"
    ///            - "GovtPct"
	///            - "MasterTrust"
    ///            - "DefermentReduceDuration"
    ///            - "DefermentReduceMonthsToZero"
    ///            - "ForbearanceReduceDuration"
    ///            - "ForbearanceReduceMonthsToZero"
    /// @param[in] value The value of the parameter.
    /// @param[in] len The buffer length of value. If the buffer length is not enough, needed length will be returned through this parameter.
    ///
    /// @retval     0 Success.
    /// @retval    -1 Deal not open.
    /// @retval    -2 paraName is invalid.
    /// @retval    -3 PA model is not setup.
    /// @retval    -4 Setting is not found.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "AMEXCAMT");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_PA_SETTINGS, false));
    ///     assert(0 == set_pa_default_pool_data(pDeal,"WAFICO","600.0");
    ///
    ///     int buflen = 50;
    ///     char value[buflen]={0};
    ///     assert(0 == get_pa_default_pool_data(pDeal,"WAFICO",value,buflen);
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI get_pa_default_pool_data(void* tid,const char* paraName, char* value, int &len);

    /// Replace pool data of PA model.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to PA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] poolID The 1-based index of the pool or -1 to apply to all pool in the deal.
    /// @param[in] paraName The name of the parameter user want to replace:
    ///            - "WALoanAge"
    ///            - "WARemainingTerm"
    ///            - "WAFICO"
    ///            - "WACoupon"
    ///            - "WAPrepayPenaltyTerm"
    ///            - "AverageOriginalLoanAmount"
    ///            - "WAOriginalLTV"
    ///            - "PurposePurchase"
    ///            - "PurposeRefi"
    ///            - "OccupancyOwner"
    ///            - "OccupancySecondHome"
    ///            - "OccupancyInvestor"
    ///            - "Property1Unit"
    ///            - "Property24Unit"
    ///            - "OriginatorThirdParty"
    ///            - "OriginatorRetail"
    ///            - "HARP1"
    ///            - "HARP2"
    ///            - "FHA"
    ///            - "WACAtIssuance"
    ///            - "WAFixedRatePeriod"
    ///            - "ArmIndex"
    ///            - "WAResetInterval"
    ///            - "WALifetimeCap"
    ///            - "WALifetimeFloor"
    ///            - "WAPeriodicCap"
    ///            - "WAInitialCap"
    ///            - "WAMargin"
    ///            - "HistoricalPrepaymentPeriod"
    ///            - "HistoricalPrepaymentRate"
    ///            - "Factor"
    ///            - "Rate30"
    ///            - "Rate60"
    ///            - "Rate90"
    ///            - "CPR"
    ///            - "CDR"
    ///            - "Severity"
    ///            - "PrincipalPayment"
    ///            - "SellerPercentage"
    ///            - "Yield"
    ///            - "ChargeOff"
    ///            - "RepaymentRate"
    ///            - "DefermentRate"
    ///            - "ForbearanceRate"
    ///            - "DefermentReduceMonth"
    ///            - "DefermentReduceTo"
    ///            - "ForbearanceReduceMonth"
    ///            - "ForbearanceReduceTo"
    ///            - "GovtPct"
	///            - "MasterTrust"
    ///            - "DefermentReduceDuration"
    ///            - "DefermentReduceMonthsToZero"
    ///            - "ForbearanceReduceDuration"
    ///            - "ForbearanceReduceMonthsToZero"
    /// @param[in] value The value of the parameter user want to replace.
    ///
    /// @retval     0 Success.
    /// @retval    -1 Deal not opened.
    /// @retval    -2 Parameter is NULL.
    /// @retval    -3 Current mode is not PA mode
    /// @retval    -4 Parameter is not supported.
    ///
	/// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "AMEXCAMT");   
    /// 
    ///     open_deal_ex(tid, pCmo);
    ///     set_moodys_credit_model_settings(tid, MOODYS_PA_SETTINGS, false);
    ///
    ///     replace_pa_pool_data(tid,1,"PurposePurchase","0");
    /// 
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;  
    /// @endsample
    ///
    int CHASAPI replace_pa_pool_data(void* tid,int poolID, const char* paraName, const char* value);

    /// This function will get PA model available scenarios.
    ///
    /// @since 2.0.0
    /// @avail CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to PA with API set_moodys_credit_model_settings().
    ///
    /// @param[in]  tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in]  scenario_list The list of scenarios. If this parameter is NULL, this function will return with number of available scenarios.
    ///             The memory of scenario_list should be allocated by user. The lengh of each element of scenario_list should be 100.
    ///
    /// @retval >=0 Success. Number of available scenarios.
    /// @retval  -1 Deal not opened.
    /// @retval  -2 Error - Invalid pointer.
    /// @retval  -3 Error - PA model is not setup.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "AMEXCAMT");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_PA_SETTINGS, false));
    ///
    ///     int scenario_count = get_pa_scenarios(pDeal, NULL);
    ///     assert(scenario_count > 0);
    ///     
    ///     std::vector<char *> scenario_list(scenario_count);
    ///     std::vector<char> scenario_strs(scenario_count*100);
    ///     for (int i = 0; i < scenario_count; ++i)
    ///         scenario_list[i] = &scenario_strs[i*100];
    ///
    ///     assert(scenario_count == get_pa_scenarios(pDeal, &scenario_list[0]));
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;  
    /// @endsample
    int CHASAPI get_pa_scenarios(void* tid, char *scenario_list[]);

    /// This function will set a credit scenario for current PA module. idx will be index of array scenarios obtains from function get_pa_scenarios().
    ///
    /// @since 2.0.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to PA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] idx An index indicating which scenario to be set for PA.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid idx.
    /// @retval  -3 Error - PA model is not setup.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @see
    /// - get_pa_scenarios()
    /// - get_current_pa_scenario()
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "AMEXCAMT");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_PA_SETTINGS, false));
    ///
    ///     assert(0 == set_current_pa_scenario(pDeal, 1));
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;  
    /// @endsample
    int CHASAPI set_current_pa_scenario(void *tid, int idx);

    /// This function will return the index of the PA credit scenario that is using under current opened deal.
    ///
    /// @since 2.0.0
    /// @avail CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to PA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    ///
    /// @retval  >0 Index of scenario which applying current deal .
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -3 Error - PA model is not setup.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @see
    /// - get_pa_scenarios()
    /// - set_current_pa_scenario()
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "AMEXCAMT");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_PA_SETTINGS, false));
    ///     assert(0 == set_current_pa_scenario(pDeal, 1));
    ///
    ///     int cur_scen = get_current_pa_scenario(pDeal);
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;  
    /// @endsample
    int CHASAPI get_current_pa_scenario(void *tid);

    /// Set the analysis type for PA model.
    ///
    /// @since 2.0.0
    /// @avail CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to PA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] type PA Analysis type.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid PA analysis type.
    /// @retval  -3 Error - PA model is not setup.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "AMEXCAMT");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_PA_SETTINGS, false));
    ///
    ///     assert(0 == set_pa_analysis_type(pDeal, PA_MEDC_SINGLE_PATH));
    ///
    ///     assert(0 == set_current_pa_scenario(pDeal, 1));
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;  
    /// @endsample
    int CHASAPI set_pa_analysis_type(void* tid, PA_ANALYSIS_TYPE type);

    /// This function will set a user defined scenario, which contains customized forecast number for several economic indicators.
    ///
    /// @since 2.0.0
    /// @avail CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to PA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] factor Identifier to set, should be one of factor name.
    /// @param[in] year A pointer to a year array.
    /// @param[in] quarter A pointer to a quarter array.
    /// @param[in] value A pointer to a value array.
    /// @param[in] length The length of year/quarter/value array.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid PA analysis type for customized scenario.
    /// @retval  -3 Error - Current credit model is not PA.
    /// @retval  -4 Error - Invalid year,quarter,value pointer or length.
    /// @retval  -5 Error - Invalid factor, use get_deal_error_msg() to see details.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @par Factor
    ///     | Factor Name | Description |
    ///     | ----------- | ----------- |
    ///     | "UNEMPLOYMENT" | U.S. Unemployment Rate. | 
    ///     | "HPI" | U.S. HPI, House Price Index. | 
    ///     | "GDP" | U.S. HP,Gross Domestic Product. | 
    ///     | "TSY1Y" | Treasury 1 Year. | 
    ///     | "TSY10Y" | Treasury 10 Year. | 
    ///     | "LIBOR6MSPREAD" | 6 Month LIBOR Spread. |
    ///     | "TREASURY3M" | Treasury 3 Month, full economy. | 
    ///     | "TREASURY6M" | Treasury 6 Month, full economy. | 
    ///     | "TREASURY1YR" | Treasury 1 Year, full economy. | 
    ///     | "TREASURY5YR" | Treasury 5 Year, full economy. | 
    ///     | "TREASURY10YR" | Treasury 10 Year, full economy. | 
    ///     | "LIBOR1M" | 1 Month LIBOR, full economy. |     
    ///     | "LIBOR3M" | 3 Month LIBOR, full economy. |  
    ///     | "LIBOR6M" | 6 Month LIBOR, full economy. |  
    ///     | "LIBOR12M" | 12 Month LIBOR, full economy. |  
    ///     | "PRIME" | Prime, full economy. |  
    ///     | "FREDDIEMAC30YR" | Freddie Mac 30 Year, full economy. |  
    ///     | "NRI"|full economy. |
    ///     | "PCI"|full economy. |
    ///     | "FREDDIEMAC15YR"|full economy. |
    ///     | "FREDDIEMAC1YR"|full economy. |
    ///     | "USEDCARRATE"|full economy. |
    ///     | "NEWCARRATE"|full economy. |
    ///     | "OILPRICE"|full economy. |
    ///     | "USEDCARINDEX"|full economy. |
    ///     | "FREDDIEMAC5YR"|full economy. |
	///     | "NEWVEHICLESALES"|full economy since PA V1957. |
    ///     | "CPPIAPT"|full economy since PA V1957. |
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "AMEXCAMT");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_PA_SETTINGS, false));
    ///     assert(0 == set_pa_analysis_type(pDeal, PA_CUST_MEDC_SINGLE_PATH));
    ///
    ///     int year[]={2013,2014,2014,2014,2014,2015,2015,2015,2015,2016,2016,2016,2016,2017,2017,2017,2017,2018,2018,2018};
    ///     int quarter[]={4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3};
    ///     double unemploy[] = {7.89,7.85,7.77,7.56,7.67,7.42,7.39,7.31,7.26,7.29,7.33,7.28,7.25,7.17,7.21,7.19,7.14,7.03,7.01,6.88};
    ///     double hpi[] = {2.34,2.98,3.59,4.12,4.46,4.02,4.13,4.19,4.35,4.67,4.95,4.97,5.23,5.69,6.19,8.43,10.13,13.11,14.01,18.63};
    ///     double gdp[] = {0.99,1.34,1.77,1.98,2.09,2.43,2.53,2.69,3.16,3.87,4.05,5.11,6.19,9.67,13.42,15.55,16.95,18.34,23.39,29.66};
    ///     double try1y[] = {0.23,0.27,0.39,0.46,0.55,0.69,0.79,0.92,1.23,1.34,1.56,1.77,1.95,2.03,2.15,2.34,2.39,2.41,2.68,2.71};
    ///     double try10y[] = {2.25,2.34,2.39,2.44,2.59,2.48,2.41,2.37,2.43,2.57,2.64,2.83,2.96,3.24,3.35,3.81,3.49,3.67,4.22,4.53};
    ///     double libor6m[] = {0.62,0.61,0.63,0.78,0.96,1.26,1.35,1.67,1.69,1.75,1.86,1.92,2.11,2.23,2.42,2.53,2.67,2.92,3.32,3.36};
    ///
    ///     assert(0 == set_pa_custom_scenario(pDeal,"UNEMPLOYMENT",year,quarter,unemploy,20));
    ///     assert(0 == set_pa_custom_scenario(pDeal,"HPI",year,quarter,hpi,20));
    ///     assert(0 == set_pa_custom_scenario(pDeal,"GDP",year,quarter,gdp,20));
    ///     assert(0 == set_pa_custom_scenario(pDeal,"TSY1Y",year,quarter,try1y,20));
    ///     assert(0 == set_pa_custom_scenario(pDeal,"TSY10Y",year,quarter,try10y,20));
    ///     assert(0 == set_pa_custom_scenario(pDeal,"LIBOR6MSPREAD",year,quarter,libor6m,20));
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;  
    /// @endsample
    int CHASAPI set_pa_custom_scenario(void * tid, const char* factor, const int* year, const int* quarter, const double* value, int length);

    /// This function will set a user defined scenario, which contains customized forecast number for several economic indicators.
    ///
    /// @since 2.1.0
    /// @avail SFW,CHS
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to PA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] factor Identifier to set, should be one of factor name.
    /// @param[in] country country identifier, should be one of country identifier.
    /// @param[in] region Reserve for future use, currently input "" or Null.
    /// @param[in] year A pointer to a year array.
    /// @param[in] quarter A pointer to a quarter array.
    /// @param[in] value A pointer to a value array.
    /// @param[in] length The length of year/quarter/value array.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid PA analysis type for customized scenario.
    /// @retval  -3 Error - Current credit model is not PA.
    /// @retval  -4 Error - Invalid year,quarter,value pointer or length.
    /// @retval  -5 Error - Invalid factor, use get_deal_error_msg() to see details.
    /// @retval  -6 Error - Invalid country.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @par Factor
    ///     | Factor Name | Description |
    ///     | ----------- | ----------- |
    ///     | "UNEMPLOYMENT" | Unemployment Rate. | 
    ///     | "HPI" | HPI, House Price Index. | 
    ///     | "GDP" | HP,Gross Domestic Product. | 
    ///     | "TSY10Y" | Treasury 10 Year. | 
    ///     | "TREASURY3M" | Treasury 3 Month, full economy. | 
    ///     | "TREASURY6M" | Treasury 6 Month, full economy. | 
    ///     | "TREASURY1YR" | Treasury 1 Year, full economy. | 
    ///     | "TREASURY5YR" | Treasury 5 Year, full economy. | 
    ///     | "TREASURY10YR" | Treasury 10 Year, full economy. | 
    ///     | "LIBOR1M" | 1 Month LIBOR, full economy. |     
    ///     | "LIBOR3M" | 3 Month LIBOR, full economy. |  
    ///     | "LIBOR6M" | 6 Month LIBOR, full economy. |  
    ///     | "LIBOR12M" | 12 Month LIBOR, full economy. |  
    ///     | "PRIME" | Prime, full economy. |  
    ///     | "FREDDIEMAC30YR" | Freddie Mac 30 Year, full economy. |  
    ///     | "NRI"|full economy. |
    ///     | "PCI"|full economy. |
    ///     | "FREDDIEMAC15YR"|full economy. |
    ///     | "FREDDIEMAC1YR"|full economy. |
    ///     | "USEDCARRATE"|full economy. |
    ///     | "NEWCARRATE"|full economy. |
    ///     | "OILPRICE"|full economy. |
    ///     | "USEDCARINDEX"|full economy. |
    ///     | "FREDDIEMAC5YR"|full economy. |
	///     | "NEWVEHICLESALES"|full economy since PA V1957. |
    ///     | "CPPIAPT"|full economy since PA V1957. |
    ///
    /// @par country
    ///     | Country Identifier | Country |
    ///     | ----------- | ----------- |
    ///     | "US"   | US | 
    ///     | "IAUS" | Australia | 
    ///     | "IDEU" | Germany | 
    ///     | "IESP" | Spain | 
    ///     | "IFRA" | France | 
    ///     | "IIRL" | Ireland | 
    ///     | "INLD" | Netherlands | 
    ///     | "IPRT" | Portugal | 
    ///     | "IITA" | Italy | 
    ///     | "IGBR" | UK | 
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     //deal has been opened and set PA model
    ///
    ///     int year[]={2013,2014,2014,2014,2014,2015,2015,2015,2015,2016,2016,2016,2016,2017,2017,2017,2017,2018,2018,2018};
    ///     int quarter[]={4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3};
    ///     double unemploy[] = {7.89,7.85,7.77,7.56,7.67,7.42,7.39,7.31,7.26,7.29,7.33,7.28,7.25,7.17,7.21,7.19,7.14,7.03,7.01,6.88};
    ///     double hpi[] = {2.34,2.98,3.59,4.12,4.46,4.02,4.13,4.19,4.35,4.67,4.95,4.97,5.23,5.69,6.19,8.43,10.13,13.11,14.01,18.63};
    ///     double gdp[] = {0.99,1.34,1.77,1.98,2.09,2.43,2.53,2.69,3.16,3.87,4.05,5.11,6.19,9.67,13.42,15.55,16.95,18.34,23.39,29.66};
    ///     double try10y[] = {2.25,2.34,2.39,2.44,2.59,2.48,2.41,2.37,2.43,2.57,2.64,2.83,2.96,3.24,3.35,3.81,3.49,3.67,4.22,4.53};
    ///
    ///     assert(0 == set_pa_analysis_type(pDeal, PA_CUST_MEDC_SINGLE_PATH));
    ///     assert(0 == set_pa_custom_scenario_ex(pDeal,"UNEMPLOYMENT","IAUS","",year,quarter,unemploy,20));
    ///     assert(0 == set_pa_custom_scenario_ex(pDeal,"HPI","IAUS","",year,quarter,hpi,20));
    ///     assert(0 == set_pa_custom_scenario_ex(pDeal,"GDP","IFRA","",year,quarter,gdp,20));
    ///     assert(0 == set_pa_custom_scenario_ex(pDeal,"TSY10Y","IITA","",year,quarter,try10y,20));
    ///     assert(0 == set_pa_custom_scenario_ex(pDeal,"TSY10Y","IGBR","",year,quarter,try10y,20));
    /// @endsample
    int CHASAPI set_pa_custom_scenario_ex(void * tid, const char* factor, const char* country, const char* region, const int* year, const int* quarter, const double* value, int length);

	/// This function will set the number of path for PA simulation.
    ///
    /// @since 3.0.0
    /// @avail SFW, CHS
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to PA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] number The number of path for PA simulation, the value should be in [1, 10000]. The default path number is 10000.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Invalid input simulation number.
    /// @retval  -3 Error - Current credit model is not PA.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "AMEXCAMT");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_PA_SETTINGS, false));
    ///     assert(0 == set_pa_analysis_type(pDeal, PA_LOSS_SIMULATION));
    ///
    ///     assert(0 == set_pa_simulation_path_num(pDeal, 5));
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_pa_simulation_path_num(void * tid, int number);

    /// This function will return the PA model economy date(year and quarter).
    ///
    /// @since 2.0.0
    /// @avail All
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[out] year The year of the PA economy date, e.g. 2013.
    /// @param[out] quarter The quarter of the PA economy date, the value of quarter should be an integer in the range of [1, 4].
    ///
    /// @retval   0 Success.
    /// @retval  -2 Error - Invalid year or quarter pointers.
    /// @retval  -3 Error - Current credit model is not PA.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///
    ///     int year = 0, quarter = 0;
    ///     assert(0 == get_pa_economy_date(pDeal, &year, &quarter)); // e.g. year = 2013, quarter = 4
    ///
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_PA_SETTINGS, false));
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI get_pa_economy_date(void *tid, int *year, int *quarter);
    
    /// This method gets pa model generated vectors
    ///
    /// @exptl Subject to change
    /// @since 2.0.1
    /// @avail CHS,SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to PA with API set_moodys_credit_model_settings().
    /// @pre run_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] group_number The group number of collateral.
    /// @param[in] identifier The indentifier of vector type.
    ///
    /// @retval The pointer to the vector
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "AMEXCAMT");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_PA_SETTINGS, false));
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     double* v = get_pa_vector(pDeal,0,PA_VECTOR_CHANGEOFF);
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;  
    /// @endsample
    /// @note Only PA_CPR is supported in CHS.
    double* CHASAPI get_pa_vector(void* tid,int group_number,PA_POOL_VECTOR_TYPE identifier);

    /// This function will set multipliers for PA analysis, including prepay, default and severity. By default, PA runs as all multiplier with value 1.00.
    ///
    /// @since 3.0.0
    /// @avail SFW, CHS
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to PA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] type The type of PA multiplier.
    /// @param[in] is_vector The length of the vector pointed to by pval or 0 if pval is a constant. Currently only constant supported.
    /// @param[in] pval A pointer to the multiplier vector to be set.
    /// @param[in] pool_num Pool number, -1 for all collateral pools. Currently only deal level setting supported.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Invalid PA multiplier or invalid pool number or invalid pval.
    /// @retval  -3 Error - Current credit model is not PA.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "AMEXCAMT");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_PA_SETTINGS, false));
    ///     assert(0 == set_pa_analysis_type(pDeal, PA_MEDC_SINGLE_PATH));
    ///     assert(0 == set_current_pa_scenario(pDeal, 1));
    ///
    ///     // deal level settings
    ///     double ppy_multiplier = 2.0;
    ///     double def_multiplier = 3.0;
    ///     double sev_multiplier = 4.0;
    ///     assert(0 == set_pa_multiplier(pDeal, PA_MULTIPLIER_PREPAY, 0, &ppy_multiplier, -1));
    ///     assert(0 == set_pa_multiplier(pDeal, PA_MULTIPLIER_DEFAULT, 0, &def_multiplier, -1));
    ///     assert(0 == set_pa_multiplier(pDeal, PA_MULTIPLIER_SEVERITY, 0, &sev_multiplier, -1));
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_pa_multiplier(void *tid, PA_MULTIPLIER_TYPE type, short is_vector, double *pval, long pool_num);

	/// Sets the balloon extension assumption used for the specified piece of collateral.
    ///
    /// @exptl Subject to change
    /// @since 2.0.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// 
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
	/// @param[in] reremic_deal_id_or_null If reremic deal, this is the id, otherwise null
    /// @param[in] months A pointer to an array of number of months for each balloon extension.
	/// @param[in] rates A pointer to an array of extension penalty rates in percentage for each balloon extension.
	/// @param[in] length The number of extensions set, up to 3 extensions
    /// @param[in] delay The number of months delayed as the extension penalties are paid.
    /// @param[in] loan_num The 1-based index of the loan or -1 to apply to all collateral in the deal.
    ///
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Invalid pointer
	/// @retval  -3 Error - Invalid loan number
	/// @retval  -5 Error - Invalid value passed
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @note For CMBS only
    ///            
    /// @sample
    ///     void* pDeal = NULL;
    ///     //deal has been opened
    ///
	///     int blMonths[]={10,15,25};
	///     // rates: 5%, 10%, 15%
	///     double blRates[] = {5, 10, 15};
	///     int blLength = 3;
	///     int blDelay = 10;
	///     long loan_num = 94;
	///     int ret = set_balloon_extension_assumptions(pDeal,NULL,blMonths,blRates,blLength,blDelay,loan_num);
    ///     if(ret < 0)
    ///     {
    ///         //error handling
    ///     }
    /// @endsample
	int CHASAPI set_balloon_extension_assumptions(void *tid, const char* reremic_deal_id_or_null, int *months, double *rates, int length, int delay, long loan_num);

	/// Get the balloon extension assumption used for the specified piece of collateral.
    ///
    /// @exptl Subject to change
    /// @since 2.6.5
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// 
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
	/// @param[in] reremic_deal_id_or_null If reremic deal, this is the id, otherwise null
    /// @param[out] months A pointer to an array which stores the months that the user set for each balloon extension period.
	/// @param[out] rates A pointer to an array which stores the rates that the user set for calculating the extension penalties for each balloon extension period
	/// @param[in] length The number of extensions set, up to 3 extensions
    /// @param[out] delay A pointer to an int which sotres the number of months delayed as the extension penalties are paid.
    /// @param[in] loan_num The 1-based index of the loan in the deal.
    ///
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Invalid input length
	/// @retval  -3 Error - Invalid loan number
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @note For CMBS only
    ///            
    /// @sample
	///		void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "CMBS_CCC070C3");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
	///     int pblMonths[3]={0};
	///     double pblRates[3] = {0};
	///     int blLength = 3;
	///     int pblDelay[1] = {0};
	///     long loan_num = 94;
	///     int ret = get_balloon_extension_assumptions(pDeal,NULL, pblMonths,pblRates,blLength,pblDelay,loan_num);
	///     if(ret < 0)
    ///     {
    ///         //error handling
    ///     }
    ///
	///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI get_balloon_extension_assumptions(void *tid, const char* reremic_deal_id_or_null, int *months, double *rates, int length, int* delay, long loan_num); 

    /// Sets the call option type for the deal. It is an extension of clean_up_call_ex(). The setting will apply to all underlying deals if set_sup_remic is TRUE.
    /// By default, deal will run into maturity instead of call.
    ///
    /// @exptl Subject to change
    /// @since 2.1.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] type Type of the call option that the user want to run, should be enums of ::CALL_OPTION_TYPE.
    /// @param[in] set_sup_remic The clean-up call setting will apply to all underlying deals if set_up_remic is TRUE.
    ///            It will NOT apply to underlying deals if it is FALSE.
    ///
    /// @retval  0 No error
    /// @retval -1 Error - Deal not opened
    /// @retval -2 Error - Invalid call option
    /// @retval -99 Error - Invalid dso identifier (tid) and other errors
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "PARAGONM11");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     short callType = RUN_TO_MATURITY;
    ///     bool isSetUpRemic = false;
    ///     assert(0, set_call_option(pDeal, callType, isSetUpRemic));
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @see
    /// - clean_up_call_ex()
    long CHASAPI set_call_option(void *tid, short type, BOOLYAN set_sup_remic);

    /// This method returns the status of the "Custom Call" of a deal.
    ///
    /// @since 2.1.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[out] status TRUE/FALSE indicate custom call checkbox value of a deal.
    ///
    /// @retval   0 No error.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid pointer to status.
    /// @retval -99 Error - Other error, call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///    void* tid = NULL;
    ///    CMO_STRUCT *pCmo = new CMO_STRUCT;
    ///    memset(pCmo, 0, sizeof(*pCmo));
    ///    strcpy(pCmo->dealid, "PARAGONM11");
    ///
    ///    set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///    assert(0 == open_deal_ex(tid, pCmo));
    ///
    ///    BOOLYAN isCustomCall = false;
    ///    assert(0 == get_custom_call_status(tid, NULL, &isCustomCall));
    ///
    ///    assert(0 == run_deal_ex(tid, pCmo));
    ///    assert(0 == close_deal_ex(tid, pCmo));
    ///    delete pCmo;
    ///    pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI get_custom_call_status(void *tid, const char *reremic_deal_id_or_null, BOOLYAN *status);

    /// This method returns the optional redemption date of a deal.
    ///
    /// @since 2.1.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[out] date A pointer to a null-terminated string (of format MM/DD/YY). This parameter must be pre-allocated with at least 11 characters.
    ///
    /// @retval   0 The issued date of the requested pool group has been obtained successfully.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid pointer to date.
    /// @retval -99 Error - Other error, call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///    void* tid = NULL;
    ///    CMO_STRUCT *pCmo = new CMO_STRUCT;
    ///    memset(pCmo, 0, sizeof(*pCmo));
    ///    strcpy(pCmo->dealid, "PARAGONM11");
    ///
    ///    set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///    assert(0 == open_deal_ex(tid, pCmo));
    ///
    ///    char optionalRedemptionDate[11] =  {0};
    ///    assert(0 == get_optional_redemption_date(tid, NULL, optionalRedemptionDate));
    ///
    ///    assert(0 == run_deal_ex(tid, pCmo));
    ///    assert(0 == close_deal_ex(tid, pCmo));
    ///    delete pCmo;
    ///    pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI get_optional_redemption_date(void *tid, const char *reremic_deal_id_or_null, char *date);

    /// This method returns the coupon step-up date of a deal.
    ///
    /// @since 2.1.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[out] date A pointer to a null-terminated string (of format MM/DD/YY). This parameter must be pre-allocated with at least 11 characters.
    ///
    /// @retval   0 The issued date of the requested pool group has been obtained successfully.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid pointer to date.
    /// @retval -99 Error - Other error, call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///    void* tid = NULL;
    ///    CMO_STRUCT *pCmo = new CMO_STRUCT;
    ///    memset(pCmo, 0, sizeof(*pCmo));
    ///    strcpy(pCmo->dealid, "PARAGONM11");
    ///
    ///    set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///    assert(0 == open_deal_ex(tid, pCmo));
    ///
    ///    char couponStepupDate[11] =  {0};
    ///    assert(0 == get_coupon_stepup_date(pDeal, NULL, couponStepupDate));
    ///
    ///    assert(0 == run_deal_ex(tid, pCmo));
    ///    assert(0 == close_deal_ex(tid, pCmo));
    ///    delete pCmo;
    ///    pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI get_coupon_stepup_date(void *tid, const char *reremic_deal_id_or_null, char *date);

    /// This method returns the refinance dates of a deal.
    ///
    /// @since 2.7.0
    /// @avail CDONET
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[out] refinance_dates_array A pointer to the array of deal reprice/refinance dates (YYYYMMDD).
	/// @param[in] num_dates Fields size of refinance_dates_array.
    ///
	/// @retval >=0 Number of refinance dates.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid refinance_dates_array and num_dates passed.
    /// @retval -99 Error - Other error, call get_deal_error_msg() for detail.
    ///
	/// @note refinance_dates_array required to be null and num_dates required to be zero when the first call  
	///     
	///
    /// @sample
    ///    void* tid = NULL;
    ///    CMO_STRUCT *pCmo = new CMO_STRUCT;
    ///    memset(pCmo, 0, sizeof(*pCmo));
    ///    strcpy(pCmo->dealid, "EURO3");
    ///
    ///    set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
    ///    assert(0 == open_deal_ex(tid, pCmo));
    ///    
	///    int num_dates; 
    ///    num_dates = get_deal_refinance_date(pDeal, NULL, 0);
    ///
	///    int refinance_dates_array[] = new int[num_dates];
	///    assert(num_dates == get_deal_refinance_date(pDeal, refinance_dates_array, num_dates));
    ///
    ///    assert(0 == close_deal_ex(tid, pCmo));
    ///    delete pCmo;
    ///    pCmo = NULL;
    /// @endsample
    ///
	int CHASAPI get_deal_refinance_date(void *tid, int refinance_dates_array[], int num_dates);

    /// This method sets the simulation engine that users want to run.
    ///
    /// @since 2.1.0
    /// @avail CDOnet, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] simulation_type The simulation engine that user want to run should be enums of ::SIMULATION_TYPE.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid simulation type.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_MONTE_CARLO)); 
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_simulation_engine(void *tid, short simulation_type);
 
    /// This method is used to set the Monte Carlo assumptions. 
    ///
    /// @since 2.1.0
    /// @avail CDOnet, SFW
    ///
    /// @pre set_simulation_engine() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] basic_assumption Structure which contains the basic assumption to run the Monte Carlo simulation.
    /// @param[in] def_ppy_rec_assumption Structure which contains other required assumption to run the Monte Carlo simulation.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_MONTE_CARLO));
    ///     
    ///     MONTE_CARLO_ASSUMPTION basic_assumption;
    ///     // assign members of basic_assumption
    ///     MONTE_CARLO_DEF_PPY_REC_ASSUMPTION def_ppy_rec_assumption;
    ///     // assign members of def_ppy_rec_assumption
    ///     assert(0 == set_monte_carlo_assumption(pDeal, &basic_assumption, &def_ppy_rec_assumption)); 
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_monte_carlo_assumption(void *tid, const MONTE_CARLO_ASSUMPTION* basic_assumption, const MONTE_CARLO_DEF_PPY_REC_ASSUMPTION* def_ppy_rec_assumption);

    /// This method is used to run the Monte Carlo simulation. 
    /// After calling this method, users can use methods get_monte_carlo_result(), get_bond_flow_sim() and get_collateral_flow_sim() to get the corresponding simulation results. 
    ///
    /// @since 2.1.0
    /// @avail CDOnet, SFW
    ///
    /// @pre set_simulation_engine() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_MONTE_CARLO));
    ///
    ///     assert(0 == run_monte_carlo_simulation(pDeal)); 
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI run_monte_carlo_simulation(void *tid);

    /// This method returns the monte carlo simulation results.
    ///
    /// @since 2.1.0
    /// @avail CDOnet, SFW
    ///
    /// @pre run_monte_carlo_simulation() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid The name of tranche whose result is being requested.
    /// @param[out] result The result being requested.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 bondid not recognized.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_MONTE_CARLO));
    ///     assert(0 == run_monte_carlo_simulation(pDeal));
    ///
    ///     MONTE_CARLO_RESULT result;
    ///     assert(0 == get_monte_carlo_result(pDeal, "A1", &result);
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI get_monte_carlo_result(void *tid, const char * bondid, MONTE_CARLO_RESULT* result);

    /// This method returns the simulation bond cashflow calculated. The maximum for path number of cash flows populated is 100 by default. 
	/// To remove the limit, please call function remove_simulation_cashflow_populated_limit.
    ///
    /// @since 2.1.0
    /// @avail CDOnet, SFW
    ///
    /// @pre run_monte_carlo_simulation() or run_default_probability_distribution() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] path The path number of the requested bond cashflow.0 for the average bond cashflows.
    /// @param[in] bondid The name of tranche whose bond cashflow is being requested.
    /// @param[in] flow_identifier The bond cashflow identifier being requested.
    ///
    /// @retval OTHER Pointer to the vector of cashflows.
    /// @retval NULL  Error - Call get_deal_error_msg(). 
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_MONTE_CARLO));
    ///     MONTE_CARLO_ASSUMPTION basic_assumption;
    ///     // assign members of basic_assumption
    ///     MONTE_CARLO_DEF_PPY_REC_ASSUMPTION def_ppy_rec_assumption;
    ///     // assign members of def_ppy_rec_assumption
    ///     assert(0 == set_monte_carlo_assumption(pDeal, &basic_assumption, &def_ppy_rec_assumption)); 
    ///     assert(0 == run_monte_carlo_simulation(pDeal));
    ///
    ///     double* balance = get_bond_flow_sim(pDeal, 1, "A1", FLOW_BOND_BALANCE);
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    double* CHASAPI get_bond_flow_sim(void *tid, short path, const char *bondid, int flow_identifier);

    /// This method returns the simulation collateral cashflow calculated. The maximum for path number of cash flows populated is 100 by default. 
	/// To remove the limit, please call function remove_simulation_cashflow_populated_limit.
    ///
    /// @since 2.1.0
    /// @avail CDOnet, SFW
    ///
    /// @pre run_monte_carlo_simulation() or run_default_probability_distribution() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] path The path number of the requested collateral cashflow.0 for the average collateral flow.
    /// @param[in] flow_identifier The collateral cashflow identifier being requested.
    ///
    /// @retval OTHER Pointer to the vector of cashflows.
    /// @retval NULL  Error - Call get_deal_error_msg(). 
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_MONTE_CARLO));
    ///     MONTE_CARLO_ASSUMPTION basic_assumption;
    ///     // assign members of basic_assumption
    ///     MONTE_CARLO_DEF_PPY_REC_ASSUMPTION def_ppy_rec_assumption;
    ///     // assign members of def_ppy_rec_assumption
    ///     assert(0 == set_monte_carlo_assumption(pDeal, &basic_assumption, &def_ppy_rec_assumption)); 
    ///     assert(0 == run_monte_carlo_simulation(pDeal));
    ///
    ///     double* losses = get_collateral_flow_sim(pDeal, 1, FLOW_COLLATERAL_LOSSES);
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    double* CHASAPI get_collateral_flow_sim(void *tid, short path, int flow_identifier);

    /// This method retrieves the list of the issuer names of the portfolio matrix.
    ///
    /// @since 2.1.0
    /// @avail CDOnet
    ///
    /// @pre set_simulation_engine() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[out] issuer_names A pointer to a client-allocated array of character strings in which the issuer names of the portfolio matrix will be stored. 61 characters should be allocated for each string.
    /// @param[in] size The size of the array that the user has passed down.
    ///
    /// @retval >=0 Success. If issuer_names[] is NULL, it will be the number of the issuer names of the portfolio matrix. If issuer_names is not NULL, it will be the number of issuer names that have been populated.
    /// @retval  -1 Deal not opened.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "1776");
    ///
    ///     set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_MONTE_CARLO));
    ///
    ///     int numIssuer = get_monte_carlo_global_issuers(pDeal, NULL, 0);
    ///     if (numIssur > 0)
    ///     {
    ///         std::vector<char> issuer_buf(numIssur*61);
    ///         std::vector<char*> issuers(numIssur);
    ///         for(int i = 0; i<numIssur; ++i)
    ///         {
    ///             issuers[i] = &issuer_buf[i*61];
    ///         }
    ///         get_monte_carlo_global_issuers(pDeal, &issuers.front(), numIssuer);
    ///     }
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI get_monte_carlo_global_issuers(void *tid, char *issuer_names[], short size);

    /// This method sets the correlation between two fields. Depending on the correlation type, the fields can be either issuer names or industry names.
    ///
    /// @since 2.1.0
    /// @avail CDOnet
    ///
    /// @pre set_simulation_engine() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] type Indicates which correlation table the user wants to change, refer to enum ::MONTE_CARLO_CORRELATION_TYPE.
    /// @param[in] field1 Name of field1 whose correlation to field2 will be updated.
    /// @param[in] field2 Name of field2 whose correlation to field1 will be updated.
    /// @param[in] correlation The correlation value between field1 and field2.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid pointer.
    /// @retval  -3 Unrecognized issuer/industry name
    /// @retval -99 Other error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "1776");
    ///
    ///     set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_MONTE_CARLO));
    ///
    ///     assert(0 == set_monte_carlo_correlation(pDeal, MONTE_CARLO_CORRELATION_INDUSTRY, "DJUSST", "DJUSTA", 0.52));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_monte_carlo_correlation(void *tid, MONTE_CARLO_CORRELATION_TYPE type, char *field1, char *field2, double correlation);

    /// This method gets the correlation between two fields. Depending on the correlation type, the fields can be either issuer names or industry names.
    ///
    /// @since 2.1.0
    /// @avail CDOnet
    ///
    /// @pre set_simulation_engine() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] type Indicates which correlation table the user wants to change, refer to enum ::MONTE_CARLO_CORRELATION_TYPE.
    /// @param[in] field1 Name of field1 whose correlation to field2 will be get.
    /// @param[in] field2 Name of field2 whose correlation to field1 will be get.
    /// @param[out] correlation The correlation value between field1 and field2.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid pointer.
    /// @retval  -3 Unrecognized issuer/industry name
    /// @retval -99 Other error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "1776");
    ///
    ///     set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_MONTE_CARLO));
    ///
    ///     double correlation = 0.0;
    ///     assert(0 == get_monte_carlo_correlation(pDeal, MONTE_CARLO_CORRELATION_INDUSTRY, "DJUSST", "DJUSTA", &correlation));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI get_monte_carlo_correlation(void *tid, MONTE_CARLO_CORRELATION_TYPE type, char *field1, char *field2, double* correlation);

    /// This method sets the default time and recovery rate of a loan for the Monte Carlo simulation.
    ///
    /// @since 2.1.0
    /// @avail CDOnet,SFW
    ///
    /// @pre set_simulation_engine() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] num_path Index of the path of the simulation to which the default timing for a specified loan is given.
    /// @param[in] num_loan The loan to be set.
    /// @param[in] default_time -1 means the loan has already defaulted, 1000 means the loan will never default, any other positive integer represents the period in which the loan will default.
    /// @param[in] recovery Recovery rate to be set.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval -99 Other error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_MONTE_CARLO));
    ///
    ///     assert(0 == set_monte_carlo_default_time_and_recovery(pDeal, 1, 0, 3, 0.2));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_monte_carlo_default_time_and_recovery(void *tid, short num_path, short num_loan, short default_time, double recovery);

    /// This function returns the length of available cash flows of a specified bond in a specified run.
    ///
    /// @since 2.1.0
    /// @avail CDOnet, SFW, CHS
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre run_deal_ex() or run_monte_carlo_simulation() or run_default_probability_distribution() has been called.
    ///
    /// @param[in]  tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in]  path The index of the path whose bond cashflows are being requested, 0 for the average bond cashflows or for the bond cashflow of run_deal_ex(). CHS currently only supports the bond cashflow of run_deal_ex(). 
    /// @param[in]  bondid Name of the tranche whose results are being requested. The length should be 20.
    ///
    /// @retval >=0 Success. Return the length of available cash flows of the specified bond in the specified run.
    /// @retval  -1 Deal not opened.
    /// @retval  -2 Error - Bond id not recognized.
    /// @retval  -3 Error - Current simulation engine is not set to any available one.
    /// @retval  -4 Error - Invalid path.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_MONTE_CARLO));
    ///     assert(0 == run_monte_carlo_simulation(pDeal));
    ///
    ///     assert(get_bond_cf_length(pDeal, 1, "A1") > 0);
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI get_bond_cf_length(void *tid, short path, const char *bondid);

    /// This function returns the length of available cash flows of collateral in a specified run.
    ///
    /// @since 2.1.0
    /// @avail CDOnet, SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre run_monte_carlo_simulation() or run_default_probability_distribution() has been called.
    ///
    /// @param[in]  tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in]  path The index of the path whose collateral cashflows are being requested, 0 for the average collateral cashflows.
    ///
    /// @retval >=0 Success. Return the length of available collateral cashflows in the specified run.
    /// @retval  -1 Deal not opened.
    /// @retval  -3 Error - Current simulation engine is not set to any available one.
    /// @retval  -4 Error - Invalid path.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_MONTE_CARLO));
    ///     assert(0 == run_monte_carlo_simulation(pDeal));
    ///
    ///     assert(get_coll_cf_length(pDeal, 1) > 0);
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI get_coll_cf_length(void *tid, short path);

    /// This function will get SEDF model available scenarios.
    ///
    /// @since 2.0.1
    /// @avail CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to SEDF with API set_moodys_credit_model_settings().
    ///
    /// @param[in]  tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in]  scenario_list The list of scenarios. If this parameter is NULL, this function will return with number of available scenarios.
    ///             The memory of scenario_list should be allocated by user. The lengh of each element of scenario_list should be 20.
    ///
    /// @retval >=0 Success. Number of available scenarios.
    /// @retval  -1 Deal not opened.
    /// @retval  -2 Error - Invalid pointer.
    /// @retval  -3 Error - SEDF model is not setup.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     int buflen = 50;
    ///     char value[buflen]={0};
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "1776");
    ///
    ///     set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_SEDF_SETTINGS, false));
    ///
    ///     int scenario_count = get_edf_scenarios(pDeal, NULL);
    ///     assert(scenario_count > 0);
    ///     
    ///     std::vector<char *> scenario_list(scenario_count);
    ///     std::vector<char> scenario_strs(scenario_count*20);
    ///
    ///     for (int i = 0; i < scenario_count; ++i)
    ///         scenario_list[i] = &scenario_strs[i*20];
    ///     assert(scenario_count == get_edf_scenarios(pDeal, &scenario_list[0]));
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI get_edf_scenarios(void* tid, char *scenario_list[]);

    /// This function will set a credit scenario for current SEDF module. idx will be index of array scenarios obtains from function get_edf_scenarios().
    ///
    /// @since 2.0.1
    /// @avail CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to SEDF with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] idx An index indicating which scenario to be set for SEDF.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Invalid idx.
    /// @retval  -3 Error - Current credit model is not SEDF.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "1776");
    ///
    ///     set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_SEDF_SETTINGS, false));
    ///     
    ///     int scenario_count = get_edf_scenarios(pDeal, NULL);
    ///     assert(scenario_count > 0);
    ///     
    ///     assert(0 == set_current_edf_scenario(pDeal, scenario_count-1));
    ///     
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @see
    /// - get_edf_scenarios()
    /// - get_current_edf_scenario()
    int CHASAPI set_current_edf_scenario(void *tid, int idx);

    /// This function will return the index of the SEDF credit scenario that is using under current opened deal.
    ///
    /// @since 2.0.1
    /// @avail CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to SEDF with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    ///
    /// @retval  >0 Index of scenario which applying current deal .
    /// @retval  -1 Error - Deal not open.
    /// @retval  -3 Error - Current credit model is not SEDF.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "1776");
    ///
    ///     set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_SEDF_SETTINGS, false));  
    ///     
    ///     int scenario_count = get_edf_scenarios(pDeal, NULL);
    ///     assert(scenario_count > 0);
    ///     
    ///     assert(0 == set_current_edf_scenario(pDeal, scenario_count-1));
    ///
    ///     assert(scenario_count-1 == get_current_edf_scenario(pDeal));
    ///     
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @see
    /// - get_edf_scenarios()
    /// - set_current_edf_scenario()
    int CHASAPI get_current_edf_scenario(void *tid);

    /// This method retrieves EDF data of a specified loan.
    ///
    /// @since 2.0.1
    /// @avail CDOnet, SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre For CDONet deal, the current credit model should been set to SEDF with API set_moodys_credit_model_settings(), but SFW deal does not need.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[in] loan_num Ordinal collateral number in a deal.
    /// @param[out] pd A pointer to a client-allocated array of double to store EDF data. At least 5 elements should be allocated.
    /// @param[in] length The number of the elements pointed by pd.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid loan_num or tid.
    /// @retval  -3 Current credit model is not Stress EDF.
    /// @retval  -4 Invalid EDF data array.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "1776");
    ///
    ///     set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     double pd[5]={0.0};
    ///     assert(0 == get_loan_edf(pDeal, NULL, 2, pd, 5));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI get_loan_edf(void *tid, const char* reremic_deal_id_or_null, long loan_num, double pd[], int length);

    /// This method sets the default probability data for a specified loan.
    ///
    /// @since 2.0.1
    /// @avail CDOnet, SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre For CDONet deal, the current credit model should been set to SEDF with API set_moodys_credit_model_settings(), but SFW deal does not need.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[in] loan_num Ordinal collateral number in a deal.
    /// @param[in] pd A pointer to an array of double provided by user. At least 5 elements should be provided.
    /// @param[in] length The number of the elements pointed by pd.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid loan_num or tid.
    /// @retval  -3 Current credit model is not Stress EDF.
    /// @retval  -4 Invalid EDF data array.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "1776");
    ///
    ///     set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     double userPd[] = {0.1,0.2,0.3,0.4,0.5};
    ///     assert(0 == set_loan_edf(pDeal, NULL, loan_num, userPd, 5));
    ///
    ///     assert(0 == run_deal_ex(pDeal,pCmo)); // userPd are applied to loan after calling run_deal_ex()
    ///
    ///     double loanEdfValue[5] = {0};
    ///     assert(5 == get_loan_edf(pDeal, NULL, loan_num, loanEdfValue, 5)); // the value of loanEdfValue should be the same as userPd
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    /// @note The loan EDF values would be applied after calling run_deal_ex().
    int CHASAPI set_loan_edf(void *tid, const char* reremic_deal_id_or_null, long loan_num, double* pd, int length);

    /// This method retrieves weighted average default probabilities of a specified reinvestment loan.
    ///
    /// @since 2.0.1
    /// @avail CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to SEDF with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] loan_num Ordinal collateral number in a deal.
    /// @param[out] pd A pointer to a client-allocated array of double to store EDF data. At least 5 elements should be allocated.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid loan_num or tid.
    /// @retval  -3 Current credit model is not Stress EDF.
    /// @retval  -4 Invalid EDF data array.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "1776");
    ///
    ///     set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     double pd[5]={0.0};
    ///     assert(0 == get_reinv_weighted_avg_pd(pDeal, 1, pd));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI get_reinv_weighted_avg_pd(void *tid, long loan_num, double pd[]);

    /// This method retrieves reinvestment recovery rate of a specified reinvestment loan.
    ///
    /// @since 2.0.1
    /// @avail CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to SEDF with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] loan_num Ordinal collateral number in a deal.
    /// @param[out] recovery_rate A pointer to a client-allocated of double to store recovery rate data.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid loan_num or tid.
    /// @retval  -3 Current credit model is not Stress EDF.
    /// @retval  -4 Invalid pointer.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "1776");
    ///
    ///     set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     double rec_rate = 0.0;
    ///     assert(0 == get_reinv_recovery_rate(pDeal, 1, &rec_rate));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI get_reinv_recovery_rate(void * tid, long loan_num, double* recovery_rate);

    /// This method sets default multiplier for SEDF model. By default, SEDF runs as default multiplier with value 1.00.
    ///
    /// @since 2.0.1
    /// @avail CDOnet
    /// 
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to SEDF with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] multiplier multiplier value to be set.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid multiplier.
    /// @retval  -3 Current credit model is not Stress EDF.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "1776");
    ///
    ///     set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == set_edf_default_multiplier(pDeal, 1.0));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_edf_default_multiplier(void *tid, double multiplier);

    /// This method returns a list of high level trigger information(trigger names, trigger descriptions, number of sub-triggers that a trigger has) of an SFW deal.
    ///
    /// @since 2.0.1
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[out] trigger_names A pointer to a client-allocated array of character strings in which the names of the triggers will be stored. 21 characters should be allocated for each string.
    /// @param[out] trigger_descs A pointer to a client-allocated array of character strings in which the descriptions of the triggers will be stored. 1025 characters should be allocated for each string.
    /// @param[out] num_sub_triggers A pointer to a client-allocated array of integer in which the sum of the sub-triggers for each main triggers.
    /// @param[in] size The length of the arrays that the user has passed down.
    ///
    /// @retval   >0 Number of triggers whose information has been returned.
    /// @retval  =0 Success, but deal haven't triggers
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid pointer
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     //deal has been opened
    ///
    ///     char trg_name[5][21] = {0};
    ///     char trg_dec[5][1025] = {0};
    ///     int num_subtriggers[5] = {0};
    ///     std::vector<char*> vec_names(5);
    ///     std::vector<char*> vec_decs(5);
    ///     for(int i = 0; i<5; i++)
    ///     {
    ///         vec_names[i] = trg_name[i];
    ///         vec_decs[i] = trg_dec[i];
    ///     }
    ///     int ret = get_trigger_avail_ex(pDeal, NULL, &vec_names.front(), &vec_decs.front(), num_subtriggers, 5);
    ///     if(ret < 0)
    ///     {
    ///         //Error handle    
    ///     }
    /// @endsample
    /// @note Pass NULL for  trigger_names ,trigger_descs and num_sub_triggers to get just the number of triggers.
    int CHASAPI get_trigger_avail_ex(void *tid, const char *reremic_deal_id_or_null, char *trigger_names[], char *trigger_descs[], int* num_sub_triggers, int size);

    /// This method returns the information(breached, logic, sub-trigger names, sub-trigger descriptions) of the master trigger that the user requested.
    ///
    /// @since 2.0.1
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre run_deal_ex() has been called to get the breached.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[in] trigger_name The case-sensitive name of the trigger whose information are being requested. 
    /// @param[out] breached A pointer to an array of the projection of the status of the requested trigger. The size of the array is MAX_PERIODS.
    /// @param[out] sub_trigger_logic Master trigger's logic, can be either "All Yes" or "Any Yes".21 characters should be allocated for the string.
    /// @param[out] sub_trigger_names A pointer to a client-allocated array of character strings in which the names of the sub triggers of the requested trigger will be stored. 21 characters should be allocated for each string.
    /// @param[out] sub_trigger_descs A pointer to a client-allocated array of character strings in which the descriptions of the sub triggers will be stored. 1025 characters should be allocated for each string.
    /// @param[in] size The length of the arrays that the user has passed down.
    ///
    /// @retval   >0 Number of sub-triggers of the requested master trigger.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid pointer
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     //Deal has been run
    ///
    ///     std::vector<char> name_buf(10*21), desc_buf(10*1025); 
    ///     std::vector<char*> names(10), descs(10);
    ///     for(int i = 0; i<10; i++)
    ///     {
    ///         names[i] = &name_buf[i*21];
    ///         descs[i] = &desc_buf[i*1025];
    ///     }
    ///     signed char status[MAX_PERIODS] = {0};
    ///     char sub_trigger_logic[21] = {0};
    ///     int ret = get_master_trigger_info(pDeal, NULL, "PRO-RATA", status, sub_trigger_logic, &names.front(), &descs.front(), 10);
    ///     if(ret < 0)
    ///     {
    ///         //Error handle
    ///     }
    /// @endsample
    int CHASAPI get_master_trigger_info(void *tid, const char *reremic_deal_id_or_null, const char *trigger_name, SBYTE *breached, char *sub_trigger_logic, char *sub_trigger_names[], char *sub_trigger_descs[], int size);

    /// This method return the details of the current and projected status of a sub-trigger.
    ///
    /// @since 2.0.1
    /// @avail SFW
    ///
    /// @pre run_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[in] sub_trigger_name The case-sensitive name of the sub-trigger whose trigger information are requested. 
    /// @param[out] sub_trigger_type Trigger type of the requested sub-trigger. 1025 characters should be allocated for the string. 
    /// @param[out] sub_trigger_operator Trigger operator of the requested sub-trigger. 10 characters should be allocated for the string.
    /// @param[out] current_level A pointer to a client-allocated array in which the current and projected values of the level of the requested sub-trigger will be stored. The size of the array should be of MAX_PERIODS.
    /// @param[out] threshold A pointer to a client-allocated array in which the current and projected values of the threshold of the requested sub-trigger will be stored. The size of the array should be of MAX_PERIODS.
    /// @param[out] status  	A pointer to a client-allocated array in which the current and projected values of the status of the sub-trigger will be stored. The size of the array should be of MAX_PERIODS.
    /// @param[out] curable This flag indicates if the sub-trigger is curable or not. 
    /// @param[out] override_type The flag indicates if the sub-trigger is overridden or will be overridden. 0 for no override, 1 for always "no" and 2 for always "yes".
    /// @param[out] override_date Override date of the sub-trigger if available. The format is "YYYYMMDD".
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid pointer
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     //Deal has been run
    ///
    ///     char trigger_type[1025] = {0};
    ///     char trigger_operator[10] = {0};
    ///     double threshold[MAX_PERIODS] = {0.};
    ///     signed char status[MAX_PERIODS] = {0};
    ///     double currentlevel[MAX_PERIODS] = {0.};
    ///     BOOLYAN curable = 0;
    ///     signed char override_type = 0;
    ///     int override_date = 0;
    ///     int ret = get_sub_trigger_info(pDeal, NULL, "PRO-RATA-1", trigger_type, trigger_operator, currentlevel, threshold, status, &curable, &override_type, &override_date);
    ///     if(ret != 0)
    ///     {
    ///         //Error handle
    ///     }
    /// @endsample
    int CHASAPI get_sub_trigger_info(void *tid, const char *reremic_deal_id_or_null, const char *sub_trigger_name, char *sub_trigger_type, char *sub_trigger_operator, double *current_level, double *threshold, SBYTE *status, BOOLYAN *curable, SBYTE *override_type, int *override_date);

    /// This method overrides the requested sub-trigger.
    ///
    /// @since 2.0.1
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[in] sub_trigger_name The case-sensitive name of the requested sub-trigger be overridden.
    /// @param[in] override_type  The type of sub-trigger is overridden or will be overridden. 0 for no override, 1 for always "no" and 2 for always "yes".
    /// @param[in] override_date The date from when the sub trigger status will be overridden. the format is "YYYYMMDD". It is would not set when the "override_type" is 0.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Trigger not found.
    /// @retval  -3 Invalid params.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     //Deal has been run
    ///
    ///     int ret = set_trigger_override_ex(pDeal, NULL, "PRO-RATA-1", 1, 20070701);
    ///     if(ret < 0)
    ///     {
    ///         //Error handle
    ///     }
    /// @endsample
    /// @note The trigger would be overridden by after calling run_deal_ex().
    int CHASAPI set_trigger_override_ex(void *tid, const char *reremic_deal_id_or_null, const char *sub_trigger_name, SBYTE override_type, int override_date);

    /// This method sets the underlying level of resec deals.
    ///
    /// @since 2.0.2
    ///
    /// @avail CDOnet
    ///
    /// @pre None.
    ///
    /// @param[in] level The number of underlying levels will open/run.
    ///            - input -1 means will open/run all level underlying deals.
    ///            - input 0 means just open/run top deal.
    ///            - input x>0 means open/run x levels of underlying deals.
    ///
    /// @retval void
    ///
    /// @sample
    ///     set_resec_underlying_level(-1);
    /// @endsample
    /// @note The function need to be called before calling open_deal_ex().
    void CHASAPI set_resec_underlying_level(int level);

    /// This method runs the default probability distribution simulation using preset setting.\n
    /// After calling this method, all the cashflows for the scenarios will be generated and stored in the memory.\n
    /// User can use methods get_dpd_results(), get_bond_flow_sim() and get_collateral_flow_sim() to get the corresponding simulation results.
    ///
    /// @since 2.1.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre set_simulation_engine() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid tid.
    /// @retval  -3 Error - Current simulation engine is not set to "Default Probability Distribution".
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_DEFAULT_PROBABILITY_DISTRIBUTION));
    ///
    ///     DPD_ASSUMPTION dpdAssumption;
    ///     memset(&dpdAssumption, 0, sizeof(dpdAssumption));
    ///     dpdAssumption.distribution = DPD_DISTRIBUTION_LOGNORMAL;
    ///     dpdAssumption.scenario_type = 1;
    ///     dpdAssumption.mean = 10.0;
    ///     dpdAssumption.use_milan_aaa_ce = 0;
    ///     dpdAssumption.milan_aaa_ce = 0.0;
    ///     dpdAssumption.standard_deviation = 1.0;
    ///     dpdAssumption.discounted_recoveries = 0;
    ///     std::fill(dpdAssumption.revolving_default_factors, dpdAssumption.revolving_default_factors+500, 0.01);
    ///     dpdAssumption.num_scenarios = 3;
    ///     dpdAssumption.use_revolving_def_timing = 1;
    ///     dpdAssumption.rating_cap_primary = 3;
    ///     dpdAssumption.rating_cap_surveillance = 5;
    ///     
    ///     assert(0 == set_dpd_assumption(pDeal, &dpdAssumption));
    ///     
    ///     double currentDefaultTimingVector[] = {0.01};
    ///     double revolvingDefaultTimingVector[] = {0.02};
    ///     assert(0 == set_dpd_current_default_timing(pDeal, currentDefaultTimingVector, 1, 0));
    ///     assert(0 == set_dpd_revolving_default_timing(pDeal, revolvingDefaultTimingVector, 1, 0));
    ///     
    ///     assert(0 == set_dpd_el_pd_factors(pDeal, 0.55, 0.45));
    ///     for (int i = 1; i <= 30; ++i)
    ///     {
    ///         assert(0 == set_dpd_threshold(pDeal, "Aaa", i, 0.000005 + 0.00003*i));
    ///         assert(0 == set_dpd_threshold(pDeal, "Aa1", i, 0.00005 + 0.00003*i));
    ///         assert(0 == set_dpd_threshold(pDeal, "Aa2", i, 0.00001 + 0.00003*i));
    ///     }
    ///     
    ///     assert(0 == run_default_probability_distribution(pDeal));
    ///     
    ///     int scenariosNum = get_dpd_scenarios(pDeal, 0, 0);
    ///     assert(scenariosNum > 0);
    ///     DPD_SCENARIO *scenarios = new DPD_SCENARIO[scenariosNum];
    ///     assert(scenariosNum == get_dpd_scenarios(pDeal, scenarios, scenariosNum));
    ///     delete [] scenarios;
    ///     
    ///     DPD_RESULT result;
    ///     assert(0 == get_dpd_results(pDeal, "A1", &result));
    ///     
    ///     const int bondcfIds[] = {FLOW_BOND_BALANCE, FLOW_BOND_INTEREST, FLOW_BOND_PRINCIPAL, FLOW_BOND_INTEREST_DUE, FLOW_BOND_RATE};
    ///     const int collcfIds[] = {
    ///         FLOW_COLLATERAL_BALANCE, FLOW_COLLATERAL_INTEREST, FLOW_COLLATERAL_PRINCIPAL, FLOW_COLLATERAL_SCHED_PRINCIPAL, FLOW_COLLATERAL_PREPAYMENTS,
    ///         FLOW_COLLATERAL_DEFAULTS, FLOW_COLLATERAL_LOSSES, FLOW_COLLATERAL_LIQUIDATIONS, FLOW_COLLATERAL_REINVESTMENT, FLOW_COLLATERAL_INTEREST_OF_BUYS
    ///     };
    ///     
    ///     const double *bondcf[5];
    ///     const double *collcf[10];
    ///     
    ///     // get cf for scenario 2
    ///     for (int i = 0; i < sizeof(collcfIds)/sizeof(collcfIds[0]); ++i)
    ///     {
    ///         collcf[i] = get_collateral_flow_sim(pDeal, 2, collcfIds[i]);
    ///         assert(collcf[i] != NULL);
    ///     }
    ///     for (int i = 0; i < sizeof(bondcfIds)/sizeof(bondcfIds[0]); ++i)
    ///     {
    ///         bondcf[i] = get_bond_flow_sim(pDeal, 2, "A1", bondcfIds[i]);
    ///         assert(bondcf[i] != NULL);
    ///     }
    ///     
    ///     // get average cf
    ///     for (int i = 0; i < sizeof(collcfIds)/sizeof(collcfIds[0]); ++i)
    ///     {
    ///         collcf[i] = get_collateral_flow_sim(pDeal, 0, collcfIds[i]);
    ///         assert(collcf[i] != NULL);
    ///     }
    ///     for (int i = 0; i < sizeof(bondcfIds)/sizeof(bondcfIds[0]); ++i)
    ///     {
    ///         bondcf[i] = get_bond_flow_sim(pDeal, 0, "A2", bondcfIds[i]);
    ///         assert(bondcf[i] != NULL);
    ///     }
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI run_default_probability_distribution(void *tid);

    /// This method sets the general assumption for the default distribution to run.
    ///
    /// @since 2.1.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre set_simulation_engine() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] assumption Structure which stores the assumption for the simulation to run.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid parameter.
    /// @retval  -3 Error - Current simulation engine is not set to "Default Probability Distribution".
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_DEFAULT_PROBABILITY_DISTRIBUTION));
    ///
    ///     DPD_ASSUMPTION dpdAssumption;
    ///     memset(&dpdAssumption, 0, sizeof(dpdAssumption));
    ///     dpdAssumption.distribution = DPD_DISTRIBUTION_LOGNORMAL;
    ///     dpdAssumption.scenario_type = 1;
    ///     dpdAssumption.mean = 10.0;
    ///     dpdAssumption.use_milan_aaa_ce = 0;
    ///     dpdAssumption.milan_aaa_ce = 0.0;
    ///     dpdAssumption.standard_deviation = 1.0;
    ///     dpdAssumption.discounted_recoveries = 0;
    ///     std::fill(dpdAssumption.revolving_default_factors, dpdAssumption.revolving_default_factors+500, 0.01);
    ///     dpdAssumption.num_scenarios = 3;
    ///     dpdAssumption.use_revolving_def_timing = 1;
    ///     dpdAssumption.rating_cap_primary = 3;
    ///     dpdAssumption.rating_cap_surveillance = 5;
    ///     
    ///     assert(0 == set_dpd_assumption(pDeal, &dpdAssumption));
    ///
    ///     // add settings for DPD
    ///     
    ///     assert(0 == run_default_probability_distribution(pDeal));
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @sa run_default_probability_distribution()
    int CHASAPI set_dpd_assumption(void *tid, const DPD_ASSUMPTION* assumption);

    /// This method sets the scenarios for the default distribution to run. This method needs to be called before running the simulation if users choose to use their own scenarios instead of the ones generated by log-normal/inverse normal distributions. 
    ///
    /// @since 2.1.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre set_simulation_engine() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] scenarios An array which stores the user-defined scenarios for the simulation to run.
    /// @param[in] size_scenario Size of the user-defined scenarios.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid parameters.
    /// @retval  -3 Error - Current simulation engine is not set to "Default Probability Distribution".
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_DEFAULT_PROBABILITY_DISTRIBUTION));
    ///
    ///     DPD_SCENARIO userScenarios[] = {
    ///         {1, 6.00, 8.00},
    ///         {2, 11.00, 90.00},
    ///         {3, 13.00, 1.0},
    ///         {4, 16.00, 1.0}
    ///     };
    ///     assert(0 == set_dpd_scenarios(pDeal, userScenarios, sizeof(userScenarios)/sizeof(userScenarios[0])));
    ///
    ///     // other settings for DPD
    ///
    ///     assert(0 == run_default_probability_distribution(pDeal));
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    /// @sa run_default_probability_distribution()
    int CHASAPI set_dpd_scenarios(void *tid, const DPD_SCENARIO *scenarios, short size_scenario);

    /// This methods sets the default timing curves for current assets.
    ///
    /// @since 2.1.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre set_simulation_engine() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] timing A user allocated array which stores the information of default timing for current assets.
    /// @param[in] size_timing The size of the default timing for current assets.
    /// @param[in] seasoning TRUE if the vector is seasoned and FALSE otherwise.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid parameters.
    /// @retval  -3 Error - Current simulation engine is not set to "Default Probability Distribution".
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_DEFAULT_PROBABILITY_DISTRIBUTION));
    ///
    ///     double currentDefaultTimingVector[] = {0.01};
    ///     assert(0 == set_dpd_current_default_timing(pDeal, currentDefaultTimingVector, 1, 0));
    ///
    ///     // other settings for DPD
    ///
    ///     assert(0 == run_default_probability_distribution(pDeal));
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @sa run_default_probability_distribution()
    int CHASAPI set_dpd_current_default_timing(void *tid, const double *timing, short size_timing, BOOLYAN seasoning);

    /// This methods sets the default timing curves for revolving assets.
    ///
    /// @since 2.1.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre set_simulation_engine() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] timing A user allocated array which stores the information of default timing for revolving assets.
	/// @param[in] size_timing The size of the default timing for revolving assets.
	/// @param[in] seasoning TRUE if the vector is seasoned and FALSE otherwise.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid parameters.
    /// @retval  -3 Error - Current simulation engine is not set to "Default Probability Distribution".
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_DEFAULT_PROBABILITY_DISTRIBUTION));
    ///
    ///     double revolvingDefaultTimingVector[] = {0.02};
    ///     assert(0 == set_dpd_revolving_default_timing(pDeal, revolvingDefaultTimingVector, 1, 0));
    ///
    ///     // other settings for DPD
    ///
    ///     assert(0 == run_default_probability_distribution(pDeal));
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @sa run_default_probability_distribution()
    int CHASAPI set_dpd_revolving_default_timing(void *tid, const double *timing, short size_timing, BOOLYAN seasoning);

    /// This method sets the rating threshold.
    ///
    /// @since 2.1.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre set_simulation_engine() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] rating The rating whose corresponding threshold will be updated.
    /// @param[in] year The year of the rating of which the corresponding threshold will be updated.
    /// @param[in] threshold Threshold value for the specified rating of the specified year.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid parameters.
    /// @retval  -3 Error - Current simulation engine is not set to "Default Probability Distribution".
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_DEFAULT_PROBABILITY_DISTRIBUTION));
    ///
    ///     for (int i = 1; i <= 30; ++i)
    ///     {
    ///         assert(0 == set_dpd_threshold(pDeal, "Aaa", i, 0.000005 + 0.00003*i));
    ///         assert(0 == set_dpd_threshold(pDeal, "Aa1", i, 0.00005 + 0.00003*i));
    ///         assert(0 == set_dpd_threshold(pDeal, "Aa2", i, 0.00001 + 0.00003*i));
    ///     }
    ///     assert(0 == set_dpd_el_pd_factors(pDeal, 0.55, 0.45));
    /// 
    ///     // other settings for DPD
    ///     
    ///     assert(0 == run_default_probability_distribution(pDeal));
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @sa run_default_probability_distribution()
    int CHASAPI set_dpd_threshold(void *tid, const char *rating, short year, double threshold);

    /// This method sets the E.L. and P.D. table factors.
    ///
    /// @since 2.1.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre set_simulation_engine() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] el_factor E.L. factor.
    /// @param[in] pd_factor D.P. factor.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid parameters.
    /// @retval  -3 Error - Current simulation engine is not set to "Default Probability Distribution".
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_DEFAULT_PROBABILITY_DISTRIBUTION));
    ///
    ///     assert(0 == set_dpd_el_pd_factors(pDeal, 0.55, 0.45));
    /// 
    ///     // other settings for DPD
    ///     
    ///     assert(0 == run_default_probability_distribution(pDeal));
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @sa run_default_probability_distribution()
    int CHASAPI set_dpd_el_pd_factors(void *tid, double el_factor, double pd_factor);

    /// This method will return an array of structures which contains the user defined or system generated scenarios. If NULL is passed down for parameter scenarios, then the method will return the size that is needed to store all the scenarios.
    ///
    /// @since 2.1.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre run_default_probability_distribution() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[out] scenarios a user allocated array which stores the information of default/loss scenarios. Call get_dpd_scenarios(tid, NULL, 0) to get the count of scenarios.
    /// @param[in] size_scenarios the size of scenarios. Call get_dpd_scenarios(tid, NULL, 0) to get the count of scenarios.
    ///
    /// @retval >=0 Number of scenarios that have been populated. Number of scenarios that have been populated. Size that is needed to store all the scenarios if NULL is passed down for parameter scenarios.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid parameters.
    /// @retval  -3 Error - Current simulation engine is not set to "Default Probability Distribution".
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     // settings for DPD
    ///
    ///     assert(0 == run_default_probability_distribution(pDeal));
    ///     
    ///     int scenariosNum = get_dpd_scenarios(pDeal, 0, 0);
    ///     assert(4 == scenariosNum); // assume scenario number is 4
    ///
    ///     DPD_SCENARIO scenarios[4];
    ///     assert(4 == get_dpd_scenarios(pDeal, scenarios, sizeof(scenarios)/sizeof(scenarios[0])));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @sa run_default_probability_distribution()
    int CHASAPI get_dpd_scenarios(void *tid, DPD_SCENARIO *scenarios, short size_scenarios);

    /// This methods returns the default timing curves for current assets. If the user input for the default timing does not add up to 100%, the simulator will scale them up/down to 100% when running the simulation. Users can use this method to get the actual values that are used after running the simulation.
    ///
    /// @since 2.1.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre set_simulation_engine() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    ///
    /// @retval  >0  A pointer to an array which stores the default timing for current assets. The size of the array will be MAX_PERIODS.
    /// @retval NULL Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_DEFAULT_PROBABILITY_DISTRIBUTION));
    ///
    ///     // add settings for DPD
    ///     
    ///     assert(0 == run_default_probability_distribution(pDeal));
    ///
    ///     double *currDefaultTimingCurve = get_dpd_current_default_timing(pDeal);
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @sa run_default_probability_distribution()
    double* CHASAPI get_dpd_current_default_timing(void *tid);

    /// This methods returns the default timing curves for revolving assets. If the user input for the default timing does not add up to 100%, the simulator will scale them up/down to 100% when running the simulation. Users can use this method to get the actual values that are used after running the simulation.
    ///
    /// @since 2.1.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre set_simulation_engine() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    ///
    /// @retval  >0   A pointer to an array which stores the revolving default timing for current assets. The size of the array will be MAX_PERIODS.
    /// @retval  NULL Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_DEFAULT_PROBABILITY_DISTRIBUTION));
    ///
    ///     // add settings for DPD
    ///     
    ///     assert(0 == run_default_probability_distribution(pDeal));
    ///
    ///     double *revDefaultTimingCurve = get_dpd_revolving_default_timing(pDeal);
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @sa run_default_probability_distribution()
    double* CHASAPI get_dpd_revolving_default_timing(void *tid);


    /// This method returns the Default Probability Distribution Simulation results.
    ///
    /// @since 2.1.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre run_default_probability_distribution() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid The name of the tranche whose results are being requested. The length should be 20.
    /// @param[out] result A user allocated structure in which the results of the Default Probability Distribution will be stored.
    ///
    /// @retval >=0 number of bonds whose Default Probability Distribution Simulation results have been populated.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid parameters.
    /// @retval  -3 Error - Current simulation engine is not set to "Default Probability Distribution".
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_DEFAULT_PROBABILITY_DISTRIBUTION));
    ///
    ///     // add settings for DPD
    ///     
    ///     assert(0 == run_default_probability_distribution(pDeal));
    ///
    ///     DPD_RESULT result;
    ///     assert(0 == get_dpd_results(pDeal, "A1", &result));
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @sa run_default_probability_distribution()
    int CHASAPI get_dpd_results(void *tid, const char *bondid, DPD_RESULT *result);

    /// This method returns the rating threshold.
    ///
    /// @since 2.1.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre set_simulation_engine() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] rating The rating whose corresponding threshold will be updated.
    /// @param[in] year The year of the rating of which the corresponding threshold will be updated.
    /// @param[out] threshold The threshold value of the specified rating of the specified year.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid parameters.
    /// @retval  -3 Error - Current simulation engine is not set to "Default Probability Distribution".
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_DEFAULT_PROBABILITY_DISTRIBUTION));
    ///
    ///     double threshod = 0.0;
    ///     assert(0 == get_dpd_threshold(pDeal, "Aaa", 3, &threshod)); // can call it before/after run.
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @sa run_default_probability_distribution()
    int CHASAPI get_dpd_threshold(void *tid, const char *rating, short year, double *threshold);

    /// This method returns the E.L. and P.D. table factors.
    ///
    /// @since 2.1.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre set_simulation_engine() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[out] el_factor E.L. factor.
    /// @param[out] pd_factor D.P. factor.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid parameters.
    /// @retval  -3 Error - Current simulation engine is not set to "Default Probability Distribution".
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_DEFAULT_PROBABILITY_DISTRIBUTION));
    ///
    ///     double elFactor = 0.0, pdFactor = 0.0;
    ///     assert(0 == get_dpd_el_pd_factors(pDeal, &elFactor, &pdFactor));    // can call it before/after run.
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @sa run_default_probability_distribution()
    int CHASAPI get_dpd_el_pd_factors(void *tid, double *el_factor, double *pd_factor);

    /// This method overides the loss given default(LGD) for a specified loan.
    ///
    /// @since 2.1.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[in] loan_num The 1-based index of the loan or -1 to apply to all collateral in the deal.
    /// @param[in] lgd A pointer to the user input LGD vector, the max vector length of lgd be overridden is 10.
    /// @param[in] length The length of the user input lgd vector.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid loan number ,invalid deal id or invalid length of lgd.
    /// @retval -99 Error - Invalid dso identifier (tid) and other errors, call get_deal_error_msg() for details.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     double userlgd[] = {0.1,0.2,0.3,0.4,0.5};
    ///     assert(0 == set_loan_lgd(pDeal, NULL, 1, userlgd, 5));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    /// @note The loan lgd would be overridden by after calling run_deal_ex().
    int CHASAPI set_loan_lgd(void *tid, const char* reremic_deal_id_or_null, long loan_num, double* lgd, int length);

    /// This method returns the cash flow dates of a specified bond.
    ///
    /// @since 2.1.0
    /// @avail CDOnet, SFW, CHS
    ///
    /// @pre deal is run
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid The NULL terminated string indicate the name of the tranche whose results are being requested.
    ///
    /// @retval  >0   A pointer to an array which stores the dates requested. The size of the vector is MAX_PERIODS. The first period stores the last payment date of the current deal update.
    /// @retval  NULL Cashflow is not available. Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_MONTE_CARLO));
    ///
    ///     // add settings for simulation and run simulation
    ///
    ///     assert(NULL != get_bond_cf_dates(pDeal, "A2")); // call after run simulation
    ///     assert(NULL != get_coll_cf_dates(pDeal));       // call after run simulation
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int* CHASAPI get_bond_cf_dates(void *tid, const char *bondid);

    /// This method returns the cash flow dates of the collateral.
    ///
    /// @since 2.1.0
    /// @avail CDOnet, SFW
    ///
    /// @pre deal is run
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    ///
    /// @retval  >0   A pointer to an array which stores the dates requested. The size of the vector is MAX_PERIODS.
    /// @retval  NULL Cashflow is not available. Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == set_simulation_engine(pDeal, SIMULATION_MONTE_CARLO));
    ///
    ///     // add settings for simulation and run simulation
    ///
    ///     assert(NULL != get_bond_cf_dates(pDeal, "A2")); // call after run simulation
    ///     assert(NULL != get_coll_cf_dates(pDeal));       // call after run simulation
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int* CHASAPI get_coll_cf_dates(void *tid);

    /// This method sets base of service advance rate, either default balance or delinquent balance.
    ///
    /// @since 2.2.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The service advance type has been set to SERVICER_ADVANCES_INTEREST or SERVICER_ADVANCES_BOTH with set_service_advances_ex().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] type Setting advance assumption on delinquent balance or default balance, should be one of ::SERVICER_ADVANCES_BASE.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2	Invalid parameter.
    /// @retval -99 Other error, use get_deal_error_msg() to see details.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_service_advances_ex(pDeal, SERVICER_ADVANCES_BOTH, true));
    ///
    ///     assert(0 == set_service_advances_rates_type(pDeal, SERVICER_ADVANCES_BASE_DEFAULT));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    /// @see set_service_advances_ex() set_service_advances_rates()
    /// @note If not set, servicer advance projections are based on SERVICER_ADVANCES_BASE_DEFAULT.
    int CHASAPI set_service_advances_rates_type(void* tid, short type);

    /// This method sets service advance rate, either a vector or a constant, with ability to apply on different scope(pool group level or deal level).
    ///
    /// @since 2.2.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The service advance type has been set to SERVICER_ADVANCES_INTEREST or SERVICER_ADVANCES_BOTH with set_service_advances_ex().
    /// @pre The base of service advance rate have been set with set_service_advances_rates_type().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] group_number The collateral group number, 0 for total (deal level).
    /// @param[in] is_vector 0 for constant percentage (pval), length for double array pval.
    /// @param[in] pval constant or vector in decimal for advances from servicer.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2	Invalid parameter.
    /// @retval -99 Other error, use get_deal_error_msg() to see details.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == enable_sfw_delinq_projection(pDeal, true));
    ///     assert(0 == set_service_advances_ex(pDeal, SERVICER_ADVANCES_BOTH, true));
    ///     assert(0 == set_service_advances_rates_type(pDeal, SERVICER_ADVANCES_BASE_DEFAULT));
    ///
    ///     int group_number = 1;   // or 0 for all pools
    ///     double rates[] = {0.5};
    ///     assert(0 == set_service_advances_rates(pDeal, group_number, sizeof(rates)/sizeof(rates[0]), rates));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    /// @see set_service_advances_ex() set_service_advances_rates_type()
    int CHASAPI set_service_advances_rates(void* tid, int group_number, short is_vector, double* pval);

    /// This method gets the implied loss for a bond.
    ///
    /// @since 2.4.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid A pointer to the name of the bond.
    /// @param[out] implied_loss The implied loss value.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid pointer.
    /// @retval  -3 Implied loss is "NOT AVAILABLE".
    /// @retval  -10 Bond not found.
    /// @retval -99 Other error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "AL2010-A");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     double impliedLosses = 0.0;
    ///     assert(0 == get_bond_implied_loss(pDeal, "A1", &impliedLosses));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI get_bond_implied_loss(void *tid, const char* bondid, double* implied_loss);

    /// This method gets the implied loss for a bond.
    ///
    /// @since 2.5.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] int_capital_code_override_type The interest capitalization code override type to set, should be one of ::INT_CAPITAL_CODE_OVERRIDE.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid override type.
    /// @retval  -3 The function is only available for SLABS deals.
    /// @retval -99 Other error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "WSLT2006-1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_int_capital_code_override(pDeal, INT_CAPITAL_CODE_OVERRIDE_ANNUALLY));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_int_capital_code_override(void * tid, short int_capital_code_override_type);

    /// This method sets non performing loans as default.
    ///
    /// @since 2.0.2
    ///
    /// @avail SFW CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] is_defaulted Enable treat non performing loans as default or not.
    /// @param[in] non_perf_status An array contains the loan status which will be treaded as default. non_perf_status size should be 5 at least and CDO deal will ignore this param.
    ///            possible value of non_perf_status:
    ///            non_perf_status[NON_PERFORMING_DELINQUENT]: -1 to MAX_PERIODS. -1 repesent turn off this flag. The value n>0 represent that treat loan deqlinq n month as non performing loan.
    ///            non_perf_status[NON_PERFORMING_BANKRUPTED]: -1 to MAX_PERIODS. -1 repesent turn off this flag. The value n>0 represent that treat loan bankrupted n month as non performing loan.
    ///            non_perf_status[NON_PERFORMING_REO]:-1,0. -1 represent turn off this flag. 0 represent that treat loan in REO status as non-perfroming loan.
    ///            non_perf_status[NON_PERFORMING_FORECLOSED]:-1,0. -1 repesent turn off this flag. 0 represent that treat loan in foreclose status as non-perfroming loan.
    /// @param[in] set_sup_remic Settings are applied to underlying deals if TRUE. Otherwise, it will not.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not opened.
    /// @retval -99 Other errors.
    ///
    /// @sample
    ///     //Deal has been opened
    ///
    ///     short non_perf_status[NON_PERFORMING_SIZE]={-1};
    ///     non_perf_status[NON_PERFORMING_DELINQUENT]=3;
    ///     non_perf_status[NON_PERFORMING_BANKRUPTED]=3;
    ///     non_perf_status[NON_PERFORMING_REO]=0;
    ///     non_perf_status[NON_PERFORMING_FORECLOSED]=0;
    ///     int ret = set_default_non_performing_loans(pDeal,true,non_perf_status);
    /// @endsample
    ///
    int CHASAPI set_default_non_performing_loans(void *tid, BOOLYAN is_defaulted, short *non_perf_status, BOOLYAN set_sup_remic);

    /// This method sets non performing loans months to liquidation.
    ///
    /// @since 3.0.0
    ///
    /// @avail SFW 
    ///
    /// @pre open_deal_ex() has been called.
	/// @pre set_default_non_performing_loans() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] value The lag in months. If value is <0, will not be applied to non performing loans.
    /// @param[in] set_sup_remic Settings are applied to underlying deals if TRUE. Otherwise, it will not.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not opened.
    /// @retval -99 Other errors.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "WSLT2006-1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
	///
    ///     short non_perf_status[NON_PERFORMING_SIZE]={-1};
    ///     non_perf_status[NON_PERFORMING_DELINQUENT]=3;
    ///     non_perf_status[NON_PERFORMING_BANKRUPTED]=3;
    ///     non_perf_status[NON_PERFORMING_REO]=0;
    ///     non_perf_status[NON_PERFORMING_FORECLOSED]=0;
	///     assert(0 == set_default_non_performing_loans(pDeal, true, non_perf_status, false));
    ///
	///     assert(0 == set_non_perf_recovery_lag(pDeal,4, false));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI set_non_perf_recovery_lag(void *tid, short value, BOOLYAN set_sup_remic);

    /// This method sets the smooth losses flag.
    ///
    /// @since 2.7.0
    ///
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
	/// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] status Enable smooth losses or not
    ///            - true means enable smooth losses
    ///            - false means disable smooth losses
    /// @param[in] set_sup_remic Settings are applied to underlying deals if TRUE. Otherwise, it will not.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not opened.
    /// @retval -99 Other errors.
    ///
    /// @sample
    ///     //Deal has been opened
    ///     int ret = set_smooth_losses(pDeal, true, true);
    /// @endsample
    ///
    /// @note This function does not support student loan deals
    ///
    int CHASAPI set_smooth_losses(void *tid, BOOLYAN status, BOOLYAN set_sup_remic);

    /// The function will retrieve pa support type for the deal.
    ///
    /// @exptl Subject to change
    /// @since 2.7.0
    /// @avail CHS,SFW
    ///
    /// @pre open_deal_ex() has been called.
    
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[out] pa_model_type PA model type for the deal, should pre-allocate at least 50 bytes for this field.
    /// @param[out] pa_avail_vector PA output available vector type for the deal, should pre-allocate int array of length NUM_PA_POOL_VECTOR_TYPES.
    /// @param[out] avail_vector_num Number of available PA vector type.

    /// @retval    0 No error
    /// @retval   -1 Error - Deal not opened
    /// @retval  -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    //deal has been opened
    ///
    ///    char pa_model_type[50] = {0};
    ///    int pa_avail_vector[NUM_PA_POOL_VECTOR_TYPES];
    ///    int avail_num = 0;
    ///    int ret = get_pa_model_type(pDeal, pa_model_type, pa_avail_vector, &avail_num);
    ///    if (0 != ret)
    ///    {
    ///        // error handle
    ///    }
    /// @endsample
    ///
    int CHASAPI get_pa_model_type(void *tid, char *pa_model_type, int pa_avail_vector[], int* avail_vector_num);

    /// This method is to get bond payflag vector.
    ///
    /// @since 2.7.0
    /// @avail SFW,CDOnet
    /// @pre run_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[in] bondid The specified bond id.
    ///
    /// @retval NULL  Error - Call get_deal_error_msg(). 
    /// @retval OTHER Pointer to the vector of cashflows.
    ///
    /// @sample
    ///     void* pDeal= NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SANGO1101P");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     // open deal
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     // run deal
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     // get pay flag
    ///     int* payflag = get_bond_payflag(pDeal, NULL, "A1");
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int* CHASAPI get_bond_payflag(void * tid, const char * reremic_deal_id_or_null, const char *bondid);

    /// Retrieves loan level cashflows
    ///
    /// @exptl Subject to change
    /// @since 2.7.0
    /// @avail SFW
    ///
    /// @pre run_deal_ex() has been called. calculation level set to CALC_LEVEL_FULL_WITH_LOAN by set_deal_calc_level
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] loan_number The 1-based index of the loan.
    /// @param[in] reremic_deal_id_or_null Pass NULL for main deal or remic name for underlying deal
    /// @param[in] flow_identifier Cashflow identifier. Currently loan level cashflow support:
    ///            ::FLOW_COLLATERAL_BALANCE
    ///            ::FLOW_COLLATERAL_INTEREST
    ///            ::FLOW_COLLATERAL_PRINCIPAL
    ///            ::FLOW_COLLATERAL_CASH 
    ///
    /// @retval  NULL  The requested loan cashflow was not retrieved successfully.
    /// @retval  Other A pointer to an array which stores the requested loan cashflow,the array length is MAX_PERIODS.
    ///
    /// @sample
    ///     void* ptid= NULL;
    ///     CMO_STRUCT cmo;
    ///     memset(&cmo,0,sizeof(CMO_STRUCT));
    ///     strcpy(cmo.dealid,"ACE06NC1");
    ///
    ///     open_deal_ex(ptid,&cmo);
    ///
    ///     set_deal_calc_level(ptid, CALC_LEVEL_FULL_WITH_LOAN, 1); 
    ///     run_deal_ex(ptid,&cmo);
    ///     // Deal is already opened successfully.
    ///     // Deal is already run successfully.
    ///
    ///     // Get bond flow of balance.
    ///     double* pLoanBalance = get_loan_flow(ptid, 1, NULL, FLOW_COLLATERAL_BALANCE);
    ///
    ///     close_deal_ex(ptid, &cmo);
    /// @endsample
    ///
    double* CHASAPI get_loan_flow(void * tid, int loan_number, const char * reremic_deal_id_or_null, int flow_identifier);

	/// Retrieves brief loan cashflow to the cashflow structure MOODYS_LOAN_CASHFLOW
    ///
    /// @exptl Subject to change
    /// @since 3.3.0
    /// @avail CDOnet, SFW
    ///
    /// @pre run_deal_ex() has been called. calculation level set to CALC_LEVEL_FULL_WITH_LOAN by set_deal_calc_level
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] loan_number The 1-based index of the loan.
    /// @param[in] cf The loan level cash flow data.
    ///
    /// @retval    0 No error
    /// @retval   -1 Error - Deal not opened.
	/// @retval   -1 Error - Invalid loan number.
	/// @retval   -1 Error - Loan not found.
    /// @retval  -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
	///     void* ptid= NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///
    ///     std::vector<WHOLE_LOAN_STRUCT> loans;
    ///     // set informations for each loan in vector loans
    ///     set_whole_loan(ptid, &loans.front(), 10, 20160101);
    ///
    ///     set_deal_calc_level(ptid, CALC_LEVEL_FULL_WITH_LOAN, 1); 
    ///     run_deal_ex(ptid, pCmo);
    ///     // Deal is already opened successfully.
    ///     // Deal is already run successfully.
    ///
	///     MOODYS_LOAN_CASHFLOW MoodysLoanFlow;
	///     memset(&MoodysLoanFlow, 0, sizeof(MOODYS_LOAN_CASHFLOW));
    ///     int ret = get_loan_flow_ex(ptid, 1, &MoodysLoanFlow);
    ///
    ///     close_deal_ex(ptid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
	int CHASAPI get_loan_flow_ex (void * tid, int loan_number, MOODYS_LOAN_CASHFLOW * cf);

	/// Retrieves loan pay dates
    ///
    /// @exptl Subject to change
    /// @since 2.9.0
    /// @avail SFW
    ///
    /// @pre run_deal_ex() has been called. calculation level set to CALC_LEVEL_FULL_WITH_LOAN by set_deal_calc_level
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] loan_number The 1-based index of the loan.
    ///
    /// @retval  >0   A pointer to an array which stores the loan pay dates. The size of the vector is MAX_PERIODS.
    /// @retval  NULL pay dates is not available. Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal= NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///
    ///     std::vector<WHOLE_LOAN_STRUCT> loans;
	///     // set informations for each loan in vector loans
    ///     set_whole_loan(pDeal, &loans.front(), 10, 20160101);
    ///
    ///     set_deal_calc_level(ptid, CALC_LEVEL_FULL_WITH_LOAN, 1); 
    ///     run_deal_ex(pDeal, pCmo);
    ///     // Deal is already run successfully.
    ///
    ///     // Get loan dates.
    ///     assert(NULL != get_loan_dates(pDeal, 1));
    ///
    ///     close_deal_ex(pDeal, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int* CHASAPI get_loan_dates(void * tid, int loan_number);

	/// Retrieves loan pay cashflow size
    ///
    /// @exptl Subject to change
    /// @since 3.4.1
    /// @avail CDONET, SFW
    ///
    /// @pre set_whole_loan() has been called. calculation level set to CALC_LEVEL_FULL_WITH_LOAN by set_deal_calc_level
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] loan_number The 1-based index of the loan.
    ///
    /// @retval  >=0   The size of the loan cashflow.
    /// @retval  -2 Error - Invalid loan number.
	/// @retval  -4 Error - Other error.
	/// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal= NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///
    ///     std::vector<WHOLE_LOAN_STRUCT> loans;
	///     // set informations for each loan in vector loans
    ///     set_whole_loan(pDeal, &loans.front(), 10, 20160101);
    ///
    ///     set_deal_calc_level(ptid, CALC_LEVEL_FULL_WITH_LOAN, 1); 
    ///     run_deal_ex(pDeal, pCmo);
    ///     // Deal is already run successfully.
    ///
    ///     // Get loan cashflow size.
    ///     int ret = get_loan_flow_size(pDeal, 1);
    ///
    ///     close_deal_ex(pDeal, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
	int CHASAPI get_loan_flow_size(void * tid, int loan_number);

    /// Retrieves additional deal information
    ///
    /// @exptl Subject to change
    /// @since 2.6.4
    /// @avail CDOnet, SFW, CHS
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null Pass NULL for main deal or remic name for underlying deal
    /// @param[out] deal_info additional deal information, .
    ///
    /// @retval    0 No error
    /// @retval   -1 Error - Deal not opened
    /// @retval  -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    //deal has been opened
    ///
    ///    MOODYS_DEAL_INFO mdi={};
    ///    int ret = get_deal_info_ex(pDeal, NULL, &mdi);
    ///    if (0 != ret)
    ///    {
    ///        // error handle
    ///    }
    /// @endsample
    ///
    long CHASAPI get_deal_info_ex(void *tid, const char* reremic_deal_id_or_null, MOODYS_DEAL_INFO* deal_info);

    /// Set whole loan information
    ///
    /// @exptl Subject to change
    /// @since 2.7.0
    /// @avail SFW, CDOnet
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] whole_loan Whole loan detail information
    /// @param[in] length Number of loans.
    /// @param[in] initial_date Initial date of cash flow projection.
    ///
    /// @retval    0 No error
    /// @retval   -2 Error - Invalid factor for whole loan, use get_deal_error_msg() to see details.
    /// @retval   -3 Error - Invalid length
    /// @retval  -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///     void *pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///
    ///     std::vector<WHOLE_LOAN_STRUCT> loans;
    ///     // set informations for each loan in vector loans
    ///     set_whole_loan(pDeal, &loans.front(), 10, 20160101);
    ///
    ///     set_deal_calc_level(pDeal, CALC_LEVEL_FULL_WITH_LOAN, 1);
    ///    
    ///
    ///    // get loan cashflow
    /// @endsample
    ///
    int CHASAPI set_whole_loan(void* tid, const WHOLE_LOAN_STRUCT* whole_loan, int length, int initial_date);

	/// Set distressed property recovery information
    ///
    /// @exptl Subject to change
    /// @since 3.4.0
    /// @avail SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
	/// @param[in] loan_number The 1-based index of the loan.
    /// @param[in] recovery_inputs distressed property recovery detail information.
    ///
    /// @retval    0 No error
    /// @retval  -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
	///     void *pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///
    ///     std::vector<WHOLE_LOAN_STRUCT> loans;
    ///     // set informations for each loan in vector loans
    ///     set_whole_loan(pDeal, &loans.front(), 10, 20160101);
    ///
    ///     set_deal_calc_level(pDeal, CALC_LEVEL_FULL_WITH_LOAN, 1);
    ///   
	///		DISTRESSED_PROPERTY_RECOVERY recovery_inputs;
	///		memset(&recovery_inputs, 0, sizeof(recovery_inputs));
	///		recovery_inputs.recovery_lag = 6;
	///		recovery_inputs.inflation_start_period = 0;
	///		recovery_inputs.inflation_rate = 0.02;
	///		recovery_inputs.distressed_property_value = 105263;
	///		recovery_inputs.variable_foreclosure_cost = 0.01;
	///		recovery_inputs.fixed_foreclosure_cost = 1000;
	///		set_distressed_property_recovery(pDeal, -1, &recovery_inputs);
	///
    ///     run_deal_ex(pDeal, pCmo);
    ///
    ///     // get loan cashflow
    ///
    ///     close_deal_ex(pDeal, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI set_distressed_property_recovery( void * tid, int loan_number, DISTRESSED_PROPERTY_RECOVERY * recovery_inputs);

    /// This method returns the array of rate reset date of a specified bond.
    ///
    /// @since 2.7.0
    /// @avail SFW
    ///
    /// @pre run_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid The NULL terminated string indicate the name of the tranche whose results are being requested.
    ///
    /// @retval  >0   A pointer to an array which stores the bond rate reset dates requested. The size of the vector is 500.
    /// @retval  NULL reset date dates is not available. Call get_deal_error_msg() for detail.
    ///
    /// @note The rate reset dates just available for the floater bonds.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     assert(NULL != get_bond_rate_reset_dates(pDeal, "A2")); 
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int* CHASAPI get_bond_rate_reset_dates(void *tid, const char *bondid);

    /// This method sets the max number of sfw dll copies in RAM.
    ///
    /// @since 2.8.0
    ///
    /// @avail SFW
    ///
    /// @pre None.
    ///
    /// @param[in] num The max sfw dll number to set.
    ///
    /// @retval None
    ///
    /// @sample
    ///     set_sfw_dll_num(8);
    /// @endsample
    ///
    /// @note
    /// - This function should be called before open_deal_ex
    /// - For 32-bit system, the max dll number is capped by 16, and for 64-bit system the max dll number is capped by 4096.
    ///
    void CHASAPI set_sfw_dll_num(const int& num);

    /// This method sets the max number of cdonet dll copies in RAM.
    ///
    /// @since 2.8.0
    ///
    /// @avail CDOnet
    ///
    /// @pre None.
    ///
    /// @param[in] num The max cdonet dll number to set.
    ///
    /// @retval None
    ///
    /// @sample
    ///     set_cdonet_dll_num(8);
    /// @endsample
    ///
    /// @note
    /// - This function should be called before open_deal_ex
    /// - For 32-bit system, the max dll number is capped by 16, and for 64-bit system the max dll number is capped by 4096.
    ///
    void CHASAPI set_cdonet_dll_num(const int& num);

	/// Set the keep dll space flag to determine release dll space in memory or not when closing deals for SFW.
    ///
    /// @since 3.3.0
    /// @avail SFW
    ///
    /// @param[in] unload_dll Means release dll space in memory or not when closing deals. True means realse, False means not.
    ///
    /// @retval None.
    ///
    /// @sample
	///		set_sfw_unload_flag(true);
    /// @endsample
    ///
	void CHASAPI set_sfw_unload_flag(bool unload_dll);

	
	/// Set the keep dll space flag to determine release dll space in memory or not when closing deals for CDONET.
    ///
    /// @since 3.3.0
    /// @avail CDONET
    ///
    /// @param[in] unload_dll Means release dll space in memory or not when closing deals. True means realse, False means not.
    ///
    /// @retval None.
    ///
    /// @sample
	///		set_cdonet_unload_flag(true);
    /// @endsample
    ///
	void CHASAPI set_cdonet_unload_flag(bool unload_dll);

    /// This method get the currency code for a specified bond .
    ///
    /// @since 2.5.0
    /// @avail All
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid The specified bond id.
    /// @param[out] currency A pre-allocated pointer of at least 4 characters for the currency code of the specified bond.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid bondid.
    /// @retval  -3 currency pointer is null.
    /// @retval -99 Error ,call get_deal_error_msg() for details.
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    //deal has been opened
    ///
    ///    char currency[4] = {0};
    ///    int ret = get_bond_currency(pDeal, "A1", currency);
    ///    if (0 != ret)
    ///    {
    ///        // error handle
    ///    }
    /// @endsample
    int CHASAPI get_bond_currency(void* tid, const char* bondid, char* currency);

	/// This method get the bond step up coupon and related infomation for a specified bond.
    ///
    /// @since 2.9.0
    /// @avail SFW
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid The specified bond id.
    /// @param[out] all_set_up_coupons A pre-allocated array of client-allocated  BOND_STEP_UP_COUPON struct for the step-up coupon of the specified bond.
    /// @param[in] array_size Size of the array of BOND_STEP_UP_COUPON that user has passed.
	/// @param[out] num_available Total number of step-up coupons for . 
	/// @retval  >=0 Success. Actual number of coupon for the specified bond.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid bondid.
    /// @retval  -3 Bond not found.
    /// @retval -99 Error ,call get_deal_error_msg() for details.
    ///
    /// @sample
	///     void *pDeal = NULL;
    ///     //deal has been opened
    ///
    ///     int iret = get_bond_step_up_coupon(pDeal, "A" , NULL, 0, NULL);
	///     if (iret < 0)
    ///     {
    ///         // error handle
    ///     }
    ///
    ///     BOND_STEP_UP_COUPON * pbond_stepup_coupon = (BOND_STEP_UP_COUPON*)malloc(sizeof(BOND_STEP_UP_COUPON) * iret);
    ///     memset(pbond_stepup_coupon, 0, sizeof(BOND_STEP_UP_COUPON) * iret);
    ///     int num_coupons = 0;
    ///     iret = get_bond_step_up_coupon(pDeal, "A", pbond_stepup_coupon, iret, &num_coupons);
    /// @endsample
    int CHASAPI get_bond_step_up_coupon(void * tid, const char * bondid, BOND_STEP_UP_COUPON all_set_up_coupons[], int array_size, int * num_available);

    /// This method returns a list of hedge information(ids, descriptions, margin, hedge override info) of an SFW deal.
    ///
    /// @since 2.9.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[out] hedge_info A client-allocated array of ::MOODYS_HEDGE_STRUCT in hedges informationwhich will be stored
    /// @param[in] size The length of the array hedge_info that the user has passed down.
    /// @param[out] num_hedges Total number of available hedges.
    ///
    /// @retval  >=0 Actual number of hedges returned.
    /// @retval  -1 Deal not open.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    //deal has been opened
    ///
    ///    MOODYS_HEDGE_STRUCT hedgeinfo[1] = {};
    ///    memset(hedgeinfo, 0, sizeof(MOODYS_HEDGE_STRUCT)*1);
    ///    int num_hedges = 0;
    ///    int iret = get_deal_hedge(pDeal, NULL, hedgeinfo, 1, &num_hedges);
    ///    if(iret > 0 && num_hedges > iret)
    ///    {
    ///        std::vector<MOODYS_HEDGE_STRUCT> hedgeinfovec(num_hedges);
    ///        iret = get_deal_hedge(pDeal, NULL, &hedgeinfovec.front(), num_hedges, &num_hedges);
    ///    }
    /// @endsample
    /// @note Pass NULL for hedge_info to get the number of hedges.
    ///     The hedges can be overridden by calling set_deal_hedge_override before calling run_deal_ex().
    int CHASAPI get_deal_hedge(void *tid, const char *reremic_deal_id_or_null, MOODYS_HEDGE_STRUCT hedge_info[], int size, int* num_hedges);

    /// This method override the specific hedge of an SFW deal.
    ///
    /// @since 2.9.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[in] hedge_id The id of hedge which need to be overrided.
    /// @param[in] hedge_override_info The override hedge information.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid hedge id or not found the hedge.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    //deal has been opened
    ///
    ///    MOODYS_HEDGE_OVERRIDE override_hedge;
    ///    memset(&override_hedge, 0, sizeof(MOODYS_HEDGE_OVERRIDE));
    ///    override_hedge.use_paying_margin_override = true;
    ///    override_hedge.paying_margin_override_from = 20160814;
    ///    override_hedge.paying_margin_override_to= 20181011;
    ///    override_hedge.override_paying_margin = 0.08;
    ///    int iret = set_deal_hedge_override(pDeal, NULL, "FIXED", override_hedge);
    /// @endsample
    int CHASAPI set_deal_hedge_override(void *tid, const char *reremic_deal_id_or_null, const char *hedge_id, MOODYS_HEDGE_OVERRIDE hedge_override_info);

    /// This method returns a list of fee information(name, type, value, day count info) of a deal.
    ///
    /// @since 2.9.0
    /// @avail SFW, CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[out] fee_info A client-allocated array of ::MOODYS_FEE_STRUCT for the fee information which will be stored
    /// @param[in] size The length of the array fee_info that the user has passed down.
    /// @param[out] num_fees Total number of available Fees.
    ///
    /// @retval  >=0 Actual number of fees returned.
    /// @retval  -1 Deal not open.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    //deal has been opened
    ///
    ///    MOODYS_FEE_STRUCT fee_info[1] = {};
    ///    memset(fee_info, 0, sizeof(MOODYS_FEE_STRUCT)*1);
    ///    int num_fees = 0;
    ///    int iret = get_deal_fee(pDeal, NULL, fee_info, 1, &num_fees);
    ///    if(iret > 0 && num_fees > iret)
    ///    {
    ///        std::vector<MOODYS_FEE_STRUCT> fee_info_vec(num_fees);
    ///        iret = get_deal_fee(pDeal, NULL, &fee_info_vec.front(), num_fees, &num_fees);
    ///    }
    /// @endsample
    /// @note Pass NULL for fee_info to get the number of fees.
    ///     The fees can be overridden by calling set_deal_fee_override before calling run_deal_ex().
    int CHASAPI get_deal_fee(void *tid, const char *reremic_deal_id_or_null,  MOODYS_FEE_STRUCT fee_info[], int size, int* num_fees);

    /// This method returns the fee flow of a specific fee item through fee name of a deal.
    ///
    /// @since 3.3.0
    /// @avail SFW, CDOnet
    ///
    /// @pre run_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[in] fee_name The fee name, can be retrieved from function get_deal_fee.
    ///
    /// @retval  NULL Error - Call get_deal_error_msg().
    /// @retval  OTHER Pointer to the vector of cashflows.
    ///
    /// @sample
    ///    void *pDeal = NULL;
	///    CMO_STRUCT *pCmo = new CMO_STRUCT();
	///    memset(pCmo, 0, sizeof(*pCmo));
	///    strcpy(pCmo->dealid, "STO16IIBV");
    ///
	///    set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
	///    assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///    MOODYS_FEE_STRUCT fee_info[1] = {};
    ///    memset(fee_info, 0, sizeof(MOODYS_FEE_STRUCT)*1);
    ///    int num_fees = 0;
    ///    int iret = get_deal_fee(pDeal, NULL, fee_info, 1, &num_fees);
    ///
    ///    assert(0 == run_deal_ex(pDeal, pCmo));
	///
    ///    double * flow = get_deal_fee_flow(pDeal, NULL, feeinfo[0].fee_name);
    ///
	///    assert(0 == close_deal_ex(pDeal, pCmo));
	///    delete pCmo;
	///    pCmo = NULL;
    /// @endsample
    ///
    double * CHASAPI get_deal_fee_flow(void * tid, const char * reremic_deal_id_or_null, char * fee_name);

    /// This method override the specific fee of an SFW deal.
    ///
    /// @since 2.9.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[in] fee_id The 1-based index of the fee.
    /// @param[in] fee_type The override type of fee, the value is one of ::MOODYS_FEE_CAL_CODE.
    /// @param[in] override_value The override value of fee.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Deal not open.
    /// @retval  -2 Invalid fee id.
	/// @retval  -3 Fee type not found.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///    void *pDeal = NULL;
    ///    //deal has been opened
    ///
    ///    int iret = set_deal_fee_override(pDeal, NULL, 1, FEES_TOTAL_BONDS_CALC, 5.0);
    ///    if (iret < 0)
    ///         //error handling
    /// @endsample
    int CHASAPI set_deal_fee_override(void *tid, const char *reremic_deal_id_or_null, int fee_id, short fee_type, double override_value);

	/// This function will set metric inputs for deal run, in order to get metric results.
	///
	/// @since 3.0.0
	/// @avail ALL
	///
	/// @pre open_deal_ex() has been called.
	///
	/// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
	/// @param[in] metric_inputs The inputs information for market metrics run.
	///
	/// @retval   0 Success.
	/// @retval  -1 Error - Deal not open.
	/// @retval  -2 Error - Invalid input simulation number.
	/// @retval  -3 Error - Invalid rate shift amount.
	/// @retval  -4 Error - Invalid number of paths.
	/// @retval  -5 Error - Invalid extended metrics cal mode.
	/// @retval  -6 Error - Invalid pointer of metric input struct.
	/// @retval -99 Error - Call get_deal_error_msg() for detail.
	///
	/// @sample
	///     void* pDeal = NULL;
	///     CMO_STRUCT *pCmo = new CMO_STRUCT();
	///     memset(pCmo, 0, sizeof(*pCmo));
	///     strcpy(pCmo->dealid, "AMEXCAMT");
    ///
	///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
	///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
	///     METRIC_INPUT_STRUCT_EX metric_input_ex;
	///     memset(&metric_input_ex, 0, sizeof(METRIC_INPUT_STRUCT_EX));
	///     metric_input_ex.shift_amt = 0.00001;
	///     metric_input_ex.num_paths = 10;
	///     metric_input_ex.oas_mode = ENABLE_ALL;
	///     assert(0 == set_metrics_input_ex(pDeal, &metric_input_ex));
	///
	///     assert(0 == run_deal_ex(pDeal, pCmo));
	///
	///     assert(0 == close_deal_ex(pDeal, pCmo));
	///     delete pCmo;
	///     pCmo = NULL;
	/// @endsample
	int CHASAPI set_metrics_input_ex(void* tid, METRIC_INPUT_STRUCT_EX* metric_inputs);

	/// This function will set adjustment table parameter alpha and phi for bank loan call.
	///
	/// @since 3.0.0
	/// @avail CDONet
	///
	/// @pre set_whole_loan() has been called.
	///
	/// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
	/// @param[in] bankloan_adj The inputs information adjustment table parameter.
	/// @param[in] length The length of BANKLOAN_CALL_ADJ_PARAM.
	///
	/// @retval   0 Success.
	/// @retval  -2 Error - Invalid pointer of bank loan adj input struct.
	/// @retval  -3 Error - Invalid bank loan adj structure length.
	/// @retval -99 Error - Call get_deal_error_msg() for detail.
	///
    /// @sample
    ///     void* pDeal = NULL;
    ///
    ///     std::vector<WHOLE_LOAN_STRUCT> loans;
    ///     // set informations for each loan in vector loans
    ///     set_whole_loan(pDeal, &loans.front(), 10, 20160101);
	///
	///	    const int path_number = 100;     
	///	    METRIC_INPUT_STRUCT_EX metric_input_ex;
	///	    memset(&metric_input_ex, 0, sizeof(METRIC_INPUT_STRUCT_EX));
	///	    metric_input_ex.shift_amt = 0.005;
	///	    metric_input_ex.num_paths = path_number;
	///	    metric_input_ex.oas_mode = ENABLE_ALL;
	///	    assert(0 == set_metrics_input_ex(pDeal, &metric_input_ex));
    ///
    ///	    const int rate_size = 200;
    ///	    double **pVal = new double*[path_number];
    ///	    for (int i = 0; i < path_number; ++i)
    ///	    {
    ///	        pVal[i] = new double[rate_size];
    ///	    }
    ///	    //Fill the index rate values for each path
	///
	///	    std::vector<BANKLOAN_CALL_ADJ_PARAM> adjs;
	///	    adjs.clear();
	///	    BANKLOAN_CALL_ADJ_PARAM adj_1;
	///	    memset(&adj_1, 0, sizeof(adj_1));
	///	    adj_1.moodys_rating = MOODYS_RATING_B3;
	///	    adj_1.esg_spotspread_rating = ESG_RATING_B;
	///	    adj_1.alpha = 70;
	///	    adj_1.phi = 6;
    ///	    adjs.push_back(adj_1);
	///	    set_bankloan_call_adj_param(pDeal, &adjs.front(), 1);
    /// @endsample
    ///
	int CHASAPI set_bankloan_call_adj_param(void* tid, const BANKLOAN_CALL_ADJ_PARAM* bankloan_adj, int length);

	/// This method returns the market risk metrics calculation result of a specified bond.
	///
	/// @since 3.0.0
	/// @avail ALL
	///
	/// @pre run_deal_ex() has been called.
	///
	/// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
	/// @param[in] bondid A pointer to the name of the bond..
	/// @param[in] metric_inputs The inputs for calculating metrics result.
	/// @param[out] metric_results The pointer of the market risk result.
	///
	/// @retval  =0   Success
	/// @retval  -1 Deal not open.
	/// @retval  -2 Error - Invalid bond name.
	/// @retval  -3 Error - bond not found.
	/// @retval  -4 Error - Invalid index type.
	/// @retval  -5 Error - Invalid pointer of metric results.
	/// @retval -99 Error - Call get_deal_error_msg() for detail.
	///
	/// @sample
	///     void* pDeal = NULL;
	///     CMO_STRUCT *pCmo = new CMO_STRUCT();
	///     memset(pCmo, 0, sizeof(*pCmo));
	///     strcpy(pCmo->dealid, "AMEXCAMT");
    ///
	///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
	///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
	///     METRIC_INPUT_STRUCT metric_input;
	///     memset(&metric_input, 0, sizeof(METRIC_INPUT_STRUCT));
	///     metric_input.clean_price = 100;
	///     metric_input.apply_spread_to = APPLY_SPREAD_TO_TSY;
	///
	///     assert(0 == run_deal_ex(pDeal, pCmo));
	///
	///     METRIC_RESULTS_STRUCT results_m;
	///     memset(&results_m, 0, sizeof(METRIC_RESULTS_STRUCT));
	///     assert(0 == get_bond_market_risk_metrics(pDeal, bondid, &metric_input, &results_m));
	///
	///     assert(0 == close_deal_ex(pDeal, pCmo));
	///     delete pCmo;
	///     pCmo = NULL;
	/// @endsample
    int CHASAPI get_bond_market_risk_metrics(void *tid, const char *bondid, METRIC_INPUT_STRUCT* metric_inputs, METRIC_RESULTS_STRUCT* metric_results);

	/// This method returns the market risk metrics calculation result of a specified loan.
	///
	/// @since 3.2.0
	/// @avail CDONet
	///
	/// @pre set_whole_loan() has been called.
	///
	/// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
	/// @param[in] LoanID Loan Number..
	/// @param[in] metric_inputs The inputs for calculating metrics result.
	/// @param[out] metric_results The pointer of the market risk result.
	///
	/// @retval  =0   Success
	/// @retval  -1 Deal not open.
	/// @retval  -2 Error - Invalid loan number.
	/// @retval  -3 Error - Loan not found.
	/// @retval  -4 Error - Invalid index type.
	/// @retval  -5 Error - Invalid pointer of metric results.
	/// @retval -99 Error - Call get_deal_error_msg() for detail.
	///
	/// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///
    ///     std::vector<WHOLE_LOAN_STRUCT> loans;
    ///     // set informations for each loan in vector loans
    ///     set_whole_loan(pDeal, &loans.front(), 10, 20160101);
    ///
	///     METRIC_INPUT_STRUCT metric_input;
	///     memset(&metric_input, 0, sizeof(METRIC_INPUT_STRUCT));
	///     metric_input.clean_price = 100;
	///     metric_input.apply_spread_to = APPLY_SPREAD_TO_TSY;
	///
	///     assert(0 == run_deal_ex(pDeal, pCmo));
	///
	///     METRIC_RESULTS_STRUCT results_m;
	///     memset(&results_m, 0, sizeof(METRIC_RESULTS_STRUCT));
	///     assert(0 == get_loan_market_risk_metrics(pDeal, loanID, &metric_input, &results_m));
	///
	///     assert(0 == close_deal_ex(pDeal, pCmo));
	///     delete pCmo;
	///     pCmo = NULL;
	/// @endsample
    int CHASAPI get_loan_market_risk_metrics(void *tid, int LoanID, METRIC_INPUT_STRUCT* metric_inputs, METRIC_RESULTS_STRUCT* metric_results);

	/// This method returns the extra market risk metrics calculation result of a specified bond.
	///
	/// @since 3.0.0
	/// @avail ALL
	///
	/// @pre set_metrics_input_ex() with OAS_CAL_MODE enabled has been called.
	///
	/// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
	/// @param[in] bondid A pointer to the name of the bond.
	/// @param[in] anchor_type Anchor type for metrics calculation. Available options: MARKET_PRICE or OAS, if input MARKET_PRICE then return oas result, otherwise return market price.
	/// @param[in] anchor_value Value of the provided metric anchor, corresponding to anchor_type in METRIC_INPUT_STRUCT_EX
	/// @param[in] apply_to The type of metric calculation apply spread to TREATURY/LIBOR Curves.
	/// @param[out] results_ex The pointer of the market risk result.
	///
	/// @retval  =0   Success
	/// @retval  -1 Deal not open.
	/// @retval  -2 Error - Invalid bond name.
	/// @retval  -3 Error - bond not found.
	/// @retval  -4 Error - Invalid anchor type.
    /// @retval  -5 Error - Invalid spread apply type.
	/// @retval  -6 Error - Invalid pointer of metric results.
	/// @retval -99 Error - Call get_deal_error_msg() for detail.
	///
	/// @sample
	///     void* pDeal = NULL;
	///     CMO_STRUCT *pCmo = new CMO_STRUCT();
	///     memset(pCmo, 0, sizeof(*pCmo));
	///     strcpy(pCmo->dealid, "AMEXCAMT");
	///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
	///     assert(0 == open_deal_ex(pDeal, pCmo));
	///
	///     assert(0 == run_deal_ex(pDeal, pCmo));
	///
	///     METRIC_RESULTS_STRUCT_EX metric_result_ex;
	///     memset(&metric_result_ex, 0, sizeof(metric_result_ex));
	///     assert(0 == get_bond_market_risk_metrics_ex(pDeal, bondid, MARKET_PRICE, 100, APPLY_SPREAD_TO_LIBOR, &metric_result_ex));
	///
	///     assert(0 == close_deal_ex(pDeal, pCmo));
	///     delete pCmo;
	///     pCmo = NULL;
	/// @endsample
	/// @note If want to get the metric results of this method, set_metrics_input_ex must be called before.
    /// @warning In each path of OAS-related analysis, LIBOR 6 month spread input to MPA and PA is floored to 0.01% as required by MPA and PA; TSY 1 year and TSY 10 year input to MPA and PA is floored to -2% as required by MPA and PA.
    int CHASAPI get_bond_market_risk_metrics_ex(void* tid, char * bondid, METRIC_ANCHORS anchor_type, double anchor_value, APPLY_SPREAD_TYPE apply_to, METRIC_RESULTS_STRUCT_EX * results_ex);

	/// This method returns the extra market risk metrics calculation result of a specified loan.
	///
	/// @since 3.0.0
	/// @avail ALL
	///
	/// @pre set_metrics_input_ex() with OAS_CAL_MODE enabled has been called.
	///
	/// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
	/// @param[in] LoanID Loan number.
	/// @param[in] anchor_type Anchor type for metrics calculation. Available options: MARKET_PRICE or OAS, if input MARKET_PRICE then return oas result, otherwise return market price.
	/// @param[in] anchor_value Value of the provided metric anchor, corresponding to anchor_type in METRIC_INPUT_STRUCT_EX
	/// @param[in] apply_to The type of metric calculation apply spread to TREATURY/LIBOR Curves.
	/// @param[out] results_ex The pointer of the market risk result.
	///
	/// @retval  =0   Success
	/// @retval  -1 Deal not open.
	/// @retval  -2 Error - Invalid loan number.
	/// @retval  -3 Error - loan not found.
	/// @retval  -4 Error - Invalid anchor type.
    /// @retval  -5 Error - Invalid spread apply type.
	/// @retval  -6 Error - Invalid pointer of metric results.
	/// @retval -99 Error - Call get_deal_error_msg() for detail.
	///
	/// @sample
	///     void* pDeal = NULL;
	///     CMO_STRUCT *pCmo = new CMO_STRUCT();
	///     memset(pCmo, 0, sizeof(*pCmo));
	///     strcpy(pCmo->dealid, "AMEXCAMT");
    ///     char *bondid = "20151A";
    ///
	///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
	///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     double rate_L1 = .0024563;
    ///     double rate_T3 = .001;
    ///     short index_L1 = LIBOR_1, index_T3 = TSY_3;
    ///     assert(0 == set_index_rate(pDeal, "USD", &index_L1, 0, &rate_L1));
    ///     assert(0 == set_index_rate(pDeal, "USD", &index_T3, 0, &rate_T3));
    ///
	///     METRIC_INPUT_STRUCT_EX metric_input_ex;
	///     memset(&metric_input_ex, 0, sizeof(METRIC_INPUT_STRUCT_EX));
	///     metric_input_ex.shift_amt = 0.00001;
	///     metric_input_ex.num_paths = 10;
	///     metric_input_ex.oas_mode = ENABLE_ALL;
	///     assert(0 == set_metrics_input_ex(pDeal, &metric_input_ex));
	///
	///     assert(0 == run_deal_ex(pDeal, pCmo));
	///
	///     METRIC_RESULTS_STRUCT_EX metric_result_ex;
	///     memset(&metric_result_ex, 0, sizeof(metric_result_ex));
	///     assert(0 == get_loan_market_risk_metrics_ex(pDeal, LoanID, MARKET_PRICE, 100, APPLY_SPREAD_TO_LIBOR, &metric_result_ex));
	///
	///     assert(0 == close_deal_ex(pDeal, pCmo));
	///     delete pCmo;
	///     pCmo = NULL;
	/// @endsample
	/// @note If want to get the metric results of this method, set_metrics_input_ex must be called before.
    /// @warning In each path of OAS-related analysis, LIBOR 6 month spread input to MPA and PA is floored to 0.01% as required by MPA and PA; TSY 1 year and TSY 10 year input to MPA and PA is floored to -2% as required by MPA and PA.
    int CHASAPI get_loan_market_risk_metrics_ex(void* tid, int LoanID, METRIC_ANCHORS anchor_type, double anchor_value, APPLY_SPREAD_TYPE apply_to, METRIC_RESULTS_STRUCT_EX * results_ex);

	/// This function will set the number of thread for MPA simulation.
    ///
    /// @since 3.0.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called and set MPA mode to simulation.
    /// @pre The current credit model has been set to MPA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] number The number of thread for MPA simulation, the value should be in [1, 32]. The default thread number is equal to CPU core number.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Invalid input thread number.
    /// @retval  -3 Error - Current credit model is not in MPA simulation mode.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, false));
    ///     assert(0 == set_mpa_analysis_type(pDeal, MPA_LOSS_SIMULATION));
    ///
    ///     assert(0 == set_mpa_thread_count(pDeal, 10));
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_mpa_thread_count(void *tid, int number);

	/// This function will set the number of thread for PA simulation.
    ///
    /// @since 3.0.0
    /// @avail SFW, CHS
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre The current credit model has been set to PA with API set_moodys_credit_model_settings().
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] number The number of thread for PA simulation, the value should be in [1, 32]. The default path number is equal to CPU core number.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Invalid input thread number.
    /// @retval  -3 Error - Current credit model is not PA.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "AMEXCAMT");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_PA_SETTINGS, false));
    ///     assert(0 == set_pa_analysis_type(pDeal, PA_LOSS_SIMULATION));
    ///
    ///     assert(0 == set_pa_thread_count(pDeal, 5));
    ///
    ///     assert(0 == run_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_pa_thread_count(void * tid, int number);

	/// This function will enable sfw delinquency projection.
    ///
    /// @since 3.0.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] is_enabled The flag indicate whether using default delinquency assumption.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "AMEXCAMT");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     assert(0 == enable_sfw_delinq_projection(pDeal, 1));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    long CHASAPI enable_sfw_delinq_projection(void * tid, BOOLYAN is_enabled);

	/// This function will get a detail rating agency information for a bond.
    ///
    /// @since 3.0.0
    /// @avail CDOnet,SFW
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid A pointer to the name of the bond.
    /// @param[in] agency Rating agency of bond. Should be one of ::RATING_AGENCY.
    /// @param[out] rating The rating agency information. A pointer to the user allocated structure holding bond rating agency information.
    ///
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Invalid bondid.
    /// @retval -99 Error - For details call get_deal_error_msg()
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "1776");
    ///
    ///     set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
	///		char rating[5] = {0};
	///		memset(rating, 0, sizeof(rating));
    ///     assert(0 == get_bond_rating_by_tranche(pDeal, "A2", MOODYS_CURRENT, rating));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI get_bond_rating_by_tranche(void * tid, const char * bondid, RATING_AGENCY agency, char * rating);

	/// This function will set the nonperf assert recovery.
    ///
    /// @since 3.0.0
    /// @avail CDONET
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] use_indiv_recovery_nonperf If use_indiv_recovery_nonperf is set to TRUE, individual recovery rate under each asset will be applied. If use_indiv_recovery_nonperf is set to FALSE, the value of input parameter nonperf_recovery_rate will be applied.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "1776");
    ///
    ///     set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == set_indiv_recovery_nonperf(pDeal, false));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_indiv_recovery_nonperf(void *tid, BOOLYAN use_indiv_recovery_nonperf);

	/// This method sets whether to expose the loans with zero face value in "REINV" pool group. If input parameter use_zero_loan is TRUE, API will read not only the loans with non-0 face value, but also loans with zero face value in "REINV" pool group; if it is FALSE, API will only read the loans with non-0 face value.
    ///
    /// @since 3.0.0
    /// @avail CDONET
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] populate_reinv_loan If use_zero_loan is set to TRUE, loans with zero face value in "REINV" pool group will be read. If use_zero_loan is set to FALSE, only loans with non-0 face value will be read.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "1776");
    ///
    ///     set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     assert(0 == enable_reinv_loan(pDeal, true));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI enable_reinv_loan(void *tid, BOOLYAN populate_reinv_loan);

	/// This function will Enable whether to pass loan level cashflow to WSAAPI or not.
    ///
    /// @since 3.1.0
    /// @avail SFW
    ///
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] enable_loan_cf "True" means enable MPA passing loan cashflow to WSAAP,otherwise not.
    ///
    /// @retval   0 Success.
	/// @retval  -2 Error - Current calculation level is not CALC_LEVEL_FULL_WITH_LOAN.
	/// @retval  -3 Error - set_whole_loan() has not been called.
	/// @retval  -4 Error - Current credit model is not MPA.
	/// @retval  -5 Error - Other errors.
	/// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
	///     void* pDeal= NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///
    ///     std::vector<WHOLE_LOAN_STRUCT> loans;
    ///     // set informations for each loan in vector loans
    ///     set_whole_loan(pDeal, &loans.front(), 10, 20160101);
    ///
    ///     set_deal_calc_level(pDeal, CALC_LEVEL_FULL_WITH_LOAN, 1);
    ///     set_moodys_credit_model_settings(pDeal, MOODYS_MPA_SETTINGS, false);
    ///     set_mpa_analysis_type(pDeal, MPA_MEDC_SINGLE_PATH);
    ///
    ///     set_mpa_loan_cashflow(pDeal, true);
    ///
    ///     run_deal_ex(pDeal, pCmo);
    ///
    ///     close_deal_ex(pDeal, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_mpa_loan_cashflow(void *tid, BOOLYAN enable_loan_cf);

	/// This function will Enable setting Sinkfund schedule to CDONET bank loan.
    ///
    /// @since 3.2.0
    /// @avail CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] loan_number The 1-based index of the loan.
	/// @param[in] sink_fund_info Sink fund info structure.
    ///
    /// @retval   0 Success.
	/// @retval  -1 Error - Set_whole_loan() not called.
	/// @retval  -2 Error - Invalid loan number.
	/// @retval  -3 Error - Sizes of three vectors are not valid.
	/// @retval  -4 Error - Other error.
	/// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
	///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "STATICLO");
    ///
    ///     set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(tid, pCmo));
    ///
    ///     WHOLE_LOAN_SINK_FUND sink_fund_info;
    ///     sink_fund_info.size = 14;
    ///     memset(sink_fund_info.pdate, 0, sizeof(int));
    ///     memset(sink_fund_info.pprin, 0, sizeof(double));
    ///     int date[]={20150701,20151001,20160101,20160401,20160701,20161001,20170101,20170401,20170701,20171001,20180101,20180401,20180701,20181001};
    ///     double prin[]={0,0,8000,8000,8000,8000,8000,8000,8000,8000,8000,8000,8000,8000};
    ///     for(int i=0; i < sink_fund_info.size; i++)
    ///     {
    ///         sink_fund_info.pdate[i] = date[i];
    ///         sink_fund_info.pprin[i] = prin[i];
    ///     }
    ///     assert(0 == set_loan_schedule(tid, 1, &sink_fund_info));
    ///
    ///     assert(0 == run_deal_ex(tid, pCmo));
    ///     assert(0 == close_deal_ex(tid, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_loan_schedule(void *tid, long loan_number, WHOLE_LOAN_SINK_FUND* sink_fund_info);

	/// Set the default amount calculation method for whole loan analyzer.Except mode "NORMAL", other modes also need "cumulative default rate" and "cumulative default pattern" being set as well.
    ///
    /// @since 3.1.0
    /// @avail SFW
    ///
    /// @pre set_whole_loan() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] type_index The specified method type, should be enums of WHOLE_LOAN_DEFAULT_METHOD_TYPE.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - set_whole_loan() not called.
	/// @retval  -2 Error - Other error.
	/// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
	///     void* ptid= NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///
    ///     std::vector<WHOLE_LOAN_STRUCT> loans;
    ///     // set informations for each loan in vector loans
    ///     set_whole_loan(ptid, &loans.front(), 10, 20160101);
    ///
    ///     set_deal_calc_level(ptid, CALC_LEVEL_FULL_WITH_LOAN, 1); 
    ///
    ///     // set whole loan default method.
    ///     WHOLE_LOAN_DEFAULT_METHOD_TYPE type_index= DEFAULT_PATTERN_NONBINARY;
    ///     int ret = set_whole_loan_default_method(ptid, type_index);
    ///
    ///     run_deal_ex(ptid, pCmo);
    ///     close_deal_ex(ptid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_whole_loan_default_method(void *tid, WHOLE_LOAN_DEFAULT_METHOD_TYPE type_index);

	/// Set the cumulative default rate for loans/mortgages in whole loan analyzer. 
	/// Under DEFAULT_PATTERN_BINARY mode, the cumulative default rate is at the portfolio level, representing the total portion of the starting PORTFOLIO balance that will default throughout the whole projection; 
	/// Under DEFAULT_PATTERN_NONBINARY mode, it is at the per-loan level, representing the total portion of the starting LOAN balance that will default throughout the whole projection. 
	/// The cumulative default rate setting only works at DEFAULT_PATTERN_BINARY and DEFAULT_PATTERN_NONBINARY mode, which can be set by set_whole_loan_default_method().
    ///
    /// @since 3.1.0
    /// @avail SFW
    ///
    /// @pre set_whole_loan() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] val The cumulative rate.
	/// @param[in] loan_num The 1-based index of the loan or -1 to apply to all collateral in the deal.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - set_whole_loan() not called.
	/// @retval  -2 Error - Invalid loan number.
	/// @retval  -3 Error - Other error.
	/// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
	///     void* ptid= NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///
    ///     std::vector<WHOLE_LOAN_STRUCT> loans;
    ///     // set informations for each loan in vector loans
    ///     set_whole_loan(ptid, &loans.front(), 10, 20160101);
    ///
    ///     set_deal_calc_level(ptid, CALC_LEVEL_FULL_WITH_LOAN, 1); 
    ///
    ///     // set whole loan cumulative rate.
    ///     int ret = set_whole_loan_cumulative_rate(ptid, 0.25, -1);
    ///
    ///     run_deal_ex(ptid, pCmo);
    ///     close_deal_ex(ptid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_whole_loan_cumulative_rate(void *tid, double val, long loan_num);

	/// Set the cumulative default pattern for whole loan analyzer.
    ///
    /// @since 3.1.0
    /// @avail SFW
    ///
    /// @pre set_whole_loan() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] vector_length The length of the vector pointed to by pval.
	/// @param[in] pval A pointer to the default pattern(timing). Value for current period (0-indexed element) will not be applied.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - set_whole_loan() not called.
	/// @retval  -2 Error - Other error.
	/// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
	///     void* ptid= NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///
    ///     std::vector<WHOLE_LOAN_STRUCT> loans;
    ///     // set informations for each loan in vector loans
    ///     set_whole_loan(ptid, &loans.front(), 10, 20160101);
    ///
    ///     set_deal_calc_level(ptid, CALC_LEVEL_FULL_WITH_LOAN, 1); 
    ///
    ///     // set whole loan default timing.
    ///     double value = 0.1;
    ///     int ret = set_whole_loan_default_timing(ptid, 1, &value);
    ///
    ///     run_deal_ex(ptid, pCmo);
    ///     close_deal_ex(ptid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
	/// @note Vector_length input longer than the actual size of pval would cause projection issue.
	///       The first element value would not be used in pval in projection; so recommended to set to "0".
	int CHASAPI set_whole_loan_default_timing(void *tid, short vector_length, double *pval);

	/// This function obtains a pointer to the internal to the WSA API collateral iterator. This pointer should be passed to 
    ///     consecutive calls to get_next_collat_ex() to retrieve collateral information.
    ///
    /// Keep in mind that the second call to this function for the same deal will invalidate all
    /// the collateral pointers retrieved from the WSA API during the first call.
    /// 
    /// @since 3.0.0
    /// @avail CDOnet, SFW, CHS
    ///
	/// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier.
    /// @param[in] reremic_deal_id_or_null Pass 0 for main deal or remic name for collateral of the child deal 
    /// 
    /// @retval   Pointer to be passed to get_next_collat_ex() function
    /// @retval   0 Error
    /// 
    /// @sample
	///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "STATICLO");
    ///
    ///     set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(tid, pCmo));
    ///
    ///     MOODYS_POOL_INFO* coll_info =0;
    ///     void* coll_it =obtain_collat_iterator_ex(tid, 0);
    ///     if(coll_it == 0)
    ///     {
    ///         std::cout << "Failure to start collat iteration " << get_deal_error_msg(tid) << std::endl;
    ///     }
    ///     while(coll_info =  get_next_collat_ex(tid,coll_it))
    ///     {   
    ///         // do what you need with collateral
    ///     }   
    ///
    ///     assert(0 == close_deal_ex(tid, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    /// 
	void *CHASAPI obtain_collat_iterator_ex(void *tid, const char *reremic_deal_id_or_null);

	/// This function gets next collateral information using iterator obtained from calling obtain_collat_iterator_ex().
    /// When iterator goes to the end of the collateral set, it returns NULL.
    ///
    /// @since 3.0.0
    /// @avail CDOnet, SFW, CHS
    ///
	/// @pre open_deal_ex() has been called.
    /// @pre obtain_collat_iterator_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier
    /// @param[in] collat_iterator Pointer to collateral iterator returned by calling obtain_collat_iterator_ex().
    /// 
    /// @retval Pointer Pointer to the next collateral information
    /// @retval   0 No more information left
    /// 
    /// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "STATICLO");
    ///
    ///     set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(tid, pCmo));
    ///
    ///     MOODYS_POOL_INFO* coll_info =0;
    ///     void* coll_it =obtain_collat_iterator_ex(tid, 0);
    ///     if(coll_it == 0)
    ///     {
    ///         std::cout << "Failure to start collat iteration " << get_deal_error_msg(tid) << std::endl;
    ///     }
    ///     while(coll_info =  get_next_collat_ex(tid,coll_it))
    ///     {
    ///         // do what you need with collateral
    ///     }
    ///
    ///     assert(0 == close_deal_ex(tid, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    /// 
    /// @note Function returns pointers to collateral information allocated by the API.  
    ///     These pointers will be valid until deal is closed or another call to obtain_collat_iterator_ex()
    ///     function is made using the same parameters.
    ///     The iterator will be released when close_deal_ex() is called. The iterator will be overwritten when obtain_collat_iterator_ex() is called again.
    /// 
	MOODYS_POOL_INFO *CHASAPI get_next_collat_ex(void *tid, void * collat_iterator);

	/// Retrieves the additional desctiptive bond information by bondid
    ///
    /// @exptl Subject to change
    /// @since 3.0.0
    /// @avail CDOnet, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null Pass NULL for main deal or remic name for underlying deal.
	/// @param[in] bondid A pointer to the name of the bond.
    /// @param[out] bond_info Pointer to the structure holding bond info.
    ///
    /// @retval    0 No error
    /// @retval   -1 Error - Deal not opened
    /// @retval  -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     MOODYS_BOND_INFO result;
    ///     assert(0 == get_bond_info_by_tranche_ex(pDeal, NULL, "A1", &result));
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI get_bond_info_by_tranche_ex(void *tid, const char *reremic_deal_id_or_null, const char *bondid, MOODYS_BOND_INFO *bond_info);

	/// Retrieves the additional desctiptive bond information by index
    ///
    /// @exptl Subject to change
    /// @since 3.0.0
    /// @avail CDOnet, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null Pass NULL for main deal or remic name for underlying deal.
	/// @param[in] index The 1-based index of the bond in the array of bonds.
    /// @param[out] bond_info Pointer to the structure holding bond info.
    ///
    /// @retval    0 No error
    /// @retval   -1 Error - Deal not opened
    /// @retval  -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     MOODYS_BOND_INFO result;
    ///     assert(0 == get_bond_info_by_index_ex(pDeal, NULL, 1, &result));
    ///     
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI get_bond_info_by_index_ex(void *tid, const char *reremic_deal_id_or_null, int index, MOODYS_BOND_INFO *bond_info);

    /// Sets simulation path interest rate that will be used for the specified currency and index.
    ///
    /// @since 3.0.0
    /// @avail CDOnet, SFW, CHS
    ///
    /// @pre set_metrics_input_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] currency The ISO name of the currency of the requested market index.
    /// @param[in] idx A pointer to the index to set. *idx must be one of: 
    ///         - enums of ::INDEX_TYPE (In indextypes.h).
    ///         - enums of ::INDEX_TYPE_EX (SFW and CDOnet deals).
	/// @param[in] num_paths Number of paths in the user input.
    /// @param[in] rate_size The vector length of each path in two-dimensional array idx_val.
    /// @param[in] idx_val A pointer to a two-dimensional array of interest rates in decimal.
    ///
    /// @retval   0 No error.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Invalid currency ISO name.
    /// @retval  -3 Error - Invalid index to market index.
    /// @retval  -4 Error - Invalid path number.
	/// @retval  -5 Error - Invalid rate vector size of each path.
    /// @retval  -6 Error - Invalid rate pointer.
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     //deal has been opened.
	///
	///     const int path_number = 100;     
	///     METRIC_INPUT_STRUCT_EX metric_input_ex;
	///     memset(&metric_input_ex, 0, sizeof(METRIC_INPUT_STRUCT_EX));
	///     metric_input_ex.shift_amt = 0.005;
	///     metric_input_ex.num_paths = path_number;
	///     metric_input_ex.oas_mode = ENABLE_ALL;
	///     assert(0 == set_metrics_input_ex(pDeal, &metric_input_ex));
    ///
    ///     const int rate_size = 200;
    ///     double **pVal = new double*[path_number];
    ///     for (int i = 0; i < path_number; ++i)
    ///     {
    ///         pVal[i] = new double[rate_size];
    ///     }
    ///     //Fill the index rate values for each path
	///
	///     short idx = LIBOR_3;
	///     assert(0 == set_index_rate_ex(pDeal, "USD", &idx, path_number, rate_size, pVal));
	///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
	///     
	///     for (int i = 0; i < path_number; ++i)
	///         delete pVal[i];
	///     delete[] pVal;
    /// @endsample
    ///
    int CHASAPI set_index_rate_ex(void *tid, const char* currency, short* idx, int num_paths, short rate_size, double** idx_val);

	/// Sets spot spread rate that will be used for the calculation of bank loan call period.
    ///
    /// @since 3.2.0
    /// @avail CDOnet
    ///
	/// @pre set_metrics_input_ex(), set_whole_loan() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] currency The ISO name of the currency of the requested market index.
    /// @param[in] rating_type ESG rating type.
    /// @param[in] term_type ESG rating Term.
	/// @param[in] num_paths Number of paths in the user input.
    /// @param[in] rate_size The vector length of each path in two-dimensional array idx_val.
    /// @param[in] idx_val A pointer to a two-dimensional array of interest rates in decimal.
    ///
    /// @retval   0 No error.
    /// @retval  -2 Error - Invalid currency ISO name.
    /// @retval  -4 Error - Invalid path number.
	/// @retval  -5 Error - Invalid rate vector size of each path.
    /// @retval  -6 Error - Invalid rate pointer.
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     //whole loan has been set.
	///
	///     const int path_number = 100;     
	///     METRIC_INPUT_STRUCT_EX metric_input_ex;
	///     memset(&metric_input_ex, 0, sizeof(METRIC_INPUT_STRUCT_EX));
	///     metric_input_ex.shift_amt = 0.005;
	///     metric_input_ex.num_paths = path_number;
	///     metric_input_ex.oas_mode = ENABLE_ALL;
	///     assert(0 == set_metrics_input_ex(pDeal, &metric_input_ex));
    ///
    ///     const int rate_size = 200;
    ///     double **pVal = new double*[path_number];
    ///     for (int i = 0; i < path_number; ++i)
    ///     {
    ///         pVal[i] = new double[rate_size];
    ///     }
    ///     //Fill the index rate values for each path
	///
	///     ESG_RATING_TYPE rating_type = ESG_RATING_B;
	///     ESG_RATING_TERM term_type = ESG_TERM_3M;
	///     assert(0 == set_spot_spread(pDeal, "USD", rating_type, term_type, path_number, rate_size, pVal));
	///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
	///     
	///     for (int i = 0; i < path_number; ++i)
	///		    delete pVal[i];
	///     delete[] pVal;
    /// @endsample
    ///
	int CHASAPI set_spot_spread(void *tid, const char* currency, ESG_RATING_TYPE rating_type, ESG_RATING_TERM term_type, int num_paths, short rate_size, double **idx_val);

    /// Run the deal for the FFIEC test
    ///
    /// @since 3.2.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
	/// @param[in] prepay_type The input prepay type for FFIEC test, must be one of prepayment type.
	/// @param[in] prepay_rates The input prepay rate for FFIEC test scenarios, it is a pointer to the prepay rates array whose length must be 7. If input NULL, FFIEC tests run deal with no prepayments.
    
    /// @retval    0 No error
    /// @retval   -1 Error - Deal not opened
    /// @retval  -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    /// 
    /// @note Current number of FFIEC scenarios is 7,1~7 means interest rate +300, +200, +100, +0, -100, -200 , -300bps.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     //deal has been opened.
    ///     run_FFIEC_test(pDeal, PREPAY_CURVE_SMM, NULL);
    /// @endsample
    ///
    int CHASAPI run_FFIEC_test(void *tid, int prepay_type, double* prepay_rates);

    /// Get the FFIEC test results for the specified bond
    ///
    /// @since 3.2.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// @pre run_FFIEC_test() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
	/// @param[in] bondid A pointer to the name of the bond.
	/// @param[in] FFIEC_inputs FFIEC input information, if NULL, FFIEC test results would base on NO_BENCH_MODE return.
    /// @param[out] FFIEC_results A user allocated array which stores the FFIEC results returned, the array length must be at least 7.

    /// @retval    0 No error
    /// @retval   -1 Error - Deal not opened
    /// @retval  -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @note Current number of FFIEC scenarios is 7,1~7 means interest rate +300, +200, +100, +0, -100, -200 , -300bps. The FFIEC_results input array length must be at least 7.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     //deal has been opened.
    ///
    ///     run_FFIEC_test(pDeal, PREPAY_CURVE_SMM, NULL);
    ///     FFIEC_RESULTS FFIECResults[7];
    ///     memset(FFIECResults, sizeof(FFIEC_RESULTS) * 7);
    ///     get_bond_FFIEC_results(pDeal, pCmo->bond.stripped_id, NULL, FFIECResults);
    /// @endsample
    ///
    int CHASAPI get_bond_FFIEC_results(void *tid, const char *bondid, FFIEC_INPUT_PARAMS* FFIEC_inputs, FFIEC_RESULTS FFIEC_results[]);

    /// This method overrides the date of non-call end with input non_call_end_date; If not called, the date of non-call end will still use the date of non-call end in deal file.
    ///
    /// @since 3.0.0
    /// @avail CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] non_call_end_date This input will be used to override the non_call_end_date, format "YYYYMMDD".
    ///
    /// @retval    0 No error
    /// @retval   -1 Error - Deal not opened
    /// @retval   -2 Error - Invalid date value
    /// @retval  -99 Error - Other error
    ///
    /// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT cmos;
    ///     memset(&cmos, 0, sizeof(cmos));
    ///     strcpy(cmo.dealid, "CQSCLO2");
    ///
    ///     assert(0 == open_deal_ex(tid, &cmos));
    /// 
    ///     assert(0 == set_non_call_end(tid, 20120328));
	///
    ///     assert(0 == close_deal_ex(tid, &cmos));
    /// @endsample
    ///
    int CHASAPI set_non_call_end(void *tid, int non_call_end_date);

    /// Expose next reset date.
    ///
    /// @since 3.0.0
    /// @avail SFW, CDOnet
    ///
    /// @pre run_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid The NULL terminated string indicate the name of the tranche whose results are being requested.
    /// @param[out] next_reset_date The value of next reset date, format "YYYYMMDD".
    ///
    /// @retval    0 No error
    /// @retval   -1 Error - Deal not opened
    /// @retval   -2 Error - Bond not found
    /// @retval   -3 Error - Next reset date is not available
    /// @retval  -99 Error - Other error
    ///
    /// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT cmos;
    ///     memset(&cmos, 0, sizeof(cmos));
    ///     strcpy(cmo.dealid, "CQSCLO2");
    ///
    ///     assert(0 == open_deal_ex(tid, &cmos));
    ///     assert(0 == run_deal_ex(tid, &cmos));
    /// 
    ///     int next_reset_date = 0;
    ///     assert(0 == get_bond_next_reset_date(tid, "B", &next_reset_date));
	///
    ///     assert(0 == close_deal_ex(tid, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI get_bond_next_reset_date(void *tid, const char* bondid, int* next_reset_date);

    /// Setup ESG model simulation interest rates, for OAS calculation, it would depends on the ESG model simulation interest rates.
    ///
    /// @since 3.0.0
    /// @avail All
    ///
    /// @pre set_input_path() has been called.
    ///
    /// @param[in] esg_inputs ESG model inputs param.
    /// @param[in] esg_currency_inputs An array user allocated of the ESG currencies rates inputs.
    /// @param[in] esg_currency_inputs_size Array size of esg_currency_inputs
    ///
    /// @retval    0 success, ESG output rates generated success.
    /// @retval  < 0 Error - error
    ///
    /// @sample
    ///    ESG_MODEL_INPUTS esgParam;
    ///    memset(&esgParam, 0, sizeof(ESG_MODEL_INPUTS));
    ///    strcpy(esgParam.ESGbhmPath,"C:\\installer\\8.5.0\\Models");
    ///    strcpy(esgParam.ESGLicenseFilePath,"C:\\installer\\Moody's Analytics-Internal Master Licence - For Staff Use Only-030717-011017.lic");
    ///    strcpy(esgParam.ESGCalibrationFilePath,"C:\\installer\\lmmp_testcalib_a.bhc");
    ///    strcpy(esgParam.ESGRatesOutputPath,"C:\\temp");
    ///    esgParam.YYYYMMDD = 20170801;
    ///    esgParam.SimulationPaths = 10;
    ///    esgParam.Periods = 500;
    ///    ESG_CURRENCY_RATE_INPUTS esgRateInput[2];
    ///    memset(esgRateInput, 0, sizeof(ESG_CURRENCY_RATE_INPUTS)*2);
    ///    strcpy(esgRateInput[0].Currency, "GBP");
    ///    strcpy(esgRateInput[1].Currency, "USD");
    ///    set_up_ESG_model_interest_rates(&esgParam, esgRateInput, 2);
    /// @endsample
    /// @note The order of the currencies array inputs may affect the ESG simulation output rates. The first currency in esg_currency_inputs would be the base economy of the ESG simulation.
    /// @warning ESG generates even number of paths for improved accuracy of the interest rate simulation. The last interest rate path from ESG would be ignored in OAS analysis if user sets ESG_MODEL_INPUTS::SimulationPaths to an odd number.
    int CHASAPI set_up_ESG_model_interest_rates(ESG_MODEL_INPUTS* esg_inputs, ESG_CURRENCY_RATE_INPUTS esg_currency_inputs[], int esg_currency_inputs_size);

    /// Set market rates in decimal not specific to a deal. Any interest rate set by this function overrides the corresponding interest rate from MWSA rate DB if available.
    ///
    /// @since 3.1.0
    /// @avail SFW, CDOnet, CHS
    ///
    /// @pre None.
    ///
    /// @param[in] currency The ISO currency code of the specified interest rates (e.g., "USD").
    /// @param[in] rate_size Number of interest rates to be specified.
    /// @param[in] rate_types Array of interest rate types to be specified. Any element in *rate_types must be a TREASURY or LIBOR rate and one of: enums of INDEX_TYPE (In indextypes.h) or enums of INDEX_TYPE_EX (SFW and CDOnet deals).
    /// @param[in] rate_values Array of interest rate values corresponding to elements in rate_types.
    ///
    /// @retval    0 Success.
    /// @retval   -1 Error - Currency code not supported.
    /// @retval   -2 Error - At least one element in rate_types is invalid.
    /// @retval  -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg().
    ///
    /// @sample
    ///    short rate_types[] = { LIBOR_60, TSY_240, SVR };
    ///    double rate_values[] = { 0.0217, 0.025, 0.00053 };
    ///    assert(0 == set_global_rates("USD", 3, rate_types, rate_values));
    /// @endsample
    ///
    int CHASAPI set_global_rates(const char* currency, short rate_size, short* rate_types, double* rate_values);

    /// Set FRA.
    ///
    /// @since 3.0.0
    /// @avail SFW, CDOnet, CHS
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] currency The ISO name of the currency of the requested market index.
    /// @param[in] rate_type Type of interest rate. Available inputs: "LIBOR".
    /// @param[in] start_month Number of months until contract effective date.
    /// @param[in] end_month Number of months until contract termination date.
    /// @param[in] rate_value Rate value in decimal.
    ///
    /// @retval    0 No error
    /// @retval   -1 Error - Deal not opened
    /// @retval   -2 Error - Currency code not supported
    /// @retval   -3 Error - Type of interest rate not supported.
    /// @retval  -99 Error - Other error
    ///
    /// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT cmos;
    ///     memset(&cmos, 0, sizeof(cmos));
    ///     strcpy(cmos.dealid, "CQSCLO2");
    ///
    ///     assert(0 == open_deal_ex(tid, &cmos));
    ///
    ///     assert(0 == set_FRA(tid, "USD", "LIBOR", 5, 9, 0.012));
	///
    ///     assert(0 == close_deal_ex(tid, &cmos));
    /// @endsample
    ///
	int CHASAPI set_FRA(void * tid, const char* currency, const char* rate_type, short start_month, short end_month, double rate_value);

    /// Generate forward interest rate curves from user-input market rates.
    ///
    /// @since 3.0.0
    /// @avail SFW, CDOnet, CHS
    ///
    /// @pre set_index_rate(), load_MWSA_rates() or set_rate_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    ///
    /// @retval    0 Success.
    /// @retval   -1 Insufficient market rate information for [currency] [rate type]. Minimum inputs are [...].
    /// @retval  -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "CQSCLO2");
    ///
    ///     open_deal_ex(tid, pCmo);
    ///
    ///     short idx = LIBOR_1;
    ///     double rate = 0.00853;
    ///     set_index_rate(pDeal, "USD", &idx, 0, &rate);
    ///
    ///     generate_forward_interest_rates(tid);
    ///     double *pRate = get_forward_interest_rates(tid, "USD", &idx);
	///
    ///     assert(0 == close_deal_ex(tid, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI generate_forward_interest_rates(void * tid);

    /// Get the forward interest rate curves after generate_forward_interest_rates() is successfully called.
    ///
    /// @since 3.0.0
    /// @avail SFW, CHS, CDONET
    ///
    /// @pre generate_forward_interest_rates() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] currency ISO currency code (e.g., USD).
    /// @param[in] rate_type Type of interest rate. *rate_type must be one of: enums of INDEX_TYPE (In indextypes.h). enums of INDEX_TYPE_EX (SFW and CDOnet deals).
    ///
    /// @retval  NULL Forward curve for the requested interest rate is not available.
    /// @retval Other A pointer to an array which stores the forward curve for the requested interest rate. The array length is MAX_PERIODS(=612).
    ///
    /// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "CQSCLO2");
    ///
    ///     open_deal_ex(tid, pCmo);
    ///
    ///     short idx = LIBOR_1;
    ///     double rate = 0.00853;
    ///     set_index_rate(pDeal, "USD", &idx, 0, &rate);
    ///     generate_forward_interest_rates(tid);
    ///
    ///     double *pRate = get_forward_interest_rates(tid, "USD", &idx);
	///
    ///     assert(0 == close_deal_ex(tid, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    double* CHASAPI get_forward_interest_rates(void * tid, const char* currency, short* rate_type);

	/// Get asset type list from WSAAPI.DB.
    ///
    /// @since 3.1.0
    /// @avail SFW, CHS, CDONET
    ///
    /// @param[out] asset_type_list. A client-allocated char array for the asset type information which will be stored, Null if first call. 
    /// @param[out] err_buffer. Buffer to get error message content.
    /// @param[in] err_length. The length of err_buffer defined by user.
    ///
    /// @retval  >=0 Actual number of asset type returned.
    /// @retval  -1 Error - DB not found.
    /// @retval -99 Error - Please examine err_buffer for error.
    ///
    /// @sample
    ///     char err_buffer[200] = "";
    ///     int err_length = 200;
    ///     int asset_number = get_asset_type_list(NULL, err_buffer, &err_length);  // first call to get number of asset type
	///     if (asset_number > 0)
	///     {		
	///         std::vector<char*> asset_type_list(asset_number);
	///         std::vector<char> asset_type_list_buf(asset_number*200);
	///         for(int i = 0; i < asset_number; i++) asset_type_list[i] = &asset_type_list_buf[i*200];
	///         get_asset_type_list(&asset_type_list.front(), err_buffer, err_length);  // second call to get asset_type_list
	///     }
    /// @endsample
    /// @note Pass NULL for asset_type_list to get the number of asset type on first call. And then get asset_type_list on second call.
    ///     
	int CHASAPI get_asset_type_list(char* asset_type_list[], char* err_buffer, int err_length);

    /// This function calculates cashflow analytics for a given loan. It should be called after running cashflow
    /// 
    /// @since 3.2
    /// @avail CDOnet, SFW
    ///
    /// @pre run_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] loan_number The 1-based index of the loan or -1 to apply to all collateral in the deal.
    /// @param[in] anchorType Type of anchor for pricing, available options: PRICE, YIELD, or DM
    /// @param[in] anchorValue Value of the provided pricing anchor
    /// @param[out] results Calculated loan analytics
    /// 
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open
    /// @retval  -2 Error - Current calculation level is not CALC_LEVEL_FULL_WITH_LOAN
	/// @retval  -3 Error - run_deal_ex() has not been called
    /// @retval -99 Error - Other error, use get_deal_error_msg() to see details.
	///
	/// @sample
    ///    void* tid = NULL;
    ///    CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///    memset(pCmo, 0, sizeof(*pCmo));
    ///    strcpy(pCmo->dealid, "DRYDEN34");
    ///
    ///    set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
	///    assert(0 == open_deal_ex(tid, pCmo));
    ///
	///    set_deal_calc_level(pDeal, CALC_LEVEL_FULL_WITH_LOAN, 1);
    ///    assert(0 == run_deal_ex(tid,pCmo));
    ///
    ///    double dAnchor             = 2.0000;
    ///    PRICING_ANCHORS anchorType = YIELD;
    ///    PRICING_RESULTS results;
    ///    memset(&results, 0x00, sizeof(PRICING_RESULTS));
    ///    int nRet = price_loan(tid, 1, anchorType, dAnchor, &results);
    ///    if(nRet !=0)
    ///    {
    ///    	   //error handle;
    ///    }
	///
    ///    assert(0 == close_deal_ex(tid, pCmo));
    ///    delete pCmo;
    ///    pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI price_loan(void * tid, int loan_number, PRICING_ANCHORS anchorType, double anchorValue, PRICING_RESULTS * results);

	/// This extended function calculates cashflow analytics for a given loan. It should be called after running cashflow
    /// 
    /// @since 3.2
    /// @avail CDOnet, SFW
    ///
    /// @pre run_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] loan_number The 1-based index of the loan or -1 to apply to all collateral in the deal.
    /// @param[in] pricing_param_input structure of price loan input information.
    /// @param[out] results Calculated bond analytics
    /// 
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open
    /// @retval  -2 Error - Current calculation level is not CALC_LEVEL_FULL_WITH_LOAN
	/// @retval  -3 Error -  invalid loan number
	/// @retval  -4 Error -  invalid rate index
    /// @retval -99 Error - Other error, use get_deal_error_msg() to see details.
	///
	/// @sample
    ///    void* tid = NULL;
    ///    CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///    memset(pCmo, 0, sizeof(*pCmo));
    ///    strcpy(pCmo->dealid, "DRYDEN34");
    ///
    ///    set_engine_preference(PICK_CDONET_ENGINE_FOR_MAPPED_DEALS);
	///    assert(0 == open_deal_ex(tid, pCmo));
    ///
	///    set_deal_calc_level(pDeal, CALC_LEVEL_FULL_WITH_LOAN, 1);
    ///    assert(0 == run_deal_ex(tid,pCmo));
    ///
    ///    LOAN_PRICING_INPUT pricing_input;
    ///    // set informations for the pricing_input
    ///    PRICING_RESULTS results;
    ///    memset(&results, 0x00, sizeof(PRICING_RESULTS));
    ///    int nRet = price_loan_ex(tid, 1, pricing_input, &results);
    ///    if(nRet !=0)
    ///    {
    ///    	   //error handle;
    ///    }
	///
    ///    assert(0 == close_deal_ex(tid, pCmo));
    ///    delete pCmo;
    ///    pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI price_loan_ex(void * tid, int loan_number, LOAN_PRICING_INPUT pricing_param_input, PRICING_RESULTS * results);
    
    /// This function gets next reset date for a given loan.
    /// 
    /// @since 3.3
    /// @avail CDOnet
    ///
    /// @pre run_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] loan_number The loan identifier.
    /// @param[out] next_reset_date The next reset date of specified loan
    /// 
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Invalid loan number.
	/// @retval  -3 Error - Next reset date is not available.
    /// @retval -99 Error - Other error, use get_deal_error_msg() to see details.
	///
	/// @sample
    ///    void* tid = NULL;
    ///    CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///    memset(pCmo, 0, sizeof(*pCmo));
    ///    strcpy(pCmo->dealid, "DRYDEN34");
    ///
	///    assert(0 == open_deal_ex(tid, pCmo));
    ///    assert(0 == run_deal_ex(tid,pCmo));
    ///
	///    int next_reset_date = 0;
    ///    int nRet = get_loan_next_reset_date(tid, 1, &next_reset_date);
    ///    if(nRet !=0)
    ///    {
    ///    	   //error handle;
    ///    }
	///
    ///     assert(0 == close_deal_ex(tid, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI get_loan_next_reset_date(void *tid, int loan_number, int* next_reset_date);
	
    /// Set the loan extension assumptions for CMBS deal.
    ///
    /// @since 3.1.0
    /// @avail SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] use_default On/off switch suggesting if apply CMBS default loan extension assumption , 0 for "No", Non-0 stands for "Yes".
    /// @param[in] apply_flag On/off switch suggesting if apply CMBS loan extension assumption, 0 for "Not Use", Non-0 stands for "Use".
    /// @param[in] non_perf_loan On/off switch suggesting if CMBS loan extension assumption applies to non performing loan ,1 for on and 0 for off. 
    /// @param[in] maturity_cutoff Apply the assumption if a loan's maturity is before the cutoff date ,format "YYYYMMDD".
    /// @param[in] extend_years Number of years to extend.
    /// @param[in] edf_threshold Apply the extention assumption if edf of the loan is over the threshold, should be value between 0 and 1.0. 
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Not a CMBS deal.
    /// @retval  -3 Error - Invalid maturity_cutoff date.
    /// @retval  -4 Error - Invalid number of years to extend, should be >= 0.
    /// @retval  -5 Error - Invalid threshold, should be between 0 and 1.0.                           
    /// @retval -99 Error - Other error, use get_deal_error_msg() to see details.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "CMBS_CCC070C3");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///     set_service_advances_ex(pDeal, SERVICER_ADVANCES_BOTH, false);
    ///     assert(0 == enable_sfw_delinq_projection(pDeal, false));
    ///     double edf[5] = {.3, 0, 0, 0, 0};
    ///     assert(0 == set_loan_edf(pDeal, NULL, -1, edf, 5));
    ///     
    ///     assert(0 == set_cmbs_loan_extension_assumption(pDeal, false, true, true, 20180630, 3, 0.2));
    ///     
    ///     assert(0 == run_deal_ex(pDeal, pCmo)); // userPd are applied to loan after calling run_deal_ex()
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///     
    int CHASAPI set_cmbs_loan_extension_assumption(void *tid, BOOLYAN use_default, BOOLYAN apply_flag, BOOLYAN non_perf_loan, int maturity_cutoff, int extend_years, double edf_threshold); 

    /// Set the macroeconomic simulation for OAS analysis (where applicable). This function only applies to CMBS using CMM custom scenario for OAS calculation.
    ///
    /// @since 3.3.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
	/// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] country The three-character country codes, please refer to ISO 3166-1 alpha-3.(e.g., "USA").
    /// @param[in] factor_type One of the enums from ::MACROECONOMIC_FACTOR_TYPE.
	/// @param[in] num_paths Number of paths in the user input, it would be the number of rows in two-dimensional array factor_val.
    /// @param[in] val_size The number of columns in two-dimensional array factor_val.
    /// @param[in] factor_val A pointer to a two-dimensional array of quarterly interest rates in decimal.
    ///
    /// @retval   0 No error.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Invalid country ISO name.
    /// @retval  -3 Error - Invalid factor_type.
    /// @retval  -4 Error - Invalid path number.
	/// @retval  -5 Error - Invalid rate vector size of each path.
    /// @retval  -6 Error - Invalid rate pointer.
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     //deal has been opened.
	///
	///     // set path macro economy data
	///     double pVal[3][40];
    ///     double *parray[3] = {pVal[0], pVal[1], pVal[2]};
    ///     double **pp = parray;
	///     for (int i = 0; i < metric_input_ex.num_paths; ++i)
	///     {
	///         for (int j = 0; j < 40; ++j)
	///             pVal[i][j] = 0.004325 + i*0.001 + j*0.00001;
	///     }
	///     short factorType = REALGDPGROWTH;
	///     int iret = set_macroeconomic_factor_ex(pDeal, mdi.country, &factorType, metric_input_ex.num_paths, 40, pp);
    /// @endsample
    ///
    /// @note If the value of num_paths in set_macroeconomic_factor_ex() is smaller than METRIC_INPUT_STRUCT_EX::num_paths, the corresponding vector from set_cmm_custom_scenario() will be repeated for the missing paths. 
	///       If the macro economic factor set for CMM custom scenario, the number of columns in two-dimensional array factor_val must be 40.
	int CHASAPI set_macroeconomic_factor_ex(void* tid, const char* country, short* factor_type, int num_paths, short val_size, double** factor_val);

    /// Set the ECON rate shift setting.
    ///
    /// @since 3.3.0
    /// @avail CDOnet
    ///
    /// @pre open_deal_ex() has been called.
    ///
	/// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] rate_shift Econ rate shifts settings.
    ///
    /// @retval   0 No error.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Apply CDOnet deals only.
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     // deal has been opened.
	///
	///     // set rate shift setting
    ///     RATE_SHIFT_SETTING rate_shift;
    ///     rate_shift.alwaysUseScenRateShift = SCENARIO_NO;
    ///     rate_shift.rateShiftFromSettle = true;
    ///     rate_shift.shiftRelativeToCurrentRates = true;
    ///     set_rate_shift_setting(pDeal, rate_shift);
    /// @endsample    
    int CHASAPI set_rate_shift_setting(void * tid, RATE_SHIFT_SETTING rate_shift);	

    /// Use the actual coupon rate adjusted by day calendar to do cashflow amortization in whole loan analyzer.
    ///
    /// @since 3.4.0
    /// @avail SFW
    ///
    /// @pre set_whole_loan() has been called.
    ///
	/// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] flag_periodic_rate If TRUE the projection would use the periodic coupon rate.
    ///
    /// @retval   0 No error.
    /// @retval  -1 Error - set_whole_loan() was not called.
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg()
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     std::vector<WHOLE_LOAN_STRUCT> loans;
	///     //set informations for each loan in vector loans
    ///     set_whole_loan(pDeal, &loans.front(), 10, 20160101);
	///     assert(0 == enable_periodic_coupon_rate_projection(pDeal, 1));
	///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample  
	///
	/// @note If this set to true, the loan coupon will be modified in pay reset month.
	int CHASAPI enable_periodic_coupon_rate_projection(void * tid, BOOLYAN flag_periodic_rate);

	/// Get interest rates from MWSA DB before open deal. The function looks back as many as 7 calendar days to avoid data availability gaps from weekends and holidays.
    ///
    /// @since 3.3.0
    /// @avail CDOnet, SFW, CHS
    ///
	/// @param[in] file_path Directory where the MWSA DB resides.
    /// @param[in] yyyymmdd User input date with format YYYYMMDD. In case the MWSA DB of YYYYMMDD is not available, look back as many as 7 days.
	/// @param[in] currency The ISO currency code of the specified interest rate (e.g., "USD").
    /// @param[in] idx The rate index type. *idx must be one of:
	///										enums of INDEX_TYPE
	///										enums of INDEX_TYPE_EX.
    ///
    /// @retval  NULL - The requested index rate was not retrieved successfully.
    /// @retval  Other - A pointer to an array which stores the interest rates in decimal from MWSA DB.
    ///
    /// @sample
	///     const char* file_path = "path";
	///     int yyyymmdd = 20170602;
	/// 	const char* currency_sek = "SEK";
	/// 	short idx_libor3m = LIBOR_3;
	/// 	double* sek_libor3m_rates = load_MWSA_rate(file_path, yyyymmdd, currency_sek, &idx_libor3m);
    /// @endsample    
	double* CHASAPI load_MWSA_rate(const char* file_path, int yyyymmdd, const char* currency, short* idx);

	/// Get MA interest rates from MWSA table before open deal. The function looks back as many as 7 calendar days to avoid data availability gaps from weekends and holidays.
    ///
    /// @since 3.7.0
    /// @avail CDOnet, SFW, CHS
    ///
	/// @param[in] file_path Directory where the MWSA DB resides.
    /// @param[in] yyyymmdd User input date with format YYYYMMDD. In case the MWSA DB of YYYYMMDD is not available, look back as many as 7 days.
	/// @param[in] ma_scenario User input scenario type.
	/// @param[in] currency The ISO currency code of the specified interest rate (e.g., "USD").
    /// @param[in] idx The rate index type. *idx must be one of:
	///										enums of INDEX_TYPE
	///										enums of INDEX_TYPE_EX.
    ///
    /// @retval  NULL - The requested index rate was not retrieved successfully.
    /// @retval  Other - A pointer to an array which stores the interest rates in decimal from MWSA DB.
    ///
    /// @sample
	///     const char* file_path = "path";
	///     int yyyymmdd = 20170602;
	/// 	const char* currency_sek = "SEK";
	/// 	short idx_libor3m = LIBOR_3;
	/// 	double* sek_libor3m_rates = load_MA_rate(file_path, yyyymmdd, "BL", currency_sek, &idx_libor3m);
    /// @endsample    
	double* CHASAPI load_MA_rate(const char* file_path, int yyyymmdd, const char * ma_scenario, const char* currency, short* idx);

	/// This function gets the available MA rate shifts scenarios list for the specified date yyyymmdd. The function looks back as many as 7 calendar days to avoid data availability gaps from weekends and holidays.
    ///
    /// @since 3.7.0
    /// @avail CDOnet, SFW, CHS
    ///
    /// @param[in] file_path Directory where the MWSA DB resides.
    /// @param[in] yyyymmdd User input date with format YYYYMMDD. In case the MWSA DB of YYYYMMDD is not available, look back as many as 7 days.
	/// @param[in]  scenario_list The list of scenarios. If this parameter is NULL, this function will return with number of available scenarios.
    ///             The memory of scenario_list should be allocated by user. The lengh of each element of scenario_list should be 20.
    ///
    /// @retval >=0 Success. Number of available scenarios.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     int buflen = 50;
    ///     char value[buflen]={0};
    ///     int scenario_count = get_MA_rate_shifts_scenarios(get_input_path(), 20181101, NULL);
    ///     assert(scenario_count > 0);
    ///     
    ///     std::vector<char *> scenario_list(scenario_count);
    ///     std::vector<char> scenario_strs(scenario_count*20);
    ///
    ///     for (int i = 0; i < scenario_count; ++i)
    ///         scenario_list[i] = &scenario_strs[i*20];
    ///     assert(scenario_count == get_MA_rate_shifts_scenarios(get_input_path(), 20181101, &scenario_list[0]));
    /// @endsample
    int CHASAPI get_MA_rate_shifts_scenarios(const char* file_path, int yyyymmdd, char *scenario_list[]);

	/// This method exposes first loss calculator functionality in WSAAPI
    ///
    /// @since 3.4.1
    /// @avail SFW, CDOnet
	///
	/// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
	/// @param[in] bondid A pointer to the name of the bond.
    /// @param[in] first_loss_input Input information for calculating bond first loss.
	/// @param[out] first_loss_result A pointer to the result of calculate bond first loss.
    /// @retval  0  No error.
    /// @retval  -1 Error - Deal not open.
	/// @retval  -2 Error - Invalid Input for first_loss_input.first_loss_run_mode.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    /// @sample
	///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "TCCT-II");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
	///		FIRST_LOSS_INPUT first_loss_input;
	///		first_loss_input.first_loss_run_mode = FIRST_LOSS_RUN_MODE_LOSS_RATE;
	///		first_loss_input.is_percentage = false;
	///		first_loss_input.prepayment_type = PREPAY_CURVE_CPR;
	/// 	first_loss_input.prepayment_rate = 0.0;
    ///   
	///		first_loss_input.default_type = DEFAULT_CURVE_CDR;
	/// 	first_loss_input.default_rate = 0.0;
	/// 	first_loss_input.forbearance_rate = 0.0;
	/// 	first_loss_input.deferment_rate = 0.0;
	/// 	first_loss_input.first_loss_threshold = FIRST_LOSS_THRESHOLD_INTEREST;
    ///
	/// 	first_loss_input.principal_payment_rate = 0;
	/// 	first_loss_input.monthly_purchase_rate = 0;
	/// 	first_loss_input.portfolio_yield = 0;
	/// 	first_loss_input.loss_rate = 0;
	///
	/// 	first_loss_input.principal_payment_rate = 0;
	/// 	first_loss_input.prin_loss_serverity = 0.0;
    ///
	/// 	FIRST_LOSS_RESULT first_loss_result; 
    ///     assert(0 == calculate_bond_first_loss(pDeal, "1601-A", first_loss_input, &first_loss_result));
    ///
	///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
	/// 
	/// @note For CDONET reremic deal, this function only takes top deal into calculation.
    int CHASAPI calculate_bond_first_loss(void *tid, const char * bondid, FIRST_LOSS_INPUT first_loss_input, FIRST_LOSS_RESULT* first_loss_result);

	/// This method exposes get china bond info by tranche in WSAAPI
    ///
    /// @since 3.4.1
    /// @avail SFW
	///
	/// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
	/// @param[in] reremic_deal_id_or_null Pass NULL for main deal or remic name for underlying deal.
	/// @param[in] bondid A pointer to the name of the bond.
	/// @param[out] bond_info A pointer to the returned china bond info.
    /// @retval  0  No error.
    /// @retval  -1 Error - Deal not open.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    /// @sample
	///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "HEXIANG171");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
	///		const char* bondid = {"A1"};
	///		CHINA_BOND_INFO bond_info;
	///		assert(0, get_china_bond_info_by_tranche(pDeal, NULL, bondid, &bond_info));
	///
	///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
	/// 
	int CHASAPI get_china_bond_info_by_tranche(void* tid, const char* reremic_deal_id_or_null, const char* bondid, CHINA_BOND_INFO* bond_info);

	/// This method is to set the flag of simulation cashflow populated limit,If flag is set to TRUE, all simulation paths of cash flows can be 
	/// retrieved by function get_collateral_flow_sim and get_bond_flow_sim. If flag is set to FALSE, the paths of cash flows populated can not 
	/// be greater than 100. By default, flag is set to FALSE.
    ///
    /// @since 3.6
    /// @avail SFW
	///
	/// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
	/// @param[in] flag The flag of simulation cashflow populated limit. 
    /// @retval  0  No error.
    /// @retval  -1 Error - Deal not open.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    /// @sample
	///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
	///     assert(0 == remove_simulation_cashflow_populated_limit(pDeal, true));
	///
	///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
	/// 
	int CHASAPI remove_simulation_cashflow_populated_limit(void *tid, BOOLYAN flag);

    /// Gets prospectus prepayment curves from deal.
    ///
    /// @since 3.7
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[out] all_PPCs A client-allocated array of PPC_STRUCT for prospectus prepayment curves information.
    /// @param[in] size The length of the array all_PPCs that the user has passed.
    /// @param[out] num_curves Total number of available curves.
    ///
    /// @retval >=0 Success. Actual number of prospectus prepayment curves returned.
    /// @retval  -1 Error - Deal not open.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @note If pass NULL, the function will return the number of prospectus prepayment curves.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT;
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "NL2017-B");
    ///
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     int num_curves = 0;
    ///     assert(0 == get_prospectus_prepayment_curves(pDeal, NULL, NULL, 0, &num_curves));
    ///
    ///     PPC_STRUCT *all_ppc = new PPC_STRUCT[num_curves];
    ///     assert(0 <= get_prospectus_prepayment_curves(pDeal, NULL, all_ppc, num_curves, &num_curves));
    ///
    ///     delete[] all_ppc;
    ///     all_ppc = NULL;
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI get_prospectus_prepayment_curves(void *tid, const char *reremic_deal_id_or_null, PPC_STRUCT all_PPCs[], int size, int *num_curves);

    /// Sets prepayment speed from prospectus. It will be used for all collateral with ability to apply to underlying deals if it is a reremic.
    ///
    /// @since 3.7
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] PPC_index The 0-based index of the prospectus_prepayment_curves.
    /// @param[in] loan_num The 0-based index of the loan or -1 to apply to all collateral in the deal. Only -1 is supported.
    /// @param[in] set_sup_remic If TRUE this will replace any specified underlying deal settings. If FALSE, this will NOT replace any underlying deal settings.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not open.
    /// @retval  -2 Error - Invalid PPC index.
    /// @retval  -3 Error - Invalid loan number.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT;
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "NL2017-B");
    ///
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
    ///
    ///     int num_curves = 0;
    ///     assert(0 == get_prospectus_prepayment_curves(pDeal, NULL, NULL, 0, &num_curves));
    ///     assert(0 < num_curves);
    ///
    ///     assert(0 == set_prospectus_prepayment_curves(pDeal, 0, -1, FALSE));
    /// @endsample
    ///
    int CHASAPI set_prospectus_prepayment_curves(void *tid, short PPC_index, int loan_num, BOOLYAN set_sup_remic);

	/// This method is adjust PA vectors for SFW student loan deals.
    ///
    /// @since 3.6
    /// @avail SFW
	///
	/// @pre open_deal_ex(), set_moodys_credit_model_settings() has been called.
	///
	/// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @retval  0  No error.
    /// @retval  -1 Error - Deal not open.
	/// @retval  -2 Error - Not set PA credit model.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    /// @sample
	///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
	///     assert(0 == set_moodys_credit_model_settings(pDeal, MOODYS_PA_SETTINGS, false));
    ///
	///     assert(0 == adjust_PA_vectors(pDeal, true));
	///
	///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
	/// 
	/// @note This function only be called after set PA credit model. 
	int CHASAPI adjust_PA_vectors(void *tid, bool enable);

	/// This method is to use spot values for initial period to be consistent with PA credit model for SFW student loan deals.
    ///
    /// @since 3.6
    /// @avail SFW
	///
	/// @pre open_deal_ex() has been called.
	///
	/// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @retval  0  No error.
    /// @retval  -1 Error - Deal not open.
    /// @retval -99 Error - Call get_deal_error_msg() for detail.
    /// @sample
	///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
	///     assert(0 == use_spot_values_for_initial_period(pDeal, true));
	///
	///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
	/// 
	int CHASAPI use_spot_values_for_initial_period(void *tid, bool enable);

	/// Enable Snapshot Date Logic in Whole Loan Analyzer loan cashflow projection, which impacts the order of loan payments. 
	/// The snapshot dates are the dates on which the engine snapshots the state of the whole portfolio. 
	/// The snapshot dates are the dates monthly incremental based on settlement date set in set_whole_loan().
	/// Snapshot date logic assumes:
	///		- Default events, including default, loss and recovery, can only happen on snapshot day of each month, regardless of loan pay frequency.
	///		- All other payments happen on loan payment dates.
	///		- If snapshot day and loan payment day are the same, default event goes first.
	///
    ///
    /// @since 3.7.0
    /// @avail SFW
    ///
    /// @pre set_whole_loan() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] flag_snapshot The flag of using snapshot date logic or not.
    ///
    /// @retval   0 Success.
	/// @retval  -1 Error - Set_whole_loan() not called.
	/// @retval  -4 Error - Other error.
	/// @retval -99 Error - Call get_deal_error_msg() for detail.
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     std::vector<WHOLE_LOAN_STRUCT> loans;
	///     //set informations for each loan in vector loans
    ///     set_whole_loan(pDeal, &loans.front(), 10, 20160101);
	///     enable_default_on_snapshot_date(pDeal, true);
	///     assert(0 == run_deal_ex(pDeal, pCmo));
	///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample  
    int CHASAPI enable_default_on_snapshot_date(void * tid, BOOLYAN flag_snapshot);


    /// Sets how the prepayment would be compounded, based on "monthly" or asset's "periodicity". By default it is monthly.
    ///
    /// @since 4.0.0
    /// @avail SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] prepay_default_compound The specified method of prepayment compounding base, should be enums of ::PREPAY_DEFAULT_COMPOUNDING_METHOD.
    ///
    /// @retval   0 No error.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid prepay compounding method.
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg().
    ///
    /// @sample
	///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
	///     assert(0 == set_prepay_default_compounding_method(pDeal, PREPAY_DEFAULT_COMPOUNDING_MONTHLY));
	///
	///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI set_prepay_default_compounding_method(void *tid, PREPAY_DEFAULT_COMPOUNDING_METHOD prepay_default_compound);

	/// Retrieves account flows for running dynamic cashflows.
    ///
    /// @exptl Subject to change
    /// @since 4.0.0
    /// @avail CDOnet, SFW
    ///
    /// @pre run_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[in] loan_number The 1-based index of the loan.
	/// @param[in] account_name The account name, refer to the field id from ::DEAL_ACCOUNT_INFO.
	/// @param[in] cf The account level cash flow data.
    ///
    /// @retval   NULL  Error - Call get_deal_error_msg().
    /// @retval   OTHER   Pointer to the vector of cashflows.
    ///
    /// @sample
	///     void* pDeal = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(pDeal, pCmo));
	///     assert(0 == run_deal_ex(pDeal, pCmo));
	///     MOODYS_ACCOUNT_CASHFLOW cf;
	///     memset(&cf, 0, sizeof(MOODYS_ACCOUNT_CASHFLOW));
    ///     int ret = get_deal_account_flow(pDeal, "xxx", &cf);
	///
	///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
	int CHASAPI get_deal_account_flow(void *tid, const char* reremic_deal_id_or_null, char* account_name, MOODYS_ACCOUNT_CASHFLOW * cf);
	
    /// This method gets the total loss for a bond.
    ///
    /// @since 4.0.0
    /// @avail CDOnet, SFW, CHS
    ///
    /// @pre run_deal_ex() has been called.
	///
	/// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid  A pointer to the name of the bond.
    /// @param[out] total_loss  The total loss value.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid pointer.
	/// @retval  -3 Error - Bond not found.
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg().
    ///
    /// @sample
    ///    void* tid = NULL;
    ///    CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///    memset(pCmo, 0, sizeof(*pCmo));
    ///    strcpy(pCmo->dealid, "DRYDEN34");
    ///
	///    assert(0 == open_deal_ex(tid, pCmo));
    ///    assert(0 == run_deal_ex(tid,pCmo));
    ///
    ///    const char* bondid = "1A1";
	///	   double total_loss = 0.;
    ///    int nRet = get_bond_total_loss(tid, bondid, &total_loss);
    ///    if(nRet !=0)
    ///    {
    ///    	   //error handle;
    ///    }
	///
    ///     assert(0 == close_deal_ex(tid, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    int CHASAPI get_bond_total_loss(void * tid, const char * bondid, double * total_loss);
	
    /// This method returns the first principal payment month corresponding to the requested bond.
    ///
    /// @since 4.0.0
    /// @avail CDOnet, SFW
    ///
    /// @pre run_deal_ex() has been called.
	///
	/// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid  The name of the bond whose date is being requested.
    /// @param[out] first_prin_pay_month  The time when the tranche receives its first principal. A pointer to a null-terminated string (YYYYMM). This must be pre-allocated with at least 9 characters.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid pointer.
	/// @retval  -3 Error - Bond not found.
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg().
    ///
    /// @sample
    ///    void* tid = NULL;
    ///    CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///    memset(pCmo, 0, sizeof(*pCmo));
    ///    strcpy(pCmo->dealid, "DRYDEN34");
    ///
	///    assert(0 == open_deal_ex(tid, pCmo));
    ///    assert(0 == run_deal_ex(tid,pCmo));
    ///
    ///    const char* bondid = "1A1";
	///	   char first_prin_pay_month[9];
    ///    int nRet = get_first_principal_pay_month(tid, bondid, first_prin_pay_month);
    ///    if(nRet !=0)
    ///    {
    ///    	   //error handle;
    ///    }
	///
    ///     assert(0 == close_deal_ex(tid, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
	int CHASAPI get_first_principal_pay_month(void* tid,const char* bondid, char* first_prin_pay_month);

    /// This method returns the last principal payment month corresponding to the requested bond.
    ///
    /// @since 4.0.0
    /// @avail CDOnet, SFW
    ///
    /// @pre run_deal_ex() has been called.
	///
	/// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid  The name of the bond whose date is being requested.
    /// @param[out] last_prin_pay_month  The time when the tranche receives its last principal. A pointer to a null-terminated string (YYYYMM). This must be pre-allocated with at least 9 characters.
    ///
    /// @retval   0 Success.
    /// @retval  -1 Error - Deal not opened.
    /// @retval  -2 Error - Invalid pointer.
	/// @retval  -3 Error - Bond not found.
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, for details call get_deal_error_msg().
    ///
    /// @sample
    ///    void* tid = NULL;
    ///    CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///    memset(pCmo, 0, sizeof(*pCmo));
    ///    strcpy(pCmo->dealid, "DRYDEN34");
    ///
	///    assert(0 == open_deal_ex(tid, pCmo));
    ///    assert(0 == run_deal_ex(tid,pCmo));
    ///
    ///    const char* bondid = "1A1";
	///	   char last_prin_pay_month[9];
    ///    int nRet = get_last_principal_pay_month(tid, bondid, last_prin_pay_month);
    ///    if(nRet !=0)
    ///    {
    ///    	   //error handle;
    ///    }
	///
    ///     assert(0 == close_deal_ex(tid, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
	int CHASAPI get_last_principal_pay_month(void* tid,const char* bondid, char* last_prin_pay_month);

#if __cplusplus
}
#endif

#endif
