#ifndef _ERROR_DEFINE_H_
#define _ERROR_DEFINE_H_

typedef enum MODULE_ERROR
{
    eMOD_ERROR_SUCCESS = 0,
    eMOD_ERROR_FAIL,
    eMOD_ERROR_ARGUMENT_INVALID,
    eMOD_ERROR_MEMORY_ERROR,
    eMOD_ERROR_INSTANCE_NULL,
    eMOD_ERROR_ENCRYPT_FAILED,
    eMOD_ERROR_DECRYPT_FAILED, 
    eMOD_ERROR_WRITE_FILE_FAILED,
    eMOD_ERROR_READ_FILE_FAILED, 
    eMOD_ERROR_MAX
} eModError;


#endif /* _ERROR_DEFINE_H_ */
