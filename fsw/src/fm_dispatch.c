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
#include "fm_events.h"
#include "fm_cmds.h"
#include "fm_app.h"

#include "cfe.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- verify command packet length             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_IsValidCmdPktLength(const CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength, uint32 EventID, const char *CmdText)
{
    bool   FunctionResult = true;
    size_t ActualLength   = 0;

    CFE_MSG_GetSize(MsgPtr, &ActualLength);

    /* Verify command packet length */
    if (ActualLength != ExpectedLength)
    {
        FunctionResult = false;

        CFE_EVS_SendEvent(EventID, CFE_EVS_EventType_ERROR,
                          "%s error: invalid command packet length: expected = %d, actual = %d", CmdText,
                          (int)ExpectedLength, (int)ActualLength);
    }

    return FunctionResult;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- NOOP                                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CFE_Status_t FM_NoopVerifyDispatch(const FM_NoopCmd_t *BufPtr)
{
    /* Verify message length */
    if (!FM_IsValidCmdPktLength(&BufPtr->CommandHeader.Msg, sizeof(FM_NoopCmd_t), FM_NOOP_PKT_ERR_EID, "No-op"))
    {
        return CFE_STATUS_VALIDATION_FAILURE;
    }

    return FM_NoopCmd(BufPtr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Reset Counters                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CFE_Status_t FM_ResetCountersVerifyDispatch(const FM_ResetCountersCmd_t *BufPtr)
{
    /* Verify message length */
    if (!FM_IsValidCmdPktLength(&BufPtr->CommandHeader.Msg, sizeof(FM_ResetCountersCmd_t), FM_RESET_PKT_ERR_EID,
                                "Reset Counters"))
    {
        return CFE_STATUS_VALIDATION_FAILURE;
    }

    return FM_ResetCountersCmd(BufPtr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Copy File                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CFE_Status_t FM_CopyFileVerifyDispatch(const FM_CopyFileCmd_t *BufPtr)
{
    /* Verify command packet length */
    if (!FM_IsValidCmdPktLength(&BufPtr->CommandHeader.Msg, sizeof(FM_CopyFileCmd_t), FM_COPY_PKT_ERR_EID, "Copy File"))
    {
        return CFE_STATUS_VALIDATION_FAILURE;
    }

    return FM_CopyFileCmd(BufPtr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Move File                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CFE_Status_t FM_MoveFileVerifyDispatch(const FM_MoveFileCmd_t *BufPtr)
{
    /* Verify command packet length */
    if (!FM_IsValidCmdPktLength(&BufPtr->CommandHeader.Msg, sizeof(FM_MoveFileCmd_t), FM_MOVE_PKT_ERR_EID, "Move File"))
    {
        return CFE_STATUS_VALIDATION_FAILURE;
    }

    return FM_MoveFileCmd(BufPtr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Rename File                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CFE_Status_t FM_RenameFileVerifyDispatch(const FM_RenameFileCmd_t *BufPtr)
{
    /* Verify command packet length */
    if (!FM_IsValidCmdPktLength(&BufPtr->CommandHeader.Msg, sizeof(FM_RenameFileCmd_t), FM_RENAME_PKT_ERR_EID,
                                "Rename File"))
    {
        return CFE_STATUS_VALIDATION_FAILURE;
    }

    return FM_RenameFileCmd(BufPtr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Delete File                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CFE_Status_t FM_DeleteFileVerifyDispatch(const FM_DeleteFileCmd_t *BufPtr)
{
    /* Verify command packet length */
    if (!FM_IsValidCmdPktLength(&BufPtr->CommandHeader.Msg, sizeof(FM_DeleteFileCmd_t), FM_DELETE_PKT_ERR_EID,
                                "Delete File"))
    {
        return CFE_STATUS_VALIDATION_FAILURE;
    }

    return FM_DeleteFileCmd(BufPtr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Delete All Files                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CFE_Status_t FM_DeleteAllFilesVerifyDispatch(const FM_DeleteAllFilesCmd_t *BufPtr)
{
    /* Verify message length */
    if (!FM_IsValidCmdPktLength(&BufPtr->CommandHeader.Msg, sizeof(FM_DeleteAllFilesCmd_t), FM_DELETE_ALL_PKT_ERR_EID,
                                "Delete All Files"))
    {
        return CFE_STATUS_VALIDATION_FAILURE;
    }

    return FM_DeleteAllFilesCmd(BufPtr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Decompress File                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CFE_Status_t FM_DecompressFileVerifyDispatch(const FM_DecompressFileCmd_t *BufPtr)
{
    /* Verify command packet length */
    if (!FM_IsValidCmdPktLength(&BufPtr->CommandHeader.Msg, sizeof(FM_DecompressFileCmd_t), FM_DECOM_PKT_ERR_EID,
                                "Decompress File"))
    {
        return CFE_STATUS_VALIDATION_FAILURE;
    }

    return FM_DecompressFileCmd(BufPtr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Concatenate Files                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CFE_Status_t FM_ConcatFilesVerifyDispatch(const FM_ConcatFilesCmd_t *BufPtr)
{
    /* Verify command packet length */
    if (!FM_IsValidCmdPktLength(&BufPtr->CommandHeader.Msg, sizeof(FM_ConcatFilesCmd_t), FM_CONCAT_PKT_ERR_EID,
                                "Concat Files"))
    {
        return CFE_STATUS_VALIDATION_FAILURE;
    }

    return FM_ConcatFilesCmd(BufPtr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Get File Info                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CFE_Status_t FM_GetFileInfoVerifyDispatch(const FM_GetFileInfoCmd_t *BufPtr)
{
    /* Verify command packet length */
    if (!FM_IsValidCmdPktLength(&BufPtr->CommandHeader.Msg, sizeof(FM_GetFileInfoCmd_t), FM_GET_FILE_INFO_PKT_ERR_EID,
                                "Get File Info"))
    {
        return CFE_STATUS_VALIDATION_FAILURE;
    }

    return FM_GetFileInfoCmd(BufPtr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Get List of Open Files                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CFE_Status_t FM_GetOpenFilesVerifyDispatch(const FM_GetOpenFilesCmd_t *BufPtr)
{
    /* Verify command packet length */
    if (!FM_IsValidCmdPktLength(&BufPtr->CommandHeader.Msg, sizeof(FM_GetOpenFilesCmd_t), FM_GET_OPEN_FILES_PKT_ERR_EID,
                                "Get Open Files"))
    {
        return CFE_STATUS_VALIDATION_FAILURE;
    }

    return FM_GetOpenFilesCmd(BufPtr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Create Directory                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CFE_Status_t FM_CreateDirectoryVerifyDispatch(const FM_CreateDirectoryCmd_t *BufPtr)
{
    /* Verify command packet length */
    if (!FM_IsValidCmdPktLength(&BufPtr->CommandHeader.Msg, sizeof(FM_CreateDirectoryCmd_t), FM_CREATE_DIR_PKT_ERR_EID,
                                "Create Directory"))
    {
        return CFE_STATUS_VALIDATION_FAILURE;
    }

    return FM_CreateDirectoryCmd(BufPtr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Delete Directory                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CFE_Status_t FM_DeleteDirectoryVerifyDispatch(const FM_DeleteDirectoryCmd_t *BufPtr)
{
    /* Verify command packet length */
    if (!FM_IsValidCmdPktLength(&BufPtr->CommandHeader.Msg, sizeof(FM_DeleteDirectoryCmd_t), FM_DELETE_DIR_PKT_ERR_EID,
                                "Delete Directory"))
    {
        return CFE_STATUS_VALIDATION_FAILURE;
    }

    return FM_DeleteDirectoryCmd(BufPtr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Get List of Directory Entries (to file)   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CFE_Status_t FM_GetDirListFileVerifyDispatch(const FM_GetDirListFileCmd_t *BufPtr)
{
    /* Verify command packet length */
    if (!FM_IsValidCmdPktLength(&BufPtr->CommandHeader.Msg, sizeof(FM_GetDirListFileCmd_t), FM_GET_DIR_FILE_PKT_ERR_EID,
                                "Directory List to File"))
    {
        return CFE_STATUS_VALIDATION_FAILURE;
    }

    return FM_GetDirListFileCmd(BufPtr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Get List of Directory Entries (to pkt)    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CFE_Status_t FM_GetDirListPktVerifyDispatch(const FM_GetDirListPktCmd_t *BufPtr)
{
    /* Verify command packet length */
    if (!FM_IsValidCmdPktLength(&BufPtr->CommandHeader.Msg, sizeof(FM_GetDirListPktCmd_t), FM_GET_DIR_PKT_PKT_ERR_EID,
                                "Directory List to Packet"))
    {
        return CFE_STATUS_VALIDATION_FAILURE;
    }

    return FM_GetDirListPktCmd(BufPtr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Get File System Free Space                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CFE_Status_t FM_MonitorFilesystemSpaceVerifyDispatch(const FM_MonitorFilesystemSpaceCmd_t *BufPtr)
{
    /* Verify command packet length */
    if (!FM_IsValidCmdPktLength(&BufPtr->CommandHeader.Msg, sizeof(FM_MonitorFilesystemSpaceCmd_t),
                                FM_GET_FREE_SPACE_PKT_ERR_EID, "Get Free Space"))
    {
        return CFE_STATUS_VALIDATION_FAILURE;
    }

    return FM_MonitorFilesystemSpaceCmd(BufPtr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Set Table Entry Enable/Disable State      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CFE_Status_t FM_SetTableStateVerifyDispatch(const FM_SetTableStateCmd_t *BufPtr)
{
    /* Verify command packet length */
    if (!FM_IsValidCmdPktLength(&BufPtr->CommandHeader.Msg, sizeof(FM_SetTableStateCmd_t),
                                FM_SET_TABLE_STATE_PKT_ERR_EID, "Set Table State"))
    {
        return CFE_STATUS_VALIDATION_FAILURE;
    }

    return FM_SetTableStateCmd(BufPtr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Set Permissions for a file                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CFE_Status_t FM_SetPermissionsVerifyDispatch(const FM_SetPermissionsCmd_t *BufPtr)
{
    /* Verify command packet length */
    if (!FM_IsValidCmdPktLength(&BufPtr->CommandHeader.Msg, sizeof(FM_SetPermissionsCmd_t), FM_SET_PERM_ERR_EID,
                                "Set Permissions"))
    {
        return CFE_STATUS_VALIDATION_FAILURE;
    }

    return FM_SetPermissionsCmd(BufPtr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Send Housekeeping                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_SendHkVerifyDispatch(const FM_SendHkCmd_t *BufPtr)
{
    /* Verify command packet length */
    if (!FM_IsValidCmdPktLength(&BufPtr->CommandHeader.Msg, sizeof(FM_SendHkCmd_t), FM_HK_REQ_ERR_EID, "HK Request"))
    {
        return;
    }

    FM_SendHkCmd(BufPtr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM application -- input packet processor                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ProcessPkt(const CFE_SB_Buffer_t *BufPtr)
{
    CFE_SB_MsgId_t MessageID = CFE_SB_INVALID_MSG_ID;

    CFE_MSG_GetMsgId(&BufPtr->Msg, &MessageID);

    switch (CFE_SB_MsgIdToValue(MessageID))
    {
        /* Housekeeping request */
        case FM_SEND_HK_MID:
            FM_SendHkVerifyDispatch((FM_SendHkCmd_t *)BufPtr);
            break;

        /* FM ground commands */
        case FM_CMD_MID:
            FM_ProcessCmd(BufPtr);
            break;

        default:
            CFE_EVS_SendEvent(FM_MID_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Main loop error: invalid message ID: mid = 0x%08lX",
                              (unsigned long)CFE_SB_MsgIdToValue(MessageID));
            break;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM application -- command packet processor                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void FM_ProcessCmd(const CFE_SB_Buffer_t *BufPtr)
{
    CFE_Status_t      Status      = FM_ERROR;
    CFE_MSG_FcnCode_t CommandCode = 0;

    CFE_MSG_GetFcnCode(&BufPtr->Msg, &CommandCode);

    /* Invoke specific command handler */
    switch (CommandCode)
    {
        case FM_NOOP_CC:
            Status = FM_NoopVerifyDispatch((FM_NoopCmd_t *)BufPtr);
            break;

        case FM_RESET_COUNTERS_CC:
            Status = FM_ResetCountersVerifyDispatch((FM_ResetCountersCmd_t *)BufPtr);
            break;

        case FM_COPY_FILE_CC:
            Status = FM_CopyFileVerifyDispatch((FM_CopyFileCmd_t *)BufPtr);
            break;

        case FM_MOVE_FILE_CC:
            Status = FM_MoveFileVerifyDispatch((FM_MoveFileCmd_t *)BufPtr);
            break;

        case FM_RENAME_FILE_CC:
            Status = FM_RenameFileVerifyDispatch((FM_RenameFileCmd_t *)BufPtr);
            break;

        case FM_DELETE_FILE_CC:
            Status = FM_DeleteFileVerifyDispatch((FM_DeleteFileCmd_t *)BufPtr);
            break;

        case FM_DELETE_ALL_FILES_CC:
            Status = FM_DeleteAllFilesVerifyDispatch((FM_DeleteAllFilesCmd_t *)BufPtr);
            break;

        case FM_DECOMPRESS_FILE_CC:
            Status = FM_DecompressFileVerifyDispatch((FM_DecompressFileCmd_t *)BufPtr);
            break;

        case FM_CONCAT_FILES_CC:
            Status = FM_ConcatFilesVerifyDispatch((FM_ConcatFilesCmd_t *)BufPtr);
            break;

        case FM_GET_FILE_INFO_CC:
            Status = FM_GetFileInfoVerifyDispatch((FM_GetFileInfoCmd_t *)BufPtr);
            break;

        case FM_GET_OPEN_FILES_CC:
            Status = FM_GetOpenFilesVerifyDispatch((FM_GetOpenFilesCmd_t *)BufPtr);
            break;

        case FM_CREATE_DIRECTORY_CC:
            Status = FM_CreateDirectoryVerifyDispatch((FM_CreateDirectoryCmd_t *)BufPtr);
            break;

        case FM_DELETE_DIRECTORY_CC:
            Status = FM_DeleteDirectoryVerifyDispatch((FM_DeleteDirectoryCmd_t *)BufPtr);
            break;

        case FM_GET_DIR_LIST_FILE_CC:
            Status = FM_GetDirListFileVerifyDispatch((FM_GetDirListFileCmd_t *)BufPtr);
            break;

        case FM_GET_DIR_LIST_PKT_CC:
            Status = FM_GetDirListPktVerifyDispatch((FM_GetDirListPktCmd_t *)BufPtr);
            break;

        case FM_MONITOR_FILESYSTEM_SPACE_CC:
            Status = FM_MonitorFilesystemSpaceVerifyDispatch((FM_MonitorFilesystemSpaceCmd_t *)BufPtr);
            break;

        case FM_SET_TABLE_STATE_CC:
            Status = FM_SetTableStateVerifyDispatch((FM_SetTableStateCmd_t *)BufPtr);
            break;

        case FM_SET_PERMISSIONS_CC:
            Status = FM_SetPermissionsVerifyDispatch((FM_SetPermissionsCmd_t *)BufPtr);
            break;

        default:
            CFE_EVS_SendEvent(FM_CC_ERR_EID, CFE_EVS_EventType_ERROR, "Main loop error: invalid command code: cc = %d",
                              CommandCode);
            break;
    }

    if (Status == CFE_SUCCESS)
    {
        /* Increment command success counter */
        if (CommandCode != FM_RESET_COUNTERS_CC)
        {
            FM_GlobalData.CommandCounter++;
        }
    }
    else
    {
        /* Increment command error counter */
        FM_GlobalData.CommandErrCounter++;
    }
}
