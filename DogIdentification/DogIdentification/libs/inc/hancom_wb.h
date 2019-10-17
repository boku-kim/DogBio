#ifndef _APIS_H_
#define _APIS_H_

#if defined(__cplusplus)
extern "C" {
#endif

    /*
    * @brief : Load white box table from buffer
    * @param [in, optional] : pWbEncryptableBuff; If Not NULL, whitebox talbe for ciphering will be loaded.
    * @param [in, optional] : pWbDecryptTableBuff; If Not NULL, whitebox talbe for deciphering will be loaded.
    * @return : return 0 on SUCCESS or FAILED.
    * @note : If you cipher or decipher contents, you can proceed to next after calling this function.
    */
    int set_wb128_table_by_buff(const BYTE* pWbEncryptableBuff, DWORD enc_buff_size, const BYTE* pWbDecryptTableBuff, DWORD dec_buff_size);

    /*
    * @brief : reset the whitebox table in memory.
    * @return : return 0 on SUCCESS or FAILED.
    */
    int reset_wb128_table();

    /*
    * @brief : encrypt contents by white box in aes128 cbc mode 
    * @param [in pSource] : Plain Data; This will be encrypted.
    * @param [in source_size] : Plain Data size; How many data should be encrypted?.
    * @param [out ppTarget] : encryption result will be copyed to ppTarget.
    * @param [out target_size] : The size of encrypted result will be set.
    * @return : return 0 on SUCCESS or FAILED.
    * @Notice : ppTarget will be allocated by encrypt_wb128_cbc. So you MUST free allocated memory.
    */
    int encrypt_wb128_cbc(const BYTE* pSource, DWORD source_size, BYTE** ppTarget, DWORD& target_size);

    /*
    * @brief : encrypt contents by white box in aes128 cbc mode
    * @param [in out pBuff] : Encrypted Data
    * @param [in out buff_size] : Encrypted data size; How many data should be decrypted?.
    * @return : return 0 on SUCCESS or FAILED.
    * @Notice : decrypted result and result size will be set to pBuff, buff_size.
    *           So decrypted result is valid only if function's result is 0.
    */
    int decrypt_wb128_cbc(const BYTE* pBuff, DWORD& buff_size);

#if defined(__cplusplus)
}
#endif

#endif /* _APIS_H_ */