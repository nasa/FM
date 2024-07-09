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
 *   Unit specification for the CFS File Manager Application.
 */
#ifndef FM_DISPATCH_H
#define FM_DISPATCH_H

#include "cfe.h"
#include "fm_msg.h"

/**
 *  \brief Process Input Command Packets
 *
 *  \par Description
 *
 *       Branch to appropriate input packet handler: HK request or FM commands.
 *
 *  \par Assumptions, External Events, and Notes: None
 *
 *  \param [in] BufPtr Pointer to Software Bus message buffer.
 *
 *  \sa #FM_SendHkCmd, #FM_ProcessCmd
 */
void FM_ProcessPkt(const CFE_SB_Buffer_t *BufPtr);

/**
 *  \brief Process FM Ground Commands
 *
 *  \par Description
 *
 *       Branch to the command specific handlers for FM ground commands.
 *
 *  \par Assumptions, External Events, and Notes: None
 *
 *  \param [in]  BufPtr Pointer to Software Bus message buffer.
 */
void FM_ProcessCmd(const CFE_SB_Buffer_t *BufPtr);

/**
 *  \brief Verify Command Packet Length Function
 *
 *  \par Description
 *       This function is invoked from each of the command handlers to verify the
 *       length of the command packet.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  MsgPtr         Pointer to Message
 *  \param [in]  ExpectedLength Expected packet length (command specific)
 *  \param [in]  EventID        Error event ID (command specific)
 *  \param [in]  CmdText        Error event text (command specific)
 *
 *  \return Boolean valid packet length response
 *  \retval true  Packet length valid
 *  \retval false Packet length invalid
 */
bool FM_IsValidCmdPktLength(const CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength, uint32 EventID,
                            const char *CmdText);

/*
 * Internal dispatch function for each command -
 * These are declared here so they can be directly invoked by the unit test for coverage
 */
CFE_Status_t FM_NoopVerifyDispatch(const FM_NoopCmd_t *BufPtr);
CFE_Status_t FM_ResetCountersVerifyDispatch(const FM_ResetCountersCmd_t *BufPtr);
CFE_Status_t FM_CopyFileVerifyDispatch(const FM_CopyFileCmd_t *BufPtr);
CFE_Status_t FM_MoveFileVerifyDispatch(const FM_MoveFileCmd_t *BufPtr);
CFE_Status_t FM_RenameFileVerifyDispatch(const FM_RenameFileCmd_t *BufPtr);
CFE_Status_t FM_DeleteFileVerifyDispatch(const FM_DeleteFileCmd_t *BufPtr);
CFE_Status_t FM_DeleteAllFilesVerifyDispatch(const FM_DeleteAllFilesCmd_t *BufPtr);
CFE_Status_t FM_DecompressFileVerifyDispatch(const FM_DecompressFileCmd_t *BufPtr);
CFE_Status_t FM_ConcatFilesVerifyDispatch(const FM_ConcatFilesCmd_t *BufPtr);
CFE_Status_t FM_GetFileInfoVerifyDispatch(const FM_GetFileInfoCmd_t *BufPtr);
CFE_Status_t FM_GetOpenFilesVerifyDispatch(const FM_GetOpenFilesCmd_t *BufPtr);
CFE_Status_t FM_CreateDirectoryVerifyDispatch(const FM_CreateDirectoryCmd_t *BufPtr);
CFE_Status_t FM_DeleteDirectoryVerifyDispatch(const FM_DeleteDirectoryCmd_t *BufPtr);
CFE_Status_t FM_GetDirListFileVerifyDispatch(const FM_GetDirListFileCmd_t *BufPtr);
CFE_Status_t FM_GetDirListPktVerifyDispatch(const FM_GetDirListPktCmd_t *BufPtr);
CFE_Status_t FM_MonitorFilesystemSpaceVerifyDispatch(const FM_MonitorFilesystemSpaceCmd_t *BufPtr);
CFE_Status_t FM_SetTableStateVerifyDispatch(const FM_SetTableStateCmd_t *BufPtr);
CFE_Status_t FM_SetPermissionsVerifyDispatch(const FM_SetPermissionsCmd_t *BufPtr);
void         FM_SendHkVerifyDispatch(const FM_SendHkCmd_t *BufPtr);

#endif
