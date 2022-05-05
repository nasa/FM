#ifndef _fm_test_utils_h_
#define _fm_test_utils_h_

#include "cfe.h"

#include "common_types.h"

#include "fm_platform_cfg.h"
#include "fm_app.h"
#include "utstubs.h"

typedef struct {
    uint16 EventID;
    uint16 EventType;
    const char *Spec;
} __attribute__((packed)) CFE_EVS_SendEvent_context_t;

#define MAX_UINT8  255                  /* pow(2, 8) - 1 */
#define MAX_UINT16 65535                /* pow(2, 16) - 1 */
#define MAX_INT    2147484647           /* pow(2, 31) - 1 */
#define MIN_INT32  -2147484648          /* negative pow(2, 31)*/
#define MAX_UINT32 4294967295           /* pow(2, 32) - 1 */
#define MAX_UINT64 18446744073709551615 /* pow(2, 64) - 1 */

uint32 UT_Utils_Any_uint32(void);

uint32 UT_Utils_Any_uint32_GreaterThan(uint32 floor);

uint8 UT_Utils_Any_uint8(void);

uint8 UT_Utils_Any_uint8_BetweenInclusive(uint8 floor, uint8 ceiling);

uint8 UT_Utils_Any_uint8_LessThan(uint8 ceiling);

int32 UT_Utils_Any_int32(void);

int32 UT_Utils_Any_int32_BetweenInclusive(int32 floor, int32 ceiling);

int32 UT_Utils_stub_reporter_hook(void *UserObj, int32 StubRetcode, 
                                  uint32 CallCount, const UT_StubContext_t *Context);

void fm_tests_Setup(void);

void fm_tests_Teardown(void);

void unimplemented(const char* func, const char* file, int line);

void TestUtil_InitMsg(void *MsgPtr, CFE_SB_MsgId_t MsgId,
                      uint16 Length, bool Clear);
#endif
