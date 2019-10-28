#include"stdafx.h"
#include "SecureStorage.h"

#include "resource.h"
#include "hancom_wb.h"

SecureStorage* SecureStorage::m_pInst = NULL;

static int WriteBinary(char* pszFilePath, char* mode, BYTE* pData, unsigned int data_size)
{
    FILE * pFile = NULL;
    size_t write_size = 0;
	char szMessage[128] = { 0 };
	
    if ((pszFilePath == NULL) || (mode == NULL) || (pData == NULL) || (data_size == 0))
    {
		sprintf_s(szMessage, 128, "1111111 %s %s %s %ud.\n", pszFilePath,mode,pData,data_size);
		OutputDebugString(szMessage);
        return -1;
    }

    fopen_s(&pFile, pszFilePath, mode);

    if (pFile == NULL)
    {
		sprintf_s(szMessage, 128, "22222222.\n");
		OutputDebugString(szMessage);
        return -1;
    }
    write_size = fwrite(pData, sizeof(BYTE), data_size, pFile);
    if (write_size != data_size)
    {
		sprintf_s(szMessage, 128, "33333333333.\n");
		OutputDebugString(szMessage);
        fclose(pFile);
        return -1;
    }
    fclose(pFile);

    return (int)write_size;
}

static int ReadBinary(char* pszFilePath, char* mode, BYTE** ppData, DWORD& data_size)
{
    FILE * pFile = NULL;
    size_t read_size = 0;
    long size = 0;

    if ((pszFilePath == NULL) || (mode == NULL) || (ppData == NULL))
    {
        return -1;
    }

    fopen_s(&pFile, pszFilePath, mode);
    if (pFile == NULL)
    {
        return -1;
    }
    fseek(pFile, 0, SEEK_END);
    size = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);
    *ppData = (BYTE*)malloc(size);
    if (*ppData == NULL)
    {
        fclose(pFile);
        return -1;
    }
    read_size = fread_s(*ppData, size, sizeof(BYTE), size, pFile);
    if (read_size != size)
    {
        free(*ppData);
        fclose(pFile);
        return -1;
    }
    fclose(pFile);
    data_size = (DWORD)read_size;
    return (int)read_size;
}

SecureStorage::SecureStorage()
    : m_bLoaded(false)
{
    InitiateWb();
}


SecureStorage::~SecureStorage()
{
    DeInitiateWb();
}

SecureStorage* SecureStorage::GetInstance()
{
    if (m_pInst == NULL)
    {
        m_pInst = new SecureStorage();
    }
    return m_pInst;
}

void SecureStorage::ReleaseInstance()
{
    if (m_pInst)
    {
        delete m_pInst;
    }
}

void SecureStorage::InitiateWb()
{
    HRSRC resource;
    DWORD size;
    HGLOBAL mem;
    PVOID ptr;
    HRSRC resource2;
    DWORD size2;
    HGLOBAL mem2;
    PVOID ptr2;
    int ret = 0;

    resource = FindResource(NULL, MAKEINTRESOURCE(IDR_BIN_WB_ENCRYPT), "BIN");
    if (!resource)
    {
        return;
    }

    size = SizeofResource(NULL, resource);
    if (!size)
    {
        return;
    }

    mem = LoadResource(NULL, resource);
    if (!mem)
    {
        return;
    }

    ptr = LockResource(mem);
    if (!ptr)
    {
        return;
    }

    resource2 = FindResource(NULL, MAKEINTRESOURCE(IDR_BIN_WB_DECRYPT), "BIN");
    if (!resource2)
    {
        return;
    }

    size2 = SizeofResource(NULL, resource2);
    if (!size2)
    {
        return;
    }

    mem2 = LoadResource(NULL, resource2);
    if (!mem2)
    {
        return;
    }

    ptr2 = LockResource(mem2);
    if (!ptr2)
    {
        return;
    }

    ret = set_wb128_table_by_buff((const BYTE*)ptr, size, (const BYTE*)ptr2, size2);
    if (ret != 0)
    {
        OutputDebugString("security module was not loaded.");
        OutputDebugString("");
        AfxMessageBox("security module was not loaded.");
    
        m_bLoaded = true;
    }

    UnlockResource(mem);
    FreeResource(mem);

    UnlockResource(mem2);
    FreeResource(mem2);
}

void SecureStorage::DeInitiateWb()
{
    // when prograrm stops
    reset_wb128_table();
}

eModError SecureStorage::Store(char* pPath, unsigned char* pData, unsigned int data_size)
{
    eModError ret = eMOD_ERROR_SUCCESS;
    int func_ret = 0;
    BYTE* pEncrypt = NULL;
    DWORD encrypt_size = 0;
	CString check;

    if ((pPath == NULL) || (pData == NULL) || data_size == 0)
    {
        return eMOD_ERROR_ARGUMENT_INVALID;
    }

	

    func_ret = encrypt_wb128_cbc(pData, data_size, &pEncrypt, encrypt_size);
    if (func_ret == 0)
    {
        if (pEncrypt && encrypt_size > 0)
        {
            if (WriteBinary(pPath, "wb", pEncrypt, (unsigned int)encrypt_size) < 1)
            {
                ret = eMOD_ERROR_WRITE_FILE_FAILED;
            }
        }

        if (pEncrypt)
        {
            free(pEncrypt);
            pEncrypt = NULL;
        }
        encrypt_size = 0;
    }
    else
    {
        ret = eMOD_ERROR_ENCRYPT_FAILED;
    }

    return ret;
}

eModError SecureStorage::Load(char* pPath, unsigned char** ppData, unsigned int& data_size)
{
    eModError ret = eMOD_ERROR_SUCCESS;
    int func_ret = 0;
    BYTE* pDecrypt = NULL;
    DWORD decrypt_size = 0;
    BYTE* pBuff = NULL;
    DWORD buff_size = 0;

    if ((pPath == NULL) || (ppData == NULL))
    {
        return eMOD_ERROR_ARGUMENT_INVALID;
    }

    if (ReadBinary(pPath, "rb", &pBuff, buff_size) > 0)
    {
        func_ret = decrypt_wb128_cbc(pBuff, buff_size);
        if (func_ret == 0)
        {
            data_size = buff_size;
            *ppData = (unsigned char*)malloc(buff_size);
            if (*ppData == NULL)
            {
                data_size = 0;
                ret = eMOD_ERROR_MEMORY_ERROR;
            }
            else
            {
                memcpy(*ppData, pBuff, buff_size);
            }
        }
        else
        {
            ret = eMOD_ERROR_DECRYPT_FAILED;
        }

        if (pBuff)
        {
            free(pBuff);
            buff_size = 0;
        }
    }
    else
    {
        ret = eMOD_ERROR_READ_FILE_FAILED;
    }    

    return ret;
}
eModError SecureStorage::Compare(char* pPath, unsigned char* ppData, unsigned int& data_size) {
	eModError ret = eMOD_ERROR_SUCCESS;
	int func_ret = 0;
	BYTE* pDecrypt = NULL;
	DWORD decrypt_size = 0;
	BYTE* pBuff = NULL;
	DWORD buff_size = 0;
	unsigned char* decrypt_str=NULL; //복호화된 암호

	if ((pPath == NULL) || (ppData == NULL))
	{
		return eMOD_ERROR_ARGUMENT_INVALID;
	}

	if (ReadBinary(pPath, "rb", &pBuff, buff_size) > 0)
	{
		func_ret = decrypt_wb128_cbc(pBuff, buff_size);
		if (func_ret == 0)
		{
			decrypt_str = (unsigned char*)malloc(buff_size);
			if (*ppData == NULL)
			{
				data_size = 0;
				ret = eMOD_ERROR_MEMORY_ERROR;
			}
			else
			{
				memcpy(decrypt_str, pBuff, buff_size); 
				*(decrypt_str + buff_size) = '\0';

				/*복호화된 암호와 입력된 암호 비교*/
				if (strcmp((char*)decrypt_str, (char*)ppData) == 0) {
				}
				else {
					ret = eMOD_ERROR_ARGUMENT_INVALID;
				}
			}
		}
		else
		{
			ret = eMOD_ERROR_DECRYPT_FAILED;
		}

		if (pBuff)
		{
			free(pBuff);
			buff_size = 0;
		}
	}
	else
	{
		ret = eMOD_ERROR_READ_FILE_FAILED;
	}
	return ret;
}