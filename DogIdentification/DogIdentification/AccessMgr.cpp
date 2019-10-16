#include "stdafx.h"
#include "AccessMgr.h"

AccessMgr* AccessMgr::m_Inst = NULL;

AccessMgr::AccessMgr()
{
}


AccessMgr::~AccessMgr()
{
}

AccessMgr* AccessMgr::GetInstance()
{
    if (m_Inst == NULL)
    {
        m_Inst = new AccessMgr();
    }
    return m_Inst;
}

void AccessMgr::ReleaseInstance()
{
    if (m_Inst)
    {
        delete m_Inst;
    }
}

eModError AccessMgr::RegisterPassword(char* pszPassword, eSecurityType data_type)
{
    if (pszPassword == NULL)
    {
        return eMOD_ERROR_ARGUMENT_INVALID;
    }
    if (data_type < eSECURITY_TYPE_IMPORTANT ||
        data_type >= eSECURITY_TYPE_MAX)
    {
        return eMOD_ERROR_ARGUMENT_INVALID;
    }



    return eMOD_ERROR_SUCCESS;
}