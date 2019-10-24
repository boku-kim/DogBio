/**
****************************************************************************************************
* @file utils.h
* @brief 
* @author 
* @date Created Oct 01, 2019
* @see Coding standard guideline
****************************************************************************************************
*/
#ifndef _UTILS_H_
#define _UTILS_H_

#ifdef _OS_WINDOWS
#include "os_windows.h"
#elif defined(_OS_LINUX)
#include "os_linux.h"
#endif /* _OS_WINDOWS || _OS_LINUX */

#ifdef __cplusplus 
extern "C" {
#endif /* !__cplusplus */

//#define LEAK_CHECK

#define DT_SECURE_VALUE 0xc0

#ifdef LEAK_CHECK

#define DT_MALLOC(data, type, size) \
        (data) = (type*)malloc(size); \
        DogTrack_malloc(data, size, __FUNCTION__, __LINE__); 

#define DT_MALLOC_SECURE(data, type, size) \
    (data) = (type*)malloc(size); \
    if (data) { memset(data, DT_SECURE_VALUE, size); }

#define DT_MEMSET_SECURE(data, size) \
    memset(data, DT_SECURE_VALUE, size);

#define DT_FREE(arg) \
        DogTrack_free(arg); \
	    if (arg) { free(arg); arg = NULL; }

#define DT_FREE_SECURE(arg, size) \
        DogTrack_free(arg); \
        if (arg) { memset(arg, 0x00, size); free(arg); arg = NULL;}

#define DT_MEMCPY(tar, src, size) \
        DogTrack_memcpy((void*)(tar), (void*)(src), size); \
        memcpy(tar, src, size);

#else

#define DT_MALLOC(data, type, size) \
	    data = (type*)malloc(size); 

#define DT_FREE(arg) \
	    if (arg) { free(arg); arg = NULL; }

#define DT_FREE_SECURE(arg, size) \
        if (arg) { memset(arg, 0x00, size); free(arg); arg = NULL;}

#define DT_MEMCPY(tar, src, size) \
        memcpy(tar, src, size);

#define DT_MALLOC_SECURE(data, type, size) \
    data = (type*)malloc(size); \
    if (data) { memset(data, DT_SECURE_VALUE, size); }

#define DT_MEMSET_SECURE(data, size) \
    memset(data, DT_SECURE_VALUE, size);

#endif /* LEAK_CHECK */


#define DT_TIME_SECOND_TO_MILLI_SECONDS  1000
#define DT_DEBUG_OUTPUT_POS 40
#define DT_LOG_DELIMETER ">>>>> "
#define DT_LOG_DIR_NAME  "logs"
#define DT_LOG_FILE_EXT  ".txt"

#define DT_CHECK_POINT(fmt, ...) DogTrack_LogPrint(__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#define DT_CHECK_POINT2(fmt, ...) DogTrack_LogPrint2(fmt, ##__VA_ARGS__)

#define DT_IN            DT_CHECK_POINT("[%s][IN]", __FUNCTION__); 

#define DT_OUT           DT_CHECK_POINT("[%s][OUT]", __FUNCTION__);
#define DT_ERROR         DT_CHECK_POINT("make error code and output error log.");
#define DT_DEBUG_LOG     DT_CHECK_POINT("print debug error message but going on.");

typedef struct MemEntry
{
    char* pFunction;
    int line;
    int size;
} MemEntry_t, *pMemEntry_t;

void DogTrack_LogPrint(const char* function, int line, const char* szFormat, ...);
void DogTrack_LogPrint2(const char* szFormat, ...);

//#endif /**/
void DogTrack_SleepMs(DWORD milli_seconds);

int WriteBinary(const char* pszFilePath, const char* mode, BYTE* pData, DWORD data_size);
int ReadBinary(const char* pszFilePath, const char* mode, BYTE** ppData, DWORD& data_size);

void ShowHexAlignColumn(char* pTitle, BYTE* pData, DWORD data_size, int align_cloumn);

#ifdef LEAK_CHECK
void DogTrack_malloc(void* pData, DWORD size, const char* pFunction, int line);

/* @brief :
* @param : [in,out]
* @return :
* @descriptions :
* @note :
**/
void DogTrack_free(void* pData);

/* @brief :
* @param : [in,out]
* @return :
* @descriptions :
* @note :
**/
void DogTrack_memcpy(void* target, void* source, int size);

/* @brief :
* @param : [in,out]
* @return :
* @descriptions :
* @note :
**/
void DogTrack_mem_leak_display();

/* @brief :
* @param : [in,out]
* @return :
* @descriptions :
* @note :
**/
void DogTrack_terminate_mem_check();
#endif /* LEAK_CHECK */

#ifdef __cplusplus 
}
#endif /* !__cplusplus */

#endif /* _UTILS_H_ */
