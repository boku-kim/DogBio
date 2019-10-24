/**
****************************************************************************************************
* @file utils.cpp
* @brief Utility functions (log, error, etc...)
* @author 
* @date Created Oct 01, 2019
* @date Updated
* @see Coding standard guideline
****************************************************************************************************
*/
#if defined(_OS_WINDOWS) && defined(WIN32)
#include "stdafx.h"
#elif defined(_OS_LINUX)
#else
#error Unknown OS - not supported
#endif /* _OS_WINDOWS || _OS_LINUX */

#include "Utils.h"

DWORD gDecideLogPath = 0;
char gszLogPath[MAX_PATH + 1] = { 0 };

typedef std::map<DWORD, pMemEntry_t>MemStoreMap;

MemStoreMap gMemStoreMap;

/**
****************************************************************************************************
* @brief : print the log to console.
****************************************************************************************************
*/
void DogTrack_LogPrint(const char* function, int line, const char* szFormat, ...)
{
#ifdef _DEBUG
    char szBuff[1024] = { 0 };
    char szBuff2[1024] = { 0 };
    char szBuffFinal[1024] = { 0 };
    int len = 0;
    int rest_size = 0;
    int copy_size = 0;
    int write_pos = 0;
    int debug_out = DT_DEBUG_OUTPUT_POS;

    va_list arg;
    va_start(arg, szFormat);
    _vsnprintf_s(szBuff, sizeof(szBuff), szFormat, arg);
    va_end(arg);

    sprintf_s(szBuff2, sizeof(szBuff2), "%s(%d)", function, line);
    len = (int)strlen(szBuff2);
    memset(szBuffFinal, 0x00, sizeof(szBuffFinal));
    DT_MEMCPY(szBuffFinal, szBuff2, len);
    
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
    if (rest_size >= (int)(strlen(DT_LOG_DELIMETER) + 3))
    {
        copy_size = strlen(DT_LOG_DELIMETER);
    }
    else
    {
        copy_size = rest_size - 2;
    }
    DT_MEMCPY(szBuffFinal + write_pos, DT_LOG_DELIMETER, copy_size);

    write_pos += copy_size;
    rest_size -= copy_size;
    if (rest_size >= (int)(strlen(szBuff) + 3))
    {
        copy_size = strlen(szBuff);
    }
    else
    {
        copy_size = rest_size - 2;
    }
    DT_MEMCPY(szBuffFinal + write_pos, szBuff, copy_size);
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
#if defined(_OS_WINDOWS)
	OutputDebugString(szBuffFinal);
    printf("%s", szBuffFinal);
#elif defined(_OS_LINUX)
	printf("%s", szBuffFinal);
#endif /* _OS_WINDWS _OS_LINUX */
#endif /* _DEBUG */
}

/**
****************************************************************************************************
* @brief : print the log to file.
****************************************************************************************************
*/
void DogTrack_LogPrint2(const char* szFormat, ...)
{
    /*
    int len = 0;
    char szBuff[1024] = { 0 };

    va_list arg;
    va_start(arg, szFormat);
    _vsnprintf_s(szBuff, sizeof(szBuff), szFormat, arg);
    va_end(arg);
    len = strlen(szBuff);
    if (len <= (int)(sizeof(szBuff) - 2))
    {
        szBuff[len] = 0x0d;
        szBuff[len + 1] = 0x0a;
        szBuff[len + 2] = 0x00;
    }
    else
    {
        szBuff[sizeof(szBuff) - 3] = 0x0d;
        szBuff[sizeof(szBuff) - 2] = 0x0a;
        szBuff[sizeof(szBuff) - 1] = 0x00;
    }
    
#ifdef _DEBUG
#if defined(_OS_WINDWS)
	OutputDebugString(szBuff);
#elif defined(_OS_LINUX)
	printf("%s", szBuff);
#endif // _OS_WINDWS _OS_LINUX 
#endif // _DEBUG
    // Decide the log directory
    boost::filesystem::path current = boost::filesystem::current_path();
    boost::filesystem::path log_file_path = current / DT_LOG_DIR_NAME;
    if (!gDecideLogPath)
    {
        if (boost::filesystem::exists(log_file_path) == false)
        {
            if (boost::filesystem::create_directories(log_file_path) == false)
            {    
                sprintf_s(gszLogPath, sizeof(gszLogPath), "%s", current.generic_string().c_str());
            }
            else
            {
                sprintf_s(gszLogPath, sizeof(gszLogPath), "%s", log_file_path.generic_string().c_str());

            }
        }
        else
        {
            sprintf_s(gszLogPath, sizeof(gszLogPath), "%s", log_file_path.generic_string().c_str());
        }
        gDecideLogPath = 1;
    }

    const std::locale fmt(std::locale::classic(), new boost::gregorian::date_facet("%Y%m%d")); // ISO format

    FILE* pFile = NULL;
    size_t write_size = 0;
    size_t need_size = 0;
    char szFileName[MAX_PATH + 1] = { 0 };
    boost::posix_time::ptime current_local_time = boost::posix_time::second_clock::local_time();
    boost::gregorian::date current_date = current_local_time.date();
    std::ostringstream os;
    os.imbue(fmt);
    os << current_date;
    sprintf_s(szFileName, sizeof(szFileName), "%s%s", os.str().c_str(), DT_LOG_FILE_EXT);
    boost::filesystem::path log_file = gszLogPath;
    log_file /= szFileName;
#if defined(_OS_WINDOWS)
    fopen_s(&pFile, log_file.generic_string().c_str(), "a");
#elif defined(_OS_LINUX)
    pFile = fopen(log_file.generic_string().c_str(), "a");
#endif // _OS_WINDOWS || _OS_LINUX
    if (pFile == NULL)
    {
        return;
    }
    need_size = strlen(szBuff);
    write_size = fwrite(szBuff, sizeof(BYTE), need_size, pFile);
    if (write_size != need_size)
    {
        fclose(pFile);
        return;
    }
    fclose(pFile);
*/
}

/**
****************************************************************************************************
* @brief : sleep(milli seconds)
****************************************************************************************************
*/
void DogTrack_SleepMs(DWORD milli_seconds)
{
	if (milli_seconds)
	{ 
        usleep(milli_seconds * 1000);
	}
}

/**
****************************************************************************************************
* @brief : Write the file by the specified file name in the specified mode.
****************************************************************************************************
*/
int WriteBinary(const char* pszFilePath, const char* mode, BYTE* pData, DWORD data_size)
{
    FILE * pFile = NULL;
    size_t write_size = 0;
    if ((pszFilePath == NULL) || (mode == NULL) || (pData == NULL) || (data_size == 0))
    {
        printf("전달된 아규먼트에 문제가 있습니다.\n");
        return DT_STATUS_INVALID_ARGS;
    }

#if defined(_OS_WINDOWS)
    fopen_s(&pFile, pszFilePath, mode);
#elif defined(_OS_LINUX)
    pFile = fopen(pszFilePath, mode);
#endif /* _OS_WINDOWS || _OS_LINUX */
    if (pFile == NULL)
    {
        printf("파일 개방에 실패했습니다(%s, %s).\n", pszFilePath, mode);
        return DT_STATUS_FILE_OPEN_ERROR;
    }
    write_size = fwrite(pData, sizeof(BYTE), data_size, pFile);
    if (write_size != data_size)
    {
        fclose(pFile);
        printf("파일에 지정된 크기만큼 출력하지 못했거나 에러가 발생했습니다(%u:%u).\n", 
            data_size, (unsigned int)write_size);
        return DT_STATUS_FILE_WRITE_FAILED;
    }
    fclose(pFile);

    return write_size;
}

/**
****************************************************************************************************
* @brief : Read the specified file in the specified mode.
****************************************************************************************************
*/
int ReadBinary(const char* pszFilePath, const char* mode, BYTE** ppData, DWORD& data_size)
{
    FILE * pFile = NULL;
    size_t read_size = 0;
    long size = 0;

    if ((pszFilePath == NULL) || (mode == NULL) || (ppData == NULL))
    {
        printf("전달된 아규먼트에 문제가 있습니다.\n");
        return DT_STATUS_INVALID_ARGS;
    }
#if defined(_OS_WINDOWS)
    fopen_s(&pFile, pszFilePath, mode);
#elif defined(_OS_LINUX)
    pFile = fopen(pszFilePath, mode);
#endif /* _OS_WINDOWS || _OS_LINUX */
    if (pFile == NULL)
    {
        printf("파일 개방에 실패했습니다(%s, %s).\n", pszFilePath, mode);
        return DT_STATUS_FILE_OPEN_ERROR;
    }
    fseek(pFile, 0, SEEK_END);
    size = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);

    DT_MALLOC(*ppData, BYTE, size * sizeof(BYTE));
    if (*ppData == NULL)
    {
        printf("메모리 할당에 실패했습니다.\n");
        fclose(pFile);
        return DT_STATUS_MEM_ERROR;
    }
#if defined(_OS_WINDOWS)    
    read_size = fread_s(*ppData, size, sizeof(BYTE), (size_t)size, pFile);
#elif defined(_OS_LINUX)
    read_size = fread(*ppData, sizeof(BYTE), size, pFile);
#endif /* _OS_WINDOWS || _OS_LINUX */
    if (read_size != (size_t)size)
    {
        DT_FREE(*ppData);
        fclose(pFile);
        printf("파일에서 지정된 크기만큼 읽지 못했거나 에러가 발생했습니다(%u:%u).\n",
            (unsigned int)size, (unsigned int)read_size);
        return DT_STATUS_FILE_READ_FAILED;
    }
    fclose(pFile);
    data_size = read_size;
    return read_size;
}

/**
****************************************************************************************************
* @brief : print the passed data to hexa format.
****************************************************************************************************
*/
void ShowHexAlignColumn(char* pTitle, BYTE* pData, DWORD data_size, int align_cloumn)
{
#ifdef _DEBUG
    DWORD col_cnt = (DWORD)align_cloumn;

    printf("[date size:%d\n", data_size);

    if ((pData == NULL) || (data_size == 0))
    {
        return;
    }
    if (col_cnt == 0)
    {
        return;
    }

    for (DWORD idx = 0; idx < data_size; ++idx)
    {
        if (idx == 0)
        {
            if (pTitle != NULL)
            {
                printf("%s >> \n", pTitle);
            }
        }
        printf("[%02x]", pData[idx]);
        if (idx == (data_size - 1))
        {
            printf("\n");
        }
        else
        {
            if (!((idx + 1) % col_cnt))
            {
                printf("\n");
            }
        }
    }
#endif /* _DEBUG */
}

#ifdef LEAK_CHECK

/**
****************************************************************************************************
*
****************************************************************************************************
*/
void DogTrack_malloc(void* pData, DWORD size, const char* pFunction, int line)
{
    int len = 0;
    DWORD address = 0;

    if ((pData == NULL) || (pFunction == NULL) || (size == 0))
    {
        return;
    }

    /*if (g_pMemStoreCsLock == NULL)
    {
        g_pMemStoreCsLock = new CUserCsLock("Memory_Store");
    }*/
    pMemEntry_t pEntry = NULL;
    pEntry = (pMemEntry_t)malloc(sizeof(MemEntry_t));
    if (pEntry == NULL)
    {
        printf("[%s][%d]Critical error in memory leak checking.........\n", __FUNCTION__, __LINE__);
        return;
    }
    pEntry->line = line;
    pEntry->size = size;
    len = strlen(pFunction);
    pEntry->pFunction = (char*)malloc(len + 1);
    if (pEntry->pFunction == NULL)
    {
        free(pEntry);
        return;
    }
    memcpy(pEntry->pFunction, pFunction, len);
    (pEntry->pFunction)[len] = 0x00;
    address = (DWORD)pData;
    std::pair<DWORD, pMemEntry_t>NewMapItem(address, pEntry);
    //g_pMemStoreCsLock->Lock();
    gMemStoreMap.insert(NewMapItem);
    //g_pMemStoreCsLock->Unlock();
    return;
}

/**
****************************************************************************************************
*
****************************************************************************************************
*/
void DogTrack_free(void* pData)
{
    MemStoreMap::const_iterator iter;
    pMemEntry_t pEntry = NULL;
    DWORD key = 0;

    if (pData == NULL)
    {
        return;
    }

    key = (DWORD)pData;
    //g_pMemStoreCsLock->Lock();
    iter = gMemStoreMap.find(key);
    if (iter != gMemStoreMap.end())
    {
        pEntry = (pMemEntry_t)iter->second;
        if (pEntry)
        {
            if (pEntry->pFunction)
            {
                free(pEntry->pFunction);
                pEntry->pFunction = NULL;
            }
            free(pEntry);
            pEntry = NULL;
            gMemStoreMap.erase(iter);
        }
    }
    else
    {
        printf("[%s][%d]Critical error NOT found[%d]\n", __FUNCTION__, __LINE__, key);
    }
    //g_pMemStoreCsLock->Unlock();
}

/**
****************************************************************************************************
*
****************************************************************************************************
*/
void DogTrack_memcpy(void* target, void* source, int size)
{
    MemStoreMap::const_iterator iter;
    MemStoreMap::const_iterator iter2;
    pMemEntry_t pEntry = NULL;
    pMemEntry_t pEntry2 = NULL;
    DWORD target_address = 0;
    DWORD source_address = 0;
    int target_alloc_size = 0;
    int source_alloc_size = 0;

    if (target == NULL)
    {
        printf("memcpy target param is invalid.\n");
        assert(0);
        return;
    }
    if ((source != NULL) && (size < 1))
    {
        printf("source params is invalid.\n");
        assert(0);
        return;
    }
    target_address = (DWORD)target;
    source_address = (DWORD)source;
    //g_pMemStoreCsLock->Lock();
    iter = gMemStoreMap.find(target_address);
    if (iter != gMemStoreMap.end())
    {
        pEntry = (pMemEntry_t)iter->second;
        target_alloc_size = pEntry->size;

        iter2 = gMemStoreMap.find(source_address);
        if (iter2 != gMemStoreMap.end())
        {
            pEntry2 = (pMemEntry_t)iter->second;
            source_alloc_size = pEntry2->size;

            if ((size > target_alloc_size) || (size > source_alloc_size))
            {
                printf("memory copying looks invalid. [target allocated size:%d, source allocated size:%d, copying size:%d]\n", target_alloc_size, source_alloc_size, size);
                DogTrack_mem_leak_display();
                //g_pMemStoreCsLock->Unlock();
                assert(0);
                return;
            }
        }
        else
        {
            if (size > target_alloc_size)
            {
                printf("memory copying looks invalid. [target allocated size:%d, source allocated size:UNKNOWN, copying size:%d]\n", target_alloc_size, size);
                DogTrack_mem_leak_display();
                //g_pMemStoreCsLock->Unlock();
                assert(0);
                return;
            }
        }
    }
    //g_pMemStoreCsLock->Unlock();
}

/**
****************************************************************************************************
*
****************************************************************************************************
*/
void DogTrack_mem_leak_display()
{
    MemStoreMap::const_iterator iter;
    pMemEntry_t pEntry = NULL;
    //g_pMemStoreCsLock->Lock();
    printf("========================= allocated list ========================= START\n");
    for (iter = gMemStoreMap.begin(); iter != gMemStoreMap.end(); ++iter)
    {
        pEntry = (pMemEntry_t)iter->second;
        if (pEntry)
        {
            printf("[%s][%d]  => allocated size : %d\n", pEntry->pFunction, pEntry->line, pEntry->size);
        }
    }
    printf("========================= allocated list ========================= STOP\n");
    //g_pMemStoreCsLock->Unlock();
}

/**
****************************************************************************************************
*
****************************************************************************************************
*/
void DogTrack_terminate_mem_check()
{
    MemStoreMap::const_iterator iter;
    pMemEntry_t pEntry = NULL;

    for (iter = gMemStoreMap.begin(); iter != gMemStoreMap.end(); ++iter)
    {
        pEntry = (pMemEntry_t)iter->second;
        if (pEntry)
        {
            if (pEntry->pFunction)
            {
                free(pEntry->pFunction);
                pEntry->pFunction = NULL;
            }
            free(pEntry);
            pEntry = NULL;
        }
    }

    gMemStoreMap.clear();

    /*if (g_pMemStoreCsLock)
    {
        delete g_pMemStoreCsLock;
    }*/
}

#endif /* LEAK_CHECK */

