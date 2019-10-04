#include "stdafx.h"
#include "Util.h"



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
