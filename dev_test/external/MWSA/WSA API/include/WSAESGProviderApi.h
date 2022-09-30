#ifndef WSAESGProvider_API_H
#define WSAESGProvider_API_H

#include "wsa_api.h"

#ifdef _WIN32
#ifdef WSAESGPROVIDER_EXPORTS
#define ESG_PROVIDER_API __declspec(dllexport)
#else
#define ESG_PROVIDER_API __declspec(dllimport)
#endif
#else
#define ESG_PROVIDER_API
#endif


extern "C"
{
    /// Initialize ESG model files.
    ///
    /// @param[in] LicenseFile  ESG license file full path and file name.
    /// @param[in] BhModelPath  ESG BHM model full path.
    /// @param[in] CalibrationFile  ESG calibration file full path and file name.
    /// @param[in, out] ErrorMsg   User allocated to store the error message when the funtion called failed.
    /// @param[in] ErrorMsgSize  The size of the error message. 
    ///
    /// @retval 0 Success.
    /// @retval < 0 other Fail.
    ///
    int ESG_PROVIDER_API InitializeESGModelFile(const char* LicenseFile, const char* BhModelPath, const char* CalibrationFile, const char* DataPath, char* ErrorMsg, int ErrorMsgSize);

    /// Generate ESG model simulation path rates.
    ///
    /// @param[in] Currency  Standard currency code, refer to ISO 4217, currently just support the "USD".
    /// @param[in] YYYYMMDD  Simulation date, the format would be yyyymmdd.
    /// @param[in] PathNum   Number of simulation paths.
    /// @param[in] Periods   Number of periods Simulation.
    /// @param[in] RatesOutputFile  The full path for ESG model simulation path rates outputs. 
    /// @param[in] RatesInputArray  The array of ESG rates input.
    /// @param[in] RatesInputArraySize   The number of elements of RatesInputArray.
	//	@param[in] RatesExtraArray  The array of ESG rates extra info,Ex:UpdateYieldCurve inputs.
    /// @param[in, out] ErrorMsg   User allocated to store the error message when the funtion called failed.
    /// @param[in] ErrorMsgSize  The size of the error message. 

    ///
    /// @retval 0 Success.
    /// @retval < 0 other Fail.
    ///
    int ESG_PROVIDER_API GenerateESGSimulationPathRates(int YYYYMMDD, int PathNum, int Periods, const char* RatesOutputPath, ESG_CURRENCY_RATE_INPUTS* RatesInputArray, int RatesInputArraySize, ESG_EXTRA_INPUTS* RatesExtraArray, char* ErrorMsg, int ErrorMsgSize, ESG_RATE_TYPE RateType);
}

#endif