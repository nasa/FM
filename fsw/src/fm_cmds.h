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
 *   Specification for the CFS FM ground commands.
 */
#ifndef FM_CMDS_H
#define FM_CMDS_H

#include "cfe_error.h"
#include "fm_msg.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler function prototypes                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 *  \brief Housekeeping Request Command Handler
 *
 *  \par Description
 *
 *       Allow CFE Table Services the opportunity to manage the File System
 *       Free Space Table.  This provides a mechanism to receive table updates.
 *
 *       Populate the FM application Housekeeping Telemetry packet.  Timestamp
 *       the packet and send it to ground via the Software Bus.
 *
 *  \par Assumptions, External Events, and Notes: None
 *
 *  \param [in]  Msg Pointer to Software Bus command packet.
 *
 *  \sa #FM_SendHkCmd_t, #FM_HkTlm_t
 */
CFE_Status_t FM_SendHkCmd(const FM_SendHkCmd_t *Msg);

/**
 *  \brief Noop Command Handler
 *
 *  \par Description
 *       This function generates an event that displays the application version
 *       numbers.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Msg Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_NOOP_CC, #FM_NoopCmd_t
 */
CFE_Status_t FM_NoopCmd(const FM_NoopCmd_t *Msg);

/**
 *  \brief Reset Counters Command Handler Function
 *
 *  \par Description
 *       This function resets the FM housekeeping telemetry counters.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Msg Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_RESET_COUNTERS_CC, #FM_ResetCountersCmd_t
 */
CFE_Status_t FM_ResetCountersCmd(const FM_ResetCountersCmd_t *Msg);

/**
 *  \brief Copy File Command Handler Function
 *
 *  \par Description
 *       This function copies the command specified source file to the command
 *       specified target file.
 *
 *       Because of the possibility that this command might take a very long time
 *       to complete, command argument validation will be done immediately but
 *       copying the file will be performed by a lower priority child task.
 *       As such, the return value for this function only refers to the result
 *       of command argument verification and being able to place the command on
 *       the child task interface queue.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Msg Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_COPY_FILE_CC, #FM_CopyFileCmd_t
 */
CFE_Status_t FM_CopyFileCmd(const FM_CopyFileCmd_t *Msg);

/**
 *  \brief Move File Command Handler Function
 *
 *  \par Description
 *       This function moves the command specified source file to the command
 *       specified target filename.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Msg Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_MOVE_FILE_CC, #FM_MoveFileCmd_t
 */
CFE_Status_t FM_MoveFileCmd(const FM_MoveFileCmd_t *Msg);

/**
 *  \brief Rename File Command Handler Function
 *
 *  \par Description
 *       This function renames the command specified source file to the command
 *       specified target filename.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Msg Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_RENAME_FILE_CC, #FM_RenameFileCmd_t
 */
CFE_Status_t FM_RenameFileCmd(const FM_RenameFileCmd_t *Msg);

/**
 *  \brief Delete File Command Handler Function
 *
 *  \par Description
 *       This function deletes the command specified file.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Msg Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_DELETE_FILE_CC, #FM_DeleteFileCmd_t
 */
CFE_Status_t FM_DeleteFileCmd(const FM_DeleteFileCmd_t *Msg);

/**
 *  \brief Delete All Files Command Handler Function
 *
 *  \par Description
 *       This function deletes all files from the command specified directory.
 *
 *       Because of the possibility that this command might take a very long time
 *       to complete, command argument validation will be done immediately but
 *       reading the directory and deleting each file will be performed by a
 *       lower priority child task.
 *       As such, the return value for this function only refers to the result
 *       of command argument verification and being able to place the command on
 *       the child task interface queue.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Msg Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_DELETE_ALL_FILES_CC, #FM_DeleteAllFilesCmd_t
 */
CFE_Status_t FM_DeleteAllFilesCmd(const FM_DeleteAllFilesCmd_t *Msg);

/**
 *  \brief Decompress Files Command Handler Function
 *
 *  \par Description
 *       This function decompresses the command specified source file into the
 *       command specified target file.
 *
 *       Because of the possibility that this command might take a very long time
 *       to complete, command argument validation will be done immediately but
 *       decompressing the source file into the target file will be performed by
 *       a lower priority child task.
 *       As such, the return value for this function only refers to the result
 *       of command argument verification and being able to place the command on
 *       the child task interface queue.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Msg Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_DECOMPRESS_FILE_CC, #FM_DecompressFileCmd_t
 */
CFE_Status_t FM_DecompressFileCmd(const FM_DecompressFileCmd_t *Msg);

/**
 *  \brief Concatenate Files Command Handler Function
 *
 *  \par Description
 *       This function concatenates two command specified source files into the
 *       command specified target file.
 *
 *       Because of the possibility that this command might take a very long time
 *       to complete, command argument validation will be done immediately but
 *       copying the first source file to the target file and then appending the
 *       second source file to the target file will be performed by a lower priority
 *       child task.
 *       As such, the return value for this function only refers to the result
 *       of command argument verification and being able to place the command on
 *       the child task interface queue.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Msg Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_CONCAT_FILES_CC, #FM_ConcatFilesCmd_t
 */
CFE_Status_t FM_ConcatFilesCmd(const FM_ConcatFilesCmd_t *Msg);

/**
 *  \brief Get File Information Command Handler Function
 *
 *  \par Description
 *       This function creates a telemetry packet and populates the packet with
 *       the current information regarding the command specified file.
 *
 *       Because of the possibility that this command might take a very long time
 *       to complete, command argument validation will be done immediately but
 *       collecting the status data and calculating the CRC will be performed by
 *       a lower priority child task.
 *       As such, the return value for this function only refers to the result
 *       of command argument verification and being able to place the command on
 *       the child task interface queue.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Msg Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_GET_FILE_INFO_CC, #FM_GetFileInfoCmd_t, #FM_FileInfoPkt_t
 */
CFE_Status_t FM_GetFileInfoCmd(const FM_GetFileInfoCmd_t *Msg);

/**
 *  \brief Get Open Files List Command Handler Function
 *
 *  \par Description
 *       This function creates a telemetry packet and populates it with a list of
 *       the current open files.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Msg Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_GET_OPEN_FILES_CC, #FM_GetOpenFilesCmd_t, #FM_OpenFilesPkt_t
 */
CFE_Status_t FM_GetOpenFilesCmd(const FM_GetOpenFilesCmd_t *Msg);

/**
 *  \brief Create Directory Command Handler Function
 *
 *  \par Description
 *       This function creates the command specified directory.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Msg Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_CREATE_DIRECTORY_CC, #FM_CreateDirectoryCmd_t
 */
CFE_Status_t FM_CreateDirectoryCmd(const FM_CreateDirectoryCmd_t *Msg);

/**
 *  \brief Delete Directory Command Handler Function
 *
 *  \par Description
 *       This function deletes the command specified directory.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Msg Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_DELETE_DIRECTORY_CC, #FM_DeleteDirectoryCmd_t
 */
CFE_Status_t FM_DeleteDirectoryCmd(const FM_DeleteDirectoryCmd_t *Msg);

/**
 *  \brief Get Directory List to Packet Command Handler Function
 *
 *  \par Description
 *       This function creates an output file and writes a listing of the command
 *       specified directory to the file.
 *
 *       Because of the possibility that this command might take a very long time
 *       to complete, command argument validation will be done immediately but
 *       reading the directory will be performed by a lower priority child task.
 *       As such, the return value for this function only refers to the result
 *       of command argument verification and being able to place the command on
 *       the child task interface queue.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Msg Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_GET_DIR_LIST_FILE_CC, #FM_GetDirListFileCmd_t,
 *      #FM_DirListFileStats_t, FM_DirListEntry_t
 */
CFE_Status_t FM_GetDirListFileCmd(const FM_GetDirListFileCmd_t *Msg);

/**
 *  \brief Get Directory List to Packet Command Handler Function
 *
 *  \par Description
 *       This function creates a telemetry packet and populates the packet with
 *       the directory listing data for the command specified directory, starting
 *       at the command specified directory entry.
 *
 *       Because of the possibility that this command might take a very long time
 *       to complete, command argument validation will be done immediately but
 *       reading the directory will be performed by a lower priority child task.
 *       As such, the return value for this function only refers to the result
 *       of command argument verification and being able to place the command on
 *       the child task interface queue.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Msg Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_GET_DIR_LIST_PKT_CC, #FM_GetDirListPktCmd_t, #FM_DirListPkt_t
 */
CFE_Status_t FM_GetDirListPktCmd(const FM_GetDirListPktCmd_t *Msg);

/**
 *  \brief Monitor Filesystem Command Handler Function
 *
 *  \par Description
 *       This function creates a telemetry packet and populates the packet with
 *       disk usage data for each location listed in the FM File System Monitor
 *       Table.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Msg Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_MONITOR_FILESYSTEM_SPACE_CC, #FM_MonitorFilesystemSpaceCmd_t, #FM_MonitorReportPkt_t
 */
CFE_Status_t FM_MonitorFilesystemSpaceCmd(const FM_MonitorFilesystemSpaceCmd_t *Msg);

/**
 *  \brief Set Table Entry State Command Handler Function
 *
 *  \par Description
 *       This function modifies the enable/disable state for a single entry in
 *       the File System Free Space Table.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Msg Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_SET_TABLE_STATE_CC, #FM_SetTableStateCmd_t, #FM_MonitorTableEntry_t
 */
CFE_Status_t FM_SetTableStateCmd(const FM_SetTableStateCmd_t *Msg);

/**
 *  \brief Set File Permissions of a file
 *
 *  \par Description
 *       This function is a direct call to OS_chmod to set the file access
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Msg Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_SET_PERMISSIONS_CC, #FM_SetPermissionsCmd_t, #FM_SET_PERM_CMD_INF_EID, #FM_SET_PERM_ERR_EID
 */
CFE_Status_t FM_SetPermissionsCmd(const FM_SetPermissionsCmd_t *Msg);

#endif
