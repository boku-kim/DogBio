#ifndef _STORAGE_MGR_
#define _STORAGE_MGR_

class StorageMgr
{
public:
    StorageMgr();
    ~StorageMgr();
	CString stors;
	void Setm_stor1(CString c);
	CString Getm_stor1();
private:
	CString m_stor1;
};

#endif /* _STORAGE_MGR_ */