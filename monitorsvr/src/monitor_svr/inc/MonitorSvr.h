/**
****************************************************************************************************
* @file MonitorSvr.h
* @brief linux server for 
* @author 
* @date Created Oct 01, 2019
* @see Coding standard guideline
****************************************************************************************************
*/
#ifndef _MONITOR_SVR_H_
#define _MONITOR_SVR_H_

#if defined (_OS_WINDOWS)
#include "os_windows.h"
#elif defined (_OS_LINUX)
#include "os_linux.h"
#else
#error Unknown OS - not supported
#endif /* _OS_WINDOWS || _OS_LINUX */

#include "ros/ros.h"
#include "std_msgs/String.h"

#define DT_MONITOR_SVR_SOCKET_PORT              11111
#define DT_MONITOR_SVR_SOCKET_IN_QUEUE_CNT      5
#define DT_MONITOR_SVR_OK_PACKET                "ok"
#define DT_MONITOR_SVR_RECV_BUFF_LENGTH         1024
//#define DT_MONITOR_SVR_SEND_BUFF_LENGTH         1024
#define DT_MONITOR_SVR_ACCEPT_WAIT_TIME         1000

#define DT_PACKET_STX                           0x02
#define DT_PACKET_ETX                           0x03

#define DT_PACKET_STX_POS                       0
#define DT_PACKET_FULL_LENGTH_POS               1
#define DT_PACKET_DATA_POS                      5

#define DT_PACKET_STX_SIZE                      1
#define DT_PACKET_FULL_LENGTH_SIZE              4
#define DT_PACKET_ETX_SIZE                      1

#define DT_SEND_PACKET_AREA_INFO_COUNT          5


//#define DT_TEST_IMAGE_PATH                      "/home/fory2k/work/hancomprj/dog-tracker/test_image.jpg"
#define DT_TEST_IMAGE_PATH                      "/home/fory2k/work/dog-tracker/test_image.jpg"


#define IS_VALID_LENGTH(limit_size, current_size, failed) \
   if (current_size > limit_size) \
   { \
      failed = 1; \
      break; \
   } 

///<---------------------------------------------------------
typedef struct DtDetectedArea
{
    int x;
    int y;
    int w;
    int h;
    int cls;
    struct DtDetectedArea* pNext;
} DtDetectedArea_t;

typedef struct DtSendDataWrap
{
    float steering_angle;
    DWORD speed;    
    DWORD detected_count;    
    DtDetectedArea_t* detected_info;
} DtSendDataWrap_t;

///<---------------------------------------------------------
typedef struct DtFullDataWrap
{
    DWORD full_length;   ///< packet full length(stx ~ etx)
    DWORD data_size;     ///< packet data size
    BYTE* pData;        ///< packet data (variable size)
} DtFullDataWrap_t;

///<---------------------------------------------------------
/*
packet format
1. STX - 1 byte
2. full length - 4 byte
3. data area - variable bytes
4. ETX - 1 byte
*/
#pragma pack(push, 1)
union DT_PACKET_FULL_FORMAT
{
    struct PACKET_DETAIL
    {
        BYTE  packet_start;		// 0    
        DWORD full_length;		// 1 - 4
                                // variable data_area   // variable
                                // BYTE packet_stop
    } buffer_detail;
    BYTE buffer[21];
}
#if defined(_OS_LINUX)
__attribute__((packed));
#endif /* OS_LINUX */
;
#pragma pack(pop)

///<---------------------------------------------------------
#pragma pack(push, 1)
union DWORD_TO_BYTES
{
    struct DETAIL
    {
        DWORD value;
    } detail;
    BYTE buffer[4];
}
#if defined(_OS_LINUX)
__attribute__((packed));
#endif /* _OS_LINUX */
;
#pragma pack(pop)

///<---------------------------------------------------------
#pragma pack(push, 1)
union WORD_TO_BYTES
{
    struct DETAIL
    {
        WORD value;
    } detail;
    BYTE buffer[2];
}
#if defined(_OS_LINUX)
__attribute__((packed));
#endif /* _OS_LINUX */
;
#pragma pack(pop)

///<---------------------------------------------------------
#pragma pack(push, 1)
union uint_and_float 
{
    DWORD uValue;
    float fValue;
}
#if defined(_OS_LINUX)
__attribute__((packed));
#endif /* _OS_LINUX */
;
#pragma pack(pop)

///<---------------------------------------------------------
typedef std::vector<DtSendDataWrap_t*>SendDataQueueType;

///< Class MonitorServer Declaration
class MonitorServer
{
private:
	MonitorServer();
    
public:
	virtual ~MonitorServer();

	static MonitorServer* GetInstance();
	static void ReleaseInstance();

	DT_STATUS StartServer();
	DT_STATUS StopServer();
    DT_STATUS AddItem(DtSendDataWrap_t* pItem);
    DT_STATUS GetItem(DtSendDataWrap_t** pItem);

private:
    
        
public:
    int m_server_fd; // socket descriptor
    int m_client_fd;
    /*
    sockaddr 구조체 : 소켓의 주소를 담는 기본 구조체
    sockaddr_in 구조체 : sockaddr 구조체에서 AF_INET인 경우에 사용
    */
    struct sockaddr_in m_server_addr; 
    
private:
    
	static MonitorServer* m_pInstance;
    
    pthread_t m_tid;
    
    pthread_mutex_t m_mutex_queue;
    SendDataQueueType m_queue;
    
};


#endif /* _MONITOR_SVR_H_ */
