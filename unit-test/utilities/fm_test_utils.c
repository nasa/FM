#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "fm_msg.h"

/* UT includes */
#include "uttest.h"
#include "utassert.h"
#include "utstubs.h"

/* CF includes */
#include "fm_test_utils.h"

uint32 UT_Utils_Any_uint32(void)
{
    return (uint32)rand();
}

uint32 UT_Utils_Any_uint32_GreaterThan(uint32 floor)
{
    uint32 random_val;

    random_val = MAX_UINT32 - (rand() % (MAX_UINT32 - floor - 1));

    return random_val;
}

uint8 UT_Utils_Any_uint8(void)
{
    uint8 random_val = (uint8)rand();

    return random_val;
}

uint8 UT_Utils_Any_uint8_BetweenInclusive(uint8 floor, uint8 ceiling)
{
    uint8 random_val;
    uint8 diff = ceiling - floor + 1; /* +1 for inclusive */

    random_val = (rand() % diff) + floor;

    return random_val;
}

uint8 UT_Utils_Any_uint8_LessThan(uint8 ceiling)
{
    uint8 random_val;

    random_val = rand() % ceiling;

    return random_val;
}


int32 UT_Utils_Any_int32(void)
{
    int32 random_val = rand() % MAX_INT;

    bool coin_toss = rand() % 2;

    if (coin_toss == true)
    {
        /* 0 to INT_MAX becomes -1 to INT_MIN */
        random_val *= -1; /* flip sign */
        random_val += -1;
    }

    return random_val;
}

int32 UT_Utils_Any_int32_BetweenInclusive(int32 floor, int32 ceiling)
{
    int32 random_val = UT_Utils_Any_int32();

    while ((random_val < floor) || (random_val > ceiling))
    {
        random_val = UT_Utils_Any_int32();
    }

    return random_val;
}





int32 UT_Utils_stub_reporter_hook(void *UserObj, int32 StubRetcode, uint32 CallCount, const UT_StubContext_t *Context)
{
    uint8 i            = 0;    /* i is index */
    uint8 size_used    = 0;    /* determines size of argument to be saved */
    const void *val_location = NULL; /* pointer to arg value to be saved */
    uint8 *obj_ptr;             /* tracker indicates where to push data into UserObj */

    /* Determine where in the UserObj we should be located dependent upon CallCount */
    if (CallCount == 0)
    {
        obj_ptr = (uint8*)UserObj;
    }
    else
    {
        uint8 context_size = 0;

        for (i = 0; i < Context->ArgCount; ++i)
        {
            /* A UT_STUBCONTEXT_ARG_TYPE_DIRECT type indicates the arg itself is the ptr argument, add a (void*) size */
            if (Context->Meta[i].Type == UT_STUBCONTEXT_ARG_TYPE_DIRECT)
            {
                context_size += sizeof(void *);
            }
            else /* UT_STUBCONTEXT_ARG_TYPE_INDIRECT indicates the arg is pointing to the value to be saved, add its
                    size */
            {
                context_size += Context->Meta[i].Size;
            }
        }

        /* obj_ptr moves a full context_size for every call (initial value is 0) -- user object for calls > 1 must be an
         * array of contexts */
        obj_ptr = (uint8*)UserObj + (context_size * CallCount);
    }

    for (i = 0; i < Context->ArgCount; ++i)
    {
        /* UT_STUBCONTEXT_ARG_TYPE_DIRECT indicates the arg is the ptr that is to be saved */
        if (Context->Meta[i].Type == UT_STUBCONTEXT_ARG_TYPE_DIRECT)
        {
            val_location = &Context->ArgPtr[i];
            size_used    = sizeof(void *);
        }
        else /* UT_STUBCONTEXT_ARG_TYPE_INDIRECT indicates the arg is pointing to the value to be saved */
        {
            val_location = Context->ArgPtr[i];
            size_used    = Context->Meta[i].Size;
        }
        /* put the argument value into the user object */
        memcpy(obj_ptr, val_location, size_used);
        /* move to end of this size item in the user object */
        obj_ptr += size_used;
    }

    return StubRetcode;
}
/******************************************************************************
* The Setup and Teardown 
******************************************************************************/

void fm_tests_Setup(void)
{

    UT_ResetState(0);

    memset(&FM_GlobalData, 0, sizeof(FM_GlobalData_t));
}

void fm_tests_Teardown(void)
{
    /* do nothing by design */
}

/******************************************************************************
* ut utility functions that we may want to add to ut_assert 
******************************************************************************/

void unimplemented(const char* func, const char* file, int line)
{
    printf("NOT YET IMPLEMENTED stub for function '%s' in \n%s:line #%d\n", 
      func, file, line);
    exit(-86);
}

void TestUtil_InitMsg(void *MsgPtr,
                      CFE_SB_MsgId_t MsgId,
                      uint16 Length,
                      bool Clear)
{
	printf("TestUtil_InitMsg - NO OPERATION PERFORMED\n");
}
