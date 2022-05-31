/************************************************************************
 * NASA Docket No. GSC-18,918-1, and identified as “Core Flight
 * Software System (cFS) File Manager Application Version 2.6.1”
 *
 * Copyright (c) 2021 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * @file
 *   Test utility implementations
 */
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "fm_msg.h"

/* UT includes */
#include "uttest.h"
#include "utassert.h"
#include "utstubs.h"

#include "fm_test_utils.h"

#define UT_MAX_SENDEVENT_DEPTH 4
CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent[UT_MAX_SENDEVENT_DEPTH];

UT_CmdBuf_t UT_CmdBuf;

void UT_Handler_CFE_EVS_SendEvent(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context, va_list va)
{
    uint16 CallCount;
    uint16 idx;

    CallCount = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    if (CallCount > (sizeof(context_CFE_EVS_SendEvent) / sizeof(context_CFE_EVS_SendEvent[0])))
    {
        UtAssert_Failed("CFE_EVS_SendEvent UT depth %u exceeded: %u, increase UT_MAX_SENDEVENT_DEPTH",
                        UT_MAX_SENDEVENT_DEPTH, CallCount);
    }
    else
    {
        idx                                      = CallCount - 1;
        context_CFE_EVS_SendEvent[idx].EventID   = UT_Hook_GetArgValueByName(Context, "EventID", uint16);
        context_CFE_EVS_SendEvent[idx].EventType = UT_Hook_GetArgValueByName(Context, "EventType", uint16);

        strncpy(context_CFE_EVS_SendEvent[idx].Spec, UT_Hook_GetArgValueByName(Context, "Spec", const char *),
                CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);
        context_CFE_EVS_SendEvent[idx].Spec[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH - 1] = '\0';
    }
}

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

void FM_Test_Setup(void)
{

    UT_ResetState(0);

    memset(&FM_GlobalData, 0, sizeof(FM_GlobalData));
    memset(context_CFE_EVS_SendEvent, 0, sizeof(context_CFE_EVS_SendEvent));
    memset(&UT_CmdBuf, 0, sizeof(UT_CmdBuf));

    /* Register custom handlers */
    UT_SetVaHandlerFunction(UT_KEY(CFE_EVS_SendEvent), UT_Handler_CFE_EVS_SendEvent, NULL);
}

void FM_Test_Teardown(void)
{
    /* do nothing by design */
}
