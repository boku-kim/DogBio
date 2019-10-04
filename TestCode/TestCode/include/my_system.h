#ifndef _MY_SYSTEM_H_
#define _MY_SYSTEM_H_

#if defined(_OS_WINDOWS)
#include "error_define.h"
#include "my_define.h"

#if defined (_LANG_KO)
#include "ko_string.h"
#elif defined(_LANG_EN)
#include "en_string.h"
#else
#include "en_string.h"
#endif // LANG_KO  LANG_EN

#elif defined(_OS_LINUX)
#include "error_define.h"
#include "my_define.h"

#if defined (_LANG_KO)
#include "ko_string.h"
#elif defined(_LANG_EN)
#include "en_string.h"
#else
#include "en_string.h"
#endif // LANG_KO  LANG_EN

#else

#error Not supported

#endif // OS_WINDOWS  OS_LINUX

#endif /* _MY_SYSTEM_H_ */
