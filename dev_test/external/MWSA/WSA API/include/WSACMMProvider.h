#ifndef MARKITCMMPROVIDERAPI_H
#define MARKITCMMPROVIDERAPI_H

#ifndef __MWSA_DLL__
#include "ccmo.h"
#else
#include "wsa.h"
#include "wsa_api.h"
#endif

#ifdef _WIN32
#ifdef MARKITCMMPROVIDER_EXPORTS
#define CMM_PROVIDER_API __declspec(dllexport)
#else
#define CMM_PROVIDER_API __declspec(dllimport)
#endif
#else
#define CMM_PROVIDER_API
#endif



extern "C"
{

    /// Install CMM custom model to WSA API.
    ///
    /// @param[in] tid The deal/scenario object identifier. Null if using non-thread-safe calls.
    /// @param[in] cmm_user       CMM model user name.
    /// @param[in] cmm_password   CMM model password.
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
    ///     SetupCMMCustomModel(tid, "user", "password");
    ///     run_deal_ex(tid, pCmo);
    ///     RemoveCMMCustomModel(tid);
    /// @endsample
    ///
    int CMM_PROVIDER_API SetupCMMCustomModel(void* tid, const char* cmm_user, const char* cmm_password);

    /// Uninstall CMM custom model from WSA API.
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
    ///     SetupCMMCustomModel(tid, "user", "password");
    ///     run_deal_ex(tid, pCmo);
    ///     RemoveCMMCustomModel(tid);
    /// @endsample
    ///
    int CMM_PROVIDER_API RemoveCMMCustomModel(void* tid);
   
}

#endif
