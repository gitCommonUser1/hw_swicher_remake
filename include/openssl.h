#ifndef __SSL_H
#define __SSL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


#define PUB_KEY_FILE    ".pubkey.pem"    // 公钥路径
#define PRI_KEY_FILE    ".prikey.pem"    // 私钥路径

#define PUB_KEY         "pub"    // 公钥
#define PRI_KEY         "pri"    // 私钥


//int md5(const void *buf, const int len, uint8_t _md5[16]);

/**
 * 对称加解密
 */
// 加密 aes模式
uint8_t *aes_encrypt(const uint8_t *inText, const int inSize, int *outSize, const char *key);

// 解密 aes模式
uint8_t *aes_decrypt(const uint8_t *inText, const int inSize, int *outSize, const char *key);

// 加密 ecb模式
char* des_encrypt(const char *clearText, const int tSzie, int *vecSize, const char *key);

// 解密 ecb模式
char * des_decrypt(const char *cipherText, const int tSzie, int *vecSize, const char *key);

/**
 * RSA非对称加解密
 */
// 函数方法生成密钥对
void generateRSAKey(char **pub_key, char **pri_key);

// 私钥加密
uint8_t *rsa_pri_encrypt(const uint8_t *inText, const int inSize, int *outSize, const char *priKey);

//私钥解密
uint8_t *rsa_pri_decrypt(const uint8_t *inText, const int inSize, int *outSize, const char *priKey);

//公钥加密
uint8_t *rsa_pub_encrypt(const uint8_t *inText, const int inSize, int *outSize, const char *pubKey);

// 公钥解密
uint8_t *rsa_pub_decrypt(const uint8_t *inText, const int inSize, int *outSize, const char *pubKey);

#ifdef __cplusplus
}
#endif

#endif
