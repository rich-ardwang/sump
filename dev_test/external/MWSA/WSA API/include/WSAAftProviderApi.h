///
/// @file WSAAftProviderApi.h
///

#ifndef MARKITAFTPROVIDERAPI_H
#define MARKITAFTPROVIDERAPI_H


#ifdef _WIN32
#ifdef MAKITAFTPROVIDER_EXPORTS
#define AFT_PROVIDER_API __declspec(dllexport)
#else
#define AFT_PROVIDER_API __declspec(dllimport)
#endif
#else
#define AFT_PROVIDER_API
#endif


///
/// @brief The fine tune parameters for the AFT default model.
/// This supplements structure MarkitAftTuningParam, and is optional.
///
#ifndef __cplusplus
typedef
#endif
struct
#ifdef  __cplusplus
    AFT_PROVIDER_API
    MarkitAftDefaultModelDials
#endif
{
    double dTransitionToPop00Multiplier;    ///< Multiplier for transition to delinq 0
    double dTransitionToPop30Multiplier;    ///< Multiplier for transition to delinq 30
    double dTransitionToPop60Multiplier;    ///< Multiplier for transition to delinq 60
    double dTransitionToPop90Multiplier;    ///< Multiplier for transition to delinq 90
    double dTransitionToPopFcMultiplier;    ///< Multiplier for transition to delinq Foreclosure
    double dTransitionToPopDfMultiplier;    ///< Multiplier for transition to delinq Default
    double dPrepayFromCurrMultiplier;       ///< Multiplier for Prepayment from current
    double dPrepayFromPop00Multiplier;      ///< Multiplier for Prepayment from delinq 0
    double dPrepayFromPop30Multiplier;      ///< Multiplier for Prepayment from delinq 30
    double dPrepayFromPop60Multiplier;      ///< Multiplier for Prepayment from delinq 60
    double dPrepayFromPop90Multiplier;      ///< Multiplier for Prepayment from delinq 90
    double dPrepayFromPopFcMultiplier;      ///< Multiplier for Prepayment from delinq Foreclosure
    double dDefaultAgeMultiplier;           ///< Multiplier for Default Age
    double dPaymentIncreaseMultiplier;      ///< Multiplier for payment increase
    double dAdjustedCurrLTVMultiplier;      ///< Multiplier for Adjusted Current LTV
    int nApplyDialsToBothFixedOrArmFlag;    ///< This flag takes the following values
                                            ///<   - -1 If dials are for mapped model type (from defdials.def)
                                            ///<   - 0 Apply these dials to both portions of a hybrid
                                            ///<   - 1 Apply these dials to only the fixed portion of a hybrid
                                            ///<   - 2 Apply these dials to only the arm portion of a hybrid
                                            ///<
    double dDefaultRateMultiplier;          ///< Multiplier for Default Rate
    double dLossSeverityMultiplier;         ///< Multiplier for Loss Severity

#ifdef  __cplusplus
    MarkitAftDefaultModelDials();
#endif
}
#ifndef __cplusplus
MarkitAftDefaultModelDials
#endif
;

///
/// @brief The fine tune parameters for the AFT prepay model.
/// This supplements structure MarkitAftTuningParam, and is optional.
///
#ifndef __cplusplus
typedef
#endif
struct
#ifdef  __cplusplus
    AFT_PROVIDER_API
    MarkitAftPrepayModelDials
#endif
{
    double  *projGrossWACPercent;                           ///< The AFT prepayment model allows users to pass an array of projected variables WACs (as percents) to be used with fixed rate mortgages. This feature is optional; set this item to zero to disable the feature.
    double  rfMultiplier;                                   ///< A factor to be applied to the refinancing component of the prepay speed for each monthly projection.
    double  htMultiplier;                                   ///< A factor to be applied to the housing turnover component of the prepay speed for each monthly projection.
    double  ageMultiplier;                                  ///< A factor used to extend or shorten the aging effect. Values greater than one extend the aging function.  Values less than one shorten the function.
    double  burnMultiplier1;                                ///< A factor used to increase or diminish the effects of pool burnout. Values greater than one increase the effect; values less than one reduce the effect.
    double  burnMultiplier2;                                ///< Additional factor used to increase or diminish the effects of pool burnout. Values greater than one increase the effect; values less than one reduce the effect.
    double  premiumOriginationAdjFactor;                    ///< A factor used to increase or diminish the impact of premium origination on the refinancing component of the prepayment model.  The default value is one; setting this member to zero turns off the premium origination effect completely.  May take any value from zero to (and including) one.
    int     ageShiftInHousingTurnoverAgingFunction;         ///< An additive factor used to adjust the span of the housing aging function. The default is zero; units are months.For example, a value of five results in passing "current age + 5 months" to the housing turnover module.
    int     useMultiplicativeOrAdditivePopShiftFlag;        ///< Specifies the type of population shift input to be used. The population shift methods - additive and multiplicative - are mutually exclusive.  The default is multiplicative. Select additive by setting this member to one.
    int     applyPopShiftFromWhatDateFlag;                  ///< Designates when population shifts will begin being applied; applies regardless of the population shift method specified (additive | multiplicative).
                                                            ///<       - Set to one to specify that shifts should be applied beginning at the mortgage origination.
                                                            ///<       - Set to two to specify that shifts should be applied beginning at the settlement date.
                                                            ///<       - For any other value the prepay model applies the shifts beginning from the first date of the mortgage rate projections.  (See mrateDateYyyyMm.)
                                                            ///<
    double  additivePop1ToPop2Shift;                        ///< Specifies the magnitude of the additive shift from Population 1 to Population 2.
    double  additivePop2ToPop3Shift;                        ///< Specifies the magnitude of the additive shift from Population 2 to Population 3. This member functions the same as the prior item except that the shift occurs between populations two and three.  (See additivePop1ToPop2Shift above.)
    double  curPop1ToPop2Shift;                             ///< Specifies the percentage shift from Population 1 to Population 2. Expressed as a value greater than one.  For instance, a value of 1.1 specifies a 10% shift from population #1 to population #2.
    double  curPop2ToPop3Shift;                             ///< Specifies the percentage shift from Population 2 to Population 3. This member functions the same as the prior item except that the shift occurs between populations two and three  (See curPop1ToPop2Shift above.)
    int     calcHousingSalesStartingFromMRatedateFlag;      ///< Designates to the prepay model when to begin calculating housing sales. Typically, the prepay model will use both historical housing sales figures and consensus economic projections provided by AFT. Set this element to zero to implement that. Set the member value to a non-zero value to direct the model to start calculating housing sales numbers from mrateDateYyyyMm a member of the EspPrepayProjStruct structure. In doing so, historical data and consensus projections are ignored.
    int     applyPremiumOriginationAdjFactorOnHousingTurnoverFlag;      ///< Designates to the prepay model whether to apply premium origination factor to housing turnover. Set to one to apply the factor; set to zero to disable the feature.
    int     turnOffShortTermMultiplicativeAdjustments;                  ///< Determines whether short term adjustment processing is applied.  Default value is zero which enables sort term adjustment processing. Set to one to disable the application of short term adjustments.
    int     applyPrepaymentMultipliersToFixedPeriodOnlyForHybridARM;    ///< Designates how prepay multipliers will be applied to Hybrid ARMs.  By default, in the case of Hybrid ARMs prepay multipliers are applied to both the fixed and adjustable rate periods.  Set this member to one to direct the prepay model to apply the prepay multipliers in this structure to the fixed period only.
    double  elbowShiftForRefiMortgageRateInPercent;                     ///< Elbow shift for refinancing mortgage rate (in percent). Default value is zero, i.e. no shift.
    double  publicityMultiplierChange;                                  ///< Additive adjustment to the publicity multiplier.  For example, for a publicity multiplier of N, applying this member value (d) is equal to N + d.
    double  refiLagInMonth;                                             ///< Lag shift in months for the refi component of the prepay model.  The value is applied to the calculation of the effective mortgage rate for determination of the refinancing incentive.  You may enter positive or negatives values including fractions.  The default value is zero.
    double  changeOfCreditRelatedPrepaymentMultiplier;                  ///< Factor used to increase or decrease the credit-related prepayment multiplier. The credit related prepayment is modified by 1 + (value of this member).
#ifdef  __cplusplus
    MarkitAftPrepayModelDials();
#endif
}
#ifndef __cplusplus
MarkitAftPrepayModelDials
#endif
;


///
/// @brief The main tuning parameters for the AFT default model.
///
#ifndef __cplusplus
typedef
#endif
struct
#ifdef  __cplusplus
    AFT_PROVIDER_API
    MarkitAftTuningParam
#endif
{   /// Index to apply spread to.
    typedef enum { APPLY_SPREAD_TO_LIBOR, APPLY_SPREAD_TO_TRS } APPLY_SPREAD_TO;
    /// Enum for the the actions when encounter error.
    typedef enum { ON_ERROR_STOP, ON_ERROR_CONTINUE } ERROR_HANDLING;
    /// Enum for the model types.
    typedef enum { MODEL_PREPAY_ONLY, MODEL_DEFAULT_AND_PREPAY} MODEL_TYPES;
    enum    { DATA_PATH_SIZE = 1024 };
    char    AftDataPath[DATA_PATH_SIZE];    ///< Path to AFT's data directory.
    MODEL_TYPES modelType;                  ///< Either AFT prepay model only, or with default.
    double  prepay_magnitude;               ///< multiplier to apply to SMMs returned by the model.
    double  default_magnitude;              ///< Magnitude for default rate.
    int     recovery_lag;                   ///< Recovery lag applied to each loan.
    int     servicer_advancing;             ///< Servicer advancing.
    double  spread_30Y_Mtg_to_30Y_Index;    ///< Spread from 30 years mortgage to 30Y index.
    double  spread_15Y_Mtg_to_10Y_Index;    ///< Spread from 15 years mortgage to 10Y index.
    double  spread_7Y_Mtg_to_7Y_Index;      ///< Spread from 7 years mortgage to 7Y index.
    double  spread_5Y_Mtg_to_5Y_Index;      ///< Spread from 5 years mortgage to 5Y index.
    APPLY_SPREAD_TO applySpreadTo;          ///< Which index should apply the spread to.
    ERROR_HANDLING errorHandling;           ///< User can choose to continue or stop at errors.
    double  smmForFailedLoans;              ///< SMM in percentage.
    double  mdrForFailedLoans;              ///< MDR in percentage.
    double  recoveryForFailedLoans;         ///< Recovery in percentage.
    double  ltvToUseIfNotAvailable;         ///< Default LTV, in percentage.
    int     ficoToUseIfNotAvailable ;       ///< Fico to use for loans which do not have this INFO.
    int     zipCodeToUseIfNotAvailable;     ///< Default MSA code if not available from input data.
    int     userProvidedPerformanceInfo;    ///< Set this flag to 1 if you replaced collateral, and new collareral has loan performance info.
    int     HPI_vector_size;                ///< The number of elements of HPI_vector.
    double* HPI_vector;                     ///< Vector for HPI.
    int     ficoSubprimeThreashold;         ///< Loan is considered subprime if FICO is less than this value ( 620 if not provided).

    void*   userData;                       ///< When this pointer is set, CB functions will be called with userData set. Set to NULL if you do not need it.
    /// The signature of the call back function after process each loan.
    typedef int (*PROGRESS_CB)(void* userData,int pools_processed, int pools_succeeded,
                                        int pools_in_deal,
                                        char* error_message,
                                        int max_size_of_error_message);
    PROGRESS_CB progressCb;                 ///< Called after each loan is processed.

    MarkitAftDefaultModelDials* defaultModelDials;  ///< Fine tune params, set to 0 if not needed
    int     checkResultsFor_NAN_Values;             ///< Set to non-0 to verify the NAN values for each period, or 0 to disable the check. The default value is 0.
    MarkitAftPrepayModelDials*  prepayModelDials;   ///< Fine tune params, set to 0 if not needed

    /// The signature of the call back function before process each loan.
    typedef void (*LOAN_TUNNING_CB)(
        void* pool_info,    ///< Cast to  PAY_POOL_INFO*.
        void* tid,          ///< Use for calling SetPrepayModelDials or SetDefaultModelDials.
        void* userData      ///< UserData member of MarkitAftTuningParam structure.
        );
    LOAN_TUNNING_CB loanTunningCb;  ///< Called before each loan is processed.

#ifdef  __cplusplus
    MarkitAftTuningParam();
#endif
}
#ifndef __cplusplus
MarkitAftTuningParam
#endif
;


///
/// @brief The Scenario parameters users can change from run to run, see ResetAFTScenario().
///
#ifndef __cplusplus
typedef
#endif
struct
#ifdef  __cplusplus
    AFT_PROVIDER_API
    MarkitAftScenarioParams
#endif
{
    int     HPI_vector_size;    ///< The number of elements of HPI_vector.
    double* HPI_vector;         ///< Vector for HPI.
    int     reserved;           ///< Reserved for future use.

#ifdef  __cplusplus
    MarkitAftScenarioParams();
#endif
}
#ifndef __cplusplus
MarkitAftScenarioParams
#endif
;


extern "C"
{
    /// Install AFT model to WSA API.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] tuning A pointer to a client-allocated MarkitAftTuningParam structure.
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
    ///     MarkitAftTuningParam aftParam;
    ///     MarkitAftPrepayModelDials pDials;
    ///     aftParam.prepayModelDials = &pDials;
    ///     SetupAFTModel(tid, &aftParam);
    ///     run_deal_ex(tid, pCmo);
    ///       
    /// @endsample
    ///
    int AFT_PROVIDER_API SetupAFTModel(void* tid, MarkitAftTuningParam* tuning);

    /// Uninstall AFT model from WSA API.
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
    ///     MarkitAftTuningParam AftParam;
    ///     MarkitAftPrepayModelDials pDials;
    ///     AftParam.prepayModelDials = &pDials;
    ///     SetupAFTModel(tid,&AftParam);
    ///     run_deal_ex(tid, pCmo);
    ///
    ///     RemoveAFTModel(tid);
    /// @endsample
    ///
    int AFT_PROVIDER_API RemoveAFTModel(void* tid);

    /// Reset all interest rates and set the parameters in AFT model.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] scenParam A pointer to a client-allocated MarkitAftScenarioParams structure.
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
    ///     MarkitAftTuningParam AftParam;
    ///     MarkitAftPrepayModelDials pDials;
    ///     AftParam.prepayModelDials = &pDials;
    ///     SetupAFTModel(tid,&AftParam);
    ///
    ///     MarkitAftScenarioParams params;
    ///     ResetAFTScenario(tid,&params);
    /// @endsample
    ///
    int AFT_PROVIDER_API ResetAFTScenario(void* tid,MarkitAftScenarioParams* scenParam);

    /// Reset all interest rates in AFT model.
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
    ///     MarkitAftTuningParam AftParam;
    ///     MarkitAftPrepayModelDials pDials;
    ///     AftParam.prepayModelDials = &pDials;
    ///     SetupAFTModel(tid,&AftParam);
    ///
    ///     ResetAFTInterestRates(tid);
    /// @endsample
    ///
    int AFT_PROVIDER_API ResetAFTInterestRates(void* tid);

    /// Reset HPI rates in AFT model.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] HPI_vector_size The length of user allocated array HPI_vector.
    /// @param[in] HPI_vector A pointer to client-allocated HPI_vector array.
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
    ///     MarkitAftTuningParam AftParam;
    ///     MarkitAftPrepayModelDials pDials;
    ///     AftParam.prepayModelDials = &pDials;
    ///     SetupAFTModel(tid,&AftParam);
    ///
    ///     int HPI_vector_size = 10;
    ///     double HPI_vector[10];
    ///     ResetAFTHpiRates(tid, HPI_vector_size, HPI_vector);
    /// @endsample
    ///
    int AFT_PROVIDER_API ResetAFTHpiRates(void* tid, int HPI_vector_size, double* HPI_vector);

    /// Set prepay model dials in AFT.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] prepayModelDials The pointer of the MarkitAftPrepayModelDials object.
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
    ///     MarkitAftPrepayModelDials aftPrepDials;
    ///     SetPrepayModelDials(tid,&aftPrepDials);
    /// @endsample
    ///
    int AFT_PROVIDER_API SetPrepayModelDials(void* tid, MarkitAftPrepayModelDials* prepayModelDials);


    /// Set default model dials in AFT.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] defaultModelDials The pointer of the MarkitAftDefaultModelDials object.
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
    ///     MarkitAftDefaultModelDials	defaultModelDials;
    ///     SetDefaultModelDials(tid, &defaultModelDials);
    /// @endsample
    ///
    int AFT_PROVIDER_API SetDefaultModelDials(void* tid, MarkitAftDefaultModelDials* defaultModelDials);
}

#endif
