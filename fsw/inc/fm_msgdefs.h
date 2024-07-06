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
 *   Specification for the CFS FM command and telemetry message
 *   macro definitions.
 */
#ifndef FM_MSGDEFS_H
#define FM_MSGDEFS_H

/**
 * \defgroup cfsfmcmdcodes CFS File Manager Command Codes
 * \{
 */

/**
 * \brief No Operation
 *
 *  \par Description
 *       This command performs no operation other than to generate an
 *       informational event that also contains software version data.
 *       The command is most often used as a general aliveness test by
 *       demonstrating that the application can receive commands and
 *       generate telemetry.
 *
 *  \par Command Packet Structure
 *       #FM_NoopCmd_t
 *
 *  \par Command Success Verification
 *       - Informational event #FM_NOOP_INF_EID will be sent
 *       - #FM_HousekeepingPkt_Payload_t.CommandCounter will increment
 *
 *  \par Command Error Conditions
 *       - Invalid command packet length
 *
 *  \par Command Failure Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandErrCounter will increment
 *       - Error event #FM_NOOP_PKT_ERR_EID will be sent
 *
 *  \par Criticality
 *       - There are no critical issues related to this command.
 */
#define FM_NOOP_CC 0

/**
 * \brief Reset Counters
 *
 *  \par Description
 *       This command resets the following housekeeping telemetry:
 *       - #FM_HousekeepingPkt_Payload_t.CommandCounter
 *       - #FM_HousekeepingPkt_Payload_t.CommandErrCounter
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdCounter
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdErrCounter
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdWarnCounter
 *
 *  \par Command Packet Structure
 *       #FM_ResetCountersCmd_t
 *
 *  \par Command Success Verification
 *       - Command counters will be set to zero (see description)
 *       - Informational event #FM_RESET_INF_EID will be sent
 *
 *  \par Command Error Conditions
 *       - Invalid command packet length
 *
 *  \par Command Failure Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandErrCounter will increment
 *       - Error event #FM_RESET_PKT_ERR_EID will be sent
 *
 *  \par Criticality
 *       - There are no critical issues related to this command.
 *
 */
#define FM_RESET_COUNTERS_CC 1

/**
 * \brief Copy File
 *
 *  \par Description
 *       This command copies the source file to the target file.
 *       The source must be an existing file and the target must not be a
 *       directory name.
 *       If the Overwrite command argument is TRUE, then the target may be
 *       an existing file, provided that the file is closed.
 *       If the Overwrite command argument is FALSE, then the target must not exist.
 *       The source and target may be on different file systems.
 *
 *       Because of the possibility that this command might take a very long time
 *       to complete, command argument validation will be done immediately but
 *       copying the file will be performed by a lower priority child task.
 *       As such, the command result for this function only refers to the result
 *       of command argument verification and being able to place the command on
 *       the child task interface queue.
 *
 *  \par Command Packet Structure
 *       #FM_CopyFileCmd_t
 *
 *  \par Command Success Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandCounter will increment after validation
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdCounter will increment after completion
 *       - Informational event #FM_COPY_CMD_INF_EID will be sent
 *
 *  \par Command Error Conditions
 *       - Invalid command packet length
 *       - Overwrite is not TRUE (one) or FALSE (zero)
 *       - Source filename is invalid
 *       - Source file does not exist
 *       - Source filename is a directory
 *       - Target filename is invalid
 *       - Target file already exists
 *       - Target filename is a directory
 *       - Child task interface queue is full
 *       - Child task interface logic is broken
 *       - Failure of OS copy function
 *
 *  \par Command Failure Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandErrCounter may increment
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdErrCounter may increment
 *       - Error event #FM_COPY_PKT_ERR_EID may be sent
 *       - Error event #FM_COPY_OVR_ERR_EID may be sent
 *       - Error event #FM_COPY_SRC_INVALID_ERR_EID may be sent
 *       - Error event #FM_COPY_SRC_DNE_ERR_EID may be sent
 *       - Error event #FM_COPY_SRC_ISDIR_ERR_EID may be sent
 *       - Error event #FM_COPY_TGT_INVALID_ERR_EID may be sent
 *       - Error event #FM_COPY_TGT_EXIST_ERR_EID may be sent
 *       - Error event #FM_COPY_TGT_ISDIR_ERR_EID may be sent
 *       - Error event #FM_COPY_CHILD_DISABLED_ERR_EID may be sent
 *       - Error event #FM_COPY_CHILD_FULL_ERR_EID may be sent
 *       - Error event #FM_COPY_CHILD_BROKEN_ERR_EID may be sent
 *       - Error event #FM_COPY_OS_ERR_EID may be sent
 *
 *  \par Criticality
 *       Copying files may consume file space needed by other
 *       critical tasks.  Also, copying very large files may
 *       consume more CPU resource than anticipated.
 *
 *  \sa #FM_MOVE_FILE_CC, #FM_RENAME_FILE_CC
 */
#define FM_COPY_FILE_CC 2

/**
 * \brief Move File
 *
 *  \par Description
 *       This command moves the source file to the target file.
 *       The source must be an existing file and the target must not be a
 *       directory name.
 *       If the Overwrite command argument is TRUE, then the target may be
 *       an existing file, provided that the file is closed.
 *       If the Overwrite command argument is FALSE, then the target must not exist.
 *       Source and target must both be on the same file system.
 *       The move command does not actually move any file data.
 *       The command modifies the file system directory structure to
 *       create a different file entry for the same file data.
 *       If the user wishes to move a file across file systems, he
 *       must first copy the file and then delete the original.
 *
 *       Because of the possibility that this command might take a very long time
 *       to complete, command argument validation will be done immediately but
 *       moving the file will be performed by a lower priority child task.
 *       As such, the command result for this function only refers to the result
 *       of command argument verification and being able to place the command on
 *       the child task interface queue.
 *
 **
 *
 *  \par Command Packet Structure
 *       #FM_MoveFileCmd_t
 *
 *  \par Command Success Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandCounter will increment after validation
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdCounter will increment after completion
 *       - Informational event #FM_MOVE_CMD_INF_EID will be sent
 *
 *  \par Command Error Conditions
 *       - Invalid command packet length
 *       - Overwrite is not TRUE (one) or FALSE (zero)
 *       - Source filename is invalid
 *       - Source file does not exist
 *       - Source filename is a directory
 *       - Target filename is invalid
 *       - Target file already exists
 *       - Target filename is a directory
 *       - Failure of OS move function
 *
 *  \par Command Failure Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandErrCounter may increment
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdErrCounter may increment
 *       - Error event #FM_MOVE_PKT_ERR_EID may be sent
 *       - Error event #FM_MOVE_OVR_ERR_EID may be sent
 *       - Error event #FM_MOVE_SRC_INVALID_ERR_EID may be sent
 *       - Error event #FM_MOVE_SRC_DNE_ERR_EID may be sent
 *       - Error event #FM_MOVE_SRC_ISDIR_ERR_EID may be sent
 *       - Error event #FM_MOVE_TGT_INVALID_ERR_EID may be sent
 *       - Error event #FM_MOVE_TGT_EXIST_ERR_EID may be sent
 *       - Error event #FM_MOVE_TGT_ISDIR_ERR_EID may be sent
 *       - Error event #FM_MOVE_CHILD_DISABLED_ERR_EID may be sent
 *       - Error event #FM_MOVE_CHILD_FULL_ERR_EID may be sent
 *       - Error event #FM_MOVE_CHILD_BROKEN_ERR_EID may be sent
 *       - Error event #FM_MOVE_OS_ERR_EID may be sent
 *
 *  \par Criticality
 *       - There are no critical issues related to this command.
 *
 *  \sa #FM_COPY_FILE_CC, #FM_RENAME_FILE_CC
 */
#define FM_MOVE_FILE_CC 3

/**
 * \brief Rename File
 *
 *  \par Description
 *       This command renames the source file to the target file.
 *       Source must be an existing file and target must not exist.
 *       Source and target must both be on the same file system.
 *       The rename command does not actually move any file data.
 *       The command modifies the file system directory structure to
 *       create a different file entry for the same file data.
 *       If the user wishes to rename a file across file systems, he
 *       must first copy the file and then delete the original.
 *
 *       Because of the possibility that this command might take a very long time
 *       to complete, command argument validation will be done immediately but
 *       renaming the file will be performed by a lower priority child task.
 *       As such, the command result for this function only refers to the result
 *       of command argument verification and being able to place the command on
 *       the child task interface queue.
 *
 *  \par Command Packet Structure
 *       #FM_RenameFileCmd_t
 *
 *  \par Command Success Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandCounter will increment after validation
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdCounter will increment after completion
 *       - Informational event #FM_RENAME_CMD_INF_EID will be sent
 *
 *  \par Command Error Conditions
 *       - Invalid command packet length
 *       - Source filename is invalid
 *       - Source file does not exist
 *       - Source filename is a directory
 *       - Target filename is invalid
 *       - Target file already exists
 *       - Target filename is a directory
 *       - Failure of OS rename function
 *
 *  \par Command Failure Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandErrCounter may increment
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdErrCounter may increment
 *       - Error event #FM_RENAME_PKT_ERR_EID may be sent
 *       - Error event #FM_RENAME_SRC_INVALID_ERR_EID may be sent
 *       - Error event #FM_RENAME_SRC_DNE_ERR_EID may be sent
 *       - Error event #FM_RENAME_SRC_ISDIR_ERR_EID may be sent
 *       - Error event #FM_RENAME_TGT_INVALID_ERR_EID may be sent
 *       - Error event #FM_RENAME_TGT_EXIST_ERR_EID may be sent
 *       - Error event #FM_RENAME_TGT_ISDIR_ERR_EID may be sent
 *       - Error event #FM_RENAME_CHILD_DISABLED_ERR_EID may be sent
 *       - Error event #FM_RENAME_CHILD_FULL_ERR_EID may be sent
 *       - Error event #FM_RENAME_CHILD_BROKEN_ERR_EID may be sent
 *       - Error event #FM_RENAME_OS_ERR_EID may be sent
 *
 *  \par Criticality
 *       - There are no critical issues related to this command.
 *
 *  \sa #FM_COPY_FILE_CC, #FM_MOVE_FILE_CC
 */
#define FM_RENAME_FILE_CC 4

/**
 * \brief Delete File
 *
 *  \par Description
 *       This command deletes the source file.
 *       Source must be an existing file that is not open.
 *
 *       Because of the possibility that this command might take a very long time
 *       to complete, command argument validation will be done immediately but
 *       deleting the file will be performed by a lower priority child task.
 *       As such, the command result for this function only refers to the result
 *       of command argument verification and being able to place the command on
 *       the child task interface queue.
 *
 *  \par Command Packet Structure
 *       #FM_DeleteFileCmd_t
 *
 *  \par Command Success Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandCounter will increment after validation
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdCounter will increment after completion
 *       - Informational event #FM_DELETE_CMD_INF_EID will be sent
 *
 *  \par Command Error Conditions
 *       - Invalid command packet length
 *       - Filename is invalid
 *       - File does not exist
 *       - File is open
 *       - Filename is a directory
 *       - Failure of OS delete function
 *
 *  \par Command Failure Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandErrCounter will increment
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdErrCounter may increment
 *       - Error event #FM_DELETE_PKT_ERR_EID may be sent
 *       - Error event #FM_DELETE_SRC_INVALID_ERR_EID may be sent
 *       - Error event #FM_DELETE_SRC_DNE_ERR_EID may be sent
 *       - Error event #FM_DELETE_SRC_ISDIR_ERR_EID may be sent
 *       - Error event #FM_DELETE_SRC_OPEN_ERR_EID may be sent
 *       - Error event #FM_DELETE_CHILD_DISABLED_ERR_EID may be sent
 *       - Error event #FM_DELETE_CHILD_FULL_ERR_EID may be sent
 *       - Error event #FM_DELETE_CHILD_BROKEN_ERR_EID may be sent
 *       - Error event #FM_DELETE_OS_ERR_EID may be sent
 *
 *  \par Criticality
 *       The FM application does not provide a method to restore deleted
 *       files.  Critical data may be lost when deleting files.
 *
 *  \sa #FM_DELETE_ALL_FILES_CC, #FM_DELETE_DIRECTORY_CC
 */
#define FM_DELETE_FILE_CC 5

/**
 * \brief Delete All Files
 *
 *  \par Description
 *       This command deletes all files in the source directory.
 *       Source must be an existing directory.
 *       Open files and sub-directories are not deleted.
 *
 *       Because of the possibility that this command might take a very long time
 *       to complete, command argument validation will be done immediately but
 *       reading the directory and deleting each file will be performed by a
 *       lower priority child task.
 *       As such, the return value for this function only refers to the result
 *       of command argument verification and being able to place the command on
 *       the child task interface queue.
 *
 *  \par Command Packet Structure
 *       #FM_DeleteAllFilesCmd_t
 *
 *  \par Command Success Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandCounter will increment after validation
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdCounter will increment after completion
 *       - Informational event #FM_DELETE_ALL_CMD_INF_EID will be sent
 *
 *  \par Command Warning Conditions
 *       - Directory entry is not a file (sub-directory)
 *       - Directory entry is an open file
 *
 *  \par Command Warning Verification
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdWarnCounter will increment
 *       - Informational event #FM_DELETE_ALL_FILES_ND_WARNING_EID may be sent
 *       - Informational event #FM_DELETE_ALL_SKIP_WARNING_EID may be sent
 *
 *  \par Command Error Conditions
 *       - Invalid command packet length
 *       - Invalid directory name
 *       - Directory does not exist
 *       - Directory name + separator + filename is too long
 *       - Failure of OS delete function
 *
 *  \par Command Failure Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandErrCounter may increment
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdErrCounter may increment
 *       - Error event #FM_DELETE_ALL_PKT_ERR_EID may be sent
 *       - Error event #FM_DELETE_ALL_SRC_INVALID_ERR_EID may be sent
 *       - Error event #FM_DELETE_ALL_SRC_DNE_ERR_EID may be sent
 *       - Error event #FM_DELETE_ALL_SRC_FILE_ERR_EID may be sent
 *       - Error event #FM_DELETE_ALL_CHILD_DISABLED_ERR_EID may be sent
 *       - Error event #FM_DELETE_ALL_CHILD_FULL_ERR_EID may be sent
 *       - Error event #FM_DELETE_ALL_CHILD_BROKEN_ERR_EID may be sent
 *       - Error event #FM_DELETE_ALL_OS_ERR_EID may be sent
 *
 *  \par Criticality
 *       The FM application does not provide a method to restore deleted
 *       files.  Critical data may be lost when deleting files.  Also,
 *       deleting a very large number of files may consume more CPU resource
 *       than anticipated.
 *
 *  \sa #FM_DELETE_FILE_CC, #FM_DELETE_DIRECTORY_CC
 */
#define FM_DELETE_ALL_FILES_CC 7

/**
 * \brief Decompress File
 *
 *  \par Description
 *       This command invokes a CFE function to decompress the source
 *       file into the target file.
 *       Source must be an existing file and target must not exist.
 *       Source and target may be on different file systems.
 *
 *       Because of the possibility that this command might take a very long time
 *       to complete, command argument validation will be done immediately but
 *       decompressing the source file into the target file will be performed by
 *       a lower priority child task.
 *       As such, the return value for this function only refers to the result
 *       of command argument verification and being able to place the command on
 *       the child task interface queue.
 *
 *       This command will only have an effect if FM is compiled with a decompression
 *       algorithm enabled.  If compression is not enabled, issuing this command
 *       will generate an error event.
 *
 *  \par Command Packet Structure
 *       #FM_DecompressFileCmd_t
 *
 *  \par Command Success Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandCounter will increment after validation
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdCounter will increment after completion
 *       - Informational event #FM_DECOM_CMD_INF_EID will be sent
 *
 *  \par Command Error Conditions
 *       - Invalid command packet length
 *       - Invalid source filename
 *       - Source file does not exist
 *       - Invalid target filename
 *       - Target file does exist
 *       - Failure of CFE_FS_Decompress function
 *
 *  \par Command Failure Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandErrCounter may increment
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdErrCounter may increment
 *       - Error event #FM_DECOM_PKT_ERR_EID may be sent
 *       - Error event #FM_DECOM_SRC_INVALID_ERR_EID may be sent
 *       - Error event #FM_DECOM_SRC_DNE_ERR_EID may be sent
 *       - Error event #FM_DECOM_SRC_ISDIR_ERR_EID may be sent
 *       - Error event #FM_DECOM_SRC_OPEN_ERR_EID may be sent
 *       - Error event #FM_DECOM_TGT_INVALID_ERR_EID may be sent
 *       - Error event #FM_DECOM_TGT_EXIST_ERR_EID may be sent
 *       - Error event #FM_DECOM_TGT_ISDIR_ERR_EID may be sent
 *       - Error event #FM_DECOM_CHILD_DISABLED_ERR_EID may be sent
 *       - Error event #FM_DECOM_CHILD_FULL_ERR_EID may be sent
 *       - Error event #FM_DECOM_CHILD_BROKEN_ERR_EID may be sent
 *       - Error event #FM_DECOM_CFE_ERR_EID may be sent
 *
 *  \par Criticality
 *       Decompressing a very large file may consume more CPU resource than
 *       anticipated.
 */
#define FM_DECOMPRESS_FILE_CC 8

/**
 * \brief Concatenate Files
 *
 *  \par Description
 *       This command concatenates two source files into the target
 *       file.
 *       Sources must both be existing files and target must not exist.
 *       Sources and target may be on different file systems.
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
 *  \par Command Packet Structure
 *       #FM_ConcatFilesCmd_t
 *
 *  \par Command Success Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandCounter will increment after validation
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdCounter will increment after completion
 *       - Informational event #FM_CONCAT_CMD_INF_EID will be sent
 *
 *  \par Command Error Conditions
 *       - Invalid command packet length
 *       - Invalid source filename
 *       - Source file does not exist
 *       - Invalid target filename
 *       - Target file does exist
 *       - Failure of OS function (copy, open, read, write, etc.)
 *
 *  \par Command Failure Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandErrCounter may increment
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdErrCounter may increment
 *       - Error event #FM_CONCAT_PKT_ERR_EID may be sent
 *       - Error event #FM_CONCAT_OSCPY_ERR_EID may be sent
 *       - Error event #FM_CONCAT_OPEN_SRC2_ERR_EID may be sent
 *       - Error event #FM_CONCAT_OPEN_TGT_ERR_EID may be sent
 *       - Error event #FM_CONCAT_OSRD_ERR_EID may be sent
 *       - Error event #FM_CONCAT_OSWR_ERR_EID may be sent
 *       - Error event #FM_CONCAT_SRC1_INVALID_ERR_EID may be sent
 *       - Error event #FM_CONCAT_SRC1_DNE_ERR_EID may be sent
 *       - Error event #FM_CONCAT_SRC1_ISDIR_ERR_EID may be sent
 *       - Error event #FM_CONCAT_SRC1_OPEN_ERR_EID
 *       - Error event #FM_CONCAT_SRC2_INVALID_ERR_EID may be sent
 *       - Error event #FM_CONCAT_SRC2_DNE_ERR_EID may be sent
 *       - Error event #FM_CONCAT_SRC2_ISDIR_ERR_EID may be sent
 *       - Error event #FM_CONCAT_SRC2_OPEN_ERR_EID may be sent
 *       - Error event #FM_CONCAT_TGT_INVALID_ERR_EID may be sent
 *       - Error event #FM_CONCAT_TGT_EXIST_ERR_EID may be sent
 *       - Error event #FM_CONCAT_TGT_ISDIR_ERR_EID may be sent
 *       - Error event #FM_CONCAT_CHILD_DISABLED_ERR_EID may be sent
 *       - Error event #FM_CONCAT_CHILD_FULL_ERR_EID may be sent
 *       - Error event #FM_CONCAT_CHILD_BROKEN_ERR_EID may be sent
 *
 *  \par Criticality
 *       Concatenating very large files may consume more CPU resource
 *       than anticipated.
 *
 *  \sa #FM_COPY_FILE_CC
 */
#define FM_CONCAT_FILES_CC 9

/**
 * \brief Get File Information
 *
 *  \par Description
 *       This command creates an FM file information telemetry packet
 *       for the source file.
 *       The file information packet includes status that indicates
 *       whether source is a file that is open or closed, a directory,
 *       or does not exist.
 *       The file information data also includes a CRC, file size,
 *       last modify time and the source name.
 *
 *       Because of the possibility that this command might take a very long time
 *       to complete, command argument validation will be done immediately but
 *       collecting the status data and calculating the CRC will be performed by
 *       a lower priority child task.
 *       As such, the return value for this function only refers to the result
 *       of command argument verification and being able to place the command on
 *       the child task interface queue.
 *
 *  \par Command Packet Structure
 *       #FM_GetFileInfoCmd_t
 *
 *  \par Command Success Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandCounter will increment after validation
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdCounter will increment after completion
 *       - Informational event #FM_GET_FILE_INFO_CMD_INF_EID will be sent
 *
 *  \par Command Warning Conditions
 *       - File is open and CRC cannot be calculated
 *       - Specified CRC type is not valid
 *       - CRC cannot be calculated because file cannot be read
 *
 *  \par Command Warning Verification
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdWarnCounter will increment
 *       - Informational event #FM_GET_FILE_INFO_STATE_WARNING_EID may be sent
 *       - Informational event #FM_GET_FILE_INFO_TYPE_WARNING_EID may be sent
 *       - Informational event #FM_GET_FILE_INFO_READ_WARNING_EID may be sent
 *
 *  \par Command Error Conditions
 *       - Invalid command packet length
 *       - Invalid source filename
 *       - Failure of OS_stat function
 *
 *  \par Command Failure Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandErrCounter may increment
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdErrCounter may increment
 *       - Error event #FM_GET_FILE_INFO_OPEN_ERR_EID may be sent
 *       - Error event #FM_GET_FILE_INFO_PKT_ERR_EID may be sent
 *       - Error event #FM_GET_FILE_INFO_SRC_ERR_EID may be sent
 *       - Error event #FM_FILE_INFO_CHILD_DISABLED_ERR_EID may be sent
 *       - Error event #FM_FILE_INFO_CHILD_FULL_ERR_EID may be sent
 *       - Error event #FM_FILE_INFO_CHILD_BROKEN_ERR_EID may be sent
 *
 *  \par Criticality
 *       Calculating the CRC for a very large file may consume more CPU resource than
 *       anticipated.
 *
 *  \sa #FM_GET_OPEN_FILES_CC, #FM_GET_DIR_LIST_FILE_CC, #FM_GET_DIR_LIST_PKT_CC
 */
#define FM_GET_FILE_INFO_CC 10

/**
 * \brief Get Open Files Listing
 *
 *  \par Description
 *       This command creates an FM open files telemetry packet.
 *       The open files packet includes the number of open files and
 *       for each open file, the name of the file and the name of the
 *       application that has the file opened.
 *
 *  \par Command Packet Structure
 *       #FM_GetOpenFilesCmd_t
 *
 *  \par Command Success Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandCounter will increment
 *       - Informational event #FM_GET_OPEN_FILES_CMD_INF_EID will be sent
 *
 *  \par Command Error Conditions
 *       - Invalid command packet length
 *
 *  \par Command Failure Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandErrCounter will increment
 *       - Error event #FM_GET_OPEN_FILES_PKT_ERR_EID will be sent
 *
 *  \par Criticality
 *       - There are no critical issues related to this command.
 *
 *  \sa #FM_GET_FILE_INFO_CC, #FM_GET_DIR_LIST_FILE_CC, #FM_GET_DIR_LIST_PKT_CC
 */
#define FM_GET_OPEN_FILES_CC 11

/**
 * \brief Create Directory
 *
 *  \par Description
 *       This command creates the source directory.
 *       Source must be a valid directory name that does not exist.
 *
 *       Because of the possibility that this command might take a very long time
 *       to complete, command argument validation will be done immediately but
 *       creation of the directory will be performed by a lower priority child task.
 *       As such, the command result for this function only refers to the result
 *       of command argument verification and being able to place the command on
 *       the child task interface queue.
 *
 *  \par Command Packet Structure
 *       #FM_CreateDirectoryCmd_t
 *
 *  \par Command Success Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandCounter will increment after validation
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdCounter will increment after completion
 *       - Informational event #FM_CREATE_DIR_CMD_INF_EID will be sent
 *
 *  \par Command Error Conditions
 *       - Invalid command packet length
 *       - Invalid directory name
 *       - Directory name already exists
 *       - Failure of OS_mkdir function
 *
 *  \par Command Failure Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandErrCounter will increment
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdErrCounter may increment
 *       - Error event #FM_CREATE_DIR_PKT_ERR_EID may be sent
 *       - Error event #FM_CREATE_DIR_SRC_INVALID_ERR_EID may be sent
 *       - Error event #FM_CREATE_DIR_SRC_DNE_ERR_EID may be sent
 *       - Error event #FM_CREATE_DIR_SRC_ISDIR_ERR_EID may be sent
 *       - Error event #FM_CREATE_DIR_CHILD_DISABLED_ERR_EID may be sent
 *       - Error event #FM_CREATE_DIR_CHILD_FULL_ERR_EID may be sent
 *       - Error event #FM_CREATE_DIR_CHILD_BROKEN_ERR_EID may be sent
 *       - Error event #FM_CREATE_DIR_OS_ERR_EID may be sent
 *
 *  \par Criticality
 *       - There are no critical issues related to this command.
 *
 *  \sa #FM_DELETE_DIRECTORY_CC
 */
#define FM_CREATE_DIRECTORY_CC 12

/**
 * \brief Remove Directory
 *
 *  \par Description
 *       This command deletes the source directory, it does
 *       not delete the contents of the directory.
 *       Source must be a valid directory name that exists.
 *
 *       Because of the possibility that this command might take a very long time
 *       to complete, command argument validation will be done immediately but
 *       removal of the directory will be performed by a lower priority child task.
 *       As such, the command result for this function only refers to the result
 *       of command argument verification and being able to place the command on
 *       the child task interface queue.
 *
 *  \par Command Packet Structure
 *       #FM_DeleteDirectoryCmd_t
 *
 *  \par Command Success Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandCounter will increment after validation
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdCounter will increment after completion
 *       - Informational event #FM_DELETE_DIR_CMD_INF_EID will be sent
 *
 *  \par Command Error Conditions
 *       - Invalid command packet length
 *       - Invalid directory name
 *       - Directory does not exist
 *       - Directory is not empty
 *       - Failure of OS function (OS_opendir, OS_rmdir)
 *
 *  \par Command Failure Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandErrCounter will increment
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdErrCounter may increment
 *       - Error event #FM_DELETE_DIR_PKT_ERR_EID may be sent
 *       - Error event #FM_DELETE_DIR_EMPTY_ERR_EID may be sent
 *       - Error event #FM_DELETE_OPENDIR_OS_ERR_EID may be sent
 *       - Error event #FM_DELETE_RMDIR_OS_ERR_EID may be sent
 *       - Error event #FM_DELETE_DIR_SRC_INVALID_ERR_EID may be sent
 *       - Error event #FM_DELETE_DIR_SRC_DNE_ERR_EID may be sent
 *       - Error event #FM_DELETE_DIR_CHILD_DISABLED_ERR_EID may be sent
 *       - Error event #FM_DELETE_DIR_CHILD_FULL_ERR_EID may be sent
 *       - Error event #FM_DELETE_DIR_CHILD_BROKEN_ERR_EID may be sent
 *       - Error event #FM_DELETE_RMDIR_OS_ERR_EID may be sent
 *
 *  \par Criticality
 *       The unexpected loss of a directory may affect a critical
 *       tasks ability to store data.
 *
 *  \sa #FM_CREATE_DIRECTORY_CC
 */
#define FM_DELETE_DIRECTORY_CC 13

/**
 * \brief Get Directory Listing to a File
 *
 *  \par Description
 *       This command writes a listing of the contents of the source
 *       directory to the target file.
 *       If the target filename buffer is empty, then the default
 *       target filename #FM_DIR_LIST_FILE_DEFNAME is used.
 *       The command will overwrite a previous copy of the target
 *       file, if one exists.
 *
 *       Because of the possibility that this command might take a very long time
 *       to complete, command argument validation will be done immediately but
 *       reading the directory will be performed by a lower priority child task.
 *       As such, the return value for this function only refers to the result
 *       of command argument verification and being able to place the command on
 *       the child task interface queue.
 *
 *  \par Command Packet Structure
 *       #FM_GetDirListFileCmd_t
 *
 *  \par Command Success Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandCounter will increment after validation
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdCounter will increment after completion
 *       - Informational event #FM_GET_DIR_FILE_CMD_INF_EID will be sent
 *
 *  \par Command Warning Conditions
 *       - Combined directory and entry name is too long
 *
 *  \par Command Warning Verification
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdWarnCounter will increment
 *       - Informational event #FM_GET_DIR_FILE_WARNING_EID may be sent
 *
 *  \par Command Error Conditions
 *       - Invalid command packet length
 *       - Invalid source directory name
 *       - Source directory does not exist
 *       - Directory name + separator is too long
 *       - Directory name + directory entry is too long
 *       - Invalid target filename
 *       - Target file is already open
 *       - Failure of OS function (OS_opendir, OS_creat, OS_write)
 *
 *  \par Command Failure Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandErrCounter may increment
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdErrCounter may increment
 *       - Error event #FM_GET_DIR_FILE_PKT_ERR_EID may be sent
 *       - Error event #FM_GET_DIR_FILE_OSOPENDIR_ERR_EID may be sent
 *       - Error event #FM_GET_DIR_FILE_WRBLANK_ERR_EID may be sent
 *       - Error event #FM_GET_DIR_FILE_WRHDR_ERR_EID may be sent
 *       - Error event #FM_GET_DIR_FILE_OSCREAT_ERR_EID may be sent
 *       - Error event #FM_GET_DIR_FILE_WRENTRY_ERR_EID may be sent
 *       - Error event #FM_GET_DIR_FILE_UPSTATS_ERR_EID may be sent
 *       - Error event #FM_GET_DIR_FILE_SRC_INVALID_ERR_EID may be sent
 *       - Error event #FM_GET_DIR_FILE_SRC_DNE_ERR_EID may be sent
 *       - Error event #FM_GET_DIR_FILE_SRC_ISDIR_ERR_EID may be sent
 *       - Error event #FM_GET_DIR_FILE_TGT_INVALID_ERR_EID may be sent
 *       - Error event #FM_GET_DIR_FILE_TGT_ISDIR_ERR_EID may be sent
 *       - Error event #FM_GET_DIR_FILE_CHILD_DISABLED_ERR_EID may be sent
 *       - Error event #FM_GET_DIR_FILE_CHILD_FULL_ERR_EID may be sent
 *       - Error event #FM_GET_DIR_FILE_CHILD_BROKEN_ERR_EID may be sent
 *
 *  \par Criticality
 *       Reading a directory that contains thousands of files may consume more CPU
 *       resource than anticipated.
 *
 *  \sa #FM_GET_DIR_LIST_PKT_CC
 */
#define FM_GET_DIR_LIST_FILE_CC 14

/**
 * \brief Get Directory Listing to a Packet
 *
 *  \par Description
 *       This command creates a telemetry packet #FM_DirListPkt_t that
 *       contains a listing of the entries in the specified directory.
 *       Since the packet will likely hold fewer entries than will be
 *       possible in a directory, the command also provides an index
 *       argument to define which entry in the directory is the first
 *       entry reported in the telemetry packet.
 *       After reading the directory list and skipping entries until
 *       reaching the index of the first entry reported, the remaining
 *       entries in the packet are filled sequentially until either
 *       the packet is full or until there are no more entries in the
 *       directory.
 *       The first entry index is zero based - thus, when the first
 *       entry index is zero the first directory entry will be the
 *       first packet entry.
 *       The number of entries per packet #FM_DIR_LIST_PKT_ENTRIES
 *       is a platform configuration definition.
 *
 *       Because of the possibility that this command might take a very long time
 *       to complete, command argument validation will be done immediately but
 *       reading the directory will be performed by a lower priority child task.
 *       As such, the return value for this function only refers to the result
 *       of command argument verification and being able to place the command on
 *       the child task interface queue.
 *
 *  \par Command Packet Structure
 *       #FM_GetDirListPktCmd_t
 *
 *  \par Command Verification
 *       Successful execution of this command may be verified with
 *       the following telemetry:
 *       - #FM_HousekeepingPkt_Payload_t.CommandCounter will increment after validation
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdCounter will increment after completion
 *       - The #FM_DirListPkt_t telemetry packet will be sent
 *       - The #FM_GET_DIR_PKT_CMD_INF_EID informational event will be sent
 *
 *  \par Command Warning Conditions
 *       - Combined directory and entry name is too long
 *
 *  \par Command Warning Verification
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdWarnCounter will increment
 *       - Informational event #FM_GET_DIR_PKT_WARNING_EID may be sent
 *
 *  \par Error Conditions
 *       This command may fail for the following reason(s):
 *       - OS error received opening directory
 *       - OS error received requesting directory size
 *       - OS error received closing directory
 *       - Invalid directory pathname received
 *       - Command packet length not as expected
 *
 *  \par Evidence of failure may be found in the following telemetry:
 *       - #FM_HousekeepingPkt_Payload_t.CommandErrCounter may increment
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdErrCounter may increment
 *       - Error event #FM_GET_DIR_PKT_PKT_ERR_EID may be sent
 *       - Error event #FM_GET_DIR_PKT_OS_ERR_EID may be sent
 *       - Error event #FM_GET_DIR_PKT_SRC_INVALID_ERR_EID may be sent
 *       - Error event #FM_GET_DIR_PKT_SRC_DNE_ERR_EID may be sent
 *       - Error event #FM_GET_DIR_PKT_SRC_ISDIR_ERR_EID may be sent
 *       - Error event #FM_GET_DIR_PKT_CHILD_DISABLED_ERR_EID may be sent
 *       - Error event #FM_GET_DIR_PKT_CHILD_FULL_ERR_EID may be sent
 *       - Error event #FM_GET_DIR_PKT_CHILD_BROKEN_ERR_EID may be sent
 *
 *  \par Criticality
 *       Reading a directory that contains thousands of files may consume more CPU
 *       resource than anticipated.
 *
 *  \sa #FM_GET_DIR_LIST_FILE_CC
 */
#define FM_GET_DIR_LIST_PKT_CC 15

/**
 * \brief Monitor Filesystem Space
 *
 *  \par Description
 *       This command queries the specified location for each of the
 *       enabled entries in the file system monitor table.  The data
 *       is then placed in a telemetry packet and sent to ground.
 *
 *  \par Command Packet Structure
 *       #FM_MonitorFilesystemSpaceCmd_t
 *
 *  \par Evidence of success may be found in the following telemetry:
 *       - #FM_HousekeepingPkt_Payload_t.CommandCounter will increment
 *       - Informational event #FM_MONITOR_FILESYSTEM_SPACE_CMD_INF_EID will be sent
 *       - Telemetry packet #FM_MonitorReportPkt_t will be sent
 *
 *  \par Error Conditions
 *       - Invalid command packet length
 *       - Free space table is not loaded
 *
 *  \par Evidence of failure may be found in the following telemetry:
 *       - #FM_HousekeepingPkt_Payload_t.CommandErrCounter will increment
 *       - Error event #FM_GET_FREE_SPACE_PKT_ERR_EID may be sent
 *       - Error event #FM_GET_FREE_SPACE_TBL_ERR_EID may be sent
 *
 *  \par Criticality
 *       - There are no critical issues related to this command.
 */
#define FM_MONITOR_FILESYSTEM_SPACE_CC 16

/**
 * \brief Set Free Space Table Entry State
 *
 *  \par Description
 *       This command enables or disables a single entry in the FM
 *       file system free space table.  Only table entries that are
 *       currently enabled or disabled may be modified by command.
 *       Unused table entries cannot be modified.
 *
 *  \par Command Packet Structure
 *       #FM_SetTableStateCmd_t
 *
 *  \par Evidence of success may be found in the following telemetry:
 *       - #FM_HousekeepingPkt_Payload_t.CommandCounter will increment
 *       - Informational event #FM_SET_TABLE_STATE_CMD_EID will be sent
 *
 *  \par Error Conditions
 *       - Invalid command packet length
 *       - FM file system free space table has not yet been loaded
 *       - Invalid command argument, table entry index arg is out of range
 *       - Invalid command argument, entry state arg is not enable/disable
 *       - Invalid current table entry state, entry is unused
 *
 *  \par Evidence of failure may be found in the following telemetry:
 *       - #FM_HousekeepingPkt_Payload_t.CommandErrCounter will increment
 *       - Error event #FM_SET_TABLE_STATE_PKT_ERR_EID may be sent
 *       - Error event #FM_SET_TABLE_STATE_TBL_ERR_EID may be sent
 *       - Error event #FM_SET_TABLE_STATE_ARG_IDX_ERR_EID may be sent
 *       - Error event #FM_SET_TABLE_STATE_ARG_STATE_ERR_EID may be sent
 *       - Error event #FM_SET_TABLE_STATE_UNUSED_ERR_EID may be sent
 *
 *  \par Criticality
 *       - There are no critical issues related to this command.
 */
#define FM_SET_TABLE_STATE_CC 17

/**
 * \brief Set Permissions of a file
 *
 *  \par Description
 *       This command sets the permissions for a file. This is a direct interface
 *       to OS_chmod in the OSAL. OS_chmod accepts a uint32 to set the file's mode.
 *
 *       Examples for a regular file:
 *
 *       OS_READ_ONLY - Read only file access
 *       OS_WRITE_ONLY - Write only file access
 *       OS_READ_WRITE - Read write file access
 *
 *       Because of the possibility that this command might take a very long time
 *       to complete, command argument validation will be done immediately but
 *       setting permissions will be performed by a lower priority child task.
 *       As such, the command result for this function only refers to the result
 *       of command argument verification and being able to place the command on
 *       the child task interface queue.
 *
 *  \par Command Packet Structure
 *       #FM_SetPermissionsCmd_t
 *
 *  \par Command Success Verification
 *       - #FM_HousekeepingPkt_Payload_t.CommandCounter will increment after validation
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdCounter will increment after completion
 *       - Informational event #FM_SET_PERM_CMD_INF_EID will be sent
 *
 *  \par Error Conditions
 *       - Invalid command packet length
 *       - Error from call to OS_chmod
 *
 *  \par Evidence of failure may be found in the following telemetry:
 *       - #FM_HousekeepingPkt_Payload_t.CommandErrCounter may increment
 *       - #FM_HousekeepingPkt_Payload_t.ChildCmdErrCounter may increment
 *       - Error event #FM_SET_PERM_ERR_EID may be sent
 *       - Error event #FM_SET_PERM_OS_ERR_EID may be sent
 *
 *  \par Criticality
 *       - There are no critical issues related to this command.
 */
#define FM_SET_PERMISSIONS_CC 19

/**\}*/

#endif
