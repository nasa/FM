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
 *   Specification for the CFS FM ground commands.
 */
#ifndef FM_CMDS_H
#define FM_CMDS_H

#include "cfe.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler function prototypes                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 *  \brief Move File Command Handler Function
 *
 *  \par Description
 *       This function generates an event that displays the application version
 *       numbers.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  BufPtr Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_NOOP_CC, #FM_NoopCmd_t
 */
bool FM_NoopCmd(const CFE_SB_Buffer_t *BufPtr);

/**
 *  \brief Reset Counters Command Handler Function
 *
 *  \par Description
 *       This function resets the FM housekeeping telemetry counters.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  BufPtr Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_RESET_COUNTERS_CC, #FM_ResetCountersCmd_t
 */
bool FM_ResetCountersCmd(const CFE_SB_Buffer_t *BufPtr);

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
 *  \param [in]  BufPtr Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_COPY_FILE_CC, #FM_CopyFileCmd_t
 */
bool FM_CopyFileCmd(const CFE_SB_Buffer_t *BufPtr);

/**
 *  \brief Move File Command Handler Function
 *
 *  \par Description
 *       This function moves the command specified source file to the command
 *       specified target filename.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  BufPtr Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_MOVE_FILE_CC, #FM_MoveFileCmd_t
 */
bool FM_MoveFileCmd(const CFE_SB_Buffer_t *BufPtr);

/**
 *  \brief Rename File Command Handler Function
 *
 *  \par Description
 *       This function renames the command specified source file to the command
 *       specified target filename.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  BufPtr Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_RENAME_FILE_CC, #FM_RenameFileCmd_t
 */
bool FM_RenameFileCmd(const CFE_SB_Buffer_t *BufPtr);

/**
 *  \brief Delete File Command Handler Function
 *
 *  \par Description
 *       This function deletes the command specified file.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  BufPtr Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_DELETE_FILE_CC, #FM_DeleteFileCmd_t
 */
bool FM_DeleteFileCmd(const CFE_SB_Buffer_t *BufPtr);

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
 *  \param [in]  BufPtr Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_DELETE_ALL_FILES_CC, #FM_DeleteAllFilesCmd_t
 */
bool FM_DeleteAllFilesCmd(const CFE_SB_Buffer_t *BufPtr);

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
 *  \param [in]  BufPtr Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_DECOMPRESS_FILE_CC, #FM_DecompressFileCmd_t
 */
bool FM_DecompressFileCmd(const CFE_SB_Buffer_t *BufPtr);

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
 *  \param [in]  BufPtr Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_CONCAT_FILES_CC, #FM_ConcatFilesCmd_t
 */
bool FM_ConcatFilesCmd(const CFE_SB_Buffer_t *BufPtr);

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
 *  \param [in]  BufPtr Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_GET_FILE_INFO_CC, #FM_GetFileInfoCmd_t, #FM_FileInfoPkt_t
 */
bool FM_GetFileInfoCmd(const CFE_SB_Buffer_t *BufPtr);

/**
 *  \brief Get Open Files List Command Handler Function
 *
 *  \par Description
 *       This function creates a telemetry packet and populates it with a list of
 *       the current open files.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  BufPtr Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_GET_OPEN_FILES_CC, #FM_GetOpenFilesCmd_t, #FM_OpenFilesPkt_t
 */
bool FM_GetOpenFilesCmd(const CFE_SB_Buffer_t *BufPtr);

/**
 *  \brief Create Directory Command Handler Function
 *
 *  \par Description
 *       This function creates the command specified directory.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  BufPtr Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_CREATE_DIRECTORY_CC, #FM_CreateDirectoryCmd_t
 */
bool FM_CreateDirectoryCmd(const CFE_SB_Buffer_t *BufPtr);

/**
 *  \brief Delete Directory Command Handler Function
 *
 *  \par Description
 *       This function deletes the command specified directory.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  BufPtr Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_DELETE_DIRECTORY_CC, #FM_DeleteDirectoryCmd_t
 */
bool FM_DeleteDirectoryCmd(const CFE_SB_Buffer_t *BufPtr);

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
 *  \param [in]  BufPtr Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_GET_DIR_LIST_FILE_CC, #FM_GetDirListFileCmd_t,
 *      #FM_DirListFileStats_t, FM_DirListEntry_t
 */
bool FM_GetDirListFileCmd(const CFE_SB_Buffer_t *BufPtr);

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
 *  \param [in]  BufPtr Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_GET_DIR_LIST_PKT_CC, #FM_GetDirListPktCmd_t, #FM_DirListPkt_t
 */
bool FM_GetDirListPktCmd(const CFE_SB_Buffer_t *BufPtr);

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
 *  \param [in]  BufPtr Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_MONITOR_FILESYSTEM_SPACE_CC, #FM_MonitorFilesystemSpaceCmd_t, #FM_MonitorReportPkt_t
 */
bool FM_MonitorFilesystemSpaceCmd(const CFE_SB_Buffer_t *BufPtr);

/**
 *  \brief Set Table Entry State Command Handler Function
 *
 *  \par Description
 *       This function modifies the enable/disable state for a single entry in
 *       the File System Free Space Table.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  BufPtr Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_SET_TABLE_STATE_CC, #FM_SetTableStateCmd_t, #FM_MonitorTableEntry_t
 */
bool FM_SetTableStateCmd(const CFE_SB_Buffer_t *BufPtr);

/**
 *  \brief Set File Permissions of a file
 *
 *  \par Description
 *       This function is a direct call to OS_chmod to set the file access
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  BufPtr Pointer to Software Bus command packet.
 *
 *  \return Boolean command success response
 *  \retval true  Command successful
 *  \retval false Command not successful
 *
 *  \sa #FM_SET_PERMISSIONS_CC, #FM_SetPermissionsCmd_t, #FM_SET_PERM_CMD_INF_EID, #FM_SET_PERM_ERR_EID
 */
bool FM_SetPermissionsCmd(const CFE_SB_Buffer_t *BufPtr);

#endif
