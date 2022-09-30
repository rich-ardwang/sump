///
/// @file    indextypes.h
///

#ifndef __INDEX_TYPES__
#define __INDEX_TYPES__


/// Can be used to indicate the pool types.
/// @see
///  * CCMO_POOL_INFO::type
///  * MARKIT_POOL_INFO::type
typedef enum
{
    FHLMC,                  ///< Mortgage-backed securities issued by Freddie Mac (The Federal Home Loan Mortgage Corporation).
    GOLD,                   ///< Gold Participant Certificate Securities, mortgage-backed securities issued by Freddie Mac.
    FNMA,                   ///< Mortgage-backed securities issued by Fannie Mae (The Federal National Mortgage Association).
    GNMA1,                  ///< Single-issuer pools mortgage-backed securities issued by Ginnie Mae (Government National Mortgage Association).
    GNMA2,                  ///< Multiple-issuer pools mortgage-backed securities issued by Ginnie Mae (Government National Mortgage Association).
    WHOLE,                  ///< Whole loan.
    ARM,                    ///< Adjustable-rate mortgage.
    REMIC,                  ///< A tranche of a REMIC (Real Estate Mortgage Investment Conduit). The pool is a bond (of an underlying deal).
    MIXED,                  ///< Mixed-use mortgage
    SBA,                    ///< Small business administration.
    STUD_LN,                ///< Student loan.
    AUTO,                   ///< Auto loan.
    AUTO_LS,                ///< Auto leases.
    HELOC,                  ///< Home equity line of credit.
    POOL_TYPE_OTHER,        ///< All other pool types.
    OTHER=POOL_TYPE_OTHER,  ///< @deprecated Same as ::POOL_TYPE_OTHER
} POOL_TYPE;

/// Enum of index rate type, used by set_rate_ex()
/// @see
///  * CCMO_ARM_INFO::index
///  * CCMO_POOL_INFO::arm_index
typedef enum	{	NONE, 
					LIBOR_1,              ///< Libor 1 month
					LIBOR_3,              ///< Libor 3 months
					LIBOR_6,              ///< Libor 6 months
					LIBOR_12,             ///< Libor 1 year
					LIBOR_24,             ///< Libor 2 years
					LIBOR_36,             ///< Libor 3 years
					LIBOR_48,             ///< Libor 4 years
					LIBOR_60,             ///< Libor 5 years
					LIBOR_84,             ///< Libor 7 years
					LIBOR_120,            ///< Libor 10 years
					LIBOR_240,            ///< Libor 20 years
					LIBOR_360,            ///< Libor 30 years
					TSY_1,                ///< Treasury 1 month
					TSY_3,                ///< Treasury 3 months
					TSY_6,                ///< Treasury 6 months
					TSY_12,               ///< Treasury 1 year
					TSY_24,               ///< Treasury 2 years
					TSY_36,               ///< Treasury 3 years
					TSY_48,               ///< Treasury 4 years
					TSY_60,               ///< Treasury 5 years
					TSY_84,               ///< Treasury 7 years
					TSY_120,              ///< Treasury 10 years
					TSY_240,              ///< Treasury 20 years
					TSY_360,              ///< Treasury 30 years
					COFI,                 ///< Cost of funds index
					PRIME,                ///< Prime interest rate
                    INDEX_TYPE_WAC,       ///< Weighted coupon
					WAC=INDEX_TYPE_WAC,   ///< Weighted coupon
					MAX_INDEX_TYPES} 
			INDEX_TYPE;

/// Enum of loan purpose
/// @see
///  * MARKIT_POOL_INFO::purpose
///  * CCMO_POOL_INFO_EX::purpose
typedef enum  {	POOL_PURPOSE_OTHER,                 ///< Other purpose
				POOL_PURPOSE_PURCHASE,              ///< For purchase purpose
				POOL_PURPOSE_CASHOUT_REFINANCE,     ///< For cash out purpose, including education or medical expense
				POOL_PURPOSE_HOME_IMPROVEMENT,      ///< For home improvement purpose
				POOL_PURPOSE_NEW_CONSTRUCTION,      ///< For new construction purpose
				POOL_PURPOSE_RATE_TERM_REFINANCE,   ///< For refinance purpose
				POOL_PURPOSE_NOT_AVAILABLE}         ///< Not available
	POOL_PURPOSE_TYPES;

/// Enum of loan property type
/// @see
///  * MARKIT_POOL_INFO::property_type
///  * CCMO_POOL_INFO_EX::property_type
typedef enum  {	POOL_PROPERTY_OTHER,                  ///< Other property type
				POOL_PROPERTY_SINGLE_FAMILY,          ///< Single family property
				POOL_PROPERTY_MULTI_FAMILY,           ///< Multifamily property
				POOL_PROPERTY_CONDO,                  ///< Condo property
				POOL_PROPERTY_PUD,                    ///< Planned unit development
				POOL_PROPERTY_COMMERCIAL,             ///< Commercial property
				POOL_PROPERTY_COOP,                   ///< Co-op property
				POOL_PROPERTY_MOBILE_HOME,            ///< Mobile home
				POOL_PROPERTY_MANUFACTURED_HOUSING,   ///< Manufactured housing
				POOL_PROPERTY_NOT_AVAILABLE}          ///< Not available
	POOL_PROPERTY_TYPES;

/// Enum of loan occupancy type
/// @see
///  * MARKIT_POOL_INFO::occupancy
///  * CCMO_POOL_INFO_EX::occupancy
typedef enum  { POOL_OCCUPANCY_OTHER,               ///< Other
				POOL_OCCUPANCY_OWNER,               ///< Primary 
				POOL_OCCUPANCY_SECOND,              ///< Secondary 
				POOL_OCCUPANCY_INVESTOR,            ///< Investment 
				POOL_OCCUPANCY_NOT_AVAILABLE}       ///< Not available  
	POOL_OCCUPANCY_TYPES;

/// Enum of loan delinquency states
/// @see
///  * MARKIT_POOL_INFO::delinquency
///  * CCMO_POOL_INFO_EX::delinquency
typedef enum  { 
				POOL_DELINQ_CURRENT,          ///< Delinquency current status
				POOL_DELINQ_30P,              ///< Delinquency (30, 60] days; or non-performing for bank loans.
				POOL_DELINQ_60P,              ///< Delinquency (60, 90] days.
				POOL_DELINQ_90P,              ///< Delinquency (90, 120] days.
				POOL_FORECLOSED,              ///< Foreclosed
				POOL_REO,                     ///< Real estate owned
				POOL_TERMINATED,              ///< Terminated, including paid off, repurchased, liquidated, closed etc.
				POOL_DELINQ_STATES_SIZE}      ///< Size of delinquency states
	POOL_DELINQ_STATES;

/// Enum of loan documen type
/// @see
///  * MARKIT_POOL_INFO::doc
typedef enum {	POOL_DOCUM_OTHER,             ///< Other
				POOL_DOCUM_FULL,              ///< Full document type
				POOL_DOCUM_LIMITED,           ///< Limited
				POOL_DOCUM_SISA,              ///< Stated income, stated assets type
				POOL_DOCUM_SIVA,              ///< Stated income, verified  assets type
				POOL_DOCUM_NINA,              ///< No income, no asset type
				POOL_DOCUM_NO_RATIO,          ///< No ratio type
				POOL_DOCUM_NO_DOC,            ///< No doc type
				POOL_DOCUM_ALTERNATIVE,       ///< Alternative document type 
				POOL_DOCUM_UNKNOWN =99}       ///< Unknown
	POOL_DOCUM_TYPES;

/// Enum of loan line type
/// @see
///  * MARKIT_POOL_INFO::lien_type
typedef enum {	POOL_LIEN_TYPE_OTHER,         ///< Other
				POOL_LIEN_TYPE_FIRST,         ///< First lien
				POOL_LIEN_TYPE_SECOND,        ///< Second lien
				POOL_LIEN_TYPE_THIRD}         ///< Third lien
	POOL_LIEN_TYPES;



/// Enum for the length of some pool fields.
typedef enum {	POOL_ID_LENGTH=20,               ///< Length of Pool ID
				POOL_SERVICER_NAME_LENGTH=11,    ///< Length of servicer name
				POOL_STATE_LENGTH =3,            ///< Length of state
				POOL_COUNTRY_LENGTH = 4,         ///< Length of country
				POOL_PURPOSE_LENGHT = 5,         ///< Length of purpose
				POOL_PROPERTY_TYPE_LENGTH =5,    ///< Length of property type
				POOL_OCCUPANCY_LENGTH=5,         ///< Length of occupancy
				POOL_ZIP_LENGTH=6,               ///< Length of ZIP code
                POOL_CURRENCY_LENGTH = 4}        ///< Length of currency
    POOL_FIELDS_LENGTH;



/// Enum of group delinquency states, used by set_addit_group_delinquencies()
typedef enum {	GROUP_DELINQ_30,      ///< Group delinquencies less than 30 days
				GROUP_DELINQ_60,      ///< Group delinquencies within 30 ~ 59 days
				GROUP_DELINQ_90,      ///< Group delinquencies within 60 ~ 89 days
				GROUP_DELINQ_120,     ///< Group delinquencies within 90 ~ 119 days
				GROUP_DELINQ_150,     ///< Group delinquencies within 120 ~ 149 days
				GROUP_DELINQ_180,     ///< Group delinquencies within 150 ~ 179 days
				GROUP_DELINQ_210,     ///< Group delinquencies within 180 ~ 209 days
				GROUP_DELINQ_240,     ///< Group delinquencies within 210 ~ 239 days
				GROUP_DELINQ_270,     ///< Group delinquencies within 240 ~ 269 days
				GROUP_DELINQ_300,     ///< Group delinquencies within 270 ~ 299 days
				GROUP_DELINQ_330,     ///< Group delinquencies within 300 ~ 329 days
				GROUP_DELINQ_360,     ///< Group delinquencies within 330 ~ 359 days
				GROUP_DELINQ_SIZE     ///< Size of group delinquency states
				} GROUP_DELINQ_STATES;



#endif
