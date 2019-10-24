#pragma once

#include "my_system.h"

class SecureStorage
{
public:
    virtual ~SecureStorage();

private:
    SecureStorage();


public:
    static SecureStorage* GetInstance();
    static void ReleaseInstance();

    eModError Store(char* pPath, unsigned char* pData, unsigned int data_size);
    eModError Load(char* pPath, unsigned char** ppData, unsigned int& data_size);
	eModError Compare(char* pPath, unsigned char* ppData, unsigned int& data_size);

private:
    void InitiateWb();
    void DeInitiateWb();

    

private:
    static SecureStorage* m_pInst;
    bool m_bLoaded;
};

