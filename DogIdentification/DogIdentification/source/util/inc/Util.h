#pragma once

#include "my_system.h"

#ifdef __cplusplus 
extern "C" {
#endif /* !__cplusplus */

#define MY_DEBUG_OUTPUT_POS 40
#define MY_LOG_DELIMETER ">>>>> "

#define MY_CHECK_POINT(fmt, ...) My_LogPrint(__FUNCTION__, __LINE__, fmt, __VA_ARGS__)

#define MY_IN            MY_CHECK_POINT("[%s][IN]", __FUNCTION__); 

#define MY_OUT           MY_CHECK_POINT("[%s][OUT]", __FUNCTION__);
#define MY_ERROR         MY_CHECK_POINT("make error code and output error log.");
#define MY_DEBUG_LOG     MY_CHECK_POINT("print debug error message but going on.");

void My_LogPrint(const char* function, int line, const char* szFormat, ...);

#ifdef __cplusplus 
}
#endif /* !__cplusplus */

class Util
{
private:
    Util();
    ~Util();

public:
    static Util* GetInstance();
    static void ReleaseInstance();

    ///< method
    bool ExistFile(char* pszFilePath);

private:
    static Util* m_Inst;
};

