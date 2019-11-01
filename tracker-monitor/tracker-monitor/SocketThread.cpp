// SocketThread.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tracker-monitor.h"
#include "SocketThread.h"

///< ++++++++++++++++++++++++++++++++
CClientSocket gSockClient;

DtFullDataWrap_t* NewDataWrap()
{
	DtFullDataWrap_t* pTemp = NULL;
	pTemp = (DtFullDataWrap_t*)malloc(sizeof(DtFullDataWrap_t));
	return pTemp;
}

void FreeDataWrap(DtFullDataWrap_t* pWrap)
{
	if (pWrap)
	{
		DT_FREE(pWrap->pData);
		DT_FREE(pWrap);
	}
}

static void FreeInfoDataWrap(DtDataWrap_t* pWrap)
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

// CSocketThread
IMPLEMENT_DYNCREATE(CSocketThread, CWinThread)

CSocketThread::CSocketThread()
	: m_hSocket()
	, m_hWnd()
	, m_bLoop(true)
	, m_pThread(NULL)
{
}

CSocketThread::~CSocketThread()
{
}

BOOL CSocketThread::InitInstance()
{
	// TODO:  여기에서 각 스레드에 대한 초기화를 수행합니다.
	//m_ClientSocket.Attach(m_hSocket);
	//m_ClientSocket.m_pThread = this;
	//m_ClientSocket.SetWnd(m_hWnd);

	return TRUE;
}

int CSocketThread::ExitInstance()
{
	// TODO:  여기에서 각 스레드에 대한 정리를 수행합니다.
	m_bLoop = false;

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSocketThread, CWinThread)
	ON_THREAD_MESSAGE(DT_START_MESSAGE_EXCHANGE, &CSocketThread::OnStartMessageExchange)
	ON_THREAD_MESSAGE(DT_STOP_MESSAGE_EXCHANGE, &CSocketThread::OnStopMessageExchange)
END_MESSAGE_MAP()


// CSocketThread 메시지 처리기입니다.
void CSocketThread::SetWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}

int CSocketThread::Run()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	/*
	BYTE szRecvBuffer[DT_RECEIVE_BUFFER_LENGTH] = { 0 };
	char szSendBuffer[DT_SEND_BUFFER_LENGTH] = { 0 };
	BYTE* pBuffer = NULL;
	DtDataWrap_t* pInfoData = NULL;
	CString message = "";
	int	cbReceived = 0;
	int cbSent = 0;
	//int recv_buffer_size = 0;
	int send_buffer_size = 0;
	int buffer_size = 0;
	int nInvalid = 0;
	int ret = 0;
	int nReceiveOk = 0;
	int nGotoTop = 0;

	//client_socket_handle = pDlg->m_socket.Detach();
	//client_socket.Attach(client_socket_handle);

	while (m_bLoop)
	{
		///< send packet
		sprintf_s(szSendBuffer, sizeof(szSendBuffer), "ok");
		send_buffer_size = strlen(szSendBuffer);
		cbSent = m_ClientSocket.Send(szSendBuffer, send_buffer_size);
		if ((cbSent == SOCKET_ERROR) ||
			(cbSent != send_buffer_size))
		{
			//message.Format("데이터를 수신중 에러가 발생했습니다.(size:%d, error:%d).", cbSent, GetLastError());
			//pDlg->PostLog(message);
			m_bLoop = false;
			continue;
		}

		///< Receive packet
		while (!nReceiveOk)
		{
			m_ClientSocket.SetTimeout(10000);
			cbReceived = m_ClientSocket.Receive(szRecvBuffer, sizeof(szRecvBuffer));
			if (cbReceived == SOCKET_ERROR)
			{
				//message.Format("데이터를 수신중 에러가 발생했습니다.(size:%d, error:%d).", cbSent, GetLastError());
				//pDlg->PostLog(message);
				m_ClientSocket.KillTimeout();
				m_bLoop = false;
				nInvalid = 1;
				nGotoTop = 1;

				break;
			}
			m_ClientSocket.KillTimeout();

			ret = MergePacket(&pBuffer, buffer_size, szRecvBuffer, cbReceived);
			if ((ret == 0) && (pBuffer) && (buffer_size > 0))
			{
				if ((pBuffer[DT_PACKET_STX_POS] == DT_PACKET_STX) &&
					(pBuffer[buffer_size - 1] == DT_PACKET_ETX))
				{
					nReceiveOk = 1;
				}
			}
			else if (ret == 1)
			{
				DT_FREE(pBuffer);
			}
			else
			{
				; ///< Ignore other errors.
			} // end if (ret == DT_STATUS_OK)
		} // end while (!nReceiveOk)

		if (nGotoTop == 1)
		{
			nGotoTop = 0;
			continue;
		}

		if ((pBuffer == NULL) || (buffer_size <= 0))
		{
			DT_FREE(pBuffer);
			m_bLoop = false;
			nInvalid = 1;
			continue;
		}

		ret = ParsePacket(pBuffer, buffer_size, &pInfoData);
		if (ret == 0) // succeeded
		{
			; // pDlg->PostMessage(DT_RECEIVED_MESSAGE, 0, (LPARAM)pInfoData);
		}
		else
		{
			DT_FREE(pBuffer);
			m_bLoop = false;
			nInvalid = 1;
			continue;
		}
	} // end while (m_bLoop)

	//*/
	return CWinThread::Run();
}

int CSocketThread::MergePacket(BYTE** ppTargetBinary, int& nTargetSize, BYTE* pSourceBinary, int nSourceSize)
{
	int ret = 0;
	BYTE* pTemp = NULL;
	int temp_size = 0;

	if ((ppTargetBinary == NULL) ||
		(pSourceBinary == NULL) ||
		(nTargetSize < 0) ||
		(nSourceSize <= 0))
	{
		return 1;
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
			ret = 1;
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
				ret = 1;
			}
			DT_FREE(pTemp);
			temp_size = 0;
		}
		else
		{
			ret = 1;
		} // end if (pTemp != NULL)
	} // end if (pBuffer == NULL)

	return ret;
}

int CSocketThread::ParsePacket(BYTE* pBuffer, int buffer_size, DtDataWrap_t** ppInfoWrap)
{
	WORD_TO_BYTES word_byte = { 0 };
	UINT_TO_BYTES uint_bytes = { 0 };
	uint_and_float uint_and_float = { 0 };
	int data_read_pos = 0;
	int full_length = 0;
	int data_size = 0;
	int invalid_data_packet = 0;
	BYTE* pData = NULL;
	DtDetectedArea_t* pInfoItem = NULL;
	DtDetectedArea_t* pInfoNextStore = NULL;

	if ((pBuffer == NULL) ||
		(buffer_size <= 0) ||
		(ppInfoWrap == NULL))
	{
		return 1;
	}

	if ((pBuffer[DT_PACKET_STX_POS] != DT_PACKET_STX) ||
		(pBuffer[buffer_size - 1] != DT_PACKET_ETX))
	{
		return 1;
	}

	*ppInfoWrap = (DtDataWrap_t*)malloc(sizeof(DtDataWrap_t));
	if (*ppInfoWrap == NULL)
	{
		return 1;
	}
	memset(*ppInfoWrap, 0x00, sizeof(DtDataWrap_t));

	///< full length
	memcpy(uint_bytes.buffer, pBuffer + DT_PACKET_FULL_LENGTH_POS, sizeof(UINT));
	full_length = ntohl(uint_bytes.detail.value);
	if (full_length != buffer_size)
	{
		FreeInfoDataWrap(*ppInfoWrap);
		return 1;
	}

	///< data size
	data_size = full_length - DT_PACKET_STX_SIZE - DT_PACKET_FULL_LENGTH_SIZE - DT_PACKET_ETX_SIZE;
	if (data_size < 1)
	{
		FreeInfoDataWrap(*ppInfoWrap);
		return 1;
	}

	///< steering_angle
	data_read_pos = DT_PACKET_DATA_POS;
	memcpy(uint_bytes.buffer, pBuffer + data_read_pos, sizeof(UINT));
	uint_and_float.uValue = ntohl(uint_bytes.detail.value);
	(*ppInfoWrap)->steering_angle = uint_and_float.fValue;
	data_read_pos += sizeof(UINT);
	IS_VALID_LENGTH(full_length, data_read_pos, invalid_data_packet);
	if (invalid_data_packet)
	{
		FreeInfoDataWrap(*ppInfoWrap);
		return 1;
	}

	///< speed
	memcpy(uint_bytes.buffer, pBuffer + data_read_pos, sizeof(UINT));
	(*ppInfoWrap)->speed = ntohl(uint_bytes.detail.value);
	data_read_pos += sizeof(UINT);
	IS_VALID_LENGTH(full_length, data_read_pos, invalid_data_packet);
	if (invalid_data_packet)
	{
		FreeInfoDataWrap(*ppInfoWrap);
		return 1;
	}

	///< detected count
	memcpy(uint_bytes.buffer, pBuffer + data_read_pos, sizeof(UINT));
	(*ppInfoWrap)->detected_count = ntohl(uint_bytes.detail.value);
	data_read_pos += sizeof(UINT);
	IS_VALID_LENGTH(full_length, data_read_pos, invalid_data_packet);
	if (invalid_data_packet)
	{
		FreeInfoDataWrap(*ppInfoWrap);
		return 1;
	}

	for (int idx = 0; idx < (*ppInfoWrap)->detected_count; ++idx)
	{
		pInfoItem = (DtDetectedArea_t*)malloc(sizeof(DtDetectedArea_t));
		if (pInfoItem == NULL)
		{
			FreeInfoDataWrap(*ppInfoWrap);
			return 1;
		}
		memset(pInfoItem, 0x00, sizeof(DtDetectedArea_t));

		memcpy(uint_bytes.buffer, pBuffer + data_read_pos, sizeof(UINT));
		pInfoItem->x = ntohl(uint_bytes.detail.value);
		data_read_pos += sizeof(UINT);
		IS_VALID_LENGTH(full_length, data_read_pos, invalid_data_packet);
		if (invalid_data_packet)
		{
			DT_FREE(pInfoItem);
			FreeInfoDataWrap(*ppInfoWrap);
			return 1;
		}

		memcpy(uint_bytes.buffer, pBuffer + data_read_pos, sizeof(UINT));
		pInfoItem->y = ntohl(uint_bytes.detail.value);
		data_read_pos += sizeof(UINT);
		IS_VALID_LENGTH(full_length, data_read_pos, invalid_data_packet);
		if (invalid_data_packet)
		{
			DT_FREE(pInfoItem);
			FreeInfoDataWrap(*ppInfoWrap);
			return 1;
		}

		memcpy(uint_bytes.buffer, pBuffer + data_read_pos, sizeof(UINT));
		pInfoItem->w = ntohl(uint_bytes.detail.value);
		data_read_pos += sizeof(UINT);
		IS_VALID_LENGTH(full_length, data_read_pos, invalid_data_packet);
		if (invalid_data_packet)
		{
			DT_FREE(pInfoItem);
			FreeInfoDataWrap(*ppInfoWrap);
			return 1;
		}

		memcpy(uint_bytes.buffer, pBuffer + data_read_pos, sizeof(UINT));
		pInfoItem->h = ntohl(uint_bytes.detail.value);
		data_read_pos += sizeof(UINT);
		IS_VALID_LENGTH(full_length, data_read_pos, invalid_data_packet);
		if (invalid_data_packet)
		{
			DT_FREE(pInfoItem);
			FreeInfoDataWrap(*ppInfoWrap);
			return 1;
		}

		memcpy(uint_bytes.buffer, pBuffer + data_read_pos, sizeof(UINT));
		pInfoItem->cls = ntohl(uint_bytes.detail.value);
		data_read_pos += sizeof(UINT);
		IS_VALID_LENGTH(full_length, data_read_pos, invalid_data_packet);
		if (invalid_data_packet)
		{
			DT_FREE(pInfoItem);
			FreeInfoDataWrap(*ppInfoWrap);
			return 1;
		}

		if (idx == 0)
		{
			(*ppInfoWrap)->detected_info = pInfoItem;
		}
		else
		{
			pInfoNextStore->pNext = pInfoItem;
		}
		pInfoNextStore = pInfoItem;
	}
	///< image cols
	memcpy(uint_bytes.buffer, pBuffer + data_read_pos, sizeof(UINT));
	(*ppInfoWrap)->cols = ntohl(uint_bytes.detail.value);
	data_read_pos += sizeof(UINT);
	IS_VALID_LENGTH(full_length, data_read_pos, invalid_data_packet);
	if (invalid_data_packet)
	{
		FreeInfoDataWrap(*ppInfoWrap);
		return 1;
	}
	///< image rows
	memcpy(uint_bytes.buffer, pBuffer + data_read_pos, sizeof(UINT));
	(*ppInfoWrap)->rows = ntohl(uint_bytes.detail.value);
	data_read_pos += sizeof(UINT);
	IS_VALID_LENGTH(full_length, data_read_pos, invalid_data_packet);
	if (invalid_data_packet)
	{
		FreeInfoDataWrap(*ppInfoWrap);
		return 1;
	}
	
	///< image 
	if ((*ppInfoWrap)->rows > 0) {
		BYTE* image_bytes = (BYTE*)malloc((*ppInfoWrap)->cols * (*ppInfoWrap)->rows * 3);
		char buf[100];
		sprintf_s(buf, sizeof(buf), "ParsePacket>>>>>>>image.cols: %d, image.rows: %d\n", (*ppInfoWrap)->cols, (*ppInfoWrap)->rows);
		OutputDebugString(buf);
		memcpy(image_bytes, pBuffer + data_read_pos+1, (*ppInfoWrap)->cols* (*ppInfoWrap)->rows * 3);
		(*ppInfoWrap)->byte = image_bytes;
		//BYTE tmpbytes[480 * 640 * 3];
		data_read_pos += (*ppInfoWrap)->cols * (*ppInfoWrap)->rows * 3;

		OutputDebugString("\nimage complete!\n");

		IS_VALID_LENGTH(full_length, data_read_pos, invalid_data_packet);
		if (invalid_data_packet)
		{
			FreeInfoDataWrap(*ppInfoWrap);
			return 1;
		}
	}
	
	return 0;
}

int CSocketThread::MakeOnePacket(BYTE** pSendBuffer, int& send_size)
{
	int ret = 0;
	BYTE* pData = NULL;
	DWORD data_size = 0;
	//DWORD data_store_pos = 0;
	//DWORD detect_count = 0;
	//DWORD convert_count = 0;
	UINT_TO_BYTES uint_bytes;
	//DWORD nConvFailed = 0;

	if (pSendBuffer == NULL)
	{
		return 1;
	}

	send_size = DT_PACKET_DATA_SIZE; // "ok"
	data_size = send_size;
	send_size += DT_PACKET_STX_SIZE + DT_PACKET_FULL_LENGTH_SIZE + DT_PACKET_ETX_SIZE;
	*pSendBuffer = (BYTE*)malloc(send_size);
	if (*pSendBuffer)
	{
		(*pSendBuffer)[DT_PACKET_STX_POS] = DT_PACKET_STX;
		(*pSendBuffer)[send_size - 1] = DT_PACKET_ETX;
		uint_bytes.detail.value = htonl(send_size);
		memcpy((*pSendBuffer) + DT_PACKET_FULL_LENGTH_POS, uint_bytes.buffer, sizeof(uint_bytes.buffer));

		pData = (BYTE*)malloc(data_size);
		if (pData)
		{
			//data_store_pos = 0;
			///< "ok"            
			memcpy(pData, DT_PACKET_DATA_RESPONSE, DT_PACKET_DATA_SIZE);
			//data_store_pos += DT_PACKET_DATA_SIZE;
			memcpy((*pSendBuffer) + DT_PACKET_DATA_POS, pData, data_size);
			DT_FREE(pData);
		}
		else
		{
			DT_FREE(*pSendBuffer);
			send_size = 0;
			ret = 1;
		} // end if (pData)
	}
	else
	{
		ret = 1;
	} // end if (*pSendBuffer)
	return ret;
}

void CSocketThread::OnStartMessageExchange(WPARAM wParam, LPARAM lParam)
{
	OutputDebugString("\nOnStartMessageExchange called\n");

	PassData_t* pPassData_t = NULL;

	pPassData_t = (PassData_t*)malloc(sizeof(PassData_t));
	if (pPassData_t == NULL)
	{
		return;
	}
	memset(pPassData_t, 0x00, sizeof(PassData_t));
	pPassData_t->hSocket = m_hSocket;
	pPassData_t->hWnd = m_hWnd;
	pPassData_t->pThread = this;

	m_pThread = AfxBeginThread(Worker, pPassData_t);
	if (m_pThread == NULL)
	{
		DT_FREE(pPassData_t);
		CloseHandle(m_pThread);
		m_pThread = NULL;
	}

	return;
}

void CSocketThread::OnStopMessageExchange(WPARAM wParam, LPARAM lParam)
{
	m_bLoop = false;
	OutputDebugString("\nOnStopMessageExchange called\n");

	gSockClient.CancelBlockingCall();
	return;
}

UINT CSocketThread::Worker(LPVOID pData)
{
	//*
	SOCKET hSocket;
	HWND hWnd = NULL;
	CSocketThread* pParent = NULL;

	if (pData == NULL)
	{
		return 1;
	}
	// CtrackermonitorDlg* pDlg = (CtrackermonitorDlg*)AfxGetApp()->m_pMainWnd;
	PassData_t* pPassData_t = (PassData_t*)pData;
	if ((pPassData_t->hWnd == NULL) || (pPassData_t->pThread == NULL))
	{
		DT_FREE(pData);
		return 1;
	}

	hSocket = pPassData_t->hSocket;
	hWnd = pPassData_t->hWnd;
	pParent = (CSocketThread*)pPassData_t->pThread;
	DT_FREE(pData);

	gSockClient.Attach(hSocket);

	BYTE szRecvBuffer[DT_RECEIVE_BUFFER_LENGTH] = { 0 };
	BYTE* pSendBuffer = NULL;
	BYTE* pBuffer = NULL;
	DtDataWrap_t* pInfoData = NULL;
	CString message = "";
	int	cbReceived = 0;
	int cbSent = 0;
	//int recv_buffer_size = 0;
	int send_buffer_size = 0;
	int buffer_size = 0;
	int nInvalid = 0;
	int ret = 0;
	int nReceiveOk = 0;
	int nGotoTop = 0;

	//client_socket_handle = pDlg->m_socket.Detach();
	//client_socket.Attach(client_socket_handle);

	while (pParent->m_bLoop)
	{
		nInvalid = 0;
		nGotoTop = 0;
        ret = 0;
		///< Make one packet
		ret = pParent->MakeOnePacket(&pSendBuffer, send_buffer_size);
		if (ret != 0)
		{
			DT_FREE(pSendBuffer);
			send_buffer_size = 0;
			pParent->m_bLoop = false;
			nInvalid = 1;
			continue;
		}
		if ((pSendBuffer == NULL) || (send_buffer_size <= 0))
		{
			DT_FREE(pSendBuffer);
			send_buffer_size = 0;
			pParent->m_bLoop = false;
			nInvalid = 1;
			continue;
		}

		///< send packet
		cbSent = gSockClient.Send(pSendBuffer, send_buffer_size);
		if ((cbSent == SOCKET_ERROR) ||
			(cbSent != send_buffer_size))
		{
			/* message.Format("데이터를 수신중 에러가 발생했습니다.(size:%d, error:%d).", cbSent, GetLastError());
			 pDlg->PostLog(message);*/
			DT_FREE(pSendBuffer);
			send_buffer_size = 0;
			pParent->m_bLoop = false;
			nInvalid = 1;
			continue;
		}
		DT_FREE(pSendBuffer);
		send_buffer_size = 0;

		///< Receive packet
		nReceiveOk = 0;
		while (!nReceiveOk)
		{
			cbReceived = gSockClient.Receive(szRecvBuffer, sizeof(szRecvBuffer));
			if (cbReceived == SOCKET_ERROR)
			{
				/* message.Format("데이터를 수신중 에러가 발생했습니다.(size:%d, error:%d).", cbSent, GetLastError());
				 pDlg->PostLog(message);*/
				OutputDebugString("Receive() return errors.\n");
				pParent->m_bLoop = false;
				nInvalid = 1;
				nGotoTop = 1;
				break;
			}
			else if (cbReceived == 0)
			{
				OutputDebugString("Receive() return 0.\n");
				pParent->m_bLoop = false;
				nInvalid = 1;
                nGotoTop = 1;
                break;;
			}

			ret = pParent->MergePacket(&pBuffer, buffer_size, szRecvBuffer, cbReceived);
			if ((ret == 0) && (pBuffer) && (buffer_size > 0))
			{
				if ((pBuffer[DT_PACKET_STX_POS] == DT_PACKET_STX) &&
					(pBuffer[buffer_size - 1] == DT_PACKET_ETX))
				{
                    UINT_TO_BYTES uint_bytes;
                    int full_length = 0;
                    memset(uint_bytes.buffer, 0x00, sizeof(uint_bytes.buffer));
                    memcpy(uint_bytes.buffer, pBuffer + DT_PACKET_FULL_LENGTH_POS, sizeof(UINT));
                    full_length = ntohl(uint_bytes.detail.value);
                    if (full_length == buffer_size)
                    {
                        nReceiveOk = 1;
                    }
				}
			}
			else if (ret == 1)
			{
				DT_FREE(pBuffer);
			}
			else
			{
				; ///< Ignore other errors.
			} // end if (ret == DT_STATUS_OK)
		} // end while (!nReceiveOk)

		if (nGotoTop == 1)
		{
			nGotoTop = 0;
			continue;
		}

		if ((pBuffer == NULL) || (buffer_size <= 0))
		{
			DT_FREE(pBuffer);
			pParent->m_bLoop = false;
			nInvalid = 1;
			continue;
		}

		ret = pParent->ParsePacket(pBuffer, buffer_size, &pInfoData);
		if (ret == 0) // succeeded
		{
			///< TODO  SendMessage UI
			//pParent->PostThreadMessage(DT_RECEIVED_MESSAGE, 0, (LPARAM)pInfoData);
			//OutputDebugString("The message from server was received. So this message will being passed to UI thread.\n");
			SendMessage(hWnd, DT_SEND_RECEIVED_MESSAGE_TO_UI, 0, (LPARAM)pInfoData);  // refer to DT_RECEIVED_MESSAGE
			char buf[200];
			sprintf_s(buf, sizeof(buf), "[received data] steering: %lf, speed: %d, detected_count: %d, image.cols: %d, image.rows: %d\n", 
				pInfoData->steering_angle, pInfoData->speed, pInfoData->detected_count,pInfoData->cols,pInfoData->rows);
			char detectedinfo[1000];
			DtDetectedArea_t* cur = pInfoData->detected_info;
			OutputDebugString(buf);
			/*int count = 1;
			while (cur) {
				sprintf_s(detectedinfo, sizeof(detectedinfo), "%d) x: %d, y: %d, w: %d, h: %d , cls: %d\n", count, cur->x, cur->y, cur->w, cur->h, cur->cls);
				cur = cur->pNext;
			}
			
			OutputDebugString(detectedinfo);*/
			
			
			/*if (pInfoData->rows >0) {
				Mat temp = Mat(pInfoData->rows, pInfoData->cols, CV_8UC3);
				temp.data = pInfoData->byte;
				imshow("window", temp);
			}*/
			
			pInfoData = NULL;
			DT_FREE(pBuffer);
			nInvalid = 0;
		}
		else
		{
            OutputDebugString("parsing error");
			pInfoData = NULL;
			DT_FREE(pBuffer);
			/*pParent->m_bLoop = false;
			nInvalid = 1;*/
			continue;
		}
	} // end while (m_bLoop)

	if (nInvalid)
	{
		DT_FREE(pSendBuffer);
		DT_FREE(pBuffer);
		DT_FREE(pInfoData);
	}

	gSockClient.Close();

	OutputDebugString("Communication thread exit.");
	/*pDlg->DisConnectFromServer();
	message.Format("통신 연결이 종료되었습니다.");
	pDlg->PostLog(message);
	pDlg->PostMessage(DT_UPDATE_CONTROL, (WPARAM)SOCKET_STATUS_NOT_CONNECTED, (LPARAM)0);
	pDlg->PostMessage(DT_UPDATE_HELP, (WPARAM)SOCKET_STATUS_NOT_CONNECTED, (LPARAM)0);*/
	//*/
	return 0;
}



