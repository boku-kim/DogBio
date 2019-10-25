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

#include "MonitorSvr.h"
#include "Utils.h"

static DWORD s_loop = 1;
static DWORD s_internal_loop = 1;

///< Monitor Server Logic Implementation

void FreeSendDataWrap(DtSendDataWrap_t* pWrap);

/**
****************************************************************************************************
* @brief : check whether reading is enabled.
****************************************************************************************************
*/
//TODO ClearAll
void MonitorServer::ClearAll(){
	
 	pthread_mutex_lock(&m_mutex_queue);
    while (1)
    {   
		DtSendDataWrap_t* ppItem = NULL;
   		if (m_queue.size() > 0)
    	{
        	ppItem = m_queue[m_queue.size() - 1];
        	m_queue.pop_back();
   		}

        //GetItem(&pItem);
        
        if (ppItem == NULL)
        {
            break;
        }
        FreeSendDataWrap(ppItem);
    }
    m_queue.clear();
	DT_CHECK_POINT("^^^^^^^^^^^^^^^^^^^^queue size: %d", m_queue.size());
	pthread_mutex_unlock(&m_mutex_queue);
}

//TODO redady_for_reading
DT_STATUS ready_for_reading(int socket_fd, DWORD msec)
{

    if ( socket_fd < 0 )
    {
        DT_CHECK_POINT("Invalid socket descriptor");
        DogTrack_SleepMs(msec);
        return DT_STATUS_FAIL;
    }

    struct timeval timeout;
    timeout.tv_sec = msec / 1000;
    timeout.tv_usec = (msec % 1000) * 1000;

    fd_set dSet;
    FD_ZERO(&dSet);
    FD_SET(socket_fd, &dSet );

    //return (select(socket_fd + 1, &dSet, NULL, NULL, NULL) > 0 ) ? DT_STATUS_OK : DT_STATUS_FAIL;
    return (select(socket_fd + 1, &dSet, NULL, NULL, &timeout) > 0 ) ? DT_STATUS_OK : DT_STATUS_FAIL;

}

/**
****************************************************************************************************
* @brief : accept connection asynchronously
****************************************************************************************************
*/
//TODO accept_connection
DT_STATUS accept_connection(int socket_fd, int& client_fd, DWORD msec)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = 0;
    char temp[64] = { 0 };
    
    if (ready_for_reading(socket_fd, msec) != DT_STATUS_OK)
    {
       // DT_CHECK_POINT("time out after %d msec", msec);
        return DT_STATUS_FAIL;
    }
    client_addr_len = sizeof(client_addr); 
    memset(&client_addr, 0x00, client_addr_len);
    client_fd = accept(socket_fd,
                       (struct sockaddr *)&client_addr,
                       &client_addr_len );
    DT_CHECK_POINT("client_fd: %d", client_fd);
                   
    if (client_fd == -1)
    {
        DT_CHECK_POINT("accept() error[%d]", errno);
        return DT_STATUS_FAIL;
    }

    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, temp, sizeof(temp));
    DT_CHECK_POINT("[%s] client connected..", temp);
        
    return DT_STATUS_OK;
}

//TODO NewDataWrap
DtFullDataWrap_t* NewDataWrap()
{
    DtFullDataWrap_t* pTemp = NULL;
    pTemp = (DtFullDataWrap_t*)malloc(sizeof(DtFullDataWrap_t));
    return pTemp;
}
//TODO FreeDataWrap
void FreeDataWrap(DtFullDataWrap_t* pWrap)
{
    if (pWrap)
    {
        DT_FREE(pWrap->pData);
        DT_FREE(pWrap);
    }
}
//TODO NewSendDataWrap
DtSendDataWrap_t* NewSendDataWrap()
{
    DtSendDataWrap_t* pTemp = NULL;
    pTemp = (DtSendDataWrap_t*)malloc(sizeof(DtSendDataWrap_t));
    if (pTemp)
    {
        memset(pTemp, 0x00, sizeof(DtSendDataWrap_t));
    }
    return pTemp;
}

//TODO FreeSendDataWrap
void FreeSendDataWrap(DtSendDataWrap_t* pWrap)
{
    DtDetectedArea_t* pTemp = NULL;
    DtDetectedArea_t* pTarget = NULL;
    
    if (pWrap)
    {
        pTarget = pWrap->detected_info;
        while (pTarget != NULL)
        {
            pTemp = pTarget->pNext;
            DT_FREE(pTarget);
            pTarget = pTemp;
        }
        DT_FREE(pWrap);
    }
}

/**
****************************************************************************************************
* @brief : Make the raw packet which will being sent to client.
****************************************************************************************************
*/
//TODO MakeOnePacket
DT_STATUS MakeOnePacket(BYTE **pSendBuffer, int& send_size)
{
    DT_STATUS ret = DT_STATUS_OK;
    MonitorServer* pMonitorServer = NULL;
    DtSendDataWrap_t* pItem = NULL;
    DtDetectedArea_t* pTemp = NULL;
    DtDetectedArea_t* pTarget = NULL;
    BYTE* pData = NULL;
    DWORD data_size = 0;
    DWORD data_store_pos = 0;
    DWORD detect_count = 0;
    DWORD convert_count = 0;
    DWORD_TO_BYTES dword_bytes;
    uint_and_float conv_uint_float;
    DWORD nConvFailed = 0;

    if (pSendBuffer == NULL)
    {
        return DT_STATUS_INVALID_ARGS;
    }

    pMonitorServer = MonitorServer::GetInstance();
    if (pMonitorServer)
    {
        pMonitorServer->GetItem(&pItem);
        if (pItem)
        {
            send_size = sizeof(float); // steering angle
            send_size += (sizeof(DWORD) * 2); // speed, detected_count
            send_size += ((sizeof(int) * DT_SEND_PACKET_AREA_INFO_COUNT) * pItem->detected_count); // detected info total count
            data_size = send_size;
            DT_CHECK_POINT("data_size:%d, detected count:%d, steer angle:%f, speed:%d", 
                data_size, pItem->detected_count, pItem->steering_angle, pItem->speed);
            send_size += DT_PACKET_STX_SIZE + DT_PACKET_FULL_LENGTH_SIZE + DT_PACKET_ETX_SIZE;
            *pSendBuffer = (BYTE*)malloc(send_size);
            if (*pSendBuffer)
            {
                (*pSendBuffer)[DT_PACKET_STX_POS] = DT_PACKET_STX;
                (*pSendBuffer)[send_size - 1] = DT_PACKET_ETX;

                dword_bytes.detail.value = htonl(send_size);
                memcpy((*pSendBuffer) + DT_PACKET_FULL_LENGTH_POS, dword_bytes.buffer, sizeof(dword_bytes.buffer));

                pData = (BYTE*)malloc(data_size);
                if (pData)
                {
                    data_store_pos = 0;
                    ///< steering_angle
                    conv_uint_float.fValue = pItem->steering_angle;
                    dword_bytes.detail.value = htonl(conv_uint_float.uValue);
                    memcpy(pData + data_store_pos, dword_bytes.buffer, sizeof(dword_bytes.buffer));
                    data_store_pos += sizeof(dword_bytes.buffer);

                    ///< speed
                    dword_bytes.detail.value = htonl(pItem->speed);
                    memcpy(pData + data_store_pos, dword_bytes.buffer, sizeof(dword_bytes.buffer));
                    data_store_pos += sizeof(dword_bytes.buffer);

                    ///< detected_count
                    dword_bytes.detail.value = htonl(pItem->detected_count);
                    memcpy(pData + data_store_pos, dword_bytes.buffer, sizeof(dword_bytes.buffer));
                    data_store_pos += sizeof(dword_bytes.buffer);

                    detect_count = pItem->detected_count;
                    pTarget = pItem->detected_info;
                    while (pTarget != NULL)
                    {                        
                        dword_bytes.detail.value = htonl(pTarget->x);
                        memcpy(pData + data_store_pos, dword_bytes.buffer, sizeof(dword_bytes.buffer));
                        data_store_pos += sizeof(dword_bytes.buffer);
                        IS_VALID_LENGTH(data_size, data_store_pos, nConvFailed);

                        dword_bytes.detail.value = htonl(pTarget->y);
                        memcpy(pData + data_store_pos, dword_bytes.buffer, sizeof(dword_bytes.buffer));
                        data_store_pos += sizeof(dword_bytes.buffer);
                        IS_VALID_LENGTH(data_size, data_store_pos, nConvFailed);

                        dword_bytes.detail.value = htonl(pTarget->w);
                        memcpy(pData + data_store_pos, dword_bytes.buffer, sizeof(dword_bytes.buffer));
                        data_store_pos += sizeof(dword_bytes.buffer);
                        IS_VALID_LENGTH(data_size, data_store_pos, nConvFailed);

                        dword_bytes.detail.value = htonl(pTarget->h);
                        memcpy(pData + data_store_pos, dword_bytes.buffer, sizeof(dword_bytes.buffer));
                        data_store_pos += sizeof(dword_bytes.buffer);
                        IS_VALID_LENGTH(data_size, data_store_pos, nConvFailed);
                        
                        dword_bytes.detail.value = htonl(pTarget->cls);
                        memcpy(pData + data_store_pos, dword_bytes.buffer, sizeof(dword_bytes.buffer));
                        data_store_pos += sizeof(dword_bytes.buffer);
                        IS_VALID_LENGTH(data_size, data_store_pos, nConvFailed);
                        
                        pTemp = pTarget->pNext;
                        pTarget = pTemp;
                        ++convert_count;
                    } // end while (pTarget != NULL)
                    if (nConvFailed || (convert_count != detect_count))
                    {
                        DT_CHECK_POINT("convert error.");
                        DT_FREE(*pSendBuffer);
                        send_size = 0;
                        DT_FREE(pData);
                        ret = DT_STATUS_PACKET_CONVERT_FAILED;
                    }
                    else
                    {
                        memcpy((*pSendBuffer) + DT_PACKET_DATA_POS, pData, data_size);
                    }
                } 
                else
                {
                    DT_FREE(*pSendBuffer);
                    send_size = 0;
                    ret = DT_STATUS_MEM_ERROR;
                } // end if (pData)
                
            }
            else
            {
                ret = DT_STATUS_MEM_ERROR;
            } // end if (pData)
            FreeSendDataWrap(pItem);
        }
        else
        {
            ret = DT_STATUS_QUEUE_GET_ITEM_FAILED;
        } // end if (pItem)
    }
    else
    {
        ret = DT_STATUS_MEM_ERROR;
    } // end if (pMonitorServer)

    return ret;
}

/**
****************************************************************************************************
* @brief : parse the received raw packet.
****************************************************************************************************
*/
//TODO ParsePacket
DT_STATUS ParsePacket(BYTE* pBuffer, int buffer_size, DtFullDataWrap_t** ppDataWrap)
{
    DT_STATUS ret = DT_STATUS_OK;
    //WORD_TO_BYTES word_byte;
    DWORD_TO_BYTES dword_bytes;

    if ((pBuffer == NULL) ||
        (buffer_size <= 0) ||
        (ppDataWrap == NULL))
    {
        DT_CHECK_POINT("invalid arguments.");
        return DT_STATUS_INVALID_ARGS;
    }

    ShowHexAlignColumn("ParsePacket's input packet : ", pBuffer, buffer_size, 16);

    if ((pBuffer[DT_PACKET_STX_POS] != DT_PACKET_STX) ||
        (pBuffer[buffer_size - 1] != DT_PACKET_ETX))
    {
        DT_CHECK_POINT("can not find the packet sign.");
        return DT_STATUS_PACKET_INVALID_SIGN;
    }

    DT_CHECK_POINT("maigc sign is valid.");

    *ppDataWrap = NewDataWrap();
    if (*ppDataWrap)
    {
        memset(*ppDataWrap, 0x00, sizeof(DtFullDataWrap_t));
        memcpy(dword_bytes.buffer, pBuffer + DT_PACKET_FULL_LENGTH_POS, sizeof(DWORD));
        (*ppDataWrap)->full_length = ntohl(dword_bytes.detail.value);
        DT_CHECK_POINT("full length : %d", (*ppDataWrap)->full_length);
        if ((*ppDataWrap)->full_length == (DWORD)buffer_size)
        {
            (*ppDataWrap)->data_size = buffer_size - 
                                       DT_PACKET_STX_SIZE - 
                                       DT_PACKET_FULL_LENGTH_SIZE - 
                                       DT_PACKET_ETX_SIZE;
            if ((*ppDataWrap)->data_size > 0)
            {
                (*ppDataWrap)->pData = (BYTE*)malloc((*ppDataWrap)->data_size);
                if ((*ppDataWrap)->pData != NULL)
                {
                    memcpy((*ppDataWrap)->pData, pBuffer + DT_PACKET_DATA_POS, (*ppDataWrap)->data_size);
                }
                else
                {
                    DT_FREE(*ppDataWrap);
                    DT_CHECK_POINT("data size is invalid[%d].");
                    ret = DT_STATUS_MEM_ERROR;
                }
            }
            else
            {
                DT_FREE(*ppDataWrap);
                DT_CHECK_POINT("data size is invalid[%d].", (*ppDataWrap)->data_size);
                ret = DT_STATUS_PACKET_INVALID_DATA_LENGTH;
            }
        }
        else
        {
            DT_FREE(*ppDataWrap);
            DT_CHECK_POINT("packet size is not matched[%d][%d].", (*ppDataWrap)->data_size, buffer_size);
            ret = DT_STATUS_PACKET_INVALID_FULL_LENGTH;
        }
    }
    else
    {
        DT_CHECK_POINT("allocating memory was failed.");
        ret = DT_STATUS_MEM_ERROR;
    }
    return ret;
}

/**
****************************************************************************************************
* @brief : Merge the splited packet to one packet.
****************************************************************************************************
*/
//TODO MergePacket
DT_STATUS MergePacket(BYTE** ppTargetBinary, int& nTargetSize, BYTE* pSourceBinary, int nSourceSize)
{
    DT_STATUS ret = DT_STATUS_OK;
    BYTE* pTemp = NULL;
    int temp_size = 0;

    if ((ppTargetBinary == NULL) || 
        (pSourceBinary == NULL) ||
        (nTargetSize < 0) || 
        (nSourceSize <= 0))
    {
        DT_CHECK_POINT("invalid arguments.");
        return DT_STATUS_INVALID_ARGS;
    }

    if (*ppTargetBinary == NULL)
    {
        *ppTargetBinary = (BYTE*)malloc(nSourceSize);
        if (*ppTargetBinary != NULL)
        {
            memcpy(*ppTargetBinary, pSourceBinary, nSourceSize);
            nTargetSize = nSourceSize;
        }
        else
        {
            DT_CHECK_POINT("allocating memory was failed.");
            ret = DT_STATUS_MEM_ERROR;
        }
    }
    else
    {
        // backup the previous data
        pTemp = (BYTE*)malloc(nTargetSize);
        if (pTemp != NULL)
        {
            memcpy(pTemp, *ppTargetBinary, nTargetSize);
            DT_FREE(*ppTargetBinary);
            temp_size = nTargetSize;
            nTargetSize += nSourceSize;

            *ppTargetBinary = (BYTE*)malloc(nTargetSize);
            if (*ppTargetBinary != NULL)
            {
                memcpy(*ppTargetBinary, pTemp, temp_size);
                memcpy(*ppTargetBinary + temp_size, pSourceBinary, nSourceSize);
            }
            else
            {
                DT_CHECK_POINT("allocating memory was failed.");
                ret = DT_STATUS_MEM_ERROR;
            }
            DT_FREE(pTemp);
            temp_size = 0;
        }
        else
        {
            DT_CHECK_POINT("allocating memory was failed.");
            ret = DT_STATUS_MEM_ERROR;
        } // end if (pTemp != NULL)
    } // end if (pBuffer == NULL)
    
    return ret;
}

/**
****************************************************************************************************
* @brief : communicate to client
****************************************************************************************************
*/
//TODO communicate_to_client
void communicate_to_client(int socket_fd,MonitorServer* pParent)
{
    DT_STATUS ret = DT_STATUS_OK;
    DtFullDataWrap_t* pDataWrap = NULL;
    BYTE ReceiveBuff[DT_MONITOR_SVR_RECV_BUFF_LENGTH] = {0};
    BYTE* pBuffer = NULL;
    BYTE* pSendBuffer = NULL;
    int read_size = 0;
    int buffer_size = 0;
    int send_size = 0;
    int ret_socket = 0;
    int nReceiveOk = 0;

    while (s_internal_loop)
    {	
        DT_CHECK_POINT("Wait for the packet being sent from client.");
        ///< Receive packet
        nReceiveOk = 0;
 		
        while (!nReceiveOk)
        {
            read_size = read(socket_fd, ReceiveBuff, sizeof(ReceiveBuff));
            if (read_size <= 0)
            {
                DT_CHECK_POINT("reading socket was failed[%d,%d].", read_size, errno);
                return;
            }
            DT_CHECK_POINT("read success[size:%d].", read_size);
            ShowHexAlignColumn("read packet : ", ReceiveBuff, read_size, 16);
            ret = MergePacket(&pBuffer, buffer_size, ReceiveBuff, read_size);
            if (ret == DT_STATUS_OK)
            {
                DT_CHECK_POINT("MergePacket was succeeded.");
                ShowHexAlignColumn("MergePacket's result packet : ", pBuffer, buffer_size, 16);
                if ((pBuffer[DT_PACKET_STX_POS] == DT_PACKET_STX) &&
                    (pBuffer[buffer_size - 1] == DT_PACKET_ETX))
                {
                    DT_CHECK_POINT("maigc sign is valid.");
                    nReceiveOk = 1;
                }
            }
            else if (ret == DT_STATUS_MEM_ERROR)
            {
                DT_CHECK_POINT("MergePacket was failed[%d].", ret);
                DT_FREE(pBuffer);
            }
            else
            {
                DT_CHECK_POINT("MergePacket was failed[%d].", ret); ///< Ignore other errors.
                DT_FREE(pBuffer);
            } // end if (ret == DT_STATUS_OK)
        } // end while (!nReceiveOk)

        if ((pBuffer == NULL) || (buffer_size <= 0))
        {
            DT_CHECK_POINT("Internel logic error[client will being closed].");
            return;
        }

        ///< parsing packet
        ret = ParsePacket(pBuffer, buffer_size, &pDataWrap);
		
        if (ret != DT_STATUS_OK)
        {
            DT_CHECK_POINT("ParsePacket was failed.");
            DT_FREE(pBuffer);
            return;
        }
        DT_FREE(pBuffer);

        if (strncmp((const char*)pDataWrap->pData, DT_MONITOR_SVR_OK_PACKET, strlen(DT_MONITOR_SVR_OK_PACKET)) != 0)
        {
            DT_CHECK_POINT("Comparing was failed.");
            FreeDataWrap(pDataWrap);
            return;
        }
        DT_CHECK_POINT("Comparing was succeeded.");

        ///< NOTE : if you wanna handle data, you have to do just here.
        FreeDataWrap(pDataWrap);

        ret = MakeOnePacket(&pSendBuffer, send_size);
        if (ret == DT_STATUS_OK)
        {
            DT_CHECK_POINT("MakeOnePacket was succeeded.");
            
            int nWritePacketFailed = 0;
            ret_socket = write(socket_fd, pSendBuffer, send_size);
            if (ret_socket == send_size)
            {
                DT_CHECK_POINT("Written data size was succeeded[%d,%d].", ret_socket, send_size);            
            }
            else if (ret_socket == -1)
            {
                DT_CHECK_POINT("writing data to socket was failed[%d,%d].", ret_socket, errno);
                nWritePacketFailed = 1;
            }
            else if (ret_socket != send_size) // this is NOT error, but return for application purpose.
            {
                DT_CHECK_POINT("Written data size was invalid[%d,%d].", ret_socket, send_size);
                nWritePacketFailed = 1;
            }
            else
            {
                DT_CHECK_POINT("Unknown error in writing[%d].", ret_socket);
                nWritePacketFailed = 1;
            }
            DT_FREE(pSendBuffer);
            send_size = 0;

            if (nWritePacketFailed == 1)
            {
                return;
            }
        } // end if (ret == DT_STATUS_OK)
        else
        {
            DT_CHECK_POINT("MakeOnePacket was failed.");
            DT_FREE(pSendBuffer);
            send_size = 0;
            return; ///< TODO : need to remove the comment.
        }
		pParent->ClearAll();
        DogTrack_SleepMs(1000);
    } // end while (s_internal_loop)
    
}

/**
****************************************************************************************************
* @brief : accept socket client and handling client
****************************************************************************************************
*/
//TODO dt_server_handler
void* dt_server_handler(void* data)
{
    MonitorServer* pParent = NULL;    
    DT_STATUS ret = DT_STATUS_OK;
    
	if (data == NULL)
	{
		return NULL;
	}
    pParent = (MonitorServer*)data;
 
    while (s_loop)
    {
        DT_CHECK_POINT("receiving data");
        ret = accept_connection(pParent->m_server_fd, pParent->m_client_fd, DT_MONITOR_SVR_ACCEPT_WAIT_TIME);
        if (ret != DT_STATUS_OK)
        {
            continue;
        }

        if (pParent->m_client_fd < 0)
        {
            DT_CHECK_POINT("invalid client socket descriptor[%d].", pParent->m_client_fd);
            continue;
        }
        communicate_to_client(pParent->m_client_fd,pParent);
        if (pParent->m_client_fd > 0)
        {
            close(pParent->m_client_fd);
        }
        DT_CHECK_POINT("Server : client closed.");
    }

	return NULL;
}

MonitorServer* MonitorServer::m_pInstance = NULL;

/**
****************************************************************************************************
* @brief : creating Monitor Server's instance(singleton)
****************************************************************************************************
*/
MonitorServer* MonitorServer::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new MonitorServer();
	}
	return m_pInstance;
}

/**
****************************************************************************************************
* @brief : destroying Monitor Server's instance
****************************************************************************************************
*/
void MonitorServer::ReleaseInstance()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

#if defined(_OS_WINDOWS)
#elif defined(_OS_LINUX) 

/**
****************************************************************************************************
* @brief : Creator
****************************************************************************************************
*/
MonitorServer::MonitorServer()
    : m_server_fd(-1)
    , m_client_fd(-1)
    , m_server_addr()
    , m_tid()
    , m_mutex_queue(PTHREAD_MUTEX_INITIALIZER)
    , m_queue()
{
    StartServer();
}

/**
****************************************************************************************************
* @brief : Destroyer
****************************************************************************************************
*/
MonitorServer::~MonitorServer()
{
    StopServer();
}

/**
****************************************************************************************************
* @brief : Start Monitor Server
****************************************************************************************************
*/

//TODO StartServer
DT_STATUS MonitorServer::StartServer()
{
    int ret = 0;

    DT_CHECK_POINT("Start Monitor Server");
    /*
    int socket(domain,type, protocol);
    domain : integer, communication domain e.g., AF_INET(IPv4 protocol),AF_INET6(IPv6 protocol)
    type : communication type. SOCK_STREAM(TCP), SOCK_DGRAM(UDP)
    protocol : Protocol value for IP, which is 0. This si the same number which appears on protocol field in th IP header of a packet. 
    */
    m_server_fd = socket(AF_INET, SOCK_STREAM, 0); 
    if (m_server_fd == -1)
    {
        DT_CHECK_POINT("creating socket was failed[%d].", errno);
        return DT_STATUS_SOCKET_CREATE_FAILED;
    }
    memset(&m_server_addr, 0x00, sizeof(struct sockaddr_in));

    if (fcntl(m_server_fd, F_SETFL, O_NONBLOCK ) == -1 )
    {
        DT_CHECK_POINT("fcntl() failed[%d]", errno);
        return DT_STATUS_SOCKET_FCNTL_FAILED;
    }
        
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY : ??œe²?i?? IP i￡¼i†Œe￥? ?????™?œ¼e¡? i°¾i????œ ?????…
    m_server_addr.sin_port = htons(DT_MONITOR_SVR_SOCKET_PORT);

    /*
    after creation of the socket, bind function binds the socket to address and port number specified in addr
    */
    if(bind(m_server_fd, (struct sockaddr *)&m_server_addr, sizeof(struct sockaddr_in)) < 0)
    {
        DT_CHECK_POINT("Can't bind local address[%d].", errno);
        return DT_STATUS_SOCKET_BIND_FAILED;
    }
    
    if(listen(m_server_fd, DT_MONITOR_SVR_SOCKET_IN_QUEUE_CNT) < 0) 
    {
        DT_CHECK_POINT("Can't listening connect[%d].", errno);
        return DT_STATUS_SOCKET_LISTEN_FAILED;
    }
    DT_CHECK_POINT("listen complete");
    ret = pthread_create(&m_tid, NULL, &dt_server_handler, (void *)this);
    if (ret != 0)
    {
        return DT_STATUS_FAIL;
    }
	return DT_STATUS_OK;
}

/**
****************************************************************************************************
* @brief : Stop Monitor Server
****************************************************************************************************
*/
DT_STATUS MonitorServer::StopServer()
{
	s_loop = 0;
    s_internal_loop = 0;

    if (m_client_fd > 0)
    {
        close(m_client_fd);
    }

    if (m_server_fd > 0)
    {
        close(m_server_fd);
    }

    DtSendDataWrap_t* pItem = NULL;
    while (1)
    {
        GetItem(&pItem);
        if (pItem == NULL)
        {
            break;
        }
        FreeSendDataWrap(pItem);
    }
    m_queue.clear();

    DT_CHECK_POINT("Stop Monitor Server");
    if (m_tid)
    {
        pthread_join(m_tid, NULL); 
    }
    
	return DT_STATUS_OK;
}

//TODO AddItem
DT_STATUS MonitorServer::AddItem(DtSendDataWrap_t* pItem)
{
    DT_STATUS ret = DT_STATUS_OK;

    if (pItem == NULL)
    {
        DT_CHECK_POINT("Invalid argument. item is NULL.");
        return DT_STATUS_MUTEX_ITEM_NULL;
    }
    pthread_mutex_lock(&m_mutex_queue);

    if (m_queue.size() > 0)
    {
        auto iter = m_queue.begin();
        m_queue.insert(iter, pItem);
    }
    else
    {
        m_queue.push_back(pItem);
    }

    pthread_mutex_unlock(&m_mutex_queue); 
    
    return ret;
}
//TODO GetItem
DT_STATUS MonitorServer::GetItem(DtSendDataWrap_t** ppItem)
{
    DT_STATUS ret = DT_STATUS_OK;

    pthread_mutex_lock(&m_mutex_queue); 

    *ppItem = NULL;
    if (m_queue.size() > 0)
    {
        *ppItem = m_queue[m_queue.size() - 1];
        m_queue.pop_back();
    }

    pthread_mutex_unlock(&m_mutex_queue); 
    
    return ret;
}

#endif /* _OS_WINDOWS || _OS_LINUX */
