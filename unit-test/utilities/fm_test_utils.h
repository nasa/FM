/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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

#ifndef FM_TEST_UTILS_H
#define FM_TEST_UTILS_H

#include "cfe.h"

#include "common_types.h"

#include "fm_platform_cfg.h"
#include "fm_msg.h"
#include "utstubs.h"

/* ====== */
/* Macros */
/* ====== */

#define ADD_TEST(test) UtTest_Add(test, FM_Test_Setup, FM_Test_Teardown, #test)
#define FM_UT_OBJID_1  OS_ObjectIdFromInteger(1)
#define FM_UT_OBJID_2  OS_ObjectIdFromInteger(2)

/* ======== */
/* Typedefs */
/* ======== */

typedef struct
{
    uint16 EventID;
    uint16 EventType;
    char   Spec[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
} CFE_EVS_SendEvent_context_t;

extern CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent[];

/* =================== */
/* Function Prototypes */
/* =================== */

void FM_Test_Verify_Event(uint8 IssuedOrder, uint16 EventId, uint16 EventType, const char *EventText);

void FM_Test_Setup(void);

void FM_Test_Teardown(void);

#endif
