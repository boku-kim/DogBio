/**
****************************************************************************************************
* @file sys_error.h
* @brief 
* @author 
* @date Created
* @see Coding standard guideline
****************************************************************************************************
*/
#ifndef _SYS_ERROR_H_
#define _SYS_ERROR_H_

typedef enum eDT_STATUS
{
    DT_STATUS_OK = 0,
    DT_STATUS_FAIL,
    DT_STATUS_INVALID_ARGS,
    DT_STATUS_EVENT_LOOP_INVALID,
    DT_STATUS_FILE_OPEN_ERROR,
    DT_STATUS_FILE_WRITE_FAILED,
    DT_STATUS_FILE_READ_FAILED,
    DT_STATUS_SOCKET_CREATE_FAILED,
    DT_STATUS_SOCKET_FCNTL_FAILED,
    DT_STATUS_SOCKET_BIND_FAILED,
    DT_STATUS_SOCKET_LISTEN_FAILED,
    DT_STATUS_PACKET_INVALID_SIGN,
    DT_STATUS_PACKET_INVALID_FULL_LENGTH,
    DT_STATUS_PACKET_INVALID_DATA_LENGTH,
    DT_STATUS_PACKET_CONVERT_FAILED,
    DT_STATUS_MUTEX_ITEM_NULL,
    DT_STATUS_QUEUE_GET_ITEM_FAILED,
    DT_STATUS_MEM_ERROR,
    DT_STATUS_MAX
} DT_STATUS;


#endif /* _SYS_ERROR_H_ */
