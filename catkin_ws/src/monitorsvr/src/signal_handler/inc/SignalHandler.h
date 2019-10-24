/**
****************************************************************************************************
* @file SignalHandler.h
* @brief General interface for linux signal
* @author 
* @date Created Oct 01, 2019
* @see Coding standard guideline
****************************************************************************************************
*/
#ifndef _SIGNAL_HANDLER_H_
#define _SIGNAL_HANDLER_H_

#if defined (_OS_WINDOWS)
#include "os_windows.h"
#elif defined (_OS_LINUX)
#include "os_linux.h"
#else
#error Unknown OS - not supported
#endif /* _OS_WINDOWS || _OS_LINUX */

#if defined(_OS_WINDOWS)
#include <signal.h> 
// https://docs.microsoft.com/ko-kr/cpp/c-runtime-library/reference/signal?view=vs-2017
#elif defined(_OS_LINUX)
#include "csignal"
#endif


class SignalHandler
{
private:
	SignalHandler();
public:
	virtual ~SignalHandler();

	static SignalHandler* GetInstance();
	static void ReleaseInstance();

	Bool IsSignalReceived();
	Bool IsSignalReceived(int sigNum);
	void ResetRecievedSignal();
	static void SetSignal(int sigNum);
	void PrintReceivedSignal();

	DT_STATUS StartHandler(void* pData);
	DT_STATUS StopHandler();
#if defined(_OS_LINUX)
    DT_STATUS CatchSignal(int sigNum);
#endif /* _OS_LINUX */
private:
	static SignalHandler* m_pInstance;
	static volatile sig_atomic_t m_signaled;
#if defined(_OS_LINUX)
    struct sigaction m_sig_handler;
#endif /* _OS_LINUX */
    pthread_t m_tid;
};


#endif /* _SIGNAL_HANDLER_H_ */