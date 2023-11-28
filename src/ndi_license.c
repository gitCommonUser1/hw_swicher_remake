#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "hw_config.h"
#include "device_info.h"
#include "openssl.h"
#include "md5.h"

#include "files.h"

#include "osee_errors.h"
#include "osee_syslog.h"

#include "ndi_license.h"


#define NDI_PATH    DATA_PATH
#define NDI_SUFFIX  ".ndi"

#define SN_VALID_LEN    9


const char pub_key[] = "-----BEGIN RSA PUBLIC KEY-----\n"
        "MIIBCAKCAQEA9ZW/hNaRDPu4XMaO/pUXK54acHrYvR3xTC+h5k7lIg4wy87K8K3J\n"
        "77WZxzy4ep7wBebNtb353lW305ULUnuX5mJ2/Kmr/rsFow2Rb/HnUjS3z+j2Jt5Q\n"
        "FDZinym7e81xYxfLYo3QM8M4g7yws7DOeYPQB6QS9FMweJ1paoCMIfzeepaehrJK\n"
        "k2OkSwcr63hC+7VDU5Wo9IJfVI30I7pYj2nVbXQXeNRhgXSmg8ohk+oHZPJ4cTms\n"
        "BMhi57vWRvvlwrZYFgTxYFOrl7TxF4sj6JD9OpDCb4p/YQSuZ2yqsz5UWetlZWAh\n"
        "hkxDG/1GlnOEeN0fqhuLrtajJmwTxDrxYwIBAw==\n"
        "-----END RSA PUBLIC KEY-----\n";


char *check_xml_head(const char *text)
{
	char *p = NULL;

	p = strstr(text, "<GoStreamLicense>");
	if (p == NULL)
		return NULL;

	p += 17;

	return p;
}

char *get_xml_element(const char *text, const char *element)
{
	char *p = NULL;
	char *p_start = NULL;
	char *p_end = NULL;

	if (text == NULL
			|| strlen(text) <= 0)
		return NULL;

	p_end = &text[strlen(text) - 1];

	char *keyword = (char *)malloc(strlen(element) + 4);

	if (keyword == NULL)
		return NULL;

    snprintf(keyword, malloc_usable_size(keyword), "<%s>", element);

	p_start = strstr(text, keyword);
	if (p_start == NULL)
	{
		free(keyword);
		return NULL;
	}

#if 0
    p_start = strchr(p_start, '>');
    if ((*p_start) == '>')
	{
        p_start++;
    }
#else
    p_start += strlen(keyword);
#endif

    snprintf(keyword, malloc_usable_size(keyword), "</%s>", element);

    p = strstr(text, keyword);
	if (p == NULL)
		p = p_end + 1;

	if (p == p_start)
	{
		free(keyword);
		return NULL;
	}

	char *value = (char *)malloc(p - p_start + 1);
	memmove(value, p_start, p - p_start);
	value[p - p_start] = '\0';

	free(keyword);

	return value;
}

/**
 * @brief check_ndi_license
 * @param sn
 * @param filename
 * @return
 * <0: 失败;
 * 0: 成功
 * >0: 到期时间
 */
long check_ndi_license(const char *sn, const char *filename)
{
    long ret = 0;

    uint8_t *_rsa_decrypt = NULL;
    uint8_t *_aes_decrypt = NULL;
    int rsa_size = 0;
    int aes_size = 0;

    char *SerialNumber = NULL;
    char *NdiLicense = NULL;
    char *NdiIssue = NULL;
    char *NdiExpires = NULL;

    if (sn == NULL
            || strlen(sn) <= 0
            || filename == NULL
            || strlen(filename) <= 0)
        return -ERROR_PARAM_NULL;

    if (access(filename, R_OK) != 0)
    {
        SYS_ERR("%s: accesss %s error(%d - %s)\n", __func__, filename, errno, strerror(errno));

        return -ERROR_CANNOT_ACCESS;
    }

    long filesize = get_file_size(filename);
    if (filesize <= 0)
    {
        SYS_ERR("%s: filesize %s error(%d - %s)\n", __func__, filename, errno, strerror(errno));

        return -ERROR_NO_EXIST;
    }

    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        SYS_ERR("%s: open %s error(%d - %s)\n", __func__, filename, errno, strerror(errno));

        return -ERROR_OPEN;
    }

    uint8_t *buf = (uint8_t *)mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (buf == NULL)
    {
        SYS_ERR("%s: mmap %s error(%d - %s)\n", __func__, filename, errno, strerror(errno));

        close(fd);
        return -ERROR_MEM;
    }

    _rsa_decrypt = rsa_pub_decrypt(buf, filesize,
                                      &rsa_size, pub_key);

    munmap(buf, filesize);
    close(fd);

    if (_rsa_decrypt == NULL)
    {
        ret = -ERROR_DATA;
        goto CHECK_NDI_ERR;
    }

    _aes_decrypt = aes_decrypt(_rsa_decrypt, rsa_size,
                                       &aes_size,
                                       sn);

    if (_aes_decrypt == NULL)
    {
        ret = -ERROR_DATA;
        goto CHECK_NDI_ERR;
    }

    //printf("[%s]\n", _aes_decrypt);

    ret = 0;

    char *p = check_xml_head((const char *)_aes_decrypt);
    if (p == NULL)
    {
        SYS_ERR("%s: head %s error(%d - %s)\n", __func__, filename, errno, strerror(errno));

        ret = -ERROR_DATA;
        goto CHECK_NDI_ERR;
    }

    SerialNumber = get_xml_element(p, "SerialNumber");
    if (SerialNumber == NULL
            || strcmp(SerialNumber, sn) != 0)
    {
        SYS_ERR("%s: SerialNumber %s error(%d - %s)\n", __func__, filename, errno, strerror(errno));

        ret = -ERROR_DATA;
        goto CHECK_NDI_ERR;
    }

    //printf("SerialNumber:[%s]\n", SerialNumber);

    NdiLicense = get_xml_element(p, "NdiLicense");
    if (NdiLicense == NULL
            || strlen(NdiLicense) < 32)
    {
        SYS_ERR("%s: NdiLicense %s error(%d - %s)\n", __func__, filename, errno, strerror(errno));

        ret = -ERROR_DATA;
        goto CHECK_NDI_ERR;
    }

    //printf("NdiLicense:[%s]\n", NdiLicense);

    uint8_t _md5[16], __md5[16];

    memset(_md5, 0, sizeof (_md5));
    md5(sn, strlen(sn), _md5);

    int i = strlen(NdiLicense);
    if (i > 30)
    {
        i = 30;
    }
    else
    {
        i &= ~1;
    }
    while(i >= 0)
    {
        NdiLicense[i + 2] = '\0';

        int tmp = 0;
        sscanf(&NdiLicense[i], "%x", &tmp);

        __md5[i >> 1] = tmp;

        i -= 2;
    }

    if (memcmp(_md5, __md5, 16) != 0)
    {
        SYS_ERR("%s: NdiLicense %s error(%d - %s)\n", __func__, filename, errno, strerror(errno));

        /*for(i = 0; i < 16; i++)
        {
            printf("%.2x %.2x\n", _md5[i], __md5[i]);
        }*/

        ret = -ERROR_DATA;
        goto CHECK_NDI_ERR;
    }

    //NdiIssue = get_xml_element(p, "NdiIssue");

    NdiExpires = get_xml_element(p, "NdiExpires");
    if (NdiExpires)
    {
        sscanf(NdiExpires, "%ld", &ret);
    }

CHECK_NDI_ERR:
    if (SerialNumber)
    {
        free(SerialNumber);
        SerialNumber = NULL;
    }
    if (NdiLicense)
    {
        free(NdiLicense);
        NdiLicense = NULL;
    }
    if (NdiExpires)
    {
        free(NdiExpires);
        NdiExpires = NULL;
    }

    if (_aes_decrypt)
    {
        free(_aes_decrypt);
        _aes_decrypt = NULL;
    }
    if (_rsa_decrypt)
    {
        free(_rsa_decrypt);
        _rsa_decrypt = NULL;
    }

    return ret;
}

int get_ndi_license_state()
{
    int ret = 0;
    int pos = 0;
    char *sn_valid = NULL;

    if (access(NDI_PATH, F_OK) != 0)
    {
        return -ERROR_NULL;
    }

    char *sn = get_device_id();
    if (sn == NULL)
    {
        return -ERROR_PARAM_NULL;
    }

    pos = strlen(sn) - SN_VALID_LEN;
    if (pos < 0)
        pos = 0;

    sn_valid = (char *)&sn[pos];

    char *filename = (char *)malloc(strlen(NDI_PATH) + strlen(sn_valid) + 2 + 4 + 1);
    if (filename == NULL)
    {
        ret = -ERROR_MEM;
        goto GET_NDI_STATE_EXIT;
    }

    snprintf(filename, malloc_usable_size(filename), "%s/%s%s", NDI_PATH, sn_valid, NDI_SUFFIX);

    long state = check_ndi_license(sn_valid, filename);
    if (state <= 0)
    {
        ret = state;
    }
    else
    {
        time_t _cur = time(NULL);
        if (state < _cur)
        {
            ret = -ERROR_OUTRANGE;
        }
        else
        {
            ret = 0;
        }
    }

GET_NDI_STATE_EXIT:
    if (filename)
    {
        free(filename);
        filename = NULL;
    }

    if (sn)
    {
        free(sn);
        sn = NULL;
    }

    return ret;
}

int get_ndi_license()
{
    int ret = 0;
    int pos = 0;
    char *sn_valid = NULL;

    char *filename = NULL;
    char *filename1 = NULL;

    if (! is_path_mount(SD_MOUNTED_PATH))
    {
        return -ERROR_NULL;
    }

    char *sn = get_device_id();
    if (sn == NULL)
    {
        return -ERROR_PARAM_NULL;
    }

    pos = strlen(sn) - SN_VALID_LEN;
    if (pos < 0)
        pos = 0;

    sn_valid = (char *)&sn[pos];

    filename = (char *)malloc(strlen(SD_MOUNTED_PATH) + strlen(sn_valid) + 2 + 4 + 1);
    if (filename == NULL)
    {
        ret = -ERROR_MEM;
        goto GET_NDI_LICENSE_EXIT;
    }

    snprintf(filename, malloc_usable_size(filename), "%s/%s%s", SD_MOUNTED_PATH, sn_valid, NDI_SUFFIX);

    long state = check_ndi_license(sn_valid, filename);
    if (state < 0)
    {
        ret = state;
        goto GET_NDI_LICENSE_EXIT;
    }

    filename1 = (char *)malloc(strlen(NDI_PATH) + strlen(sn_valid) + 2 + 4 + 1);
    if (filename1 == NULL)
    {
        ret = -ERROR_MEM;
        goto GET_NDI_LICENSE_EXIT;
    }

    snprintf(filename1, malloc_usable_size(filename1), "%s/%s%s", NDI_PATH, sn_valid, NDI_SUFFIX);

    if (0 != filecmp(filename, filename1))
    {
        ret = filecp(filename, filename1);
        if (ret != get_file_size(filename))
        {
            ret = ERROR_WRITE;
        }
    }

GET_NDI_LICENSE_EXIT:
    if (sn)
    {
        free(sn);
        sn = NULL;
    }
    if (filename)
    {
        free(filename);
        filename = NULL;
    }
    if (filename1)
    {
        free(filename1);
        filename1 = NULL;
    }

    return ret;
}
