#ifndef _NDI_LICENSE_H
#define _NDI_LICENSE_H


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief get_ndi_license_state
 * @return <0: 失败; 0: 成功
 */
int get_ndi_license_state();

/**
 * @brief get_ndi_license
 * @return <0: 失败; >0: 文件长度
 */
int get_ndi_license();

#ifdef __cplusplus
}
#endif

#endif //_NDI_LICENSE_H
