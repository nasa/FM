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

/**
 * @file
 *  Core Flight System (CFS) File Manager (FM) Application
 *
 *  The File Manager (FM) Application provides onboard file system
 *  management services by processing commands for copying and moving
 *  files, decompressing files, concatenating files, creating directories,
 *  deleting files and directories, and providing file and directory status.
 *  When the File Manager application receives a housekeeping request
 *  (scheduled within the scheduler application), FM  reports it's housekeeping
 *  status values via telemetry messaging.
 */

#include "fm_dispatch.h"
#include "fm_msg.h"
#include "fm_msgdefs.h"
#include "fm_msgids.h"
#include "fm_eventids.h"
#include "fm_cmds.h"
#include "fm_app.h"
#include "fm_fcncodes.h"

#include "cfe.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- verify command packet length             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_VerifyCmdLength(const CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength)
{
    bool              result       = true;
    size_t            ActualLength = 0;
    CFE_SB_MsgId_t    MsgId        = CFE_SB_INVALID_MSG_ID;
    CFE_MSG_FcnCode_t FcnCode      = 0;

    CFE_MSG_GetSize(MsgPtr, &ActualLength);

    /*
    ** Verify the command packet length.
    */
    if (ExpectedLength != ActualLength)
    {
        CFE_MSG_GetMsgId(MsgPtr, &MsgId);
        CFE_MSG_GetFcnCode(MsgPtr, &FcnCode);

        CFE_EVS_SendEvent(FM_CMD_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u",
                          (unsigned int)CFE_SB_MsgIdToValue(MsgId), (unsigned int)FcnCode, (unsigned int)ActualLength,
                          (unsigned int)ExpectedLength);

        result = false;

        ++FM_AppData.HkTlm.Payload.CommandErrCounter;
    }

    return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM application -- command packet processor                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void FM_ProcessGroundCommand(const CFE_SB_Buffer_t *BufPtr)
{
    CFE_MSG_FcnCode_t CommandCode = 0;

    CFE_MSG_GetFcnCode(&BufPtr->Msg, &CommandCode);
    switch (CommandCode)
    {
        case FM_NOOP_CC:
            if (FM_VerifyCmdLength(&BufPtr->Msg, sizeof(FM_NoopCmd_t)))
            {
                FM_NoopCmd((FM_NoopCmd_t*) BufPtr);
            }
            break;

        case FM_RESET_COUNTERS_CC:
            if (FM_VerifyCmdLength(&BufPtr->Msg, sizeof(FM_ResetCountersCmd_t)))
            {
                FM_ResetCountersCmd((FM_ResetCountersCmd_t*) BufPtr);
            }
            break;

        case FM_COPY_FILE_CC:
            if (FM_VerifyCmdLength(&BufPtr->Msg, sizeof(FM_CopyFileCmd_t)))
            {
                FM_CopyFileCmd((FM_CopyFileCmd_t*) BufPtr);
            }
            break;

        case FM_MOVE_FILE_CC:
            if (FM_VerifyCmdLength(&BufPtr->Msg, sizeof(FM_MoveFileCmd_t)))
            {
                FM_MoveFileCmd((FM_MoveFileCmd_t*) BufPtr);
            }
            break;

        case FM_RENAME_FILE_CC:
            if (FM_VerifyCmdLength(&BufPtr->Msg, sizeof(FM_RenameFileCmd_t)))
            {
                FM_RenameFileCmd((FM_RenameFileCmd_t*) BufPtr);
            }
            break;

        case FM_DELETE_FILE_CC:
            if (FM_VerifyCmdLength(&BufPtr->Msg, sizeof(FM_DeleteFileCmd_t)))
            {
                FM_DeleteFileCmd((FM_DeleteFileCmd_t*) BufPtr);
            }
            break;

        case FM_DELETE_ALL_FILES_CC:
            if (FM_VerifyCmdLength(&BufPtr->Msg, sizeof(FM_DeleteAllFilesCmd_t)))
            {
                FM_DeleteAllFilesCmd((FM_DeleteAllFilesCmd_t*) BufPtr);
            }
            break;

        case FM_DECOMPRESS_FILE_CC:
            if (FM_VerifyCmdLength(&BufPtr->Msg, sizeof(FM_DecompressFileCmd_t)))
            {
                FM_DecompressFileCmd((FM_DecompressFileCmd_t*) BufPtr);
            }
            break;

        case FM_CONCAT_FILES_CC:
            if (FM_VerifyCmdLength(&BufPtr->Msg, sizeof(FM_ConcatFilesCmd_t)))
            {
                FM_ConcatFilesCmd((FM_ConcatFilesCmd_t*) BufPtr);
            }
            break;

        case FM_GET_FILE_INFO_CC:
            if (FM_VerifyCmdLength(&BufPtr->Msg, sizeof(FM_GetFileInfoCmd_t)))
            {
                FM_GetFileInfoCmd((FM_GetFileInfoCmd_t*) BufPtr);
            }
            break;

        case FM_GET_OPEN_FILES_CC:
            if (FM_VerifyCmdLength(&BufPtr->Msg, sizeof(FM_GetOpenFilesCmd_t)))
            {
                FM_GetOpenFilesCmd((FM_GetOpenFilesCmd_t*) BufPtr);
            }
            break;

        case FM_CREATE_DIRECTORY_CC:
            if (FM_VerifyCmdLength(&BufPtr->Msg, sizeof(FM_CreateDirectoryCmd_t)))
            {
                FM_CreateDirectoryCmd((FM_CreateDirectoryCmd_t*) BufPtr);
            }
            break;

        case FM_DELETE_DIRECTORY_CC:
            if (FM_VerifyCmdLength(&BufPtr->Msg, sizeof(FM_DeleteDirectoryCmd_t)))
            {
                FM_DeleteDirectoryCmd((FM_DeleteDirectoryCmd_t*) BufPtr);
            }
            break;

        case FM_GET_DIR_LIST_FILE_CC:
            if (FM_VerifyCmdLength(&BufPtr->Msg, sizeof(FM_GetDirListFileCmd_t)))
            {
                FM_GetDirListFileCmd((FM_GetDirListFileCmd_t*) BufPtr);
            }
            break;

        case FM_GET_DIR_LIST_PKT_CC:
            if (FM_VerifyCmdLength(&BufPtr->Msg, sizeof(FM_GetDirListPktCmd_t)))
            {
                FM_GetDirListPktCmd((FM_GetDirListPktCmd_t*) BufPtr);
            }
            break;

        case FM_MONITOR_FILESYSTEM_SPACE_CC:
            if (FM_VerifyCmdLength(&BufPtr->Msg, sizeof(FM_MonitorFilesystemSpaceCmd_t)))
            {
                FM_MonitorFilesystemSpaceCmd((FM_MonitorFilesystemSpaceCmd_t*) BufPtr);
            }
            break;

        case FM_SET_TABLE_STATE_CC:
            if (FM_VerifyCmdLength(&BufPtr->Msg, sizeof(FM_SetTableStateCmd_t)))
            {
                FM_SetTableStateCmd((FM_SetTableStateCmd_t*) BufPtr);
            }
            break;

        case FM_SET_PERMISSIONS_CC:
            if (FM_VerifyCmdLength(&BufPtr->Msg, sizeof(FM_SetPermissionsCmd_t)))
            {
                FM_SetPermissionsCmd((FM_SetPermissionsCmd_t*) BufPtr);
            }
            break;

        default:
            FM_AppData.HkTlm.Payload.CommandErrCounter++;
            CFE_EVS_SendEvent(FM_CC_ERR_EID, CFE_EVS_EventType_ERROR,
                                "Invalid ground command code: cc = %d",
                                CommandCode);
            break;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM application -- input packet processor                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_TaskPipe(const CFE_SB_Buffer_t *BufPtr)
{
    static CFE_SB_MsgId_t CMD_MID     = CFE_SB_MSGID_RESERVED;
    static CFE_SB_MsgId_t SEND_HK_MID = CFE_SB_MSGID_RESERVED;

    CFE_SB_MsgId_t MessageId = CFE_SB_INVALID_MSG_ID;


    /* Cache the local MID values here, this avoid repeat lookups */
    if (!CFE_SB_IsValidMsgId(CMD_MID))
    {
        CMD_MID     = CFE_SB_ValueToMsgId(FM_CMD_MID);
        SEND_HK_MID = CFE_SB_ValueToMsgId(FM_SEND_HK_MID);
    }

    CFE_MSG_GetMsgId(&BufPtr->Msg, &MessageId);

    if (CFE_SB_MsgId_Equal(MessageId, CMD_MID))
    {
        FM_ProcessGroundCommand(BufPtr);
    }
    else if (CFE_SB_MsgId_Equal(MessageId, SEND_HK_MID))
    {
        FM_SendHkCmd((const FM_SendHkCmd_t*) BufPtr);
    }
    else
    {
        FM_AppData.HkTlm.Payload.CommandErrCounter++;
        CFE_EVS_SendEvent(FM_MID_ERR_EID, CFE_EVS_EventType_ERROR,
                            "Invalid command pipe message ID: 0x%08lX",
                            (unsigned long)CFE_SB_MsgIdToValue(MessageId));
    }
}