///
/// @file WSAAdcoProviderApi.h
///

#ifndef MARKITADCOPROVIDERAPI_H
#define MARKITADCOPROVIDERAPI_H

#ifdef CHAS_NT
#ifdef MARKITADCOPROVIDER_EXPORTS
#define ADCO_PROVIDER_API __declspec(dllexport)
#else
#define ADCO_PROVIDER_API __declspec(dllimport)
#endif
#else
#define ADCO_PROVIDER_API
#endif


///
/// @brief The fine tune parameters for the ADCO prepay model.
/// This supplements structure ::MarkitAdcoTuningParam, and is optional.
///
#ifndef __cplusplus
typedef
#endif
struct
#ifdef  __cplusplus
    ADCO_PROVIDER_API
    MarkitAdcoPrepayModelDials
#endif
{
    double  TuneAge;                ///< Speed-up/Slow-down Aging(range [0,2], default 1.0. Bigger means faster prepays)
    double  TuneBurnout;            ///< Tune Burnout(range [0,2], default 1.0. Bigger means faster prepays)
    double  TuneCATO;               ///< Scale CATO Effect
    double  TuneCashout;            ///< Scale Cashout SMM(range [0,2], default 1.0. Bigger means faster prepays)
    double  TuneCure;               ///< Scale Cure SMM(range [0,2], default 1.0. Bigger means faster prepays)
    double  TuneLag;                ///< Adjust Lag(range [0,2], default 1.0. Bigger means more lag)
    double  TuneRefi;               ///< Scale Refi SMM(range [0,2], default 1.0. Bigger means faster prepays)
    double  TuneSATO;               ///< Scale SATO Effect
    double  TuneScale;              ///< Scale overall SMM(range [0,2], default 1.0. Bigger means faster prepays)
    double  TuneSlide;              ///< Slide S-Curve by adding X bp to the spread over CCY (Default 0.0, positive means slower speeds)
    double  TuneTurnOver;           ///< Scale Turnover SMM(range [0,2], default 1.0. Bigger means faster prepays)
    int     TuningStartYear;        ///< Start year to apply tuning parameters
    int     TuningStartMonth;       ///< Start month to apply tuning parameters
    int     TuningRampMonths;       ///< Number of month for tunings to ramp up to full effect from the start date
    int     TuningEndYear;          ///< End year to apply tuning parameters
    int     TuningEndMonth;         ///< End month to apply tuning parameters
    int     TuningFadeMonths;       ///< Number of months for tunings to fade back to default effect after the end date

#ifdef  __cplusplus
    MarkitAdcoPrepayModelDials();
#endif
}
#ifndef __cplusplus
MarkitAdcoPrepayModelDials
#endif
;


///
/// @brief The fine tune parameters for the ADCO default model.
/// This supplements structure ::MarkitAdcoTuningParam, and is optional.
///
#ifndef __cplusplus
typedef
#endif
struct
#ifdef  __cplusplus
    ADCO_PROVIDER_API
    MarkitAdcoDefaultModelDials
#endif
{
    double  TuneCD;                 ///< Tune C-D transition
    double  TuneDC;                 ///< Tune D-C transition
    double  TuneDS;                 ///< Tune D-S transition
    double  TuneDT;                 ///< Tune D-T transition
    double  TuneFICO_Slide;         ///< Tune FICO/Credit Score by sliding left (negative) or right (positive)
    double  TuneFICO_Stretch;       ///< Tune FICO/Credit Score by stretching difference between its value and a midpoint. > 1.0 stretches, < 1.0 contracts
    double  TuneHPI_Slide;          ///< Tune Home Price Growth by sliding left (negative) or right (positive)
    double  TuneHPI_Stretch;        ///< Tune Home Price Growth by stretching difference between its value and a midpoint. > 1.0 stretches, < 1.0 contracts
    double  TuneMDR;                ///< Scale Default Rate
    double  TuneProbLossTC;         ///< Tune Probability of Loss from C Termination
    double  TuneProbLossTD;         ///< Tune Probability of Loss from D Termination
    double  TuneProbLossTS;         ///< Tune Probability of Loss from S Termination
    double  TuneSATOHat;            ///< Tune Sato hat in bp
    double  TuneSATO_Residual;      ///< Tune SATO residual. Direct multiplier on the SATO residual value
    double  TuneSC;                 ///< Tune S-C transition
    double  TuneST;                 ///< Tune S-T transition
    double  TuneSeverity;           ///< Scale Severity
    double  TuneSeverityTC;         ///< Tune Loss Severity from C Termination
    double  TuneSeverityTD;         ///< Tune Loss Severity from D Termination
    double  TuneSeverityTS;         ///< Tune Loss Severity from S Termination
    double  TuneWAOLTV_Slide;       ///< Tune Weighted Average LTV by sliding left (negative) or right (positive)
    double  TuneWAOLTV_Stretch;     ///< Tune Weighted Average LTV by stretching difference between its value and a midpoint. > 1.0 stretches, < 1.0 contracts

#ifdef  __cplusplus
    MarkitAdcoDefaultModelDials();
#endif
}
#ifndef __cplusplus
MarkitAdcoDefaultModelDials
#endif
;

///
/// @brief The main tuning parameters for the ADCO default model.
///
#ifndef __cplusplus
typedef
#endif
struct
#ifdef  __cplusplus
    ADCO_PROVIDER_API
    MarkitAdcoTuningParam
#endif
{
    /// Enum for the the actions when encounter error.
    typedef enum { ON_ERROR_STOP, ON_ERROR_CONTINUE } ERROR_HANDLING;
    /// Enum for the model types.
    typedef enum { MODEL_PREPAY_ONLY, MODEL_DEFAULT_AND_PREPAY } MODEL_TYPES;
	/// Enum for the DEFAULT DEFINITIONS.
	typedef enum { Repurchase = 1, D180, Liquidation } DEFAULT_DEFINITIONS;
    /// Enum for the types of curve.
    typedef enum { PAR_SWAP, PAR_TSY, SPOT_SWAP, SPOT_TSY } CURVE_TYPES;
    enum { DATA_PATH_SIZE = 1024 };
    char        AdcoDataPath[DATA_PATH_SIZE];   ///< ADCo model data path
    MODEL_TYPES modelType;                      ///< ADCo model type: MODEL_PREPAY_ONLY, MODEL_DEFAULT_AND_PREPAY
    double      prepay_magnitude;               ///< Magnitude for prepay rate
    double      default_magnitude;              ///< Magnitude for default rate
    int         recovery_lag;                   ///< Recovery lag applyed to each loan
    int         servicer_advancing;             ///< Servicer_advancing;
    ERROR_HANDLING errorHandling;               ///< Choose what to do when error occurs: stop if set ON_ERROR_STOP, continue if set ON_ERROR_CONTINUE.
    double      smmForFailedLoans;              ///< Failed loan's smm in percentage
    double      mdrForFailedLoans;              ///< Failed loan's mdr in percentage
    double      recoveryForFailedLoans;         ///< Failed loan's recovery in percentage
    int         ficoToUseIfNotAvailable ;       ///< Fico to use for loans which do not have this INFO
    int         userProvidedPerformanceInfo;    ///< Set this flag to 1 if you replaced collateral, and new collateral has loan performance info.
                                                ///<  - If user code has called api replace_collateral(), this flag can be set so user provided loan performance info will be populated to ADCo model. Loan performance info include: "Orig_Face", "State", "Original_LTV", "Credit_Score", "Property_Type", "NumUnits", "Loan_Purpose", "Occupancy", "Documentation", "CanNegAm", "ServicingFee", "Cur_Face", "Cur_LTV", "ZipCode", "CurFICO", "IsSecondMortgage", "PrepayPenaltyPercent", and more for ADCo default model, "DelinquencyStatus", "LienPosition", "OrigCombinedLTV", "CurTotalLTV", "RecastPeriod", "MaxNegAm", "CurMinimumPayment", "PayCap", "PayResetFreq", "OrigMIFlag", "CurMIFlag", "MIAmount". If loan level data are available (not repline), those info will aslo be populated.
    CURVE_TYPES curveType;                      ///< Loan is considered subprime if FICO is less than this value ( 620 if not provided)
    int         HPI_vector_size;                ///< The number of elements of HPI_vector.
    double*     HPI_vector;                     ///< Vector for HPI.
    int         ficoSubprimeThreashold;         ///< Loan is considered subprime if FICO is less than this value ( 620 if not provided)
    /// The signature of the call back function after process each loan.
    typedef int (*PROGRESS_CB)(void* userData,int pool_processed, int pools_succeeded,
                                            int pools_in_deal,
                                            char* error_message,
                                            int max_size_of_error_message);
    PROGRESS_CB progressCb;                         ///< Called after each loan is processed. If user code installs the cb function, it is up to the user to decide what to do. If somehow the cb finds there are issues, it can return negative code so current api will exit with error.
    void* userData;                                 ///< When this pointer is set, CB functions will be called with userData set. It is up to user to decide. Set to NULL if you do not need it.
    MarkitAdcoPrepayModelDials* prepayModelDials;   ///< Fine tune params, set to 0 if not needed.
    MarkitAdcoDefaultModelDials* defaultModelDials; ///< Fine tune params, set to 0 if not needed.
    int     logLevel;                               ///< Log level for write_log(). For CHS deal only for now. level: 0, no log, 1, message box for windows, >=2 and log file open, log to log file (see create_deal_scenario_object()).
    double  recovery_magnitude;                     ///< Recovery magnitude.
	short default_definitions;                      ///< default definitions
                                                    ///< Value | Meaning
													///< ------|--------
                                                    ///<  0    | users do not specify; ADCO model would decide the value
                                                    ///<  MarkitAdcoTuningParam::DEFAULT_DEFINITIONS    | users specify default definition as one of the enum
	bool UseADCO2yr10yrFcst;						///< flag of use adco 2yr and 10yr forecast or not.
													///< Value | Meaning
													///< ------|--------
													///< false | (by default),users should set LIBOR 24M and 120M rate vectors.
													///< true  | ADCO LDM model uses "Fcst_2yr.txt" and "fcst_10yr.txt" from monthly data files as default values; ADCO Prepayment Model uses "libor2yr_new.txt" and "libor10yr_new" from monthly data files as default values.

    /// The signature of the call back function before process each loan.
    typedef void (*LOAN_TUNNING_CB)(
        void* pool_info,            ///< Cast to  PAY_POOL_INFO*
        void* tid,                  ///< Use for calling SetTuningParam
        void* userData              ///< UserData member of  MarkitAdcoTuningParam structure
        );
    LOAN_TUNNING_CB loanTunningCb;              ///< Called before each loan is processed. It is for both. If it is installed, client code can set tuning parameters for each loan processed 

#ifdef  __cplusplus
    MarkitAdcoTuningParam();
#endif
}
#ifndef __cplusplus
MarkitAdcoTuningParam
#endif
;

///
/// @brief The Scenario parameters users can change from run to run, see ResetADCOScenario().
///
#ifndef __cplusplus
typedef
#endif
struct
#ifdef  __cplusplus
    ADCO_PROVIDER_API
    MarkitAdcoScenarioParams
#endif
{
    int     HPI_vector_size;    ///< The number of elements of HPI_vector.
    double* HPI_vector;         ///< Vector for HPI.
    int     reserved;           ///< Reserved for future use.

#ifdef  __cplusplus
    MarkitAdcoScenarioParams();
#endif
}
#ifndef __cplusplus
MarkitAdcoScenarioParams
#endif
;


extern "C"
{
    /// Install ADCo model to WSA API.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] tuning   A pointer to a client-allocated MarkitAdcoTuningParam structure.
    ///
    /// @retval     0 Success.
    /// @retval other Fail.
    ///
	/// @sample
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SAS059XS");
    ///     open_deal_ex(tid, pCmo);
    ///
    ///     MarkitAdcoTuningParam AdcoParam;
    ///     MarkitAdcoPrepayModelDials pDials;
    ///     AdcoParam.prepayModelDials = &pDials;
    ///     SetupADCOModel(tid,&AdcoParam);
    ///     run_deal_ex(tid, pCmo);
    /// @endsample
    ///
    int ADCO_PROVIDER_API SetupADCOModel(void* tid, MarkitAdcoTuningParam* tuning);

    /// Uninstall ADCo model from WSA API.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    ///
    /// @retval     0 Success.
    /// @retval other Fail.
    ///
	/// @sample
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SAS059XS");
    ///     open_deal_ex(tid, pCmo);
    ///
    ///     MarkitAdcoTuningParam AdcoParam;
    ///     MarkitAdcoPrepayModelDials pDials;
    ///     AdcoParam.prepayModelDials = &pDials;
    ///     SetupADCOModel(tid,&AdcoParam);
    ///     run_deal_ex(tid, pCmo);
    ///
    ///     RemoveADCOModel(tid);
    /// @endsample
    ///
    int ADCO_PROVIDER_API RemoveADCOModel(void* tid);

    /// Reset all interest rates and set the parameters in ADCo model.
    ///
    /// @param[in]  tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in]  scenParam   A pointer to a client-allocated MarkitAdcoScenarioParams structure.
    ///
    /// @retval     0 Success.
    /// @retval other Fail.
    ///
	/// @sample
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SAS059XS");
    ///     open_deal_ex(tid, pCmo);
    ///
    ///     MarkitAdcoTuningParam AdcoParam;
    ///     MarkitAdcoPrepayModelDials pDials;
    ///     AdcoParam.prepayModelDials = &pDials;
    ///     SetupADCOModel(tid,&AdcoParam);
    ///
    ///     MarkitAdcoScenarioParams params;
    ///     ResetADCOScenario(tid,&params);
    /// @endsample
    ///
    int ADCO_PROVIDER_API ResetADCOScenario(void* tid, MarkitAdcoScenarioParams* scenParam);

    /// Reset all interest rates in ADCo model.
    ///
    /// @param[in]  tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    ///
    /// @retval     0 Success.
    /// @retval other Fail.
    ///
	/// @sample
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SAS059XS");
    ///     open_deal_ex(tid, pCmo);
    ///
    ///     MarkitAdcoTuningParam AdcoParam;
    ///     MarkitAdcoPrepayModelDials pDials;
    ///     AdcoParam.prepayModelDials = &pDials;
    ///     SetupADCOModel(tid,&AdcoParam);
    ///
    ///     ResetADCOInterestRates(tid);
    /// @endsample
    ///
    int ADCO_PROVIDER_API ResetADCOInterestRates(void* tid);

    /// Reset HPI rates in ADCo model.
    ///
    /// @param[in] tid  The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] HPI_vector_size  The length of user allocated array HPI_vector.
    /// @param[in] HPI_vector   A pointer to client-allocated HPI_vector array.
    ///
    /// @retval     0 Success.
    /// @retval other Fail.
    ///
	/// @sample
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SAS059XS");
    ///     open_deal_ex(tid, pCmo);
    ///
    ///     MarkitAdcoTuningParam AdcoParam;
    ///     MarkitAdcoPrepayModelDials pDials;
    ///     AdcoParam.prepayModelDials = &pDials;
    ///     SetupADCOModel(tid,&AdcoParam);
    ///
    ///     int HPI_vector_size = 10;
    ///     double HPI_vector[10];
    ///     ResetADCOHpiRates(tid, HPI_vector_size, HPI_vector);
    /// @endsample
    ///
    int ADCO_PROVIDER_API ResetADCOHpiRates(void* tid, int HPI_vector_size, double* HPI_vector);

    /// Get The ADCO version.
    ///
    /// @param[in]  tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[out] version The buffer to store ADCO version.
    /// @param[in]  size The buffer size of the output pointer 'version'.
    ///
    /// @retval     0 Success.
    /// @retval other Fail.
    ///
	/// @sample
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SAS059XS");
    ///     open_deal_ex(tid, pCmo);
    ///
    ///     char version[10];
    ///     int size = 10;
    ///     GetADCOVersion(tid, version, size);
    /// @endsample
    ///
    int ADCO_PROVIDER_API GetADCOVersion(void* tid, char* version, int size);

    /// Set tuning parameters. This function should be called from LOAN_TUNNING_CB.
    ///
    /// @param[in]  tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in]  paramName   The tuning parameter name in ADCo model.
    /// @param[in]  paramValue  The tuning parameter value.
    /// @param[in]  max_size_of_error_message   The max size of user allocated array error_message.
    /// @param[out] error_message   A pointer to client-allocated array for storing error message.
    ///
    /// @retval     0 Success.
    /// @retval other Fail.
    ///
	/// @sample
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SAS059XS");
    ///     open_deal_ex(tid, pCmo);
    ///
    ///     const int ERR_SIZE = 200;
    ///     char error[ERR_SIZE] = {0};
    ///     SetTuningParam(tid, "SmmTuneAge", 5.0, error, ERR_SIZE);
    /// @endsample
    ///
    int ADCO_PROVIDER_API SetTuningParam(void* tid, const char* paramName, double paramValue, char* error_message, int max_size_of_error_message);

    /// Get the type of loan that are dealed with. This function should be called from LOAN_TUNNING_CB.Used internally for debugging; Soon to be deprecated.
    ///
    /// @param[in]  tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in]  max_size_of_loan_type   The max size of user allocated array loanType.
    /// @param[in]  max_size_of_error_message   The max size of user allocated array error_message.
    /// @param[out] loanType    A pointer to client-allocated char array.
    /// @param[out] error_message   A pointer to client-allocated char array.
    ///
    /// @retval     0 Success.
    /// @retval other Fail.
    ///
	/// @sample
    ///     void *tid = NULL;
    ///     CMO_STRUCT *pCmo = new CMO_STRUCT();
    ///     memset(pCmo, 0, sizeof(*pCmo));
    ///     strcpy(pCmo->dealid, "SAS059XS");
    ///     open_deal_ex(tid, pCmo);
    ///
    ///     char loanType[100];
    ///     int max_size_of_loan_type = 100;
    ///     char error_message[200];
    ///     int max_size_of_error_message = 200;
    ///     GetCurrentLoanType(tid, loanType, max_size_of_loan_type, error_message, max_size_of_error_message);
    /// @endsample
    ///
    int ADCO_PROVIDER_API GetCurrentLoanType(void* tid, char* loanType, int max_size_of_loan_type, char* error_message, int max_size_of_error_message);
}

#endif
