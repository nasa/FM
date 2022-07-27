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

#ifndef FM_TEST_UTILS_H
#define FM_TEST_UTILS_H

#include "cfe.h"

#include "common_types.h"

#include "fm_platform_cfg.h"
#include "fm_app.h"
#include "utstubs.h"

typedef struct
{
    uint16 EventID;
    uint16 EventType;
    char   Spec[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
} CFE_EVS_SendEvent_context_t;

extern CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent[];

/* Command buffer typedef for any handler */
typedef union
{
    CFE_SB_Buffer_t       Buf;
    FM_HousekeepingCmd_t  HousekeepingCmd;
    FM_NoopCmd_t          NoopCmd;
    FM_ResetCmd_t         ResetCmd;
    FM_CopyFileCmd_t      CopyFileCmd;
    FM_MoveFileCmd_t      MoveFileCmd;
    FM_RenameFileCmd_t    RenameFileCmd;
    FM_DeleteFileCmd_t    DeleteFileCmd;
    FM_DeleteAllCmd_t     DeleteAllCmd;
    FM_DecompressCmd_t    DecompressCmd;
    FM_ConcatCmd_t        ConcatCmd;
    FM_GetFileInfoCmd_t   GetFileInfoCmd;
    FM_GetOpenFilesCmd_t  GetOpenFilesCmd;
    FM_CreateDirCmd_t     CreateDirCmd;
    FM_DeleteDirCmd_t     DeleteDirCmd;
    FM_GetDirFileCmd_t    GetDirFileCmd;
    FM_GetDirPktCmd_t     GetDirPktCmd;
    FM_GetFreeSpaceCmd_t  GetFreeSpaceCmd;
    FM_SetTableStateCmd_t SetTableStateCmd;
    FM_SetPermCmd_t       SetPermCmd;
} UT_CmdBuf_t;

extern UT_CmdBuf_t UT_CmdBuf;

/* Unit test osal ID, generic w/ no type */
#define FM_UT_OBJID_1 OS_ObjectIdFromInteger(1)
#define FM_UT_OBJID_2 OS_ObjectIdFromInteger(2)

void FM_Test_Setup(void);

void FM_Test_Teardown(void);

#endif
