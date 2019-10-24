
#include "stdafx.h"
#include "StorageMgr.h"
//#include "UserPassword.h"
#include "SecureStorage.h"

StorageMgr* StorageMgr::m_pInst = NULL;

StorageMgr::StorageMgr()
{

}

StorageMgr::~StorageMgr()
{
}

//CString StorageMgr::Getm_stor1() {
//	return m_stor1;
//}
//
//void StorageMgr::Setm_stor1(CString str) {
//	m_stor1 = str;
//}

StorageMgr* StorageMgr::GetInstance()
{
    if (m_pInst == NULL)
    {
        m_pInst = new StorageMgr();
    }
    return m_pInst;
}

void StorageMgr::ReleaseInstance()
{
    if (m_pInst)
    {
        delete m_pInst;
    }
}


eModError StorageMgr::Store(eSecurityType data_type, char* pPath, unsigned char* pData, unsigned int data_size)
{
    eModError ret = eMOD_ERROR_SUCCESS;
    SecureStorage* pSecureInst = NULL;

    if ((data_type < eSECURITY_TYPE_IMPORTANT) || (data_type >= eSECURITY_TYPE_MAX))
    {
        return eMOD_ERROR_ARGUMENT_INVALID;
    }

    if ((pPath == NULL) || (pData == NULL) || (data_size == 0))
    {
        return eMOD_ERROR_ARGUMENT_INVALID;
    }

    if (data_type == eSECURITY_TYPE_IMPORTANT)
    {
        ///< save to secure storage
        pSecureInst = SecureStorage::GetInstance();
        if (pSecureInst)
        {
            ret = pSecureInst->Store(pPath, pData, data_size);
        }
        else
        {
            ret = eMOD_ERROR_INSTANCE_NULL;
        }
    }
    else
    {
        ///< TODO : save to db
    }
    
    return ret;
}

eModError StorageMgr::Load(eSecurityType data_type, char* pPath, unsigned char** ppData, unsigned int& data_size)
{
    eModError ret = eMOD_ERROR_SUCCESS;
    SecureStorage* pSecureInst = NULL;

    if (data_type == eSECURITY_TYPE_IMPORTANT)
    {
        ///< save to secure storage
        pSecureInst = SecureStorage::GetInstance();
        if (pSecureInst)
        {
			ret=pSecureInst->Load(pPath, ppData, data_size);
        }
        else
        {
            ret = eMOD_ERROR_INSTANCE_NULL;
        }
    }
    else
    {
        ///< save to db
    }

    return ret;
}
eModError StorageMgr::Compare(eSecurityType data_type, char* pPath, unsigned char* ppData, unsigned int& data_size) {
	eModError ret = eMOD_ERROR_SUCCESS;
	SecureStorage* pSecureInst = NULL;

	if (data_type == eSECURITY_TYPE_IMPORTANT)
	{
		///< compare to secure storage
		pSecureInst = SecureStorage::GetInstance();
		if (pSecureInst)
		{
			ret = pSecureInst->Compare(pPath, ppData, data_size);
		}
		else
		{
			ret = eMOD_ERROR_INSTANCE_NULL;
		}
	}
	else
	{
	}

	return ret;
}