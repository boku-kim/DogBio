/**
****************************************************************************************************
* @file SignalHandler.cpp
* @brief General interface for linux signal
* @author 
* @date Created Oct 01, 2019
* @see Coding standard guideline
****************************************************************************************************
*/
#if defined(_OS_WINDOWS) && defined(WIN32)
#include "stdafx.h"
#elif defined(_OS_LINUX)
#else
#error Unknown OS - not supported
#endif /* _OS_WINDOWS || _OS_LINUX */

#include "SignalHandler.h"
#include "Utils.h"

#include "ros/ros.h"

volatile sig_atomic_t SignalHandler::m_signaled = 0;
static const DWORD c_second = 1000L;
static DWORD s_loop = 1;


///< Signal Handler Usage Logic Implementation

/**
****************************************************************************************************
* @brief : Thread function for signal catch
****************************************************************************************************
*/

//TODO dt_signal_handler
void* dt_signal_handler(void* data)
{
	/* //[[ comment___BEGIN -- tmep.nohgan
	if (data == NULL)
	{
		return NULL;
	}

	GMainLoop *main_loop = static_cast<GMainLoop *>(data);
	//*/ //]] comment___END -- tmep.nohgan
	SignalHandler* pSigHandler = SignalHandler::GetInstance();
	if (pSigHandler == NULL)
	{
		/* //[[ comment___BEGIN -- tmep.nohgan
		g_main_loop_quit(main_loop);
		//*/ //]] comment___END -- tmep.nohgan
		return NULL;
	}

	while (s_loop)
	{
		if (pSigHandler->IsSignalReceived())
		{	
			pSigHandler->PrintReceivedSignal();
#if defined (_OS_WINDOWS)
			
#elif defined (_OS_LINUX)
			if (pSigHandler->IsSignalReceived(SIGTERM))
			{
                DT_CHECK_POINT("SIGTERM signaled\n");
				/* //[[ comment___BEGIN -- tmep.nohgan
				g_main_loop_quit(main_loop);
				//*/ //]] comment___END -- tmep.nohgan
				ros::shutdown();
				break;
			}
            if (pSigHandler->IsSignalReceived(SIGABRT))
			{
                DT_CHECK_POINT("SIGABRT signaled\n");
				/* //[[ comment___BEGIN -- tmep.nohgan
				g_main_loop_quit(main_loop);
				//*/ //]] comment___END -- tmep.nohgan
				ros::shutdown();
				break;
			}
            else if (pSigHandler->IsSignalReceived(SIGPIPE))
			{
                DT_CHECK_POINT("SIGPIPE signaled\n");
				///< TODO pSigHandler->ResetRecievedSignal();
				///< TODO continue;
				/* //[[ comment___BEGIN -- tmep.nohgan
				g_main_loop_quit(main_loop);
				//*/ //]] comment___END -- tmep.nohgan
				ros::shutdown();
				break;
			}
            else if (pSigHandler->IsSignalReceived(SIGKILL))
            {
                DT_CHECK_POINT("SIGKILL signaled\n");
                ///< TODO pSigHandler->ResetRecievedSignal();
                ///< TODO continue;
                /* //[[ comment___BEGIN -- tmep.nohgan
                g_main_loop_quit(main_loop);
                //*/ //]] comment___END -- tmep.nohgan
                ros::shutdown();
				break;
            }
            else if (pSigHandler->IsSignalReceived(SIGHUP))
            {
                DT_CHECK_POINT("entered signaled\n");
                /* //[[ comment___BEGIN -- tmep.nohgan
                g_main_loop_quit(main_loop);
                //*/ //]] comment___END -- tmep.nohgan
                ros::shutdown();
                break;
            }
            else if (pSigHandler->IsSignalReceived(SIGHUP))
            {
                DT_CHECK_POINT("SIGHUP signaled\n");
                /* //[[ comment___BEGIN -- tmep.nohgan
                g_main_loop_quit(main_loop);
                //*/ //]] comment___END -- tmep.nohgan
                ros::shutdown();
                break;
            }
			else
			{
                DT_CHECK_POINT("other signal signaled\n");
				///< TODO pSigHandler->ResetRecievedSignal();
				///< TODO continue;
                /* //[[ comment___BEGIN -- tmep.nohgan
                g_main_loop_quit(main_loop);
                //*/ //]] comment___END -- tmep.nohgan
                ros::shutdown();
				break;
			}
#endif /* _OS_WINDOWS or _OS_LINUX */
		}
		DogTrack_SleepMs(c_second / 2);
	} /* end while (TRUE) */

	return NULL;
}

//__________________________________________________________
SignalHandler* SignalHandler::m_pInstance = NULL;

SignalHandler* SignalHandler::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new SignalHandler();
	}
	return m_pInstance;
}

void SignalHandler::ReleaseInstance()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

//_____________________________________________________________
#if defined(_OS_WINDOWS)
#elif defined(_OS_LINUX) 

/**
****************************************************************************************************
* @brief : Set the signal handler
****************************************************************************************************
*/
void SigHandler(int sigNum)
{
	SignalHandler::GetInstance()->SetSignal(sigNum);
}

/**
****************************************************************************************************
* @brief : creator
****************************************************************************************************
*/
SignalHandler::SignalHandler()
    : m_tid()
{
    m_sig_handler.sa_handler = SigHandler;
    m_sig_handler.sa_flags = 0;
    
    if ( sigemptyset( &m_sig_handler.sa_mask ) < 0 )
    {
        DT_CHECK_POINT("[critical error] sigemptyset was failed");
        return;
    }
    CatchSignal(SIGTERM);
    CatchSignal(SIGINT);
    CatchSignal(SIGABRT);
    CatchSignal(SIGQUIT);
    CatchSignal(SIGTSTP);
    CatchSignal(SIGPIPE);
    CatchSignal(SIGHUP);
}

/**
****************************************************************************************************
* @brief : destructor
****************************************************************************************************
*/
SignalHandler::~SignalHandler()
{
}

/**
****************************************************************************************************
* @brief : register the signal number that you wanna catch
****************************************************************************************************
*/
DT_STATUS SignalHandler::CatchSignal(int sigNum)
{
    if (sigaction(sigNum, &m_sig_handler, NULL ) < 0)
    {
        DT_CHECK_POINT("[critical error]sigaction() failed");
        return DT_STATUS_FAIL;
    }

    return DT_STATUS_OK;
}

/**
****************************************************************************************************
* @brief : check whether the system signal was occurred.
****************************************************************************************************
*/
Bool SignalHandler::IsSignalReceived()
{
	return (m_signaled == 0) ? FALSE : TRUE;
}

/**
****************************************************************************************************
* @brief : check whether the specified signal was occurred.
****************************************************************************************************
*/
Bool SignalHandler::IsSignalReceived(int sigNum)
{
	return (m_signaled == sigNum) ? TRUE : FALSE;
}

/**
****************************************************************************************************
* @brief : reset the signal handler
****************************************************************************************************
*/
void SignalHandler::ResetRecievedSignal()
{
	SetSignal(0);
}

/**
****************************************************************************************************
* @brief : This function was callled from void SigHandler(int sigNum)
****************************************************************************************************
*/
void SignalHandler::SetSignal(int sigNum)
{
	SignalHandler::m_signaled = sigNum;
}

/**
****************************************************************************************************
* @brief : show the signal number which was occurred just before.
****************************************************************************************************
*/
void SignalHandler::PrintReceivedSignal()
{
	if (m_signaled != 0)
	{
		printf("Signal (%d) was received.\n", m_signaled);
	}
}

#endif /* _OS_WINDOWS || _OS_LINUX */

/**
****************************************************************************************************
* @brief : The function starting the signal handler. For being called from external 
****************************************************************************************************
*/
DT_STATUS SignalHandler::StartHandler(void* pData)
{
    int ret = 0;
	/* //[[ comment___BEGIN -- tmep.nohgan
	GMainLoop *main_loop = NULL;

	if (pData == NULL)
	{
		return DT_STATUS_INVALID_ARGS;
	}
	main_loop = static_cast<GMainLoop *>(pData);
	//*/ //]] comment___END -- tmep.nohgan

    ret = pthread_create(&m_tid, NULL, dt_signal_handler, NULL /*(void *)main_loop*/);
    if (ret != 0)
    {
        return DT_STATUS_FAIL;
    }
	return DT_STATUS_OK;
}

/**
****************************************************************************************************
* @brief : The function stopping the signal handler. For being called from external 
****************************************************************************************************
*/
DT_STATUS SignalHandler::StopHandler()
{
	s_loop = 0;

    if (m_tid)
    {
        pthread_join(m_tid, NULL);    
    }
    
	return DT_STATUS_OK;
}
