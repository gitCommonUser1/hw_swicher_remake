#ifndef _ERRORS_H
#define _ERRORS_H

#include <string.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
  * ERRORs
  */
#define RESULT_OK                               0

#define ERROR_NO_ENUOUGH_SPACE		81

#define ERROR_CMD_UNKNOWN		82
#define ERROR_CMD_UNSUPPORT		83
#define ERROR_CMD_SHORT			84
#define ERROR_CMD_XOR			85
#define ERROR_CMD_LONG			86

#define ERROR_OUTRANGE          87

#define ERROR_NULL              88

#define ERROR_CANNOT_ACCESS                     89
#define ERROR_OPEN                              90
#define ERROR_READ                              91
#define ERROR_WRITE                             92
#define ERROR_EXEC                              93

#define ERROR_DATA                              94
#define ERROR_MEM                               95

#define ERROR_SOCKET                            96
#define ERROR_IOCTL                             97
#define ERROR_BIND                              98
#define ERROR_LISTEN                            99
#define ERROR_CONNECT                           100

#define ERROR_PARAM_NULL                        101
#define ERROR_NO_EXIST                          102

#define ERROR_TIMEOUT                           103

#define ERROR_DISCONNECT			104


#define CODE_HEX2DEC(n)					((n) - 0x100)

#ifdef __cplusplus
}
#endif

#endif //_ERRORS_H
