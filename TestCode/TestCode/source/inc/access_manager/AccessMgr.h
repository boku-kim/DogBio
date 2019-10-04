#ifndef _ACCESS_MGR_H_
#define _ACCESS_MGR_H_

#include "my_system.h"
#include "AccessMgr.h"

class AccessMgr
{
private:
    AccessMgr();
    ~AccessMgr();

public:
    static AccessMgr* GetInstance();
    static void ReleaseInstance();

    eModError RegisterPassword(char* pszPassword, eSecurityType data_type);

private:
    static AccessMgr* m_Inst;
};

#endif /* _ACCESS_MGR_H_ */