/**
****************************************************************************************************
* @file sys_general.h
* @brief 
* @author 
* @date Created 
* @see Coding standard guideline
****************************************************************************************************
*/
#ifndef _SYS_GENERAL_H_
#define _SYS_GENERAL_H_

#if defined (_OS_WINDOWS)
typedef int Bool;
#undef  TRUE
#define TRUE			1
#undef  FALSE
#define FALSE			0
typedef unsigned char	BYTE;
typedef unsigned short  WORD;
#undef DWORD
#define DWORD unsigned int

#elif defined (_OS_LINUX)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <iostream>
#include <stdarg.h>
#include <linux/limits.h>
#include <assert.h>

typedef int                    Bool;
typedef int                    BOOL;
#undef  TRUE
#define TRUE			        1
#undef  FALSE
#define FALSE			        0
#define MAX_PATH              PATH_MAX
#define _ASSERT               assert
typedef unsigned char	      BYTE;
typedef unsigned short        WORD;
typedef unsigned int          DWORD;

#define sprintf_s           snprintf
#define _vsnprintf_s        vsnprintf

#else
#error Unknown OS - not supported
#endif  /* #if defined (_OS_WINDOWS || _OS_LINUX) */


#if defined(OS_LINUX)
#if __cplusplus <= 199711L
  #error This library needs at least a C++11 compliant compiler
#endif
#endif /* OS_LINUX */

#endif /* _SYS_GENERAL_H_ */
