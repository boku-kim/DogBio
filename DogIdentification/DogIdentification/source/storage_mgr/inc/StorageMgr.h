#pragma once
#ifndef _STORAGE_MGR_
#define _STORAGE_MGR_

#include "my_system.h"

class StorageMgr
{
private:
	StorageMgr();

public:
	virtual ~StorageMgr();

public:
    static StorageMgr* GetInstance();
    static void ReleaseInstance();
	
	/*void Setm_stor1(CString c);
	CString Getm_stor1();*/

    eModError Store(eSecurityType data_type, char* pPath, unsigned char* pData, unsigned int data_size);
    eModError Load(eSecurityType data_type, char* pPath, unsigned char** ppData, unsigned int& data_size);
	eModError Compare(eSecurityType data_type, char* pPath, unsigned char* ppData, unsigned int& data_size);

private:
    static StorageMgr* m_pInst;
	/*CString m_stor1;
    CString stors;*/
};

#endif /* _STORAGE_MGR_ */