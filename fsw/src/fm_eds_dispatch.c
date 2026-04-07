/*******************************************************************************
**
**      GSC-18128-1, "Core Flight Executive Version 6.7"
**
**      Copyright (c) 2006-2019 United States Government as represented by
**      the Administrator of the National Aeronautics and Space Administration.
**      All Rights Reserved.
**
**      Licensed under the Apache License, Version 2.0 (the "License");
**      you may not use this file except in compliance with the License.
**      You may obtain a copy of the License at
**
**        http://www.apache.org/licenses/LICENSE-2.0
**
**      Unless required by applicable law or agreed to in writing, software
**      distributed under the License is distributed on an "AS IS" BASIS,
**      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**      See the License for the specific language governing permissions and
**      limitations under the License.
**
** File: fm_eds_dispatch.c
**
** Purpose:
**   This file contains the source code for the Command Ingest task.
**
*******************************************************************************/

/*
**   Include Files:
*/

#include "fm_app.h"
#include "fm_cmds.h"
#include "fm_dispatch.h"
#include "fm_eventids.h"
#include "fm_msgids.h"
#include "fm_msg.h"

#include "fm_eds_dispatcher.h"
#include "fm_eds_dictionary.h"

/*
 * Define a lookup table for FM lab command codes
 */
/* clang-format off */
static const EdsDispatchTable_EdsComponent_FM_Application_CFE_SB_Telecommand_t FM_TC_DISPATCH_TABLE =
{
    .CMD =
    {
        .NoopCmd_indication                   = FM_NoopCmd,
        .ResetCountersCmd_indication          = FM_ResetCountersCmd,
        .SendHkCmd_indication                 = FM_SendHkCmd,
        .CopyFileCmd_indication               = FM_CopyFileCmd,
        .MoveFileCmd_indication               = FM_MoveFileCmd,
        .RenameFileCmd_indication             = FM_RenameFileCmd,
        .DeleteFileCmd_indication             = FM_DeleteFileCmd,
        .DeleteAllFilesCmd_indication         = FM_DeleteAllFilesCmd,
        .DecompressFileCmd_indication         = FM_DecompressFileCmd,
        .ConcatFilesCmd_indication            = FM_ConcatFilesCmd,
        .GetFileInfoCmd_indication            = FM_GetFileInfoCmd,
        .GetOpenFilesCmd_indication           = FM_GetOpenFilesCmd,
        .CreateDirectoryCmd_indication        = FM_CreateDirectoryCmd,
        .DeleteDirectoryCmd_indication        = FM_DeleteDirectoryCmd,
        .GetDirListFileCmd_indication         = FM_GetDirListFileCmd,
        .GetDirListPktCmd_indication          = FM_GetDirListPktCmd,
        .MonitorFilesystemSpaceCmd_indication = FM_MonitorFilesystemSpaceCmd,
        .SetTableStateCmd_indication          = FM_SetTableStateCmd,
        .SetPermissionsCmd_indication         = FM_SetPermissionsCmd,
    },
    .SEND_HK =
    {
        .indication = FM_SendHkCmd
    }
};
/* clang-format on */

/* ==================== */
/* Function Definitions */
/* ==================== */

void FM_TaskPipe(const CFE_SB_Buffer_t *BufPtr)
{
    CFE_Status_t      Status;
    CFE_SB_MsgId_t    MsgId;
    CFE_MSG_Size_t    MsgSize;
    CFE_MSG_FcnCode_t MsgFc;

    Status = EdsDispatch_EdsComponent_FM_Application_Telecommand(BufPtr, &FM_TC_DISPATCH_TABLE);

    if (Status != CFE_SUCCESS)
    {
        CFE_MSG_GetMsgId(&BufPtr->Msg, &MsgId);
        CFE_MSG_GetSize(&BufPtr->Msg, &MsgSize);
        CFE_MSG_GetFcnCode(&BufPtr->Msg, &MsgFc);
        ++FM_AppData.HkTlm.Payload.CommandErrorCounter;

        if (Status == CFE_STATUS_UNKNOWN_MSG_ID)
        {
            CFE_EVS_SendEvent(FM_MID_ERR_EID,
                              CFE_EVS_EventType_ERROR,
                              "FM: invalid command packet,MID = 0x%x",
                              (unsigned int)CFE_SB_MsgIdToValue(MsgId));
        }
        else if (Status == CFE_STATUS_WRONG_MSG_LENGTH)
        {
            CFE_EVS_SendEvent(FM_CMD_LEN_ERR_EID,
                              CFE_EVS_EventType_ERROR,
                              "FM: Invalid Msg length: ID = 0x%X, CC = %u, Len = %u",
                              (unsigned int)CFE_SB_MsgIdToValue(MsgId),
                              (unsigned int)MsgFc,
                              (unsigned int)MsgSize);
        }
        else
        {
            CFE_EVS_SendEvent(FM_CC_ERR_EID,
                              CFE_EVS_EventType_ERROR,
                              "FM: Invalid ground command code: CC = %d",
                              (int)MsgFc);
        }
    }
}
