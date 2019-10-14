
#include "stdafx.h"
#include "StorageMgr.h"
#include "UserPassword.h"


StorageMgr::StorageMgr()
{

}
StorageMgr::~StorageMgr()
{
}
CString StorageMgr::Getm_stor1() {
	return m_stor1;
}
void StorageMgr::Setm_stor1(CString str) {
	m_stor1 = str;
}