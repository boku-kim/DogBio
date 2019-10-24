#include "stdafx.h"
#include "Util.h"

void My_LogPrint(const char* function, int line, const char* szFormat, ...)
{
#ifdef _DEBUG
    char szBuff[1024] = { 0 };
    char szBuff2[1024] = { 0 };
    char szBuffFinal[1024] = { 0 };
    int len = 0;
    int rest_size = 0;
    int copy_size = 0;
    int write_pos = 0;
    int debug_out = MY_DEBUG_OUTPUT_POS;

    va_list arg;
    va_start(arg, szFormat);
    _vsnprintf_s(szBuff, sizeof(szBuff), szFormat, arg);
    va_end(arg);

    sprintf_s(szBuff2, sizeof(szBuff2), "%s(%d)", function, line);
    len = (int)strlen(szBuff2);
    memset(szBuffFinal, 0x00, sizeof(szBuffFinal));
    memcpy(szBuffFinal, szBuff2, len);

    if (len > debug_out)
    {
        write_pos = len;
    }
    for (int idx = len; idx < debug_out; ++idx)
    {
        szBuffFinal[idx] = 0x20;
        write_pos = debug_out;
    }

    // calculate rest buffer
    rest_size = sizeof(szBuffFinal) - write_pos;
    if (rest_size >= (strlen(MY_LOG_DELIMETER) + 3))
    {
        copy_size = strlen(MY_LOG_DELIMETER);
    }
    else
    {
        copy_size = rest_size - 2;
    }
    memcpy(szBuffFinal + write_pos, MY_LOG_DELIMETER, copy_size);

    write_pos += copy_size;
    rest_size -= copy_size;
    if (rest_size >= (strlen(szBuff) + 3))
    {
        copy_size = strlen(szBuff);
    }
    else
    {
        copy_size = rest_size - 2;
    }
    memcpy(szBuffFinal + write_pos, szBuff, copy_size);
    rest_size -= copy_size;
    write_pos += copy_size;
    if (rest_size >= 3)
    {
        szBuffFinal[write_pos] = 0x0d;
        szBuffFinal[write_pos + 1] = 0x0a;
        szBuffFinal[write_pos + 2] = 0x00;
    }
    else
    {
        szBuffFinal[sizeof(szBuffFinal) - 3] = 0x0d;
        szBuffFinal[sizeof(szBuffFinal) - 2] = 0x0a;
        szBuffFinal[sizeof(szBuffFinal) - 1] = 0x00;
    }

    OutputDebugString(szBuffFinal);
    printf("%s", szBuffFinal);
#endif /* _DEBUG */
}

Util* Util::m_Inst = NULL;

Util::Util()
{
}


Util::~Util()    
{
}

Util* Util::GetInstance()
{
    if (m_Inst == NULL)
    {
        m_Inst = new Util;
    }
    return m_Inst;
}

void Util::ReleaseInstance()
{
    if (m_Inst)
    {
        delete m_Inst;
    }
}

bool Util::ExistFile(char* pszFilePath)
{
    FILE* pFile = NULL;

    fopen_s(&pFile, pszFilePath, "r");
    if (pFile)
    {
        fclose(pFile);
        return true;
    }
    else
    {
        return false;
    }
}
