#pragma once
#include "ClientSocket.h"

// CSocketThread
#define DT_START_MESSAGE_EXCHANGE               WM_USER + 201
#define DT_STOP_MESSAGE_EXCHANGE                WM_USER + 202

///< Note : Refer to  DT_RECEIVED_MESSAGE  definition.
#define DT_SEND_RECEIVED_MESSAGE_TO_UI          WM_USER + 1   // This is only  for sending received message to UI  main.  

#define DT_RECEIVE_BUFFER_LENGTH                10240
#define DT_SEND_BUFFER_LENGTH                   1024

#define DT_PACKET_STX                           0x02
#define DT_PACKET_ETX                           0x03

#define DT_PACKET_STX_POS                       0
#define DT_PACKET_FULL_LENGTH_POS               1
#define DT_PACKET_DATA_POS                      5

#define DT_PACKET_STX_SIZE                      1
#define DT_PACKET_FULL_LENGTH_SIZE              4
#define DT_PACKET_ETX_SIZE                      1
#define DT_PACKET_DATA_SIZE                     2       ///< "ok"

#define DT_PACKET_DATA_RESPONSE                 "ok"

#define DT_FREE(arg) \
    if (arg) { free(arg); arg = NULL; }

#define IS_VALID_LENGTH(limit_size, current_size, failed) \
   if (current_size > limit_size) \
   { \
      failed = 1; \
   } 

typedef enum SocketStatus
{
    SOCKET_STATUS_NOT_CONNECTED,
    SOCKET_STATUS_CONNECTED,
    SOCKET_STATUS_MAX
} eSocketStatus;

/*
packet format
1. STX - 1 byte
2. full length - 4 byte
3. data area - variable bytes
4. ETX - 1 byte

data area
1. steering_angle
2. speed
3. detected_count
4. detected informations
*/

typedef struct DtDetectedArea
{
    int x;
    int y;
    int w;
    int h;
    int cls;
    struct DtDetectedArea* pNext;
} DtDetectedArea_t;

typedef struct DtDataWrap
{
    float steering_angle;
    DWORD speed;
    DWORD detected_count;
    DtDetectedArea_t* detected_info;
} DtDataWrap_t;

typedef struct DtFullDataWrap
{
    UINT full_length;   ///< packet full length(stx ~ etx)
    UINT data_size;     ///< packet data size
    BYTE* pData;        ///< packet data (variable size)
} DtFullDataWrap_t;

#pragma pack(push, 1)
union UINT_TO_BYTES
{
    struct DETAIL
    {
        UINT value;
    } detail;
    BYTE buffer[4];
};
#pragma pack(pop)

/**
****************************************************************************************************
*
****************************************************************************************************
*/
#pragma pack(push, 1)
union WORD_TO_BYTES
{
    struct DETAIL
    {
        WORD value;
    } detail;
    BYTE buffer[2];
};
#pragma pack(pop)

#pragma pack(push, 1)
union uint_and_float
{
    DWORD uValue;
    float fValue;
};
#pragma pack(pop)

typedef struct stPassData
{
    SOCKET hSocket;
    HWND hWnd;
    CWinThread *pThread;
} PassData_t;

class CSocketThread : public CWinThread
{
	DECLARE_DYNCREATE(CSocketThread)

protected:
	CSocketThread();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CSocketThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

    void SetWnd(HWND hWnd);
    afx_msg void OnStartMessageExchange(WPARAM wParam, LPARAM lParam);
    afx_msg void OnStopMessageExchange(WPARAM wParam, LPARAM lParam);
    static UINT Worker(LPVOID pData); //Thread 동작함수

public:
    //CClientSocket m_ClientSocket;
    SOCKET m_hSocket;

    HWND m_hWnd;

private:
    bool m_bLoop;
    CWinThread* m_pThread;

protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual int Run();

private:
    int MergePacket(BYTE** ppTargetBinary, int& nTargetSize, BYTE* pSourceBinary, int nSourceSize);
    int ParsePacket(BYTE* pData, int data_size, DtDataWrap_t** ppInfoWrap);
    int MakeOnePacket(BYTE **pSendBuffer, int& send_size);
};


