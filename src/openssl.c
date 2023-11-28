#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <string.h>
//#include <sys/ioctl.h>
//#include <sys/stat.h>
#include <assert.h>
#include <malloc.h>

#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/bio.h"
#include "openssl/aes.h"
#include "openssl/des.h"
#include "openssl/rsa.h"
#include "openssl/pem.h"
#include "openssl/md5.h"

#include "data_list.h"

#include "openssl.h"


#if 0
int md5(const void *buf, const int len, uint8_t _md5[MD5_DIGEST_LENGTH])
{
    int ret = 0;
    MD5_CTX c;

    ret = MD5_Init(&c);
    if (1 != ret)
    {
        return -1;
    }

    ret = MD5_Update(&c, buf, len);
    if (1 != ret)
    {
        return -2;
    }

    ret = MD5_Final(_md5, &c);
    if (1 != ret)
    {
        return -3;
    }

    return 0;
}
#endif

int getPKCS7PaddedLength(int dataLen, int alignSize)
{
    // 计算填充的字节数（按alignSize字节对齐进行填充）
    int remainder = dataLen % alignSize;
    int paddingSize = (remainder == 0) ? alignSize : (alignSize - remainder);
    return (dataLen + paddingSize);
}

uint8_t *PKCS7Padding(uint8_t *in, int len, int *outSize, int alignSize)
{
    // 计算需要填充字节数（按alignSize字节对齐进行填充）
    int remainder = len % alignSize;
    int paddingSize = (remainder == 0) ? alignSize : (alignSize - remainder);

    uint8_t *out = (uint8_t *)malloc(len + paddingSize);
    if (out == NULL)
    {
        printf("%s: malloc error(%d - %s)\n", __func__, errno, strerror(errno));

        return NULL;
    }

    memmove(out, in, len);
    memset(&out[len], paddingSize, paddingSize);

    *outSize = len + paddingSize;

    return out;
}

uint8_t *PKCS7UnPadding(uint8_t *in, int len, int *outSize)
{
    uint8_t paddingSize = in[len - 1];

    *outSize = len - paddingSize;

    return in;
}

// ---- 对称加解密 ---- //
// 加密 ecb模式
uint8_t *aes_encrypt(const uint8_t *inText, const int inSize, int *outSize, const char *key)
{
    if (inText == NULL
            || inSize <= 0
            || key == NULL
            || strlen(key) <= 0)
        return NULL;

    AES_KEY aes_key;
    uint8_t _key[33];

    uint8_t *outbuffer = NULL;

    memset(_key, 0, sizeof(_key));
    if (strlen(key) <= 32)
        memmove(_key, key, strlen(key));

    if (AES_set_encrypt_key((const unsigned char*)_key, 32 * 8, &aes_key) != 0)
    {
        printf("%s: to key error\n", __func__);

        return NULL;
    }

    uint8_t *inbuffer = PKCS7Padding(inText, inSize, outSize, AES_BLOCK_SIZE);
    if (inbuffer == NULL)
    {
        return NULL;
    }

    outbuffer = (uint8_t *)malloc(*outSize + 1);
    if (outbuffer == NULL)
    {
        free(inbuffer);

        return NULL;
    }

    for(int i = 0; i < *outSize / AES_BLOCK_SIZE; i++)
    {
        AES_ecb_encrypt((const uint8_t*)&inbuffer[AES_BLOCK_SIZE * i],
                (uint8_t*)&outbuffer[AES_BLOCK_SIZE * i],
                &aes_key,
                AES_ENCRYPT);
    }

    outbuffer[*outSize] = '\0';

    free(inbuffer);

    return outbuffer;
}

// 解密 ecb模式
uint8_t *aes_decrypt(const uint8_t *inText, const int inSize, int *outSize, const char *key)
{
    //char * inText; // 明文

    if (inText == NULL
            || inSize <= 0
            || key == NULL
            || strlen(key) <= 0)
        return NULL;

    AES_KEY aes_key;
    uint8_t _key[33];

    uint8_t *outbuffer = NULL;

    memset(_key, 0, sizeof(_key));
    if (strlen(key) <= 32)
        memmove(_key, key, strlen(key));

    if (AES_set_decrypt_key((const unsigned char*)_key, 32 * 8, &aes_key) != 0)
    {
        printf("%s: to key error\n", __func__);

        return NULL;
    }

    outbuffer = (uint8_t *)malloc(inSize + 1);
    if (outbuffer == NULL)
    {
        return NULL;
    }

    for(int i = 0; i < inSize / AES_BLOCK_SIZE; i++)
    {
        AES_ecb_encrypt((const uint8_t*)&inText[AES_BLOCK_SIZE * i],
                (uint8_t*)&outbuffer[AES_BLOCK_SIZE * i],
                &aes_key,
                AES_DECRYPT);
    }

    PKCS7UnPadding(outbuffer, inSize, outSize);
    outbuffer[*outSize] = '\0';

    return outbuffer;
}

// ---- des对称加解密 ---- //
// 加密 ecb模式
char* des_encrypt(const char *inText, const int tSzie, int *outSize, const char *key)
{
    //char *inText; // 密文

    if (inText == NULL
            || tSzie <= 0
            || key == NULL
            || strlen(key) <= 0)
        return NULL;

	DES_cblock keyEncrypt;
	memset(keyEncrypt, 0, 8);

	// 构造补齐后的密钥
	if (strlen(key) <= 8)
		memcpy(keyEncrypt, key, strlen(key));
	else
		memcpy(keyEncrypt, key, 8);

	// 密钥置换
	DES_key_schedule keySchedule;
	DES_set_key_unchecked(&keyEncrypt, &keySchedule);

	// 循环加密，每8字节一次
	const_DES_cblock inputText;
    DES_cblock outputText;
    *outSize = (tSzie + 7) / 8 * 8 + 1;
    uint8_t *vecinText = (uint8_t *)malloc(*outSize);
    int offset = 0;
    uint8_t tmp[8];

    for (int i = 0; i < tSzie / 8; i++)
	{
        memcpy(inputText, inText + i * 8, 8);
		DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_ENCRYPT);
		memcpy(tmp, outputText, 8);

		for (int j = 0; j < 8; j++)
                        vecinText[offset++] = tmp[j];
	}

    if (tSzie % 8 != 0)
	{
        int tmp1 = tSzie / 8 * 8;
        int tmp2 = tSzie - tmp1;
		memset(inputText, 0, 8);
        memcpy(inputText, inText + tmp1, tmp2);
		// 加密函数
		DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_ENCRYPT);
		memcpy(tmp, outputText, 8);

		for (int j = 0; j < 8; j++)
                        vecinText[offset++] = tmp[j];
	}

    //inText.clear();
    //inText.assign(vecinText.begin(), vecinText.end());

    *outSize -= 1;
    vecinText[*outSize] = '\0';

    return vecinText; //inText;
}

// 解密 ecb模式
char * des_decrypt(const char *inText, const int tSzie, int *outSize, const char *key)
{
    //char * inText; // 明文

    if (inText == NULL
            || tSzie <= 0
            || key == NULL
            || strlen(key) <= 0)
        return NULL;

	DES_cblock keyEncrypt;
	memset(keyEncrypt, 0, 8);

	if (strlen(key) <= 8)
		memcpy(keyEncrypt, key, strlen(key));
	else
		memcpy(keyEncrypt, key, 8);

	DES_key_schedule keySchedule;
	DES_set_key_unchecked(&keyEncrypt, &keySchedule);

	const_DES_cblock inputText;
    DES_cblock outputText;
    *outSize = (tSzie + 7) / 8 * 8 + 1;
    uint8_t *vecinText = (uint8_t *)malloc(*outSize);
    int offset = 0;
    uint8_t tmp[8];

    for (int i = 0; i < tSzie / 8; i++)
	{
        memcpy(inputText, inText + i * 8, 8);
		DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_DECRYPT);
		memcpy(tmp, outputText, 8);

		for (int j = 0; j < 8; j++)
            vecinText[offset++] = tmp[j];
	}

    if (tSzie % 8 != 0)
	{
        int tmp1 = tSzie / 8 * 8;
        int tmp2 = tSzie - tmp1;
		memset(inputText, 0, 8);
        memcpy(inputText, inText + tmp1, tmp2);
		// 解密函数
		DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_DECRYPT);
		memcpy(tmp, outputText, 8);

		for (int j = 0; j < 8; j++)
            vecinText[offset++] = tmp[j];
	}

    //inText.clear();
    //inText.assign(vecinText.begin(), vecinText.end());

    *outSize -= 1;
    vecinText[*outSize] = '\0';

    return vecinText; //inText;
}

// ---- rsa非对称加解密 ---- //
#define KEY_LENGTH      2048            // 密钥长度

// 函数方法生成密钥对
void generateRSAKey(char **pub_key, char **pri_key)
{
	// 公私密钥对
	size_t pri_len;
	size_t pub_len;
	char *_pub_key = NULL;
	char *_pri_key = NULL;

	// 生成密钥对
	RSA *keypair = RSA_generate_key(KEY_LENGTH, RSA_3, NULL, NULL);

	BIO *pri = BIO_new(BIO_s_mem());
	BIO *pub = BIO_new(BIO_s_mem());

	PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);
	PEM_write_bio_RSAPublicKey(pub, keypair);

	// 获取长度
	pri_len = BIO_pending(pri);
	pub_len = BIO_pending(pub);

	// 密钥对读取到字符串
	_pri_key = (char *)malloc(pri_len + 1);
	_pub_key = (char *)malloc(pub_len + 1);

	BIO_read(pri, _pri_key, pri_len);
	BIO_read(pub, _pub_key, pub_len);

	_pri_key[pri_len] = '\0';
	_pub_key[pub_len] = '\0';

	// 存储密钥对
	*pub_key = _pub_key;
	*pri_key = _pri_key;

#if 1
	// 存储到磁盘（这种方式存储的是begin rsa public key/ begin rsa private key开头的）
    FILE *pubFile = fopen(PUB_KEY_FILE, "w");
	if (pubFile == NULL)
	{
        // 内存释放
        RSA_free(keypair);
        BIO_free_all(pub);
        BIO_free_all(pri);

		//assert(false);
		return;
	}
	fputs(_pub_key, pubFile);
	fclose(pubFile);

	FILE *priFile = fopen(PRI_KEY_FILE, "w");
	if (priFile == NULL)
	{
        // 内存释放
        RSA_free(keypair);
        BIO_free_all(pub);
        BIO_free_all(pri);

		//assert(false);
		return;
	}
	fputs(_pri_key, priFile);
	fclose(priFile);
#endif

	// 内存释放
	RSA_free(keypair);
	BIO_free_all(pub);
	BIO_free_all(pri);
}

// 命令行方法生成公私钥对（begin public key/ begin private key）
// 找到openssl命令行工具，运行以下
// openssl genrsa -out prikey.pem 1024
// openssl rsa - in privkey.pem - pubout - out pubkey.pem
//
//公钥加密
uint8_t *rsa_pub_encrypt(const uint8_t *inText, const int inSize, int *outSize, const char *pubKey)
{
    uint8_t *strRet = NULL;
    RSA *rsa = NULL;

    if (inText == NULL
            || inSize <= 0
            || outSize == NULL
            || pubKey == NULL
            || strlen(pubKey) <= 0)
        return NULL;

    LIST_HEAD(tmp_list);
    struct data_list_t *pdata = NULL;

    BIO *keybio = BIO_new_mem_buf((uint8_t *)pubKey, -1);

    // 此处有三种方法
    // 1, 读取内存里生成的密钥对，再从内存生成rsa
    // 2, 读取磁盘里生成的密钥对文本文件，在从内存生成rsa
    // 3，直接从读取文件指针生成rsa
    rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);

    int keySize = RSA_size(rsa);
    uint8_t *tmpBuffer = (uint8_t *)malloc(keySize + 1);

    int pos = 0;
    int last = inSize;
    while(last > 0)
    {
        memset(tmpBuffer, 0, keySize + 1);

        int select = keySize - RSA_PKCS1_PADDING_SIZE;
        if (select > last)
            select = last;

        // 加密函数
        int ret = RSA_public_encrypt(select, (const uint8_t*)&inText[pos], (uint8_t*)tmpBuffer, rsa, RSA_PKCS1_PADDING);
        if (ret < 0)
            break;

        data_list_add((uint8_t *)tmpBuffer, ret, &tmp_list);

        pos += select;
        last -= select;
    }

    // 释放内存
    free(tmpBuffer);
    BIO_free_all(keybio);
    RSA_free(rsa);

    *outSize = data_list_len(&tmp_list);

    if (*outSize > 0)
    {
        strRet = malloc((*outSize) + 1);
        if (strRet == NULL)
        {
            printf("%s: malloc error(%d - %s)\n", __func__, errno, strerror(errno));

            return NULL;
        }

        data_list_dump(&tmp_list, 0, strRet, *outSize);
        strRet[*outSize] = '\0';

        while (! list_empty(&tmp_list))
        {
            pdata = list_first_entry(&tmp_list, struct data_list_t, entry);

            list_del(&pdata->entry);

            if (pdata->data)
                free(pdata->data);
            free(pdata);
        }
    }

    return strRet;
}

// 公钥解密
uint8_t *rsa_pub_decrypt(const uint8_t *inText, const int inSize, int *outSize, const char *pubKey)
{  
    uint8_t *strRet = NULL;
    RSA *rsa = NULL;

    if (inText == NULL
            || inSize <= 0
            || outSize == NULL
            || pubKey == NULL
            || strlen(pubKey) <= 0)
        return NULL;

    LIST_HEAD(tmp_list);
    struct data_list_t *pdata = NULL;

    BIO *keybio = BIO_new_mem_buf((uint8_t *)pubKey, -1);

	// 此处有三种方法  
	// 1, 读取内存里生成的密钥对，再从内存生成rsa  
	// 2, 读取磁盘里生成的密钥对文本文件，在从内存生成rsa  
	// 3，直接从读取文件指针生成rsa   
	rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);  

    int keySize = RSA_size(rsa);
    uint8_t *tmpBuffer = (uint8_t *)malloc(keySize + 1);

    int pos = 0;
    int last = inSize;
    while(last > 0)
    {
        memset(tmpBuffer, 0, keySize + 1);

        // 解密函数
        int ret = RSA_public_decrypt(keySize, (const uint8_t*)&inText[pos], (uint8_t*)tmpBuffer, rsa, RSA_PKCS1_PADDING);
        if (ret < 0)
            break;

        data_list_add((uint8_t *)tmpBuffer, ret, &tmp_list);

        pos += keySize;
        last -= keySize;
    }

	// 释放内存  
    free(tmpBuffer);
	BIO_free_all(keybio);  
    RSA_free(rsa);

    *outSize = data_list_len(&tmp_list);

    if (*outSize > 0)
    {
        strRet = malloc((*outSize) + 1);
        if (strRet == NULL)
        {
            printf("%s: malloc error(%d - %s)\n", __func__, errno, strerror(errno));

            return NULL;
        }

        data_list_dump(&tmp_list, 0, strRet, *outSize);
        strRet[*outSize] = '\0';

        while (! list_empty(&tmp_list))
        {
            pdata = list_first_entry(&tmp_list, struct data_list_t, entry);

            list_del(&pdata->entry);

            if (pdata->data)
                free(pdata->data);
            free(pdata);
        }
    }

	return strRet;  
}  

// 私钥加密
uint8_t *rsa_pri_encrypt(const uint8_t *inText, const int inSize, int *outSize, const char *priKey)
{  
    uint8_t * strRet = NULL;

    if (inText == NULL
            || inSize <= 0
            || outSize == NULL
            || priKey == NULL
            || strlen(priKey) <= 0)
        return NULL;

    LIST_HEAD(tmp_list);
    struct data_list_t *pdata = NULL;

	RSA *rsa = RSA_new();  
    BIO *keybio;
    keybio = BIO_new_mem_buf((uint8_t *)priKey, -1);

	// 此处有三种方法  
	// 1, 读取内存里生成的密钥对，再从内存生成rsa  
	// 2, 读取磁盘里生成的密钥对文本文件，在从内存生成rsa  
	// 3，直接从读取文件指针生成rsa  
	rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);  

    int keySize = RSA_size(rsa);
    uint8_t *tmpBuffer = (uint8_t *)malloc(keySize + 1);

    int pos = 0;
    int last = inSize;
    while(last > 0)
    {
        memset(tmpBuffer, 0, keySize + 1);

        int select = keySize - RSA_PKCS1_PADDING_SIZE;
        if (select > last)
            select = last;

        // 加密函数
        int ret = RSA_private_encrypt(select, (const uint8_t*)&inText[pos], (uint8_t*)tmpBuffer, rsa, RSA_PKCS1_PADDING);
        if (ret < 0)
            break;

        data_list_add((uint8_t *)tmpBuffer, ret, &tmp_list);

        pos += select;
        last -= select;
    }

	// 释放内存  
    free(tmpBuffer);
	BIO_free_all(keybio);  
	RSA_free(rsa);

    *outSize = data_list_len(&tmp_list);

    if (*outSize > 0)
    {
        strRet = malloc((*outSize) + 1);
        if (strRet == NULL)
        {
            printf("%s: malloc error(%d - %s)\n", __func__, errno, strerror(errno));

            return NULL;
        }

        data_list_dump(&tmp_list, 0, strRet, *outSize);
        strRet[*outSize] = '\0';

        while (! list_empty(&tmp_list))
        {
            pdata = list_first_entry(&tmp_list, struct data_list_t, entry);

            list_del(&pdata->entry);

            if (pdata->data)
                free(pdata->data);
            free(pdata);
        }
    }

	return strRet;  
}  

//私钥解密
uint8_t *rsa_pri_decrypt(const uint8_t *inText, const int inSize, int *outSize, const char *priKey)
{
    uint8_t * strRet = NULL;

    if (inText == NULL
            || inSize <= 0
            || outSize == NULL
            || priKey == NULL
            || strlen(priKey) <= 0)
        return NULL;

    LIST_HEAD(tmp_list);
    struct data_list_t *pdata = NULL;

    RSA *rsa = RSA_new();
    BIO *keybio;
    keybio = BIO_new_mem_buf((uint8_t *)priKey, -1);

    // 此处有三种方法
    // 1, 读取内存里生成的密钥对，再从内存生成rsa
    // 2, 读取磁盘里生成的密钥对文本文件，在从内存生成rsa
    // 3，直接从读取文件指针生成rsa
    rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);

    int keySize = RSA_size(rsa);
    uint8_t *tmpBuffer = (uint8_t *)malloc(keySize + 1);
    memset(tmpBuffer, 0, keySize + 1);

    int pos = 0;
    int last = inSize;
    while(last > 0)
    {
        memset(tmpBuffer, 0, keySize + 1);

        // 解密函数
        int ret = RSA_private_decrypt(keySize, (const uint8_t*)&inText[pos], (uint8_t*)tmpBuffer, rsa, RSA_PKCS1_PADDING);
        if (ret < 0)
            break;

        data_list_add((uint8_t *)tmpBuffer, ret, &tmp_list);

        pos += keySize;
        last -= keySize;
    }

    // 释放内存
    free(tmpBuffer);
    BIO_free_all(keybio);
    RSA_free(rsa);

    *outSize = data_list_len(&tmp_list);

    if (*outSize > 0)
    {
        strRet = malloc((*outSize) + 1);
        if (strRet == NULL)
        {
            printf("%s: malloc error(%d - %s)\n", __func__, errno, strerror(errno));

            return NULL;
        }

        data_list_dump(&tmp_list, 0, strRet, *outSize);
        strRet[*outSize] = '\0';

        while (! list_empty(&tmp_list))
        {
            pdata = list_first_entry(&tmp_list, struct data_list_t, entry);

            list_del(&pdata->entry);

            if (pdata->data)
                free(pdata->data);
            free(pdata);
        }
    }

    return strRet;
}

//int descrypt(const char *text)
/*int main(int argc, char *argv[])
{
	char *text = "0xe2af81053be01be7";

	char *encryptText;
	char *decryptText;

	//SSL_library_init();

	// rsa
	char *pub_key = NULL;
	char *pri_key = NULL;

	generateRSAKey(&pub_key, &pri_key);
	printf("Pub:[%s]\n", pub_key);
	printf("Private:[%s]\n", pri_key);

	encryptText = rsa_pri_encrypt(text, pri_key);
	printf("encryptText: [%s]\n", encryptText);

	decryptText = rsa_pub_decrypt(encryptText, pub_key);
	printf("decryptText: [%s]\n", decryptText);

	//des
	printf("DES:\n");

	char desKey[] = "12345";
	encryptText = des_encrypt(text, desKey);
	printf("encryptText: [%s]\n", encryptText);

	decryptText = des_decrypt(encryptText, desKey);
	printf("decryptText: [%s]\n", decryptText);

	return 0;
}*/
