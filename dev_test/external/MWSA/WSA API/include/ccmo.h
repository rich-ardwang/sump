///
/// @file    ccmo.h
/// @version 4.0.1.0
/// @date    2011-2019
///


#ifndef CMO_h_AFA80241_7BBA_11d1_9CCF_0040053302AE
#define CMO_h_AFA80241_7BBA_11d1_9CCF_0040053302AE

/****************************************************************************************/
#define VERSION "4, 0, 1, 0"
/****************************************************************************************/

#ifdef _WIN32
    #ifndef CHAS_NT
        #define CHAS_NT 1
    #endif
#else
    #ifndef CHAS_LINUX
        #define CHAS_LINUX 1
    #endif
#endif


#if CHAS_NT
    #include <windows.h>
    #define CHASAPI WINAPI
#else
    #define CHASAPI
#endif

#include "indextypes.h"

/// Can be used to indicate the agency type of the deal (if it is agency deal).
/// @see MARKIT_DEAL_INFO::agency
typedef enum
{
    NON,        ///< Not one of the listed agency types. Can be any non-agency or agency type not listed.
    FNMA_,      ///< FNMA, all others
    FNMA_GNMA,  ///< FNMA, GNMA
    FRED_75,    ///< FHLMC, all others
    FRED_GOLD,  ///< FHLMC, GOLD
    FRED_GNMA,  ///< FHLMC, GNMA
    GNMA_,      ///< GNMA, all others
} AGENCY_TYPE;

typedef enum
{
    DEAL_SIZE_OF_DEALNAME =80,		///< Max size of deal name is 80 character 
    DEAL_SIZE_OF_ISSUER=80,			///< Max size of issuer name is 80 character 
    DEAL_SIZE_OF_UNDERWRITER=20,	///< Max size of underwriter is 20 character 
    DEAL_SIZE_OF_ASSET_TYPE=26		///< Max size of asset type is 26 character
} DEAL_CONSTANTS;

/// Can be used to indicate the collateral level.
/// @see MARKIT_DEAL_INFO::collat_loaded_level
typedef enum
{
    COLLAT_LEVEL_REPLINES,                          ///< Collateral level: repline
    COLLAT_LEVEL_ACTUAL_LOANS,                      ///< Collateral level: actual loan
    COLLAT_LEVEL_USER,                              ///< Collateral level: user defined (collateral replaced by user input, see replace_collateral())
    COLLAT_LEVEL_ACTUAL_LOANS_LATEST_PARTIAL_UPDATE ///< Collateral level: actual loan, and additional actual loan information from partial deal update for requested update date, if available (CHS only)
} COLLAT_LEVEL;

/// Bond cf mode types
/// @see set_bond_cf_mode
typedef enum
{
    BOND_CF_MODE_TRADING,       ///< Bond cashflow generated as if the bond has been traded (per MARKIT_DEAL_INFO::trade_settlement_date)
    BOND_CF_MODE_HOLDING,       ///< Bond cashflow generated as if the bond is not traded (holding) (per MARKIT_DEAL_INFO::trade_settlement_date)
    BOND_CF_MODE_HOLDING_CAPPED_ACCRUAL ///< Bond cashflow generated as if the bond is not traded (holding), accrual interest is capped. This is only for SFW and CDOnet deals.  
} BOND_CF_MODE;

/// Bond payment dates types
/// @see set_bond_cf_mode
typedef enum
{
    BOND_PAYMENT_DATES_THEORETICAL, ///< Bond payment dates not adjusted for business day rules
    BOND_PAYMENT_DATES_BUS_RULES    ///< Bond payment dates adjusted for business day rules
} BOND_PAYMENT_DATES_TYPE;

/// Enum for credit card assumption type.
typedef enum
{
    CREDIT_CARD_ASSUMP_YIELD,               ///< Portfolio/Annual Yield.
    CREDIT_CARD_ASSUMP_REPAYMENT,           ///< Repayment Rate.
    CREDIT_CARD_ASSUMP_DEFAULT,             ///< Default Rate, only use for CHS engine.
    CREDIT_CARD_ASSUMP_RECOVERY,            ///< Loss Rate.
    CREDIT_CARD_ASSUMP_PURCHASE,            ///< Purchase Rate.
    CREDIT_CARD_ASSUMP_PRINCIPAL_PAYMENT    ///< Principal Payment Rate.
} CREDIT_CARD_ASSUMP_TYPE;


// Surveillance data related constants

typedef enum
{
    PERFORM_MEASURE_NOT_SET=-1, 
    PERFORM_MEASURE_CDR=0, 
    PERFORM_MEASURE_CPR=1, 
    PERFORM_MEASURE_SIZE=2
} SURV_PERFORM_MEASURE_TYPE;

typedef enum
{
    PERFORM_PERIOD_NOT_SET=-1, 
    PERFORM_PERIOD_1M=0, 
    PERFORM_PERIOD_3M=1, 
    PERFORM_PERIOD_SIZE=2
} SURV_PERFORM_PERIOD_TYPE;


typedef enum
{
    AMORTIZATION_TYPE_LEVEL_PAYMENT, 
    AMORTIZATION_TYPE_STRAIGHT_AMORTIZER,
    AMORTIZATION_TYPE_SIZE
} AMORTIZATION_TYPE;

typedef enum
{
    BANKRUPTCY_LOSS,
    FRAUD_LOSS,
    HAZARD_LOSS,
    ADDITIONAL_LOSS_TYPE_SIZE
} ADDITIONAL_LOSS_TYPE;

typedef enum
{
    ADDITIONAL_LOSS_RATE_MONTHLY=0,
    ADDITIONAL_LOSS_RATE_ANNUAL=1
} ADDITIONAL_LOSS_UNIT;

/// The maximum length of the description of surveillance distress state.
const int SUVR_DISTRESS_STATE_DESC_LENGTH = 30;
const int  SURV_PERFORM_DATA_SIZE = PERFORM_MEASURE_SIZE * PERFORM_PERIOD_SIZE;

/// Can be used to indicate how temporary files are treated when the API exits.
/// @sa set_tmp_dir_treatment()
typedef enum
{
    TMP_DIR_DO_NOTHING,             ///< No extra cleanup will be performed.
    TMP_DIR_REMOVE_THIS_PROCESS,    ///< Obsolete, no extra cleanup will be performed.
    TMP_DIR_REMOVE_ALL,             ///< The API will try to remove all temporary files if possible.
} TMP_DIR_TREATMENT;

/// Multiplier type for PA
/// @exptl Subject to change
/// @see set_pa_multiplier()
enum PA_MULTIPLIER_TYPE
{
    PA_MULTIPLIER_PREPAY = 0,      ///< Prepay multiplier.
    PA_MULTIPLIER_DEFAULT,         ///< Default multiplier.
    PA_MULTIPLIER_SEVERITY,        ///< Severity multiplier.

    NUM_PA_MULTIPLIER_TYPE,        ///< Number of PA multiplier type.
};

/// The maximum length of the forecasts (including the current period, time 0). The highest index is MAX_PERIODS-1.
#define MAX_PERIODS 612
/// The maximum number of scheduled balances.
#define MAX_PACS    50
/// The maximum number of collateral groups.
#define MAX_COLL_GROUPS 25

#define DEFAULT_CURVE_CDR           0    ///< The Constant Default Rate is the percentage of the mortgages/loans outstanding at the beginning of the year assumed to terminate during the year through events of default.
#define DEFAULT_CURVE_SDA           1    ///< The SDA Standard Default Assumptions rate specifies an annual default percentage as a function of the seasoning of the mortgages/loans.
#define DEFAULT_CURVE_MDR           2    ///< The Monthly Default Rate is the percentage of the mortgages/loans outstanding at the beginning of the month assumed to terminate during the month through events of default.
#define DEFAULT_CURVE_SEASONED_CDR  3    ///< The Constant Default Rate is the percentage of the mortgages/loans outstanding at the beginning of the year assumed to terminate during the year through events of default.
#define DEFAULT_CURVE_SEASONED_MDR  4    ///< The Monthly Default Rate is the percentage of the mortgages/loans outstanding at the beginning of the month assumed to terminate during the month through events of default.
#define DEFAULT_CURVE_ORIG_MDR      5    ///<
#define DEFAULT_CURVE_ORIG_CDR      6    ///<
#define DEFAULT_CURVE_PCT           7    ///< The PCT is similar to the MDR curve except that defaults are applied each month to the period 0 balance of the loan, rather than the current balance of the loan.
#define DEFAULT_CURVE_PLD           8	 ///< The PLD is the Project Loan Default rate curve. This option is available in SFW engine and CHS engine.

#define PREPAY_CURVE_PSA            1    ///< Standard prepayment curve measuring for prepayments in the residential mortgage market.
#define PREPAY_CURVE_SMM            0    ///< Monthly prepayment or default rate.
#define PREPAY_CURVE_CPR            2    ///< Constant Prepayment Rate(CPR): Prepayment percentage expressed as an annual compounded rate.
#define PREPAY_CURVE_HEP            3    ///< Home Equity Prepayment: A measure of prepayments for closed-end, fixed rate HEL loans. This curve accounts for the faster seasoning ramp for home equity loans.
#define PREPAY_CURVE_ABS            4    ///< Asset-Backed Securities(ABS): It is used in ABS markets, where prepayments differ significantly from standard mortgages. This model defines an increasing sequence of monthly prepayment rates, which correspond to a constant absolute level of loan prepayments in all future periods.
#define PREPAY_CURVE_CUS            5    ///<
#define PREPAY_CURVE_MHP            6    ///<
#define PREPAY_CURVE_SEASONED_SMM   7    ///<
#define PREPAY_CURVE_SEASONED_CPR   8    ///<
#define PREPAY_CURVE_CPB            9    ///<
#define PREPAY_CURVE_CPY            10   ///< Constant Prepayment Yield(CPY): It is equivalent to the Constant Prepayment Rate(CPR) except that it assumes prepayment only happens after contractual lockout and yield maintenance period.


/// Balance of current period (see ::EXTENDED_FLOW_BOND_IDENTIFIER for more info)
#define FLOW_BOND_BALANCE                   0
/// Interest received in current period
#define FLOW_BOND_INTEREST                  1
/// Principal received by bond holder, including prepayment, default recoveries and schedule principal
#define FLOW_BOND_PRINCIPAL                 2
/// Sum of interest and principal, prepayment, recoveries from default that received in current period
#define FLOW_BOND_CASH                      3
/// Return sinking fund schedule
#define FLOW_BOND_PAC_SCHED_REG             4
/// Reserve for future use. Currently it is the same as FLOW_BOND_PAC_SCHED_REG
#define FLOW_BOND_PAC_SCHED_MAX             5
/// Reserve for future use. Currently it is the same as FLOW_BOND_PAC_SCHED_REG
#define FLOW_BOND_PAC_SCHED_MIN             6
/// Bond (capped, if any) coupon rate
#define FLOW_BOND_CAPPED_RATE               7

/// Additional bond cashflow identifiers
///
/// EXTENDED_FLOW_BOND_IDENTIFIER is the extension to bond cashflow identifiers in
/// ccmo.h:
///     Bond cashflow identifiers in ccmo.h | Bond cashflow
///     ----------------------------------- | -------------
///     ::FLOW_BOND_BALANCE                 | Balance of current period
///     ::FLOW_BOND_INTEREST                | Interest received in current period
///     ::FLOW_BOND_PRINCIPAL               | Principal received by bond holder, including prepayment, default recoveries and schedule principal
///     ::FLOW_BOND_CASH                    | Sum of interest and principal, prepayment, recoveries from default that received in current period
///     ::FLOW_BOND_PAC_SCHED_REG           | Return sinking fund schedule
///     ::FLOW_BOND_PAC_SCHED_MAX           | Reserve for future use. Currently it is the same as FLOW_BOND_PAC_SCHED_REG
///     ::FLOW_BOND_PAC_SCHED_MIN           | Reserve for future use. Currently it is the same as FLOW_BOND_PAC_SCHED_REG
///     ::FLOW_BOND_CAPPED_RATE             | Bond (capped, if any) coupon rate
/// @see
///     * get_bond_flow_ex()
typedef enum
{
    FLOW_BOND_PRINCIPAL_WRITEDOWN = 50, ///< Principal writedown
    FLOW_BOND_RATE,                     ///< Bond (capped, if any) coupon rate (same as ::FLOW_BOND_CAPPED_RATE)
    FLOW_BOND_FLT_INDEX,                ///< Index of reference rate
    FLOW_BOND_INTEREST_DUE,             ///< Interest calculated from coupon rate and outstanding balance
    FLOW_BOND_INTEREST_SHORTFALL,       ///< Interest shortfall
    FLOW_BOND_UNCAPPED_RATE,            ///< Uncapped coupon rate
    FLOW_BOND_CAPPED_COUPON,            ///< Capped coupon rate
    FLOW_BOND_BASIS_SHORTFALL,          ///< Basis shortfall
    FLOW_BOND_REIMB_LOSSES,             ///< Reimbursed losses
    FLOW_BOND_DEFERRED_INTEREST,        ///< Deferred interest
    FLOW_BOND_ATTACHMENT,               ///< Attachment point projection, see ::MOODYS_SSFA_CALC for more info
    FLOW_BOND_DETACHMENT,               ///< Detachment point projection, see ::MOODYS_SSFA_CALC for more info
    FLOW_BOND_SSFA_W,                   ///< SSFA W projection, see ::MOODYS_SSFA_CALC for more info
    FLOW_BOND_SSFA_KA,                  ///< SSFA Ka projection, see ::MOODYS_SSFA_CALC for more info
    FLOW_BOND_SSFA_KSSFA,               ///< SSFA Kssfa projection, see ::MOODYS_SSFA_CALC for more info
    FLOW_BOND_SSFA_RW,                  ///< SSFA RW (risk weight) projection, see ::MOODYS_SSFA_CALC for more info
    FLOW_BOND_SSFA_KG,                  ///< SSFA Kg projection, see ::MOODYS_SSFA_CALC for more info
    FLOW_BOND_IMPLIED_LOSS,             ///< Implied losses (SFW only)
    FLOW_BOND_CURR_INS_INT_DUE,         ///< Current interest due from insurance account.  (SFW only)
    FLOW_BOND_CURR_INS_INT_PAID,        ///< Current interest paid from insurance account. (SFW only)
    FLOW_BOND_CURR_INS_PRIN_DUE,        ///< Current principal due from insurance account. (SFW only) 
    FLOW_BOND_CURR_INS_PRIN_PAID,       ///< Current principal paid from insurance account.(SFW only)
    FLOW_BOND_PAST_INS_INT_DUE,         ///< Past interest due from insurance account.     (SFW only)
    FLOW_BOND_PAST_INS_INT_PAID,        ///< Past interest paid from insurance account.    (SFW only)
    FLOW_BOND_PAST_INS_PRIN_DUE,        ///< Past principal due from insurance account.    (SFW only)
    FLOW_BOND_PAST_INS_PRIN_PAID,       ///< Past principal paid from insurance account.   (SFW only)
    FLOW_BOND_IO_BALANCE,               ///< Balance flow of IO tranches.
    FLOW_BOND_NONIO_BALANCE,            ///< Balance flow of non-IO tranches.
    FLOW_BOND_MONTH_END_ACCRUED_INT,    ///< Interest accrued from last coupon payment date to the end of the month.
} EXTENDED_FLOW_BOND_IDENTIFIER;


/// Account cashflow identifiers
///
/// @see
///     * get_collateral_flow_ex()
typedef enum
{
	FLOW_ACCOUNT_BALANCE = 0,                          ///< Balance of account cashflow.
    FLOW_ACCOUNT_SCHED_WITHDRAWAL = 1,                 ///< Withdrawal amount of account cashflow.
    FLOW_ACCOUNT_DEPOSIT = 2,                          ///< Deposit amount of account cashflow.
    FLOW_ACCOUNT_TARGET = 3,                           ///< Target amount of account cashflow.
    FLOW_ACCOUNT_INTEREST = 4,                         ///< Interest amount of account cashflow.
    FLOW_ACCOUNT_DEFERRED_INTEREST = 5,                ///< Deferred Interest of account cashflow.
    FLOW_ACCOUNT_COMMIT_FEE =6,                        ///< Deferred commit_fee of account cashflow.
    FLOW_ACCOUNT_DEFERRED_FEE = 7,                     ///< Deferred deferred_fee of account cashflow.
	FLOW_ACCOUNT_DATES=50,							   ///< account cashflow dates.
}
EXTENDED_FLOW_ACCOUNT_IDENTIFIER;

/// Collateral cashflow identifiers
///
/// @see
///     * get_collateral_flow_ex()
typedef enum
{
    FLOW_COLLATERAL_BALANCE = 0,                            ///< Balance (at the end) of current period
    FLOW_COLLATERAL_SCHED_PRINCIPAL = 1,                    ///< Amount of the scheduled principal
    FLOW_COLLATERAL_PREPAYMENTS = 2,                        ///< Amount of voluntary prepayment
    FLOW_COLLATERAL_DEFAULTS = 3,                           ///< Amount of defaults brought forward to the end of liquidation period
    FLOW_COLLATERAL_LOSSES = 4,                             ///< Amount of losses after property liquidation
    FLOW_COLLATERAL_LIQUIDATIONS = 5,                       ///< Principal recovered after liquidation, same as ::FLOW_COLLATERAL_PRIN_RECOVERIES
    FLOW_COLLATERAL_PRIN_RECOVERIES = 5,                    ///< Principal recovered after liquidation, same as ::FLOW_COLLATERAL_LIQUIDATIONS
    FLOW_COLLATERAL_INTEREST = 6,                           ///< Actual interest paid
    FLOW_COLLATERAL_REINVESTMENT = 7,                       ///< Collateral reinvestment
    FLOW_COLLATERAL_CASH = 8,                               ///< Total amount of cash available for distribution by payment rules, can be calculated as sum of scheduled principal, interest, prepayment, and recovery; for HELOCs the amount of draw amount is subtracted.
    FLOW_COLLATERAL_BOND_VALUE = 9,                         ///< Value of the collateral (set identical to ::FLOW_COLLATERAL_BALANCE)
    FLOW_COLLATERAL_PO_BALANCE = 10,                        ///< Sum of the balance portions of all discount loans in ratio strip POs
    FLOW_COLLATERAL_EXCESS_INTEREST = 11,                   ///< Excessive interest (for ratio strip IOettes)
    FLOW_COLLATERAL_DEL_30 = 12,                            ///< Projections of 30 days of delinquency rates expressed in decimals (0.05 for 5%),CHS only.
    FLOW_COLLATERAL_DEL_60 = 13,                            ///< Projections of 60 days of delinquency rates expressed in decimals (0.05 for 5%),CHS only.
    FLOW_COLLATERAL_DEL_90P = 14,                           ///< Projections of 90+ days of delinquency rates expressed in decimals (0.05 for 5%),CHS only.
    FLOW_COLLATERAL_PREPAY_PENALTY = 15,                    ///< Amount paid as a result of application of payment penalty rules (points, yield maintenance, etc), mostly for CMBS
    FLOW_COLLATERAL_NEGATIVE_AMORT = 16,                    ///< Amount of negative amortization during given period, ether 0 or absolute value of negative scheduled principal
    FLOW_COLLATERAL_BEG_BAL_INTEREST = 17,                  ///< Interest calculated on the amount at the start of the period
    FLOW_COLLATERAL_PO_LOSSES = 18,                         ///< PO losses (for ratio strip POs)
    FLOW_COLLATERAL_DRAW_AMOUNT = 19,                       ///< Amount of additional money drawn during the period (for HELOC)
    FLOW_COLLATERAL_EXCESS_LOSS = 20,                       ///< Excessive loss
    FLOW_COLLATERAL_STUDENT_LOAN_DELAYED_INTEREST = 21,     ///< Delayed interest for student loan, residual balance auto lease
    FLOW_COLLATERAL_AUTO_LEASE_RESIDUE_LOSS = 22,           ///< Auto lease residue losses
    FLOW_COLLATERAL_AUTO_LEASE_NET_RESIDUE_SCHED_FLOW = 23, ///< Auto lease net residue (scheduled)

    FLOW_COLLATERAL_CPR_1M = 24,                            ///< CPR prepayment rate calculated from the voluntary prepayment amount
    FLOW_COLLATERAL_CDR_1M = 25,                            ///< CDR default rate calculated from the defaulted principal amount
    FLOW_COLLATERAL_END_PERFORMING_BAL = 26,                ///< Performing balance at the end of the period
    FLOW_COLLATERAL_DEFAULTS_NO_DELAY = 27,                 ///< Defaults without delay (same as ::FLOW_COLLATERAL_DEFAULTS for some cases)
    FLOW_COLLATERAL_DELBAL_30 = 28,                         ///< Ending balance of defaulted loans outstanding (not yet liquidated) for 30 - 59 days (SFW RMBS, CMBS, and Auto Deals)
    FLOW_COLLATERAL_DELBAL_60 = 29,                         ///< Ending balance of defaulted loans outstanding (not yet liquidated) for 60 - 89 days (SFW RMBS, CMBS, and Auto Deals)
    FLOW_COLLATERAL_DELBAL_90P = 30,                        ///< Ending balance of defaulted loans outstanding (not yet liquidated) for 90+ day (SFW RMBS, CMBS, and Auto Deals)

    FLOW_COLLATERAL_SCHED_P_AND_I = 50,                     ///< Scheduled principal and interest
    FLOW_COLLATERAL_PO_SCHED_PRIN = 51,                     ///< Scheduled principal for PO strips
    FLOW_COLLATERAL_PO_PREPAYMENTS = 52,                    ///< Prepayments for PO strips
    FLOW_COLLATERAL_PO_PRIN_RECOVERIES = 53,                ///< Principal recoveries for PO strips
    FLOW_COLLATERAL_PERFORMING_BALANCE = 54,                ///< Performing balance
    FLOW_COLLATERAL_IO_BALANCE = 55,                        ///< Balance of IO strips
    FLOW_COLLATERAL_ACCRUED_INTEREST = 56,                  ///< Interest accrued
    FLOW_COLLATERAL_ACCURED_INTEREST = 56,                  ///< Typo, same as ::FLOW_COLLATERAL_ACCRUED_INTEREST, will be removed later
    FLOW_COLLATERAL_UNSCHED_PRINCIPAL = 57,                 ///< Principal from default recovery and prepayment
    FLOW_COLLATERAL_PRINCIPAL = 58,                         ///< Sum of unscheduled principal and scheduled principal
    FLOW_COLLATERAL_STUDENT_LOAN_BAL_SCH_AND_GRACE = 59,    ///< Balance of loans under school or grace period (SLABS)
    FLOW_COLLATERAL_STUDENT_LOAN_BAL_DEFERMENT = 60,        ///< Balance of loans under deferment period (SLABS)
    FLOW_COLLATERAL_STUDENT_LOAN_BAL_FOREBEARANCE = 61,     ///< Balance of loans under forbearance period (SLABS)
    FLOW_COLLATERAL_STUDENT_LOAN_BAL_REPAYMENT = 62,        ///< Balance of loans under repayment period (SLABS)
    FLOW_COLLATERAL_STUDENT_LOAN_FFELP_INT_RECOVERY = 63,   ///< Interest reimbursement due to recovered default principal (SLABS)
    FLOW_COLLATERAL_STUDENT_LOAN_ACCRUED_TO_BE_CAPITALIZED = 64,    ///< Cumulative accrued interest to be capitalized (SLABS)
    FLOW_COLLATERAL_STUDENT_LOAN_CAPITALIZED_INT = 65,      ///< Capitalized interest amount which will be added into current balance (SLABS)
    FLOW_COLLATERAL_STUDENT_LOAN_INT_LOSS = 66,             ///< Shortfall of due interest from defaulted principal (SLABS)
    FLOW_COLLATERAL_STUDENT_LOAN_MONTHLY_ACCRUAL_SAP_PAYMENT = 67,  ///< Monthly SAP payment accrued (SLABS)
    FLOW_COLLATERAL_STUDENT_LOAN_SAP_PAYMENT = 68,          ///< SAP payment with delay (SLABS)
    FLOW_COLLATERAL_STUDENT_LOAN_MONTHLY_ACCRUAL_SAP_EXCESS_INT_PAID_TO_DOE = 69,   ///< Monthly excess interest accrued paid to department of education (SLABS)
    FLOW_COLLATERAL_STUDENT_LOAN_SAP_EXCESS_INT_PAID_TO_DOE = 70,   ///< Excess interest paid to department of education (SLABS)
    FLOW_COLLATERAL_STUDENT_LOAN_MONTHLY_ACCRUAL_SUBSIDY_PAYMENT = 71,  ///< Monthly subsidy payment accrued (SLABS)
    FLOW_COLLATERAL_STUDENT_LOAN_SUBSIDY_PAYMENT = 72,      ///< Subsidy payment (SLABS)
    FLOW_COLLATERAL_INTEREST_OF_BUYS = 73,                  ///< Interest of Buys
    FLOW_COLLATERAL_REINVESTMENT_INCOME = 74,               ///< Reinvestment Income
    FLOW_COLLATERAL_LOSS_SEVERITY = 75,                     ///< Loss Severity (CDOnet only)
	FLOW_COLLATERAL_BALANCE_OF_BUYS = 76,					///< Balance of Buys (CDOnet only)
	FLOW_COLLATERAL_DEFAULT_OF_BUYS = 77,					///< Defaulted of Buys (CDOnet only)
	FLOW_COLLATERAL_LOSSES_OF_BUYS = 78,					///< Losses of Buys (CDOnet only)
	FLOW_COLLATERAL_RECOVERY_OF_BUYS = 79,					///< Recovery of Buys (CDOnet only)
	FLOW_COLLATERAL_SCHED_PRIN_OF_BUYS = 80,				///< Scheduled principal of Buys (CDOnet only)
	FLOW_COLLATERAL_TOTAL_PRIN_OF_BUYS = 81,				///< Total principal of Buys (CDOnet only)
	FLOW_COLLATERAL_UNSCHED_PRIN_OF_BUYS = 82,				///< Unscheduled principal of Buys (CDOnet only)
	FLOW_COLLATERAL_WAC = 1001,                             ///< Weighted average coupon.(SFW only)
} EXTENDED_FLOW_COLLATERAL_IDENTIFIER;


#define CLEANUP_CALL_NONE           0       ///< No cleanup call.
#define CLEANUP_CALL_DATE           1       ///< Callable by date.
#define CLEANUP_CALL_PERCENT        2       ///< Callable by percent.
#define CLEANUP_CALL_EITHER         3       ///< Callable by date or percent.
#define CLEANUP_CALL_BOTH           4       ///< Callable by date and percent.

#define DAYCOUNT_DEFAULT            0   ///< Same as ::DAYCOUNT_30_360
#define DAYCOUNT_ACTUAL_360         1   ///< Actual days per month, 360 days per year
#define DAYCOUNT_ACTUAL_365         2   ///< Actual days per month, 365 days per year
#define DAYCOUNT_ACTUAL_ACTUAL      3   ///< Actual days per month, actual days per year
#define DAYCOUNT_30_360             4   ///< 30 days per month, 360 days per year
#define DAYCOUNT_30_365             5   ///< 30 days per month, 365 days per year
#define DAYCOUNT_30_360E            6   ///< 30 days per month, 360 days per year, last day in Feb = 30th
#define DAYCOUNT_ACTUAL_AVG         7   ///< Actual days per month, 365.25 days per year
#define DAYCOUNT_SIZE               8   ///< Number of supported day count conventions



#define CUSTOM_AMORT_NONE           0   ///< WSA API amortizes the collateral.
#define CUSTOM_AMORT_BY_DEAL        1   ///< Cashflows are set at the deal level (collateral group 0),SFW engine not supported deal level currently.
#define CUSTOM_AMORT_BY_GROUP       2   ///< Cashflows are set by collateral group.

#define SERVICER_ADVANCES_INTEREST  1   ///< The servicer advances interest only.
#define SERVICER_ADVANCES_NOTHING   0   ///< The servicer advances neither interest nor principal.
#define SERVICER_ADVANCES_BOTH      2   ///< The servicer advances both interest and principal.

#define DEFLT_FROM_CURBAL   0       ///< See set_default_from_ex() for more info.
#define DEFLT_FROM_ORIGBAL  1       ///< See set_default_from_ex() for more info.
#define DEFLT_FROM_ZERO_CPR 2       ///< See set_default_from_ex() for more info.

#define RECOVERY_RATE_AT_RECOVERY 0 ///< Recovery rate at the time of recovery in case of recovery lag
#define RECOVERY_RATE_AT_DEFAULT  1 ///< Recovery rate at the time of default in case of recovery lag.

#define LOG_OPTION_SUPPRESS         0       ///< No console or log file output
#define LOG_OPTION_POPUP            1       ///< Console output only - no log file
#define LOG_OPTION_OVERWRITE        2       ///< Log file only - overwrite the existing file
#define LOG_OPTION_APPEND           3       ///< Log file only - append to the existing file
#define LOG_OPTION_OVERWRITE_POPUP  4       ///< Log file and console - overwrite the existing file
#define LOG_OPTION_APPEND_POPUP     5       ///< Log file and console - append to the existing file



/// Enum for error handling level.
/// @sa set_error_handling_level()
typedef enum
{ 
    ERROR_HANDLING_LEVEL_LOG_IT,            ///< Log error message to log file when error happens.
    ERROR_HANDLING_LEVEL_STOP_CALCULATION   ///< Stop running when error happens.
} ERROR_HANDLING_LEVEL;

/// Enum for deal search mode.
/// @sa set_deal_search_mode()
typedef enum
{ 
    DEAL_SEARCH_FROM_FILE, 
    DEAL_SEARCH_FROM_MEMORY 
} DEAL_SEARCH_MODE;

/// Enum for the calculation level of API.
/// @sa set_deal_calc_level() get_deal_calc_level()
typedef enum
{ 
    CALC_LEVEL_BASIC,   ///< Basic Calculation Level.
    CALC_LEVEL_FULL,    ///< Full Calculation Level.
    CALC_LEVEL_NONE,    ///< For PA only for now. When call run_deal_ex() with this flag, only PA vectors will be generated and no cashflow generated.
    CALC_LEVEL_FULL_WITH_LOAN  ///< Full Calculation Level and also with loan level cashflows, currently SFW only. 
} CALC_LEVEL;

/// Enum for the action when some interest rates are missing.
/// @sa set_missing_interest_rates_handling()
typedef enum
{
    MISSING_INTEREST_RATES_USE_ZERO,        ///< Use 0 for missing interest rate.
    MISSING_INTEREST_RATES_TREAT_AS_ERROR   ///< Treat missing interest rate as error.
} MISSING_INTEREST_RATES_HANDLING;

/// Enum for the pricing anchors.
/// @sa price_bond()
typedef enum
{ 
    PRICE,  ///< Price
    YIELD,  ///< Yield
    DM      ///< Discount Margin
} PRICING_ANCHORS;

#ifndef DAYTDEF
/// SDK date type, expressed as number of days from 1980/01/01.
typedef long DAYT;
#define DAYTDEF 1
#endif
#ifndef BOOLDEF
typedef short BOOLYAN;          ///< Bool type, 0 for false and non-0 for true.
	#define BOOLDEF 1
#endif
#ifndef SBYTE
	#define SBYTE signed char   ///< Single byte, signed.
#endif


/// @brief This structure holds adjustable rate information about collateral.
///
/// Some of the ARM information is duplicated in the CCMO_POOL_INFO for backward compatibility.
/// @see
///     * CCMO_POOL_INFO
///     * CCMO_POOL_INFO_EX
///     * MARKIT_POOL_INFO
///     * view_colls_ex()
///     * view_reremic_colls_ex()
typedef struct
{
    short index;                     ///< The market index. See INDEX_TYPE enum for types.
    BOOLYAN is_mega;                 ///< If true it is mega. If false it is not.
    short lookback;                  ///< The number of days to look back to get the index rate for resets. Note: The rate period will be the prior month less the lookback.
    double multiplier;               ///< The amount the index rate is multiplied by to calculate the rate the borrower pays.
    double gross_margin;             ///< The amount added to the index rate to determine the rate the borrower pays.
    double net_margin;               ///< The gross margin less fees (such as servicing). This is added to the index rate to determine the rate passed through to the deal.
    double lifetime_cap;             ///< The maximum interest rate allowed. If this is an accumulation of collateral, it will be a weighted average.
    double lifetime_floor;           ///< The minimum interest rate allowed. If this is an accumulation of collateral, it will be a weighted average.
    double periodic_cap;             ///< The maximum absolute interest rate change per period. i.e., 2.0% means absolute rate change is at most 2.0% 
    double pay_periodic_cap;         ///< The maximum percentage change in the payment per period. i.e., 0.075 = 7.5% maximum change
    short first_reset;               ///< Months to the first interest rate reset based on the date the deal is opened as of.
    short first_pay_reset;           ///< Months to the first payment reset based on the date the deal is opened as of.
    short reset_freq;                ///< The months between interest rate resets.
    short pay_reset_freq;            ///< The months between payment resets.
    double neg_amort_limit;          ///< The maximum lifetime percentage increase in balance due to negative amortization.
    double first_pay_cap;            ///< The maximum absolute payment change for the first reset based on the date the deal is opened as of.
    double first_reset_cap;          ///< The maximum absolute interest rate change for the first reset based on the date the deal is opened as of.
    short recast_month;              ///< Reserved for future use.
    short original_first_reset;      ///< Months to original first rate reset (important for hybrids)
    double original_coupon;          ///< Coupon at origination
    short original_first_pay_reset;  ///< Months to original first payment reset
    short consecutive_recast_months; ///< Months between requests after the first one
} CCMO_ARM_INFO;


/// @brief This structure holds information about collateral.
/// @see
///     * CMO_STRUCT
///     * CCMO_POOL_INFO_EX
///     * MARKIT_POOL_INFO
///     * get_pool_by_index_ex()
///     * get_pool_ptr_by_index_ex()
///     * get_reremic_pool_ptr_by_index()
///     * view_colls_ex()
///     * view_reremic_colls_ex()
typedef struct
{
    char id[20];                    ///< Identifies the piece of collateral. Collateral id - "MBS" or deal name
    long number;                    ///< For internal use. ONLY USED FOR READING FRED.MTG FILES 
    short type;                     ///< The type of collateral. Must be enum of ::POOL_TYPE. This corresponds to CCMO_POOL_INFO::type_str.
    char type_str[10];              ///< The description of the type. 
    double pass_through;            ///< The current pass_through rate as of the date the deal is opened. This will be net of trustee and servicing fees. If this is an accumulation of collateral, it will be a weighted average.
    double original_balance;        ///< The original balance of the collateral. If this is an accumulation of collateral, it will only include collateral that is still outstanding.
    double factor;                  ///< The factor of the collateral. The current balance is the factor * original_balance.
    double gross_coupon;            ///< The highest gross coupon of the collateral in an agency pool. Irrelevant for non-agencies. FOR BOND VALUE COMPUTATION 
    double wac;                     ///< The actual gross coupon of the collateral piece. If this is an accumulation of collateral, it will be a weighted average.
    short term;                     ///< The longest original maturity (in months) of the collateral in an agency pool. Irrelevant for non-agencies. FOR BOND VALUE COMPUTATION 
    short wam;                      ///< The remaining months until maturity (as of the date the deal is opened). If this is an accumulation of collateral, it will be a weighted average.
    short original_term;            ///< The original term in months. If this is an accumulation of collateral, it will be a weighted average.
    double psa_coupon;              ///< Strip PO: The coupon used by prepayment models (the actual coupon is 0.) Irrelevant otherwise, set to the pass-through rate. FOR GENERIC PREPAY MODELING 
    short wala;                     ///< The age of the collateral (as of the date the deal is opened). If this is an accumulation of collateral, it will be a weighted average.
    short balloon_period;           ///< The original balloon term.
    short coll_group;               ///< The collateral group this piece belongs to. All collateral in a bucket will belong to the same collateral group. COLLATERAL GROUP 
    double level_pay;               ///< The current level payment for the collateral. If 0, the payment will be calculated.
    double price;                   ///< Reserved for future use.
    short prepay_lockout;           ///< The remaining prepayment lockout in months (as of the date the deal is opened).
    short yield_maintain;           ///< Reserved for future use.
    short day_count;                ///< Reserved for future use.
    short penalty_4;                ///< Reserved for future use.
    short penalty_3;                ///< Reserved for future use.
    short penalty_2;                ///< Reserved for future use.
    short penalty_1;                ///< Reserved for future use.
    short prin_lockout;             ///< The remaining principal lockout in months (as of the date the deal is opened).
    short forward_purchase;         ///< The remaining forward purchase months (as of the date the deal is opened).
    BOOLYAN io;                     ///< If true (non-zero), collateral is interest-only.

    // Additional information necessary for ARMs
    //  this information is now in the arm member. This is kept for 
    //  backwards compatability.
    short arm_index;                ///< ARM only – the market index. See INDEX_TYPE enum for types.
    short first_reset;              ///< ARM only – months to the next interest rate reset.
    short reset_freq;               ///< ARM only – months between interest rate resets.
    short first_pay_reset;          ///< ARM only – months to the next payment reset.
    short pay_reset_freq;           ///< ARM only – months between payment resets.
    double net_margin;              ///< ARM only – amount added to the index to calculate pass-through rate. If this is an accumulation of collateral, it will be a weighted average.
    double gross_margin;            ///< ARM only – amount added to the index to calculate the gross interest rate for the collateral. If this is an accumulation of collateral, it will be a weighted average.
    double periodic_cap;            ///< ARM only – The maximum absolute rate change allowed in one period. If this is an accumulation of collateral, it will be a weighted average.
    double lifetime_cap;            ///< ARM only – The maximum interest rate allowed. If this is an accumulation of collateral, it will be a weighted average.
    double first_reset_cap;         ///< ARM only – The maximum absolute rate change allowed at the first rate reset. If this is an accumulation of collateral, it will be a weighted average.
    double life_cap_spread;         ///< ARM only – The maximum amount the coupon can vary from the original coupon over the life of the collateral. If this is an accumulation of collateral, it will be a weighted average.
    double current_balance;         ///< Current unpaid balance (as of the date the deal is opened).
    double * bv_ratio;              ///< Not currently used
} CCMO_POOL_INFO;

                                             
/// @brief This structure holds information about collateral.
///
/// This structure is an extension to ::CCMO_POOL_INFO.
/// @see
///     * CCMO_POOL_INFO
///     * MARKIT_POOL_INFO
///     * view_colls_ex()
///     * view_reremic_colls_ex()
typedef struct
{
    // Additions in version 2.1.9.0
    CCMO_ARM_INFO * arm;       ///< The structure containing adjustable rate information. This should either be allocated or set to NULL.
    short periodicity;         ///< The number of payments per year.
    double avg_loan_bal;       ///< The average loan balance.
    double ltv;                ///< Loan To Value.
    short fico;                ///< FICO score.
    char servicer_seller[11];  ///< The servicer/seller for the collateral.
    char delinquency[5];       ///< The delinquency status of the collateral.
    char state[3];             ///< The two-character state code.
    char country[4];           ///< The three-character country code.
    char purpose[5];           ///< The purpose of the loan.
                               ///<    Value | Meaning
                               ///<    ------|---------
                               ///<          0| Other
                               ///<          1| Purchase
                               ///<          2| Cash Out Refinance
                               ///<          3| Home Improvement
                               ///<          4| New Construction
                               ///<          5| Rate Term Refinance
                               ///<          6| Not Available
                               ///<
    char property_type[5];     ///< The type of property.
                               ///<    Value | Meaning
                               ///<    ------|---------
                               ///<          0| Other
                               ///<          1| Single Family
                               ///<          2| Multi-Family
                               ///<          3| Condo
                               ///<          4| PUD
                               ///<          5| Commercial
                               ///<          6| Coop
                               ///<          7| Mobile Home
                               ///<          8| Manufactured Housing
                               ///<          9| Not Available
                               ///<          10| Duplex
                               ///<          11| Triplex
                               ///<          12| Fourplex
                               ///<          13| 5+ Units
                               ///<
    char occupancy[5];         ///< The type of occupancy.
                               ///<    Value | Meaning
                               ///<    ------|---------
                               ///<         1| Primary
                               ///<         2| Secondary
                               ///<         3| Investment
                               ///<         4| Unknown
} CCMO_POOL_INFO_EX;

/// @brief This structure holds individual bond information.
/// @see
///     * CMO_STRUCT
///     * MARKIT_BOND_INFO
///     * get_bond_by_index_ex()
///     * get_bond_by_name_ex()
///     * view_all_bonds_ex()
typedef struct
{
    // INPUTS found in bond file 
    double orig_balance;            ///< The original balance of the bond when the deal is issued.
    double current_balance;         ///< The bond outstanding principal as of the date the deal is opened.
    BOOLYAN notional;               ///< This field is true if the bond has a notional balance i.e. it is an interest only bond.
    double coupon;                  ///< The coupon as of the status date for which the deal is opened (see ::CMO_STRUCT.settlement_date).
    DAYT date;                      ///< This is the beginning of the bond accrual period for the current distribution date. This is in DAYT format. Please use dayt_to_str_with_day_count() to convert to mm/dd/yy. 
                                    ///< Parameter dayCount can be determined by MARKIT_BOND_INFO::day_count.
    short delay_days;               ///< The number of days delay between end of interest accrual and bond payment. 
    char id[20];                    ///< The bond ID, including the deal ID.
    char stripped_id[20];           ///< The bond ID, stripped of the deal ID.
    char cusip[10];                 ///< The CUSIP for the bond. 
    BOOLYAN z_bond;                 ///< This field is true if the bond is an accrual bond.
    DAYT stated_maturity;           ///< The bond's stated maturity expressed as a long integer. Use dayt_to_str() to convert to mm/dd/yy.
    char prin_type[20];             ///< A description of the principal type.
    char int_type[20];              ///< A description of the interest type.
    double cap;                     ///< Floaters Only: The maximum interest rate for this bond.
    double floor;                   ///< Floaters Only: The minimum interest rate for this bond.
    double spread;                  ///< Floaters Only: This is added to the current index rate * multiplier.
    double multiplier;              ///< Floaters Only: This is multiplied by the current index rate to calculate the bond coupon at reset.
    short index;                    ///< Floaters Only: The market index used to calculate the coupon.
    short index2;                   ///< Floaters Only: An optional second index used to calculate the coupon.
    short reset;                    ///< Floaters Only: Months between rate resets.
    double per_adj_cap;             ///< Floaters Only: The maximum change in coupon for one reset.
    short lockout;                  ///< Floaters Only: The number of months before the coupon can be adjusted.
    short last_float_per;           ///< Floaters Only: The last period the coupon can be adjusted.
    double resume_coupon;           ///< Floaters Only: Coupon rate after the last_float_per.
    short component;                ///< Flag field indicating if bond is a component bond.
                                    ///<    Value | Meaning
                                    ///<    ------|---------
                                    ///<      >0  | this is a component.
                                    ///<      <0  | this is a owner.
                                    ///<       0  | normal bond
                                    ///<
                                    ///< The absolute values of the owner bond and the components component field are the same.
                                    ///<
    short type;                     ///< Parity Type
                                    ///<	Value  | Meaning
                                    ///<	------ | ---------
                                    ///<          0| Internal use
                                    ///<	      4| Tranche principal payment type is SEQ
                                    ///<	      5| Z bond
                                    ///<	      6| Principal only bond
                                    ///<	      7| Interest only bond
} CCMO_BONDS_S;


/// @brief This structure holds individual bond information.
///
/// This structure is an extension to ::CCMO_BONDS_S.
/// @see
///     * CMO_STRUCT
///     * CCMO_BONDS_S
///     * get_bond_info_by_tranche()
///     * get_bond_info_by_index()
typedef struct
{

    double   orig_balance;                ///< The original balance of the bond when the deal is issued.
    double   current_balance;             ///< The outstanding principal of the bond as of the date the deal is opened.
    int      next_payment_date;           ///< Next payment date for bond interest and principal.
    double   next_payment_beg_balance;    ///< The bond beginning balance on the next payment date.
    double   coupon;                      ///< The interest rate of the bond as of the date the deal is opened (see CMO_STRUCT::settlement_date).
    int      acrual_begin_date;           ///< This is the beginning of the bond accrual period for the current distribution date.
    short    delay_days;                  ///< The number of days delay between end of interest accrual period and actual bond payment.
    char     tranche_name[20];            ///< Name of the tranche
    char     cusip[10];                   ///< CUSIP id
    int      stated_maturity;             ///< The bond's stated maturity.
    char     prin_type[20];               ///< A description of the principal type of the bond.
    char     int_type[20];                ///< A description of the interest  type of the bond.
    short    floater_index;               ///< Floaters Only: The market index used to calculate the coupon.Available values: ::INDEX_TYPE_EX and ::INDEX_TYPE 
    double   floater_spread;              ///< Floaters Only: This is added to the current index rate* multiplier.
    double   floater_multiplier;          ///< Floaters Only: This is multiplied by the current index rate to calculate the bond coupon at reset.
    double   floater_cap;                 ///< Floaters Only: The maximum interest rate for this bond.
    double   floater_floor;               ///< Floaters Only: The minimum interest rate for this bond.
    double   per_adj_cap;                 ///< Floaters Only: The maximum change in coupon for one reset.
    short    first_float_per;             ///< Floaters Only: The first period the coupon starts floating.
    short    last_float_per;              ///< Floaters Only: The last period the coupon can be adjusted.
    double   resume_coupon;               ///< Floaters Only: Coupon rate after the last_float_per.
    short    component;                   ///< This indicates if the bond is a component bond(positive) or an owner bond(negative). The absolute value of the owner bond and the components component field are the same and this  equals the 1-based index of the owner bond in the capital structure. For all other bonds this field is 0.
    short    day_count;                   ///< The day count rule is used to compute interest. The interpretation for the field is:
                                          ///< <div class="generic_text_mapping_table">
                                          ///< <table>
                                          ///< <tr> <th>Code</th> <th>Description</th> <th>Notes</th> </tr>
                                          ///< <tr> <th>1</th> <td>Act/360</td> <td>Actual days in period, 360 day year</td> </tr>
                                          ///< <tr> <th>2</th> <td>Act/365</td> <td>Actual days in period, 365 day year</td> </tr>
                                          ///< <tr> <th>3</th> <td>Act/Act</td> <td>Actual days in period, actual days in year</td> </tr>
                                          ///< <tr> <th>4</th> <td>30/360</td> <td>30 days in month, 360 days in year</td> </tr>
                                          ///< <tr> <th>5</th> <td>30E/360</td> <td>30 days in month, 360 days in year, last day in Feb = 30th.</td> </tr>
                                          ///< </table>
                                          ///< </div>
                                          ///<
    char     bus_rules;                   ///< The rules for determining the payment date (such as next business day if the payment date is not a business day). The interpretation for the field is:
                                          ///< <div class="generic_text_mapping_table">
                                          ///< <table>
                                          ///< <tr> <th>Code</th> <th>Description</th> <th>Comments</th> </tr>
                                          ///< <tr> <th>A</th> <td>Next Bus Day</td> <td>If settlement is not a business day, settle on the next business day.</td> </tr>
                                          ///< <tr> <th>B</th> <td>Next Bus Day In Month</td> <td>If settlement is not a business day, settle on the next business day if it is in the current month. If not, settle on the prior business day.</td> </tr>
                                          ///< <tr> <th>C</th> <td>Next BD Aft Serv Remit</td> <td>Next business day after the master servicer remittance date.</td> </tr>
                                          ///< <tr> <th>P</th> <td>Prev Bus Day</td> <td>If settlement is not a business day, settle on the previous business day.</td> </tr>
                                          ///< <tr> <th>N</th> <td>No Adjustment</td> <td>Do not adjust for business days.</td> </tr>
                                          ///< </table>
                                          ///< </div>
                                          ///<
    char     markit_currency_code;        ///< Currency of the bond. See ::DEAL_ACCOUNT_INFO.currency for the available currency types.
    int      macr;                        ///< If positive than MACR number.
    int      notional;                    ///< This field is true if the bond has a notional balance i.e. it is an interest only bond.
    int      priority_rank;               ///< Priority rank of the tranche within the capital structure of the deal.
    int      insurance_flag;              ///< 1 if bond is insured, 0 - otherwise.
    int      z_bond;                      ///< This field is true if the bond is an accrual bond.
    int      reset_freq;                  ///< The number of months between interest rate resets.
    int      coupon_lockout;              ///< Floaters Only: The first period the coupon starts floating. 
    int      periodicity;                 ///< Payments per year.
} MARKIT_BOND_INFO;

/// @brief This is the primary structure used to pass information to and from the WSA API.
///
/// @see
///     * open_deal_ex()
///     * run_deal_ex()
///     * close_deal_ex()
typedef struct
{
    //
    // information passed from user when opening a CMO bond              
    //
    char dealid[20];                        ///< Required. This is the deal ID. Set this before opening the deal.
                                            ///< @note Value should be set before calling open_deal_ex().
                                            ///<
    char bondid[20];                        ///< Optional. This is the bond ID. Set this before opening the deal if you want to return information about a specific bond.
                                            ///< If no specific bond is needed, set this to null ('\0'), and information about the first bond will be returned (in CMO_STRUCT::bond).
                                            ///< @note Before calling open_deal_ex(), if CCMO_BONDS_S::id of CMO_STRUCT::bond is set to "EXACT", bondid must be set to a valid value.
                                            ///<
    BOOLYAN actual_coll;                    ///< Required. 0, 1 or 2.
                                            ///< Value | Meaning
                                            ///< ------|--------
                                            ///<   0   | bucketed collateral will be used. The bucketing criteria greatly speeds up the processing time while maintaining a high degree of accuracy.
                                            ///<   1   | cashflows will be projected using individual pieces of collateral.
                                            ///<   2   | cashflows will be projected using individual pieces of collateral and additional collateral information from partial deal update for requested update date, if available. (CHS only)
                                            ///<
                                            ///< @note Value should be set before calling open_deal_ex().
                                            ///<
    //
    // information returned from program after opening a CMO bond        
    //
    char next_pay_date[10];                 ///<
                                            ///<        mm/dd/yy
                                            ///<
                                            ///< The first payment date (period 1 cashflows) based on the date the deal is opened as of (see settlement_date member).
                                            ///< @note Value set by WSA API on open_deal_ex().
                                            ///<
    char settlement_date[10];               ///< 
                                            ///<        mm/dd/yy
                                            ///<
                                            ///< Optional. The status date for the deal to be opened and projections run. The deal will be opened as of that month and year.
                                            ///< If set to NULL ('\0')or not set, the deal will be opened with the most recent data.
                                            
    char orig_settlement_date[10];          ///< The original settlement date for the deal.
                                            ///< @note Value set by WSA API on open_deal_ex().
                                            ///<
    char first_pay_date[10];                ///< The first date of cashflows from origination.
                                            ///< @note Value set by WSA API on open_deal_ex().
                                            ///<
    char first_projected_date[10];          ///< Optional.If set to ("1"), the deal opened based on as of date and the bond's period-0 payment date.
                                            ///<
    short num_bonds;                        ///< The number of bonds in the deal. This includes paid-off bonds.
                                            ///< @note Value set by WSA API on open_deal_ex().
                                            ///<
    short num_colls;                        ///< The number of pieces of collateral. Paid-off collateral may not be included.
                                            ///< @note Value set by WSA API on open_deal_ex().
                                            ///<
    short periodicity;                      ///< The number of payments per year.
                                            ///< @note Value set by WSA API on open_deal_ex().
                                            ///<
    CCMO_POOL_INFO coll;                    ///< Deal-level collateral information. Coupons, loan age, etc. are weighted averages. 
                                            ///< @note Value set by WSA API on open_deal_ex().
                                            ///<
    CCMO_BONDS_S bond;                      ///< Bond information.
                                            ///< If specify bond.id with "EXACT", open deal would check the bondif(above) was set correctly,
                                            ///< if bondid is valid, the information will be for that bond, otherwise open deal would fail.
                                            ///< If not specify bond.id with "EXACT", and the bondid (above) was set, the information will be for that bond,
                                            ///< otherwise it will be for the first bond in the deal.
                                            ///<
                                            ///< @note Value set by WSA API on open_deal_ex().
                                            ///<
    //
    // information passed from user when running a CMO bond              
    //
    double deprecated_psa;                  ///< Deprecated please use set_prepayment_ex() or callback function  
    double deprecated_cpr;                  ///< Deprecated please use set_prepayment_ex() or callback function  
    double deprecated_smm[MAX_PERIODS];     ///< Deprecated please use set_prepayment_ex() or callback function  
    double coll_cash[MAX_PERIODS];          ///< Total collateral cashflows for the deal. This is only set when not using the collateral amortization.
                                            ///<
                                            ///< @note Value set by WSA API on run_deal_ex().
                                            ///<
                                            ///<  Value should be set by user after open_deal_ex() returns and before calling run_deal_ex().
                                            ///<
    double coll_bv[MAX_PERIODS];            ///< Total collateral bond value for the deal. Period 0 should be set to -1 before 
                                            ///< @note Value set by WSA API on run_deal_ex().
                                            ///
                                            ///< each run when amortizing collateral, and to the actual values to used when not
                                            ///< using the collateral amortization.
                                            ///<
                                            ///< @note Value set by WSA API on run_deal_ex().
                                            ///<
                                            ///<  Value should be set by user after open_deal_ex() returns and before calling run_deal_ex().
                                            ///
    double deprecated_rate1[MAX_PERIODS];   ///< Obsolete field.
    double deprecated_rate2[MAX_PERIODS];   ///< Obsolete field.

    //
    // information returned from program after running a CMO bond        
    //
    double principal[MAX_PERIODS];          ///< Principal payments for the bond requested (or first bond in deal).
                                            ///< @note Value set by WSA API on run_deal_ex().
                                            ///
    double interest[MAX_PERIODS];           ///< Interest payments for the bond requested (or first bond in deal).
                                            ///< @note Value set by WSA API on run_deal_ex().
                                            ///
    double balance[MAX_PERIODS];            ///< Outstanding balance for the bond requested (or first bond in deal).
                                            ///< @note Value set by WSA API on run_deal_ex().
                                            ///
} CMO_STRUCT;

/// This structure contains two arrays in parallel to describe the payment schedule information. 
typedef struct
{
    DAYT pay_date[MAX_PERIODS];         ///< Dates for the payment to occur
    double pay_amount[MAX_PERIODS];     ///< Amount occurs at each pay date above
} MARKIT_PAYMENT_SCHEDULE;

/// This structure contains prepayment penalty information.
typedef struct
{
    short type;                         ///< Type of prepayment penalty. MONTHS_INTEREST or PCT_OF_PPY, or 0 for none
    double multiplier;                  ///< multiplier; i.e., 6 for 6 months interest or 6% penalty 
    short ym_type;                      ///< 0 for no yield maintenance, 1 otherwise (for now)
    double scaling;                     ///< Used to adjust the prepayment penalty.
} MARKIT_PREPAY_PENALTY;

/// This structure contains additional information required by HELOC loans.
typedef struct
{

    AMORTIZATION_TYPE   amortization_type;      ///< Type of amortizations of the loan. Possible values are:
                                                ///<   - AMORTIZATION_TYPE_LEVEL_PAYMENT
                                                ///<   - AMORTIZATION_TYPE_STRAIGHT_AMORTIZER
    double              draw_limit;             ///< Ratio of the maximum allowable balance over original balance
    int                 last_draw_period;       ///< Number of months after origination after which draw is not allowed. 0 if no limit.
}   MARKIT_HELOC_LOAN_INFO;

/// Enum of the status of student loan.
enum STUDENT_LOAN_STATE
{
    STUDENT_LOAN_STATE_REPAY = 0,  		///< Loan in the repayment status
    STUDENT_LOAN_STATE_DEFER,      		///< Loan in the deferment status
    STUDENT_LOAN_STATE_FORBEAR,    		///< Loan in the forbearance status
    STUDENT_LOAN_STATE_IN_SCHOOL,   	///< Loan in the school period
    STUDENT_LOAN_STATE_GRACE_PERIOD 	///< Loan in the grace period
};

/// Enum of the repay types of student loan.
enum STUDENT_LOAN_REPAY_TYPE
{
    STUDENT_LOAN_REPAY_TYPE_REPAY = 0,
    STUDENT_LOAN_REPAY_TYPE_FULL_DEFER, ///< capitalize interest
    STUDENT_LOAN_REPAY_TYPE_PRIN_DEFER  ///< pay interest, balance flat
};

/// Enum of the prepay penalty structure.
enum PREPAY_PENALTY_STRUCTURE
{
	NOT_AVAILABLE=0,				 ///< not available
    LOCKOUT_ONLY,					 ///< lockout only   
    POINTS_ONLY,					 ///< points only
    YIELDMAINTENANCE_ONLY,			 ///< yieldmaintenance only    
    LOCKOUT_TO_POINTS,				 ///< lockout to points
    YIELDMAINTENANCE_TO_POINTS,		 ///< yieldmatainenance to points      
    UNKNOWN_TO_POINTS				 ///< unknown to points
};

/// @brief This structure holds information about student loan.
typedef struct
{
    double deferGrossMargin;                   ///< Margin in deferment period.
    double deferredInterest;                   ///< Total outstanding unpaid accrued interest.
    bool subsidizedInterestDuringDeferment;    ///< Indicates whether or not deferred interest will be subsidized.
    char loanProgramType[40];                  ///< Should be: continuing Education/Graduate/K-12/Medical/Other/Undergraduate.
    STUDENT_LOAN_STATE loanState;              ///< Indicates the status of the loan. Must be enum of ::STUDENT_LOAN_STATE.
    int remainMonthsInState;                   ///< Remaining Months to Maturity after entering repayment if the loan in payment status.
                                               ///< Remaining Months if the loan in non-payment status for (an aggregate term of any non-payment status: in-school, grace, forbearance, or deferment).                
    STUDENT_LOAN_REPAY_TYPE repayType;         ///< Indicates the loan is in full deferment of P&I or only deferment of principal. Must be enum of ::STUDENT_LOAN_REPAY_TYPE.
} MARKIT_STUDENT_LOAN_INFO;

/// @brief This structure holds information about collateral.
///
/// This structure is an extension to ::CCMO_POOL_INFO and ::CCMO_POOL_INFO_EX.
/// @see
///     * CCMO_POOL_INFO
///     * CCMO_POOL_INFO_EX
///     * PAY_POOL_INFO
///     * get_next_collat()
///     * get_average_collat()
///     * get_average_collat_by_bond()
///     * replace_collateral()
typedef struct
{
    int loan_number;                 ///< An ordinal for loans inside a deal
    char id[POOL_ID_LENGTH];         ///< Identifies the piece of collateral. If reremic, it is the deal-tranche id. 
    long number;                     ///< For internal use.
    short type;                      ///< The type of collateral. Must be enum of ::POOL_TYPE. This corresponds to MARKIT_POOL_INFO::type_str.
    char type_str[10];               ///< The description of the type.
    char remic_deal_name[80];        ///< Remic deal name if reremic, null for a regular pool.
    double pass_through;             ///< The current pass_through rate as of the date the deal is opened. This will be net of trustee and servicing fees. If this is an accumulation of collateral, it will be a weighted average.
    double original_balance;         ///< The original balance of the collateral. If this is an accumulation of collateral, it will only include collateral that is still outstanding.
    double factor;                   ///< The factor of the collateral. The current balance is the factor * original_balance.
    double gross_coupon;             ///< The highest gross coupon of the collateral in an agency pool. Irrelevant for non-agencies.
    double wac;                      ///< The actual gross coupon of the collateral piece. If this is an accumulation of collateral, it will be a weighted average.
    short term;                      ///< The longest original maturity (in months) of the collateral in an agency pool. Irrelevant for non-agencies.
    short wam;                       ///< The remaining months until maturity (as of the date the deal is opened). If this is an accumulation of collateral, it will be a weighted average.
    short original_term;             ///< The original term in months. If this is an accumulation of collateral, it will be a weighted average.
    double psa_coupon;               ///< Strip PO: The coupon used by prepayment models (the actual coupon is 0.) Irrelevant otherwise, set to the pass-through rate.
    short wala;                      ///< The age of the collateral (as of the date the deal is opened). If this is an accumulation of collateral, it will be a weighted average.
    short balloon_period;            ///< The balloon period.
    short coll_group;                ///< The collateral group this piece belongs to. All collateral in a bucket will belong to the same collateral group.
    double level_pay;                ///< The current level payment for the collateral. If 0, the payment will be calculated.
    double price;                    ///< Reserved.
    short prepay_lockout;            ///< The remaining prepayment lockout in months (as of the date the deal is opened).
    short yield_maintain;            ///< Yield maintenance term.
    short prin_lockout;              ///< The remaining principal lockout in months (as of the date the deal is opened).
    short forward_purchase;          ///< The remaining forward purchase months (as of the date the deal is opened).
    short io;                        ///< If true(non-zero), collateral is interest-only.
    double current_balance;          ///< Current unpaid balance (as of the date the deal is opened).
    double avg_loan_bal;             ///< The average loan balance.
    double ltv;                      ///< Original Loan To Value.
    short fico;                      ///< FICO score.
    char servicer_seller[POOL_SERVICER_NAME_LENGTH];    ///< The servicer/seller for the collateral.
    char delinquency[5];                                ///< The delinquency status of the collateral. The value should be|\n
                                                        ///< Value | Meaning
                                                        ///< ------|--------
                                                        ///<  "0" | POOL_DELINQ_CURRENT
                                                        ///<  "1" | Delinquent over 30 days
                                                        ///<  "2" | Delinquent over 60 days
                                                        ///<  "3" | Delinquent over 90 days
                                                        ///<  "4" | Foreclosed
                                                        ///<  "5" | Real estate owned
                                                        ///<  "6" | Terminated
                                                        ///<  "7" | Delinquent over 120 days
                                                        ///<  "8" | Delinquent over 150 days
                                                        ///<  "9" | Delinquent over 180 days
                                                        ///<  "10"| Defeasance status
                                                        ///<  "11"| Non performing matured balloon
                                                        ///<  "12"| Delinquent over 0 days
                                                        ///<  "13"| Bankrupt
                                                        ///<  "14"| Paid off
                                                        ///<  "15"| Repurchased
                                                        ///<  "16"| Liquidated
                                                        ///<  "17"| Closed
                                                        ///<
    char state[POOL_STATE_LENGTH];                      ///< The two-character state code.
    char country[POOL_COUNTRY_LENGTH];                  ///< The three-character country codes, please refer to ISO 3166-1 alpha-3.
    char purpose[POOL_PURPOSE_LENGHT];                  ///< The purpose of the loan.
                                                        ///< Value | Meaning
                                                        ///< ------|--------
                                                        ///<      0| Other
                                                        ///<      1| Purchase
                                                        ///<      2| Cash Out Refinance
                                                        ///<      3| Home Improvement
                                                        ///<      4| New Construction
                                                        ///<      5| Rate Term Refinance
                                                        ///<      6| Not Available
                                                        ///<
    char property_type[POOL_PROPERTY_TYPE_LENGTH];      ///< The type of property.
                                                        ///< Value | Meaning
                                                        ///< ------|--------
                                                        ///<      0| Other
                                                        ///<      1| Single Family
                                                        ///<      2| Multi Family
                                                        ///<      3| Condo
                                                        ///<      4| PUD
                                                        ///<      5| Commercial
                                                        ///<      6| Coop
                                                        ///<      7| Mobile Home
                                                        ///<      8| Manufactured Housing
                                                        ///<      9| Not Available
                                                        ///<      10| Duplex
                                                        ///<      11| Triplex
                                                        ///<      12| Fourplex
                                                        ///<      13| 5+ Units
                                                        ///<
    char occupancy[POOL_OCCUPANCY_LENGTH];              ///< The type of occupancy.
                                                        ///<   Value | Meaning
                                                        ///<   ------|--------
                                                        ///<        1| Primary
                                                        ///<        2| Secondary
                                                        ///<        3| Investment
                                                        ///<        4| Unknown
                                                        ///<            
    char zip[POOL_ZIP_LENGTH];                          ///< Zip code of property
    short pmi;                                          ///< Private Mortgage insurance
    short doc;                                          ///< One of POOL_DOCUM_TYPES: 
                                                        ///<     - POOL_DOCUM_OTHER
                                                        ///<     - POOL_DOCUM_FULL
                                                        ///<     - POOL_DOCUM_LIMITED
                                                        ///<     - POOL_DOCUM_SISA
                                                        ///<     - POOL_DOCUM_SIVA
                                                        ///<     - POOL_DOCUM_NINA
                                                        ///<     - POOL_DOCUM_NO_RATIO
                                                        ///<     - POOL_DOCUM_NO_DOC
                                                        ///<     - POOL_DOCUM_ALTERNATIVE
                                                        ///<     - POOL_DOCUM_UNKNOWN
                                                        ///<

    short lien_type;                                    ///< One of POOL_LIEN_TYPES: 
                                                        ///<     - POOL_LIEN_TYPE_OTHER
                                                        ///<     - POOL_LIEN_TYPE_FIRST
                                                        ///<     - POOL_LIEN_TYPE_SECOND
                                                        ///<     - POOL_LIEN_TYPE_THIRD
                                                        ///<
    short periodicity;                                  ///< The number of payments per year.
    short original_balloon_period;                      ///< Original balloon period.
    short original_prin_lockout;                        ///< Original principal lockout term.
    short original_prepay_lockout;                      ///< Original prepayment lockout term.
    double ltv_combined;                                ///< Loan To Value for all liens.
    double delinq_states[POOL_DELINQ_STATES_SIZE];      ///< Count of loans belonging to one of 6 delinquency states: current, 30+ days, 60+ days, 90+ days, foreclosed, REO, or terminated. Detailed meanings can be reviewed at POOL_DELINQ_STATES.
    double bankruptcy;                                  ///< Count of bankruptcy loans.
    double original_pmi_prct;                           ///< Original Private Mortgage Insurance percentage.
    short original_prepay_penalty_period;               ///< Original prepayment penalty period.
    double ltv_cur;                                     ///< Current Loan To Value.
    void*  usr_data;                                    ///< The pointer to store user specific pool level data. Please refer to install_collat_assump_cb() and set_pool_level_user_data_for_cb() for the usage of this field.
    MARKIT_PAYMENT_SCHEDULE * schedule;                 ///< Payment schedule, of type MARKIT_PAYMENT_SCHEDULE. This should either be allocated or set to NULL.
    MARKIT_PREPAY_PENALTY *ppen[MAX_PERIODS];           ///< Prepayment penalty along pay periods, 
                                                        ///< of type MARKIT_PREPAY_PENALTY. This should either be allocated or set to NULL.
                                                        ///<
    CCMO_ARM_INFO *arm;                                 ///< The structure containing adjustable rate information. This should either be allocated or set to NULL.
    MARKIT_HELOC_LOAN_INFO* heloc;                      ///< The pointer to HELOC_INFO structure or 0 if not a HELOC
    short day_count;                                    ///< Day count.
    short delay_days;                                   ///< Delay days.
    MARKIT_STUDENT_LOAN_INFO* sl_info;                  ///< Info about student loan.
}  MARKIT_POOL_INFO;

/// @brief This structure contains deal level information of a deal
/// @see get_deal_info()
typedef struct
{
    AGENCY_TYPE agency;                             ///< Agency type. Must be enum of ::AGENCY_TYPE
    char issuer[DEAL_SIZE_OF_ISSUER];               ///< Issuer ID
    char deal_name[DEAL_SIZE_OF_DEALNAME];          ///< Deal name
    short periodicity;                              ///< 12 = monthly, 4 = quarterly
    int  deal_settlement_date;                      ///< Settlement date of the deal
    int first_pay_date;                             ///< First payment after origination
    int next_pay_date;                              ///< Next first payment date from last update
    char underwriter[DEAL_SIZE_OF_UNDERWRITER];     ///< Deal underwriter
    int first_call_date;                            ///< The date of first possible call
    double call_percent;                            ///< The percentage of call
    int update_date;                                ///< Deal update date
    char asset_type[DEAL_SIZE_OF_ASSET_TYPE];       ///< Underlying asset type. Can be one of the following strings (not limited to):
                                                    ///<    - Agency
                                                    ///<    - Agency_CMBS
                                                    ///<    - Agency_CMBS_FHLMC
                                                    ///<    - Agency_CMBS_FNMA
                                                    ///<    - Agency_CMBS_GNMA
                                                    ///<    - Agency_FHLMC
                                                    ///<    - Agency_FNMA
                                                    ///<    - Agency_GNMA
                                                    ///<    - Agency_HECM
                                                    ///<    - Auto
                                                    ///<    - Auto_Lease
                                                    ///<    - CDO
                                                    ///<    - CMBS
                                                    ///<    - CommPaper
                                                    ///<    - Consumer_Loans
                                                    ///<    - Credit_Card
                                                    ///<    - Equip
                                                    ///<    - FloorPlan
                                                    ///<    - HECM
                                                    ///<    - HELOC
                                                    ///<    - Home_Equity
                                                    ///<    - Manufactured_Housing
                                                    ///<    - MBS
                                                    ///<    - MBS_AgencyRiskShare
                                                    ///<    - MBS_BuyToLet
                                                    ///<    - MBS_Prime
                                                    ///<    - MBS_Subprime
                                                    ///<    - PPLN
                                                    ///<    - Private_Label_Agency
                                                    ///<    - Rec
                                                    ///<    - SBA
                                                    ///<    - StructNote
                                                    ///<    - Student_Loan
                                                    ///<    - Student_Loan_FFELP
                                                    ///<    - Student_Loan_Private
                                                    ///<    - Tax_Liens
                                                    ///<    - UK_RMBS
                                                    ///<    - Whole_Loan
                                                    ///<
    COLLAT_LEVEL collat_loaded_level;               ///< Collateral level. Must be enum of ::COLLAT_LEVEL
    short num_bonds;                                ///< Number of bonds in the deal
    short num_colls;                                ///< Number of collaterals in the deal
    int   trade_settlement_date;                    ///< Trade settlement date
    int   months_from_last_update_to_settlement;    ///< Number of months from the last update to settlement date
    int   age;                                      ///< Age
    char rmbs_type[DEAL_SIZE_OF_ASSET_TYPE];        ///< Type of rmbs deal, usually be null
} MARKIT_DEAL_INFO;


struct CMM_CUSTOM_ECONOMIC_DATA
{
	double macro_data[13][40];    //represeant 13 types of CMM macro-economic data and each type macro-economic data has 40 Qs.
	bool macro_data_set[13];      //represeant the type of the CMM macro-economic datasetted or not.
	double ir_data[19][40];       //represeant 19 types of CMM interest rate data and each type data has 40 Qs.
	bool ir_data_set[19];         //represeant the type of the CMM interest rate data setted or not.
};

/// @brief This structure includes multiple coupon and balance information on a collateral group.
/// @see
///     get_group_info()
typedef struct
{
    double po_coupon;             ///< Coupon for Principal only collaterals
    double po_balance;            ///< Balance for Principal only collaterals
    double ioette_coupon;         ///< Coupon for IOette collaterals
    double excess_coupon;         ///< Excess coupon
    double ses_coupon;            ///< CHS engine only
}   MARKIT_GROUP_INFO;

/// @brief This structure is passed to assumption call back function.
typedef struct
{
    char* deal_name  ;                     ///< Name of the deal current collateral belongs to
    char* parent_deal_name;                ///< Name of the parent deal for re-remics
    char* settle_date;                     ///< Settlement date.
    MARKIT_POOL_INFO     pool_info;        ///< MARKIT_POOL_INFO structure with parameters of the collateral
    double*              wac;              ///< Array of projected WAC
    void*                unique_pool_id;   ///< Unique identifier of the pool. This field can be used as unique pool identifier by call back function

}  PAY_POOL_INFO;

/// This structure contains license realted information. 
typedef struct
{
    char user_id[100];							// user ID of the license
    char expiration_date[25];					// license expiration date
	char feature_name[255]; 					// license feature name (e.g., General feature)
} LICENSE_INFO;

/// @brief This structure is passed to per-period assumption call back function.
/// It contains amortization state of the pool up to current period.
///
/// All arrays in the structure have length of MAX_PERIODS.
/// All of them are populated up to current period.
/// The client function should not change their values.
typedef struct
{
    double* balance;                      ///< Current balance of collateral
    double* wac;                          ///< Current  weighted-average coupon of the collateral
    double* sched_principal;              ///< Scheduled principal
    double* prepayments;                  ///< prepayments
    double* defaults;                     ///< Defaults
    double* losses;                       ///< Actual losses after recovery
    double* prin_recoveries;              ///< Principal recoveries
    double* interest;                     ///< Net interest
    double* reinvestment;                 ///< Reinvestment
    double* cash;                         ///< Sum of principal and interest
    double* po_balance;                   ///< Principal-Only balance
    double* excess_interest;              ///< Interest paid on the balance at the beginning of the period
    double* negative_amortization;        ///< The amount by which scheduled principal exceeds actual principal
    double* beg_bal_interest;             ///< Interest paid on the balance at the beginning of the period
    double* performing_balance;           ///< Performing balance
    double* new_defaults;                 ///< Defaults happening at a given period
    double* draw_amount;                  ///< Draw amount
    double* total_excess_losses;          ///< Total excess losses
} PAY_POOL_STATE;

/// @brief This structure is passed to per-period assumption call back function.
/// Populate the period's element of the arrays with assumptions for the current period. 
/// For use of the per period assumptions call back functions, populate the period's element of the arrays to
/// provide assumptions for the current period.  All arrays have a size of MAX_PERIODS.
/// @see install_per_period_assump_cb()
typedef struct
{
    double* smm_vector;                  ///< Single month mortality vector expressed in decimals. 1 SMM would be 0.01
    double* mdr_vector;                  ///< Monthly default rate vector expressed in decimals. 1 MDR would be 0.01
    double* recovery_vector;             ///< Monthly vector of recoveries expressed in decimals. 80% recovery would be 0.8
    short*  recovery_lag_vec;            ///< Vector of the lags between foreclosure and recovery expressed in months
    double* dlnq[GROUP_DELINQ_SIZE];     ///< Monthly vector of percentage of delinquent loans, expressed in decimals, for 30, 60, 90+ days delinquencies
    int     servicer_advancing;          ///< Flag indicating if servicer advances principal or/and interest, See set_service_advances_ex() for available values
    double* monthly_draw_rates_vector;   ///< Monthly vector of draw rates in decimals
    double* monthly_additional_losses[ADDITIONAL_LOSS_TYPE_SIZE];   ///< Monthly additional rates come from CCMO_COLLAT_ASSUMPTIONS.additional_losses.
} CCMO_PERIOD_ASSUMPTIONS;

/// @brief This structure is passed to the per-collateral assumptions call back function. 
/// Populate its element with assumptions for all amortization periods.
/// For use of the per collateral assumptions call back function,
/// populate assumptions for a given collateral for the entire simulation period.
/// @see install_collat_assump_cb()
typedef struct
{
    int     prepay_type;                    ///< Unit of prepayment used. See set_prepayments_ex() function for available values.
    int     n_prepays;                      ///< Length of prepay projection periods.
    double* prepay_vector;                  ///< Vector of prepayment values. It should be populated starting from element 1.
                                            ///< The size of the vector does not exceeds MAX_PERIODS.
                                            ///< Numbers should be provided in decimals. The last value will be propagated to the end of simulation.
                                            ///<
    int     default_type;                   ///< Unit of default used. See set_defaults_ex() function for available values.
    int     n_defaults;                     ///< Length of default projection periods.
    double* default_vector;                 ///< Vector of default values. See prepay_vector for more info.
    int     n_recoveries;                   ///< Number of recovery rates provided.
    double* recovery_vector;                ///< Vector of recovery rates. See prepay_vector for more info.
    int     n_recovery_lags;                ///< Number of recovery lags provided.
    short*  recovery_lag_vec;               ///< Vector of recovery lag values in months. See prepay_vector for more info.
    int     n_delinquencies;                ///< Number of delinquencies provided.
    double* dlnq[GROUP_DELINQ_SIZE];        ///< Vector of 30 / 60 / 90+ days delinquencies in decimals.
    int     servicer_advancing;             ///< Flag indicating servicer if there is any advances from the services. See set_service_advances_ex().
    double* draw_rates_vector;              ///< Vector of annualized draw rates for HELOC in decimals. Element 1 corresponds to the next payment date.
    int     n_draw_rates;                   ///< Number of values in draw_rates_vector.
    int     additional_loss_unit_type;      ///< Additional loss rate unit type. reference to ADDITIONAL_LOSS_UNIT.
    double* additional_losses[ADDITIONAL_LOSS_TYPE_SIZE];    ///< Additional loss rates. This rates will be passed to CCMO_PERIOD_ASSUMPTIONS.monthly_additional_losses.
    int     n_additional_losses;            ///< The length of additional losses array.
} CCMO_COLLAT_ASSUMPTIONS;

typedef enum
{
    STUDENT_LOAN_ASSUM_DEFER = 1,
    STUDENT_LOAN_ASSUM_FORBEAR
}STUDENT_LOAN_ASSUM_STATE;

/// @brief This structure is used to provide user with bond cashflow. 
/// It has projected cashflow as well as possibly one historical cashflow some 0 delay bonds are entitled to.
/// It includes number of cashflow points as well as specific bond cashflow dates.
/// Previously only projected cashflows were available.  However some 0 delay floater bond owners might also be entitled to one historical payment.
/// It will be reported in this structure.
///
/// Keep in mind that pointers of this structure will be invalidated once a new structure for the same deal is retrieved from the API.
/// @see get_bond_flow_ex1()
typedef struct
{
    int     size;                               ///< Size of the cashflow including element of 0 representing balance at the settlement date
    int*    hist_flag;                          ///< Indicates if the cashflow is historical or projected
                                                ///< Value | Meaning
                                                ///< ------|--------
                                                ///<      0| projected payment
                                                ///<      1| historical payment
                                                ///<     -1| this is historical payment for which history data is not available. Only date of the payment is reported. Contact your Moody's Analytics representative for historical data
    int*    dates;                              ///< Array of CCYYMMDD dates. Date of actual bond payment. The first period stores the last payment date of the current deal update.
    int*    accrual_begin_dates;                ///< The beginning date that is used to calculate accrual interest for given period.
    double* balance;                            ///< Balance at the end of the period
    double* interest;                           ///< Actual interest paid in the given period
    double* principal;                          ///< Actual principal paid in the given period, including scheduled principal, prepayment and principal recoveries
    double* principal_writedown;                ///< Principal loss due to default. For projected cash flow, this value equals to previous balance minus the sum of the given period balance and principal.
    double* cash;                               ///< Sum of interest and principal
    double* rate;                               ///< Rate from which theoretical interest was calculated. For an adjustable rate bond, this rate is either the smaller number uncapped rate and cap rate or the larger number of uncapped rate and floor rate. Also, this field is subject to increase/decrease cap if applicable.
    double* flt_index;                          ///< Float index rate. This is a reference rate(like Libor/ Treasury/ COFI/ prime) that the bond refers to. This rate is specified in 
    int     start_index_as_per_settle_date;     ///< Start index as per settle date
                                                ///<  - number of months between trade settlement date (MARKIT_DEAL_INFO::trade_settlement_date) and bond accrual begin date (MARKIT_BOND_CASHFLOW::accrual_begin_dates[1])
                                                ///<  - 0 if trade settlement date is the same or earlier than bond accrual begin date
    int     reserve_size;                       ///< Field reserves for internal use. User shall not change it.
    int*    theoretical_dates;                  ///< This is the theoretical date for cashflow. It ignores the weekends and holidays. In MARKIT_BOND_INFO, bus_rules indicates the actual accrual beginning date if the theoretical date a non-business day.
    int*    theoretical_accrual_begin_dates;    ///< Theoretically, the CCYYMMDD dates where accrual begins. This is the theoretical accrual begin date for calculating the interest. It ignores the weekends and holidays. In MARKIT_BOND_INFO, bus_rules indicates the actual accrual beginning date if the theoretical date a non-business day.
    double* interest_due;                       ///< Due interest will be collected. Its value equals to the production of balance from the previous period
    double* interest_shortfall;                 ///< This field is the difference between Interest and Interest Due.
    int*    accrual_days;                       ///< Number of accrual days
    double* uncapped_rate;                      ///< Rate uncapped. It is the sum of margin rate and reference index rate from the last rate reset day.
} MARKIT_BOND_CASHFLOW;

/// This structure is a variation of structure MARKIT_BOND_CASHFLOW.
typedef struct
{
    int     size;                                            ///< Size of the cashflow including element of 0 representing balance at the settlement date
    int     hist_flag[MAX_PERIODS];                          ///< Flags that are used to indicate if the cash flow is historical or projected.
                                                             ///< See MARKIT_BOND_CASHFLOW::hist_flag
    int     dates[MAX_PERIODS];                              ///< Array of CCYYMMDD dates. Date of actual bond payment.
    int     accrual_begin_dates[MAX_PERIODS];                ///< The beginning date that uses to calculate accrual interest.
    double  balance[MAX_PERIODS];                            ///< Balance at the end of the period.
    double  interest[MAX_PERIODS];                           ///< Actual interest paid in given period.
    double  principal[MAX_PERIODS];                          ///< Actual principal paid in the given period, including schedule principal, prepayments and principal recoveries.
    double  principal_writedown[MAX_PERIODS];                ///< Principal loss due to default. For projected cash flow, this value equals to the previous balance minus the sum of the given balance and the given principal.
    double  cash[MAX_PERIODS];                               ///< Sum of interest and principal.
    double  rate[MAX_PERIODS];                               ///< Rate from which theoretical interest was calculated. For an adjustable rate bond, this rate is either smaller number of uncapped rate and cap rate or larger number of uncappeed rate and floor rate. This field is also subject to increase/decrease cap, if it is applicable.
    double  flt_index[MAX_PERIODS];                          ///< Float index rate. This is a reference rate(ie., Libor/ Treasury/ Cofi/ Prime) that the bond refers to. This rate is specified in scenario from the deal.
    int     start_index_as_per_settle_date;                  ///< Start index as per settle date.
                                                             ///< See MARKIT_BOND_CASHFLOW::start_index_as_per_settle_date
    int     theoretical_dates[MAX_PERIODS];                  ///< Theoretically, the CCYYMMDD dates. This is the theoretical date for cashflow, ignoring the weekends and holidays. In structure MARKIT_BOND_INFO, field bus_rules indicates the actual payment date if the theoretical date is a non-business day.
    int     theoretical_accrual_begin_dates[MAX_PERIODS];    ///< This is theoretical accrual begin date for calculating interest. This is theoretical accrual begin date for calculating interest. It ignores the weekends and holidays. In structure MARKIT_BOND_INFO, field bus_rules indicates the actual accrual begin date if theoretical date is non-business day.
    double  interest_due[MAX_PERIODS];                       ///< Interest due. Due interest to be collect, this field value equals to the production of balance from previous period, rate and accrual period(in measurement of year).
    double  interest_shortfall[MAX_PERIODS];                 ///< Interest shortfall. This is the difference between Interest and Interest Due.
    int     accrual_days[MAX_PERIODS];                       ///< Number of accrual days.
    double  uncapped_rate[MAX_PERIODS];                      ///< This field is the sum of margin rate and reference index rate from last rate reset day.
} MARKIT_BOND_CASHFLOW_FOR_MANAGED_CODE;

/// @brief This structure is used to provide user with collateral cashflow. 
/// It includes number of cashflow points as well as collateral cashflow dates.
/// @see get_collateral_flow_ex1()
typedef struct
{
    int     size;                                           ///< Size of the cash flow including element of 0 representing balance at the settlement date
    int     dates[MAX_PERIODS];                             ///< Array of CCYYMMDD dates. The first period stores the last payment date of the current deal update.
    double  balance[MAX_PERIODS];                           ///< Balance at the end of the period
    double  sched_principal[MAX_PERIODS];                   ///< Scheduled principal paid.
    double  prepayments[MAX_PERIODS];                       ///< Prepayments per period
    double  defaults[MAX_PERIODS];                          ///< Defaults per period with lag.
    double  losses[MAX_PERIODS];                            ///< Losses per period
    double  prin_recoveries[MAX_PERIODS];                   ///< This is principal amount recovered when collateral defaults.
    double  interest[MAX_PERIODS];                          ///< Interest paid per period
    double  reinvestment[MAX_PERIODS];                      ///< Reinvestment per period
    double  cash[MAX_PERIODS];                              ///< Cash flow per period This field is the sum of interest, scheduled principal, prepayment and principal recoveries from the default collateral.
    double  bond_value[MAX_PERIODS];                        ///< Bond value per period, identical to the "balance" field in this structure.
    double  prepay_penalties[MAX_PERIODS];                  ///< Prepayment penalty per period. It only applies to that collateral that can be called. For RMSS collateral: This value will be derived from the greater one of the yield maintenance penalty and the penalty based on months of interest or percentage of prepayments.
    int     start_index_as_per_settle_date;                 ///< Start index as per settle date
                                                            ///<  - number of months between trade settlement date (MARKIT_DEAL_INFO::trade_settlement_date) and collateral accrual begin date (MARKIT_BOND_CASHFLOW::accrual_begin_dates[1])
                                                            ///<  - 0 if trade settlement date is the same or earlier than collateral accrual begin date
    double  negative_amortization[MAX_PERIODS];             ///< Negative amortization. This is the absolute value of schedule principal if it is negative. Otherwise it is zero.
    double  gross_interest[MAX_PERIODS];                    ///< Field reserves for future use.
    double  sched_p_and_i[MAX_PERIODS];                     ///< Scheduled principal and interest. This value is composites of gross interest, scheduled principal and negative amortization.
    double  draw_amount[MAX_PERIODS];                       ///< Amount of additional money drawn during the draw period, applicable for HELOCs only, ( Home Equity Line of Credit). This amount will be added into performing balance in the next period.
    double  total_excess_losses[MAX_PERIODS];               ///< Field reserves for future use.
    double  studentLoanDelayedInterest[MAX_PERIODS];        ///< Field reserves for future use.
    double  autoLeaseNetResidualSchedFlow[MAX_PERIODS];     ///< Field reserves for future use.
    double  autoLeaseResidualLoss[MAX_PERIODS];             ///< Field reserves for future use.
    double po_balance[MAX_PERIODS];                         ///< Balance for PO strips.
    double po_sched_principal[MAX_PERIODS];                 ///< Scheduled principal for PO strips.
    double po_prepayments[MAX_PERIODS];                     ///< Prepayments for PO strips.
    double po_prin_recoveries[MAX_PERIODS];                 ///< Principal recoveries for PO strips.
    double po_losses[MAX_PERIODS];                          ///< Loss for PO strips.
    double premium_loan_balance[MAX_PERIODS];               ///< Field reserves for future use.
    double excess_interest[MAX_PERIODS];                    ///< Field reserves for future use.
} MARKIT_COLLAT_CASHFLOW;


/// This structure contains prepayment penalty information. 
typedef struct
{
    char   distress_state[SUVR_DISTRESS_STATE_DESC_LENGTH]; ///< One of the following states: 'Bankrupt', 'Delinquent', 'Foreclosing', or 'REO'.
    char   delinq_state[SUVR_DISTRESS_STATE_DESC_LENGTH];   ///< If the distress_date is "Delinquent", this field further describe how long the delinquency is. It is either "0 to 29 days", "30 to 59 days", "60 to 89 days", or "90+ days".
    double number_of_loans;                                 ///< The number of loans
    double total_current_balance;                           ///< The total current balance
    double loan_percentage;                                 ///< The percentage of the number of the distressed loans over the total number of loans.
    double balance_percentage;                              ///< The percentage of the distressed balance over the total balance 
} MARKIT_SURVEILLANCE_DELINQ_DETAILS;

/// This structure contains loan performance, specifically the prepayment speed and default information. 
typedef struct
{
    SURV_PERFORM_MEASURE_TYPE measure;                      ///< 'CDR' or 'CPR'
    SURV_PERFORM_PERIOD_TYPE  period;                       ///< '1M' or '3M'
    double value;                                           ///< Performance data
} MARKIT_SURVEILLANCE_PERFORM_DETAILS;

/// Number of surveillance distress states.
const int SURV_DISTRESS_STATES_NUMBER = 10;

/// @brief This structure is the collateral group level surveillance information for a deal.
/// This structure is contained in deal level structure ::MARKIT_DEAL_SURVEILLANCE_DATA. 
/// @deprecated This structure is deprecated.
typedef struct
{
    int    groupId;                         ///< Collateral group ID
    int    reportDate;                      ///< The date that this surveillance data is generated for
    double endSchedBalance;                 ///< Collateral group balance at end of the schedule
    double waFico;                          ///< Weighted average Fico score
    double grossWac;                        ///< Gross weighted average coupon
    double netWac;                          ///< Net weighted average coupon
    double waMaturity;                      ///< Weighted average maturity
    double loanCount;                       ///< Number of loans in the collateral group
    double waLtv;                           ///< Weighted average LTV
    double currentPeriodLoss;               ///< The loss on the current period
    double cumulativeLosses;                ///< Cumulated losses
    double originalPoolBalance;             ///< The original pool balance
    double actOCAmt;                        ///< Actual over collateral amount
    double reqOCAmt;                        ///< Required over collateral amount
    double wala;                            ///< Weighted average life age
    MARKIT_SURVEILLANCE_DELINQ_DETAILS distress_loans[SURV_DISTRESS_STATES_NUMBER];     ///< Delinquent information for distressed loans
    MARKIT_SURVEILLANCE_PERFORM_DETAILS speed[SURV_PERFORM_DATA_SIZE];                  ///< Prepayment speed and default information
} MARKIT_GROUP_SURVEILLANCE_DATA;

/// @brief This structure is the deal level surveillance information for a deal.
/// @see get_deal_surv_data()
/// @deprecated This structure is deprecated.
struct MARKIT_DEAL_SURVEILLANCE_DATA
{   
    int    userReqYYYYMM;                    ///< The date of surveillance data that user required for.
    char   ticker[DEAL_SIZE_OF_DEALNAME];    ///< Deal name.
    int    reportDate;                       ///< The date that this surveillance data is generated for.
    double reqOCAmt;                         ///< Required over collateral amount.
    double actOCAmt;                         ///< Actual over collateral amount.
    char   stepDownTrigger[2];               ///< Step down trigger. CHS engine only.
    char   delinqTrigger[2];                 ///< Delinquency trigger. CHS engine only.
    char   cumulLossTrigger[2];              ///< Cumulative loss trigger. CHS engine only.
    char   triggerEvent[2];                  ///< Trigger event. CHS engine only.
    double wala;                             ///< Weighted average life age.
    MARKIT_GROUP_SURVEILLANCE_DATA groupLevelData[MAX_COLL_GROUPS]; ///< @deprecated This field is deprecated..
} ;

/// @brief This structure used for holding pricing result, including average life, duration, yield etc.
/// @see price_bond()
typedef struct
{
    double  AverageLife;             ///< Average life: in years, calculated from bond projected principal payments, payment periods, and settlement date.
    double  AccruedInterest;         ///< Accrued interest: interest accrued from the bond accrual begin date to the settlement date based on bond updated coupon rate and day count (e.g., Act/365), in percentage of the bond updated face value.
    double  DiscountMargin;          ///< Discount margin: in percentage. The difference between yield and index rate in each payment period.
    double  InterestShortfall;       ///< Total interest shortfall in percentage of bond updated face value.
    double  ModifiedDuration;        ///< Modified duration: bond duration adjusted by compounding frequency per year.
    double  ModifiedConvexity;       ///< Modified convexity: bond convexity adjusted by compounding frequency per year.
    double  Price;                   ///< When yield is given, price is calculated from bond projected cashflows, payment periods and yield.
    double  PrincipalWritedown;      ///< Total principal writedown in percentage of bond updated face value.
    double  Yield;                   ///< Yield: in percentage. When price is given, yield is calculated from price, bond projected cashflows and payment periods.
    double  SpreadDuration;          ///< Spread duration: bond duration which calculation applied coupon index rate plus discount margin as discount factor.

} PRICING_RESULTS;

/// @brief This structure used for COLLAT_ASSUMP_CB_EX to store extended loan info
/// @see install_collat_assump_cb_ex()
typedef struct
{
    double original_coupon;            ///< The coupon at issuance.
    int issue_age;                     ///< The age of an loan at issuance.
    int seasoning;                     ///< The period between pool group at issuance and next payment.
    int aft_index_of_agencyname;       ///< The AFT related agency name index. Could be 0-17.
    int curr_date_baloons;             ///< Indicate if balloons from cutoff date. Could be 0 or 1. 1 indicate balloons from cutoff date.
    int curr_date_amtlock;             ///< Indicate if amortization lockout from cutoff date. Could be 0 or 1. 1 indicate amort lockout from cutoff date.
    int unmod_amtlockout;              ///< Amortization Lockout at issuance.
    int inv_term;                      ///< The remaining term of the loan.
    double aft_balance_factor;         ///< The ratio of active assets over all assets in this closing pool. 

    //extra arm info
    double arm_unnmod_lifecap;         ///< The life cap at issuance.
    double arm_unmod_periodic_cap;     ///< The periodic cap at issuance.
    int arm_orig_rate_rperiod;         ///< The rate reset period at issuance.
    int arm_next_rate_reset;           ///< Next rate reset period.
	int issuer_type;					   ///< issue type for whole loan.
} COLLAT_EXTEND_INFO;

typedef struct
{
    int loan_number; 									///< An ordinal for loans inside a deal.
    bool bank_loan;										///< Whether the loan is bank loan or not in CDOnet.
    bool corporate_bond;								///< Whether the loan is corporate bond or not in CDOnet.
    char seniority[15];									///< Seniority name of a loan as Junior/Senior in CDOnet.
    char currency[4];									///< Currency of the loan.
    bool structured_security;							///< Whether the loan is structured security or not.
    char cusip[10];										///< The pool cusip.
	int initial_penalty_months;							///< The initial penalty months.
	int total_prepay_penalty_months;					///< The total prepay penalty months.
	PREPAY_PENALTY_STRUCTURE prepay_penalty_structure;	///< The prepay penalty structure.
	int ever_construction_flag;							///< The ever construction flag.
	char loanx_id[20];									///< Markit's loan unique identifier.
	int moodys_rating;									///< Moodys Rating
	int maturity_date;									///< maturity date
	double market_price_1;								///< Market Price 1
	int next_pay_date;									///< Loan level next pay date.
	char moodys_issuer_name[60];						///< Pool's moodys issuer name.
	char global_issuer_name[60];						///< Pool's global issuer name.
	char company[255];									///< Pool's company name.
	char moodys_category[90];							///< Pool's category.
	char country[30];									///< Pool's country.
	char asset_group[200];								///< Pool's asset group.
	char asset_type[128];								///< Pool's asset type.
	char whole_loan_id[101];							///< An ordinal for loans inside a whole loan.
	int issue_age;										///< Loan issue age.
	int origination_date;								///< Loan level origination date.
	double recovery_rate;								///< Asset recovery rate for CDOnet.
	char non_performing[4];								///< Asset non-performing flag, "YES" or "NO" or "N/A".
	char MKMV_id[7];									///< Asset MKMV ID for CDOnet.
	double DSCR;										///< Asset DSCR for SFW.
	double NOI;											///< Asset NOI for SFW.
	int issue_date;										///< Pool's issue date.
	double appraisal_value;								///< Asset appraisal value for SFW.
} MOODYS_POOL_INFO;

/// This structure contains the bond be payment of group. 
typedef struct
{
    char bondid[10];                 ///< The bond id. 
    double percent;                  ///< The payment portion.
    BOOLYAN current_rata;            ///< Is C_RATA or not.
}MARKIT_PAYMENT_BOND;

/// This structure contains one set of payment group information. 
typedef struct
{
    int number_of_bonds;             ///< Number of bonds in the payment set. 
    MARKIT_PAYMENT_BOND payment_bonds[20]; ///< The payment bonds in information the payment set.
}MARKIT_GROUP_PAYMENT_SET;

/// @brief This structure is the deal level payment group information.
/// @see get_deal_payment_group()
typedef struct
{
    char group_name[10];             ///< The group name. 
    double percent;                  ///< The payment portion.
    int number_of_sets;              ///< Number of payment sets in the group.
    MARKIT_GROUP_PAYMENT_SET payment_sets[20]; ///< The payment sets information in the group.
}MARKIT_DEAL_PAYMENT_GROUP;


/// The signature of the user clean up call back function
///
/// @param[in] user_data Pointer to the previously  registered user data
///
/// @return None
///
typedef void (*USER_CLEANUP_CB)(void* user_data);

/// The function will be called before run CMM model, allowing the user to provide CMM custom data etc to run the custom CMM scenario. 
/// Passing 0 for cmm_custom_cb parameter will remove previously installed function for given tid.
/// After deal is closed functions will also be removed.
///
/// @param[in] custom_scen_name The name of cmm custom  scenario.
/// @param[in] input_csv_file CMM custom scenario portfolio data input file.
/// @param[in] output_file CMM custom scenario output file, if 'high_performance_generated' the output is a zip file, otherwise is a csv file.
/// @param[in] cmm_economic_data CMM macro-economic and interest rate data inputs for the custom cmm scenario which user can input via set_cmm_custom_scenario() or set_macroeconomic_factor_ex. 
/// @param[in] cmm_economic_data_length It would be the length of the array when me_data is an array, otherwise it should be zero.
/// @param[in] batch_generated If set to true can batch generate mutiple custom scenarios base on the input array of cmm_economic_data.
///            Custom scenarios files are patched in a zip file. Otherwise it only generat one custom scenario csv file.              
/// @param[out] error_message User can write  error message into this buffer. 
/// @param[in] max_size_of_error_message Maximum size of error_message buffer allocated by the WSA API
///             on user's behalf. If user write a message longer than this field a stack corruption will occur.
/// @param[in] custom_scen_name it is optional when use batch generated, otherwise it is need to input.
/// @param[in] zip_extract_path it is optional, only use when batch generated, if input it would extract batch generated zip file to the zip_extract_path, if not input would not extract zip file.
///
/// @return 
///     - If user returns negative value from the function means the custom cmm callback function failed.
///         Whatever message user had written into error_message buffer will be
///         available by calling get_deal_error_msg() function.
///     - If user returns 0 simulation will continue
/// @note
///   If batch_generated is true, the cmm_economic_data must be input an array of CMM_CUSTOM_ECONOMIC_DATA.
///   If the cmm_economic_data input an array, the callback would batch generate mutiple custom scenarios, even batch_generated set to false.
///   If batch generated the custom scenarios, custom_scen_name input must be a an integer number which the custom scenario id from, if not input or invalid input, the custom senario id would from 0.
///
typedef int (*CMM_CUSTOM_ECONOMY_CB)(char* input_csv_file, 
							  char* output_file, 
							  CMM_CUSTOM_ECONOMIC_DATA* cmm_economic_data,
							  int cmm_economic_data_length,
							  bool batch_generated,
                              char* error_message,
                              int max_size_of_error_message,
							  char* custom_scen_name,
							  char* zip_extract_path);


/// The function will be called before amortizing every collateral allowing the user to provide prepayments, defaults, recovery,etc. 
/// for the entire amortization period. Passing 0 for collat_assump_cb parameter will remove previously installed function for given tid.
/// After deal is closed functions will also be removed.
///
/// @param[in] tid deal/scenario object identifier which user passed to install_per_period_assump_cb(),
///     this object can be used for calls to other WSA API functions from inside
/// @param[in] first_period_date CCYYMMDD date of the first projected period
/// @param[in] max_periods Maximum number of periods required by simulation
/// @param[in] pool_info Information about collateral for which amortization is being run
/// @param[out] assumptions Pointer to the structure user populates to provide assumptions
/// @param[in] user_data Pointer to the user data , registered through a call to set_user_data_for_cb().
///     This data can allow user to maintain the state during simulation
/// @param[out] error_message User can write  error message into this buffer. 
///     If user returns negative value from call back function simulation will end,
///     run_deal_ex() function will return a negative values and get_deal_error_msg() function
///     will return an error_message which user specified by population this field
/// @param[in] max_size_of_error_message Maximum size of error_message buffer allocated by the WSA API
///     on user's behalf. If user write a message longer than this field a stack corruption will occur
///
/// @return 
///     - If user returns negative value from the function the run_deal_ex() function will stop and
///         return negative value ( different from the one returned by user).
///         Whatever message user had written into error_message buffer will be
///         available by calling get_deal_error_msg() function.
///     - If user returns 0 or any positive value the simulation will continue
/// 
typedef int (*COLLAT_ASSUMP_CB)(     void* tid, 
                             char* first_period_date,
                             int max_periods, 
                             PAY_POOL_INFO* pool_info,
                             CCMO_COLLAT_ASSUMPTIONS* assumptions,
                             void* user_data,
                             char* error_message,
                             int max_size_of_error_message
                         );

typedef int (*COLLAT_ASSUMP_CB_EX1)(void* tid, 
                             char* first_period_date,
                             int max_periods, 
                             PAY_POOL_INFO* pool_info,
                             CCMO_COLLAT_ASSUMPTIONS* assumptions,
                             void* user_data,
                             char* error_message,
                             int max_size_of_error_message,
                             COLLAT_EXTEND_INFO* collat_extend_info,
                             MOODYS_POOL_INFO* moodys_pool_info
                         );

/// The function will be called for every simulation period of every collateral allowing the user
///     to examine the current state of collateral and provide assumptions for the next period.
///     Passing 0 for per_period_assump_cb parameter will remove previously installed function for given tid.
/// After deal is closed functions will also be removed.
///
/// @param[in] tid deal/scenario object identifier which user passed to install_per_period_assump_cb(),
///     this object can be used for calls to other WSA API functions from inside
/// @param[in] period Current period of simulation for which user should provide assumptions
/// @param[in] max_periods Maximum number of periods required by simulation
/// @param[in] pool_info Information about collateral for which amortization is being run
/// @param[in] pool_state Information about the state of the pool up to the current period
/// @param[out] assumptions User should populate period's element of all the arrays inside this structure
///     to provide assumptions for the current period
/// @param[in] user_data Pointer to the user data , registered through a call to set_user_data_for_cb().
///     This data can allow user to maintain the state during simulation
/// @param[out] error_message User can write  error message into this buffer. If user returns negative value from
///     call back function simulation will end, run_deal_ex() function will return a negative values and get_deal_error_msg()
///     function will return an error_message which user specified by population this field
/// @param[in] max_size_of_error_message Maximum size of error_message buffer allocated by the WSA API on user's behalf.
///     If user write a message longer than this field a stack corruption will occur
///
/// @return 
///     - If user returns negative value from the function the run_deal_ex function will stop and return
///       negative value ( different from the one returned by user). Whatever message user had written into
///       error_message buffer will be available by calling get_deal_error_msg() function.
///     - If user returns 0 or any positive value the simulation will continue  
///
typedef int (*PER_PERIOD_ASSUMP_CB)(     void* tid, 
                             int period,
                             int max_periods,
                             PAY_POOL_INFO* pool_info,
                             PAY_POOL_STATE* pool_state,
                             CCMO_PERIOD_ASSUMPTIONS* assumptions,
                             void* user_data,
                             char* error_message,
                             int max_size_of_error_message
                         );



typedef void (*AMORT_PROGRESS_CB)(   void* tid, 
                             int current_pool,
                             int total_pools
                         );


typedef void (*POOL_CASHFLOW_CB)(   void* tid, 
                                    PAY_POOL_INFO* pool_info,
                                    int last_populated_period,
                                    PAY_POOL_STATE* pool_cashflow,
                                    CCMO_PERIOD_ASSUMPTIONS* assumptions,
                                    void* user_data
                                 );
typedef enum
{
    CB_FAIL = -1,
    CB_SUCCESS=0,
    CB_NOOP=1
} USER_CB_RETURN_CODE;

/// The function will be called to set extra input directory before SFW or CHS deal archive files (.SFW, .CHS, .LLD, etc) are accessed
/// if installed by install_input_dir_callback().
///
/// @param[in] deal_name The name (id) of related deal
/// @param[in] file_name The file to be accessed
/// @param[in] archive_name The deal archive to be accessed
/// @param[in] default_dir_if_noop The default input path to be used for the deal
///     if the function returns CB_NOOP. It should be the input path set by set_input_path()
/// @param[out] file_dir_to_provide User should populate extra input path,
///     if applicable, for this particular deal archive/file with this field.
/// @param[in] size_of_file_dir_buffer The size of the extra input path buffer
/// @param[out] error_msg User can write error message into this buffer.
///     If user returns CB_FAIL from call back function, file open will end.
///     get_deal_error_msg() function will return an error_message populated with this field
/// @param[in] size_of_error_msg_buffer The size of error_message buffer allocated by the WSA API on user's behalf.
///     If user write a message longer than this field a stack corruption will occur
///
/// @retval CB_FAIL Calling function from WSA API will fail. error_msg will be populated.
/// @retval CB_SUCCESS WSA API will use file_dir_to_provide populated by user function
///                    as the input path for the given deal/archive/file
/// @retval CB_NOOP WSA API will use default_dir_if_noop (which is set with set_input_path())
///                 as the input path for the given deal/archive/file
///
/// @note
///   This call back function must be thread-safe
///   if it is used within multi-threaded environments.
///   The call back function also needs to use file locks or other mechanisms
///   if there are concurrent writes to file system among different processes.
///
typedef USER_CB_RETURN_CODE (*INPUT_DIR_CB)( 
                            const char* deal_name,
                            const char* file_name,
                            const char* archive_name,
                            const char* default_dir_if_noop,
                            char* file_dir_to_provide,
                            int size_of_file_dir_buffer,
                            char* error_msg,
                            int size_of_error_msg_buffer
                            );

typedef USER_CB_RETURN_CODE (*INPUT_DIR_CB_EX)( 
                            const char* deal_name,
                            const char* file_name,
                            const char* archive_name,
                            const char* default_dir_if_noop,
                            char* file_dir_to_provide,
							const char* update_date,
                            int size_of_file_dir_buffer,
                            char* error_msg,
                            int size_of_error_msg_buffer
                            );

/// Output vector type for PA.
/// @exptl Subject to change
/// @see get_pa_vector()
enum PA_POOL_VECTOR_TYPE{
    PA_DQ_RATE_30,                  ///< Percentage of outstanding balance that is 30-59 days past due, expressed in percent (20% expressed as 20).
    PA_DQ_RATE_60,                  ///< Percentage of outstanding balance that is 60 days past due, expressed in percent (20% expressed as 20).
    PA_DQ_RATE_90,                  ///< Percentage of outstanding balance that is 90 days or more past due, expressed in percent (20% expressed as 20).
    PA_CPR,                         ///< Conditional Prepayment Rate, annualized, expressed in percent (20% expressed as 20).
    PA_CDR,                         ///< Conditional Default Rate, annualized, expressed in percent (20% expressed as 20).

    PA_SEVERITY,                    ///< Loss given default, expressed in percent (20% expressed as 20).
    PA_PRINCIPAL_PAYMENT,           ///< Percentage of the outstanding balance one month ago, paid in the last 30 days, expressed in percent (20% expressed as 20).
    PA_SELLER_PERCENTAGE,           ///< Percentage of the master trust owned by seller, expressed in percent (20% expressed as 20).
    PA_YIELD,                       ///< Finance charge collections from borrowers divided by the pool principal balance at the end fo the prior period, annualized, expressed in percent (20% expressed as 20).
    PA_CHARGEOFF,                   ///< Percentage of outstanding principal that is charged off in this month, expressed in percent (20% expressed as 20).

    PA_PP_LOC_GROWTH,               ///< Monthly growth rate, in percent, of the Unpaid Principal Balance of the Line of Credit portion of the pool, expressed in percent (20% expressed as 20).
    PA_PP_TEN_GROWTH,               ///< Monthly growth rate, in percent, of the Unpaid Principal Balance of the Tenure portion of the pool, expressed in percent (20% expressed as 20).
    PA_PP_TER_GROWTH,               ///< Monthly growth rate, in percent, of the Unpaid Principal Balance of the Term portion of the pool, expressed in percent (20% expressed as 20).
    PA_PP_MTN_GROWTH,               ///< Monthly growth rate, in percent, of the Unpaid Principal Balance of the Modified Tenure portion of the pool, expressed in percent (20% expressed as 20).
    PA_PP_MTM_GROWTH,               ///< Monthly growth rate, in percent, of the Unpaid Principal Balance of the Modified Term portion of the pool, expressed in percent (20% expressed as 20).

    PA_REPAYMENT_RATE,              ///< Percentage of outstanding balance that is due in this month, expressed in percent (20% expressed as 20).
    PA_DEFERMENT_RATE,              ///< Amount of receivables belonging to loans in which the obligor has been granted a deferment, divided by the current collateral balance, expressed in percent (20% expressed as 20).
    PA_FORBEARANCE_RATE,            ///< Amount of receivables belonging to loans in which the obligor has been granted a forbearance, divided by the current collateral balance, expressed in percent (20% expressed as 20).
    PA_TOTAL_DQ_RATE,               ///< Total delinquency rate, annualized, currently it is work the same as the 90 days of delinquency rates, expressed in percent (20% expressed as 20).

    NUM_PA_POOL_VECTOR_TYPES        ///< Number of PA output vector type.
};

typedef enum
{ 
    MARKIT_POOL_HISTORY_CPR1M = 0,   ///< 1 Month CPR Rate    
    MARKIT_POOL_HISTORY_CPR3M,       ///< 3 Month CPR Rate 
    MARKIT_POOL_HISTORY_CPR6M,       ///< 6 Month CPR Rate 
    MARKIT_POOL_HISTORY_CPR12M,      ///< 12 Month CPR Rate
    MARKIT_POOL_HISTORY_CPR24M,      ///< 24 Month CPR Rate
    MARKIT_POOL_HISTORY_CPR_LIFE,    ///< Life time CPR Rate
    MARKIT_POOL_HISTORY_PSA1M,       ///< 1 Month PSA Rate 
    MARKIT_POOL_HISTORY_PSA3M,       ///< 3 Month PSA Rate 
    MARKIT_POOL_HISTORY_PSA6M,       ///< 6 Month PSA Rate
    MARKIT_POOL_HISTORY_PSA12M,      ///< 12 Month PSA Rate
    MARKIT_POOL_HISTORY_PSA24M,      ///< 24 Month PSA Rate
    MARKIT_POOL_HISTORY_PSA_LIFE,    ///< Life time PSA Rate

    MARKIT_POOL_HISTORY_DRAW1M,      ///< 1 Month Draw Rate
    MARKIT_POOL_HISTORY_DRAW3M,      ///< 3 Month Draw Rate
    MARKIT_POOL_HISTORY_DRAW6M,      ///< 6 Month Draw Rate
    MARKIT_POOL_HISTORY_DRAW12M,     ///< 12 Month Draw Rate
    MARKIT_POOL_HISTORY_DRAW_LIFE,   ///< Life time Month Draw Rate
    NUM_OF_MARKIT_POOL_HISTORY
} MARKIT_POOL_HISTORY;


/// This structure contains the pool history data. 
typedef struct
{
    int yyyymmdd;   ///< The update date for the history data.
    double val;     ///< The history data.
} MARKIT_POOL_HISTORY_DATA;

/// Cleanup call balance types.
/// @see set_cleanup_call()
typedef enum
{
    BOND_BAL,                       ///< Bond balance
    END_COLLAT_BAL,                 ///< Ending collateral balance 
    BEG_COLLAT_BAL,                 ///< Beginning collateral balance 
    END_PERFORM_BAL,                ///< Ending performing balance 
    BEG_PERFORM_BAL,                ///< Beginning performing balance
    NUM_CALL_BALANCE_TYPE           ///< Number of call balance types 
} CLEAN_UP_CALL_BALANCE_TYPE;

/// Cleanup call link types.
/// @see set_cleanup_call()
typedef enum
{
    CLEAN_UP_CALL_LINK_AND,         ///< and
    CLEAN_UP_CALL_LINK_OR           ///< or 
} CLEAN_UP_CALL_LINK_TYPE;

typedef enum 
{
   CMM_FACTOR_TYPE_ME,     ///< ME data type
   CMM_FACTOR_TYPE_IR,     ///< IR data type
   NUM_CMM_FACTOR_TYPE     ///< NUM CMM FACTOR type
}CMM_FACTOR_TYPE;

typedef enum
{
	CMM_ME_REALGDPGROWTH,                ///< Real GDP Growth 
	CMM_ME_UNEMPRATE,                    ///< Unemployment Rate
	CMM_ME_FEDFUNDSRATE,                 ///< Federal Funds Rate 
	CMM_ME_TSY10Y,                       ///< 10 year US Treasury
	CMM_ME_CPIINFRATE,                   ///< Consumer Price Index Rate

	CMM_ME_POPGROWTH,                    ///< Population Growth Rate
	CMM_ME_NUMHOUSEHOLDSGROWTH,          ///< Households Growing in Number
	CMM_ME_RETAILSALESGROWTH,            ///< US Retail Sales Growth
	CMM_ME_TOTNONFARMEMPGROWTH,          ///< Total Nonfarming Employment Growth
	CMM_ME_NOMPERSONALINCGROWTH,         ///< Non Personal gGrowth

	CMM_ME_HOMEPRICEGROWTH,              ///< Home price growth
	CMM_ME_BAACORPYIELD,                 ///< US BAA Corporate Bond Yield 
	CMM_ME_CREPXIDXGROWTH,               ///< CRE Price Index Growth

	CMM_IR_LIBOR1M=20,                   ///< 1 Month LIBOR
	CMM_IR_LIBOR3M,                      ///< 3 Month LIBOR
	CMM_IR_LIBOR6M,                      ///< 6 Month LIBOR
	CMM_IR_LIBOR1Y,                      ///< 1 Year LIBOR
	CMM_IR_BANKPRIME,                    ///< Prime Bank

	CMM_IR_DISTRICTCOST,                 ///< District cost
	CMM_IR_CD1M,                         ///< 1 Month CD
	CMM_IR_CD3M,                         ///< 3 Month CD
	CMM_IR_CD6M,                         ///< 6 Month CD
	CMM_IR_TSY1M,                        ///< 1 Month US Treasury

	CMM_IR_TSY3M,                        ///< 3 Month US Treasury
	CMM_IR_TSY6M,                        ///< 6 Month US Treasury
	CMM_IR_TSY1Y,                        ///< 1 year US Treasury
	CMM_IR_TSY2Y,                        ///< 2 year US Treasury
	CMM_IR_TSY3Y,                        ///< 3 year US Treasury

	CMM_IR_TSY5Y,                        ///< 5 year US Treasury
	CMM_IR_TSY10Y,                       ///< 10 year US Treasury
	CMM_IR_TSY30Y,                       ///< 30 year US Treasury
	CMM_IR_MMOVINGAVGCMT,                ///< 12 month moving avg CMT
}CMM_FACTOR;


#if __cplusplus
extern "C"
{
#endif

    // Basic Functions


    /// Closes the open deal and releases all the computational resources that was taken by the deal space. This function should be called 
    /// between successive deals but does not need to be called between successive bonds in the same deal. 
    ///
    /// @since 0.9.0
    /// @avail ALL
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] cmos This must be the CMO_STRUCT used in open_deal_ex().
    ///
    /// @retval >=0 No error
    /// @retval  <0 Error
    ///
    /// @sample
    /// 	void* tid = NULL;
    /// 	CMO_STRUCT *pCmo = new CMO_STRUCT();
    /// 	memset(pCmo, 0, sizeof(CMO_STRUCT));
    /// 	char deal_id[] = "CHL06018";
    /// 	int len = sizeof(deal_id) / sizeof(char);
    /// 	memcpy(pCmo->dealid, deal_id, len);
    ///
    /// 	set_input_path("the_input_path");
    /// 	assert(0 == open_deal_ex(tid, pCmo));
    ///
    /// 	close_deal_ex(tid, pCmo);
    /// 	delete pCmo;
    /// 	pCmo = NULL;
    /// @endsample
    ///
    /// @note Once the deal is closed, it must be re-opened and the scenario information must be re-set before it can be run again. 
    ///
    long CHASAPI close_deal_ex(void *tid, CMO_STRUCT * cmos );


    /// Creates a deal_scenario object (dso). Dso returns with Tid. Dso is a context identifier.
    /// This functions is used for creating an individual context. This is required to have multiple deals open at the same time 
    /// or for multi-threading of deal scenarios. When users open multiple deals at the same time, they need to create separate context for each deal.
    /// Users need to create separate context for each thread if they are running multi-threading.
    ///
    /// @since 0.9.0
    /// @avail ALL
    ///
    /// Each dso should have a distinct log file name and should be used in only one thread at a time. However, 
    /// multiple dso's can be processed within the same thread. 
    ///
    /// @param[out] Tid         The deal/scenario object identifier. This will be passed to subsequent functions.
    /// @param[in] LogAction    Determines how messages are returned.
    ///                             See LogAction param of set_log_options() for more info.
    /// @param[in] LogFile      Fully qualified path and name of the log file.
    /// @param[in] Debug        Whether or not to produce detailed debug info. (irrelevant if LogAction = 0)
    ///                             See Debug param of set_log_options() for more info.
    ///
    /// @retval   1 Created - log file name changed to be unique (underscore(s) appended at end)
    /// @retval   0 No error
    /// @retval  -1 Error - Invalid LogAction 
    /// @retval  -2 Error - Tid is invalid or other error
    /// @retval  -3 Error - Log file could not be created
    /// @retval  -4 Error - No dso available - maximum have been used
    ///
    /// @sample
    /// 	void* tid = NULL;
    /// 	short logAction = 1;
    /// 	char *logFile="log.txt";
    /// 	create_deal_scenario_object(&tid, &logAction, logFile, true);
    /// @endsample
    ///
    /// @note All dso's must be released by calling release_deal_scenario_object() in order to release resources.
    ///       The log/messages options can be changed with set_log_options()
    /// @sa set_log_options() release_deal_scenario_object()
    int CHASAPI create_deal_scenario_object(void **Tid, short *LogAction, char *LogFile, BOOLYAN *Debug);

    /// Retrieves text error message of the previous SDK call or NULL if no errors.
    ///
    /// @since 0.9.0
    /// @avail ALL
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls. After the function call, the parameter 
    ///            will be set to the error message or NULL.
    ///
    /// @retval 0 No error
    /// @retval Address Pointer to error message of the previous call
    ///
    /// @sample
    /// 	void* tid = NULL;
    /// 	CMO_STRUCT *pCmo = new CMO_STRUCT();
    /// 	strcpy(pCmo->dealid,"AL2010-A");
    ///
    /// 	int ret = open_deal_ex(tid, pCmo);
    /// 	if (ret!=0)
    /// 	{
    /// 	    const char * ptr_err_msg = get_deal_error_msg(tid);
    /// 	}
    ///
    /// 	close_deal_ex(tid, pCmo);
    /// 	delete pCmo;
    /// 	pCmo = NULL;
    /// @endsample
    ///
    const char* CHASAPI get_deal_error_msg(void *tid);


    /// Sets the error message for the deal.
    ///
    /// @since 0.9.0
    /// @avail CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] err The error message.
    ///
    /// @retval 0 No error
    /// @retval <0 Fail - check details by calling get_deal_error_msg()
    ///
	/// @sample
    ///     void *tid = NULL;
    ///     char err[100] = {0};
    ///     set_deal_error_msg(tid, err);
    /// @endsample
    ///
    void CHASAPI set_deal_error_msg(void *tid,const char* err);


    /// Opens a deal so that it can be run. This method must be called before calling any functions that change the setting of a specified deal, run or analyze a specified deal.
    /// 
    /// @since 0.9.0
    /// @avail ALL
    ///
    /// @pre set_input_path() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in, out] cmos A pointer to a structure used to pass information to the API and return descriptive information regarding the deal.
    /// e.g. next_pay_date, first_pay_date, orig_settlement_date, periodicity, num_bond, num_colls.
    /// 
    /// @retval  0 Deal opened successfully
    /// @retval  <0 Error - Deal opened failed, check details by calling get_deal_error_msg()
    /// 
    /// @sample
    ///     void* tid(NULL);
	///		//Declare the cmos structure for the deal and initialize it.
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(CMO_STRUCT));
    ///     char deal_id[] = "BAA05006";    // also can be a CUSIP/ISIN of SFW/CDOnet bond, e.g. "31364JMQ3", "000759BP4", "00969QAJ0", "US000780BW56", "XS0333236890"
    ///     int len = sizeof(pCmo->dealid) / sizeof(pCmo->dealid[0]);
    ///     strncpy(pCmo->dealid,deal_id,len-1);
    ///
    ///     set_input_path("C:/deals");
    ///     open_deal_ex(tid, pCmo);
    ///
    ///     run_deal_ex(tid, pCmo);
    ///
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    /// 
    /// @note Structure cmos should be allocated and initialized by the user. The following fields in cmos should be set before opening the deal: 
    ///     - dealid (Required, this field also can be a CUSIP or ISIN. If the deal id is unknown, it can be found by calling get_moodys_id() with CUSIP or ISIN.)
    ///     - bondid (Optional. If not set the first bond in the deal will be used.)
    ///     - bond.cusip (Optional.  This is optional for opening deals but required for opening agency pools from LPD files.)
    ///       cusip is a 9-character alphanumeric code which uniquely identifies a security. 
    ///       bond.cusip is a char array. If it is not provided, user should leave it empty(bond.cusip[0] == 0). e.g."00252FAD3" is a legal cusip.
    ///     - actual_col (Required. 0 or 1. If 1, actual collateral is used. If 0 collateral is bucketed, which greatly speeds up processing while maintaining a high degree of accuracy.)
    ///     - settlement_date (Optional. This is the deal status date from which projections will be run. If not set, the most recent deal status will be used. mm/dd/yy.)
    ///     - first_projected_date (Optional. If set to ("1"), the deal opened based on as of date and the bond's period-0 payment date)
    ///     - bond.id  (Optional. If specify with "EXACT", bondid is required a valid value, otherwise, open deal would fail.)
    /// 
    /// ** If not running custom amortization and not setting scenarios, call set_custom_amortization_ex(tid,CUSTOM_AMORT_NONE) after open_deal_ex()
    ///    to ensure the deal is properly run.
    /// 
    long CHASAPI open_deal_ex(void *tid, CMO_STRUCT * cmos );

    /// This function opens a single pool.
    ///
    /// @since 1.1.0
    /// @avail ALL
    ///	
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] cusip The CUSIP being requested.
    /// @param[in] reserved Use NULL.
    /// @param[in] YYYYMMDD_settlement_date Settlement date in YYYYMMDD format.
    ///
    /// @retval 0 SUCCESS
    /// @retval -99 Error, for details call get_deal_error_msg()
    ///
	/// @sample
    ///    void* tid = NULL;
    ///    int iret = create_deal_scenario_object(&tid,NULL,NULL,NULL);
    ///    set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///
    ///    iret = open_pool_from_file(tid, "03072SB38", 0, 20120901);
    ///    if(iret <0)
    ///    {
    ///    	//error handle;
    ///    }	
    /// @endsample
    ///
    long CHASAPI open_pool_from_file(void* tid, const char* cusip, const char* reserved /* use NULL*/, int YYYYMMDD_settlement_date);

    /// This function releases a deal_scenario object (dso) and frees the resources. Once the dso is released, no further processing 
    /// can be performed with it.
    ///
    /// @since 0.9.0
    /// @avail ALL
    ///
    /// @pre create_deal_scenario_object() has been called.
    ///
    /// @param[in] Tid The deal/scenario identifier obtained when the dso was created.
    ///
    /// @retval 0 Success
    /// @retval -1 Error - Invalid Parameter
    /// @retval -2 Error - Runtime error, memory error or other error
    ///
    /// 
    /// @sample
    /// 	void* ptid = NULL;
    /// 	CMO_STRUCT *pCmo = new CMO_STRUCT();
    /// 	strcpy(pCmo->dealid, "AL2010-A");
    ///
    /// 	bool Debug = true;
    /// 	short LogAction = Debug ? LOG_OPTION_OVERWRITE : LOG_OPTION_SUPPRESS;
    /// 	int iret = creat_deal_scenario_object(&ptid, &LogAction, "log.txt", &Debug);
    /// 	if(iret != 0)
    /// 	{
    /// 	    return;
    /// 	}
    /// 	open_deal_ex(ptid, pCmo);
    ///
    /// 	release_deal_scenario_object(&ptid);
    ///
    /// 	close_deal_ex(ptid, pCmo);
    /// 	delete pCmo;
    /// 	pCmo = NULL;
    /// @endsample
    ///
    /// @sa create_deal_scenario_object()
    int CHASAPI release_deal_scenario_object(void **Tid);


    /// Projects cashflows for the currently open deal using specified scenario.
    /// The scenarios (such as market rates, prepayment assumptions, default rates, etc.) can be set using the corresponding functions before running the deal.
    ///
    /// @since 0.9.0
    /// @avail ALL
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in,out] cmos A pointer to the CMO_STRUCT used in open_deal_ex().
    ///
    /// @retval >= 0 Deal run successfully
    /// @retval  -1 Error - Deal not open
    /// @retval  -2 Error - Unable to run deal
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @note Scenario information (market rates, prepayment assumptions, credit sensitivity assumptions, etc) 
    ///     should be set after opening the deal and before calling run_deal_ex().
    ///     The same deal can be re-run multiple times, varying the scenarios, without re-opening the deal.
    ///     It is fastest to run one deal at a time, varying the scenarios, than to 
    ///     run one scenario at a time, varying the deal.
    ///
    /// @sample
    ///     void* tid(NULL);
    ///     CMO_STRUCT *pCmo = new CMP_STRUCT();
    ///     memset(pCmo, 0, sizeof(CMO_STRUCT));
    ///     char deal_id[] = "BAA05006";
    ///     int len = sizeof(pCmo->dealid) / sizeof(pCmo->dealid[0]);
    ///     strncpy(pCmo->dealid,deal_id,len-1);
    ///
    ///     set_input_path("C:/deals");
    ///     open_deal_ex(tid, pCmo);
    ///
    ///     run_deal_ex(tid, pCmO);
    ///
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    long CHASAPI run_deal_ex(void *tid, CMO_STRUCT * cmos );

    /// Sets the log/message options either globally or for the requested deal. If an option is NULL it will not be changed.
    /// 
    /// @since 0.9.0
    /// @avail ALL
    /// 
    /// @param[in] tid          The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] LogAction    Determines how messages are returned. Must be one of the following:
    ///         - ::LOG_OPTION_SUPPRESS 
    ///         - ::LOG_OPTION_POPUP
    ///         - ::LOG_OPTION_OVERWRITE
    ///         - ::LOG_OPTION_APPEND
    ///         - ::LOG_OPTION_OVERWRITE_POPUP
    ///         - ::LOG_OPTION_APPEND_POPUP
    /// @param[in] LogFile      Fully qualified path and name of the log file.
    /// @param[in] Debug        Whether or not to produce detailed debug info. (irrelevant if LogAction = 0)
    ///             - TRUE :  Output detailed debug info.
    ///             - FALSE : NOT output detailed debug info.
    ///
    /// @retval  1 Unable to write to log file. No file logging (pop_up notification, if requested, will be provided).
    /// @retval  0 No error
    /// @retval -1 Error - Invalid LogAction
    /// @retval -2 Error - Other error
    ///  
    /// @sample
    ///		CMO_STRUCT cmo;
    ///		memset(&cmo, 0, sizeof(CMO_STRUCT));
    ///
    ///		BOOLYAN Debug=1;
    ///		short action = LOG_OPTION_APPEND_POPUP;
    ///		set_log_options(NULL, &action, "log.txt", &Debug);
    ///
    ///		void* tid = NULL;
    ///		int iret = create_deal_scenario_object(&tid,NULL,NULL,NULL);
    ///		set_log_options(tid, &action, "log_1.txt", &Debug); //The log file must be unique for every deal/scenario object 
    ///
    ///		open_deal_ex(tid,&cmo);
    ///		close_deal_ex(tid,&cmo);
    ///		release_deal_scenario_object(&tid);
    /// @endsample
    /// @warning Different deal/scenario objects should use different LogFile.
    /// @warning Logging (esp with Debug set to TRUE) should be enabled for debugging purpose only.
    ///             Detailed pay rule information, cashflow output, and other debugging information will fill up disk space quickly.
    /// @sa create_deal_scenario_object()
    int CHASAPI set_log_options(void * tid, short *LogAction, char *LogFile, BOOLYAN *Debug);

	/// This method is to set options of cleanup call.
    ///
    /// @since 3.0.0
    /// @avail CHS, SFW
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] percentage The percentage of balance to call. It must be a decimal between 0 and 1.
    /// @param[in] call_balance_type Type of call balance. Should be one of ::CLEAN_UP_CALL_BALANCE_TYPE.
    /// @param[in] link_type Type of call link. Should be one of ::CLEAN_UP_CALL_LINK_TYPE.
    /// @param[in] yyyymm_date The specified date to call(of format YYYYMM).
    /// @param[in] set_sup_remic Settings are applied to underlying deals if TRUE. Otherwise, it will not.
    ///
    /// @retval   0 Success.
    /// @retval  -2 Error - Invalid parameter.
    /// @retval -99 Error ,call get_deal_error_msg() for details.
    ///
    /// @sample
    ///     void * pDeal = NULL;
    ///     // Open deal
    ///
    ///     assert(0 == set_call_option(pDeal, CLEANUP_CALL, false));
    ///     char call_date[11]    =  {0};
    ///     double call_percent   =  0.0;
    ///     int call_percent_calc =  0;
    ///     int iRet = get_cleanup_call_ex(pDeal, call_date, &call_percent, &call_percent_calc);
    ///     
    ///     double pct = 0.8;
    ///     int callDate = 201512;
    ///     CLEAN_UP_CALL_LINK_TYPE linkType = CLEAN_UP_CALL_LINK_AND;
    ///     assert(0 == set_cleanup_call(pDeal, &pct, NULL, &linkType, &callDate, false));
    ///     
    ///     iRet = get_cleanup_call_ex(pDeal, call_date, &call_percent, &call_percent_calc);
    /// @endsample
    ///
	/// @note
    ///			For CHS engine, the call balance type only supports the "END_COLLAT_BAL".
	///
    int CHASAPI set_cleanup_call(void * tid, double* percentage, CLEAN_UP_CALL_BALANCE_TYPE* call_balance_type, CLEAN_UP_CALL_LINK_TYPE* link_type, int* yyyymm_date, BOOLYAN set_sup_remic);

    /// Sets the deal calculation level: CALC_LEVEL_BASIC or CALC_LEVEL_FULL.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] level CALC_LEVEL_BASIC or CALC_LEVEL_FULL,some bond/collateral flow only be calculated when set to CALC_LEVEL_FULL.
    /// @param[in] propagate_to_remics If apply to remics as well
    ///
    /// @return None
    ///
    /// @sample
    ///     void * pDeal = NULL;
    ///     // Open deal
    ///
    ///     set_deal_calc_level(pDeal,CALC_LEVEL_BASIC,0);
    /// @endsample
    ///
    /// @sa
    /// get_bond_flow_ex
    /// get_bond_flow_ex1
    /// get_bond_flow_ex1_for_managed_code
    /// get_collateral_flow_ex
    /// get_collateral_flow_ex1
    void CHASAPI set_deal_calc_level(void* tid,CALC_LEVEL level, int propagate_to_remics);

    /// Gets the deal calculation level.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The dso identifier.
    ///
    ///  @retval CALC_LEVEL
    ///  @retval CALC_LEVEL_BASIC
    ///  @retval CALC_LEVEL_FULL
    ///
	/// @sample
    ///    void * tid = NULL;
    ///    // Open deal
    ///
    ///    // Get deal calculation level
    ///    CALC_LEVEL calc_level = get_deal_calc_level(tid);
    /// @endsample
    ///
    CALC_LEVEL CHASAPI get_deal_calc_level(void* tid);

    /// This function returns the Software release version of this WSA API build.
    ///
    /// @since 1.1.0
    /// @avail ALL
    ///	
    /// @retval 0 Error
    /// @retval Other The version number of the build
    ///
    /// @sample
    ///     const char* version = get_sdk_build_version();
    ///     bool support_exess_rate = strcmp(version,"2, 4, 1, 0") > 0;
    /// @endsample
    ///
    const char* CHASAPI get_sdk_build_version() ;

    /// @warning This method is obsolete and does @em NOT perform anything when it is called.
    /// @deprecated This method is deprecated and does @em NOT perform anything when it is called.
    int set_maximum_deal_scenario_objects(int max);


    /// Sets the temporary files clean up method when the API exits.
    ///
    /// @since 1.5.0
    /// @avail ALL
    ///
    /// @pre None. It can be called anywhere before the API exits.
    ///
    /// @param[in] tmp_dir_treatment Must be enum of ::TMP_DIR_TREATMENT
    ///
    /// @return
    /// None
    /// 
    /// @sample
    ///     // Try to remove all temporary files if possible when it exits
    ///     set_tmp_dir_treatment(TMP_DIR_REMOVE_ALL);
    /// @endsample
    ///
    /// @note
    ///         The API uses advisory file locks.
    ///         The current implement for cleanup procedure
    ///         will NOT remove any temporary files when it exits
    ///         if there are other WSA API processes holding the file locks.
    ///
    void CHASAPI set_tmp_dir_treatment(TMP_DIR_TREATMENT tmp_dir_treatment);


    /// Sets the mode for deal search: either DEAL_SEARCH_FROM_FILE or DEAL_SEARCH_FROM_MEMORY.
    ///
    /// @avail ALL
    ///
    /// @param[in] mode DEAL_SEARCH_FROM_FILE or DEAL_SEARCH_FROM_MEMORY
    ///
    /// @warning This method is obsolete and does @em NOT affect deal search.
    /// @deprecated This method is deprecated and does @em NOT affect deal search.
    void CHASAPI  set_deal_search_mode(DEAL_SEARCH_MODE mode);


    // Call from main thread before creating other threads or deal scenario objects

    /// Sets the path of the deals' data files. The method only sets the path to the input parameter but does NOT check the validity of the input.
    ///
    /// @since 0.9.0
    /// @avail ALL
    ///
    /// @param[in] input_path This is the fully qualified path to the data files. The ending forward slash or backslash 
    ///                   (depending on the operating system) is optional. Linux format:" /temp/deals". Windows format:"c:\\tmp\\deals".
    /// @return None
    ///
    /// @sample
    ///     // Must be called before opening a deal.
    ///     // Data located in C:\data\chs
    ///     set_input_path("C:\\data\\chs");
    ///
    ///     void * ptid = NULL;
    ///     // Declare the main structure for the deal and initialize it.
    ///     CMO_STRUCT * pCmo = new CMO_STRUCT;
    ///     memset(pCmo, 0, sizeof(CMO_STRUCT));
    ///     // Deal is 02-010E (file is 02-001E.chs)
    ///     strcpy(pCmo->dealid, "02-010E");
    ///     // Use bucketed collateral
    ///     pCmo->actual_coll = 0;
    ///     // Set the "as of" date for Sept 9, 2009 (will open the most recent)
    ///     strcpy(pCmo->settlement_date, "09/09/09");
    ///     // Set the initial bond to blank. The first bond will be used.
    ///     strcpy(pCmo->bondid, "");
    ///     // Open the deal
    ///     if(0 > open_deal_ex(ptid, pCmo))
    ///     {
    ///        delete pCmo;
    ///        return -1;
    ///     }
    ///
    ///     close_deal_ex(ptid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @note
    /// This function only stores the input_path string. If the path is invalid, open_deal_ex() will return error.
    ///
    void CHASAPI set_input_path( const char * input_path );


    /// Sets the error handling level for processing: log error message or stop running.
    ///
    /// @since 0.9.0
    /// @avail ALL
    ///
    /// @param[in] level ERROR_HANDLING_LEVEL_LOG_IT or ERROR_HANDLING_LEVEL_STOP_CALCULATION
    ///             - ERROR_HANDLING_LEVEL_LOG_IT: Log error message to log file when error happens.
    ///             - ERROR_HANDLING_LEVEL_STOP_CALCULATION : Stop running when error happens.
    ///
    /// @return None
    ///
    /// @sample
    /// 	set_error_handling_level(ERROR_HANDLING_LEVEL_LOG_IT);
    ///
    /// 	void* ptid=NULL;
    /// 	// open deal
    /// @endsample
    ///
    void CHASAPI set_error_handling_level (ERROR_HANDLING_LEVEL level);


    /// This function sets the rule to handle the case where some interest rates are missing.
    ///
    /// @since 0.9.0
    /// @avail ALL
    ///
    /// @param[in] handling one of ::MISSING_INTEREST_RATES_HANDLING
    ///
    /// @return None
    ///
    /// @sample
    ///     set_missing_interest_rates_handling(MISSING_INTEREST_RATES_TREAT_AS_ERROR);
    /// @endsample
    ///
    void CHASAPI set_missing_interest_rates_handling(MISSING_INTEREST_RATES_HANDLING handling);


    /// Enables the multithreading protection for the same deal.
    ///
    /// @since 1.2.0
    /// @avail ALL
    ///
    /// @param[in] flag If 1, will enable the same deal protection.
    ///
    /// @return None
    ///
	/// @sample
    ///    enable_same_deal_multithreading(1);
    /// @endsample
    ///
    void CHASAPI enable_same_deal_multithreading(int flag);

    // Obtain Descriptive Information
    // Idenfity a deal/bond

    /// Returns the SDK deal and bond ID for an industry-standard bond identifier.
    ///
    /// @since 0.9.0
    /// @avail ALL
    ///
    /// @param[in] id The US or international bond identifier.
    /// @param[out] deal The deal ID. At least 13 characters must have been allocated.
    /// @param[out] bond The bond ID. At least 7 characters must have been allocated.
    ///
    /// @retval -2 Error - Invalid identifier
    /// @retval -1 Error - File missing
    /// @retval 0 Identifier not found
    /// @retval 1 Identifier found in US securities file
    /// @retval 2 Identifier found in international securities file
    ///
    /// @see get_moodys_id()
    ///
    /// @deprecated This method is deprecated, use get_moodys_id() instead.
    ///
    int CHASAPI get_markit_id(const char * id, char * deal, char * bond);


    /// Returns the SDK deal and bond ID for an industry-standard bond identifier.
    /// Comparing with get_markit_id(), this function reports error messages through its last 2 parameters.
    ///
    /// @since 0.9.0
    /// @avail ALL
    ///
    /// @param[in] id The US or international bond identifier.
    /// @param[out] deal The deal ID. At least 13 characters must have been allocated.
    /// @param[out] bond The bond ID. At least 7 characters must have been allocated.
    /// @param[out] err_buffer The error message.
    /// @param[in] err_length The length of the error message.
    ///
    /// @retval -99 Please examine err_buffer for error
    /// @retval   0 No error, but deal is not found
    /// @retval   1 Found deal successfully
    ///
    /// @see get_moodys_id()
    ///
    /// @deprecated This method is deprecated, use get_moodys_id() instead.
    ///
    int CHASAPI get_markit_id1(const char * id, char * deal, char * bond, char* err_buffer, int err_length);

    /// Retrieves the band information for the bond (pricing WAL, low speed and high speed).
    /// If NULL is passed for any item, that item will not be returned.
    ///
    /// @since 1.1.0
    /// @avail CDOnet, CHS, SFW
    ///	
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid A pointer to the name of the bond.
    /// @param[out] pricing_wal The weighted average life of the bond when the deal is priced.
    /// @param[out] low The low collar speed for the band.
    /// @param[out] high The high collar speed for the band.
    ///
    /// @retval  0 No error
    /// @retval <0 Error: Check that deal is open and the bondid is valid
    ///
    /// @sample
    ///     void * tid = NULL;
    ///     // Deal is already open
    ///
    ///     // Get pricing WAL only for bond A1
    ///     double wal;
    ///     int iR = get_bond_band(tid, "A1", &wal, NULL, NULL);
    /// @endsample
    /// 
    int CHASAPI get_bond_band(void * tid, const char * bondid, double *pricing_wal, double *low,
        double *high);

    /// Finds the bond for the specified index (0-based).
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] index 0-based index number of bond
    /// @param[in,out] b Pointer to a CCMO_BONDS_S structure allocated by the calling program.
    ///
    /// @return None. The bond stripped_id is set to ERROR if the bond index was invalid or the deal not open.
    /// 
    /// @sample
    ///     void * tid = NULL;
    ///     // Deal is already open
    ///
    ///     // Get a pointer to the first bond
    ///     CCMO_BONDS_S bond={};
    ///     get_bond_by_index_ex(tid, &bond, 0);
    /// @endsample
    /// 
    void CHASAPI get_bond_by_index_ex(void *tid, CCMO_BONDS_S * b, long index );

    /// Finds the bond for the specified bond name.
    /// 
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] id Name of the bond
    /// @param[out] b Pointer to a CCMO_BONDS_S structure allocated by the calling program.
    ///
    /// @return None. The bond stripped_id is set to ERROR if the bond was not found or the deal not open.
    /// 
    /// @note The stripped_id in the CCMO_BONDS_S structure is set to ERROR if the requested bond does not exist in the deal.
    /// 
    /// @sample
    ///     void * tid = NULL;
    ///     // Deal is already open
    ///
    ///     // Get a pointer to the bond named A1
    ///     CCMO_BONDS_S *bond;
    ///     int iR = get_bond_by_name_ex(tid, bond, "A1");
    /// @endsample
    ///
    void CHASAPI get_bond_by_name_ex(void *tid, CCMO_BONDS_S * b, const char * id );

    /// Retrieves the day count convention, business day/calendar rules, and currency for the specified bond.
    /// If use_code is true, the codes will be returned. If use_code is false, the description will be returned.
    /// This function requires that the file SDKCODES.TXT be in your deal directory. set_input_path() must have been called
    ///     before this function if the description is requested (use_code is false).
    /// 
    /// @since 1.2.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid A pointer to the name of the bond.
    ///  
    /// @param[in] use_code If true, the actual code will be returned. If false, descriptive information is returned.
    /// @param[out] day_count The day count rule used to compute interest (such as 30/360). See "Day Counts" paragraph for detail.
    ///      This should be allocated by the user with at least 26 characters.
    /// @param[out] bus_rules The rules for determining the payment date (such as next business day if the
    ///      payment date is not a business day).  See "Calendar rules" paragraph for detail. This should be allocated by the user with at least 26 characters.
    /// @param[out] currency The currency of the bond. See "Currencies" paragraph for detail. This should be allocated by the user with at least 26 characters.
    /// @retval   0 No error
    /// @retval  -1 Deal not open
    /// @retval  -2 Bond not found
    /// @retval -99 Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg() 
    /// 
    /// @note If the code description file (SDKCODES.TXT) is not in your deal directory, or set_input_path() has not been called,
    ///       all description requests will return "Invalid" as the description.
    ///
    /// @par Day Counts
    /// Day counts affect how interest is accrued for a transaction. The following day count codes are used.
    ///     | Code | Description | Notes |
    ///     | ---- | ----------- | ----- |
    ///     | 1 | Act/360 | Actual days in period, 360 days in year |
    ///     | 2 | Act/365 | Actual days in period, 365 days in year |
    ///     | 3 | Act/Act | Actual days in period, actual days in year |
    ///     | 4 | 30/360 | 30 days in month, 360 days in year |
    ///     | 5 | 30E/360 | 30 days in month, 360 days in year, last day in Feb = 30th. |
    /// 
    /// @par Calendar rules
    /// Calendar rules affect when a transaction settles. The following calendar rule codes are used.
    ///     | Code | Description | Comments |
    ///     | ---- | ----------- | ----- |
    ///     | A | Next Bus Day | If settlement is not a business day, settle on the next business day. |
    ///     | B | Next Bus Day In Month | If settlement is not a business day, settle on the next business day if it is in the current month. If not, settle on the prior business day. |
    ///     | C | Next BD Aft Serv Remit | Next business day after the master servicer remittance date. |
    ///     | P | Prev Bus Day | If settlement is not a business day, settle on the previous business day. |
    ///     | N | No Adjustment | Do not adjust for business days. |
    ///
    /// @par Currencies
    ///     | Code | Description |
    ///     | ---- | ----------- |
    ///     | E | Euros |
    ///     | F | Swiss Francs |
    ///     | K | Swedish Kronors |
    ///     | L | British Pounds |
    ///     | U | US Dollars |
    ///     | R | Russian Rubles |
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
    ///     // Get the codes for the day counts, business rules and currency
    ///     // bond A1
    ///     char dayCount[26];
    ///     char busRules[26];
    ///     char currency[26];
    ///     assert(0 == get_bond_day_cal_cur_ex(pDeal, "A1", 1, dayCount, busRules, currency));
    ///
    ///     // Get the descriptions for the day counts, business rules and currency
    ///     // for bond A1
    ///     assert(0 == get_bond_day_cal_cur_ex(pDeal, "A1", 0, dayCount, busRules, currency));
    ///
    ///     assert(0 == close_deal_ex(pDeal, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    short CHASAPI get_bond_day_cal_cur_ex(void *tid, const char * bondid, BOOLYAN use_code, char * day_count, 
        char * bus_rules, char * currency);

    /// Finds the 0-based index for the requested bond.
    /// 
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] id A pointer to the name of the bond.
    /// 
    /// @retval >=0 The zero-based (0) index of the bond
    /// @retval  -1 Deal not open
    /// @retval  -2 Bond not found
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @sample
    ///     void * tid = NULL;
    ///     // Deal is already open
    ///
    ///     int index = get_bond_index_ex(tid, "A2B");
    /// @endsample
    long CHASAPI get_bond_index_ex(void *tid, const char * id );

    /// Get additional information on a bond (segment, MACR or PO). 
    /// 
    /// @since 1.2.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] Bond A pointer to the name of the bond.
    /// @param[out] IsSeg If non-zero, the bond is a segment bond. Otherwise it is not.
    /// @param[out] IsMACR If non-zero, the bond is a MACR bond. Otherwise it is not.
    /// @param[out] IsPO  If non-zero, the bond is a PO bond. Otherwise it is not.
    /// 
    /// @retval   0 No error
    /// @retval  -1 Deal not open
    /// @retval  -2 Bond not found
    /// @retval -99 Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg() 
    ///
    /// @sample
    ///     void * ptid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     // Pre-condition: Deal is already opened
    ///
    ///     BOOLYAN isSeg = 0;
    ///     BOOLYAN isMacr = 0;
    ///     BOOLYAN isPo = 0;
    ///     int ret = get_bond_misc_ex(ptid, pCmo->bond.stripped_id, &isSeg, &isMacr, &isPo);
    ///     if(ret < 0)
    ///     {
    ///           //error handle
    ///     }
    /// @endsample
    /// 
    /// @sa
    /// - get_reremic_bond_misc()
    ///
    short CHASAPI get_bond_misc_ex(void *tid, const char *Bond,BOOLYAN *IsSeg,BOOLYAN *IsMACR,BOOLYAN *IsPO);

    /// Populates an array of bond structures (CCMO_BONDS_S) with descriptive information on all bonds in the currently open deal.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[out] all_bonds An allocated array of CCMO_BONDS_S structures. 
    ///
    /// @retval   0 No error 
    /// @retval  -1 Error - Deal not open
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @note The array all_bonds[] must be allocated to be of at least as long as the number of bonds in the deal. 
    ///        The number of bonds is set by open_deal_ex() and is found in the CMO_STRUCT num_bonds member.
    ///
	/// @sample
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(CMO_STRUCT));
    ///     strcpy(pCmo->dealid, "SAS059XS");
    ///
    ///     open_deal_ex(tid,pCmo);
    ///
    ///     std::vector<CCMO_BONDS_S> pBonds(pCmo->num_bonds+1);
    ///     view_all_bonds_ex(tid,&pBonds.front());
    ///
    ///     close_deal_ex(tid,pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    long CHASAPI view_all_bonds_ex(void *tid, CCMO_BONDS_S all_bonds[] );


    // Collateral information

    /// Gets collateral information for the specified piece of collateral.
    ///
    /// @since 1.2.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[out] p A pointer to a client-allocated CCMO_POOL_INFO structure.
    /// @param[in] index The 0-based index of the piece of collateral.
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
    ///     CCMO_POOL_INFO poolInfo;        
    ///     memset(&poolInfo, 0, sizeof(CCMO_POOL_INFO));    
    ///     get_pool_by_index_ex(tid, &poolInfo, 7);
    ///
    ///     close_deal_ex(tid,pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    void CHASAPI get_pool_by_index_ex(void *tid, CCMO_POOL_INFO * p, long index );


    /// Returns a pointer to the collateral specified by the index. This can be used to modify collateral characteristics.
    /// 
    /// @since 1.2.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] index The 0-based index of the piece of collateral.
    /// 
    /// @retval NULL Invalid collateral index
    /// @retval OTHER Pointer to the CCMO_POOL_INFO structure for that piece of collateral
    /// 
	/// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF00001");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(tid, pCmo));
    ///
    ///     CCMO_POOL_INFO *pPoolInfo = NULL;
    ///     pPoolInfo = get_pool_ptr_by_index_ex(tid, 0);
    ///     if(pPoolInfo == NULL)
    ///     {
    ///         /// Invalid collateral index
    ///     }
    ///
    ///     close_deal_ex(tid,pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    CCMO_POOL_INFO * CHASAPI get_pool_ptr_by_index_ex(void *tid, long index );

    /// Returns the pool information for either all collateral or the requested piece of collateral in a deal.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] index The 0-based index of the loan (-1 for all loans).
    /// @param[in] pool_size The size of the CCMO_POOL_INFO structure.
    /// @param[in] pool_ex_size The size of the CCMO_POOL_INFO_EX structure.
    /// @param[in] arm_size The size of the CCMO_ARM_INFO structure. Pass 0 if you do not want arm information.
    /// @param[out] all_colls A client-allocated array of CCMO_POOL_INFO structures.
    /// @param[out] all_colls_ex A client-allocated array of CCMO_POOL_INFO_EX structures.
    ///
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -3 Error - Invalid loan index
    /// @retval  -4 Error - Invalid size
    /// @retval  -5 Error - No output vector passed
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @note If all collateral is requested the arrays all_colls[] and all_colls_ex must be allocated to be at least as long as the value CMO_STRUCT.num_colls returned by open_deal_ex().
    ///       If arm_info is requested the arm member of each CCMO_POOL_INFO_EX must be allocated.
    ///
	/// @sample
    ///     void* ptid=NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"AL2010-A");
    ///
    ///     create_deal_scenario_object(&ptid,NULL,NULL,NULL);
    ///     open_deal_ex(ptid,&deal);
    ///
    ///     CCMO_POOL_INFO all_colls[100]={0};
    ///     CCMO_POOL_INFO_EX all_colls_ex[100]={0};
    ///     view_colls_ex(ptid,0,NULL,NULL,0,0,0);
    ///     view_colls_ex(ptid, 0, all_colls, all_colls_ex, sizeof(CCMO_POOL_INFO), sizeof(CCMO_POOL_INFO_EX), 1);
    ///
    ///     close_deal_ex(ptid,&deal);
    ///     release_deal_scenario_object(&ptid);
    /// @endsample
    ///
    long CHASAPI view_colls_ex(void *tid, short index, CCMO_POOL_INFO all_colls[],
        CCMO_POOL_INFO_EX all_colls_ex[], short pool_size,  short pool_ex_size, short arm_size );


    // Miscellaneous information

    /// This function obtains a pointer to the internal to the WSA API collateral iterator. This pointer should be passed to 
    ///     consecutive calls to get_next_collat() to retrieve collateral information.
    ///
    /// Keep in mind that the second call to this function for the same deal will invalidate all
    /// the collateral pointers retrieved from the WSA API during the first call.
    /// 
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier.
    /// @param[in] reremic_deal_id_or_null Pass 0 for main deal or remic name for collateral of the child deal 
    /// 
    /// @retval   Pointer to be passed to get_next_collat() function
    /// @retval   0 Error
    /// 
    /// @sample
    ///     void * tid = NULL;
    ///     // deal has been opened
    ///
    ///     MARKIT_POOL_INFO* coll_info =0;
    ///     void* coll_it = obtain_collat_iterator(tid, 0);
    ///     if(coll_it == 0)
    ///     {
    ///         std::cout << "Failure to start collat iteration " << get_deal_error_msg(tid) << std::endl;
    ///     }
    ///     while(coll_info = get_next_collat(tid,coll_it))
    ///     {   
    ///         // do what you need with collateral
    ///     }   
    /// @endsample
    /// 
    void* CHASAPI obtain_collat_iterator(void* tid, const char* reremic_deal_id_or_null);

    /// This function gets next collateral information using iterator obtained from calling obtain_collat_iterator().
    /// When iterator goes to the end of the collateral set, it returns NULL.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre obtain_collat_iterator() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier
    /// @param[in] collat_iterator Pointer to collateral iterator returned by calling obtain_collat_iterator().
    /// 
    /// @retval Pointer Pointer to the next collateral information
    /// @retval   0 No more information left
    /// 
    /// @sample
    ///     void * tid = NULL;
    ///     // deal has been opened
    ///
    ///     MARKIT_POOL_INFO* coll_info =0;
    ///     void* coll_it =obtain_collat_iterator(tid, 0);
    ///     if(coll_it == 0)
    ///     {
    ///         std::cout << "Failure to start collat iteration " << get_deal_error_msg(tid) << std::endl;
    ///     }
    ///     while(coll_info =  get_next_collat(tid,coll_it))
    ///     {
    ///         // do what you need with collateral
    ///     }
    /// @endsample
    /// 
    /// @note Function returns pointers to collateral information allocated by the API.  
    ///     These pointers will be valid until deal is closed or another call to obtain_collat_iterator()
    ///     function is made using the same parameters.
    ///     The iterator will be released when close_deal_ex() is called. The iterator will be overwritten when obtain_collat_iterator is called again.
    /// 
    /// @sa get_next_collat_for_managed_code
    MARKIT_POOL_INFO* CHASAPI get_next_collat(void* tid,void* collat_iterator);

    /// Returns the average collateral for this deal. 
    /// 
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] collat_iterator Pointer to collateral iterator returned by calling obtain_collat_iterator().
    /// @param[in] group_number Collateral group number, 0 for deal level.
    /// 
    /// @return Averaged collateral information.
    /// 
    /// @sample
    ///     void* tid=NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"AL2010-A");
    ///
    ///     int ret = open_deal_ex(tid,&deal);
    ///     if(ret == 0)
    ///     {
    ///         void* iter = obtain_collat_iterator(tid, 0);
    ///         MARKIT_POOL_INFO* aver_pool = get_average_collat(tid,iter,0); 
    ///         assert(aver_pool->periodicity == deal.periodicity);
    ///
    ///         close_deal_ex(tid,&deal);
    ///     }
    /// @endsample
    /// 
    /// @sa
    ///     * get_average_collat_for_managed_code()
    ///     * get_average_collat_by_bond()
    MARKIT_POOL_INFO* CHASAPI get_average_collat(void* tid,void* collat_iterator,int group_number); // only group 0 is implemented

    /// Returns a pointer to MARKIT_POOL_INFO which holds the average value of the related poolgroup(s) of the bond specified.
    /// 
    /// @since 1.5.0
    /// @avail CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] collat_iterator Pointer to collateral iterator returned by calling obtain_collat_iterator().
    /// @param[in] bondid The name of the bond whose related collateral poolgroups are to be averaged.
    ///
    /// @return Pointer to MARKIT_POOL_INFO that holds the average value of the poolgroup(s) related to the bond specified
    ///
    /// @note
    /// To get the related poolgroup(s) of a bond, call view_bond_coll_groups().
    /// See documentation for MARKIT_POOL_INFO for aggregation/averaging logic.
    ///
    /// @sample
    /// 	void* tid=NULL;
    /// 	CMO_STRUCT cmo;
    /// 	memset(&cmo, 0, sizeof(CMO_STRUCT));
    /// 	strcpy(cmo.dealid,"ACE06NC1");
    ///
    /// 	open_deal_ex(tid,&cmo);
    ///
    /// 	std::vector<CCMO_BONDS_S> pBonds(cmo.num_bonds);
    /// 	view_all_bonds_ex(tid, &pBonds.front());
    ///
    /// 	void* coll_it = obtain_collat_iterator(tid, 0);
    /// 	if (NULL != coll_it)
    /// 	{
    /// 	    // get the average of related pools of the first bond
    /// 	    MARKIT_POOL_INFO* related_pool_info = get_average_collat_by_bond(tid, coll_it, pBonds[0].stripped_id);
    /// 	}
    ///
    /// 	close_deal_ex(tid,&deal);
    /// @endsample
    ///
    /// @sa
    ///     * get_average_collat()
    ///     * view_bond_coll_groups()
    MARKIT_POOL_INFO* CHASAPI get_average_collat_by_bond(void* tid, void* collat_iterator, const char* bondid);

    /// This function is for managed code and gets the next collateral information using iterator obtained from calling obtain_collat_iterator(). 
    /// All structures should have been allocated by the caller. There is no memory allocated by SDK.
    /// 
    /// @since 1.0.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] collat_iterator Pointer to collateral iterator returned by calling obtain_collat_iterator().
    /// @param[in] sizeOfPpenArray Periods of prepayment penalty data requested.
    /// @param[out] usr_pool The user allocated buffer to hold pool data.
    /// @param[out] arm The user allocated buffer to hold adjustable rate information of usr_pool.
    /// @param[out] sched The user allocated buffer to hold payment schedule data of usr_pool.
    /// @param[out] prepayPenalty A pointer to a client-allocated array of MARKIT_PREPAY_PENALT in which the prepayment penalty data of usr_pool will be stored.
    /// @param[out] hasArm Set to 1 if arm is set. 
    /// @param[out] hasSched Set to 1 if sched is set. 
    /// @param[out] hasPpen Just set to 0.
    /// 
    /// @retval   1 End of collateral list.
    /// @retval   0 Collateral is successfully loaded.
    /// @retval  -1 collat_iterator is NULL.
    /// @retval  -2 usr_pool should be allocated by caller
    /// @retval  -3 arm should be allocated by caller
    /// @retval  -4 sched should be allocated by caller
    /// @retval  -6 hasArm should be allocated by caller
    /// @retval  -7 hasSched should be allocated by caller
    /// @retval  -8 hasPpen should be allocated by caller
    /// @retval -99 Error, Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @sample
    ///     void * tid = NULL;
    ///     //Deal has opened
    ///
    /// 	MARKIT_POOL_INFO pool;
    /// 	CCMO_ARM_INFO arm;
    /// 	MARKIT_PAYMENT_SCHEDULE shed;
    /// 	MARKIT_PREPAY_PENALTY prepayPenalty[10];
    /// 	int has_Arm;
    /// 	int has_Sched;
    /// 	int hasPpen;
    /// 	int coll_info;
    /// 	
    /// 	void*coll_it=obtain_collat_iterator(tid,0);
    /// 	if(coll_it==0)
    /// 	{
    /// 		std::cout<<"Failure to start collat iteration"<<get_deal_error_msg(tid_<<std::endl;
    ///         return;
    /// 	}
    /// 	int iret=get_next_collat_for_managed_code(tid,coll_it,&pool,&arm,&sched,&prepayPenalty,10,&has_Arm,&has_Sched,&hasPpen);
    ///     if (0==iret)
    /// 	{
    /// 		//do what you want with pool
    /// 		If(has_Arm)
    /// 		{
    /// 		    //do what you want with arm
    /// 		}
    /// 		If(has_Sched)
    /// 		{
    /// 		    //do what you want with sched
    /// 		}
    /// 		If(has_Ppen)
    /// 		{
    /// 		    //do what you want with prepayPenalty
    /// 		}
    /// 	}
    ///     else if (iret<0)
    ///     {
    ///         //Error handling
    ///     }
    /// @endsample
    ///
    /// @sa get_next_collat()
    ///
    int CHASAPI get_next_collat_for_managed_code(void* tid, void* collat_iterator,MARKIT_POOL_INFO* usr_pool, CCMO_ARM_INFO* arm, 
        MARKIT_PAYMENT_SCHEDULE* sched, MARKIT_PREPAY_PENALTY prepayPenalty[],int sizeOfPpenArray, int* hasArm, int* hasSched, int* hasPpen);

    /// This function is for managed code and gets the average collateral information using iterator obtained from calling obtain_collat_iterator(). 
    /// All structures should have been allocated by the caller. There is no memory allocated by SDK.
    /// 
    /// @since 1.4.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] collat_iterator Pointer to collateral iterator returned by calling obtain_collat_iterator().
    /// @param[in] group_number Collateral group number, 0 for deal level.
    /// @param[in] sizeOfPpenArray Periods of prepayment penalty data requested.
    /// @param[out] usr_pool The user allocated buffer to hold pool data.
    /// @param[out] arm The user allocated buffer to hold adjustable rate information of usr_pool.
    /// @param[out] sched The user allocated buffer to hold payment schedule data of usr_pool.
    /// @param[out] prepayPenalty A pointer to a client-allocated array of MARKIT_PREPAY_PENALT in which the prepayment penalty data of usr_pool will be stored.
    /// @param[out] hasArm Set to 1 if arm is set. 
    /// @param[out] hasSched Set to 1 if sched is set. 
    /// @param[out] hasPpen Just set to 0.
    /// 
    /// @retval   0 Collateral is successfully loaded or collat_iterator is NULL.
    /// @retval  -1 usr_pool should be allocated by caller
    /// @retval  -2 arm should be allocated by caller
    /// @retval  -3 sched should be allocated by caller
    /// @retval  -4 hasArm should be allocated by caller
    /// @retval  -5 hasSched should be allocated by caller
    /// @retval  -6 hasPpen should be allocated by caller
    /// @retval -99 Error, Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @sample
    ///     void * tid = NULL;
    ///     //Deal has opened
    /// 	
    ///     void*coll_it=obtain_collat_iterator(tid,0);
    ///     if(coll_it==0)
    ///     {
    /// 	    std::cout<<"Failure to start collat iteration"<<get_deal_error_msg(tid)<<std::endl;
    ///         return;
    ///     }
    ///
    ///     MARKIT_POOL_INFO pool;
    ///     CCMO_ARM_INFO arm;
    ///     MARKIT_PAYMENT_SCHEDULE shed;
    ///     MARKIT_PREPAY_PENALTY prepayPenalty[10];
    ///     int has_Arm;
    ///     int has_Sched;
    ///     int hasPpen;
    ///     //Get pool group 1 average info
    ///     int iret = get_average_collat_for_managed_code(tid,coll_it,1,&pool,&arm,&sched,&prepayPenalty,10,&has_Arm,&has_Sched,&hasPpen);
    ///     if (iret < 0)
    ///     {
    ///         //Error handling
    ///     }
    ///     //do what you want with average pool
    ///
    ///     memset(&pool, 0, sizeof(MARKIT_POOL_INFO));
    ///     memset(&arm, 0, sizeof(CCMO_ARM_INFO));
    ///     memset(&sched, 0, sizeof(MARKIT_PAYMENT_SCHEDULE));
    ///     memset(prepayPenalty, 0, 10*sizeof(MARKIT_PREPAY_PENALTY));
    ///     has_Arm = 0;
    ///     has_Sched = 0;
    ///     hasPpen = 0;
    ///     //Get deal level average collateral info
    ///     iret = get_average_collat_for_managed_code(tid,coll_it,0,&pool,&arm,&sched,&prepayPenalty,10,&has_Arm,&has_Sched,&hasPpen);
    ///     if (iret < 0)
    ///     {
    ///         //Error handling
    ///     }
    ///     //do what you want with average pool
    /// @endsample
    ///
    /// @sa get_average_collat()
    ///
    int CHASAPI get_average_collat_for_managed_code(void* tid,void* collat_iterator,int group_number,MARKIT_POOL_INFO* usr_pool, CCMO_ARM_INFO* arm, 
        MARKIT_PAYMENT_SCHEDULE* sched, MARKIT_PREPAY_PENALTY prepayPenalty[],int sizeOfPpenArray, int* hasArm, int* hasSched, int* hasPpen);

    /// This function will return the number of collateral groups in a deal and will populate the user provided array with group numbers.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier.
    /// @param[in,out] groups_array A user allocated array to which collateral group numbers will be written.
    /// @param[in] groups_array_length The size of the groups_array. To make sure the API does not overrun user's memory.
    /// @param[out] total_groups The WSA API will provide total group number in the deal. That might be different from the value returned by the function if user did not allocate enough space.
    ///
    /// @retval number The number of groups returned by the function
    /// @retval  <0 Error - Check error message
    ///
    /// @sample
    ///     void * pDeal = NULL;
    ///     // deal has already opened.
    ///
    ///     int numGroups(0);
    ///     assert(0 == view_coll_groups(pDeal, NULL, NULL, &numGroups));  // get the total groups number
    /// 
    ///     std::vector<int> groups(numGroups,0);
    ///     int total_groups = 0;
    ///     view_coll_groups(pDeal, &groups[0], numGroups, &total_groups);
    /// @endsample
    ///
    /// @note
    ///    If groups_array is NULL and groups_array_length is 0, total groups number will be returned by total_groups parameter.
    int CHASAPI view_coll_groups(void* tid, int groups_array[], int groups_array_length, int* total_groups);

    /// Returns the number of related poolgroups of the specified bond and populates an array of collateral group IDs related to the specified bond.
    /// 
    /// @since 1.5.0
    /// @avail CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null Reremic deal id for a child deal, otherwise null.
    /// @param[in] groups_array_length Size of array groups_array
    /// @param[in] bondid A pointer to the name of the bond.
    /// @param[out] groups_array A pointer to the array of collateral group IDs related to the specified bond.
    /// @param[out] total_groups The number of groups related to the specified bond.
    /// 
    /// @return The actual number of groups returned in the array.
    /// 
    /// @sample
    /// 	void *tid = NULL;
    /// 	CMO_STRUCT *pCmo = new CMO_STRUCT();
    /// 	memset(pCmo, 0, sizeof(CMO_STRUCT));
    ///
    /// 	set_input_path("C:\\");
    /// 	if(open_deal_ex(tid, pCmo)!=0)
    /// 	{
    /// 	    delete pCmo;
    /// 	    return;
    /// 	}
    ///
    /// 	int groups[100]={0};
    /// 	int totalgroups=0;
    /// 	int num_group_ret=view_bond_coll_groups(tid, NULL, "A1", groups, 100, &totalgroups));
    /// 	for(int i=0;i<totalgroups; i++)
    /// 	{
    /// 	    //do what you want to do with groups[i]
    /// 	}
    ///
    /// 	close_deal_ex(tid, pCmo);
    /// 	delete pCmo;
    /// 	pCmo = NULL;
    /// @endsample
    ///
    /// @note total_groups and the return value can be different if:
    ///     * The array provided is smaller than the actual number of pool groups supporting the bond.
    ///         In this case, total_groups might be bigger than the return value.
    ///     * For SFW deal, for some given pool groups, there are several smaller 'virtual' pool groups
    ///         that belong to those groups. In this case, total_groups (representing the number of larger
    ///         pool groups) might be smaller than the return value.
    ///
    /// @sa
    ///     * view_coll_groups()
    ///     * get_average_collat_by_bond()
    int CHASAPI view_bond_coll_groups(void* tid,const char* reremic_deal_id_or_null,const char * bondid,int groups_array[], int groups_array_length, int* total_groups);



    /// This function replaces the collateral allocated  by the WSA API during loading stage of the deal with collateral prepared by the user. It allows the user to perform customized aggregation when use in combination with collateral iterator.
    /// 
    /// @since 1.2.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario identifier obtained when the dso was created.
    /// @param[in] reremic_deal_id_or_null  0 for parent deal or name of the child deal
    /// @param[in] collat_array  User allocated array of collateral information
    /// @param[in] collat_array_size Number of collaterals in the array
    /// 
    /// @retval  0 No error
    /// @retval <0 Error: Call the get_deal_error_msg() function for more details
    /// 
    /// @note This function might be called after deal is loaded to replace collateral read from the files with the one provided by user. User has to provide collateral for every group in the deal.
    /// User is responsible for allocating and releasing the memory passed to the collat_array parameter.  The WSA API will allocate its own memory and copy information provided by the user.
    /// 
	/// @sample 
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(CMO_STRUCT));
    ///     char deal_id[] = "AL2010-A";
    ///     int len = sizeof(deal_id) / sizeof(char);
    ///     memcpy(pCmo->dealid,deal_id,len);
    ///
    ///     replace_collateral(tid, NULL, NULL, 0);
    ///
    ///     open_deal_ex(tid,pCmo);
    ///
    ///     std::vector<MARKIT_POOL_INFO*> collat_vector;
    ///     MARKIT_POOL_INFO* coll_info =0;
    ///     void* coll_it = obtain_collat_iterator(tid, 0);
    ///
    ///     while(coll_info = get_next_collat(tid,coll_it))
    ///     {
    ///         if(!coll_info->arm && coll_info->type != REMIC)
    ///         {
    ///             int agency_type = coll_info->type;
    ///             double wacAddition = 0;
    ///             switch(agency_type)
    ///             {
    ///             case GNMA1:
    ///                 wacAddition = 0.5;
    ///                 break;
    ///             case GNMA2:
    ///                 wacAddition = 1.5;
    ///                 break;
    ///             default:
    ///                 wacAddition = 2.5;
    ///                 break;
    ///             }
    ///             wacAddition /= 100;
    ///             double wac = coll_info->psa_coupon + wacAddition;
    ///             coll_info->wac = wac;
    ///             coll_info->gross_coupon = wac;
    ///             int wala = coll_info->wala;
    ///             coll_info->wala =0;
    ///             if(coll_info->prin_lockout > 0)
    ///                 coll_info->prin_lockout += wala;
    ///         }
    ///         collat_vector.push_back(coll_info);
    ///     }
    ///
    ///     replace_collateral(tid, NULL, &collat_vector[0], -1);
    ///     collat_vector.clear();
    ///
    ///     close_deal_ex(tid,pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI replace_collateral(void* tid, const char* reremic_deal_id_or_null,MARKIT_POOL_INFO* collat_array[], int collat_array_size);


    /// Returns repline pool index for either all collateral or the requested piece of collateral in a deal
    ///
    /// repline_array element can be:
    /// * >=0, repline pool index
    /// * -1, no matching repline pool
    ///
    /// @since 2.1.1
    /// @avail CHS
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[in] loan_index The 0-based index of the loan (-1 for all loans).
    /// @param[in,out] repline_array A user allocated array to which repline index will be written.
    /// @param[in] repline_array_length The size of the repline_array. To make sure the API does not overrun user's memory.
    ///
    /// @retval >=0 Actual number of repline indices returned
    /// @retval  -1 Error - Deal not opened
    /// @retval  -3 Error - Invalid loan index
    /// @retval  -4 Error - Invalid output array size
    /// @retval  -5 Error - Invalid output array
    /// @retval  -6 Error - Actual pool data not loaded
    /// @retval  -7 Error - Repline info not available/loaded
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @sample
    ///     void* ptid=NULL;
    ///     CMO_STRUCT deal={};
    ///     strcpy(deal.dealid, "3437E");
    ///     deal.actual_coll=1;
    ///
    ///     open_deal_ex(ptid, &deal);
    ///     // deal is already open.
    ///
    ///     int repline_index[10]={};
    ///     int ret_val = get_repline_index_list(ptid, NULL, -1, repline_index, 10);
    ///     assert(ret_val > 0);
    ///     // value of repline_index[i] is the repline pool index of pool i
    ///     // done
    ///
    ///     close_deal_ex(ptid, &deal);
    /// @endsample
    ///
    /// @note If all collateral is requested, repline_array must be allocated to be at least as long as the value CMO_STRUCT.num_colls returned by open_deal_ex().
    ///
    int CHASAPI get_repline_index_list(void *tid, const char* reremic_deal_id_or_null, int loan_index, int repline_array[], int repline_array_length);

    /// Returns ID(s), such as CUSIP, for either all collateral or the requested piece of collateral in a deal
    ///
    /// @since 2.1.1
    /// @avail CHS
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[in] loan_index The 0-based index of the loan (-1 for all loans).
    /// @param[in] id_type Collateral id type. It is case-insensitive and should be:
    ///            - "CUSIP"
    /// @param[in,out] id_array A user allocated array to which IDs will be written.
    /// @param[in] id_array_length The size of the id array. To make sure the API does not overrun user's memory.
    ///
    /// @retval   >=0 Actual number of IDs returned
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Id type not support
    /// @retval  -3 Error - Invalid loan index
    /// @retval  -4 Error - Invalid output array size
    /// @retval  -5 Error - Invalid output array
    /// @retval  -6 Error - Actual pool data not loaded
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @sample
    ///     void* ptid=NULL;
    ///     CMO_STRUCT deal={};
    ///     strcpy(deal.dealid, "3437E");
    ///     deal.actual_coll=1;
    ///
    ///     open_deal_ex(ptid, &deal);
    ///     // deal is already open.
    ///
    ///     std::vector<char> id_buf(deal.num_colls*10);
    ///     std::vector<char*> id(deal.num_colls);
    ///     for(int i = 0; i<deal.num_colls; ++i)
    ///     {
    ///         id[i] = &id_buf[i*10];
    ///     }
    ///     int ret_val = get_collateral_id_ex(ptid, NULL, -1, "CUSIP", &id.front(), deal.num_colls);
    ///     assert(ret_val > 0);
    ///     // value of id[i] is the id of pool i
    ///     // done
    ///
    ///     close_deal_ex(ptid, &deal);
    /// @endsample
    ///
    /// @note If all collateral is requested, IDs array must be allocated to be at least as long as the value CMO_STRUCT.num_colls returned by open_deal_ex().
    ///
    int CHASAPI get_collateral_id_ex(void *tid, const char* reremic_deal_id_or_null, int loan_index, const char * id_type, char* id_array[], int id_array_length);

    /// Converts DAYT date to string (mm/dd/yy), only for 30/360 day count convention.
    /// 
    /// @since 0.9.0
    /// @avail ALL
    ///
    /// @param[in] julian The date in the DAYT format.
    /// @param[out] temp A pre-allocated pointer of at least 9 characters for the date (mm/dd/yy).
    /// 
    /// @retval date in mm/dd/yy format
    /// 
    /// @sample
    ///     // Gets the interest start date of the bond the deal was
    ///     // opened with. pCmo is the pointer to the CMO_STRUCT.
    ///     char temp[9];
    ///     dayt_to_str(pCmo->bond.date,temp);
    /// @endsample
    /// 
    /// @note This function works for 30/360 day count convention.
    ///       For other conventions, use dayt_to_str_with_day_count() instead.
    ///       At least 9 characters must be allocated for date.
    /// @warning The cutoff year for mm/dd/yy is 79, so yy 80 is 1980 and yy 79 is 2079.
    ///
    /// @sa
    /// - dayt_to_str_with_day_count - Allows the user to convert the DAYT format to a string date for different day count.
    char* CHASAPI dayt_to_str( DAYT julian, char * temp );

    /// Converts DAYT date to string (mm/dd/yy) with given day count convention.
    /// 
    /// @since 1.6.0
    /// @avail ALL
    ///
    /// @param[in] julian The date in the DAYT format.
    /// @param[in] dayCount The day count rule. The day count rule must be one of:
    ///                             - ::DAYCOUNT_ACTUAL_360
    ///                             - ::DAYCOUNT_ACTUAL_365
    ///                             - ::DAYCOUNT_ACTUAL_ACTUAL
    ///                             - ::DAYCOUNT_30_360
    ///                             - ::DAYCOUNT_30_365
    ///                             - ::DAYCOUNT_30_360E
    /// @param[out] temp A pre-allocated pointer of at least 9 characters for the date (mm/dd/yy).
    /// 
    /// @retval date in mm/dd/yy format
    /// 
    /// @sample
    ///     // Gets the interest start date of the bond the deal was
    ///     // opened with. pCmo is the pointer to the CMO_STRUCT.
    ///     // tid is the pointer to the deal-scenario object (dso).
    ///     MARKIT_BOND_INFO bond_info;
    ///     int ret = get_bond_info_by_tranche(tid, NULL, pCmo->bond.stripped_id, &bond_info);
    ///     if(ret != 0)
    ///     {
    ///         //error handling
    ///     }
    ///
    ///     char temp[9];
    ///     dayt_to_str_with_day_count(pCmo->bond.date, temp, bond_info.day_count);
    /// @endsample
    /// 
    /// @note At least 9 characters must be allocated for date.
    /// @warning The cutoff year for mm/dd/yy is 79, so yy 80 is 1980 and yy 79 is 2079.
    ///
    /// @sa
    /// - dayt_to_str - Allows the user to convert the DAYT format to a string date for,only use for the day cout is 30/360.
    char* CHASAPI dayt_to_str_with_day_count( DAYT julian, char * temp, const int dayCount);


    /// Indicates whether the specified deal has underlying deals(REMICS) in its assets. 
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// 
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// 
    /// @retval  2 Underlying deal(s) and regular collateral
    /// @retval  1 Underlying deal(s), no regular collateral
    /// @retval  0 Regular collateral only
    /// @retval -1 Error - Deal not opened or the tid is invalid
    /// @retval -4 Error - No collateral
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(CMO_STRUCT));
    ///     char deal_id[] = "AL2010-A";
    ///     int len = sizeof(deal_id) / sizeof(cahr);
    ///     memcpy(pCmo->dealid, deal_id,len);
    ///
    ///     set_input_path("C:\\deals");
    ///     assert(0 == open_deal_ex(tid, pCmo));
    ///     // Deal is opened.
    ///
    ///     assert(0 == deal_has_underlying_deal(tid));
    ///
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    /// 
    int CHASAPI deal_has_underlying_deal(void *tid);


    /// Retrieves information about the deal call.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The tid identifier.
    /// @param[out] CallDate The earliest call date (mm/dd/yy or mm/dd/yyyy). The calling program must allocate this with at 
    ///                 least eleven (11) characters.
    /// @param[out] CallPct The required percent for the deal to be callable.
    /// @param[out] CallPctCalc Type of call balance. Should be the pointer to one of CLEAN_UP_CALL_BALANCE_TYPE.
    ///
    /// @retval get_cleanup_call_ex Type of cleanup call 
    /// @retval CLEANUP_CALL_NONE No cleanup call
    /// @retval CLEANUP_CALL_DATE Callable by date
    /// @retval CLEANUP_CALL_PERCENT Callable by percent
    /// @retval CLEANUP_CALL_EITHER Callable by date or percent
    /// @retval CLEANUP_CALL_BOTH Callable by date and percent
    /// @retval -1 Deal not open
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
	/// @sample
	///     char callDate[11] = "03/15/16";
    ///     double callPct = 0.1;
    ///     int callPctCalc;
    ///     int rVal =get_cleanup_call_ex(tid, callDate, &callPct, &callPctCalc);
    /// @endsample
    ///
    int CHASAPI get_cleanup_call_ex(void *tid, char* CallDate,double *CallPct,int *CallPctCalc);


    /// Gets collateral update dates from an update file(*.upd) and stores them in the output parameter "UpdDate".
    /// The return value of this method will be the number of dates that stores in "UpdDate".
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @retval >=0 Number of dates returned
    /// @retval  -1 Other error
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] szArchiveName Name of Moody's Analytics Deal file, *.chs or *.sfw. E.g. MA_Deal_File or MA_Deal_File.chs
    /// @param[out] UpdDate Pointer to a client-allocated array of update dates (YYYYMM).
    ///
    /// @note 
    ///     The size of the output parameter must be greater than:
    ///         - The number of months between the current date and the deal settlement date, if which is smaller than MAX_PERIODS(==612).
    ///         - MAX_PERIODS (==612), if MAX_PERIODS is smaller than the number of months between the current date and the deal settlement date.
    ///
    /// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT cmo={};
    ///     strcpy(cmo.dealid, "AAM0401");
    ///
    ///     set_input_path("C:/deals");
    ///     set_engine_preference(PICK_CHS_ENGINE_FOR_MAPPED_DEALS);
    ///     open_deal_ex(tid, &cmo);
    ///     // Deal is opened.
    ///
    ///     int UpdDate[MAX_PERIODS] = {0};
    ///     int nResults = get_dates_from_upd_ex(tid,cmo.dealid, UpDate);
    ///     // nResults should be equal to 43.
    ///     // UpdDate[nResults] should be equal to 0.
    ///
    ///     close_deal_ex(tid, &cmo);
    /// @endsample
    ///
    int  CHASAPI get_dates_from_upd_ex(void *tid, char *szArchiveName,int UpdDate[]);

    /// Populates the user allocated buffer with surveillance data for a specific month from the deal file, if the surveillance data is available.
    /// Please also refer to MARKIT_DEAL_INFO for details.
    /// 
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal id or null if not reremic.
    /// @param[out] deal_info Fields populated with deal information. It will be stored in MARKIT_DEAL_INFO.
    ///
    /// @retval   0 Call succeeded - data has been copied to user_buffer and the length is actual_size, the size of MARKIT_DEAL_INFO.
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @sample
    ///     void* ptid=NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0,sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"BAFC08R2");
    ///
    ///     open_deal_ex(ptid,&deal);
    ///     // Pre-condition: Deal is already opened
    ///    
    ///     MARKIT_DEAL_INFO dealInfo;
    ///     memset(&dealInfo, 0, sizeof(MARKIT_DEAL_INFO));
    ///     int ret = get_deal_info(ptid, null, &dealInfo);
    ///     if(ret < 0)
    ///     {
    ///           std::cerr < "Error:" << get_deal_error_msg(ptid) << std::endl;
    ///      }
    ///
    ///     close_deal_ex(ptid,&deal);
    /// @endsample
    ///
    long CHASAPI get_deal_info(void *tid,const char* reremic_deal_id_or_null,MARKIT_DEAL_INFO* deal_info);

    /// This retrieves the collateral group info given a group number.
    ///
    /// @since 1.2.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// 
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null If reremic deal, this is the id, otherwise null
    /// @param[in] group_number The group number
    /// @param[out] group_info A pointer to the user allocated structure for group info.
    /// 
    /// @retval 0 Success 
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
	/// @sample
    ///  void * tid = NULL;
    ///  // Deal is already opened
    ///  
    ///  // get the total groups number
    ///  int numGroups(0);
    ///  view_coll_groups(tid, NULL, NULL, &numGroups);
    ///  
    ///  //get group info
    ///  for(int i = 1; i<=numGroups; i++){
    ///  	MARKIT_GROUP_INFO groupInfo;
    ///  	get_group_info(tid, NULL,i,&groupInfo);
    ///  }
    /// @endsample
    ///
    long CHASAPI get_group_info(void *tid,const char* reremic_deal_id_or_null,
        int group_number, MARKIT_GROUP_INFO* group_info);


    /// Retrieves the category of deal (Agency vs Whole_Loan) and type of deal within that category. 
    /// Information is only retrieved if a non-null pointer is passed.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[out] Issuer A pointer to a character string used to return the deal type. The string must be at least 80 
    ///               characters and allocated by the user. If NULL is passed, the variable Issuer will not be returned.
    /// @param[out] Type A pointer to a character string used to return the deal category. The string must be at least 
    ///             26 characters and allocated by the user. If NULL is passed, the variable Type will not be returned.
    ///
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Check that output parameters are fully allocated
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @sample
    ///     void* ptid=NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"BAFAC08R2");
    ///
    ///     open_deal_ex(ptid,&deal);
    ///     // Deal is already opened.
    ///     
    ///     char issuer[80]={0};
    ///     char type[80]={0};
    ///     int iret=get_deal_issuer_type(ptid,issuer,type);
    ///
    ///     close_deal_ex(ptid,&deal);
    /// @endsample
    int CHASAPI get_deal_issuer_type(void *tid, char * Issuer, char * Type);

    /// This retrieves the bond's historical factors and coupons where available, in descending order by date,
    ///     starting with the date the deal is opened "as of".
    /// 
    /// @since 1.1.0
    /// @avail CDOnet, CHS, SFW
    ///	
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] cmos Pointer to the CMO_STRUCT used to open the deal.
    /// @param[in] bondid The name of bond for which information is requested.
    /// @param[out] hist_factor A pointer to a client-allocated vector of doubles which will return the historical factors. The vector must be large enough to hold MAX_PERIODS values.
    /// @param[out] hist_coupon A pointer to a client-allocated vector of doubles which will return the historical coupons. The vector must be large enough to hold MAX_PERIODS values.
    /// 
    /// @retval >=0 Number of periods
    /// @retval  -1 Error - Deal not opened
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
	/// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "BAA03009");
    ///     
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(tid, pCmo));
    ///
    ///     double hist_factor[MAX_PERIODS] = {0};
    ///     double hist_coupon[MAX_PERIODS] = {0};
    ///     char* bondid = "1CB1";
    ///     int ret = get_hist_data_ex(tid,pCmo,bondid,hist_factor,hist_coupon);
    ///     if(ret < 0)
    ///     {
    ///         //error handle
    ///     }
    ///
    ///     close_deal_ex(tid,pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @note The latest information returned is for the settlement date specified when opening the deal. If no date was specified all available information is returned.
    ///       Factors and coupons of -1 indicate that information is not available for that date.
    int CHASAPI get_hist_data_ex(void *tid, CMO_STRUCT* cmos,char* bondid, double hist_factor[], double hist_coupon[]);

    /// This retrieves the bond's historical principal losses and paid interest where available,
    ///     in descending order by date, starting with the date the deal is opened "as of".
    /// 
    /// @since 1.3.0
    /// @avail CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] cmos Pointer to the CMO_STRUCT used to open the deal.
    /// @param[in] bondid The name of bond for which information is requested.
    /// @param[out] date A pointer to a client-allocated vector of integers which will return the historical date.
    ///     It is in descending order from the settlement date specified.  The output format is yyyymm.
    /// @param[out] principal_losses A pointer to a client-allocated vector of doubles which will return the historical principal losses. 
    ///     The vector must be large enough to hold MAX_PERIODS values.
    /// @param[out] paid_interest A pointer to a client-allocated vector of doubles which will return the historical paid interest.
    ///     The vector must be large enough to hold MAX_PERIODS values.
    /// 
    /// @retval >=0 Number of periods
    /// @retval  -1 Error - Deal not opened
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @note The latest information returned is for the settlement date specified when opening the deal.
    ///     If no date was specified all available information is returned.
    ///     For CHS deals, values of -1 indicate that information is not available for that date;
    ///     for SFW deals, values of -9.123E+19 indicate that information is not available for that date. 
    ///     For CHS deals, paid_interest is calculated from hist_coupon and hist_factor, and hist_principal loss is always 0. 
    /// 
	/// @sample
    ///      void* tid = NULL;
    ///      CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///      memset(pCmo, 0, sizeof(*pCmo));
    ///      strcpy(pCmo->dealid, "AL2010-A");	
    ///
    ///      set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///      assert(0 == open_deal_ex(tid, pCmo));
    ///
    ///      int hist_date[MAX_PERIODS] = {0};
    ///      double hist_prinlosses[MAX_PERIODS] = {0};
    ///      double hist_paidint[MAX_PERIODS] = {0};
    ///      char* bondid = "A";
    ///      int ret = get_hist_data_ex1(tid,pCmo,bondid,hist_date,hist_prinlosses,hist_paidint);
    ///      if(ret < 0)
    ///      {
    ///      	//error handle
    ///      }
    ///
    ///     close_deal_ex(tid,pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI get_hist_data_ex1(void* tid, CMO_STRUCT* cmos, char* bondid, int date[], double principal_losses[], double paid_interest[]);


    /// Returns the path to the SDK Deals. The path is stored in the static variable, input_subdiretory.
    /// The initial value of this variable is "\n" and can be reset by set_input_path().
    ///
    /// @since 0.9.0
    /// @avail ALL
    ///
    /// @pre set_input_path() has been called.
    ///
    /// @return The path to the SDK deals.
    ///
    /// @sample
    ///     set_input_path("C:/Test");
    ///     const char *path = get_input_path();
    /// @endsample
    ///
    /// @note
    /// The input_path is initials with "\n". If the user don’t call set_input_path() function, get_input_path() will return "\n".
    ///    
    const char * CHASAPI get_input_path();


    /// Returns the number of remain periods to maturity of collateral. This can be used to increase efficiency 
    /// by only processing periods where there may be activity (both in setting scenarios and processing cashflows).
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre run_deal_ex() has been called.
    /// 
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    ///
    /// @retval >=0 Number of periods
    /// @retval  -1 Error - Deal not opened
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @sample
    ///     set_input_path("C:/Deals");
    ///     set_engine_preference(PICK_CHS_ENGINE_FOR_MAPPED_DEALS);
    ///
    ///     void* tid=NULL;
    ///     CMO_STRUCT cmo={};
    ///     strcpy(cmo.dealid, "AAM0401");
    ///     open_deal_ex(tid, &cmo);
    ///     // Deal is already opened
    ///
    ///     int longest=get_longest_ex(tid);
    ///     char cf_date[11]={};
    ///     for (int i=0; i<=longest; ++i)
    ///     {
    ///         char* msg=get_cf_date(i, cf_date, tid);
    ///         std::cout << msg << std::endl;
    ///     }
    ///
    ///     close_deal_ex(tid, &cmo);
    /// @endsample
    ///
    long CHASAPI get_longest_ex(void *tid);

    /// This retrieves the names and/or descriptions of the triggers in the deal.
    /// 
    /// @since 1.2.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[out] trigger_names A pointer to a client-allocated array of character strings in which the names of the triggers will be stored. 21 characters should be allocated for each string. Only filled in if it is not NULL.
    /// @param[out] trigger_descs A pointer to a client-allocated array of character strings in which the descriptions of the triggers will be stored. 1025 characters should be allocated for each string. Pass NULL if descriptions are not required. Only filled in if not NULL.
    /// 
    /// @retval >=0 Number of triggers
    /// @retval  -1 Error - Deal not opened
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @note Pass NULL for  trigger_names and trigger_descs to get just the number of triggers.
    ///     Triggers are conditions that affect the custom paydown rules.
    ///     In each period the trigger will either fail (condition no met – no action) or pass (action taken).
    ///     The value(s) of one of these variables can be obtained by calling get_trigger_status() after running the deal (run_deal_ex() ).
    ///     The triggers can be overridden by calling set_trigger_override before calling run_deal_ex().
    /// 
	/// @sample
    ///    void* tid = NULL;
    ///    CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///    memset(pCmo, 0, sizeof(*pCmo));
    ///    strcpy(pCmo->dealid, "AB05HE3");
    ///
    ///    set_engine_preference(PICK_CHS_ENGINE_FOR_MAPPED_DEALS);
    ///    open_deal_ex(tid, pCmo);
    ///
    ///    int trg_num = get_triggers_avail(tid, NULL, NULL);
    ///    if(trg_num>0)
    ///    {
    ///        char *name_buf = (char*)malloc(trg_num*21);
    ///        char **names = (char**)malloc(trg_num * sizeof(char*));
    ///        for(int i = 0; i<trg_num; ++i)
    ///        {
    ///            names[i] = (char*)malloc(21);
    ///        }
    ///        signed char *status_buf = (signed char*)malloc(trg_num*MAX_PERIODS);
    ///        signed char **status = (signed char**)malloc(trg_num * sizeof(signed char*));
    ///        for(int i = 0; i<trg_num; ++i)
    ///        {
    ///            status[i] = (signed char*)malloc(MAX_PERIODS);
    ///        }
    ///        for(int i = 0; i<trg_num; i++ )
    ///        {
    ///            names[i] = &name_buf[i*21];
    ///            status[i] = &status_buf[i*MAX_PERIODS];
    ///        }
    ///
    ///        int triggers = get_triggers_avail(tid, names, NULL);
    ///        if(triggers <0)
    ///        {
    ///            //error handle;
    ///        }		
    ///    }
    ///
    ///     close_deal_ex(tid,pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI get_triggers_avail(void *tid, char * trigger_names[], char * trigger_descs[]);

    /// Sets the deal to use custom collateral amortization and performs various initializations.
    /// This must be called before calling set_collateral_flow_ex(). 
    ///
    /// @since 1.2.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls. 
    /// @param[in] newVal The type of custom amortization. Must be one of the following: 
    ///         - CUSTOM_AMORT_NONE: WSA API amortizes the collateral.
    ///         - CUSTOM_AMORT_BY_DEAL: Cashflows are set at the deal level (collateral group 0),SFW engine not supported deal level currently.
    ///         - CUSTOM_AMORT_BY_GROUP: Cashflows are set by collateral group.
    /// 
    /// @retval   0 No error
    /// @retval  -1 Deal not open
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @note This must be called before set_collateral_flow_ex.
    ///       SFW engine not supported deal level(CUSTOM_AMORT_BY_DEAL)currently.
    /// 
	/// @sample 
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(CMO_STRUCT));
    ///     char deal_id[] = "AL2010-A";
    ///     int len = sizeof(deal_id) / sizeof(char);
    ///     memcpy(pCmo->dealid,deal_id,len);
    ///
    ///     open_deal_ex(tid,pCmo);
    ///     run_deal_ex(tid,pCmo);
    ///
    ///     set_custom_amortization_ex(tid, CUSTOM_AMORT_BY_GROUP);
    ///
    ///     set_collateral_flow_ex(tid, 0, 100, 0, NULL, pCmo);
    ///
    ///     close_deal_ex(tid,pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    short CHASAPI set_custom_amortization_ex(void *tid, short newVal);

    /// Sets the specified collateral cashflow to the values passed (in a vector) by the calling program for
    ///     the number of periods specified (all later periods will be set to zero (0).)
    /// 
    /// @since 1.2.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] group_number The collateral group for which cashflows are requested, 0 for total (deal level).
    /// @param[in] flow_identifier Identifies the requested cash flow.
    ///         Must be one of the collateral cashflow identifiers (see ::EXTENDED_FLOW_COLLATERAL_IDENTIFIER).
    /// @param[in] flow_length The number of values being passed in the array "flows".
    /// @param[in] flows A pointer to an array of cash flows.
    /// @param[in] cmo The pointer to the CMO_STRUCT used when opening the deal.
    /// 
    /// @retval   1 No error
    /// @retval  -1 Deal not open
    /// @retval  -2 Invalid cashflow type
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @note set_custom_amortization_ex() must be called before this function.
    /// 
	/// @sample
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(CMO_STRUCT));
    ///     char deal_id[] = "AB05HE3";
    ///     int len = sizeof(deal_id) / sizeof(char);
    ///     memcpy(pCmo->dealid, deal_id, len);
    ///
    ///     open_deal_ex(tid,pCmo);
    ///     run_deal_ex(tid,pCmo);
    ///
    ///     set_custom_amortization_ex(tid, CUSTOM_AMORT_BY_GROUP);
    ///
    ///     set_collateral_flow_ex(tid, 0, 100, 0, NULL, pCmo);
    ///
    ///     close_deal_ex(tid,pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    short CHASAPI set_collateral_flow_ex(void *tid, long group_number, int flow_identifier, 
        short flow_length, double * flows, CMO_STRUCT * cmo);

    /// Returns number of interest rate indices used by current deal 
    /// and populates ps_rates array to indicate the interest rate indices used.
    /// 
    /// ps_rates array might be following:
    /// 
    ///  Array Index | 0 | 1 | 2 | 3 | 4 | 5 | 6 | ... |
    /// --|--|--|--|--|--|--|--|--|
    ///  Element Value | 1 | 0 | 1 | 0 | 0 | 0 | 0 | ... |
    ///  Mapping to Index rates | LIBOR_1 | LIBOR_3 | LIBOR_6 | LIBOR_12 | LIBOR_24 | LIBOR_36 |LIBOR_48  | ... |
    ///
    /// The ps_rates above suggesting , the current deal is using Libor 1 month and Libor 6 month index rate.
    /// The mapping comes from ::INDEX_TYPE and ::INDEX_TYPE_EX
    ///
    ///
    /// @deprecated
    /// This method is deprecated. Use get_required_rate_codes().
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[out] ps_rates The user allocated array (size>=::MAX_INDEX_TYPES_EX)
    ///                      to indicate interest rate indices used by current deal.
    ///                      * 1 - The interest rate is used
    ///                      * 0 - The interest rate is not used
    ///
    /// @retval >=0 The number of rates used
    /// @retval -1  Deal not open
    /// @retval -2  Invalid rates pointer
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @sample
    ///     void* ptid=NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"BAFC08R2");
    ///
    ///     open_deal_ex(ptid,&deal);
    ///     // Deal is already open
    ///
    ///     // Determine which market rates are required and set them
    ///     short mktRates[MAX_INDEX_TYPES_EX] = {0};
    ///     get_rates_ex(ptid, mktRates);
    ///
    ///     // Set the market rate for LIBOR_1 as a constant if it is required
    ///     if(1 == mktRates[LIBOR_1])
    ///     {
    ///         type = LIBOR_1;
    ///         rate = .0525;                         //5.25%
    ///         set_rate_ex(ptid, &type, 0, &rate);
    ///     }
    ///
    ///     // Set the market rate for LIBOR_6 as a vector if it is required
    ///     // rateVec is a vector containing the market rates
    ///     // rateVecSize is the number of rates in the vector to use
    ///     if(1 == mktRates[LIBOR_6])
    ///     {
    ///        type = LIBOR_6;
    ///        set_rate_ex(ptid, &type, rateVecSize, rateVec);
    ///     }
    ///
    ///     close_deal_ex(ptid,&deal);
    /// @endsample
    ///
    /// @sa
    /// get_rate_ex() and get_required_rate_codes()
    ///
    long CHASAPI get_rates_ex(void *tid, short * ps_rates );    // obsolete use get_required_rate_codes

    /// Returns number of interest rate indices used by current deal 
    /// and populates rate_codes array with the list of index rate codes that are used.
    /// 
    ///  one example rate_codes array might be following:
    /// 
    ///  Array Index | 0 | 1 | 2 | 3 | 4 | 5 | 6 | ... |
    ///             --|--|--|--|--|--|--|--|--|
    ///  Element Value | 1 | 3 | 0 | 0 | 0 | 0 | 0 | ... |
    ///  
    /// The element value suggests the enum value from ::INDEX_TYPE and ::INDEX_TYPE_EX.
    ///
    /// In this case, 1 maps to LIBOR_1 and 3 maps to LIBOR_6, which means ,current deal use Libor 1 month and Libor 6 month rates.
    /// 
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[out] rate_codes The list of index rates codes used in the deal.
    /// @param[in] size_of_array_codes The size of the user allocated array rate_codes.
    /// 
    /// @retval >=0 The number of rates used
    /// @retval -1  Deal not open
    /// @retval -2  Invalid rates pointer
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @sample
    ///     void* ptid=NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid, "BAFC08R2");
    ///
    ///     open_deal_ex(ptid, &deal);
    ///     // Deal is already open.
    ///
    ///     int required_rates[MAX_INDEX_TYPES_EX]={};
    ///     // the codes of index rates will return in 'required_rates'.
    ///     int actual_count = get_required_rate_codes(ptid, required_rates, MAX_INDEX_TYPES_EX);
    ///     assert(actual_count > 0);
    ///     if (LIBOR_12 == required_rates[0])
    ///     {
    ///         // do something
    ///     }
    ///     else
    ///     {
    ///         // ...
    ///     }
    ///
    ///     close_deal_ex(ptid, &deal);
    /// @endsample
    /// 
    /// @note
    /// This method is the replacement for deprecated get_rates_ex().
    ///
    /// @sa 
    /// get_rate_ex()
    ///
    int CHASAPI get_required_rate_codes(void *tid, int* rate_codes, int size_of_array_codes);


    /// Gets the rate array for the given index.
    /// 
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    /// 
    /// @pre open_deal_ex() has been called.
    /// 
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] index The rate index (enum of INDEX_TYPE or INDEX_TYPE_EX).
    /// 
    /// @retval  NULL Error, please see details by calling get_deal_error_msg().
    /// @retval  Other The array pointer to the index rate, the array length is MAX_PERIODS(=612).
    /// 
    /// @sample
    ///     void* ptid=NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"BAFC08R2");
    ///
    ///     open_deal_ex(ptid,&deal);
    ///     // Deal is already opened.
    ///
    ///     double *pfRate=NULL;
    ///     pfRate=get_rate_ex(ptid, LIBOR_3);
    ///
    ///     close_deal_ex(ptid,&deal);
    /// @endsample
    ///
    /// @sa 
    /// get_required_rate_codes()
    ///
    double* CHASAPI get_rate_ex(void *tid, short index);

    /// Sets the constant or vector interest rate that will be used for the specified index.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] idx A pointer to the index to set. idx must be one of:
    ///         - enums of ::INDEX_TYPE (In indextypes.h).
    ///         - enums of ::INDEX_TYPE_EX (SFW and CDOnet deals).
    ///         - index codes returned by get_required_rate_codes().
    /// @param[in] vector The length of the vector pointed to by pval, or 0 if pval points to a constant.
    /// @param[in] pval A pointer to the new rate value or values. Value for current period (0-indexed element) will not be applied.
    ///
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not open
    /// @retval  -3 Error - Invalid market index
    /// @retval  -4 Error - No value passed or the value of vector is negative.
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @sample
    ///     void* tid=NULL;
    ///     CMO_STRUCT pCmo;
    ///     memset(&pCmo, 0, sizeof(CMO_STRUCT));
    ///     strcpy(pCmo.dealid,"BAFC08R2");
    ///
    ///     open_deal_ex(tid,&pCmo);
    ///     // Deal is already opened 
    ///
    ///     short type;
    ///     double rate;
    ///     short mktRates[MAX_INDEX_TYPES_EX] = {0};
    ///     // Determine which market rates are required and set them
    ///     int IR = get_required_rate_codes(tid, mktRates, MAX_INDEX_TYPES_EX);
    ///
    ///     // Set the market rate for LIBOR_1 as a constant if it is required
    ///     if(1 == mktRates[LIBOR_1])
    ///     {
    ///         type = LIBOR_1;
    ///         rate = .0525;                       //5.25%
    ///         set_rate_ex(tid, &type, 0, &rate);
    ///     }
    ///
    ///     // Set the market rate for LIBOR_6 as a vector if it is required
    ///     // rateVec is a vector containing the market rates
    ///     // rateVecSize is the number of rates in the vector to use
    ///     if(1 == mktRates[LIBOR_6])
    ///     {
    ///         type = LIBOR_6;
    ///         double rateVec[] ={0,0.0436,0.0426,0.0425,0.0211,0.024,0.033,0.0124,0.0116};
    ///         short rateVecSize = sizeof(rateVec)/sizeof(rateVec[0]);
    ///         set_rate_ex(tid, &type, rateVecSize, rateVec);
    ///     }
    ///
    ///     close_deal_ex_(tid,&pCmo);
    /// @endsample
    ///
    /// @note
    ///     - The rates are expressed as a decimal: 5.25% would be .0525.
    ///     - The required rates for a deal can be determined by calling get_required_rate_codes().
    ///     - value for current period (0-indexed element) will not be applied.
    ///     - Index rates vector apply from the latest update date closest/relative to the settlement date. A floater bond will use period 1 rate assumption at first reset date since deal update date.
    ///     - Index rates vector is a monthly rate vector, regardless payment frequency of deal.
    ///     - When API run with predefined scenario in PA/MPA, user can set spot rate by set_rate_ex(), API will project rate vector by adding MPA/PA's future rate shifts to the spot rate.
    ///
    long CHASAPI set_rate_ex(void *tid, short *idx, short vector, double * pval );

    /// Sets the constant or vectored prepayment speed.
    /// It will be used for the pool specified by loan_num with the ability to apply to underlying deals if it is a reremic.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] type Type of prepayment curve. Must be one of:
    ///                          - PREPAY_CURVE_PSA - The PSA Standard Prepayment Assumptions rate specifies an annual prepayment percentage as a function of the seasoning of the mortgages/loans.
    ///                            100% PSA assumes prepayment rates of 0.2% CPR in the first month following origination of the mortgage loans and an additional 0.2% CPR in each succeeding month until the 30th month.  
    ///                            In the 30th month and beyond, 100% PSA assumes a fixed annual prepayment rate of 6% CPR. Any PSA other than 100% is calculated by multiplying the annual prepayment rate (the CPR) by that multiple of 100%.
    ///                          - PREPAY_CURVE_SMM - The Single Monthly Mortality rate is the percentage of the mortgages/loans outstanding at the beginning of the month assumed to terminate during the month.  
    ///                            It is mathematically related to CPR: ( 1 – SMM )^12 = 1 – CPR.  
    ///                            Seasoning of mortgages/loans will not be accounted for if a vectored prepayment rate is set. 
    ///                          - PREPAY_CURVE_CPR - The Constant Prepayment Rate is the percentage of the mortgages/loans outstanding at the beginning of a year assumed to terminate during the year.  
    ///                            It is mathematically related to SMM: ( 1 – SMM )^12 = 1 – CPR.  
    ///                            Seasoning of mortgages/loans will not be accounted for if a vectored prepayment rate is set. 
    ///                          - PREPAY_CURVE_CPY - The Constant Prepayment Yield rate is equivalent to the Constant Prepayment Rate(CPR) except that it assumes prepayment only happens after contractual lockout and yield maintenance period.
    ///                          - PREPAY_CURVE_HEP - The Home Equity Prepayment rate is a measure of prepayments for closed-end, fixed rate HEL loans.  
    ///                            The HEP curve is steeply ramped for the first 10 months, and then plateaus a fixed rate for the life of the loan.  
    ///                            For example, a 24% HEP means the loan will start at a prepayment speed of 2.4% CPR and grow to 24% CPR over 10 months, staying fixed at 24% CPR for the remainder of the loan. 
    ///                          - PREPAY_CURVE_ABS - Asset-Backed Securities(ABS): 
    ///                            The Asset-Backed Securities rate defines an increasing sequence of monthly prepayment rates which correspond to a constant absolute level of loan prepayments in all future periods.  
    ///                            It is mathematically related to SMM: SMM = ABS / (1 – ABS * (Seasoning – 1)).
    ///                          - PREPAY_CURVE_CUS
	///                          - PREPAY_CURVE_CPB
    /// @param[in] is_vector The length of the vector pointed to by pval or 0 if pval is a constant.
    /// @param[in] pval A pointer to the prepayment speeds (or speed). Value for current period (0-indexed element) will not be applied.
    /// @param[in] loan_num The 1-based index of the loan or -1 to apply to all collateral in the deal.
    /// @param[in] set_sup_remic If TRUE this will replace any specified underlying deal settings. If FALSE, this will NOT replace any underlying deal settings.
    ///
    /// @retval  0 No error
    /// @retval -1 Error - Deal not opened
    /// @retval -2 Error - Other error
    /// @retval -3 Error - Invalid loan number
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @sample
    ///     void* ptid=NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"BAFC08R2");
    ///
    ///     open_deal_ex(ptid,&deal);
    ///     // Deal is already opened
    ///
    ///     double rate;    // prepayment speed
    ///     short type;     // prepayment type
    ///
    ///     // Set 8% CPR for whole deal, 
    ///     // And apply to underlying
    ///     type = PREPAY_CURVE_CPR;
    ///     rate = .08;
    ///     set_prepayments_ex(ptid, type, 0, &rate, -1, true);
    /// 
    ///     // Set vector of SMM for first piece of collateral
    ///     // Do not apply to underlying deals. 
    ///     // rates is a previously set vector with 300 entries.
    ///     set_prepayments_ex(ptid, PREPAY_CURVE_SMM, 300, rates, 1, false);
    ///
    ///     run_deal_ex(ptid,&deal);
    ///     close_deal_ex(ptid,&deal);
    /// @endsample
    ///
    /// @note 
    ///     * Prepayment speeds are expressed as decimals. 5.25% would be .0525.
    ///     * If loan_num = -1, the setting will apply to all collateral loans.
    ///         If loan_num is 1-based index, the setting will only apply to the loan specified by loan_num.
    ///     * value for current period (0-indexed element) will not be applied.
    ///
    /// @sa 
    /// - set_prepayments_ex can be used to set ppy rates for both top level deals and underlying deals.
    /// - set_reremic_prepayments is used to set ppy rates for underlying deals. If the deal specified by para dealid is not an underlying deal, it will return an error.
    ///
    long CHASAPI set_prepayments_ex(void *tid, short type, short is_vector, double * pval, long loan_num,
        BOOLYAN set_sup_remic);

    /// Sets the group level delinquency for a specific type.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null The reremic deal ID if child deal. Otherwise pass null.
    /// @param[in] group_number The group ID to apply defaults to, -1 means apply delinquency assumption on deal level.
    /// @param[in] is_vector The length of the vector pointed to by pval or 0 if pval is a constant.
    /// @param[in] delinq_type Delinquent type, must be one of ::GROUP_DELINQ_STATES. SFW and CDOnet can only support GROUP_DELINQ_90 now.
    /// @param[in] dqVal Delinquent data.
    ///
    /// @retval  1 SFW or CDOnet Engine Warning: Only GROUP_DELINQ_90 rates are used by the engine currently.
    /// @retval  0 Assumption set successfully.
    /// @retval -1 Error - Deal not opened
    /// @retval -2 Invalid parameter.
    /// @retval -99 Fail - check details by calling get_deal_error_msg()
    ///
    /// @sample
    /// 	void* tid = NULL;
    /// 	CMO_STRUCT *pCmo = new CMO_STRUCT();
    /// 	memset(pCmo, 0, sizeof(CMO_STRUCT));
    /// 	char deal_id[] = "WFNMT";
    /// 	int len = sizeof(deal_id) / sizeof(char);
    /// 	memcpy(pCmo->dealid, deal_id, len);
    ///
    /// 	set_input_path("the_input_path");
    /// 	assert(0 == open_deal_ex(tid, pCmo));
    ///
    /// 	double delinq[3] = {0.5, 0.3, 0.2};
    /// 	assert(0 == set_addit_group_delinquencies(tid, NULL, -1, sizeof(delinq)/sizeof(delinq[0]), GROUP_DELINQ_90, delinq));
    /// 	
    /// 	assert(0 == run_deal_ex(tid, pCmo));
    /// 	
    /// 	close_deal_ex(tid, pCmo);
    /// 	delete pCmo;
    /// 	pCmo = NULL;
    /// @endsample
    long CHASAPI set_addit_group_delinquencies(void* tid, const char* reremic_deal_id_or_null,
        int group_number, short is_vector, int delinq_type, double* dqVal);


    /// Installs per period assumption call back function. User provided call back function will be invoked when running each period.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// 
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] per_period_assump_cb The user provided call back function.  The call back function type should be be PER_PERIOD_ASSUMP_CB.
    ///
    /// @retval  0 Success
    /// @retval <0 Error - use get_deal_error_msg() function to obtain text of error
    ///
    /// @sample
    /// 	int fPER_PERIOD_ASSUMP_CB(  void* tid,
    /// 	                            int period,
    /// 	                            int max_periods,
    /// 	                            PAY_POOL_INFO* pool_info,
    /// 	                            PAY_POOL_STATE* pool_state,
    /// 	                            CCMO_PERIOD_ASSUMPTIONS* assumptions,
    /// 	                            void* user_data,
    /// 	                            char* error_message,
    /// 	                            int max_size_of_error_message
    /// 	                        );
    ///	
    /// 	void *tid = NULL;
    /// 	CMO_STRUCT *pCmo = new CMO_STRUCT();
    /// 	strcpy(pCmo->dealid,"AL2010-A");
    ///
    /// 	int iret = open_deal_ex(tid, pCmo);
    ///
    /// 	iret = install_per_period_assump_cb(tid, fCOLLAT_ASSUMP_CB);
    ///
    /// 	iret = run_deal_ex(tid, pCmo);
    ///
    /// 	iret = close_deal_ex(tid, pCmo);
    /// 	delete pCmo;
    /// 	pCmo = NULL;
    /// @endsample
    ///
    /// @sa
    /// install_collat_assump_cb()
    ///
    int CHASAPI install_per_period_assump_cb(void *tid,PER_PERIOD_ASSUMP_CB per_period_assump_cb);


    /// Installs collateral assumption call back function. User provided call back function will be invoked when running each collateral pool.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] collat_assump_cb The user provided call back function.
    ///
    /// @retval  0 Success
    /// @retval <0 Error - use  get_deal_error_msg() function to obtain text of error
    ///
    /// @sample
    ///     int collat_assump_cb_func(void* tid,
    ///                          char* first_period_date,
    ///                          int max_periods,
    ///                          PAY_POOL_INFO* pool_info,
    ///                          CCMO_COLLAT_ASSUMPTIONS* assumptions,
    ///                          void* user_data,
    ///                          char* error_message,
    ///                          int max_size_of_error_message)
    ///     {
    ///         if (pool_info->pool_info.loan_number == 1)
    ///         {
    ///             // check user input data
    ///             assert(0 == strcmp("user data what passed in", (const char *)pool_info->pool_info.usr_data));
    ///             std::cout << "User input data for loan 1 is:" << (const char *)pool_info->pool_info.usr_data << std::endl;
    ///     
    ///             // do any settings you want
    ///             assumptions->default_type = DEFAULT_CURVE_CDR;
    ///         }
    ///         else if (pool_info->pool_info.loan_number == 2)
    ///         {
    ///             // check user input data
    ///             assert(123 == (int)pool_info->pool_info.usr_data);
    ///             std::cout << "User input data for loan 2 is:" << (int)pool_info->pool_info.usr_data << std::endl;
    ///     
    ///             // do any settings you want
    ///             assumptions->default_type = DEFAULT_CURVE_CDR;
    ///         }
    ///         else
    ///         {
    ///             // do any settings you want
    ///             assumptions->default_type = DEFAULT_CURVE_CDR;
    ///         }
    ///     
    ///         return 0;
    ///     }
    ///     
    ///     void collat_assump_cb_example()
    ///     {
    ///         void* pDeal = NULL;
    ///         CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///         memset(pCmo, 0, sizeof(*pCmo));
    ///         strcpy(pCmo->dealid, "ACE06NC1");
    ///     
    ///         set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///         // open deal
    ///         assert(0 == open_deal_ex(pDeal, pCmo));
    ///     
    ///         MARKIT_POOL_INFO* coll_info = NULL;
    ///         void* coll_it = obtain_collat_iterator(pDeal, 0);
    ///         assert(NULL != coll_it);
    ///     
    ///         // set user input data for loan 1 and loan 2
    ///         assert(0 == set_pool_level_user_data_for_cb(pDeal, NULL, 1, (void*)"user data what passed in"));   // loan 1, passed a pointer of a string
    ///         assert(0 == set_pool_level_user_data_for_cb(pDeal, NULL, 2, (void*)123));                          // loan 2, passed a int number
    ///     
    ///         // get all loans
    ///         std::vector<MARKIT_POOL_INFO*> all_collat;
    ///         while((coll_info = get_next_collat(pDeal, coll_it)))
    ///         {
    ///             all_collat.push_back(coll_info);
    ///         }
    ///         assert(all_collat.size() >= 2);
    ///     
    ///         // check user input data for loan 1
    ///         assert(1 == all_collat[0]->loan_number);
    ///         assert(0 == strcmp("user data what passed in", (const char *)all_collat[0]->usr_data));
    ///     
    ///         // check user input data for loan 2
    ///         assert(2 == all_collat[1]->loan_number);
    ///         assert(123 == (int)all_collat[1]->usr_data);
    ///     
    ///         // install call back function
    ///         assert(0 == install_collat_assump_cb(pDeal, collat_assump_cb_func));
    ///     
    ///         // run deal, call back func will be triggered many times for each loan
    ///         assert(0 == run_deal_ex(pDeal, pCmo));
    ///     
    ///         assert(0 == close_deal_ex(pDeal, pCmo));
    ///         delete pCmo;
    ///         pCmo = NULL;
    ///     }
    /// @endsample
    ///
    /// @sa
    /// install_per_period_assump_cb()
    ///
    int CHASAPI install_collat_assump_cb(void *tid,COLLAT_ASSUMP_CB collat_assump_cb);

    /// This function installs the user defined pool cashflow callback function.
    /// 
    /// @since 1.2.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] pool_cashflow_cb The user callback function for pool cash flow
    /// 
    /// @retval  0 Success
    /// @retval <0 Error - for details call get_deal_error_msg()
    /// 
	/// @sample
    ///
    ///    void fPOOL_CASHFLOW_CB(void* tid, 
    ///                       PAY_POOL_INFO* pool_info,
    ///                       int last_populated_period,
    ///                       PAY_POOL_STATE* pool_cashflow,
    ///                       CCMO_PERIOD_ASSUMPTIONS* assumptions,
    ///                       void* user_data
    ///                       );
    ///
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(CMO_STRUCT));
    ///     char deal_id[] = "AB05HE3";
    ///     int len = sizeof(deal_id) / sizeof(char);
    ///     memcpy(pCmo->dealid,deal_id,len);
    ///
    ///     open_deal_ex(tid, pCmo));
    ///
    ///     set_user_data_for_cb(tid,(void*)"user data for pool cashflow cb");
    ///
    ///     install_pool_cashflow_cb(tid, fPOOL_CASHFLOW_CB);
    ///
    ///     run_deal_ex(tid,pCmo);
    ///     close_deal_ex(tid,pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    /// 
    int CHASAPI install_pool_cashflow_cb(void *tid,POOL_CASHFLOW_CB pool_cashflow_cb);

    /// Registers user data with the WSA API which will then be passed to call back functions.
    /// This allows user to maintain the state between calls to call back functions.
    /// 
    /// @since 1.2.0
    /// @avail CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] user_data Pointer to user data
    /// 
    /// @retval   0 No error
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @note 
    ///       -# User data is stored for a given tid and is passed back to all call back function.
    ///       -# User is responsible for maintaining the data, the WSA API just passes the pointer along.
    ///       -# User might request an automatic clean up function to be called by the WSA API ( see reference to install_user_cleanup_cb() )
    /// 
	/// @sample
    ///
    ///    int fCOLLAT_ASSUMP_CB1(void* tid, 
    ///                       char* first_period_date,
    ///                       int max_periods, 
    ///                       PAY_POOL_INFO* pool_info,
    ///                       CCMO_COLLAT_ASSUMPTIONS* assumptions,
    ///                       void* user_data,
    ///                       char* error_message,
    ///                       int max_size_of_error_message
    ///                       );
    ///
    ///     void *tid = NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"AL2010-A");
    ///
    ///     create_deal_scenario_object(&tid,NULL,NULL,NULL);
    ///     open_deal_ex(tid,&deal);
    ///
    ///     set_user_data_for_cb(tid,(void*)"set_user_data_for_cb");
    ///
    ///     install_collat_assump_cb(tid,fCOLLAT_ASSUMP_CB1);
    ///
    ///     run_deal_ex(tid,&deal);
    ///     close_deal_ex(tid,&deal);
    ///     release_deal_scenario_object(&tid);
    /// @endsample
    ///
    int CHASAPI set_user_data_for_cb(void *tid,void* user_data);

    /// Registers pool level user data with the WSA API which will can be got from MARKIT_POOL_INFO.usr_data.
    /// 
    /// @since 1.2.0
    /// @avail CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null 0 for parent deal or name of the child deal.
    /// @param[in] loan_num The 1-based index of the loan or -1 to apply to all collateral in the deal.
    /// @param[in] user_data Pointer to user data.
    /// 
    /// @retval   0 No error
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @sample
    ///     void* pDeal=NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(pCmo.dealid, "BAFC08R2");
    ///
    ///     open_deal_ex(pDeal, &deal);
    ///     // Deal is already opened
    ///
    ///     // set user data for collateral 1
    ///     short loan_num = 1;
    ///     int ret = set_pool_level_user_data_for_cb(pDeal, NULL, loan_num, (void*)"user data what passed in");
    ///
    ///     void* coll_it = obtain_collat_iterator(pDeal, 0);
    ///     assert(NULL != coll_it);
    ///     MARKIT_POOL_INFO* coll_info = NULL;
    ///     // loop all collaterals and get usr_data of collateral 1
    ///     while((coll_info = get_next_collat(pDeal, coll_it)))
    ///     {	
    ///         if(loan_num == coll_info->loan_number)
    ///         {
    ///             char *szUsrData = (char *)coll_info->usr_data;
    ///             // the content of szUsrData should be "user data what passed in"
    ///             assert(0 == strcmp("user data what passed in", szUsrData);
    ///             break;
    ///         }
    ///     }
    ///
    ///     close_deal_ex(pDeal, &deal);
    /// @endsample
    /// 
    /// @note 
    ///       - User is responsible for maintaining the data, the WSA API just passes the pointer along.
    /// 
    int CHASAPI set_pool_level_user_data_for_cb(void *tid,const char* reremic_deal_id_or_null,short loan_num,void* user_data);

    /// Retrieves the registed user data with the WSA API set_user_data_for_cb().
    /// 
    /// @since 1.1.0
    /// @avail CDOnet, CHS, SFW
    ///	
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// 
    /// @return A pointer to user stored data area
    /// 
	/// @sample
    ///    int fCOLLAT_ASSUMP_CB1(void* tid, 
    ///                       char* first_period_date,
    ///                       int max_periods, 
    ///                       PAY_POOL_INFO* pool_info,
    ///                       CCMO_COLLAT_ASSUMPTIONS* assumptions,
    ///                       void* user_data,
    ///                       char* error_message,
    ///                       int max_size_of_error_message
    ///                       );
    ///
    ///     void* tid = NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"AL2010-A");
    ///
    ///     create_deal_scenario_object(&tid,NULL,NULL,NULL);
    ///     open_deal_ex(tid,&deal);
    ///
    ///     set_user_data_for_cb(tid,(void*)"set_user_data_for_cb");
    ///     install_collat_assump_cb(tid,fCOLLAT_ASSUMP_CB1);
    ///     run_deal_ex(tid,&deal);
    ///
    ///     get_user_data_for_cb(tid);
    ///
    ///     close_deal_ex(tid,&deal);
    ///     release_deal_scenario_object(&tid);
    /// @endsample
    ///
    void* CHASAPI get_user_data_for_cb(void *tid);

    /// Installs user clean up function
    /// 
    /// @since 1.1.0
    /// @avail CDOnet, CHS, SFW
    ///	
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] user_cleanup_cb User provided call back function
    /// @param[in] invoke_on_deal_close Indicate if call back function will be invoked.
    ///          - 1: The call back function will be invoked.
    ///          - 0: The call back function won't be invoked.
    /// 
    /// @retval   0 Success
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @note If user registers some user data with the WSA API, she might want the WSA API to provide a way to release the data at appropriate time.  The WSA API allows a user to register clean up call back function which will be called when deal is closed or when new user data is provided. The pointer to the previously registered user data will be passed to this function allowing the user to perform a proper release of resources associated with user data.
    ///       Calling install_user_cleanup_cb() is a way to register this call back
    ///       The signature of the call back function see USER_CLEANUP_CB.
    /// 
	/// @sample
    ///    int fCOLLAT_ASSUMP_CB1(void* tid, 
    ///                       char* first_period_date,
    ///                       int max_periods, 
    ///                       PAY_POOL_INFO* pool_info,
    ///                       CCMO_COLLAT_ASSUMPTIONS* assumptions,
    ///                       void* user_data,
    ///                       char* error_message,
    ///                       int max_size_of_error_message
    ///                       );
    ///
    ///     void fUSER_CLEANUP_CB(void* user_data);
    ///
    ///     void* tid=NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"AL2010-A");
    ///
    ///     create_deal_scenario_object(&tid,NULL,NULL,NULL);
    ///     open_deal_ex(tid,&deal);
    ///
    ///     set_user_data_for_cb(tid,(void*)"set_user_data_for_cb");
    ///     install_collat_assump_cb(tid,fCOLLAT_ASSUMP_CB1);
    ///
    ///     install_user_cleanup_cb(tid,fUSER_CLEANUP_CB,0);
    ///
    ///     run_deal_ex(tid,&deal);
    ///     close_deal_ex(tid,&deal);
    ///     release_deal_scenario_object(&tid);
    /// @endsample
    ///   
    int CHASAPI install_user_cleanup_cb(void *tid, USER_CLEANUP_CB user_cleanup_cb, int invoke_on_deal_close);

    /// Installs input_path call back function.
    /// User provided call back function will be invoked to set extra input directory
    /// before SFW or CHS deal archive files (.SFW, .CHS, .LLD, etc) are accessed
    ///
    /// @since 1.6.0
    /// @avail CHS, SFW
    ///
    /// @param[in] callback The user provided call back function.
    ///            The call back function type should be be INPUT_DIR_CB.
    ///            To uninstall the call back function, call with callback set to NULL.
    ///
    /// @return None
    ///
    /// @note Calling to this api is not thread-safe.
    ///
    /// @sample
    ///     // The CB function
    ///     USER_CB_RETURN_CODE DealDirCB(
    ///                          const char* deal_name,
    ///                          const char* file_name,
    ///                          const char* archive_name,
    ///                          const char* default_dir_if_noop,
    ///                          char* file_dir_to_provide,
    ///                          int size_of_file_dir_buffer,
    ///                          char* error_msg,
    ///                          int size_of_error_msg_buffer
    ///                          );
    ///     // should be called in main thread before other threads start
    ///     install_input_dir_callback(DealDirCB);
    /// @endsample
    ///
    void CHASAPI install_input_dir_callback(INPUT_DIR_CB callback);

	/// Installs input_path call back function Ex.
    /// User provided call back function will be invoked to set extra input directory
    /// before SFW or CHS deal archive files (.SFW, .CHS, .LLD, etc) are accessed
    ///
    /// @since 1.6.0
    /// @avail CHS, SFW
    ///
    /// @param[in] callback The user provided call back function.
    ///            The call back function type should be be INPUT_DIR_CB.
    ///            To uninstall the call back function, call with callback set to NULL.
    ///
    /// @return None
    ///
    /// @note Calling to this api is not thread-safe.
    ///
    /// @sample
    ///     // The CB function
    ///     USER_CB_RETURN_CODE DealDirCBEx(
    ///                          const char* deal_name,
    ///                          const char* file_name,
    ///                          const char* archive_name,
    ///                          const char* default_dir_if_noop,
    ///                          char* file_dir_to_provide,
	///							 const char* update_date,
    ///                          int size_of_file_dir_buffer,
    ///                          char* error_msg,
    ///                          int size_of_error_msg_buffer
    ///                          );
    ///     // should be called in main thread before other threads start
    ///     install_input_dir_callback_ex(DealDirCBEx);
    /// @endsample
    ///
    void CHASAPI install_input_dir_callback_ex(INPUT_DIR_CB_EX callback_ex);

    /// This function writes the given message to the log file.
    /// 
    /// @since 1.1.0
    /// @avail ALL
    ///	
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] message Message to be written
    /// @param[in] log_level
    /// 
	/// @sample
    ///     void *tid = NULL;
    ///     char teststring[20] = "abcdefgh";
    ///     write_log(tid, teststring, 1);
    /// @endsample
    ///
    void CHASAPI write_log(void* tid, char* message,int log_level);

    // Credit Sensitivity

    /// Indicates whether defaults and delinquencies can affect the currently open deal.
    /// 
    /// @since 1.2.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] loan_num Reserved for future use, must be -1
    /// 
    /// @retval   1 Is credit sensitive (affected be defaults and/or delinquencies)
    /// @retval   0 Is not credit sensitive
    /// @retval  -1 Deal is not open
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @sample
    ///     // Deal is already opened
    ///
    ///     // If iR is 1, the deal is credit sensitive.
    ///     int iR = is_credit_sensitive_ex(tid, -1);
    /// @endsample
    /// 
    long CHASAPI is_credit_sensitive_ex(void *tid, long loan_num );

    /// Sets which balance defaults are calculated from. The setting will also apply to underlying deals if set_sup_remic is TRUE. The default value is DEFLT_FROM_CURBAL.
    /// 
    /// @since 1.1.0
    /// @avail SFW, CDOnet, CHS
    ///	
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] type The type of balance the defaults are calculated from.
    ///         - ::DEFLT_FROM_CURBAL: The current balance for the period. SFW and CDOnet engine can only support this type.
    ///         - ::DEFLT_FROM_ORIGBAL: The balance in period 0 (deal "as of" date).
    ///         - ::DEFLT_FROM_ZERO_CPR: The scheduled balance if no prepayments and no defaults.
    /// @param[in] set_sup_remic Settings are applied to underlying deals if TRUE.
    /// 
    /// @return None
    /// 
    /// @sample
    ///     // Deal is already opened
    ///
    ///     // Set the defaults from the original balance. If the defaults
    ///     // are a constant, the dollar amount will default each period. 
    ///     // Apply this to all underlying deals
    ///     set_default_from_ex(tid, DEFLT_FROM_ORIGBAL, TRUE);
    /// @endsample
    /// 
    /// @warning when it is SFW or CDOnet deal, type could only support DEFLT_FROM_CURBAL.
    void CHASAPI set_default_from_ex(void *tid, const short type, BOOLYAN set_sup_remic);

    /// Sets the constant or vectored default rate and type that will be used for the pool specified by loan_num, 
    /// with the ability to apply to underlying deals if it is a reremic.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] type Type of default curve. Must be one of: 
    ///           - DEFAULT_CURVE_CDR - Constant Default Rate(CDR): The Constant Default Rate is the percentage of the mortgages/loans 
    ///             outstanding at the beginning of the year assumed to terminate during the year through events of default.    
    ///             It is mathematically related to MDR: ( 1 – MDR )^12 = 1 – CDR.  
    ///             Seasoning of mortgages/loans will not be accounted for if a vectored default rate is set.   
    ///           - DEFAULT_CURVE_SDA - Standard default curve: Measuring for defaults in the residential mortgage market
    ///             The SDA Standard Default Assumptions rate specifies an annual default percentage as a function of the seasoning of the mortgages/loans.  
    ///             100% SDA assumes default rates of 0.02% CDR in the first month following origination of the mortgage loans and an additional 0.02% CDR in each succeeding month until the 30th month. 
    ///             In the 30th month and beyond, 100% SDA assumes a fixed annual default rate of 0.6% CDR. 
    ///             Any SDA other than 100% is calculated by multiplying the annual default rate (the CDR) by that multiple of 100%.
    ///           - DEFAULT_CURVE_MDR - Monthly Default Rate
    ///             The Monthly Default Rate is the percentage of the mortgages/loans outstanding at the beginning of the month assumed to terminate during the month through events of default.  
    ///             It is mathematically related to CDR: ( 1 – MDR )^12 = 1 – CDR.  Seasoning of mortgages/loans will not be accounted for if a vectored default rate is set.
    ///             The engine will start with the first entry of the default vector in forecasting default.
    ///           - DEFAULT_CURVE_SEASONED_CDR - The Constant Default Rate is the percentage of the mortgages/loans outstanding at the beginning of the year assumed to terminate during the year through events of default.  
    ///             It is mathematically related to MDR: ( 1 – MDR )^12 = 1 – CDR.  Seasoning of mortgages/loans will be accounted for if a vectored default rate is set.    
    ///           - DEFAULT_CURVE_SEASONED_MDR - The Monthly Default Rate is the percentage of the mortgages/loans outstanding at the beginning of the month assumed to terminate during the month through events of default.  
    ///             It is mathematically related to CDR: ( 1 – MDR )^12 = 1 – CDR.  Seasoning of mortgages/loans will be accounted for if a vectored default rate is set.
    ///             The engine will set the projection starting point in the default vector by the age of the asset.
    ///           - DEFAULT_CURVE_PCT - The PCT is similar to the MDR curve except that defaults are applied each month to the period 0 balance of the loan, rather than the current balance of the loan. 
	///             This option is only available in SFW and CDOnet engine. In CHS engine, this option is equivalent to use option DEFAULT_CURVE_MDR with setting DEFLT_FROM_ORIGBAL in function set_default_from_ex().
	///           - DEFAULT_CURVE_PLD - Prodject Loan Default Rate: The PLD rates specifies the annum default percentage of the then-outstanding principal balance of each of the Mortgage Loans in relation to its loan age. 
	///             100% PLD represents 100% of such assumed rate of involuntary prepayments. This option is only available in SFW and CHS engine.
    /// @param[in] is_vector The length of the vector pointed to by pval or 0 if pval is a constant.
    /// @param[in] pval A pointer to the default rates (or rate). Value for current period (0-indexed element) will not be applied.
    /// @param[in] loan_num The 1-based index of the loan or -1 to apply to all collateral in the deal.
    /// @param[in] set_sup_remic If TRUE this will be applied to underlying deals. Otherwise it will not.
    ///
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Other error
    /// @retval  -3 Error - Invalid loan number
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @sample
    ///     void* ptid=NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"BAFC08R2");
    ///
    ///     open_deal_ex(ptid,&deal);
    ///     // Deal is already opened
    ///
    ///     double rate;   // default speed
    ///     short type;    // default type
    ///
    ///     // Set 8% CDR for whole deal 
    ///     // And apply to underlying
    ///     type = DEFAULT_CURVE_CDR;
    ///     rate = .08;
    ///     set_defaults_ex(ptid, type, 0, &rate, -1, true);
    /// 
    ///     // Set vector of MDR for second piece of collateral. 
    ///     // Do NOT apply to underlying. rates is a previously. 
    ///     double rate_array[]={.01,.02,.03,.04,.05};
    ///     set_defaults_ex(ptid, DEFAULT_CURVE_MDR, 5, rate_array, 2, false);
    ///
    ///     run_deal_ex(ptid,&deal);
    ///     close_deal_ex(ptid,&deal);
    /// @endsample
    ///
    /// @note 
    ///     * Default rates are expressed as decimals. 5.25% would be .0525.
    ///     * If loan_num = -1, the setting will apply to all collateral loans.
    ///         If loan_num is 1-based index, the setting will only apply to the loan specified by loan_num.
    ///     * value for current period (0-indexed element) will not be applied.
    ///
    /// @sa 
    /// - set_defaults_ex can be used to set default rates for both top level deals and underlying deals
    /// - set_reremic_defaults is used to set default rates for underling deal. If the deal specified by para dealid is not an underlying deal, it will return an error.
    ///
    long CHASAPI set_defaults_ex(void *tid, short type, short is_vector, double * pval, long loan_num,
        BOOLYAN set_sup_remic);


    /// Sets the constant or vectored recovery rate that will be used for the pool specified by loan_num , with the option to apply to underlying deals if the deal is a reremic.
    /// The recovery rate is the percentage of total liquidations that will be returned to the CMO, less any amount that is used to reimburse the servicer for principal advances. 
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// 
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] is_vector The length of the vector pointed to by pval or 0 if pval is a constant.
    /// @param[in] pval A pointer to the recovery rates (or rate). Value for current period (0-indexed element) will not be applied.
    /// @param[in] loan_num The 1-based index of the loan or -1 to apply to all collateral in the deal.
    /// @param[in] set_sup_remic If TRUE, this will be applied to underlying deals. If FALSE, this will be NOT applied to underlying deals.
    ///
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Other error
    /// @retval  -3 Error - Invalid loan number
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @sample
    ///     void* tid=NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"BAFC08R2");
    ///
    ///     open_deal_ex(tid,&deal);
    ///     // Deal is already opened
    ///
    ///     // Set the recovery rate for all collateral in the deal to 25%.
    ///     // Replace any underlying deal recoveries.
    ///     double rate = .25;
    ///     int IR = set_recoveries_ex(tid, 0, &rate, -1, true);
    ///
    ///     // Set the recovery rate for the first piece of collateral to a vector. 
    ///     // Use 100 values (rateVec1 is an existing vector). 
    ///     // Do NOT overwrite values for underyling deals
    ///     double rateVec1[100] = {0.01};
    ///     IR = set_recoveries_ex(tid, 100, rateVec1, 1, false);
    ///
    ///     run_deal_ex(tid,&deal);
    ///     close_deal_ex(tid,&deal);
    /// @endsample
    ///
    /// @note
    ///     * If recovery lag is not 0, in case of default, the loss will be realized at the time of recovery, not the time of default. 
    ///         By default, the recovery and loss will be calculated using the rate at the time of recovery (see set_recovery_from).  
    ///         If recovery is 0, the recovery and loss will happen immediately when the default happens.
    ///     * If loan_num = -1, the setting will apply to all collateral loans. 
    ///         If loan_num is 1-based index, the setting will only apply to the loan specified by loan_num.
    ///     * Recovery rates are expressed as decimals. 5.25% would be .0525.
    ///     * value for current period (0-indexed element) will not be applied.
    ///
    /// @sa
    /// - set_recoveries_ex() can be used for both top level deals and underlying deals.
    /// - set_reremic_recoveries() is used for underlying deal. If the deal specified by parameter dealid is not an underlying deal, it will return an error.
    /// - set_recovery_from() can be used to specify whether to use the recovery rate at the time of default or at the time of recovery (in the case of non-zero lag) for SFW deals.
    ///
    long CHASAPI set_recoveries_ex(void *tid, short is_vector, double * pval, long loan_num,
        BOOLYAN set_sup_remic);

    /// Sets the recovery lag (months from default to liquidation).
    /// The settings can  be applied to underlying deals optionally if they are reremic.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] val The lag in months.
    /// @param[in] loan_num The 1-based index of the loan or -1 to apply to all collateral in the deal.
    /// @param[in] set_sup_remic If TRUE, this will be applied to underlying deals; otherwise it will not.
    ///
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Other error
    /// @retval  -3 Error - Invalid loan number
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @sample
    ///     void* ptid=NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"BAFC08R2");
    ///
    ///     open_deal_ex(ptid,&deal);
    ///     // Deal is already opened
    ///
    ///     // Set the recovery lag (months between default and recovery) to
    ///     // 2 months for all collateral in the deal and the underlying
    ///     // deals
    ///     set_recovery_lag_ex(ptid, 2, -1, true);
    ///
    ///     // Set the recovery lag to 3 months for the first piece of
    ///     // collateral.
    ///     set_recovery_lag_ex(ptid, 3, 1, true);
    ///
    ///     close_deal_ex(ptid,&deal);
    /// @endsample
    ///
    /// @note If loan_num = -1, the setting will apply to all collateral loans. If loan_num is 1-based index, the setting will only apply to the loan specified by loan_num.
    /// 
    /// @sa
    /// - set_recovery_lag_ex can be used for both top level deals and underlying deals.
    /// - set_reremic_recovery_lag is used for underlying deal. If the deal specified by parameter dealid is not an underlying deal, it will return an error. 
    /// 
    long CHASAPI set_recovery_lag_ex(void *tid, short val, long loan_num, BOOLYAN set_sup_remic);

    /// Sets the type of cashflows the servicer advances when the collateral is in default,
    /// with the option to apply the setting to underlying deals if they are a reremic.
    /// 
    /// @since 1.1.0
    /// @avail CDOnet, CHS, SFW
    ///	
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] type The types of cashflow advanced. Must be one of the following:
    ///         - SERVICER_ADVANCES_NOTHING - The servicer advances neither interest nor principal.
    ///         - SERVICER_ADVANCES_INTEREST - The servicer advances interest only.
    ///         - SERVICER_ADVANCES_BOTH - The servicer advances both interest and principal.
    /// @param[in] set_sup_remic If TRUE this will be applied to underlying deals. If FALSE, then the underlying deals will use the system default, SERVICER_ADVANCES_NOTHING.
    /// 
    /// @return None
    /// 
    /// @sample
    ///     void* ptid=NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"BAFC08R2");
    ///
    ///     open_deal_ex(ptid,&deal);
    ///     // Deal is already opened
    ///
    ///     // Set the servicer to advance both interest and principal
    ///     // Apply to any underlying deals
    ///     set_service_advances_ex(ptid, SERVICER_ADVANCES_BOTH, true);
    ///
    ///     run_deal_ex(ptid,&deal);
    ///     close_deal_ex(ptid,&deal);
    /// @endsample
    /// 
    /// @note If this function is not called, the system default is to use SERVICER_ADVANCES_NOTHING.
    ///
    /// @sa 
    /// - This function replaces set_services_advances, which has been deprecated.
    /// - set_service_reimburse_advint, which can be used to set whether the servicer will be reimbursed for principal and interest advances.
    /// - set_reremic_serice_advances() sets the type of cashflow that the servicer advances when the underlying deal's collateral is delinquent or in default.
    ///
    void CHASAPI set_service_advances_ex(void *tid, const int type, 
        BOOLYAN set_sup_remic);

    /// Determines whether or not the deal will be run to call. The setting will apply to all underlying deals if set_sup_remic is TRUE.
    /// By default, deal will run into maturity instead of call.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] state Turns the call On (1) or Off (0).
    /// @param[in] loan_num Reserved for future use. Always pass -1.
    /// @param[in] set_sup_remic The clean-up call setting will apply to all underlying deals if set_up_remic is TRUE for CHS deals
    ///            It will NOT apply to underlying deals if it is FALSE. This parameter is not implemented for SFW deals at the moment.
    ///
    /// @retval  0 No error
    /// @retval -1 Error - Deal not opened
    /// @retval -2 Error - Invalid dso identifier (tid)
    /// @retval -3 Error - Invalid loan number
    ///
    ///
    /// @sample
    ///     set_input_path("C:/tmp/deals");
    ///     set_engine_preference(PICK_CHS_ENGINE_FOR_MAPPED_DEALS);
    ///
    ///     void* tid=NULL;
    ///     CMO_STRUCT *pCmo= new CMO_STRUCT;
    ///     char deal_id[] = "AAM0401";
    ///     memset(pCmo, 0, sizeof(CMO_STRUCT));
    ///     memcpy(pCmo->dealid, deal_id, sizeof(deal_id) / sizeof(char));
    ///
    ///     open_deal_ex(tid, pCmo);
    ///     // Deal is already opened
    ///
    ///     // Set the cleanup call on and apply to any underlying deals
    ///     clean_up_call_ex(tid, 1, -1, true);
    ///
    ///     close_deal_ex(tid, pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    /// @note
    ///     - The state of clean_up_call_ex() must be set to on (1) in order for the call to be processed, even if the call criteria is met or overridden.
    ///     - For SFW deals, if called with state set to on (1), it is same as calling set_call_option() with type ::FORCED_EARLIEST_CALL.
    /// @see
    /// - set_call_option()
    long CHASAPI clean_up_call_ex(void *tid, short state, long loan_num,
        BOOLYAN set_sup_remic);

    /// Overrides the trigger calculations for the indicated trigger.
    /// 
    /// @since 1.2.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] trigger_name The case-sensitive name of the trigger whose information is being overridden.
    /// @param[in] is_vector The entries in the vector being passed, or -1 if a constant is passed. 
    ///     The first entry in a vector corresponds to period 0. 
    /// @param[in] override A pointer to the value(values) being passed. Each value is a signed char(SBYTE) with one of the following values:
    ///     - -1 Ignore override – use the paydown rules calculation
    ///     -  0 Trigger fails – (condition not met).
    ///     -  1 Trigger passes – (condition met).
    ///     
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -3 Error - Trigger not in deal
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
	/// @sample 
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(CMO_STRUCT));
    ///     char deal_id[] = "AL2010-A";
    ///     int len = sizeof(deal_id) / sizeof(char);
    ///     memcpy(pCmo->dealid,deal_id,len);
    ///
    ///     open_deal_ex(tid,pCmo);
    ///
    ///     int trg_num = get_triggers_avail(tid, NULL, NULL);
    ///     if(trg_num>0)
    ///     {
    ///         std::vector<char> name_buf(trg_num*21);
    ///         std::vector<char*> names(trg_num);
    ///         for(int i = 0; i<trg_num; i++ )
    ///         {
    ///             names[i] = &name_buf[i*21];
    ///         }
    ///         get_triggers_avail(tid, &names.front(), NULL);
    ///         signed char over_ride = 1;
    ///
    ///         for(int i = 0; i<trg_num; i++)
    ///         {
    ///             set_trigger_override(tid, names[i], 0, &over_ride);
    ///         }
    ///     }
    ///
    ///     run_deal_ex(tid,pCmo);
    ///     close_deal_ex(tid,pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI set_trigger_override(void *tid, char * trigger_name, short is_vector, SBYTE *override);

    // Obtain Cashflows

    /// Calculates the following with a given bond settlement date( settlement_date).:
    /// - whole cashflow offsetting periods( months_offset ), 
    /// - days of accrued interest( days_accured ),
    /// - offsetting days( days_offset) for long or short in the first period for a bond transaction.
    ///  
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid The ID/Name of a bond.
    /// @param[in] settlement_date The transaction settlement date expressed as mm/dd/yy. May 3, 2001 would be expressed as 05/03/01
    /// @param[out] months_offset Number of months to offset cashflows returned by run_deal_ex (see notes)
    /// @param[out] days_accrued Days of accrued interest
    /// @param[out] days_offset Days in the first period for a bond transaction is long (positive) or short (negative)
    /// 
    /// @retval  0 No Error
    /// @retval -1 Invalid Bond ID
    /// @retval -2 Invalid Settlement Date – often caused by invalid format
    /// 
    /// @note 
    ///     - The first period in the cashflows is based on the date the deal is opened "as of".
    ///     The buyer of a bond may not be entitled to all of the cashflows due to the settlement date of the transaction.
    ///     The months_offset determines the first cashflow the buyer would be entitled to.
    ///     - The method only uses 30/360 day count convention for CHS deals, 
    ///     and uses different day count conventions(ACT/360, ACT/365, ACT/ACT, 30/360, 30/365,30E/360) for SFW/CDOnet deals.
    /// 
    /// @sample
    ///     // Determine information for evaluating transaction on bond QI
    ///     // settling on March 13th, 2005.
    ///     void * tid = NULL;
    ///     // Deal is already opened
    ///
    ///     int monthsOffset;
    ///     int daysAccrued;
    ///     int daysOffset;
    ///     int iR = calc_cashflow_offset_ex(tid, "QI", "03/13/05", &monthsOffset, &daysAccrued, &daysOffset);
    /// @endsample
    /// 
    int  CHASAPI calc_cashflow_offset_ex(void *tid, const char * bondid, const char * settlement_date, 
        int  *months_offset, int * days_accrued, int * days_offset);

    /// Returns a pointer to a vector of doubles containing the specified bond cashflow, the vector size is [MAX_PERIODS]. 
    ///
    /// The balance, principal and interest for the specified bond can also be returned in CMO_STRUCT (double principal[MAX_PERIODS], double balance[MAX_PERIODS]
    /// and double interest[MAX_PERIODS]). For detail information, please refer to CMO_STRUCT. 
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre run_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid The bond for which flows are requested.
    /// @param[in] flow_identifier Identifies the requested cash flow.
    ///         Must be one of the bond cashflow identifiers (see ::EXTENDED_FLOW_BOND_IDENTIFIER).
    /// @retval NULL Error - Call get_deal_error_msg(). 
    /// @retval 0   get bond flow successfuly.
    /// @retval OTHER Pointer to the vector of cashflows.
    ///
    /// @sample
    ///     void* ptid= NULL;
    ///     CMO_STRUCT cmo;
    ///     memset(&cmo,0,sizeof(CMO_STRUCT));
    ///     strcpy(cmo.dealid,"ACE06NC1");
    ///
    ///     open_deal_ex(ptid,&cmo);
    ///     run_deal_ex(ptid,&cmo);
    ///     // Deal is already opened successfully.
    ///     // Deal is already run successfully.
    ///
    ///     // Get bond flow of balance.
    ///     double* pbond_balance = get_bond_flow_ex(ptid,cmo.bond.stripped_id,FLOW_BOND_BALANCE);
    ///
    ///     close_deal_ex(ptid,&cmo);
    /// @endsample
    ///
    /// @note 
    /// - Call set_deal_calc_level() with parameter CALC_LEVEL_FULL to retrieve FLOW_BOND_INTEREST_SHORTFALL, FLOW_BOND_BASIS_SHORTFALL, FLOW_BOND_CAPPED_COUPON and FLOW_BOND_REIMB_LOSSES
    ///  via get_bond_flow_ex(),after open_deal_ex() but before run_deal_ex() 
    /// - If error message is 'Need to call open_deal_ex first!!!', it means deal is not opened.
    /// - If error message is 'bondid provided is NULL.', it means input parameter bondid is NULL.
    /// - If error message is 'Unable to find tranche ErrBondid', it means input parameter, bondid is invalid.
    ///
    /// @sa
    /// - get_bond_flow_ex can be used to get specified bond cashflows. It returns projected bond cashflows specified by flow_identifier.
    /// - get_bond_flow_ex1 can return specified bond cashflows for all bond flow identifiers.
    /// - get_bond_flow_ex1_for_managed_code is similar to get_bond_flow_ex1, but it needs the user to allocate data structure before the call.
    ///
    double * CHASAPI get_bond_flow_ex(void *tid, const char * bondid,int flow_identifier);


    /// Returns a pointer to MARKIT_BOND_CASHFLOW structure with bond cashflow.
    ///
    /// This function is different from get_bond_flow_ex() by the fact that it might have
    /// one historical payment to which many zero delay floater bonds are entitled to, while get_bond_flow_ex() returns only projected cashflows.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre run_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier.
    /// @param[in] reremic_deal_id_or_null 0 for parent deal or name of the child deal.
    /// @param[in] bondid The tranche name.
    ///
    /// @retval Null Error - call get_deal_error_msg()
    /// @retval Not-Null Pointer to MARKIT_BOND_CASHFLOW structure with all the cashflow information for the deal. Please refer to MARKIT_BOND_CASHFLOW for details.
    ///
    /// @sample
    ///     void* ptid= NULL;
    ///     CMO_STRUCT cmo;
    ///     memset(&cmo,0,sizeof(CMO_STRUCT));
    ///     strcpy(cmo.dealid,"ACE06NC1");
    ///
    ///     open_deal_ex(ptid,&cmo);
    ///     run_deal_ex(ptid,&cmo);
    ///     // Deal is already opened successfully.
    ///     // Deal is already run successfully.
    ///
    ///     // Get bond flow of balance
    ///     MARKIT_BOND_CASHFLOW* pbond_cash_flow = get_bond_flow_ex1(ptid,cmo.bond.stripped_id);
    ///     if(NULL == pbond_cash_flow)
    ///     {
    ///         const char*err_msg=get_deal_error_msg(ptid);
    ///         if(NULL!=err_msg)
    ///         {
    ///             std::cout<<"Fail to call get_bond_flow_ex1:"<<err_msg<<std:endl;
    ///         }
    ///     }
    ///
    ///     close_deal_ex(ptid,&cmo);
    /// @endsample
    ///
    /// @note 
    /// - Call set_deal_calc_level() with CALC_LEVEL_FULL setting to get
    /// the value for the following fields: double*interest_due, double*interest_shortfall, int*accrual_days, double*uncapped_rate) in MARKIT_BOND_CASHFLOW after calling open_deal_ex() but before run_deal_ex() 
    /// 
    /// @sa
    /// - get_bond_flow_ex1_for_managed_code can return specified bond cashflows for both top level deals and underlying deals. It needs user allocate MARKIT_BOND_CASHFLOW_FOR_MANAGED_CODE structure before the call.
    /// - get_bond_flow_ex1 returns a pointer to the bond cashflows of MARKIT_BOND_CASHFLOW structure.
    /// - get_bond_flow_ex can be used to get specified bond cashflows for both top level deals and underlying deals. It returns projected bond cashflows specified by flow_identifier.
    ///
    MARKIT_BOND_CASHFLOW* CHASAPI get_bond_flow_ex1(void* tid, const char* reremic_deal_id_or_null,const char * bondid);

    /// Return specified bond cashflow of MARKIT_BOND_CASHFLOW structure via output parameter "cf".
    /// It is a variation of function get_bond_flow_ex1(), which returns a pointer. The difference is that the structure MARKIT_BOND_CASHFLOW_FOR_MANAGED_CODE needs to be allocated by the user when this function is called.
    /// All data will be stored in static memory. 
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre run_deal_ex() has been called.
    /// 
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null If reremic deal, this is the id, otherwise null
    /// @param[in] bondid The bond id
    /// @param[out] cf Pointer to the structure holding bond cash flow data
    /// 
    /// @retval   0 Success
    /// @retval -99 Error, invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// @retval  -1 Error, deal is not opened
    /// @retval  <0 Others Error, for details call get_deal_error_msg()
    /// 
    /// @sample
    ///     void* ptid= NULL;
    ///     CMO_STRUCT cmo;
    ///     memset(&cmo,0,sizeof(CMO_STRUCT));
    ///     strcpy(cmo.dealid,"ACE06NC1");
    ///
    ///     open_deal_ex(ptid,&cmo);
    ///     run_deal_ex(ptid,&cmo);
    ///     // Deal is already opened successfully
    ///     // Deal is already run successfully
    ///
    ///     // Get bond flow of balance
    ///     MARKIT_BOND_CASHFLOW_FOR_MANAGED_CODE MarkitBondCf;
    ///     memset(&MarkitBondCf,0,sizeof(MARKIT_BOND_CASHFLOW_FOR_MANAGED_CODE));
    ///     int iret = get_bond_flow_ex1_for_managed_code(ptid, NULL, cmo.bond.stripped_id, &MarkitBondCf);
    ///     if(0!=ret)
    ///     {
    ///         const char*err_msg=get_deal_error_msg(ptid);
    ///         if(NULL!=err_msg)
    ///         {
    ///             std::cout<<"Fail to call get_bond_flow_ex1_for_managed_code:"<<err_msg<<std::endl;
    ///         }
    ///     }
    ///
    ///     close_deal_ex(ptid,&cmo);
    /// @endsample
    ///
    /// @note 
    /// - Call set_deal_calc_level() with CALC_LEVEL_FULL,
    /// to get value for the following fields : double interest_due[MAX_PERIOD], double interest_shortfall[MAX_PERIODS], double uncapped_rate[MAX_PERIODS] in MARKIT_BOND_CASHFLOW.
    /// 
    /// @sa
    /// - get_bond_flow_ex1_for_managed_code can return specified bond cashflows for both top level deals and underlying deals. It needs user allocate MARKIT_BOND_CASHFLOW_FOR_MANAGED_CODE structure before the call.
    /// - get_bond_flow_ex1 returns a pointer to the bond cashflows of MARKIT_BOND_CASHFLOW structure.
    /// - get_bond_flow_ex can be used to get specified bond cashflows for both top level deals and underlying deals. It returns projected bond cashflows specified by flow_identifier.
    ///
    int CHASAPI get_bond_flow_ex1_for_managed_code(void* tid, const char* reremic_deal_id_or_null,const char* bondid, MARKIT_BOND_CASHFLOW_FOR_MANAGED_CODE* cf);

    /// Get the bond info from its tranche name. 
    /// 
    /// @since 1.1.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null If reremic deal, this is the id, otherwise null
    /// @param[in] bondid The bond tranche name 
    /// @param[out] bond_info Pointer to the structure holding bond info
    /// 
    /// @retval   0 Success
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @sample
    ///     void * ptid = NULL;
    ///     // Deal is already open.
    ///
    ///     std::vector<CCMO_BONDS_S> pBonds(pCmo->num_bonds);
    ///     view_all_bonds_ex(ptid, &pBonds.front());
    ///     for(int i = 0; i <= pBonds.size(); i++)
    ///     {
    ///         MARKIT_BOND_INFO bi;
    ///         get_bond_info_by_tranche(ptid, NULL, pBonds[i].stripped_id, &bi)
    ///     }
    /// @endsample
    /// 
    /// @sa
    /// - get_bond_info_by_index()
    int CHASAPI get_bond_info_by_tranche(void* tid, const char* reremic_deal_id_or_null, const char* bondid, MARKIT_BOND_INFO* bond_info);


    /// Gets information for any single bond in the deal based on its index or position in the capital structure. The index values start from 1.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// 
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] reremic_deal_id_or_null If reremic deal, this is the ID, otherwise null.
    /// @param[in] index The 1-based index of the bond in the array of bonds.
    /// @param[out] bond_info A pointer to the structure holding the bond information.
    ///
    /// @retval  0 Success
    /// @retval  -1 Deal not open
    /// @retval  -3 Invalid bond index
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @sample
    ///     void* ptid=NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"BAFC08R2");
    ///
    ///     open_deal_ex(ptid,&deal);    
    ///     // Deal is already opened.
    ///
    ///     MARKIT_DEAL_INFO dealInfo;
    ///     memset(&dealInfo, 0, sizeof(MARKIT_DEAL_INFO));
    ///     get_deal_info(ptid,0,&dealInfo);
    ///     
    ///     for(int i =1;i<= dealInfo.num_bonds; i++)
    ///     {
    ///         MARKIT_BOND_INFO bi;
    ///         get_bond_info_by_index(ptid, NULL, i, &bi);
    ///     }
    ///     
    ///     close_deal_ex(ptid,&deal);
    /// @endsample
    ///
    /// @sa
    /// - get_bond_info_by_tranche() 
    /// - get_bond_index_ex()
    /// @warning get_bond_info_by_index() uses a 1-based index whereas get_bond_index_ex() returns a 0-based index.
    /// 
    int CHASAPI get_bond_info_by_index(void* tid, const char* reremic_deal_id_or_null,int index,MARKIT_BOND_INFO* bond_info);


    /// Sets the bond cash flow mode and payment dates type.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] mode Cashflow generating mode. Must be one of ::BOND_CF_MODE.
    /// @param[in] payment_dates_type Bond payment date type. Must be one of ::BOND_PAYMENT_DATES_TYPE.
    /// @param[in] propagate_to_remics A flag to indicate if set the same for the underlying remic deals.
    ///
    /// @retval  0 No error
    /// @retval <0 Error - check details by calling get_deal_error_msg()
    ///
    /// @warning
    ///     * If this api is not called
    ///         * CHS engine will use ::BOND_CF_MODE_TRADING as bond cf mode
    ///         * SFW engine will use ::BOND_CF_MODE_HOLDING as bond cf mode
    ///         * CDOnet engine will use ::BOND_CF_MODE_TRADING as bond cf mode
    ///     * For payment_dates_type
    ///         * It applies to CHS deal only
    ///         * SFW and CDOnet deal will use its internal logic to determine payment dates for a bond.
	///
	/// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "AL2010-A");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     open_deal_ex(tid, pCmo);
    ///
    ///     int ret =  set_bond_cf_mode(tid,BOND_CF_MODE_TRADING,BOND_PAYMENT_DATES_THEORETICAL,0);
    ///     if(ret!=0)
    ///     {
    ///         //error handle;
    ///     }
    ///
    ///     close_deal_ex(tid,pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI set_bond_cf_mode(void* tid, BOND_CF_MODE mode, BOND_PAYMENT_DATES_TYPE payment_dates_type,int propagate_to_remics);

    /// This function calculates cashflow analytics for a given bond. It should be called after running cashflow
    /// 
    /// @since 1.2.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre run_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid The bond identifier.
    /// @param[in] anchorType Type of anchor for pricing, available options: PRICE, YIELD, or DM
    /// @param[in] anchorValue Value of the provided pricing anchor
    /// @param[out] results Calculated bond analytics
    /// 
    /// @retval =0 Successful
    /// @retval <0 Failed, check details by calling get_deal_error_msg()
	///
	/// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ACE06NC1");
    ///
    ///     set_engine_preference(PICK_SFW_ENGINE_FOR_MAPPED_DEALS);
    ///     open_deal_ex(tid, pCmo);		
    ///
    ///     double dAnchor             = 2.0000;
    ///     PRICING_ANCHORS anchorType = YIELD;
    ///     char *szBondid             = "A1";
    ///     PRICING_RESULTS results;
    ///     memset(&results, 0x00, sizeof(PRICING_RESULTS));
    ///     //validate, before run_deal_ex(), call price_bond 
    ///     assert(-99 == price_bond(tid, szBondid, anchorType, dAnchor, &results));
    ///
    ///     assert(0 == run_deal_ex(tid,pCmo));
    ///
    ///     memset(&results, 0x00, sizeof(PRICING_RESULTS));
    ///     int nRet = price_bond(tid, szBondid, anchorType, dAnchor, &results);
    ///     if(nRet !=0)
    ///     {
    ///         //error handle;
    ///     }
    ///
    ///     close_deal_ex(tid,pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI price_bond(void* tid, const char * bondid,PRICING_ANCHORS anchorType,double anchorValue,PRICING_RESULTS* results);

    /// Sets the specified bond cashflow to the values passed (in a vector) for the (see ::price_bond())
    ///     the number of periods specified (all later periods will be set to zero (0).)
    /// 
    /// @since 2.1.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre run_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid The bond identifier.
    /// @param[in] flow_identifier Identifies the requested cash flow.
    ///          - For CDOnet and SFW, must be one of the following bond cashflow identifiers:
    ///          - ::FLOW_BOND_BALANCE
    ///          - ::FLOW_BOND_INTEREST
    ///          - ::FLOW_BOND_PRINCIPAL
    ///          - ::FLOW_BOND_CAPPED_RATE
    ///          - ::FLOW_BOND_RATE
    ///          - For CHS must be one of the following bond cashflow identifiers:
    ///          - ::FLOW_BOND_BALANCE
    ///          - ::FLOW_BOND_INTEREST
    ///          - ::FLOW_BOND_PRINCIPAL
    ///          - ::FLOW_BOND_FLT_INDEX
    ///          - ::FLOW_BOND_PRINCIPAL_WRITEDOWN
    ///          - ::FLOW_BOND_INTEREST_SHORTFALL 
    /// @param[in] flow_length The number of values being passed in the array "flows".
    /// @param[in] flows A pointer to an array of cash flows.
    /// 
    /// @retval   1 No error
    /// @retval  -1 Deal not open
    /// @retval  -2 Invalid bond id
    /// @retval  -3 Invalid cashflow type
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @sample
    ///     void * pDeal = NULL;
    ///     //Deal have opened
    ///
    ///     double balance[] = {12020209.49,11997482.94,11974643.97,11951692.01,11928626.51,0.0};
    ///     double principal[] = {0,22726.54,22838.90,22951.95,23065.50,0.0};
    ///     double interest[] = {0,5092.46,2899.39,2893.872,2888.32,0.0};
    ///     assert(0 == set_bond_flow(pDeal, "A1", FLOW_BOND_BALANCE, sizeof(balance)/sizeof(double), balance));
    ///     assert(0 == set_bond_flow(pDeal, "A1", FLOW_BOND_INTEREST, sizeof(interest)/sizeof(double), interest));
    ///     assert(0 == set_bond_flow(pDeal, "A1", FLOW_BOND_PRINCIPAL, sizeof(principal)/sizeof(double), principal));
    /// @endsample
    ///
    /// @warning If user want to get the price results with the custom bond cashflow, this function should be called befor price_bond()
    ///
    short CHASAPI set_bond_flow(void *tid, const char* bondid, int flow_identifier, short flow_length, double * flows);
       
    /// Returns the deal level payment date corresponding to the requested period.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called, and get_longest_ex has been called to get the max size of valid cf dates.
    ///
    /// @param[in] per The period whose cashflow date is being requested. Must be between 0 and MAX_PERIODS - 1.
    /// @param[out] date A pointer to a null-terminated string (YYYYMMDD). This must be pre-allocated with at least 11 characters.
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    ///
    /// @retval `YYYYMMDD' The payment date in YYYYMMDD format. The first period stores the last payment date of the current deal update.
    /// @retval `-2' Period(per) is out of range. Call get_deal_error_msg() for details.
    /// @retval `Error' Other errors. Call get_deal_error_msg() for details. 
    ///
    /// @note If get_cf_date() returns error code string instead of valid date string, the user should call get_deal_error_msg() to get detail error message.
    ///
    /// @sample
    /// 	set_input_path("C:/Deals");
    /// 	set_engine_preference(PICK_CHS_ENGINE_FOR_MAPPED_DEALS);
    ///
    /// 	void* tid=NULL;
    /// 	CMO_STRUCT cmo={};
    /// 	strcpy(cmo.dealid,"AAM0401");
    /// 	open_deal_ex(tid, &cmo);
    /// 	// Deal is opened
    ///	
    /// 	int longest=get_longest_ex(tid);
    /// 	char cf_date[11]={};
    /// 	for (int i=0; i<=longest; ++i)
    /// 	{
    /// 	    char* msg=get_cf_date(i, cf_date, tid);
    /// 	    std::cout<< msg << std::endl;
    /// 	}
    ///
    /// 	close_deal_ex(tid, &cmo);
    /// @endsample
    ///
    char* get_cf_date(int per, char * date, void *tid);

    /// Returns the payment date corresponding to the requested period and bond. 
    /// This method returns values from MARKIT_BOND_CASHFLOW.dates, which are adjusted based on the bond-level BUS_RULES set up in the deal file. 
    /// 
    /// @since 1.5.0
    /// @avail CHS, SFW
    ///
    /// @pre run_deal_ex() has been called, and get_longest_ex has been called to get the max size of valid cf dates.
    ///
    /// @param per The period whose cashflow date is being requested. Must be between 0 and MAX_PERIODS - 1.
    /// @param date A pointer to a null-terminated string (YYYYMMDD). This must be pre-allocated with at least 11 characters.
    /// @param tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param bondid The name of the bond whose date is being requested.
    ///
    /// @retval Pointer A pointer to a string. The date in YYYYMMDD format.
    /// @retval -2 Error - Period(per) is out of range.
    /// @retval Other Error - Call get_deal_error_msg() for details.
    ///
    /// @note
    /// - This is a variation of the existing deal-level get_cf_date() method. Different bonds can have different BUS_RULES. 
    /// This method returns the adjusted bond-level payment dates, whereas get_cf_date() returns the unadjusted deal-level payment dates.
    /// - For more information about BUS_RULE, please refer to ::MARKIT_BOND_INFO.
    ///
    /// @sample
    /// 	void* tid = NULL;
    /// 	CMO_STRUCT *pCmo = new CMO_STRUCT();
    /// 	memset(pCmo, 0, sizeof(CMO_STRUCT));
    ///
    /// 	set_input_path("C:\\");
    /// 	// open deal
    /// 	if(0 != open_deal_ex(tid, pCmo))
    /// 	{
    /// 	    delete pCmo;
    /// 	    pCmo = NULL;
    /// 	    return;
    /// 	}
    /// 	// run deal
    /// 	if(0 != run_deal_ex(tid, pCmo))
    /// 	{
    /// 	    delete pCmo;
    /// 	    pCmo = NULL;
    /// 	    return;
    /// 	}
    ///
    /// 	// get cf_date of period 9 in bond A1
    /// 	char cf_date[11] = {0};
    /// 	get_bond_cf_date(9, cf_date, tid, "A1");
    ///
    /// 	// close deal
    /// 	close_deal_ex(tid, pCmo);
    /// 	delete pCmo;
    /// 	pCmo = NULL;
    /// @endsample
    char* get_bond_cf_date(int per, char * date, void *tid, const char* bondid);


    /// Returns a pointer to a vector of doubles containing the specified collateral cashflow, the vector size is MAX_PERIODS.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre run_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] group_number The collateral group for which cashflows are requested, 0 for total (deal level).
    /// @param[in] flow_identifier Identifies the requested cash flow.
    ///         Must be one of the collateral cashflow identifiers (see ::EXTENDED_FLOW_COLLATERAL_IDENTIFIER).
    /// @retval NULL Error - Call get_deal_error_msg() for detail.
    /// @retval OTHER Pointer to the vector of cashflows
    ///
    /// @note 
    /// Call set_deal_calc_level() with parameter CALC_LEVEL_FULL after open_deal_ex() but before run_deal_ex() to retrieve the following collateral flow identifiers:
    /// - ::FLOW_COLLATERAL_PO_BALANCE,
    /// - ::FLOW_COLLATERAL_PO_LOSSES,
    /// - ::FLOW_COLLATERAL_EXCESS_INTEREST,
    /// - ::FLOW_COLLATERAL_NEGATIVE_AMORT,
    /// - ::FLOW_COLLATERAL_DRAW_AMOUNT,
    /// - ::FLOW_COLLATERAL_BEG_BAL_INTEREST,
    /// - ::FLOW_COLLATERAL_SCHED_P_AND_I,
    /// - ::FLOW_COLLATERAL_PO_SCHED_PRIN,
    /// - ::FLOW_COLLATERAL_PO_PREPAYMENTS,
    /// - ::FLOW_COLLATERAL_PO_PRIN_RECOVERIES,
    /// - ::FLOW_COLLATERAL_PERFORMING_BALANCE,
    /// - ::FLOW_COLLATERAL_IO_BALANCE,
    /// - ::FLOW_COLLATERAL_CPR_1M.
    /// - ::FLOW_COLLATERAL_CDR_1M.
    /// - ::FLOW_COLLATERAL_DELBAL_30,
    /// - ::FLOW_COLLATERAL_DELBAL_60,
    /// - ::FLOW_COLLATERAL_DELBAL_90P.
    ///
    /// @sample
    /// 	void*ptid=NULL;
    /// 	CMO_STRUCT cmo;
    /// 	memset(&cmo, 0, sizeof(CMO_STRUCT));
    /// 	strcpy(cmo.dealid,"ACE06NC1");
    ///
    /// 	open_deal_ex(ptid,&cmo);
    /// 	run_deal_ex(ptid,&cmo);
    /// 	// Deal is already opened successfully.
    /// 	// Deal is already run successfully.
    ///
    /// 	// Get collateral flow of balance.
    /// 	double*pcollat_flow=get_collateral_flow_ex(ptid, 1, FLOW_COLLATERAL_BALANCE);
    /// 	if(NULL==pcollat_flow)
    /// 	{
    /// 	    const char*err_msg=get_deal_error_msg(ptid);
    /// 	    if(NULL !=err_msg)
    /// 	    {
    /// 	        std::cout<<"Fail to call get_collateral_flow_ex:"<<err_msg<<std::endl;
    /// 	    }
    /// 	}
    ///
    /// 	close_deal_ex(tid, &cmo);
    /// @endsample
    ///
    /// @sa 
    /// - get_collateral_flow_ex() is used to get specified collateral cashflow. It returns projected collateral cashflows specified by flow_identifier.
    /// - get_collateral_flow_ex1() returns the specified group collateral cashflows. It needs user to allocate the MARKIT_COLLAT_CASHFLOW data structure.
    ///
    double * CHASAPI get_collateral_flow_ex(void *tid, long group_number, int flow_identifier );


    /// Populates the user allocated structure of type MARKIT_COLLAT_CASHFLOW with collateral cash flow data and deal settings.
    /// The dynamic memory within the structure MARKIT_COLLAT_CASHFLOW will be allocated by the SDK.
    ///
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre run_deal_ex() has been called.
    ///
    /// @retval   0 SUCCESS
    /// @retval Others Error - check the message for details
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] group_number The collateral group for which cashflows are requested, 0 for total (deal level).
    /// @param[in] reremic_deal_id_or_null Reremic deal ID or null if not reremic deal.
    /// @param[out] cf The collateral cash flow data.
    ///
    /// @sample
    /// 	void *ptid = NULL;
    /// 	CMO_STRUCT cmo;
    /// 	memset(&cmo, 0, sizeof(CMO_STRUCT));
    /// 	strcpy(cmo.dealid, "ACE06NC1");
    ///
    /// 	open_deal_ex(ptid, &cmo);
    /// 	run_deal_ex(ptid, &cmo);
    ///	
    /// 	// Get collateral flow of balance.
	///     MARKIT_COLLAT_CASHFLOW MarkitCollatFlow;
	///     int group_num = 1;
	/// 	memset(&MarkitCollatFlow, 0, sizeof(MARKIT_COLLAT_CASHFLOW));
    /// 	int ret = get_collateral_flow_ex1(ptid, group_num, NULL, &MarkitCollatFlow);
    /// 	if(0 != ret)
    /// 	{
    /// 	    const char* err_msg = get_deal_error_msg(ptid);
    /// 	    if(NULL != err_msg)
    /// 	    {
    /// 	        std::cout<< "Fail to call get_collateral_flow_ex1:" << err_msg << std::endl;
    /// 	    }
    /// 	}
    ///
	///     close_deal_ex(ptid, &cmo);
    /// @endsample
    ///
    /// @note
    /// User can call set_deal_calc_level() with parameter CALC_LEVEL_FULL after open_deal_ex() but before run_deal_ex() to retrieve the following fields in MARKIT_COLLST_CASHFLOW: 
    /// double gross_interest[MAX_PERIODS], double sched_p_and_i[MAX_PERIODS], double negative_amortization[MAX_PERIODS], double draw_amount[MAX_PERIODS], double total_excess_losses[MAX_PERIODS],
    /// double po_balance[MAX_PERIODS], double po_losses[MAX_PERIODS], double po_prepayment[MAX_PERIODS], double po_prin_recoveries[MAX_PERIODS], double po_sched_principal[MAX_PERIODS], 
    /// double premium_loan_balance[MAX_PERIODS], double excess_interest[MAX_PERIODS].
    ///
    /// @sa
    /// - get_collateral_flow_ex() can return the specified group collateral cashflows. It needs the user to allocate the data structure MARKIT_CPLLAT_CASHFLOW.
    /// - get_collateral_flow_ex1() is used to get specified collateral cashflows. It returns projected collateral cashflows specified by flow_identifier.
    int CHASAPI get_collateral_flow_ex1(void *tid, int group_number, const char* reremic_deal_id_or_null, MARKIT_COLLAT_CASHFLOW* cf);

    /// Returns the status information for the requested trigger.
    /// 
    /// @since 1.2.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] trigger_name The case-sensitive name of the trigger that status information is requested for.
    /// @param[out] status A pointer to a client-allocated array of SBYTE(signed char). Allocate MAX_PERIODS.
    /// 
    /// @retval   0 No Error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -3 Error - Trigger not in deal
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
	/// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "AB05HE3");
    ///
    ///     set_engine_preference(PICK_CHS_ENGINE_FOR_MAPPED_DEALS);
    ///     open_deal_ex(tid, pCmo);
    ///
    ///     int trg_num = get_triggers_avail(tid, NULL, NULL);
    ///     if(trg_num>0)
    ///     {
    ///         char *name_buf = (char*)malloc(trg_num*21);
    ///         char **names = (char**)malloc(trg_num * sizeof(char*));
    ///         for(int i = 0; i<trg_num; ++i)
    ///         {
    ///             names[i] = (char*)malloc(21);
    ///         }
    ///         signed char *status_buf = (signed char*)malloc(trg_num*MAX_PERIODS);
    ///         signed char **status = (signed char**)malloc(trg_num * sizeof(signed char*));
    ///         for(int i = 0; i<trg_num; ++i)
    ///         {
    ///             status[i] = (signed char*)malloc(MAX_PERIODS);
    ///         }
    ///         for(int i = 0; i<trg_num; i++ )
    ///         {
    ///             names[i] = &name_buf[i*21];
    ///             status[i] = &status_buf[i*MAX_PERIODS];
    ///         }
    ///
    ///         get_triggers_avail(tid, names, NULL);	
    ///         for(int i = 0; i<trg_num; i++)
    ///         {
    ///             get_trigger_status(tid, names[i], status[i]);
    ///         }	
    ///     }
    ///
	///     close_deal_ex(tid, pCmo);
    /// @endsample
    ///
    int CHASAPI get_trigger_status(void *tid, char * trigger_name, SBYTE *status);

    /// Populates an array of CMO_STRUCT structures with descriptive information on all underlying deals for the
    /// currently open deal if it is a reremic.
    /// Please note that only info of opened underlying deals will be returned.
    /// The array remics[] must be allocated to be of at least as long as the number of underlying remics requested.
    ///
    /// The return value is the number of underlying remics. To get the number of underlying deals, pass NULL for the remics[] parameter.
    ///
    /// To get information for all underlying deals, pass NULL for the dealid. 
    /// To get a single underlying deal info, pass the dealid for the underlying deal. Then the CMO_STRUCT will have information for that deal only.
    /// 
    /// @since 0.9.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] dealid The name of the underlying deal. To get information on all underlying deals, pass NULL.
    /// @param[out] remics An array of CMO_STRUCT structures, which must be allocated to hold at least as many deals
    ///     as requested (either one or all underlying remics, depending on the dealid parameter).
    ///
    /// @retval >=0 The number of underlying deals
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Requested underlying deal not part of the deal
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     // Deal is already opened
    ///
    ///     // Get the count and allocate the structures
    ///     int cDeals = view_reremic_deals(NULL, NULL, NULL);
    ///     CMO_STRUCT * allDeals = new CMO_STRUCT[cDeals];
    ///     // Try to get a specific underlying deal
    ///     view_reremic_deals(tid, "2563", allDeals);
    ///     // Get all of the underlying deals. Note that IR will be the same as cDeals
    ///     view_reremic_deals(tid, NULL, allDeals);
    ///
    ///     // Print the underlying deal names in a previously opened file
    ///     fprintf(fle, "Underlying deals for %s\n", pCmo->dealid);
    ///     for(idx = 0; idx < cDeals; idx++)
    ///        fprintf(fle, " %s", allDeals[idx].dealid);
    /// @endsample
    ///
    int CHASAPI view_reremic_deals(void *tid,char * dealid, CMO_STRUCT remics[]);


    // Reremic Bond Information

    /// Retrieves the band information for the bond  in the specified underlying deal (pricing WAL, low speed and high speed).
    /// If NULL is passed for any item, that item will not be returned.
    /// 
    /// @since 1.1.0
    /// @avail CHS, SFW
    ///	
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] dealid The name of the underlying deal.  
    /// @param[in] bondid A pointer to the name of the bond.
    /// @param[out] pricing_wal The weighted average life of the bond when the deal is priced.
    /// @param[out] low The low collar speed for the band.
    /// @param[out] high The high collar speed for the band.
    /// 
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Requested underlying deal not part of the deal
    /// @retval -10 Error - Bond not found
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
	/// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF04OPT4N");	
    ///
    ///     assert(0 == open_deal_ex(tid, pCmo));
    ///
    ///     double pricing_wal = 0.0;
    ///     double low = 0.0;
    ///     double high = 0.0;
    ///     int remic_num = view_reremic_deals(tid,NULL,NULL);
    ///     if(remic_num>0)
    ///     {
    ///         CMO_STRUCT *remics = (CMO_STRUCT*)malloc(remic_num * sizeof(CMO_STRUCT));
    ///         remic_num = view_reremic_deals(tid,NULL,remics);   
    ///         assert(0 == get_reremic_bond_band(tid, remics[0].dealid, "A1", &pricing_wal, &low, &high)); 
    ///     }
    ///     
    ///     close_deal_ex(tid,pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///		
    int CHASAPI get_reremic_bond_band(void *tid, char * dealid, 
        const char * bondid, double *pricing_wal, double *low,  double *high);

    /// Get additional information on a bond (segment, MACR or PO) in the underlying deal. 
    /// 
    /// @since 1.2.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] dealid The name of the underlying deal.  
    /// @param[in] Bond A pointer to the name of the bond.
    /// @param[out] IsSeg If non-zero, the bond is a segment bond. Otherwise it is not.
    /// @param[out] IsMACR If non-zero, the bond is a MACR bond. Otherwise it is not.
    /// @param[out] IsPO If non-zero, the bond is a PO bond. Otherwise it is not.
    /// 
    /// 
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Requested underlying deal not part of the deal
    /// @retval -10 Error - Bond not found
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @sample
    ///    void* ptid = NULL;
    ///    // deal has been opened
    ///
    ///    int remic_num = view_reremic_deals(ptid,NULL,NULL);
    ///    if(remic_num>0)
    ///    {
    ///        std::vector<CMO_STRUCT>remics(remic_num);
    ///        remic_num = view_reremic_deals(ptid,NULL,&remics.front());
    ///        BOOLYAN isSeg = 0;
    ///        BOOLYAN isMacr = 0;
    ///        BOOLYAN isPo = 0;
    ///        for(int i = 0;i<remic_num;++i)
    ///        {
    ///            int iret = get_reremic_bond_misc(ptid, remics[0].dealid, remics[0].bond.stripped_id, &isSeg, &isMacr, &isPo);
    ///            if (iret < 0)
    ///            {
    ///                //error handle
    ///            }
    ///        }
    ///    }
    /// @endsample
    ///
    /// @sa
    /// - get_bond_misc_ex()
    int CHASAPI get_reremic_bond_misc(void *tid, char * dealid, const char *Bond,
        BOOLYAN *IsSeg,BOOLYAN *IsMACR,BOOLYAN *IsPO);

    /// Returns a pointer to the collateral in the underlying deal specified by the index. This can be used to modify collateral characteristics.
    /// 
    /// @since 1.3.0
    /// @avail CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] dealid The name of the underlying deal.  
    /// @param[in] index The 0-based index of the piece of collateral.
    /// @param[out] error Error number. Values are:
    ///     -  -1 Deal not opened
    ///     -  -2 Requested underlying deal not part of the deal
    ///     -  -3 Invalid collateral index.
    ///     - -99 Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @retval NULL Error: Reason returned in error parameter
    /// @retval OTHER Pointer to CCMO_POOL_INFO structure for requested collateral
    /// 
	/// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF04OPT4N"); 
    ///
    ///     assert(0 == open_deal_ex(tid, pCmo));
    ///
    ///     int poolIdx = 1;  // The 0-based index of the piece of collateral
    ///     CCMO_POOL_INFO *pPoolInfo = NULL;
    ///     int error = 0;
    ///     int remic_num = view_reremic_deals(tid, NULL, NULL);
    ///     if(remic_num > 0)
    ///     {
    ///         CMO_STRUCT *remics = (CMO_STRUCT*)malloc(remic_num * sizeof(CMO_STRUCT));
    ///         remic_num = view_reremic_deals(tid, NULL, remics);
    ///         pPoolInfo = get_reremic_pool_ptr_by_index(tid, remics[0].dealid, poolIdx, error);
    ///         if(pPoolInfo == NULL)
    ///         {
    ///             // Invalid collateral index
    ///         }
    ///     }		
    ///     
    ///     assert(0 == close_deal_ex(tid, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///		
    CCMO_POOL_INFO * CHASAPI get_reremic_pool_ptr_by_index(void *tid, char * dealid,
        long index, int &error );   


    /// Returns the pool information for either all collateral or the requested piece of collateral in an underlying deal.
    ///
    /// @since 0.9.0
    /// @avail CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] dealid The name of the underlying deal.
    /// @param[in] index The 0-based index of the loan (-1 for all loans).
    /// @param[in] pool_size The size of the CCMO_POOL_INFO structure.
    /// @param[in] pool_ex_size The size of the CCMO_POOL_INFO_EX structure.
    /// @param[in] arm_size The size of the ARM_INFO structure. Pass 0 if you do not want arm information.
    /// @param[out] all_colls A client-allocated array of CCMO_POOL_INFO structures.
    /// @param[out] all_colls_ex A client-allocated array of CCMO_POOL_INFO_EX structures. The <i>arm</i> structure must either be allocated 
    ///                     (CCMO_ARM_INFO) or set to 0.
    ///
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened 
    /// @retval  -2 Error - Requested underlying deal not part of the deal 
    /// @retval  -3 Error - Invalid loan index
    /// @retval  -4 Error - Invalid structure size
    /// @retval  -5 Error - No output structure passed
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @note If all collateral is requested the arrays all_colls[] and all_colls_ex[] must be allocated to be of at least as long as the value 
    ///       num_colls returned by open_deal_ex() (obtained by view_reremic_deals()).
    ///     If arm_info is requested, the arm member of each CCMO_POOL_INFO_EX must be allocated.
    ///
	/// @sample
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(CMO_STRUCT));
    ///     char deal_id[] = "BAFC08R2";
    ///     int len = sizeof(deal_id) / sizeof(char);
    ///     memcpy(pCmo->dealid,deal_id,len);
    ///
    ///     open_deal_ex(tid,pCmo);
    ///
    ///     CMO_STRUCT remics[10] = {0};
    ///     view_reremic_deals(tid,NULL,remics);
    ///
    ///     std::vector<CCMO_POOL_INFO> cpi(remics[0].num_colls);
    ///     std::vector<CCMO_POOL_INFO_EX> cpi_ex(remics[0].num_colls);
    ///     std::vector<CCMO_ARM_INFO> cpi_arm_info(remics[0].num_colls);
    ///     size_t NUM_COLLS = std::abs(remics[0].num_colls);
    ///     for (size_t i=0; i<NUM_COLLS; ++i)
    ///     {
    ///         cpi_ex[i].arm=&(cpi_arm_info[i]);
    ///     }
    ///
    ///     view_reremic_colls_ex(tid, remics[0].dealid, -1, &cpi.front(), &cpi_ex.front(),sizeof(CCMO_POOL_INFO),sizeof(CCMO_POOL_INFO_EX),sizeof(CCMO_ARM_INFO));
    ///
    ///     close_deal_ex(tid,pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    ///     all_colls_info.clear();
    /// @endsample
    ///
    long CHASAPI view_reremic_colls_ex(void *tid, char * dealid, short index, CCMO_POOL_INFO all_colls[],
        CCMO_POOL_INFO_EX all_colls_ex[], short pool_size, short pool_ex_size,
        short arm_size);

    /// This retrieves the names and/or descriptions of the triggers in the underlying deal.
    /// 
    /// @since 1.2.0
    /// @avail CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] dealid The name of the underlying deal.  
    /// @param[out] trigger_names A pointer to a client-allocated array of character strings in which the names
    ///      of the triggers will be stored. 21 characters should be allocated for each string.
    /// @param[out] trigger_descs A pointer to a client-allocated array of character strings in which the descriptions
    ///     of the triggers will be stored. 1025 characters should be allocated for each string. Pass NULL if descriptions are not required.
    /// 
    /// @retval >=0 Number of miscellaneous variables
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Requested underlying deal not part of the deal
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @note Triggers are conditions that affect the custom paydown rules. In each period the trigger will
    ///      either fail (condition no met – no action) or pass (action taken).
    /// The value(s) of one of these variables can be obtained by calling get_trigger_status() after running the deal (run_deal_ex() ).
    /// The triggers can be overridden by calling set_trigger_override() before calling run_deal_ex().
    ///
	/// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF04OPT4N");
    ///
    ///     open_deal_ex(tid, pCmo);	
    ///
    ///     signed char tmp_status[MAX_PERIODS];
    ///     int remic_num = view_reremic_deals(tid, NULL, NULL);
    ///     if(remic_num > 0)
    ///     {
    ///         CMO_STRUCT *remics = (CMO_STRUCT*)malloc(remic_num * sizeof(CMO_STRUCT));
    ///         remic_num = view_reremic_deals(tid,NULL,remics);
    ///
    ///         int trg_num = get_reremic_triggers_avail(tid, remics[0].dealid, NULL, NULL);
    ///         if(trg_num < 0)
    ///         {
    ///             //error handling
    ///         }			
    ///     }	
    ///
    ///    	close_deal_ex(tid,pCmo);
    ///    	delete pCmo;
    ///    	pCmo = NULL;
    /// @endsample
    ///	
    int CHASAPI get_reremic_triggers_avail(void *tid, char * dealid, char * trigger_names[],
        char * trigger_descs[]);

    /// Sets the constant or vectored prepayment speed that will be used for the collateral specified by loan_num.
    ///
    /// @since 0.9.0
    /// @avail CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] dealid The name of the underlying deal.  
    /// @param[in] type Type of prepayment curve. Must be one of:
    ///         - PREPAY_CURVE_PSA - Standard prepayment curve measuring for prepayments in the residential mortgage market. 
    ///         - PREPAY_CURVE_SMM - Monthly prepayment or default rate.
    ///         - PREPAY_CURVE_CPR - Constant Prepayment Rate(CPR): Prepayment percentage expressed as an annual compounded rate.
    ///         - PREPAY_CURVE_CPY - Constant Prepayment Yield(CPY): It is equivalent to the Constant Prepayment Rate(CPR) except that it assumes prepayment only happens after contractual lockout and yield maintenance period.
    ///         - PREPAY_CURVE_HEP - Home Equity Prepayment: A measure of prepayments for closed-end, fixed rate HEL loans. This curve accounts for the faster seasoning ramp for home equity loans.
    ///         - PREPAY_CURVE_ABS - Asset-Backed Securities(ABS): It is used in ABS markets, where prepayments differ significantly from standard mortgages. This model defines an increasing sequence of monthly prepayment rates, which correspond to a constant absolute level of loan prepayments in all future periods.
    ///         - PREPAY_CURVE_CUS
	///         - PREPAY_CURVE_CPB
    /// @param[in] is_vector The length of the vector pointed to by pval or 0 if pval is a constant.
    /// @param[in] pval A pointer to the prepayment speeds (or speed). Value for current period (0-indexed element) will not be applied.
    /// @param[in] loan_num The 1-based index of the loan or -1 to apply to all collateral in the deal.
    /// 
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Requested underlying deal not part of the deal
    /// @retval  -3 Error - Invalid collateral index
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @sample
    ///     void* ptid=NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"BAFC08R2");
    ///
    ///     open_deal_ex(ptid,&deal);
    ///     // Deal is already opened
    ///
    ///     int remic_num = view_reremic_deals(ptid,NULL,NULL);
    ///     if(remic_num>0)
    ///     {
    ///         std::vector<CMO_STRUCT>remics(remic_num);
    ///         remic_num = view_reremic_deals(ptid,NULL,&remics.front());
    ///         double ppy = .01;
    ///         double ppy_array[] = {.01,.02,.03,.04,.05};
    ///         for(int i = 0;i<remic_num; ++i)
    ///         {
    ///             //set constant ppy rate for all collaterals in the underlying deal
    ///             set_reremic_prepayments(ptid, remics[i].dealid, PREPAY_CURVE_CPR, 0, &ppy, -1);
    ///
    ///             //set ppy rates vector for loan 1 in the underlying deal
    ///             set_reremic_prepayments(ptid, remics[i].dealid, PREPAY_CURVE_CPR, 5, ppy_array,1);
    ///         }
    ///     }
    ///
    ///    	close_deal_ex(ptid, &deal);
    /// @endsample
    /// 
    /// @note 
    ///     * Prepayment speeds are expressed as decimals. 5.25% would be .0525.
    ///     * If loan_num = -1, the setting will apply to all collateral loans. If loan_num is 1-based index, the setting will only apply to the loan specified by loan_num.
    ///     * If loan_num >=1 and is invalid, it will return error code -3.
    ///     * value for current period (0-indexed element) will not be applied.
    /// 
    /// @sa 
    ///  - set_prepayments_ex can be used to set ppy rates for both top level deals and underlying deals.
    ///  - set_reremic_prepayments is used to set ppy rates for underlying deal. If the deal specified by parameter dealid is not an underlying deal, it will return an error.
    ///
    long CHASAPI set_reremic_prepayments(void *tid, char * dealid, short type, short is_vector,
        double * pval, long loan_num);

    /// Sets which balance defaults are calculated from for the underlying deal.
    /// 
    /// @since 1.1.0
    /// @avail CHS
    ///	
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] dealid The name of the underlying deal.  
    /// @param[in] type The type of balance the defaults are calculated from. Must be one of:
    ///         - DEFLT_FROM_CURBAL: The current balance for the period.
    ///         - DEFLT_FROM_ORIGBAL: The balance in period 0 (deal "as of" date).
    ///         - DEFLT_FROM_ZERO_CPR: The scheduled balance if no prepayments and no defaults.
    /// 
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Requested underlying deal not part of the deal
    /// @retval -10 Error - Invalid type.
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @sample
    ///     void * tid = NULL;
    ///     // Deal is already opened
    ///
    ///     // Set the defaults from the current balance for underlying deal
    ///     // 3443Z 
    ///     set_reremic_default_from(tid, "3443Z", DEFLT_FROM_CURBAL);
    /// @endsample
    /// 
    long CHASAPI set_reremic_default_from(void *tid, char * dealid, const short type);

    /// Sets the constant or vectored default rate that will be used for the pool specified by loan_num.
    /// 
    /// @since 0.9.0
    /// @avail CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// 
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] dealid The name of the underlying deal.  
    /// @param[in] type Type of default curve. Must be one of:
    ///         - DEFAULT_CURVE_CDR - Constant Default Rate(CDR): Default percentage expressed as an annual compounded rate.
    ///         - DEFAULT_CURVE_SDA - Standard default curve, measuring for defaults in the residential mortgage market.
    ///         - DEFAULT_CURVE_MDR - Monthly Default Rate.
    ///         - DEFAULT_CURVE_PCT
    ///         - DEFAULT_CURVE_SEASONED_CDR 
    ///         - DEFAULT_CURVE_SEASONED_MDR 
    /// @param[in] is_vector The length of the vector pointed to by pval or 0 if pval is a constant.
    /// @param[in] pval A pointer to the default rates (or rate). Value for current period (0-indexed element) will not be applied.
    /// @param[in] loan_num The 1-based index of the collateral or -1 to apply to all collateral in the deal.
    /// 
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Requested underlying deal not part of the deal
    /// @retval  -3 Error - Invalid collateral index
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @sample
    ///     void* ptid=NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"BAFC08R2");
    ///
    ///     open_deal_ex(ptid,&deal);
    ///     // Deal is already opened
    ///
    ///     int remic_num = view_reremic_deals(ptid,NULL,NULL);
    ///     if(remic_num>0)
    ///     {
    ///         std::vector<CMO_STRUCT>remics(remic_num);
    ///         remic_num = view_reremic_deals(ptid,NULL,&remics.front());
    ///         double def = .01;
    ///         double def_array[] = {.01,.02,.03,.04,.05};
    ///         for(int i = 0;i<remic_num; ++i)
    ///         {
    ///           //set constant def rate for all collaterals in the underlying deal
    ///           set_reremic_defaults(ptid, remics[i].dealid, DEFAULT_CURVE_CDR, 0, &def, -1);
    ///
    ///           //set default rates vector for loan 1 in the underlying deal
    ///           set_reremic_defaults(ptid, remics[i].dealid, DEFAULT_CURVE_CDR, 5, def_array,1);
    ///         }
    ///     }
    ///
    ///     run_deal_ex(ptid,&deal);
    ///     close_deal_ex(ptid,&deal);
    /// @endsample
    /// 
    /// @note 
    ///     * Default rates are expressed as decimals. 5.25% would be .0525. 
    ///     * If loan_num = -1, the setting will apply to all collateral loans. If loan_num is 1-based index, the setting will only apply to the loan specified by loan_num.
    ///     * value for current period (0-indexed element) will not be applied.
    /// 
    /// @sa
    /// - set_defaults_ex can be used to set default rates for both top level deals and underlying deals.
    /// - set_reremic_defaults is used to set default rates for underlying deal. If the deal specified by para dealid is not an underlying deal, it will return an error. 
    ///
    long CHASAPI set_reremic_defaults(void *tid, char * dealid, short type, short is_vector, double * pval,
        long loan_num);

    /// Sets the constant or vectored recovery rate in the underlying deal for the pool specified by loan_num.  
    /// The recovery rate is the percentage of total liquidations that will be returned to the CMO, minus any amount that is used to reimburse the servicer for principal advances. 
    ///
    /// @since 0.9.0
    /// @avail CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    /// 
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] dealid The name of the underlying deal.  
    /// @param[in] is_vector The length of the vector pointed to by pval or 0 if pval is a constant.
    /// @param[in] pval A pointer to the recovery rates (or rate). Value for current period (0-indexed element) will not be applied.
    /// @param[in] loan_num The 1-based index of the collateral or -1 to apply to all collateral in the deal.
    /// 
    /// @retval   0 No error
    /// @retval  -1 Error Deal not opened
    /// @retval  -2 Error Requested underlying deal not part of the deal
    /// @retval  -3 Error Invalid collateral index
    /// @retval -99 Error Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @note 
    /// - Recovery rates are expressed as decimals. 5.25% would be .0525.
    /// - If loan_num = -1, the setting will apply to all collateral loans.
    /// If loan_num is 1-based index, the setting will only apply to the loan specified by loan_num.
    /// 
    /// @sample
    ///     void* ptid=NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"BAFC08R2");
    ///
    ///     open_deal_ex(ptid,&deal);
    ///     // Deal is already opened
    ///
    ///     int remic_num = view_reremic_deals(ptid,NULL,NULL);
    ///     if(remic_num>0)
    ///     {
    ///         std::vector<CMO_STRUCT>remics(remic_num);
    ///         remic_num = view_reremic_deals(ptid,NULL,&remics.front();
    ///         double recovery = .01;
    ///         double recovery_array[] = {.01,.02,.03,.04,.05};
    ///         for(int i = 0;i<remic_num;++i)
    ///         {
    ///           // set constant recovery rate for all collaterals in the underlying deal.
    ///           set_reremic_recoveries(ptid, remics[i].dealid, 0, &recovery, -1);
    ///
    ///           // set recovery rates vector for loan 1 in the underlying deal.
    ///           set_reremic_recoveries(ptid, remic[i].dealid, 5, recovery_array, 1);
    ///         }
    ///     }
    ///
    ///     run_deal_ex(ptid,&deal);
    ///     close_deal_ex(ptid,&deal);
    /// @endsample
    ///
    /// @note
    ///     * value for current period (0-indexed element) will not be applied.
    /// @sa 
    /// - set_recoveries_ex can be used for both top level deals and underlying deals.
    /// - set_reremic_recoveries is used for underlying deals. If the deal specified by para dealid is not an underlying deal, it will return an error.
    /// 
    long CHASAPI set_reremic_recoveries(void *tid, char * dealid, short is_vector, double * pval,
        long loan_num);

    /// Sets the recovery lag (months from default to liquidation) in the underlying deal for either the collateral specified by loan_num or for the deal as a whole if loan_num = -1.
    /// 
    /// @since 0.9.0
    /// @avail CHS, SFW
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] dealid The name of the underlying deal.  
    /// @param[in] val The lag in months.
    /// @param[in] loan_num The 1-based index of the collateral or -1 to apply to all collateral in the deal.
    /// 
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Requested underlying deal not part of the deal
    /// @retval  -3 Error - Invalid collateral index
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @sample
    ///     void* ptid=NULL;
    ///     CMO_STRUCT deal;
    ///     memset(&deal, 0, sizeof(CMO_STRUCT));
    ///     strcpy(deal.dealid,"BAFC08R2");
    ///
    ///     open_deal_ex(ptid,&deal);
    ///     // Deal is already opened
    ///
    ///     int remic_num = view_reremic_deals(ptid,NULL,NULL);
    ///     if(remic_num>0)
    ///     {
    ///         std::vector<CMO_STRUCT>remics(remic_num);
    ///         remic_num = view_reremic_deals(ptid,NULL,&remics.front());
    ///         for(int i=0; i<remic_num;++i)
    ///         {
    ///             //set recovery lag to 2 months for all collaterals in the underlying deal
    ///             set_reremic_recovery_lag(ptid, remics[i].dealid, 2, -1);
    ///
    ///             // set recovery lag to 3 months for loan 1 in the underyling deal
    ///             set_reremic_recovery_lag(ptid, remics[i].dealid, 3, 1);
    ///         }
    ///     }
    ///
    ///     run_deal_ex(ptid,&deal);
    ///     close_deal_ex(ptid,&deal);
    /// @endsample
    /// 
    /// @sa 
    /// - set_recovery_lag_ex can be used for both top level deals and underlying deals.
    /// - set_reremic_recovery_lag is used for underlying deal. If the deal specified by parameter dealid is not an underlying deal, it will return an error.
    /// 
    long CHASAPI set_reremic_recovery_lag(void *tid, char * dealid, short val, long loan_num);

    /// Sets the type of cashflows the servicer advances when the underlying deal's collateral is delinquent or in default.
    /// 
    /// @since 0.9.0
    /// @avail CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] dealid The name of the underlying deal.  
    /// @param[in] type The types of cashflow advanced. Must be one of the following:
    ///         - SERVICER_ADVANCES_NOTHING
    ///         - SERVICER_ADVANCES_INTEREST
    ///         - SERVICER_ADVANCES_BOTH
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Requested underlying deal not part of the deal
    /// @retval -10 Error - Invalid type
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
	/// @sample
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(CMO_STRUCT));
    ///     char deal_id[] = "BAFC08R2";
    ///     int len = sizeof(deal_id) / sizeof(char);
    ///     memcpy(pCmo->dealid,deal_id,len);
    ///
    ///     open_deal_ex(tid,pCmo);
    ///
    ///     set_reremic_service_advances(tid, "nullified dealid", SERVICER_ADVANCES_BOTH); 
    ///
    ///     close_deal_ex(tid,pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    long CHASAPI set_reremic_service_advances(void *tid, char * dealid, const int type);

    /// Overrides the trigger calculations for the indicated trigger in the underlying deal.
    /// 
    /// @since 1.2.0
    /// @avail CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] dealid The name of the underlying deal.  
    /// @param[in] trigger_name The case-sensitive name of the trigger whose information is being overridden.
    /// @param[in] is_vector The entries in the vector being passed, or -1 if a constant is passed.
    ///      The first entry in a vector corresponds to period 0. 
    /// @param[in] override A pointer to the value(values) being passed. Each value is a signed char(SBYTE)
    ///      with one of the following values:
    ///         - -1: Ignore override – use the paydown rules calculation
    ///         -  0: Trigger fails – (condition not met).
    ///         -  1: Trigger passes – (condition met).
    /// 
    /// @retval   0 No error
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Requested underlying deal not part of the deal
    /// @retval  -3 Error - Trigger not in deal
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
	/// @sample 
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(CMO_STRUCT));
    ///     char deal_id[] = "BAFC08R2";
    ///     int len = sizeof(deal_id) / sizeof(char);
    ///     memcpy(pCmo->dealid,deal_id,len);
    ///
    ///     open_deal_ex(tid,pCmo);
    ///
    ///     int remic_num = view_reremic_deals(tid, NULL, NULL);
    ///     std::vector<CMO_STRUCT> remics(remic_num);      
    ///     
    ///     int trg_num = get_reremic_triggers_avail(tid, remics[0].dealid, NULL, NULL);
    ///     if(trg_num>0)
    ///     {
    ///         std::vector<char> name_buf(trg_num*21);
    ///         std::vector<char*> names(trg_num);
    ///         for(int i = 0; i<trg_num; i++ )
    ///         {
    ///             names[i] = &name_buf[i*21];
    ///         }
    ///         get_reremic_triggers_avail(tid, remics[0].dealid, &names.front(), NULL);
    ///
    ///         signed char over_ride = 1;
    ///         for(int i = 0; i<trg_num; i++)
    ///         {
    ///             set_reremic_trigger_override(tid, remics[0].dealid, names[i], 0, &over_ride);
    ///         }
    ///     }
    ///
    ///     close_deal_ex(tid,pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI set_reremic_trigger_override(void *tid, char * dealid, char * trigger_name,
        short is_vector, SBYTE *override);

    /// Returns the status information for the requested trigger in the underlying deal.
    /// 
    /// @since 1.2.0
    /// @avail CHS, SFW
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] dealid The name of the underlying deal.  
    /// @param[in] trigger_name The case-sensitive name of the trigger that status information is requested for.
    /// @param[out] status A pointer to a client-allocated array of SBYTE(signed char). Allocate MAX_PERIODS.
    /// 
    /// @retval   0 No Error
    /// @retval  -1 Error Deal not opened
    /// @retval  -2 Error Requested underlying deal not part of the deal
    /// @retval  -3 Error Trigger not in deal
    /// @retval -99 Error Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
	/// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "ABF04OPT4N");
    ///
    ///     open_deal_ex(tid, pCmo);	
    ///
    ///     signed char tmp_status[MAX_PERIODS];
    ///     int remic_num = view_reremic_deals(tid, NULL, NULL);
    ///     if(remic_num > 0)
    ///     {
    ///         CMO_STRUCT *remics = (CMO_STRUCT*)malloc(remic_num * sizeof(CMO_STRUCT));
    ///         remic_num = view_reremic_deals(tid,NULL,remics);
    ///         int trg_num = get_reremic_triggers_avail(tid, remics[0].dealid, NULL, NULL);
    ///     	
    ///         if(trg_num > 0)
    ///         {
    ///             char *name_buf = (char*)malloc(trg_num*21);
    ///             char **names = (char**)malloc(trg_num * sizeof(char*));
    ///             for(int i = 0; i<trg_num; ++i)
    ///             {
    ///                 names[i] = (char*)malloc(21);
    ///             }
    ///             signed char *status_buf = (signed char*)malloc(trg_num*MAX_PERIODS);
    ///             signed char **status = (signed char**)malloc(trg_num * sizeof(signed char*));
    ///             for(int i = 0; i<trg_num; ++i)
    ///             {
    ///                 status[i] = (signed char*)malloc(MAX_PERIODS);
    ///             }
    ///             for(int i = 0; i<trg_num; i++ )
    ///             {
    ///                 names[i] = &name_buf[i*21];
    ///                 status[i] = &status_buf[i*MAX_PERIODS];
    ///             }
    ///             get_reremic_triggers_avail(tid, remics[0].dealid, names, NULL);
    ///     		
    ///             for(int i = 0; i<trg_num; i++)
    ///             {
    ///                 get_reremic_trigger_status(tid, remics[0].dealid, names[i], status[i]);
    ///             }				
    ///         }			
    ///     }	
    ///
    ///     close_deal_ex(tid,pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///		
    int CHASAPI get_reremic_trigger_status(void *tid, char * dealid, char * trigger_name, SBYTE *status);

    // Return the surveillance data if available
    /// This function retrieves the deal surveillance data as of the month and year provided in the format YYYYMM.
    /// The returned YYYYMMs are sorted in descending order, i.e., from the latest to oldest in time.
    /// If the array YYYYMMs is not big enough to hold all available data, only the latest ones will be filled in the array.
    /// The total available is passed back to user through *numAvailable. The user can re-allocate an array based on this
    ///     value and call the function again.
    /// 
    /// @since 1.0.0
    /// @avail CHS
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] sizeOfYYYYMMs The size of array YYYYMMs passed in
    /// @param[out] YYYYMMs Array to hold the available year and month
    /// @param[out] numAvailable Total number of available surveillance data YYYYMMs
    /// 
    /// @return The number of YYYYMMs passed back in the YYYYMMs array.
    /// 
	/// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "CW05J7");
    ///
    ///     open_deal_ex(tid, pCmo);
    ///
    ///     int expected_dates = 50;
    ///     int *surv_dates = (int*)malloc(expected_dates * sizeof(int));
    ///     memset(surv_dates, 0, sizeof(*surv_dates)*expected_dates);
    ///     int numAvailable=0;
    ///     int survFiles = get_surv_avail_YYYYMMs(tid, surv_dates, expected_dates, &numAvailable);
    ///     if(survFiles < 0)
    ///     {
    ///     	//error handle
    ///     }
    ///
    ///     close_deal_ex(tid,pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int  CHASAPI get_surv_avail_YYYYMMs(void *tid, int YYYYMMs[], int sizeOfYYYYMMs, int* numAvailable);

    /// Populates the user allocated buffer with surveillance data for a specific month, if the surveillance data is available.
    /// 
    /// @since 1.0.0
    /// @avail CHS
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] YYYYMM The year and month when the surveillance data was collected.
    /// @param[in,out] user_buffer The user allocated buffer to hold output data.
    /// @param[in] size_of_user_buffer The size of the allocated buffer.
    /// @param[out] actual_size Actual size of  the surveillance data.
    /// 
    /// @retval   0 Call succeeded, and data has been copied to user_buffer, and the length is actual_size.
    /// @retval  -1 Error - Deal not opened
    /// @retval  -2 Error - Surveillance data is not available for the specified date.
    /// @retval  -3 Error - User allocated buffer is not big enough to hold the data. Should call again with a bigger buffer,
    ///      at least of size actual_size.
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @sample
    ///     void * tid = NULL;
    ///     // Pre-condition: Deal is already opened
    ///
    ///     int YYYYMM = 200804;
    ///     const int SURV_DATA_SIZE_SMALL = 2048;
    ///     const int SURV_DATA_SIZE_BIG   = 4096;
    ///     char surv_buf_s[SURV_DATA_SIZE_SMALL];
    ///     char surv_buf_b[SURV_DATA_SIZE_BIG];
    /// 
    ///     size_t actSize=0;
    ///     int ret = get_surveillance_data(tid, YYYYMM, surv_buf_s, 
    ///         SURV_DATA_SIZE_SMALL,&actSize);
    ///     if(ret < 0)
    ///     {
    ///         if (ret == -3) 
    ///         {
    ///             std::cout << "Error: Buffer size is " 
    ///             << SURV_DATA_SIZE_SMALL 
    ///             << ". Need " << actual_size << "." 
    ///             << std::endl;
    ///             ret = get_surveillance_data(tid, YYYYMM, surv_buf_b, 
    ///                 SURV_DATA_SIZE_BIG,&actSize);
    ///             if (ret >= 0) 
    ///             {
    ///                 std::cout << surv_buf << std::endl;
    ///             }
    ///         }
    ///     }
    ///     else
    ///     {
    ///         std::cout << surv_buf << std::endl;
    ///     }
    /// @endsample
    /// 
    int CHASAPI get_surveillance_data(void * tid, int YYYYMM, char * user_buffer, 
        long  size_of_user_buffer, long * actual_size);

    /// This function retrieves the deal surveillance data as of the month and year provided in the format YYYYMM.
    /// 
    /// @since 1.1.0
    /// @avail CHS
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] YYYYMM the update month and year of the surveillance data for this deal
    /// @param[out] survData the surveillance data structure, the memory is allocated by the caller
    /// 
    /// @retval  0 Success
    /// @retval <0 Error: for details call get_deal_error_msg()
    /// 
	/// @sample
    ///     void * tid = NULL;
    ///   // Open deal
    ///
    ///   // retrieves the deal surveillance data as of 201501
    ///   MARKIT_DEAL_SURVEILLANCE_DATA survData;
    ///   int rVal = get_deal_surv_data(tid, &survData,201501);
    /// @endsample
    ///
    /// @deprecated This method is deprecated.
    ///
    int CHASAPI get_deal_surv_data(void* tid, MARKIT_DEAL_SURVEILLANCE_DATA* survData, int YYYYMM);

    /// This function replaces the collateral allocated  by the WSA API during loading stage of the deal with collateral prepared by the user. It allows the user to perform customized aggregation when use in combination with collateral iterator.
    /// 
    /// @since 1.2.0
    /// @avail CDOnet, CHS, SFW
    ///
    /// @param[in] tid The deal/scenario identifier obtained when the dso was created.
    /// @param[in] reremic_deal_id_or_null  0 for parent deal or name of the child deal
    /// @param[in] collat_array  User allocated array of collateral information
    /// @param[in] collat_array_size Number of collaterals in the array
    /// 
    /// @retval  0 No error
    /// @retval <0 Error: Call the get_deal_error_msg() function for more details
    /// 
    /// @note This function might be called after deal is loaded to replace collateral read from the files with the one provided by user. User has to provide collateral for every group in the deal.
    /// User is responsible for allocating and releasing the memory passed to the collat_array parameter.  The WSA API will allocate its own memory and copy information provided by the user.
    /// 
	/// @sample 
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(CMO_STRUCT));
    ///     char deal_id[] = "AL2010-A";
    ///     int len = sizeof(deal_id) / sizeof(char);
    ///     memcpy(pCmo->dealid,deal_id,len);
    ///
    ///     open_deal_ex(tid,pCmo);
    ///
    ///     std::vector<MARKIT_POOL_INFO> collat_vector;
    ///     MARKIT_POOL_INFO* coll_info =0;
    ///     void* coll_it = obtain_collat_iterator(tid, 0);
    ///
    ///     while(coll_info = get_next_collat(tid,coll_it))
    ///     {
    ///         if(!coll_info->arm && coll_info->type != REMIC)
    ///         {
    ///             int agency_type = coll_info->type;
    ///             double wacAddition = 0;
    ///             switch(agency_type)
    ///             {
    ///             case GNMA1:
    ///                 wacAddition = 0.5;
    ///                 break;
    ///             case GNMA2:
    ///                 wacAddition = 1.5;
    ///                 break;
    ///             default:
    ///                 wacAddition = 2.5;
    ///                 break;
    ///             }
    ///             wacAddition /= 100;
    ///             double wac = coll_info->psa_coupon + wacAddition;
    ///             coll_info->wac = wac;
    ///             coll_info->gross_coupon = wac;
    ///             int wala = coll_info->wala;
    ///             coll_info->wala =0;
    ///             if(coll_info->prin_lockout > 0)
    ///                 coll_info->prin_lockout += wala;
    ///         }
    ///         collat_vector.push_back(*coll_info);
    ///     }
    ///
    ///     replace_collateral_for_managed_code(tid, NULL, &collat_vector[0], -1);
    ///
    ///     close_deal_ex(tid,pCmo);
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int CHASAPI replace_collateral_for_managed_code(void* tid, const char* reremic_deal_id_or_null,MARKIT_POOL_INFO collat_array[], int collat_array_size);

    /// Returns the available dates for loan level data, the format is in YYYYMM.
    /// 
    /// @since 1.2.0
    /// @avail CHS
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] sizeOfYYYYMMs The size of array provided
    /// @param[in, out] YYYYMMs List of available loan level data dates.
    /// @param[in, out] numAvailable Total number of available YYYYMMs.
    /// 
    /// @return actual number of dates returned to the caller.
    /// 
	/// @sample
    ///     void* tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "AB05HE3");
    ///     
    ///     set_engine_preference(PICK_CHS_ENGINE_FOR_MAPPED_DEALS);
    ///     assert(0 == open_deal_ex(tid, pCmo));
    ///
    ///     int expected_dates = 30;
    ///     int dates[30];
    ///     int numAvailable=0;
    ///     int files=0;
    ///
    ///     files = get_loan_level_avail_YYYYMMs(tid, dates, expected_dates, &numAvailable);
    ///
    ///     if(files < 0)
    ///     {
    ///         //error handle
    ///     }
    ///
    ///     assert(0 == close_deal_ex(tid, pCmo));
    ///     delete pCmo;
    ///     pCmo = NULL;
    /// @endsample
    ///
    int  CHASAPI get_loan_level_avail_YYYYMMs(void *tid, int YYYYMMs[], int sizeOfYYYYMMs, int* numAvailable);

    /// Returns all available history dates for the specified pool, the format is in YYYYMM.
    /// 
    /// @since 2.4.0
    /// @avail CHS
    ///
    /// @param[in] cusip The CUSIP of the pool.
    /// @param[in] size_YYYYMMs The size of array provided
    /// @param[in, out] YYYYMMs List of available history data dates.
    /// @param[in, out] num_available Total number of available YYYYMMs.
    /// 
    /// @retval  >0 Actual number of dates returned to the caller.
    /// @retval  0  None available history dates
    /// @retval  -99 Error ,call get_deal_error_msg() for details.
    /// @sample
    ///     const char* cusip = "3128S3EF2";
    ///     int yyyymms[1] = {0};
    ///     int num_avail = 0;
    ///     int ret = get_markit_pool_history_avail_YYYYMMs(cusip, yyyymms, 1, &num_avail);
    /// @endsample
    /// 
    int CHASAPI get_markit_pool_history_avail_YYYYMMs(const char* cusip, int YYYYMMs[], int size_YYYYMMs, int* num_available);

    /// This method gets the historical data item of YYYYMM for the specified cusip, if YYYYMM=0 would return all available historical data for the specified cusip.
    ///
    /// @since 2.4.0
    /// @avail CHS
    ///
    /// @param[in] cusip The CUSIP of the pool.
    /// @param[in] history_identifier Identifies of the history data.
    ///         Must be one of the markit pool history identifiers (see ::MARKIT_POOL_HISTORY).
    /// @param[in] size_array The size of array pool_history provided.
    /// @param[in] YYYYMM The specified date (of format YYYYMM),if YYYYMM=0 would return all historical data for the specified cusip.
    /// @param[out] pool_history The list of pool history. This parameter must be pre-allocated before call this function.
    ///
    /// @retval   >0 Actual size of history array returned.
    /// @retval   0 None history data.
    /// @retval -99 Error ,call get_deal_error_msg() for details.
    /// @sample
    ///     const char* cusip = "31295WXK9";
    ///     MARKIT_POOL_HISTORY_DATA  poolhist[1] = {0};
    ///     int ret = get_markit_pool_history(cusip, MARKIT_POOL_HISTORY_CPR1M, poolhist, 1, 201507);
    /// @endsample
    ///
    int CHASAPI get_markit_pool_history(const char* cusip, const int history_identifier, MARKIT_POOL_HISTORY_DATA pool_history[], int size_array,int YYYYMM);

    /// This method gets the payment group(s) name for the specified bond.
    ///
    /// @since 2.5.0
    /// @avail CHS
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid A pointer to the name of the bond.
    /// @param[out] group_names A pointer to a client-allocated array of character strings in which the names of the groups will be stored. 10 characters should be allocated for each string. Only filled in if it is not NULL. If this parameter is NULL, the number of payment groups will return. 
    ///
    /// @retval >=0 Success. Number of payment groups.
    /// @retval -1  Deal not open.
    /// @retval -2  Bond not found.
    /// @retval -99 Error, call get_deal_error_msg() for details.
    /// @sample
    ///     void* pDeal = NULL;
    ///     // deal has been opened
    ///
    ///     int group_num = get_bond_payment_group(pDeal, "A1", NULL);
    ///     if(group_num > 0)
    ///     {
    ///         std::vector<char> name_buf(group_num*10);
    ///         std::vector<char*> names(group_num);
    ///         for(int i = 0; i<group_num; i++ )
    ///             names[i]=&name_buf[i*10];
    ///         group_num = get_bond_payment_group(pDeal, "A1", &names.front());
    ///     }
    /// @endsample
    ///
    int CHASAPI get_bond_payment_group(void *tid, const char* bondid, char* group_names[]);

    /// This method gets deal payment group information.
    /// 
    /// @since 2.6.0
    /// @avail CHS
    ///
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in,out] group_array The user allocated array to hold the group information.
    /// @param[in] group_array_size The size of the group_array.
    /// @param[in, out] num_available Total number of available deal groups.
    /// 
    /// @retval   >=0 Success. Actual number of payment groups returned.
    /// @retval  -1 Error - Deal not opened
    /// @retval -99 Error - Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    /// 
    /// @sample
    ///     void* pDeal = NULL;
    ///     //Deal has been opened
    ///
    ///     MARKIT_DEAL_PAYMENT_GROUP payment_group_array[10];
    ///     memset(&payment_group_array, 0, sizeof(MARKIT_DEAL_PAYMENT_GROUP) * 10);
    ///     int ret = get_deal_payment_group(pDeal, payment_group_array, 10, &num_avail);
    /// @endsample
    /// 
    int CHASAPI get_deal_payment_group(void *tid, MARKIT_DEAL_PAYMENT_GROUP group_array[], int group_array_size, int* num_available);
    
    /// Returns all available history dates for the specified bond, the format is in YYYYMM.
    /// 
    /// @since 2.4.0
    /// @avail CHS
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] cusip The CUSIP of the bond. if cusip equals NULL, return with deal level history available YYYYMMs.
    /// @param[in] size_YYYYMMs The size of array provided
    /// @param[in, out] YYYYMMs List of available history data dates.
    /// @param[in, out] num_available Total number of available YYYYMMs.
    /// 
    /// @retval  >0 Actual number of dates returned to the caller.
    /// @retval   0 None available history dates.
    /// @retval -99 Error ,call get_deal_error_msg() for details.
    /// @sample
    ///     void* pDeal = NULL;
    ///     //Deal has been opened
    ///
    ///     const char* cusip = "38375PKW0";
    ///     int yyyymms[1] = {0};
    ///     int num_avail = 0;
    ///     int ret = get_markit_bond_pool_history_avail_YYYYMMs(pDeal,cusip, yyyymms, 1, &num_avail);
    /// @endsample
    /// 
    int CHASAPI get_markit_bond_pool_history_avail_YYYYMMs(void *tid, const char* cusip, int YYYYMMs[], int size_YYYYMMs, int* num_available);

    /// This method gets the historical data item of YYYYMM for the specified cusip, if YYYYMM=0 would return all available historical data for the specified bond cusip.
    ///
    /// @since 2.4.0
    /// @avail CHS
    ///
	/// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] cusip The CUSIP of the bond. if cusip equals NULL, return with deal level history.
    /// @param[in] history_identifier Identifies of the history data.
    ///         Must be one of the markit pool history identifiers (see ::MARKIT_POOL_HISTORY).
    /// @param[in] size_array The size of array pool_history provided.
    /// @param[in] YYYYMM The specified date (of format YYYYMM),if YYYYMM=0 would return all historical data for the specified cusip.
    /// @param[out] pool_history The list of pool history. This parameter must be pre-allocated before call this function.
    ///
    /// @retval   >0 Actual size of history array returned.
    /// @retval   0 None history data.
    /// @retval -99 Error ,call get_deal_error_msg() for details.
    /// @sample
    ///     void* pDeal = NULL;
    ///     //Deal has been opened
    ///
    ///     const char* cusip = "38375PKW0";
    ///     MARKIT_POOL_HISTORY_DATA  poolhist[1] = {0};
    ///     int ret = get_markit_bond_pool_history(pDeal, cusip, MARKIT_POOL_HISTORY_CPR1M, poolhist, 1, 201507);
    /// @endsample
    ///
    int CHASAPI get_markit_bond_pool_history(void *tid, const char* cusip, const int history_identifier, MARKIT_POOL_HISTORY_DATA pool_history[], int size_array,int YYYYMM);

    /// Retrieves the date which two business days prior to the current accrual begin date of the bond. 
    ///
    /// @since 2.7.0
    /// @avail CHS, SFW
    ///	
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] bondid A pointer to the name of the bond.
    /// @param[out] determination_date The weighted average life of the bond when the deal is priced.
    ///
    /// @retval  0 No error
    /// @retval <0 Error: Check that deal is open and the bondid is valid
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     //Deal has been opened
    ///
    ///     int rateDeterminDate = 0;
    ///     int iRet = get_bond_rate_determination_date(pDeal, "NL", &rateDeterminDate);
    /// @endsample
    /// 
	int CHASAPI get_bond_rate_determination_date(void * tid, const char * bondid, int *determination_date);

	/// Retrieves the pool prefix for agency. 
    ///
    /// @since 2.7.0
    /// @avail CHS
    ///	
    /// @pre open_deal_ex() has been called.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[out] pool_prefix A pre-allocated pointer of at least 10 characters for the pool_prefix value get from agency LPD file.
    ///
    /// @retval  0 No error
    /// @retval -1 Error: Deal not open
	/// @retval -2 Error: could not find pool prefix
	/// @retval -99 Error: Invalid dso identifier (tid) or other errors, please see details by calling get_deal_error_msg()
    ///
    /// @sample
    ///     void* pDeal = NULL;
    ///     //Deal has been opened
    ///
	///     char pool_prefix[10];
    ///     int iRet = get_agency_pool_prefix(pDeal, pool_prefix);
    /// @endsample
    /// 
    int CHASAPI get_agency_pool_prefix(void * tid,char * pool_prefix);

	/// Retrieves the license information. 
    ///
    /// @since 3.0.0
    /// @avail SFW, CDOnet, CHS
    ///	
    ///
    /// @param[in] num_features The number of license features.
    /// @param[out] lic_info License informations.
    ///
	/// @retval >=0 Number of license features
    /// @retval -1 Error - WSA_API.LIC not found
	/// @retval -99 Error: Other errors, please see details by calling get_deal_error_msg(NULL)
    ///
    /// @sample
	///     int arraySize = get_license_info(0, NULL);
	///     LICENSE_INFO* pLisenceInfo = new LICENSE_INFO[arraySize];
    ///     memset(pLisenceInfo, 0, sizeof(LICENSE_INFO)*arraySize);
    ///     int iRet = get_license_info(arraySize, pLisenceInfo);
	///     delete [] pLisenceInfo;
    /// @endsample
    /// 
	int CHASAPI get_license_info(int num_features, LICENSE_INFO lic_info[]);

#if __cplusplus
}
#endif
#define get_chasen_id get_markit_id


#endif // CMO_h_AFA80241_7BBA_11d1_9CCF_0040053302AE
