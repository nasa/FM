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
 *   Specification for the CFS File Manager Event Identifiers.
 */
#ifndef FM_EVENTS_H
#define FM_EVENTS_H

/**
 * \defgroup cfsfmevents CFS File Manager Event IDs
 * \{
 */

/**
 * \brief FM Initialization Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message is issued after the File Manager application has
 *  successfully completed startup initialization.
 */
#define FM_INIT_INF_EID 1

/**
 * \brief FM Initialization Register For Event Services Failed Event ID
 *
 *  \par Type: Error
 *
 *  \par Cause
 *
 *  This event message is issued when the File Manager application has
 *  failed in its attempt to register for event services during startup
 *  initialization.
 *
 *  This is a fatal error that will cause the File Manager application
 *  to terminate.
 */
#define FM_STARTUP_EVENTS_ERR_EID 2

/**
 * \brief FM Initialization Create SB Input Pipe Failed Event ID
 *
 *  \par Type: Error
 *
 *  \par Cause
 *
 *  This event message is issued when the File Manager application has
 *  failed in its attempt to create a Software Bus input pipe during startup
 *  initialization.
 *
 *  This is a fatal error that will cause the File Manager application
 *  to terminate.
 */
#define FM_CR_PIPE_ERR_EID 3

/**
 * \brief FM Initialization Subscribe to HK Request Failed Event ID
 *
 *  \par Type: Error
 *
 *  \par Cause
 *
 *  This event message is issued when the File Manager application has
 *  failed in its attempt to subscribe to the HK telemetry request command
 *  during startup initialization.
 *
 *  This is a fatal error that will cause the File Manager application
 *  to terminate.
 */
#define FM_STARTUP_SUBSCRIB_HK_ERR_EID 4

/**
 * \brief FM Initialization Subscribe to FM Commands Failed Event ID
 *
 *  \par Type: Error
 *
 *  \par Cause
 *
 *  This event message is issued when the File Manager application has
 *  failed in its attempt to subscribe to the FM ground command packet
 *  during startup initialization.
 *
 *  This is a fatal error that will cause the File Manager application
 *  to terminate.
 */
#define FM_STARTUP_SUBSCRIB_GCMD_ERR_EID 5

/**
 * \brief FM Initialization Register Free Space Table Failed Event ID
 *
 *  \par Type: Error
 *
 *  \par Cause
 *
 *  This event message is issued when the File Manager application has
 *  failed in its attempt to register its file system free space table
 *  during startup initialization.
 *
 *  This is a fatal error that will cause the File Manager application
 *  to terminate.
 */
#define FM_STARTUP_TABLE_INIT_ERR_EID 6

/**
 * \brief FM Main Loop Receive from Software Bus Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is issued when the File Manager application has
 *  failed in its attempt to read from its Software Bus input pipe while
 *  processing the software main loop sequence.
 *
 *  This is a fatal error that will cause the File Manager application
 *  to terminate.
 */
#define FM_SB_RECEIVE_ERR_EID 7

/**
 * \brief FM Application Termination Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is issued when the File Manager application is about
 *  to terminate.
 */
#define FM_EXIT_ERR_EID 8

/**
 * \brief FM Main Loop Message ID Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is issued when the File Manager application has
 *  received an unexpected Software Bus packet.  There is no obvious
 *  explanation of why or how FM could receive such a packet.
 */
#define FM_MID_ERR_EID 9

/**
 * \brief FM Main Loop Command Code Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is issued when the File Manager application has
 *  received a command packet with an unexpected command code value.
 *
 *  Mal-formed command packets are generally prevented by the ground
 *  system.  Therefore, the source for the problem command is likely
 *  to be one of the on-board tables that contain commands.
 */
#define FM_CC_ERR_EID 10

/**
 * \brief FM Command Packet Length Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a housekeeping
 *  request command packet with an invalid length.
 */
#define FM_HKREQ_LEN_ERR_EID 11

/**
 * \brief FM No-op Command Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message signals the successful completion of a
 *  /FM_Noop command.
 */
#define FM_NOOP_INF_EID 12

/**
 * \brief FM No-op Command Length Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Noop
 *  command packet with an invalid length.
 */
#define FM_NOOP_PKT_ERR_EID 13

/**
 * \brief FM Reset Counters Command Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message signals the successful completion of a
 *  /FM_ResetCtrs command.
 */
#define FM_RESET_INF_EID 14

/**
 * \brief FM Reset Counters Command Length Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_ResetCtrs
 *  command packet with an invalid length.
 */
#define FM_RESET_PKT_ERR_EID 15

/**
 * \brief FM Copy File Command Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message signals the successful completion of a
 *  /FM_Copy command.
 *
 *  Note that the execution of this command generally occurs within the
 *  context of the FM low priority child task.  Thus this event may not
 *  occur until some time after the command was invoked.  However, this
 *  event message does signal the actual completion of the command.
 */
#define FM_COPY_CMD_INF_EID 16

/**
 * \brief FM Copy File Command Length Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Copy
 *  command packet with an invalid length.
 */
#define FM_COPY_PKT_ERR_EID 17

/**
 * \brief FM Copy File Command Overwrite Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Copy
 *  command packet with an invalid overwrite argument.  Overwrite
 *  must be set to TRUE (one) or FALSE (zero).
 */
#define FM_COPY_OVR_ERR_EID 18

/**
 * \brief FM Copy File Command OS Error Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated due to an OS function error that
 *  occurred after preliminary command argument verification tests
 *  indicated that the source file exists and the target name
 *  is unused and appears to be valid. Verify that the target
 *  filename is reasonable.  Also, verify that the file system has
 *  sufficient free space for this operation. Then refer to the OS
 *  specific return value.
 */
#define FM_COPY_OS_ERR_EID 19

/**
 * \brief FM Move File Command Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message signals the successful completion of a
 *  /FM_Move command.
 */
#define FM_MOVE_CMD_INF_EID 20

/**
 * \brief FM Move File Command Length Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Move
 *  command packet with an invalid length.
 */
#define FM_MOVE_PKT_ERR_EID 21

/**
 * \brief FM Move File Command Overwrite Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Move
 *  command packet with an invalid overwrite argument.  Overwrite
 *  must be set to TRUE (one) or FALSE (zero).
 */
#define FM_MOVE_OVR_ERR_EID 22

/**
 * \brief FM Move File Command OS Error Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated due to an OS function error that
 *  occurred after preliminary command argument verification tests
 *  indicated that the source file exists and the target name
 *  is unused and appears to be valid. Verify that the target
 *  filename is reasonable.  Also, verify that the file system has
 *  sufficient free space for this operation. Then refer to the OS
 *  specific return value.
 */
#define FM_MOVE_OS_ERR_EID 23

/**
 * \brief FM Rename File Command Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message signals the successful completion of a
 *  /FM_Rename command.
 */
#define FM_RENAME_CMD_INF_EID 24

/**
 * \brief FM Rename File Command Length Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Rename
 *  command packet with an invalid length.
 */
#define FM_RENAME_PKT_ERR_EID 25

/**
 * \brief FM Rename File Command Overwrite Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Rename
 *  command packet with an invalid overwrite argument.  Overwrite
 *  must be set to TRUE (one) or FALSE (zero).
 */
#define FM_RENAME_OVR_ERR_EID 26

/**
 * \brief FM Rename File Command OS Error Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated due to an OS function error that
 *  occurred after preliminary command argument verification tests
 *  indicated that the source file exists and the target name
 *  is unused and appears to be valid. Verify that the target
 *  filename is reasonable.  Also, verify that the file system has
 *  sufficient free space for this operation. Then refer to the OS
 *  specific return value.
 */
#define FM_RENAME_OS_ERR_EID 27

/**
 * \brief FM Delete File Command Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message signals the successful completion of a
 *  /FM_Delete command.
 */
#define FM_DELETE_CMD_INF_EID 28

/**
 * \brief FM Delete File Command Length Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Delete
 *  command packet with an invalid length.
 */
#define FM_DELETE_PKT_ERR_EID 29

/**
 * \brief FM Delete File Command OS Error Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated due to an OS function error that
 *  occurred after preliminary command argument verification tests
 *  indicated that the filename exists and is not open. Refer to the
 *  OS-specific return value for an indication of what might have
 *  caused this error.
 */
#define FM_DELETE_OS_ERR_EID 30

/**
 * \brief FM Delete All Files Command Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message signals the successful completion of a
 *  /FM_DeleteAll command.
 *
 *  Note that the execution of this command generally occurs within the
 *  context of the FM low priority child task.  Thus this event may not
 *  occur until some time after the command was invoked.  However, this
 *  event message does signal the actual completion of the command.
 */
#define FM_DELETE_ALL_CMD_INF_EID 31

/**
 * \brief FM Delete All Files Unable To Delete All Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This general event message is issued if for any reason some files
 *  could not be deleted.
 */
#define FM_DELETE_ALL_FILES_ND_WARNING_EID 32

/**
 * \brief FM Delete All Files Directories Skipped Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This general event message is issued if for any reason a directory
 *  skipped when processing a /FM_DeleteAll command.
 */
#define FM_DELETE_ALL_SKIP_WARNING_EID 33

/**
 * \brief FM Delete All Files Command Length Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_DeleteAll
 *  command packet with an invalid length.
 */
#define FM_DELETE_ALL_PKT_ERR_EID 34

/**
 * \brief FM Delete All Files Command OS Error Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated due to an OS function error that
 *  occurred after preliminary command argument verification tests
 *  indicated that the directory exists. Refer to the OS-specific
 *  return value for an indication of what might have caused this
 *  error.
 */
#define FM_DELETE_ALL_OS_ERR_EID 35

/**
 * \brief FM Decompress File Command Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message signals the successful completion of a
 *  /FM_Decompress command.
 *
 *  Note that the execution of this command generally occurs within the
 *  context of the FM low priority child task.  Thus this event may not
 *  occur until some time after the command was invoked.  However, this
 *  event message does signal the actual completion of the command.
 */
#define FM_DECOM_CMD_INF_EID 36

/**
 * \brief FM Decompress File Command Length Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Decompress
 *  command packet with an invalid length.
 */
#define FM_DECOM_PKT_ERR_EID 37

/**
 * \brief FM Decompress File Decompression Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated due to an API function error that
 *  occurred after preliminary command argument verification tests
 *  indicated that the source file exists. Refer to the function
 *  specific return value for an indication of what might have caused
 *  this particular error.
 */
#define FM_DECOM_CFE_ERR_EID 38

/**
 * \brief FM Concat Files Command Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message signals the successful completion of a
 *  /FM_Concat command.
 *
 *  Note that the execution of this command generally occurs within the
 *  context of the FM low priority child task.  Thus this event may not
 *  occur until some time after the command was invoked.  However, this
 *  event message does signal the actual completion of the command.
 */
#define FM_CONCAT_CMD_INF_EID 39

/**
 * \brief FM Concat Files Command Length Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Concat
 *  command packet with an invalid length.
 */
#define FM_CONCAT_PKT_ERR_EID 40

/**
 * \brief FM Concat Files Copy Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the first source file cannot be
 *  copied.
 *
 *  This event message is generated due to an API function error that
 *  occurred after preliminary command argument verification tests
 *  indicated that the source files exist. Refer to the function
 *  specific return value for an indication of what might have caused
 *  this particular error.
 */
#define FM_CONCAT_OSCPY_ERR_EID 41

/**
 * \brief FM Concat Files Command Open Second Source File Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the second source file cannot
 *  be opened.
 *
 *  This event message is generated due to an API function error that
 *  occurred after preliminary command argument verification tests
 *  indicated that the source files exist. Refer to the function
 *  specific return value for an indication of what might have caused
 *  this particular error.
 */
#define FM_CONCAT_OPEN_SRC2_ERR_EID 42

/**
 * \brief FM Concat Files Command Open Target File Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the target file cannot
 *  be opened.
 *
 *  This event message is generated due to an API function error that
 *  occurred after preliminary command argument verification tests
 *  indicated that the source files exist. Refer to the function
 *  specific return value for an indication of what might have caused
 *  this particular error.
 */
#define FM_CONCAT_OPEN_TGT_ERR_EID 43

/**
 * \brief FM Concat Files Command Read Second Source File Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the second source file cannot be read.
 *
 *  This event message is generated due to an API function error that
 *  occurred after preliminary command argument verification tests
 *  indicated that the source files exist. Refer to the function
 *  specific return value for an indication of what might have caused
 *  this particular error.
 */
#define FM_CONCAT_OSRD_ERR_EID 44

/**
 * \brief FM Concat Files Command Write Target File Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the target file cannot be written.
 *
 *  This event message is generated due to an API function error that
 *  occurred after preliminary command argument verification tests
 *  indicated that the source files exist. Refer to the function
 *  specific return value for an indication of what might have caused
 *  this particular error.
 */
#define FM_CONCAT_OSWR_ERR_EID 45

/**
 * \brief FM Get File Info Command Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message signals the successful completion of a
 *  /FM_GetFileInfo command.
 *
 *  Note that the execution of this command generally occurs within the
 *  context of the FM low priority child task.  Thus this event may not
 *  occur until some time after the command was invoked.  However, this
 *  event message does signal the actual completion of the command.
 */
#define FM_GET_FILE_INFO_CMD_INF_EID 46

/**
 * \brief FM Get File Info Unable To Compute CRC File State Invalid Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message is generated when the CRC of a file cannot be
 *  computed because the file has an invalid state.
 *
 *  This event message is generated due to an API function error that
 *  occurred after preliminary command argument verification tests
 *  indicated that the source files exist. Refer to the function
 *  specific return value for an indication of what might have caused
 *  this particular error.
 */
#define FM_GET_FILE_INFO_STATE_WARNING_EID 47

/**
 * \brief FM Get File Info Unable To Compute CRC, CRC Type Invalid Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message is generated when the CRC of a file cannot be
 *  computed because the CRC type is invalid.
 *
 *  This event message is generated due to an API function error that
 *  occurred after preliminary command argument verification tests
 *  indicated that the source files exist. Refer to the function
 *  specific return value for an indication of what might have caused
 *  this particular error.
 */
#define FM_GET_FILE_INFO_TYPE_WARNING_EID 48

/**
 * \brief FM Get File Info Unable To Compute CRC File Open Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the CRC of a file cannot be
 *  computed because the file cannot be opened.
 *
 *  This event message is generated due to an API function error that
 *  occurred after preliminary command argument verification tests
 *  indicated that the source files exist. Refer to the function
 *  specific return value for an indication of what might have caused
 *  this particular error.
 */
#define FM_GET_FILE_INFO_OPEN_ERR_EID 49

/**
 * \brief FM Get File Info Unable To Compute CRC File Read Failed Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message is generated when the CRC of a file cannot be
 *  computed because the file cannot be read.
 *
 *  This event message is generated due to an API function error that
 *  occurred after preliminary command argument verification tests
 *  indicated that the source files exist. Refer to the function
 *  specific return value for an indication of what might have caused
 *  this particular error.
 */
#define FM_GET_FILE_INFO_READ_WARNING_EID 50

/**
 * \brief FM Get File Info Command Length Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_GetFileInfo
 *  command packet with an invalid length.
 */
#define FM_GET_FILE_INFO_PKT_ERR_EID 51

/**
 * \brief FM Get File Info Command Filename Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_GetFileInfo
 *  command packet with an invalid filename.
 */
#define FM_GET_FILE_INFO_SRC_ERR_EID 52

/**
 * \brief FM Get Open Files Command Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message signals the successful completion of a
 *  /FM_GetOpenFiles command.
 */
#define FM_GET_OPEN_FILES_CMD_INF_EID 53

/**
 * \brief FM Get Open Files Command Length Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_GetOpenFiles
 *  command packet with an invalid length.
 */
#define FM_GET_OPEN_FILES_PKT_ERR_EID 54

/**
 * \brief FM Create Directory Command Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message signals the successful completion of a
 *  /FM_CreateDir command.
 */
#define FM_CREATE_DIR_CMD_INF_EID 55

/**
 * \brief FM Create Directory Command Length Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_CreateDir
 *  command packet with an invalid length.
 */
#define FM_CREATE_DIR_PKT_ERR_EID 56

/**
 * \brief FM Create Directory Command OS Error Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated due to an OS function error that
 *  occurred after preliminary command argument verification tests
 *  indicated that the directory name is unused and appears to be
 *  valid. Refer to the OS specific return value.
 */
#define FM_CREATE_DIR_OS_ERR_EID 57

/**
 * \brief FM Delete Directory Command Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message signals the successful completion of a
 *  /FM_DeleteDir command.
 */
#define FM_DELETE_DIR_CMD_INF_EID 58

/**
 * \brief FM Delete Directory Command Length Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_DeleteDir
 *  command packet with an invalid length.
 */
#define FM_DELETE_DIR_PKT_ERR_EID 59

/**
 * \brief FM Delete Directory Command Failed Directory Not Empty Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_DeleteDir
 *  command packet that references a directory that is not empty.
 */
#define FM_DELETE_DIR_EMPTY_ERR_EID 60

/**
 * \brief FM Delete Directory, Direcotry Open Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated due to an OS function error that
 *  occurred after preliminary command argument verification tests
 *  indicated that the directory exists and appears to be valid.
 *  Refer to the OS specific return values.
 */
#define FM_DELETE_OPENDIR_OS_ERR_EID 61

/**
 * \brief FM Delete Directory Remove Directory Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated due to an OS function error that
 *  occurred after preliminary command argument verification tests
 *  indicated that the directory exists and appears to be valid.
 *  Refer to the OS specific return values.
 */
#define FM_DELETE_RMDIR_OS_ERR_EID 62

/**
 * \brief FM Directory List To File Command Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message signals the successful completion of a
 *  /FM_GetDirFile command.
 *
 *  Note that the execution of this command generally occurs within the
 *  context of the FM low priority child task.  Thus this event may not
 *  occur until some time after the command was invoked.  However, this
 *  event message does signal the actual completion of the command.
 */
#define FM_GET_DIR_FILE_CMD_INF_EID 63

/**
 * \brief FM Directory List To File Command Length Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_GetDirFile
 *  command packet with an invalid length.
 */
#define FM_GET_DIR_FILE_PKT_ERR_EID 64

/**
 * \brief FM Directory List To File Command Combined Path and Name Too Long Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message is generated when the combined length of the
 *  directory name plus the directory entry name exceeds the maximum
 *  qualified filename length.  It is unclear how this condition
 *  might arise, but since we are copying both strings into a fixed
 *  length buffer, we must first verify the length.
 *
 *  The /FM_GetDirFile command handler will not write information
 *  regarding this directory entry to the output file.
 */
#define FM_GET_DIR_FILE_WARNING_EID 65

/**
 * \brief FM Directory List To File Directory Open Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated due to an OS function error that
 *  occurred after preliminary command argument verification tests
 *  indicated that the directory exists and the output filename
 *  is unused and appears to be valid. Verify that the output
 *  filename is reasonable.  Also, verify that the file system has
 *  sufficient free space for this operation. Then refer to the OS
 *  specific return values.
 */
#define FM_GET_DIR_FILE_OSOPENDIR_ERR_EID 66

/**
 * \brief FM Directory List To File Write Blank Stats Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated due to an error when writing a
 *  blank stats structure using the OS_write function.  This
 *  occurred after preliminary command argument verification tests
 *  indicated that the directory exists and the output filename
 *  is unused and appears to be valid. Verify that the output
 *  filename is reasonable.  Also, verify that the file system has
 *  sufficient free space for this operation. Then refer to the OS
 *  specific return values.
 */
#define FM_GET_DIR_FILE_WRBLANK_ERR_EID 67

/**
 * \brief FM Directory List To File Write Header Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the header cannot be written
 *  to the file using #CFE_FS_WriteHeader.  This error
 *  occurred after preliminary command argument verification tests
 *  indicated that the directory exists and the output filename
 *  is unused and appears to be valid. Verify that the output
 *  filename is reasonable.  Also, verify that the file system has
 *  sufficient free space for this operation. Then refer to the OS
 *  specific return values.
 */
#define FM_GET_DIR_FILE_WRHDR_ERR_EID 68

/**
 * \brief FM Directory List To File Create File Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated due to an OS function error that
 *  prevents the output file from being created.  This error
 *  occurred after preliminary command argument verification tests
 *  indicated that the directory exists and the output filename
 *  is unused and appears to be valid. Verify that the output
 *  filename is reasonable.  Also, verify that the file system has
 *  sufficient free space for this operation. Then refer to the OS
 *  specific return values.
 */
#define FM_GET_DIR_FILE_OSCREAT_ERR_EID 69

/**
 * \brief FM Directory List To File Write Entry Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated due to an OS function error that
 *  prevents an entry from being written.  This error
 *  occurred after preliminary command argument verification tests
 *  indicated that the directory exists and the output filename
 *  is unused and appears to be valid. Verify that the output
 *  filename is reasonable.  Also, verify that the file system has
 *  sufficient free space for this operation. Then refer to the OS
 *  specific return values.
 */
#define FM_GET_DIR_FILE_WRENTRY_ERR_EID 70

/**
 * \brief FM Directory List To File Write Update Stats Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated due to an OS function error that
 *  prevents updated statistics from being written to a file.  This
 *  occurred after preliminary command argument verification tests
 *  indicated that the directory exists and the output filename
 *  is unused and appears to be valid. Verify that the output
 *  filename is reasonable.  Also, verify that the file system has
 *  sufficient free space for this operation. Then refer to the OS
 *  specific return values.
 */
#define FM_GET_DIR_FILE_UPSTATS_ERR_EID 71

/**
 * \brief FM Directory List To Packet Command Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message signals the successful completion of a
 *  /FM_GetDirPkt command.
 *
 *  Note that the execution of this command generally occurs within the
 *  context of the FM low priority child task.  Thus this event may not
 *  occur until some time after the command was invoked.  However, this
 *  event message does signal the actual completion of the command.
 */
#define FM_GET_DIR_PKT_CMD_INF_EID 72

/**
 * \brief FM Directory List To Packet Command Directory and Entry Too Long Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message is generated when the combined length of the
 *  directory name plus the directory entry name exceeds the maximum
 *  qualified filename length.  It is unclear how this condition
 *  might arise, but since we are copying both strings into a fixed
 *  length buffer, we must first verify the length.
 */
#define FM_GET_DIR_PKT_WARNING_EID 73

/**
 * \brief FM Directory List To Packet Command Length Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_GetDirPkt
 *  command packet with an invalid length.
 */
#define FM_GET_DIR_PKT_PKT_ERR_EID 74

/**
 * \brief FM Directory List To Packet Directory Open Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  The numeric data in the event is the return value from the OS
 *  function call.  The string data identifies the name of the
 *  directory or the directory entry.
 */
#define FM_GET_DIR_PKT_OS_ERR_EID 75

/**
 * \brief FM Monitor Filesystem Command Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message signals the successful completion of a
 *  /FM_MonitorFilesystemSpace command.
 */
#define FM_MONITOR_FILESYSTEM_SPACE_CMD_INF_EID 76

/**
 * \brief FM Get Free Space Command Length Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_GetFreeSpace
 *  command packet with an invalid length.
 */
#define FM_GET_FREE_SPACE_PKT_ERR_EID 77

/**
 * \brief FM Get Free Space Table Not Loaded Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_GetFreeSpace
 *  command packet when the FM file system free space table has not yet
 *  been loaded.
 */
#define FM_GET_FREE_SPACE_TBL_ERR_EID 78

/**
 * \brief FM Set Table State Command Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message signals the successful completion of a
 *  /FM_SetTableState command.
 */
#define FM_SET_TABLE_STATE_CMD_EID 79

/**
 * \brief FM Set Table State Command Length Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_SetTableState
 *  command packet with an invalid length.
 */
#define FM_SET_TABLE_STATE_PKT_ERR_EID 80

/**
 * \brief FM Set Table State Command Table Not Loaded Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_SetTableState
 *  command packet when the FM file system free space table has not yet
 *  been loaded.
 */
#define FM_SET_TABLE_STATE_TBL_ERR_EID 81

/**
 * \brief FM Set Table State Command Index Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a FM_SetTableState
 *  command packet with an invalid table index argument.
 */
#define FM_SET_TABLE_STATE_ARG_IDX_ERR_EID 82

/**
 * \brief FM Set Table State Command State Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a FM_SetTableState
 *  command packet with an invalid entry state argument.
 */
#define FM_SET_TABLE_STATE_ARG_STATE_ERR_EID 83

/**
 * \brief FM Set Table State Command Unused Entry Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_SetTableState
 *  command packet that references an unused free space table entry.
 */
#define FM_SET_TABLE_STATE_UNUSED_ERR_EID 84

/**
 * \brief FM Free Space Table Verification Failed Empty Name Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when a file system free space table fails the table
 *  verification process because the file system name is an empty string.  Each file
 *  system table entry has only 2 fields: table entry state and file system name.
 *  The table entry state field must be either enabled or disabled.  The file system
 *  name string must have a non-zero length, include a string terminator and not
 *  contain characters considered invalid for filenames.
 *
 *  If the file system free space table loaded at startup fails verification, the FM
 *  application will not terminate.  However, the FM application will not process
 *  commands that request the file system free space telemetry packet if a file
 *  system free space table has not been successfully loaded.  Thereafter, if an
 *  attempt to load a new table fails verification, the FM application will continue
 *  to use the previous table.
 */
#define FM_TABLE_VERIFY_EMPTY_ERR_EID 85

/**
 * \brief FM Free Space Table Verification Failed Name Too Long Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when a file system free space table fails the table
 *  verification process because the file system name is too long.  Each file
 *  system table entry has only 2 fields: table entry state and file system name.
 *  The table entry state field must be either enabled or disabled.  The file system
 *  name string must have a non-zero length, include a string terminator and not
 *  contain characters considered invalid for filenames.
 *
 *  If the file system free space table loaded at startup fails verification, the FM
 *  application will not terminate.  However, the FM application will not process
 *  commands that request the file system free space telemetry packet if a file
 *  system free space table has not been successfully loaded.  Thereafter, if an
 *  attempt to load a new table fails verification, the FM application will continue
 *  to use the previous table.
 */
#define FM_TABLE_VERIFY_TOOLONG_ERR_EID 86

/**
 * \brief FM Free Space Table Verification Failed State Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when a file system free space table fails the table
 *  verification process because a table entry has an invalid state. Each file
 *  system table entry has only 2 fields: table entry state and file system name.
 *  The table entry state field must be either enabled or disabled.  The file system
 *  name string must have a non-zero length, include a string terminator and not
 *  contain characters considered invalid for filenames.
 *
 *  If the file system free space table loaded at startup fails verification, the FM
 *  application will not terminate.  However, the FM application will not process
 *  commands that request the file system free space telemetry packet if a file
 *  system free space table has not been successfully loaded.  Thereafter, if an
 *  attempt to load a new table fails verification, the FM application will continue
 *  to use the previous table.
 */
#define FM_TABLE_VERIFY_BAD_STATE_ERR_EID 88

/**
 * \brief FM Child Task Initialization Complete Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message signals the successful completion of the initialization
 *  process for the FM child task.
 */
#define FM_CHILD_INIT_EID 89

/**
 * \brief FM Child Task Initialization Create Semaphore Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message indicates an unsuccessful attempt to create the sempahore
 *  for the low priority FM child task.  Commands which would have otherwise been
 *  handed off to the child task for execution, will now be processed by the main FM
 *  application.
 */
#define FM_CHILD_INIT_SEM_ERR_EID 90

/**
 * \brief FM Child Task Initialization Create Queue Count Semaphore Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message indicates an unsuccessful attempt to create the queue count
 *  semphore for the FM child task. Commands which would have otherwise been handed off
 *  to the child task for execution, will now be processed by the main FM application.
 */
#define FM_CHILD_INIT_QSEM_ERR_EID 91

/**
 * \brief FM Child Task Initialization Create Task Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message indicates an unsuccessful attempt to create the low
 *  priority FM child task.  Commands which would have otherwise been handed off
 *  to the child task for execution, will now be processed by the main FM application.
 */
#define FM_CHILD_INIT_CREATE_ERR_EID 92

/**
 * \brief FM Child Task Termination Error Empty Queue Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message indicates that the FM child task has suffered a fatal error
 *  and has terminated.  The error occurred because the child queue was empty,
 *  indicating that the handshake between the main task and child task was
 *  broken.
 */
#define FM_CHILD_TERM_EMPTYQ_ERR_EID 93

/**
 * \brief FM Child Task Termination Error Invalid Queue Index Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message indicates that the FM child task has suffered a fatal error
 *  and has terminated.  The error occurred because the child read index was
 *  invalid (larger than the child queue depth).
 */
#define FM_CHILD_TERM_QIDX_ERR_EID 94

/**
 * \brief FM Child Task Termination Error Semaphore Take Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message indicates that the FM child task has suffered a fatal error
 *  and has terminated.  The error occurred when trying to take the child
 *  handshake semaphore.
 */
#define FM_CHILD_TERM_SEM_ERR_EID 95

/**
 * \brief FM Child Task Command Code Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message indicates that the FM child task is unable to process the current
 *  handshake request.  Either the handshake queue index or the handshake command code
 *  is invalid.  This error suggests that either the handshake interface logic is flawed,
 *  or there has been some sort of data corruption that affected the interface data.
 *  It may be necessary to restart the FM application to resync the handshake interface.
 */
#define FM_CHILD_EXE_ERR_EID 96

/**
 *  \brief FM Free Space Table Validation Results Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause:
 *
 *  This event describes the results of the Free Space Table validation
 *  function.  The cFE Table Services Manager will call this function autonomously
 *  when the default table is loaded at startup and also whenever a table validate
 *  command (that targets this table) is processed.
 */
#define FM_TABLE_VERIFY_EID 97

/**
 * \brief FM Set Permissions Command Length Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_SetPermissions
 *  command packet with an invalid length.
 */
#define FM_SET_PERM_ERR_EID 98

/**
 * \brief FM Set Permissions Command Event ID
 *
 *  \par Type: INFORMATION
 *
 *  \par Cause
 *
 *  This event message signals the successful completion of a
 *  /FM_SetPerm command.
 */
#define FM_SET_PERM_CMD_INF_EID 99

/**
 * \brief FM Set Permissions Command Chmod Error Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_SetPerm
 *  command packet with an invalid length.
 */
#define FM_SET_PERM_OS_ERR_EID 100

/**
 * \brief FM Free Space Table Verification Failed Null Pointer Detected
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message occurs when the FM validate table callback function
 *  receives a NULL pointer as the input argument.
 */
#define FM_TABLE_VERIFY_NULL_PTR_ERR_EID 101

/**
 * \brief FM Main Loop Software Bus Returned NULL On Success Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message occurs if the Software Bus returns a success status
 *  in the main loop but provided a NULL pointer as the return argument.
 */
#define FM_SB_RECEIVE_NULL_PTR_ERR_EID 102

/**
 * \brief FM Get Free Space Get File System Stats Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 *  This event message occurs if the free space for a file system cannot be read
 *  when processing the #FM_MonitorFilesystemSpaceCmd command.
 */
#define FM_OS_SYS_STAT_ERR_EID 103

/**
 * \brief FM Directory Size Estimate Failed Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause:
 *
 * This event message occurs if the system encounters an error during
 * calculation of a directory size estimate
 */
#define FM_DIRECTORY_ESTIMATE_ERR_EID 104

/** -------------------------------------------------------------
 *  NOTE: From here on, the event IDs will take the form of a
 *  "base" EID + an offset.  This is done to allow unique event
 *  IDs to be sent from utility functions.
 ** --------------------------------------------------------------*/

#define FM_FNAME_INVALID_EID_OFFSET  0
#define FM_FNAME_DNE_EID_OFFSET      1
#define FM_FNAME_EXIST_EID_OFFSET    1 /* mutually exclusive with DNE */
#define FM_FNAME_ISDIR_EID_OFFSET    2
#define FM_FNAME_ISFILE_EID_OFFSET   2 /* mutually exclusive with ISDIR */
#define FM_FNAME_ISOPEN_EID_OFFSET   3
#define FM_FNAME_ISCLOSED_EID_OFFSET 4
/* Unused EID offset 5 */
#define FM_FNAME_NUM_OFFSETS 6

#define FM_CHILD_DISABLED_EID_OFFSET 0
#define FM_CHILD_Q_FULL_EID_OFFSET   1
#define FM_CHILD_BROKEN_EID_OFFSET   2
#define FM_CHILD_NUM_OFFSETS         3

/**
 * \brief FM Child Task Copy File Source Filename Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This is the base for a number of error events generated when the
 *  /FM_Copy is received with an unusable source filename.
 */
#define FM_COPY_SRC_BASE_EID 151

/**
 * \brief FM Child Task Copy File Source Name Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Copy
 *  command packet with an invalid source filename.
 *
 *  Value: 151
 */
#define FM_COPY_SRC_INVALID_ERR_EID (FM_COPY_SRC_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/**
 * \brief FM Child Task Copy File Source File Does Not Exist Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Copy
 *  command packet with a source filename that does not exist.
 *
 *  Value: 152
 */
#define FM_COPY_SRC_DNE_ERR_EID (FM_COPY_SRC_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/**
 * \brief FM Child Task Copy File Source File Name Is Directory Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Copy
 *  command packet with a source filename that is a directory.
 *
 *  Value: 153
 */
#define FM_COPY_SRC_ISDIR_ERR_EID (FM_COPY_SRC_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/**
 * \brief FM Child Task Copy File Target Filename Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This is the base EID for a number of error events related to the
 *  target file in an /FM_COPY command.
 *
 *  Value: 157
 */
#define FM_COPY_TGT_BASE_EID (FM_COPY_SRC_BASE_EID + FM_FNAME_NUM_OFFSETS)

/**
 * \brief FM Child Task Copy File Target Filename Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Copy
 *  command packet with an invalid target filename.
 *
 *  Value: 157
 */
#define FM_COPY_TGT_INVALID_ERR_EID (FM_COPY_TGT_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/**
 * \brief FM Child Task Copy File Target File Already Exists Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Copy
 *  command packet with a target filename that already exists.
 *
 *  Value: 158
 */
#define FM_COPY_TGT_EXIST_ERR_EID (FM_COPY_TGT_BASE_EID + FM_FNAME_EXIST_EID_OFFSET)

/**
 * \brief FM Child Task Copy File Target Filename Is A Directory Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Copy
 *  command packet with a target filename that is a directory.
 *
 *  Value: 159
 */
#define FM_COPY_TGT_ISDIR_ERR_EID (FM_COPY_TGT_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/**
 * \brief FM Child Task Copy File Target Filename Exists As Open File Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Copy
 *  command packet with a target filename that is open.
 *
 *  Value: 160
 */
#define FM_COPY_TGT_ISOPEN_ERR_EID (FM_COPY_TGT_BASE_EID + FM_FNAME_ISOPEN_EID_OFFSET)

/**
 * \brief FM Child Task Copy File Child Task Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This is the base for any of several messages that are  generated when
 *  the FM child task command queue interface cannot be used.
 *
 *  Value: 163
 */
#define FM_COPY_CHILD_BASE_EID (FM_COPY_TGT_BASE_EID + FM_FNAME_NUM_OFFSETS)

/**
 * \brief FM Child Task Copy File Child Task Disabled Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task is disabled.
 *
 *  Value: 163
 */
#define FM_COPY_CHILD_DISABLED_ERR_EID (FM_COPY_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/**
 * \brief FM Child Task Copy File Child Task Queue Full Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task comand queue is full.
 *
 *  If the child task command queue is full, the problem may be temporary,
 *  caused by sending too many FM commands too quickly.  If the command
 *  queue does not empty itself within a reasonable amount of time then
 *  the child task may be hung. It may be possible to use CFE commands to
 *  terminate the child task, which should then cause FM to process all
 *  commands in the main task.
 *
 *  Value: 164
 */
#define FM_COPY_CHILD_FULL_ERR_EID (FM_COPY_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/**
 * \brief FM Child Task Copy File Child Task Interface Broken Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the interface between the main task
 *  and child task is broken.
 *
 *  If the child task queue is broken then either the handshake interface
 *  logic is flawed, or there has been some sort of data corruption that
 *  affected the interface control variables.  In either case, it may be
 *  necessary to restart the FM application to resync the interface.
 *
 *  Value: 165
 *
 */
#define FM_COPY_CHILD_BROKEN_ERR_EID (FM_COPY_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/**
 * \brief FM Child Task Move File Source Filename Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Move
 *  command packet with a source filename that is unusable for one
 *  of several reasons.
 *
 *  Value: 166
 */
#define FM_MOVE_SRC_BASE_EID (FM_COPY_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/**
 * \brief FM Child Task Move File Source Filename Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Move
 *  command packet with an invalid source filename.
 *
 *  Value: 166
 */
#define FM_MOVE_SRC_INVALID_ERR_EID (FM_MOVE_SRC_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/**
 * \brief FM Child Task Move File Source File Does Not Exist Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Move
 *  command packet with a source filename that does not exist.
 *
 *  Value: 167
 */
#define FM_MOVE_SRC_DNE_ERR_EID (FM_MOVE_SRC_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/**
 * \brief FM Child Task Move File Source Filename Is A Directory Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Move
 *  command packet with a source filename that is a directory.
 *
 *  Value: 168
 */
#define FM_MOVE_SRC_ISDIR_ERR_EID (FM_MOVE_SRC_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/**
 * \brief FM Child Task Move File Target Filename Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Move
 *  command packet with a target filename that is unusable for one
 *  of several reasons.
 *
 *  Value: 172
 */
#define FM_MOVE_TGT_BASE_EID (FM_MOVE_SRC_BASE_EID + FM_FNAME_NUM_OFFSETS)

/**
 * \brief FM Child Task Move File Target Filename Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Move
 *  command packet with an invalid target filename.
 *
 *  Value: 172
 */
#define FM_MOVE_TGT_INVALID_ERR_EID (FM_MOVE_TGT_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/**
 * \brief FM Child Task Move File Target File Already Exists Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Move
 *  command packet with a target filename that already exists
 *
 *  Value: 173
 */
#define FM_MOVE_TGT_EXIST_ERR_EID (FM_MOVE_TGT_BASE_EID + FM_FNAME_EXIST_EID_OFFSET)

/**
 * \brief FM Child Task Move File Target Filename Is A Directory Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Move
 *  command packet with a target filename that is a directory.
 *
 *  Value: 174
 */
#define FM_MOVE_TGT_ISDIR_ERR_EID (FM_MOVE_TGT_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/**
 * \brief FM Child Task Move File Target File Exists As An Open File Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Move
 *  command packet with a target filename that is open.
 *
 *  Value: 175
 */
#define FM_MOVE_TGT_ISOPEN_ERR_EID (FM_MOVE_TGT_BASE_EID + FM_FNAME_ISOPEN_EID_OFFSET)

/**
 * \brief FM Child Task Move File Child Task Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This is the base for any of several messages that are  generated when
 *  the FM child task command queue interface cannot be used.
 *
 *  Value: 178
 */
#define FM_MOVE_CHILD_BASE_EID (FM_MOVE_TGT_BASE_EID + FM_FNAME_NUM_OFFSETS)

/**
 * \brief FM Child Task Move File Child Task Disabled Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task is disabled.
 *
 *  Value: 178
 */
#define FM_MOVE_CHILD_DISABLED_ERR_EID (FM_MOVE_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/**
 * \brief FM Child Task Move File Child Task Queue Full Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task comand queue is full.
 *
 *  If the child task command queue is full, the problem may be temporary,
 *  caused by sending too many FM commands too quickly.  If the command
 *  queue does not empty itself within a reasonable amount of time then
 *  the child task may be hung. It may be possible to use CFE commands to
 *  terminate the child task, which should then cause FM to process all
 *  commands in the main task.
 *
 *  Value: 179
 */
#define FM_MOVE_CHILD_FULL_ERR_EID (FM_MOVE_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/**
 * \brief FM Child Task Move File Child Task Inteface Broken Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the interface between the main task
 *  and child task is broken.
 *
 *  If the child task queue is broken then either the handshake interface
 *  logic is flawed, or there has been some sort of data corruption that
 *  affected the interface control variables.  In either case, it may be
 *  necessary to restart the FM application to resync the interface.
 *
 *  Value: 180
 *
 */
#define FM_MOVE_CHILD_BROKEN_ERR_EID (FM_MOVE_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/**
 * \brief FM Child Task Rename File Source Filename Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Rename
 *  command packet with a source filename that is unusable for one
 *  of several reasons.
 *
 *  Value: 181
 */
#define FM_RENAME_SRC_BASE_EID (FM_MOVE_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/**
 * \brief FM Child Task Rename File Source Filename Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Rename
 *  command packet with an invalid source filename.
 *
 *  Value: 181
 */
#define FM_RENAME_SRC_INVALID_ERR_EID (FM_RENAME_SRC_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/**
 * \brief FM Child Task Rename File Source File Does Not Exist Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Rename
 *  command packet with a source filename that does not exist.
 *
 *  Value: 182
 */
#define FM_RENAME_SRC_DNE_ERR_EID (FM_RENAME_SRC_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/**
 * \brief FM Child Task Rename File Source Filename Is Directory Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Rename
 *  command packet with a source filename that is a directory.
 *
 *  Value: 183
 */
#define FM_RENAME_SRC_ISDIR_ERR_EID (FM_RENAME_SRC_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/**
 * \brief FM Child Task Rename File Target Filename Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Rename
 *  command packet with a target filename that is unusable for one
 *  of several reasons.
 *
 *  Value: 187
 */
#define FM_RENAME_TGT_BASE_EID (FM_RENAME_SRC_BASE_EID + FM_FNAME_NUM_OFFSETS)

/**
 * \brief FM Child Task Rename File Target Filename Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Rename
 *  command packet with an invalid target filename.
 *
 *  Value: 187
 */
#define FM_RENAME_TGT_INVALID_ERR_EID (FM_RENAME_TGT_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/**
 * \brief FM Child Task Rename File Target File Already Exists Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Rename
 *  command packet with a target filename that already exists.
 *
 *  Value: 188
 */
#define FM_RENAME_TGT_EXIST_ERR_EID (FM_RENAME_TGT_BASE_EID + FM_FNAME_EXIST_EID_OFFSET)

/**
 * \brief FM Child Task Rename File Target Filename Is Directory Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Rename
 *  command packet with a target filename that is a directory.
 *
 *  Value: 189
 */
#define FM_RENAME_TGT_ISDIR_ERR_EID (FM_RENAME_TGT_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/**
 * \brief FM Child Task Rename File Target Filename Exists As Open File Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Rename
 *  command packet with a target filename that is open.
 *
 *  Value: 190
 */
#define FM_RENAME_TGT_ISOPEN_ERR_EID (FM_RENAME_TGT_BASE_EID + FM_FNAME_ISOPEN_EID_OFFSET)

/**
 * \brief FM Child Task Rename File Child Task Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This is the base for any of several messages that are  generated when
 *  the FM child task command queue interface cannot be used.
 *
 *  Value: 193
 */
#define FM_RENAME_CHILD_BASE_EID (FM_RENAME_TGT_BASE_EID + FM_FNAME_NUM_OFFSETS)

/**
 * \brief FM Child Task Rename File Child Task Disabled Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task is disabled.
 *
 *  Value: 193
 */
#define FM_RENAME_CHILD_DISABLED_ERR_EID (FM_RENAME_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/**
 * \brief FM Child Task Rename File Child Task Queue Full Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task comand queue is full.
 *
 *  If the child task command queue is full, the problem may be temporary,
 *  caused by sending too many FM commands too quickly.  If the command
 *  queue does not empty itself within a reasonable amount of time then
 *  the child task may be hung. It may be possible to use CFE commands to
 *  terminate the child task, which should then cause FM to process all
 *  commands in the main task.
 *
 *  Value: 194
 */
#define FM_RENAME_CHILD_FULL_ERR_EID (FM_RENAME_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/**
 * \brief FM Child Task Rename File Child Task Inteface Broken Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the interface between the main task
 *  and child task is broken.
 *
 *  If the child task queue is broken then either the handshake interface
 *  logic is flawed, or there has been some sort of data corruption that
 *  affected the interface control variables.  In either case, it may be
 *  necessary to restart the FM application to resync the interface.
 *
 *  Value: 195
 *
 */
#define FM_RENAME_CHILD_BROKEN_ERR_EID (FM_RENAME_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/**
 * \brief FM Child Task Delete File Source Filename Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Delete
 *  command packet with a source filename that is unusable for one
 *  of several reasons.
 *
 *  Value: 196
 */
#define FM_DELETE_SRC_BASE_EID (FM_RENAME_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/**
 * \brief FM Child Task Delete File Source Filename Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Delete
 *  command packet with an invalid source filename.
 *
 *  Value: 196
 */
#define FM_DELETE_SRC_INVALID_ERR_EID (FM_DELETE_SRC_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/**
 * \brief FM Child Task Delete File Source File Does Not Exist Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Delete
 *  command packet with a source filename that does not exist.
 *
 *  Value: 197
 */
#define FM_DELETE_SRC_DNE_ERR_EID (FM_DELETE_SRC_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/**
 * \brief FM Child Task Delete File Source Filename Is Directory Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Delete
 *  command packet with a source filename that is a directory.
 *
 *  Value: 198
 */
#define FM_DELETE_SRC_ISDIR_ERR_EID (FM_DELETE_SRC_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/**
 * \brief FM Child Task Delete File File Is Open Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Delete
 *  command packet with a source filename that is already open.
 *
 *  Value: 199
 */
#define FM_DELETE_SRC_OPEN_ERR_EID (FM_DELETE_SRC_BASE_EID + FM_FNAME_ISOPEN_EID_OFFSET)

/**
 * \brief FM Child Task Delete File Child Task Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This is the base for any of several messages that are  generated when
 *  the FM child task command queue interface cannot be used.
 *
 *  Value: 202
 */
#define FM_DELETE_CHILD_BASE_EID (FM_DELETE_SRC_BASE_EID + FM_FNAME_NUM_OFFSETS)

/**
 * \brief FM Child Task Delete File Child Task Disabled Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task is disabled.
 *
 *  Value: 202
 */
#define FM_DELETE_CHILD_DISABLED_ERR_EID (FM_DELETE_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/**
 * \brief FM Child Task Delete File Child Task Queue Full Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task comand queue is full.
 *
 *  If the child task command queue is full, the problem may be temporary,
 *  caused by sending too many FM commands too quickly.  If the command
 *  queue does not empty itself within a reasonable amount of time then
 *  the child task may be hung. It may be possible to use CFE commands to
 *  terminate the child task, which should then cause FM to process all
 *  commands in the main task.
 *
 *  Value: 203
 */
#define FM_DELETE_CHILD_FULL_ERR_EID (FM_DELETE_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/**
 * \brief FM Child Task Delete File Child Task Inteface Broken Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the interface between the main task
 *  and child task is broken.
 *
 *  If the child task queue is broken then either the handshake interface
 *  logic is flawed, or there has been some sort of data corruption that
 *  affected the interface control variables.  In either case, it may be
 *  necessary to restart the FM application to resync the interface.
 *
 *  Value: 204
 *
 */
#define FM_DELETE_CHILD_BROKEN_ERR_EID (FM_DELETE_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/**
 * \brief FM Child Task Delete All Files Directory Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_DeleteAll
 *  command packet with a directory name that is unusable for one
 *  of several reasons.
 *
 *  Value: 205
 */
#define FM_DELETE_ALL_SRC_BASE_EID (FM_DELETE_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/**
 * \brief FM Child Task Delete All Files Directory Name Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Delete
 *  command packet with an invalid source filename.
 *
 *  Value: 205
 */
#define FM_DELETE_ALL_SRC_INVALID_ERR_EID (FM_DELETE_ALL_SRC_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/**
 * \brief FM Child Task Delete All Files Directory Does Not Exist Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_DeleteAll
 *  command packet with a directory name that does not exist.
 *
 *  Value: 206
 */
#define FM_DELETE_ALL_SRC_DNE_ERR_EID (FM_DELETE_ALL_SRC_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/**
 * \brief FM Child Task Delete All Files Directory Name Is A File Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Delete
 *  command packet with a directory name that is a file.
 *
 *  Value: 207
 */
#define FM_DELETE_ALL_SRC_FILE_ERR_EID (FM_DELETE_ALL_SRC_BASE_EID + FM_FNAME_ISFILE_EID_OFFSET)

/**
 * \brief FM Child Task Delete All Files Child Task Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This is the base for any of several messages that are  generated when
 *  the FM child task command queue interface cannot be used.
 *
 *  Value: 211
 */
#define FM_DELETE_ALL_CHILD_BASE_EID (FM_DELETE_ALL_SRC_BASE_EID + FM_FNAME_NUM_OFFSETS)

/**
 * \brief FM Child Task Delete All Files Child Task Disabled Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task is disabled.
 *
 *  Value: 211
 */
#define FM_DELETE_ALL_CHILD_DISABLED_ERR_EID (FM_DELETE_ALL_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/**
 * \brief FM Child Task Delete All Files Child Task Queue Full Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task comand queue is full.
 *
 *  If the child task command queue is full, the problem may be temporary,
 *  caused by sending too many FM commands too quickly.  If the command
 *  queue does not empty itself within a reasonable amount of time then
 *  the child task may be hung. It may be possible to use CFE commands to
 *  terminate the child task, which should then cause FM to process all
 *  commands in the main task.
 *
 *  Value: 212
 */
#define FM_DELETE_ALL_CHILD_FULL_ERR_EID (FM_DELETE_ALL_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/**
 * \brief FM Child Task Delete All Files Child Task Inteface Broken Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the interface between the main task
 *  and child task is broken.
 *
 *  If the child task queue is broken then either the handshake interface
 *  logic is flawed, or there has been some sort of data corruption that
 *  affected the interface control variables.  In either case, it may be
 *  necessary to restart the FM application to resync the interface.
 *
 *  Value: 213
 *
 */
#define FM_DELETE_ALL_CHILD_BROKEN_ERR_EID (FM_DELETE_ALL_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/**
 * \brief FM Child Task Decompress File Source Filename Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Decompress
 *  command packet with a source filename that is unusable for one
 *  of several reasons.
 *
 *  Value: 214
 */
#define FM_DECOM_SRC_BASE_EID (FM_DELETE_ALL_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/**
 * \brief FM Child Task Decompress File Source Filename Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Decompress
 *  command packet with an invalid source filename.
 *
 *  Value: 214
 */
#define FM_DECOM_SRC_INVALID_ERR_EID (FM_DECOM_SRC_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/**
 * \brief FM Child Task Decompress File Source Filename Does Not Exist Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Decompress
 *  command packet with a source filename that does not exist.
 *
 *  Value: 215
 */
#define FM_DECOM_SRC_DNE_ERR_EID (FM_DECOM_SRC_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/**
 * \brief FM Child Task Decompress File Source Filename Is A Directory Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Decompress
 *  command packet with a source filename that is a directory.
 *
 *  Value: 216
 */
#define FM_DECOM_SRC_ISDIR_ERR_EID (FM_DECOM_SRC_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/**
 * \brief FM Child Task Decompress File Source File Is Open Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Decompress
 *  command packet with a source filename that is already open.
 *
 *  Value: 217
 */
#define FM_DECOM_SRC_OPEN_ERR_EID (FM_DECOM_SRC_BASE_EID + FM_FNAME_ISOPEN_EID_OFFSET)

/**
 * \brief FM Child Task Decompress File Target Filename Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Decompress
 *  command packet with a target filename that is unusable for one
 *  of several reasons.
 *
 *  Value: 220
 */
#define FM_DECOM_TGT_BASE_EID (FM_DECOM_SRC_BASE_EID + FM_FNAME_NUM_OFFSETS)

/**
 * \brief FM Child Task Decompress File Target Filename Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Decompress
 *  command packet with an invalid target filename.
 *
 *  Value: 220
 */
#define FM_DECOM_TGT_INVALID_ERR_EID (FM_DECOM_TGT_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/**
 * \brief FM Child Task Decompress File Target File Already Exists Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Decompress
 *  command packet with a target filename that already exists.
 *
 *  Value: 221
 */
#define FM_DECOM_TGT_EXIST_ERR_EID (FM_DECOM_TGT_BASE_EID + FM_FNAME_EXIST_EID_OFFSET)

/**
 * \brief FM Child Task Decompress File Target Filename Is Directory Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Decompress
 *  command packet with a target filename that is a directory.
 *
 *  Value: 222
 */
#define FM_DECOM_TGT_ISDIR_ERR_EID (FM_DECOM_TGT_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/**
 * \brief FM Child Task Decompress File Child Task Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This is the base for any of several messages that are  generated when
 *  the FM child task command queue interface cannot be used.
 *
 *  Value: 226
 */
#define FM_DECOM_CHILD_BASE_EID (FM_DECOM_TGT_BASE_EID + FM_FNAME_NUM_OFFSETS)

/**
 * \brief FM Child Task Decompress File Child Task Disabled Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task is disabled.
 *
 *  Value: 226
 */
#define FM_DECOM_CHILD_DISABLED_ERR_EID (FM_DECOM_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/**
 * \brief FM Child Task Decompress File Child Task Queue Full Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task comand queue is full.
 *
 *  If the child task command queue is full, the problem may be temporary,
 *  caused by sending too many FM commands too quickly.  If the command
 *  queue does not empty itself within a reasonable amount of time then
 *  the child task may be hung. It may be possible to use CFE commands to
 *  terminate the child task, which should then cause FM to process all
 *  commands in the main task.
 *
 *  Value: 227
 */
#define FM_DECOM_CHILD_FULL_ERR_EID (FM_DECOM_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/**
 * \brief FM Child Task Decompress File Child Task Inteface Broken Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the interface between the main task
 *  and child task is broken.
 *
 *  If the child task queue is broken then either the handshake interface
 *  logic is flawed, or there has been some sort of data corruption that
 *  affected the interface control variables.  In either case, it may be
 *  necessary to restart the FM application to resync the interface.
 *
 *  Value: 228
 *
 */
#define FM_DECOM_CHILD_BROKEN_ERR_EID (FM_DECOM_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/**
 * \brief FM Child Task Concat Files Source 1 Filename Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Concat
 *  command packet with a source 1 filename that is unusable for one
 *  of several reasons.
 *
 *  Value: 229
 */
#define FM_CONCAT_SRC1_BASE_EID (FM_DECOM_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/**
 * \brief FM Child Task Concat Files Source 1 Filename Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Concat
 *  command packet with an invalid source 1 filename.
 *
 *  Value: 229
 */
#define FM_CONCAT_SRC1_INVALID_ERR_EID (FM_CONCAT_SRC1_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/**
 * \brief FM Child Task Concat Files Source 1 File Does Not Exist Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Concat
 *  command packet with a source 1 filename that does not exist.
 *
 *  Value: 230
 */
#define FM_CONCAT_SRC1_DNE_ERR_EID (FM_CONCAT_SRC1_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/**
 * \brief FM Child Task Concat Files Source 1 Filename Is Directory Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Concat
 *  command packet with a source filename that is a directory.
 *
 *  Value: 231
 */
#define FM_CONCAT_SRC1_ISDIR_ERR_EID (FM_CONCAT_SRC1_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/**
 * \brief FM Child Task Concat Files Source 1 File Already Open Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Concat
 *  command packet with a source filename that is already open.
 *
 *  Value: 232
 */
#define FM_CONCAT_SRC1_OPEN_ERR_EID (FM_CONCAT_SRC1_BASE_EID + FM_FNAME_ISOPEN_EID_OFFSET)

/**
 * \brief FM Child Task Concat Files Source 2 Filename Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Concat
 *  command packet with a source 2 filename that is unusable for one
 *  of several reasons.
 *
 *  Value: 235
 */
#define FM_CONCAT_SRC2_BASE_EID (FM_CONCAT_SRC1_BASE_EID + FM_FNAME_NUM_OFFSETS)

/**
 * \brief FM Child Task Concat Files Source 2 Filename Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Concat
 *  command packet with an invalid source 2 filename.
 *
 *  Value: 235
 */
#define FM_CONCAT_SRC2_INVALID_ERR_EID (FM_CONCAT_SRC2_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/**
 * \brief FM Child Task Concat Files Source 2 File Does Not Exist Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Concat
 *  command packet with a source 2 filename that does not exist.
 *
 *  Value: 236
 */
#define FM_CONCAT_SRC2_DNE_ERR_EID (FM_CONCAT_SRC2_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/**
 * \brief FM Child Task Concat Files Source 2 Filename Is Directory Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Concat
 *  command packet with a source filename that is a directory.
 *
 *  Value: 237
 */
#define FM_CONCAT_SRC2_ISDIR_ERR_EID (FM_CONCAT_SRC2_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/**
 * \brief FM Child Task Concat Files Source 2 File Already Open Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Concat
 *  command packet with a source filename that is already open.
 *
 *  Value: 238
 */
#define FM_CONCAT_SRC2_OPEN_ERR_EID (FM_CONCAT_SRC2_BASE_EID + FM_FNAME_ISOPEN_EID_OFFSET)

/**
 * \brief FM Child Task Concat Files Target Filename Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Concat
 *  command packet with a target filename that is unusable for one
 *  of several reasons.
 *
 *  Value: 241
 */
#define FM_CONCAT_TGT_BASE_EID (FM_CONCAT_SRC2_BASE_EID + FM_FNAME_NUM_OFFSETS)

/**
 * \brief FM Child Task Concat Files Target Filename Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Concat
 *  command packet with an invalid target filename.
 *
 *  Value: 241
 */
#define FM_CONCAT_TGT_INVALID_ERR_EID (FM_CONCAT_TGT_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/**
 * \brief FM Child Task Concat Files Target Filename Already Exists Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Concat
 *  command packet with a target filename that already exists.
 *
 *  Value: 242
 */
#define FM_CONCAT_TGT_EXIST_ERR_EID (FM_CONCAT_TGT_BASE_EID + FM_FNAME_EXIST_EID_OFFSET)

/**
 * \brief FM Child Task Concat Files Target Filename Is Directory Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Concat
 *  command packet with a target filename that is a directory.
 *
 *  Value: 243
 */
#define FM_CONCAT_TGT_ISDIR_ERR_EID (FM_CONCAT_TGT_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/**
 * \brief FM Child Task Concat Files Child Task Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This is the base for any of several messages that are generated when
 *  the FM child task command queue interface cannot be used.
 *
 *  Value: 247
 */
#define FM_CONCAT_CHILD_BASE_EID (FM_CONCAT_TGT_BASE_EID + FM_FNAME_NUM_OFFSETS)

/**
 * \brief FM Child Task Concat Files Child Task Disabled Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task is disabled.
 *
 *  Value: 247
 */
#define FM_CONCAT_CHILD_DISABLED_ERR_EID (FM_CONCAT_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/**
 * \brief FM Child Task Concat Files Child Task Queue Full Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task comand queue is full.
 *
 *  If the child task command queue is full, the problem may be temporary,
 *  caused by sending too many FM commands too quickly.  If the command
 *  queue does not empty itself within a reasonable amount of time then
 *  the child task may be hung. It may be possible to use CFE commands to
 *  terminate the child task, which should then cause FM to process all
 *  commands in the main task.
 *
 *  Value: 248
 */
#define FM_CONCAT_CHILD_FULL_ERR_EID (FM_CONCAT_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/**
 * \brief FM Child Task Concat Files Child Task Inteface Broken Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the interface between the main task
 *  and child task is broken.
 *
 *  If the child task queue is broken then either the handshake interface
 *  logic is flawed, or there has been some sort of data corruption that
 *  affected the interface control variables.  In either case, it may be
 *  necessary to restart the FM application to resync the interface.
 *
 *  Value: 249
 *
 */
#define FM_CONCAT_CHILD_BROKEN_ERR_EID (FM_CONCAT_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/**
 * \brief FM Child Task Get File Info Child Task Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This is the base for any of several messages that are generated when
 *  the FM child task command queue interface cannot be used.
 *
 *  Value: 250
 */
#define FM_FILE_INFO_CHILD_BASE_EID (FM_CONCAT_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/**
 * \brief FM Child Task Get File Info Child Task Disabled Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task is disabled.
 *
 *  Value: 250
 */
#define FM_FILE_INFO_CHILD_DISABLED_ERR_EID (FM_FILE_INFO_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/**
 * \brief FM Child Task Get File Info Child Task Queue Full Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task comand queue is full.
 *
 *  If the child task command queue is full, the problem may be temporary,
 *  caused by sending too many FM commands too quickly.  If the command
 *  queue does not empty itself within a reasonable amount of time then
 *  the child task may be hung. It may be possible to use CFE commands to
 *  terminate the child task, which should then cause FM to process all
 *  commands in the main task.
 *
 *  Value: 251
 */
#define FM_FILE_INFO_CHILD_FULL_ERR_EID (FM_FILE_INFO_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/**
 * \brief FM Child Task Get File Info Child Task Inteface Broken Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the interface between the main task
 *  and child task is broken.
 *
 *  If the child task queue is broken then either the handshake interface
 *  logic is flawed, or there has been some sort of data corruption that
 *  affected the interface control variables.  In either case, it may be
 *  necessary to restart the FM application to resync the interface.
 *
 *  Value: 252
 *
 */
#define FM_FILE_INFO_CHILD_BROKEN_ERR_EID (FM_FILE_INFO_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/**
 * \brief FM Child Task Create Directory, Directory Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Create
 *  command packet with a directory name that is unusable for one
 *  of several reasons.
 *
 *  Value: 253
 */
#define FM_CREATE_DIR_SRC_BASE_EID (FM_FILE_INFO_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/**
 * \brief FM Child Task Create Directory Name Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Create
 *  command packet with an invalid directory name.
 *
 *  Value: 253
 */
#define FM_CREATE_DIR_SRC_INVALID_ERR_EID (FM_CREATE_DIR_SRC_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/**
 * \brief FM Child Task Create Directory Name Exists As File Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Create
 *  command packet with a directory name that exists as a file.
 *
 *  Value: 254
 */
#define FM_CREATE_DIR_SRC_DNE_ERR_EID (FM_CREATE_DIR_SRC_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/**
 * \brief FM Child Task Create Directory Already Exists Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_Create
 *  command pasket with a directory that already exists.
 *
 *  Value: 255
 */
#define FM_CREATE_DIR_SRC_ISDIR_ERR_EID (FM_CREATE_DIR_SRC_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/**
 * \brief FM Child Task Create Directory Child Task Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This is the base for any of several messages that are generated when
 *  the FM child task command queue interface cannot be used.
 *
 *  Value: 259
 */
#define FM_CREATE_DIR_CHILD_BASE_EID (FM_CREATE_DIR_SRC_BASE_EID + FM_FNAME_NUM_OFFSETS)

/**
 * \brief FM Child Task Create Directory Child Task Disabled Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task is disabled.
 *
 *  Value: 259
 */
#define FM_CREATE_DIR_CHILD_DISABLED_ERR_EID (FM_CREATE_DIR_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/**
 * \brief FM Child Task Create Directory Child Task Queue Full Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task comand queue is full.
 *
 *  If the child task command queue is full, the problem may be temporary,
 *  caused by sending too many FM commands too quickly.  If the command
 *  queue does not empty itself within a reasonable amount of time then
 *  the child task may be hung. It may be possible to use CFE commands to
 *  terminate the child task, which should then cause FM to process all
 *  commands in the main task.
 *
 *  Value: 260
 */
#define FM_CREATE_DIR_CHILD_FULL_ERR_EID (FM_CREATE_DIR_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/**
 * \brief FM Child Task Create Directory Child Task Inteface Broken Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the interface between the main task
 *  and child task is broken.
 *
 *  If the child task queue is broken then either the handshake interface
 *  logic is flawed, or there has been some sort of data corruption that
 *  affected the interface control variables.  In either case, it may be
 *  necessary to restart the FM application to resync the interface.
 *
 *  Value: 261
 *
 */
#define FM_CREATE_DIR_CHILD_BROKEN_ERR_EID (FM_CREATE_DIR_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/**
 * \brief FM Child Task Delete Directory, Directory Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_DeleteDir
 *  command packet with a directory name that is unusable for one
 *  of several reasons.
 *
 *  Value: 262
 */
#define FM_DELETE_DIR_SRC_BASE_EID (FM_CREATE_DIR_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/**
 * \brief FM Child Task Delete Directory Name Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_DeleteDir
 *  command packet with an invalid directory name.
 *
 *  Value: 262
 */
#define FM_DELETE_DIR_SRC_INVALID_ERR_EID (FM_DELETE_DIR_SRC_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/**
 * \brief FM Child Task Delete Directory Name Exists As File Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_DeleteDir
 *  command packet with a directory name that exists as a file.
 *
 *  Value: 263
 */
#define FM_DELETE_DIR_SRC_DNE_ERR_EID (FM_DELETE_DIR_SRC_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/**
 * \brief FM Child Task Delete Directory Child Task Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This is the base for any of several messages that are generated when
 *  the FM child task command queue interface cannot be used.
 *
 *  Value: 268
 */
#define FM_DELETE_DIR_CHILD_BASE_EID (FM_DELETE_DIR_SRC_BASE_EID + FM_FNAME_NUM_OFFSETS)

/**
 * \brief FM Child Task Delete Directory Child Task Disabled Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task is disabled.
 *
 *  Value: 268
 */
#define FM_DELETE_DIR_CHILD_DISABLED_ERR_EID (FM_DELETE_DIR_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/**
 * \brief FM Child Task Delete Directory Child Task Queue Full Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task comand queue is full.
 *
 *  If the child task command queue is full, the problem may be temporary,
 *  caused by sending too many FM commands too quickly.  If the command
 *  queue does not empty itself within a reasonable amount of time then
 *  the child task may be hung. It may be possible to use CFE commands to
 *  terminate the child task, which should then cause FM to process all
 *  commands in the main task.
 *
 *  Value: 269
 */
#define FM_DELETE_DIR_CHILD_FULL_ERR_EID (FM_DELETE_DIR_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/**
 * \brief FM Child Task Delete Directory Child Task Inteface Broken Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the interface between the main task
 *  and child task is broken.
 *
 *  If the child task queue is broken then either the handshake interface
 *  logic is flawed, or there has been some sort of data corruption that
 *  affected the interface control variables.  In either case, it may be
 *  necessary to restart the FM application to resync the interface.
 *
 *  Value: 270
 *
 */
#define FM_DELETE_DIR_CHILD_BROKEN_ERR_EID (FM_DELETE_DIR_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/**
 * \brief FM Child Task Directory List to File Source Filename Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_GetDirFile
 *  command packet with a source directory name that is unusable for one
 *  of several reasons.
 *
 *  Value: 271
 */
#define FM_GET_DIR_FILE_SRC_BASE_EID (FM_DELETE_DIR_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/**
 * \brief FM Child Task Directory List to File Directory Name Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_GetDirFile
 *  command packet with an invalid source directory name.
 *
 *  Value: 271
 */
#define FM_GET_DIR_FILE_SRC_INVALID_ERR_EID (FM_GET_DIR_FILE_SRC_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/**
 * \brief FM Child Task Directory List to File Directory Does Not Exist Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_GetDirFile
 *  command packet with a source directory name that does not exist.
 *
 *  Value: 272
 */
#define FM_GET_DIR_FILE_SRC_DNE_ERR_EID (FM_GET_DIR_FILE_SRC_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/**
 * \brief FM Child Task Directory List to File Directory Name Is File Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_GetDirFile
 *  command packet with a source directory name that is a file.
 *
 *  Value: 273
 */
#define FM_GET_DIR_FILE_SRC_ISDIR_ERR_EID (FM_GET_DIR_FILE_SRC_BASE_EID + FM_FNAME_ISFILE_EID_OFFSET)

/**
 * \brief FM Child Task Directory List to File Target Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_GetDirFile
 *  command packet with a target filename that is unusable for one
 *  of several reasons.
 *
 *  Value: 277
 */
#define FM_GET_DIR_FILE_TGT_BASE_EID (FM_GET_DIR_FILE_SRC_BASE_EID + FM_FNAME_NUM_OFFSETS)

/**
 * \brief FM Child Task Directory List to File Target Filename Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_GetDirFile
 *  command packet with an invalid target file name.
 *
 *  Value: 277
 */
#define FM_GET_DIR_FILE_TGT_INVALID_ERR_EID (FM_GET_DIR_FILE_TGT_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/**
 * \brief FM Child Task Directory List to File Target Filename Is Directory Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_GetDirFile
 *  command packet with a target filename that is a directory.
 *
 *  Value: 279
 */
#define FM_GET_DIR_FILE_TGT_ISDIR_ERR_EID (FM_GET_DIR_FILE_TGT_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/**
 * \brief FM Child Task Directory List to File Child Task Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This is the base for any of several messages that are  generated when
 *  the FM child task command queue interface cannot be used.
 *
 *  Value: 283
 */
#define FM_GET_DIR_FILE_CHILD_BASE_EID (FM_GET_DIR_FILE_TGT_BASE_EID + FM_FNAME_NUM_OFFSETS)

/**
 * \brief FM Child Task Directory List to File Child Task Disabled Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task is disabled.
 *
 *  Value: 283
 */
#define FM_GET_DIR_FILE_CHILD_DISABLED_ERR_EID (FM_GET_DIR_FILE_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/**
 * \brief FM Child Task Directory List to File Child Task Queue Full Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task comand queue is full.
 *
 *  If the child task command queue is full, the problem may be temporary,
 *  caused by sending too many FM commands too quickly.  If the command
 *  queue does not empty itself within a reasonable amount of time then
 *  the child task may be hung. It may be possible to use CFE commands to
 *  terminate the child task, which should then cause FM to process all
 *  commands in the main task.
 *
 *  Value: 284
 */
#define FM_GET_DIR_FILE_CHILD_FULL_ERR_EID (FM_GET_DIR_FILE_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/**
 * \brief FM Child Task Directory List to File Child Task Inteface Broken Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the interface between the main task
 *  and child task is broken.
 *
 *  If the child task queue is broken then either the handshake interface
 *  logic is flawed, or there has been some sort of data corruption that
 *  affected the interface control variables.  In either case, it may be
 *  necessary to restart the FM application to resync the interface.
 *
 *  Value: 285
 *
 */
#define FM_GET_DIR_FILE_CHILD_BROKEN_ERR_EID (FM_GET_DIR_FILE_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/**
 * \brief FM Child Task Directory List to Packet Directory Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_GetDirPkt
 *  command packet with a source directory name that is unusable for one
 *  of several reasons.
 *
 *  Value: 286
 */
#define FM_GET_DIR_PKT_SRC_BASE_EID (FM_GET_DIR_FILE_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/**
 * \brief FM Child Task Directory List to Packet Directory Name Invalid Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_GetDirPkt
 *  command packet with an invalid source directory name.
 *
 *  Value: 286
 */
#define FM_GET_DIR_PKT_SRC_INVALID_ERR_EID (FM_GET_DIR_PKT_SRC_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/**
 * \brief FM Child Task Directory List to Packet Directory Does Not Exist Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_GetDirPkt
 *  command packet with a source directory name that does not exist.
 *
 *  Value: 287
 */
#define FM_GET_DIR_PKT_SRC_DNE_ERR_EID (FM_GET_DIR_PKT_SRC_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/**
 * \brief FM Child Task Directory List to Packet Directory Is File Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated upon receipt of a /FM_GetDirPkt
 *  command packet with a source directory name that is a file.
 *
 *  Value: 288
 */
#define FM_GET_DIR_PKT_SRC_ISDIR_ERR_EID (FM_GET_DIR_PKT_SRC_BASE_EID + FM_FNAME_ISFILE_EID_OFFSET)

/**
 * \brief FM Child Task Directory List to Packet Child Task Error Base ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This is the base for any of several messages that are  generated when
 *  the FM child task command queue interface cannot be used.
 *
 *  Value: 292
 */
#define FM_GET_DIR_PKT_CHILD_BASE_EID (FM_GET_DIR_PKT_SRC_BASE_EID + FM_FNAME_NUM_OFFSETS)

/**
 * \brief FM Child Task Directory List to Packet Child Task Disabled Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task is disabled.
 *
 *  Value: 292
 */
#define FM_GET_DIR_PKT_CHILD_DISABLED_ERR_EID (FM_GET_DIR_PKT_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/**
 * \brief FM Child Task Directory List to Packet Child Task Queue Full Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the child task comand queue is full.
 *
 *  If the child task command queue is full, the problem may be temporary,
 *  caused by sending too many FM commands too quickly.  If the command
 *  queue does not empty itself within a reasonable amount of time then
 *  the child task may be hung. It may be possible to use CFE commands to
 *  terminate the child task, which should then cause FM to process all
 *  commands in the main task.
 *
 *  Value: 293
 */
#define FM_GET_DIR_PKT_CHILD_FULL_ERR_EID (FM_GET_DIR_PKT_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/**
 * \brief FM Child Task Directory List to Packet Child Task Inteface Broken Event ID
 *
 *  \par Type: ERROR
 *
 *  \par Cause
 *
 *  This event message is generated when the FM child task command queue
 *  interface cannot be used because the interface between the main task
 *  and child task is broken.
 *
 *  If the child task queue is broken then either the handshake interface
 *  logic is flawed, or there has been some sort of data corruption that
 *  affected the interface control variables.  In either case, it may be
 *  necessary to restart the FM application to resync the interface.
 *
 *  Value: 294
 *
 */
#define FM_GET_DIR_PKT_CHILD_BROKEN_ERR_EID (FM_GET_DIR_PKT_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/**\}*/

#endif
