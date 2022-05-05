/*
** Filename: fm_events.h
**
** NASA Docket No. GSC-18,475-1, identified as “Core Flight Software System (CFS)
** File Manager Application Version 2.5.3
**
** Copyright © 2020 United States Government as represented by the Administrator of
** the National Aeronautics and Space Administration. All Rights Reserved.
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
**
** You may obtain a copy of the License at
** http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*
** Title: CFS File Manager (FM) Application Event ID Header File
**
** Purpose: Specification for the CFS File Manager Event Identifers.
**
** Notes:
**
** References:
**    Flight Software Branch C Coding Standard Version 1.0a
**
*/

#ifndef _fm_events_h_
#define _fm_events_h_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM event message ID's                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/** \brief <tt> 'Initialization Complete' </tt>
**  \event <tt> 'Initialization complete: version \%d.\%d.\%d.\%d' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause
**
**  This event message is issued after the File Manager application has
**  successfully completed startup initialization.
**
**  The version numbers indicate the application major version number,
**  minor version number, revision number and mission revision number.
*/
#define FM_STARTUP_EID 1

/** \brief <tt> 'Initialization Error: Register for Event Services' </tt>
**  \event <tt> 'Initialization error: register for event services: result = 0x\%08X' </tt>
**
**  \par Type: Error
**
**  \par Cause
**
**  This event message is issued when the File Manager application has
**  failed in its attempt to register for event services during startup
**  initialization.
**
**  This is a fatal error that will cause the File Manager application
**  to terminate.
**
**  The result number in the message text is the error code returned
**  from the the call to the API function #CFE_EVS_Register.
*/
#define FM_STARTUP_EVENTS_ERR_EID 2

/** \brief <tt> 'Initialization Error: Create SB Input Pipe' </tt>
**  \event <tt> 'Initialization error: create SB input pipe: result = 0x\%08X' </tt>
**
**  \par Type: Error
**
**  \par Cause
**
**  This event message is issued when the File Manager application has
**  failed in its attempt to create a Software Bus input pipe during startup
**  initialization.
**
**  This is a fatal error that will cause the File Manager application
**  to terminate.
**
**  The result number in the message text is the error code returned
**  from the the call to the API function #CFE_SB_CreatePipe.
*/
#define FM_STARTUP_CREAT_PIPE_ERR_EID 3

/** \brief <tt> 'Initialization Error: Subscribe to HK Request' </tt>
**  \event <tt> 'Initialization error: subscribe to HK request: result = 0x\%08X' </tt>
**
**  \par Type: Error
**
**  \par Cause
**
**  This event message is issued when the File Manager application has
**  failed in its attempt to subscribe to the HK telemetry request command
**  during startup initialization.
**
**  This is a fatal error that will cause the File Manager application
**  to terminate.
**
**  The result number in the message text is the error code returned
**  from the the call to the API function #CFE_SB_Subscribe.
*/
#define FM_STARTUP_SUBSCRIB_HK_ERR_EID 4

/** \brief <tt> 'Initialization Error: Subscribe to FM Commands' </tt>
**  \event <tt> 'Initialization error: subscribe to FM commands: result = 0x\%08X' </tt>
**
**  \par Type: Error
**
**  \par Cause
**
**  This event message is issued when the File Manager application has
**  failed in its attempt to subscribe to the FM ground command packet
**  during startup initialization.
**
**  This is a fatal error that will cause the File Manager application
**  to terminate.
**
**  The result number in the message text is the error code returned
**  from the the call to the API function #CFE_SB_Subscribe.
*/
#define FM_STARTUP_SUBSCRIB_GCMD_ERR_EID 5

/** \brief <tt> 'Initialization Error: Register Free Space Table' </tt>
**  \event <tt> 'Initialization error: register free space table: result = 0x\%08X' </tt>
**
**  \par Type: Error
**
**  \par Cause
**
**  This event message is issued when the File Manager application has
**  failed in its attempt to register its file system free space table
**  during startup initialization.
**
**  This is a fatal error that will cause the File Manager application
**  to terminate.
**
**  The result number in the message text is the error code returned
**  from the the call to the API function #CFE_TBL_Register.
*/
#define FM_STARTUP_TABLE_INIT_ERR_EID 6

/** \brief <tt> 'Main Loop Error: Software Bus Receive' </tt>
**  \event <tt> 'Main Loop Error: SB receive: result = 0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is issued when the File Manager application has
**  failed in its attempt to read from its Software Bus input pipe while
**  processing the software main loop sequence.
**
**  This is a fatal error that will cause the File Manager application
**  to terminate.
**
**  The result number in the message text is the error code returned
**  from the the call to the API function #CFE_SB_RcvMsg.
*/
#define FM_SB_RECEIVE_ERR_EID 7

/** \brief <tt> 'Application Terminating' </tt>
**  \event <tt> 'Application terminating: result = 0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is issued when the File Manager application is about
**  to terminate.
**
**  A non-zero result value in the event text is the error code from a
**  fatal error that has occurred.  Fatal errors all have descriptive
**  events.
**
**  If the result value in the event text is zero, then it is likely that
**  the CFE has terminated the FM application, presumably by command.
*/
#define FM_EXIT_ERR_EID 8

/** \brief <tt> 'Main Loop Error: Invalid Message ID' </tt>
**  \event <tt> 'Main loop error: invalid message ID: mid = 0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is issued when the File Manager application has
**  received an unexpected Software Bus packet.  There is no obvious
**  explanation of why or how FM could receive such a packet.
**
**  The number in the message text is the unexpected MessageID.
*/
#define FM_MID_ERR_EID 9

/** \brief <tt> 'Main Loop Error: Invalid Command Code' </tt>
**  \event <tt> 'Main loop error: invalid command code: cc = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is issued when the File Manager application has
**  received a command packet with an unexpected command code value.
**
**  Mal-formed command packets are generally prevented by the ground
**  system.  Therefore, the source for the problem command is likely
**  to be one of the on-board tables that contain commands.
**
**  The number in the message text is the unexpected command code.
*/
#define FM_CC_ERR_EID 10

/** \brief <tt> 'HK Request Error: Invalid Command Packet Length' </tt>
**  \event <tt> 'HK Request error: invalid command packet length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a housekeeping
**  request command packet with an invalid length.
*/
#define FM_HK_REQ_ERR_EID 11

/** \brief <tt> 'No-op Command Success' </tt>
**  \event <tt> 'No-op command: version \%d.\%d.\%d.\%d' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause
**
**  This event message signals the successful completion of a
**  /FM_Noop command.
**
**  The version data includes the application major version, minor version,
**  revision and mission revision numbers.
*/
#define FM_NOOP_CMD_EID 12

/** \brief <tt> 'No-op Error: Invalid Command Packet Length' </tt>
**  \event <tt> 'No-op error: invalid command packet length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Noop
**  command packet with an invalid length.
*/
#define FM_NOOP_PKT_ERR_EID 13

/** \brief <tt> 'Reset Counters Command Success' </tt>
**  \event <tt> 'Reset Counters command' </tt>
**
**  \par Type: DEBUG
**
**  This event is type debug because the command resets housekeeping
**  telemetry counters that also signal the completion of the command.
**
**  \par Cause
**
**  This event message signals the successful completion of a
**  /FM_ResetCtrs command.
*/
#define FM_RESET_CMD_EID 14

/** \brief <tt> 'Reset Counters Error: Invalid Command Packet Length' </tt>
**  \event <tt> 'Reset Counters error: invalid command packet length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_ResetCtrs
**  command packet with an invalid length.
*/
#define FM_RESET_PKT_ERR_EID 15

/** \brief <tt> 'Copy File Command Success' </tt>
**  \event <tt> 'Copy File command: src = \%s, tgt = \%s' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause
**
**  This event message signals the successful completion of a
**  /FM_Copy command.
**
**  Note that the execution of this command generally occurs within the
**  context of the FM low priority child task.  Thus this event may not
**  occur until some time after the command was invoked.  However, this
**  event message does signal the actual completion of the command.
*/
#define FM_COPY_CMD_EID 16

/** \brief <tt> 'Copy File Error: Invalid Command Packet Length' </tt>
**  \event <tt> 'Copy File error: invalid command packet length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Copy
**  command packet with an invalid length.
*/
#define FM_COPY_PKT_ERR_EID 17

/** \brief <tt> 'Copy File Error: Invalid Overwrite' </tt>
**  \event <tt> 'Copy File error: invalid overwrite = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Copy
**  command packet with an invalid overwrite argument.  Overwrite
**  must be set to TRUE (one) or FALSE (zero).
*/
#define FM_COPY_OVR_ERR_EID 18

/** \brief <tt> 'Copy File Error: OS Error' </tt>
**  \event <tt> 'Copy File error: OS_cp failed: result = \%d, src = \%s, tgt = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated due to an OS function error that
**  occurred after preliminary command argument verification tests
**  indicated that the source file exists and the target name
**  is unused and appears to be valid. Verify that the target
**  filename is reasonable.  Also, verify that the file system has
**  sufficient free space for this operation. Then refer to the OS
**  specific return value.
**
**  The numeric data in the event is the return value from the OS
**  function call.  The string data identifies the source and
**  target names for the file being copied.
*/
#define FM_COPY_OS_ERR_EID 19

/** \brief <tt> 'Move File Command Success' </tt>
**  \event <tt> 'Move File command: src = \%s, tgt = \%s' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause
**
**  This event message signals the successful completion of a
**  /FM_Move command.
*/
#define FM_MOVE_CMD_EID 20

/** \brief <tt> 'Move File Error: Invalid Command Packet Length' </tt>
**  \event <tt> 'Move File error: invalid command packet length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Move
**  command packet with an invalid length.
*/
#define FM_MOVE_PKT_ERR_EID 21

/** \brief <tt> 'Move File Error: Invalid Overwrite' </tt>
**  \event <tt> 'Move File error: invalid overwrite = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Move
**  command packet with an invalid overwrite argument.  Overwrite
**  must be set to TRUE (one) or FALSE (zero).
*/
#define FM_MOVE_OVR_ERR_EID 22

/** \brief <tt> 'Move File Error: OS Error' </tt>
**  \event <tt> 'Move File error: OS_mv error = 0x\%08X, src = \%s, tgt = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated due to an OS function error that
**  occurred after preliminary command argument verification tests
**  indicated that the source file exists and the target name
**  is unused and appears to be valid. Verify that the target
**  filename is reasonable.  Also, verify that the file system has
**  sufficient free space for this operation. Then refer to the OS
**  specific return value.
**
**  The numeric data in the event is the return value from the OS
**  function call.  The string data identifies the source and
**  target names for the file being moved.
*/
#define FM_MOVE_OS_ERR_EID 23

/** \brief <tt> 'Rename File Command Success' </tt>
**  \event <tt> 'Rename File command: src = \%s, tgt = \%s' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause
**
**  This event message signals the successful completion of a
**  /FM_Rename command.
*/
#define FM_RENAME_CMD_EID 24

/** \brief <tt> 'Rename File Error: Invalid Command Packet Length' </tt>
**  \event <tt> 'Rename File error: invalid command packet length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Rename
**  command packet with an invalid length.
*/
#define FM_RENAME_PKT_ERR_EID 25

/** \brief <tt> 'Rename File Error: Invalid Overwrite' </tt>
**  \event <tt> 'Rename File error: invalid overwrite = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Rename
**  command packet with an invalid overwrite argument.  Overwrite
**  must be set to TRUE (one) or FALSE (zero).
*/
#define FM_RENAME_OVR_ERR_EID 26

/** \brief <tt> 'Rename File Error: OS Error' </tt>
**  \event <tt> 'Rename File error: OS_rename error = 0x\%08X, src = \%s, tgt = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated due to an OS function error that
**  occurred after preliminary command argument verification tests
**  indicated that the source file exists and the target name
**  is unused and appears to be valid. Verify that the target
**  filename is reasonable.  Also, verify that the file system has
**  sufficient free space for this operation. Then refer to the OS
**  specific return value.
**
**  The numeric data in the event is the return value from the OS
**  function call.  The string data identifies the source and
**  target names for the file being renamed.
*/
#define FM_RENAME_OS_ERR_EID 27

/** \brief <tt> 'Delete File Command Success' </tt>
**  \event <tt> 'Delete File command: file = \%s' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause
**
**  This event message signals the successful completion of a
**  /FM_Delete command.
*/
#define FM_DELETE_CMD_EID 28

/** \brief <tt> 'Delete File Error: Invalid Command Packet Length' </tt>
**  \event <tt> 'Delete File error: invalid command packet length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Delete
**  command packet with an invalid length.
*/
#define FM_DELETE_PKT_ERR_EID 29

/** \brief <tt> 'Delete File Error: OS Error' </tt>
**  \event <tt> 'Delete File error: OS_remove error = 0x\%08X, file = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated due to an OS function error that
**  occurred after preliminary command argument verification tests
**  indicated that the filename exists and is not open. Refer to the
**  OS-specific return value for an indication of what might have
**  caused this error.
**
**  The numeric data in the event is the return value from the OS
**  function call.  The string data identifies the name of the file
**  being deleted.
*/
#define FM_DELETE_OS_ERR_EID 30

/** \brief <tt> 'Delete All Files Command Success' </tt>
**  \event <tt> 'Delete All Files command: deleted \%d of \%d dir entries: dir = \%s' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause
**
**  This event message signals the successful completion of a
**  /FM_DeleteAll command.
**
**  Note that the execution of this command generally occurs within the
**  context of the FM low priority child task.  Thus this event may not
**  occur until some time after the command was invoked.  However, this
**  event message does signal the actual completion of the command.
*/
#define FM_DELETE_ALL_CMD_EID 31

/** \brief <tt> 'Delete All Files command: one or more files could not be deleted. Files may be open : dir = %s' </tt>
**  \event <tt> 'Delete All Files command: one or more files could not be deleted. Files may be open : dir = %s' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause
**
**  This general event message is issued if for any reason some files
**  could not be deleted.
*/
#define FM_DELETE_ALL_FILES_ND_WARNING_EID 32

/** \brief <tt> 'Delete All Files command: one or more directories skipped : dir = %s' </tt>
**  \event <tt> 'Delete All Files command: one or more directories skipped : dir = %s' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause
**
**  This general event message is issued if for any reason a directory
**  skipped when processing a /FM_DeleteAll command.
*/
#define FM_DELETE_ALL_SKIP_WARNING_EID 33

/** \brief <tt> 'Delete All Files Error: Invalid Command Packet Length' </tt>
**  \event <tt> 'Delete All Files error: invalid command packet length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_DeleteAll
**  command packet with an invalid length.
*/
#define FM_DELETE_ALL_PKT_ERR_EID 34

/** \brief <tt> 'Delete All Files Error: OS Error' </tt>
**  \event <tt> 'Delete All Files error: OS_DirectoryOpen failed: dir = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated due to an OS function error that
**  occurred after preliminary command argument verification tests
**  indicated that the directory exists. Refer to the OS-specific
**  return value for an indication of what might have caused this
**  error.
**
**  The numeric data in the event is the return value from the OS
**  function call.  The string data identifies the name of the
**  directory or the directory entry.
*/
#define FM_DELETE_ALL_OS_ERR_EID 35

/** \brief <tt> 'Decompress File Command Success' </tt>
**  \event <tt> 'Decompress File command: src = \%s, tgt = \%s' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause
**
**  This event message signals the successful completion of a
**  /FM_Decompress command.
**
**  Note that the execution of this command generally occurs within the
**  context of the FM low priority child task.  Thus this event may not
**  occur until some time after the command was invoked.  However, this
**  event message does signal the actual completion of the command.
*/
#define FM_DECOM_CMD_EID 36

/** \brief <tt> 'Decompress File Error: Invalid Command Packet Length' </tt>
**  \event <tt> 'Decompress File error: invalid command packet length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Decompress
**  command packet with an invalid length.
*/
#define FM_DECOM_PKT_ERR_EID 37

/** \brief <tt> 'Decompress File Error: CFE Error' </tt>
**  \event <tt> 'Decompress File error: CFE_FS_Decompress failed: result = \%d, src = \%s, tgt = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated due to an API function error that
**  occurred after preliminary command argument verification tests
**  indicated that the source file exists. Refer to the function
**  specific return value for an indication of what might have caused
**  this particular error.
**
**  The numeric data in the event is the return value from the API
**  function call.  The string data identifies the names of the
**  source and target files.
*/
#define FM_DECOM_CFE_ERR_EID 38

/** \brief <tt> 'Concat Files Command Success' </tt>
**  \event <tt> 'Concat Files command: src1 = \%s, src2 = \%s, tgt = \%s' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause
**
**  This event message signals the successful completion of a
**  /FM_Concat command.
**
**  Note that the execution of this command generally occurs within the
**  context of the FM low priority child task.  Thus this event may not
**  occur until some time after the command was invoked.  However, this
**  event message does signal the actual completion of the command.
*/
#define FM_CONCAT_CMD_EID 39

/** \brief <tt> 'Concat Files Error: Invalid Command Packet Length' </tt>
**  \event <tt> 'Concat Files error: invalid command packet length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Concat
**  command packet with an invalid length.
*/
#define FM_CONCAT_PKT_ERR_EID 40

/** \brief <tt> 'Concat File Error: OS_cp failed: result = \%d, src = \%s, tgt = \%s'  </tt>
**  \event <tt> 'Concat File error: OS_cp failed: result = \%d, src = \%s, tgt = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the first source file cannot be
**  copied.
**
**  This event message is generated due to an API function error that
**  occurred after preliminary command argument verification tests
**  indicated that the source files exist. Refer to the function
**  specific return value for an indication of what might have caused
**  this particular error.
**
**  The numeric data in the event is the return value from the API
**  function call.  The string data identifies the name of the file(s)
**  being operated on by the API function.
*/
#define FM_CONCAT_OSCPY_ERR_EID 41

/** \brief <tt> 'Concat File Error: OS_open failed: result = \%d, src2 = \%s' </tt>
**  \event <tt> 'Concat File error: OS_open failed: result = \%d, src2 = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the second source file cannot
**  be opened.
**
**  This event message is generated due to an API function error that
**  occurred after preliminary command argument verification tests
**  indicated that the source files exist. Refer to the function
**  specific return value for an indication of what might have caused
**  this particular error.
**
**  The numeric data in the event is the return value from the API
**  function call.  The string data identifies the name of the file
**  being operated on by the API function.
*/
#define FM_CONCAT_OPEN_SRC2_ERR_EID 42

/** \brief <tt> 'Concat File Error: OS_open failed: result = \%d, tgt = \%s' </tt>
**  \event <tt> 'Concat File error: OS_open failed: result = \%d, tgt = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the target file cannot
**  be opened.
**
**  This event message is generated due to an API function error that
**  occurred after preliminary command argument verification tests
**  indicated that the source files exist. Refer to the function
**  specific return value for an indication of what might have caused
**  this particular error.
**
**  The numeric data in the event is the return value from the API
**  function call.  The string data identifies the name of the file(s)
**  being operated on by the API function.
*/
#define FM_CONCAT_OPEN_TGT_ERR_EID 43

/** \brief <tt> 'Concat File Error: OS_read failed: result = \%d, file = \%s' </tt>
**  \event <tt> 'Concat File error: OS_read failed: result = \%d, file = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the source file #2 cannot be read.
**
**  This event message is generated due to an API function error that
**  occurred after preliminary command argument verification tests
**  indicated that the source files exist. Refer to the function
**  specific return value for an indication of what might have caused
**  this particular error.
**
**  The numeric data in the event is the return value from the API
**  function call.  The string data identifies the name of the file
**  being operated on by the API function.
*/
#define FM_CONCAT_OSRD_ERR_EID 44

/** \brief <tt> 'Concat File Error: OS_write failed: result = \%d, expected = \%d' </tt>
**  \event <tt> 'Concat File error: OS_write failed: result = \%d, expected = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the target file cannot be written.
**
**  This event message is generated due to an API function error that
**  occurred after preliminary command argument verification tests
**  indicated that the source files exist. Refer to the function
**  specific return value for an indication of what might have caused
**  this particular error.
**
**  The numeric data in the event is the return value from the API
**  function call.  The string data identifies the name of the file(s)
**  being operated on by the API function.
*/
#define FM_CONCAT_OSWR_ERR_EID 45

/** \brief <tt> 'Get File Info Command Success' </tt>
**  \event <tt> 'Get File Info command: name = \%s' </tt>
**
**  \par Type: DEBUG
**
**  This event is type debug because the command generates a telemetry
**  packet that also signals the completion of the command.
**
**  \par Cause
**
**  This event message signals the successful completion of a
**  /FM_GetFileInfo command.
**
**  Note that the execution of this command generally occurs within the
**  context of the FM low priority child task.  Thus this event may not
**  occur until some time after the command was invoked.  However, this
**  event message does signal the actual completion of the command.
*/
#define FM_GET_FILE_INFO_CMD_EID 46

/** \brief <tt> 'Get File Info Warning: unable to compute CRC: invalid file state = \%d, file = \%s'  </tt>
**  \event <tt> 'Get File Info warning: unable to compute CRC: invalid file state = \%d, file = \%s' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause
**
**  This event message is generated when the CRC of a file cannot be
**  computed because the file has an invalid state.
**
**  This event message is generated due to an API function error that
**  occurred after preliminary command argument verification tests
**  indicated that the source files exist. Refer to the function
**  specific return value for an indication of what might have caused
**  this particular error.
**
**  The numeric data in the event is the return value from the API
**  function call.  The string data identifies the name of the file
**  being operated on by the API function.
*/
#define FM_GET_FILE_INFO_STATE_WARNING_EID 47

/** \brief <tt> 'Get File Info Warning: unable to compute CRC: invalid CRC type = \%d, file = \%s' </tt>
**  \event <tt> 'Get File Info warning: unable to compute CRC: invalid CRC type = \%d, file = \%s' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause
**
**  This event message is generated when the CRC of a file cannot be
**  computed because the CRC type is invalid.
**
**  This event message is generated due to an API function error that
**  occurred after preliminary command argument verification tests
**  indicated that the source files exist. Refer to the function
**  specific return value for an indication of what might have caused
**  this particular error.
**
**  The numeric data in the event is the return value from the API
**  function call.  The string data identifies the name of the file
**  being operated on by the API function.
*/
#define FM_GET_FILE_INFO_TYPE_WARNING_EID 48

/** \brief <tt> 'Get File Info Warning: unable to compute CRC: OS_open result = \%d, file = \%s' </tt>
**  \event <tt> 'Get File Info warning: unable to compute CRC: OS_open result = \%d, file = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the CRC of a file cannot be
**  computed because the file cannot be opened.
**
**  This event message is generated due to an API function error that
**  occurred after preliminary command argument verification tests
**  indicated that the source files exist. Refer to the function
**  specific return value for an indication of what might have caused
**  this particular error.
**
**  The numeric data in the event is the return value from the API
**  function call.  The string data identifies the name of the file
**  being operated on by the API function.
*/
#define FM_GET_FILE_INFO_OPEN_ERR_EID 49

/** \brief <tt> 'Get File Info Warning: unable to compute CRC: OS_read result = \%d, file = \%s' </tt>
**  \event <tt> 'Get File Info warning: unable to compute CRC: OS_read result = \%d, file = \%s' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause
**
**  This event message is generated when the CRC of a file cannot be
**  computed because the file cannot be read.
**
**  This event message is generated due to an API function error that
**  occurred after preliminary command argument verification tests
**  indicated that the source files exist. Refer to the function
**  specific return value for an indication of what might have caused
**  this particular error.
**
**  The numeric data in the event is the return value from the API
**  function call.  The string data identifies the name of the file
**  being operated on by the API function.
*/
#define FM_GET_FILE_INFO_READ_WARNING_EID 50

/** \brief <tt> 'Get File Info Error: Invalid Command Packet Length' </tt>
**  \event <tt> 'Get File Info error: invalid command packet length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_GetFileInfo
**  command packet with an invalid length.
*/
#define FM_GET_FILE_INFO_PKT_ERR_EID 51

/** \brief <tt> 'Get File Info Error: Source Filename' </tt>
**  \event <tt> 'Get File Info error: invalid name: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_GetFileInfo
**  command packet with an invalid filename.
*/
#define FM_GET_FILE_INFO_SRC_ERR_EID 52

/** \brief <tt> 'Get Open Files Command Success' </tt>
**  \event <tt> 'Get Open Files command' </tt>
**
**  \par Type: DEBUG
**
**  This event is type debug because the command generates a telemetry
**  packet that also signals the completion of the command.
**
**  \par Cause
**
**  This event message signals the successful completion of a
**  /FM_GetOpenFiles command.
*/
#define FM_GET_OPEN_FILES_CMD_EID 53

/** \brief <tt> 'Get Open Files Error: Invalid Command Packet Length' </tt>
**  \event <tt> 'Get Open Files error: invalid command packet length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_GetOpenFiles
**  command packet with an invalid length.
*/
#define FM_GET_OPEN_FILES_PKT_ERR_EID 54

/** \brief <tt> 'Create Directory Command Success' </tt>
**  \event <tt> 'Create Directory command: dir = \%s' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause
**
**  This event message signals the successful completion of a
**  /FM_CreateDir command.
*/
#define FM_CREATE_DIR_CMD_EID 55

/** \brief <tt> 'Create Directory Error: Invalid Command Packet Length' </tt>
**  \event <tt> 'Create Directory error: invalid command packet length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_CreateDir
**  command packet with an invalid length.
*/
#define FM_CREATE_DIR_PKT_ERR_EID 56

/** \brief <tt> 'Create Directory Error: OS Error' </tt>
**  \event <tt> 'Create Directory error: OS_mkdir failed: result = \%d, dir = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated due to an OS function error that
**  occurred after preliminary command argument verification tests
**  indicated that the directory name is unused and appears to be
**  valid. Refer to the OS specific return value.
**
**  The numeric data in the event is the return value from the OS
**  function call.  The string data identifies the directory name.
*/
#define FM_CREATE_DIR_OS_ERR_EID 57

/** \brief <tt> 'Delete Directory Command Success' </tt>
**  \event <tt> 'Delete Directory command: dir = \%s' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause
**
**  This event message signals the successful completion of a
**  /FM_DeleteDir command.
*/
#define FM_DELETE_DIR_CMD_EID 58

/** \brief <tt> 'Delete Directory Error: Invalid Command Packet Length' </tt>
**  \event <tt> 'Delete Directory error: invalid command packet length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_DeleteDir
**  command packet with an invalid length.
*/
#define FM_DELETE_DIR_PKT_ERR_EID 59

/** \brief <tt> 'Delete Directory Error: Source Filename' </tt>
**  \event <tt> 'Delete Directory error: directory is not empty: dir = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_DeleteDir
**  command packet that references a directory that is not empty.
*/
#define FM_DELETE_DIR_EMPTY_ERR_EID 60

/** \brief <tt> 'Delete Directory Error: OS_DirectoryOpen failed: dir = \%s' </tt>
**  \event <tt> 'Delete Directory error: OS_DirectoryOpen failed: dir = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated due to an OS function error that
**  occurred after preliminary command argument verification tests
**  indicated that the directory exists and appears to be valid.
**  Refer to the OS specific return values.
**
**  The numeric data in the event is the return value from the OS
**  function call.  The string data identifies the name of the
**  directory or the directory entry.
*/
#define FM_DELETE_OPENDIR_OS_ERR_EID 61

/** \brief <tt> 'Delete Directory Error: OS_rmdir failed: result = \%d, dir = \%s' </tt>
**  \event <tt> 'Delete Directory error: OS_rmdir failed: result = \%d, dir = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated due to an OS function error that
**  occurred after preliminary command argument verification tests
**  indicated that the directory exists and appears to be valid.
**  Refer to the OS specific return values.
**
**  The numeric data in the event is the return value from the OS
**  function call.  The string data identifies the name of the
**  directory or the directory entry.
*/
#define FM_DELETE_RMDIR_OS_ERR_EID 62

/** \brief <tt> 'Directory List to File command' </tt>
**  \event <tt> 'Directory List to File command: wrote \%d of \%d names: dir = \%s, filename = \%s' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause
**
**  This event message signals the successful completion of a
**  /FM_GetDirFile command.
**
**  Note that the execution of this command generally occurs within the
**  context of the FM low priority child task.  Thus this event may not
**  occur until some time after the command was invoked.  However, this
**  event message does signal the actual completion of the command.
**
**  Note that the execution of this command generally occurs within the
**  context of the FM low priority child task.  Thus this event may not
**  occur until some time after the command was invoked.  However, this
**  event message does signal the actual completion of the command.
*/
#define FM_GET_DIR_FILE_CMD_EID 63

/** \brief <tt> 'Directory List to File Error: Invalid Command Packet Length' </tt>
**  \event <tt> 'Directory List to File error: invalid command packet length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_GetDirFile
**  command packet with an invalid length.
*/
#define FM_GET_DIR_FILE_PKT_ERR_EID 64

/** \brief <tt> 'Directory List to File Warning: Pathname' </tt>
**  \event <tt> 'Directory List to File warning: combined directory and entry name too long: dir = \%s, entry = \%s'
*</tt>
**
**  \par Type: INFORMATION
**
**  \par Cause
**
**  This event message is generated when the combined length of the
**  directory name plus the directory entry name exceeds the maximum
**  qualified filename length.  It is unclear how this condition
**  might arise, but since we are copying both strings into a fixed
**  length buffer, we must first verify the length.
**
**  The /FM_GetDirFile command handler will not write information
**  regarding this directory entry to the output file.
*/
#define FM_GET_DIR_FILE_WARNING_EID 65

/** \brief <tt> 'Directory List to File Error: OS_DirectoryOpen failed: dir = \%s' </tt>
**  \event <tt> 'Directory List to File error: OS_DirectoryOpen failed: dir = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated due to an OS function error that
**  occurred after preliminary command argument verification tests
**  indicated that the directory exists and the output filename
**  is unused and appears to be valid. Verify that the output
**  filename is reasonable.  Also, verify that the file system has
**  sufficient free space for this operation. Then refer to the OS
**  specific return values.
**
**  The numeric data in the event is the return value from the OS
**  function call.  The string data identifies the name of the
**  directory or the directory entry.
*/
#define FM_GET_DIR_FILE_OSOPENDIR_ERR_EID 66

/** \brief <tt> 'Directory List to File Error: OS_write blank stats failed: result = \%d, expected = \%d' </tt>
**  \event <tt> 'Directory List to File error: OS_write blank stats failed: result = \%d, expected = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated due to an error when writing a
**  blank stats structure using the OS_write function.  This
**  occurred after preliminary command argument verification tests
**  indicated that the directory exists and the output filename
**  is unused and appears to be valid. Verify that the output
**  filename is reasonable.  Also, verify that the file system has
**  sufficient free space for this operation. Then refer to the OS
**  specific return values.
**
**  The numeric data in the event is the return value from the OS_write
**  function call.  The string data identifies the name of the
**  directory or the directory entry.
*/
#define FM_GET_DIR_FILE_WRBLANK_ERR_EID 67

/** \brief <tt> 'Directory List to File Error: CFE_FS_WriteHeader failed: result = \%d, expected = \%d'  </tt>
**  \event <tt> 'Directory List to File error: CFE_FS_WriteHeader failed: result = \%d, expected = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the header cannot be written
**  to the file using #CFE_FS_WriteHeader.  This error
**  occurred after preliminary command argument verification tests
**  indicated that the directory exists and the output filename
**  is unused and appears to be valid. Verify that the output
**  filename is reasonable.  Also, verify that the file system has
**  sufficient free space for this operation. Then refer to the OS
**  specific return values.
**
**  The numeric data in the event is the return value from the OS
**  function call.  The string data identifies the name of the
**  directory or the directory entry.
*/
#define FM_GET_DIR_FILE_WRHDR_ERR_EID 68

/** \brief <tt> 'Directory List to File Error: OS_creat failed: result = \%d, file = \%s' </tt>
**  \event <tt> 'Directory List to File error: OS_creat failed: result = \%d, file = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated due to an OS function error that
**  prevents the output file from being created.  This error
**  occurred after preliminary command argument verification tests
**  indicated that the directory exists and the output filename
**  is unused and appears to be valid. Verify that the output
**  filename is reasonable.  Also, verify that the file system has
**  sufficient free space for this operation. Then refer to the OS
**  specific return values.
**
**  The numeric data in the event is the return value from the OS
**  function call.  The string data identifies the name of the
**  directory or the directory entry.
*/
#define FM_GET_DIR_FILE_OSCREAT_ERR_EID 69

/** \brief <tt> 'Directory List to File Error: OS_write entry failed: result = \%d, expected = \%d'' </tt>
**  \event <tt> 'Directory List to File error: OS_write entry failed: result = \%d, expected = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated due to an OS function error that
**  prevents an entry from being written.  This error
**  occurred after preliminary command argument verification tests
**  indicated that the directory exists and the output filename
**  is unused and appears to be valid. Verify that the output
**  filename is reasonable.  Also, verify that the file system has
**  sufficient free space for this operation. Then refer to the OS
**  specific return values.
**
**  The numeric data in the event is the return value from the OS
**  function call.  The string data identifies the name of the
**  directory or the directory entry.
*/
#define FM_GET_DIR_FILE_WRENTRY_ERR_EID 70

/** \brief <tt> 'Directory List to File Error: OS_write update stats failed: result = \%d, expected = \%d'  </tt>
**  \event <tt> 'Directory List to File error: OS_write update stats failed: result = \%d, expected = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated due to an OS function error that
**  prevents updated statistics from being written to a file.  This
**  occurred after preliminary command argument verification tests
**  indicated that the directory exists and the output filename
**  is unused and appears to be valid. Verify that the output
**  filename is reasonable.  Also, verify that the file system has
**  sufficient free space for this operation. Then refer to the OS
**  specific return values.
**
**  The numeric data in the event is the return value from the OS
**  function call.  The string data identifies the name of the
**  directory or the directory entry.
*/
#define FM_GET_DIR_FILE_UPSTATS_ERR_EID 71

/** \brief <tt> 'Directory List to Packet command' </tt>
**  \event <tt> 'Directory List to Packet command: offset = \%d, dir = \%s' </tt>
**
**  \par Type: DEBUG
**
**  This event is type debug because the command generates a telemetry
**  packet that also signals the completion of the command.
**
**  \par Cause
**
**  This event message signals the successful completion of a
**  /FM_GetDirPkt command.
**
**  Note that the execution of this command generally occurs within the
**  context of the FM low priority child task.  Thus this event may not
**  occur until some time after the command was invoked.  However, this
**  event message does signal the actual completion of the command.
*/
#define FM_GET_DIR_PKT_CMD_EID 72

/** \brief <tt> 'Directory List to Packet Warning' </tt>
**  \event <tt> 'Directory List to Packet warning: dir + entry is too long: dir = \%s, entry = \%s' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause
**
**  This event message is generated when the combined length of the
**  directory name plus the directory entry name exceeds the maximum
**  qualified filename length.  It is unclear how this condition
**  might arise, but since we are copying both strings into a fixed
**  length buffer, we must first verify the length.
*/
#define FM_GET_DIR_PKT_WARNING_EID 73

/** \brief <tt> 'Directory List to Packet Error: Invalid Command Packet Length' </tt>
**  \event <tt> 'Directory List to Packet error: invalid command packet length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_GetDirPkt
**  command packet with an invalid length.
*/
#define FM_GET_DIR_PKT_PKT_ERR_EID 74

/** \brief <tt> 'Directory List to Packet Error: OS Error' </tt>
**  \event <tt> 'Directory List to Packet error: OS_DirectoryOpen failed: dir = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  The numeric data in the event is the return value from the OS
**  function call.  The string data identifies the name of the
**  directory or the directory entry.
*/
#define FM_GET_DIR_PKT_OS_ERR_EID 75

/** \brief <tt> 'Get Free Space Command' </tt>
**  \event <tt> 'Get Free Space command' </tt>
**
**  \par Type: DEBUG
**
**  This event is type debug because the command generates a telemetry
**  packet that also signals the completion of the command.
**
**  \par Cause
**
**  This event message signals the successful completion of a
**  /FM_GetFreeSpace command.
*/
#define FM_GET_FREE_SPACE_CMD_EID 76

/** \brief <tt> 'Get Free Space Error: Invalid Command Packet Length' </tt>
**  \event <tt> 'Get Free Space error: invalid command packet length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_GetFreeSpace
**  command packet with an invalid length.
*/
#define FM_GET_FREE_SPACE_PKT_ERR_EID 77

/** \brief <tt> 'Get Free Space Error: Table Not Loaded' </tt>
**  \event <tt> 'Get Free Space error: file system free space table is not loaded' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_GetFreeSpace
**  command packet when the FM file system free space table has not yet
**  been loaded.
*/
#define FM_GET_FREE_SPACE_TBL_ERR_EID 78

/** \brief <tt> 'Set Table State Command' </tt>
**  \event <tt> 'Set Table State command: index = \%d, state = \%d' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause
**
**  This event message signals the successful completion of a
**  /FM_SetTableState command.
*/
#define FM_SET_TABLE_STATE_CMD_EID 79

/** \brief <tt> 'Set Table State Error: Invalid Command Packet Length' </tt>
**  \event <tt> 'Set Table State error: invalid command packet length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_SetTableState
**  command packet with an invalid length.
*/
#define FM_SET_TABLE_STATE_PKT_ERR_EID 80

/** \brief <tt> 'Set Table State Error: Table Not Loaded' </tt>
**  \event <tt> 'Set Table State error: file system free space table is not loaded' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_SetTableState
**  command packet when the FM file system free space table has not yet
**  been loaded.
*/
#define FM_SET_TABLE_STATE_TBL_ERR_EID 81

/** \brief <tt> 'Set Table State Error: invalid command argument: index = \%d' </tt>
**  \event <tt> 'Set Table State error: invalid command argument: index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a FM_SetTableState
**  command packet with an invalid table index argument.
*/
#define FM_SET_TABLE_STATE_ARG_IDX_ERR_EID 82

/** \brief <tt> 'Set Table State Error: invalid command argument: state = \%d' </tt>
**  \event <tt> 'Set Table State error: invalid command argument: state = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a FM_SetTableState
**  command packet with an invalid entry state argument.
*/
#define FM_SET_TABLE_STATE_ARG_STATE_ERR_EID 83

/** \brief <tt> 'Set Table State Error: Unused Table Entry' </tt>
**  \event <tt> 'Set Table State error: cannot modify unused table entry: index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_SetTableState
**  command packet that references an unused free space table entry.
*/
#define FM_SET_TABLE_STATE_UNUSED_ERR_EID 84

/** \brief <tt> 'Free Space Table Verify Error: index = \%d, empty name string' </tt>
**  \event <tt> 'Free Space Table verify error: index = \%d, empty name string' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when a file system free space table fails the table
**  verification process because the file system name is an empty string.  Each file
**  system table entry has only 2 fields: table entry state and file system name.
**  The table entry state field must be either enabled or disabled.  The file system
**  name string must have a non-zero length, include a string terminator and not
**  contain characters considered invalid for filenames.
**
**  If the file system free space table loaded at startup fails verification, the FM
**  application will not terminate.  However, the FM application will not process
**  commands that request the file system free space telemetry packet if a file
**  system free space table has not been successfully loaded.  Thereafter, if an
**  attempt to load a new table fails verification, the FM application will continue
**  to use the previous table.
*/
#define FM_TABLE_VERIFY_EMPTY_ERR_EID 85

/** \brief <tt> 'Free Space Table Verify Error: index = \%d, name too long' </tt>
**  \event <tt> 'Free Space Table verify error: index = \%d, name too long' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when a file system free space table fails the table
**  verification process because the file system name is too long.  Each file
**  system table entry has only 2 fields: table entry state and file system name.
**  The table entry state field must be either enabled or disabled.  The file system
**  name string must have a non-zero length, include a string terminator and not
**  contain characters considered invalid for filenames.
*
**  If the file system free space table loaded at startup fails verification, the FM
**  application will not terminate.  However, the FM application will not process
**  commands that request the file system free space telemetry packet if a file
**  system free space table has not been successfully loaded.  Thereafter, if an
**  attempt to load a new table fails verification, the FM application will continue
**  to use the previous table.
*/
#define FM_TABLE_VERIFY_TOOLONG_ERR_EID 86

/** \brief <tt> 'Free Space Table Verify Error: index = \%d, invalid name = \%s' </tt>
**  \event <tt> 'Free Space Table verify error: index = \%d, invalid name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when a file system free space table fails the table
**  verification process because the file system name has invalid characters. Each file
**  system table entry has only 2 fields: table entry state and file system name.
**  The table entry state field must be either enabled or disabled.  The file system
**  name string must have a non-zero length, include a string terminator and not
**  contain characters considered invalid for filenames.
*
**  If the file system free space table loaded at startup fails verification, the FM
**  application will not terminate.  However, the FM application will not process
**  commands that request the file system free space telemetry packet if a file
**  system free space table has not been successfully loaded.  Thereafter, if an
**  attempt to load a new table fails verification, the FM application will continue
**  to use the previous table.
*/
#define FM_TABLE_VERIFY_INVALID_ERR_EID 87

/** \brief <tt> 'Free Space Table Verify Error: index = \%d, invalid state = \%d' </tt>
**  \event <tt> 'Free Space Table verify error: index = \%d, invalid state = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when a file system free space table fails the table
**  verification process because a table entry has an invalid state. Each file
**  system table entry has only 2 fields: table entry state and file system name.
**  The table entry state field must be either enabled or disabled.  The file system
**  name string must have a non-zero length, include a string terminator and not
**  contain characters considered invalid for filenames.
*
**  If the file system free space table loaded at startup fails verification, the FM
**  application will not terminate.  However, the FM application will not process
**  commands that request the file system free space telemetry packet if a file
**  system free space table has not been successfully loaded.  Thereafter, if an
**  attempt to load a new table fails verification, the FM application will continue
**  to use the previous table.
*/
#define FM_TABLE_VERIFY_BAD_STATE_ERR_EID 88

/** \brief <tt> 'Child Task Initialization Complete' </tt>
**  \event <tt> 'Child Task initialization complete' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause
**
**  This event message signals the successful completion of the initialization
**  process for the FM child task.
*/
#define FM_CHILD_INIT_EID 89

/** \brief <tt> 'Child Task Initialization Error: create semaphore failed: result = \%d' </tt>
**  \event <tt> 'Child Task initialization error: create semaphore failed: result = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message indicates an unsuccessful attempt to create the sempahore
**  for the low priority FM child task.  Commands which would have otherwise been
**  handed off to the child task for execution, will now be processed by the main FM
**  application.
*/
#define FM_CHILD_INIT_SEM_ERR_EID 90

/** \brief <tt> 'Child Task Initialization Error: create queue count semaphore failed: result = \%d'  </tt>
**  \event <tt> 'Child Task initialization error: create queue count semaphore failed: result = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message indicates an unsuccessful attempt to create the queue count
**  semphore for the FM child task. Commands which would have otherwise been handed off
**  to the child task for execution, will now be processed by the main FM application.
**  Refer to the return code in the event text for the exact cause of the error.
*/
#define FM_CHILD_INIT_QSEM_ERR_EID 91

/** \brief <tt> 'Child Task Initialization Error: create task failed: result = \%d' </tt>
**  \event <tt> 'Child Task initialization error: create task failed: result = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message indicates an unsuccessful attempt to create the low
**  priority FM child task.  Commands which would have otherwise been handed off
**  to the child task for execution, will now be processed by the main FM application.
**  Refer to the return code in the event text for the exact cause of the error.
*/
#define FM_CHILD_INIT_CREATE_ERR_EID 92

/** \brief <tt> 'Child Task Termination Error: empty queue' </tt>
**  \event <tt> 'Child Task termination error: empty queue' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message indicates that the FM child task has suffered a fatal error
**  and has terminated.  The error occurred because the child queue was empty,
**  indicating that the handshake between the main task and child task was
**  broken.
*/
#define FM_CHILD_TERM_EMPTYQ_ERR_EID 93

/** \brief <tt> 'Child Task Termination Error: invalid queue index: index = \%d' </tt>
**  \event <tt> 'Child Task termination error: invalid queue index: index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message indicates that the FM child task has suffered a fatal error
**  and has terminated.  The error occurred because the child read index was
**  invalid (larger than the child queue depth).
*/
#define FM_CHILD_TERM_QIDX_ERR_EID 94

/** \brief <tt> 'Child Task Termination Error: semaphore take failed: result = \%d' </tt>
**  \event <tt> 'Child Task termination error: semaphore take failed: result = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message indicates that the FM child task has suffered a fatal error
**  and has terminated.  The error occurred when trying to take the child
**  handshake semaphore.
*/
#define FM_CHILD_TERM_SEM_ERR_EID 95

/** \brief <tt> 'Child Task Execution Error' </tt>
**  \event <tt> 'Child Task execution error: invalid command code: cc = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message indicates that the FM child task is unable to process the current
**  handshake request.  Either the handshake queue index or the handshake command code
**  is invalid.  This error suggests that either the handshake interface logic is flawed,
**  or there has been some sort of data corruption that affected the interface data.
**  It may be necessary to restart the FM application to resync the handshake interface.
*/
#define FM_CHILD_EXE_ERR_EID 96

/**
**  \brief <tt> 'Free Space Table Validation Results' </tt>
**
**  \event <tt> 'Free Space Table verify results: good = \%d, bad = \%d, unused = \%d' </tt>
**
**  \par Type: INFORMATION
**
**  \par Cause:
**
**  This event describes the results of the Free Space Table validation
**  function.  The cFE Table Services Manager will call this function autonomously
**  when the default table is loaded at startup and also whenever a table validate
**  command (that targets this table) is processed.
**
**  The event text will indicate
**  the number of table entries that were verified without error (good), the number
**  of table entries that had one or more errors (bad) and the number of unused
**  table entries (unused).  Thus, the sum of good + bad
**  + unused results will equal the total number of table entries.
*/
#define FM_TABLE_VERIFY_EID 97

/** \brief <tt> 'Set Permissions error: Invalid Command Packet Length' </tt>
**  \event <tt> 'Set Permissions error: invalid command packet length: expected = \%d, actual = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_SetPermissions
**  command packet with an invalid length.
*/
#define FM_SET_PERM_ERR_EID 98

/** \brief <tt> 'Set Permissions Command' </tt>
**  \event <tt> 'Set Permissions command: filename = \%s, state = \%d' </tt>
**
**  \par Type: DEBUG
**
**  \par Cause
**
**  This event message signals the successful completion of a
**  /FM_SetPerm command.
*/
#define FM_SET_PERM_CMD_EID 99

/** \brief <tt> 'Set Permissions: OS_chmod error: ' </tt>
**  \event <tt> 'Set Permissions: OS_chmod error, RC=0x%08X, file = %s, access = %d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_SetPerm
**  command packet with an invalid length.
*/
#define FM_SET_PERM_OS_ERR_EID 100

/** \brief <tt> 'Free Space Table verify error - null pointer detected' </tt>
**  \event <tt> 'Free Space Table verify error - null pointer detected' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message occurs when the FM validate table callback function
**  receives a NULL pointer as the input argument.
*/
#define FM_TABLE_VERIFY_NULL_PTR_ERR_EID 101

/** \brief <tt> 'Main loop error: SB returned NULL pointer on success' </tt>
**  \event <tt> 'Main loop error: SB returned NULL pointer on success' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message occurs if the Software Bus returns a success status
**  in the main loop but provided a NULL pointer as the return argument.
*/
#define FM_SB_RECEIVE_NULL_PTR_ERR_EID 102

/** \brief <tt> 'Could not get file system free space for \%s. Returned 0x\%08X' </tt>
**  \event <tt> 'Could not get file system free space for \%s. Returned 0x\%08X' </tt>
**
**  \par Type: ERROR
**
**  \par Cause:
**
**  This event message occurs if the free space for a file system cannot be read
**  when processing the #FM_GetFreeSpaceCmd command.
*/
#define FM_OS_SYS_STAT_ERR_EID 103

/** -------------------------------------------------------------
**  NOTE: From here on, the event IDs will take the form of a
**  "base" EID + an offset.  This is done to allow unique event
**  IDs to be sent from utility functions.
** --------------------------------------------------------------*/

#define FM_FNAME_INVALID_EID_OFFSET  0
#define FM_FNAME_DNE_EID_OFFSET      1
#define FM_FNAME_EXIST_EID_OFFSET    1 /* mutually exclusive with DNE */
#define FM_FNAME_ISDIR_EID_OFFSET    2
#define FM_FNAME_ISFILE_EID_OFFSET   2 /* mutually exclusive with ISDIR */
#define FM_FNAME_ISOPEN_EID_OFFSET   3
#define FM_FNAME_ISCLOSED_EID_OFFSET 4
#define FM_FNAME_UNKNOWN_EID_OFFSET  5
#define FM_FNAME_NUM_OFFSETS         6

#define FM_CHILD_DISABLED_EID_OFFSET 0
#define FM_CHILD_Q_FULL_EID_OFFSET   1
#define FM_CHILD_BROKEN_EID_OFFSET   2
#define FM_CHILD_NUM_OFFSETS         3

/** \brief <tt> 'Copy File Error: Source Filename' </tt>
**  \event <tt> 'Copy File error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Copy File error: file does not exist: name = \%s' </tt>
**  \event <tt> 'Copy File error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This is the base for a number of error events generated when the
**  /FM_Copy is received with an unusable source filename.
*/
#define FM_COPY_SRC_BASE_EID 151

/** \brief <tt> 'Copy File error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Copy File error: filename is invalid: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Copy
**  command packet with an invalid source filename.
**
**  Value: 151
*/
#define FM_COPY_SRC_INVALID_ERR_EID (FM_COPY_SRC_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/** \brief <tt> 'Copy File error: file does not exist: name = \%s' </tt>
**  \event <tt> 'Copy File error: file does not exist: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Copy
**  command packet with a source filename that does not exist.
**
**  Value: 152
*/
#define FM_COPY_SRC_DNE_ERR_EID (FM_COPY_SRC_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/** \brief <tt> 'Copy File error: filename is a directory: name = \%s' </tt>
**  \event <tt> 'Copy File error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Copy
**  command packet with a source filename that is a directory.
**
**  Value: 153
*/
#define FM_COPY_SRC_ISDIR_ERR_EID (FM_COPY_SRC_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/** \brief <tt> 'Copy File error: filename has unknown state: name = \%s, state = \%d' </tt>
**  \event <tt> 'Copy File error: filename has unknown state: name = \%s, state = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Copy
**  command packet with a source filename of unknown state.
**
**  Value: 156
*/
#define FM_COPY_SRC_UNKNOWN_ERR_EID (FM_COPY_SRC_BASE_EID + FM_FNAME_UNKNOWN_EID_OFFSET)

/** \brief <tt> 'Copy File Error: Target Filename' </tt>
**  \event <tt> 'Copy File error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Copy File error: file already exists: name = \%s' </tt>
**  \event <tt> 'Copy File error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This is the base EID for a number of error events related to the
**  target file in an /FM_COPY command.
**
**  Value: 157
*/
#define FM_COPY_TGT_BASE_EID (FM_COPY_SRC_BASE_EID + FM_FNAME_NUM_OFFSETS)

/** \brief <tt> 'Copy File Error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Copy File error: filename is invalid: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Copy
**  command packet with an invalid target filename.
**
**  Value: 157
*/
#define FM_COPY_TGT_INVALID_ERR_EID (FM_COPY_TGT_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/** \brief <tt> 'Copy File error: file already exists: name = \%s' </tt>
**  \event <tt> 'Copy File error: file already exists: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Copy
**  command packet with a target filename that already exists.
**
**  Value: 158
*/
#define FM_COPY_TGT_EXIST_ERR_EID (FM_COPY_TGT_BASE_EID + FM_FNAME_EXIST_EID_OFFSET)

/** \brief <tt> 'Copy File error: filename is a directory: name = \%s' </tt>
**  \event <tt> 'Copy File error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Copy
**  command packet with a target filename that is a directory.
**
**  Value: 159
*/
#define FM_COPY_TGT_ISDIR_ERR_EID (FM_COPY_TGT_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/** \brief <tt> 'Copy File error: filename exists as an open file: name = \%s' </tt>
**  \event <tt> 'Copy File error: filename exists as an open file: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Copy
**  command packet with a target filename that is open.
**
**  Value: 160
*/
#define FM_COPY_TGT_ISOPEN_ERR_EID (FM_COPY_TGT_BASE_EID + FM_FNAME_ISOPEN_EID_OFFSET)

/** \brief <tt> 'Copy File error: filename has unknown state: name = \%s, state = \%d' </tt>
**  \event <tt> 'Copy File error: filename has unknown state: name = \%s, state = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Copy
**  command packet with a target filename with unknown state.
**
**  Value: 162
*/
#define FM_COPY_TGT_UNKNOWN_ERR_EID (FM_COPY_TGT_BASE_EID + FM_FNAME_UNKNOWN_EID_OFFSET)

/** \brief <tt> 'Copy File Error: Child Task' </tt>
**  \event <tt> 'Copy File error: child task is disabled' </tt>
**  \event <tt> 'Copy File error: child task queue is full' </tt>
**  \event <tt> 'Copy File error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This is the base for any of several messages that are  generated when
**  the FM child task command queue interface cannot be used.
**
**  Value: 163
*/
#define FM_COPY_CHILD_BASE_EID (FM_COPY_TGT_BASE_EID + FM_FNAME_NUM_OFFSETS)

/** \brief <tt> 'Copy File Error: child task is disabled' </tt>
**  \event <tt> 'Copy File error: child task is disabled' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task is disabled.
**
**  Value: 163
*/
#define FM_COPY_CHILD_DISABLED_ERR_EID (FM_COPY_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/** \brief <tt> 'Copy File Error: child task queue is full' </tt>
**  \event <tt> 'Copy File error: child task queue is full' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task comand queue is full.
**
**  If the child task command queue is full, the problem may be temporary,
**  caused by sending too many FM commands too quickly.  If the command
**  queue does not empty itself within a reasonable amount of time then
**  the child task may be hung. It may be possible to use CFE commands to
**  terminate the child task, which should then cause FM to process all
**  commands in the main task.
**
**  Value: 164
*/
#define FM_COPY_CHILD_FULL_ERR_EID (FM_COPY_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/** \brief <tt> 'Copy File Error: child task interface is broken: count = \%d, index = \%d' </tt>
**  \event <tt> 'Copy File error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the interface between the main task
**  and child task is broken.
**
**  If the child task queue is broken then either the handshake interface
**  logic is flawed, or there has been some sort of data corruption that
**  affected the interface control variables.  In either case, it may be
**  necessary to restart the FM application to resync the interface.
**
**  Value: 165
**
*/
#define FM_COPY_CHILD_BROKEN_ERR_EID (FM_COPY_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/** \brief <tt> 'Move File Error: Source Filename' </tt>
**  \event <tt> 'Move File error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Move File error: file does not exist: name = \%s' </tt>
**  \event <tt> 'Move File error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Move
**  command packet with a source filename that is unusable for one
**  of several reasons.
**
**  Value: 166
*/
#define FM_MOVE_SRC_BASE_EID (FM_COPY_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/** \brief <tt> 'Move File Error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Move File error: filename is invalid: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Move
**  command packet with an invalid source filename.
**
**  Value: 166
*/
#define FM_MOVE_SRC_INVALID_ERR_EID (FM_MOVE_SRC_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/** \brief <tt> 'Move File Error: file does not exist: name = \%s' </tt>
**  \event <tt> 'Move File error: file does not exist: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Move
**  command packet with a source filename that does not exist.
**
**  Value: 167
*/
#define FM_MOVE_SRC_DNE_ERR_EID (FM_MOVE_SRC_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/** \brief <tt> 'Move File Error: filename is a directory: name = \%s'  </tt>
**  \event <tt> 'Move File error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Move
**  command packet with a source filename that is a directory.
**
**  Value: 168
*/
#define FM_MOVE_SRC_ISDIR_ERR_EID (FM_MOVE_SRC_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/** \brief <tt> 'Move File Error: Target Filename' </tt>
**  \event <tt> 'Move File error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Move File error: file does not exist: name = \%s' </tt>
**  \event <tt> 'Move File error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Move
**  command packet with a target filename that is unusable for one
**  of several reasons.
**
**  Value: 172
*/
#define FM_MOVE_TGT_BASE_EID (FM_MOVE_SRC_BASE_EID + FM_FNAME_NUM_OFFSETS)

/** \brief <tt> 'Move File Error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Move File error: filename is invalid: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Move
**  command packet with an invalid target filename.
**
**  Value: 172
*/
#define FM_MOVE_TGT_INVALID_ERR_EID (FM_MOVE_TGT_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/** \brief <tt> 'Move File Error: file already exists: name = \%s' </tt>
**  \event <tt> 'Move File error: file already exists: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Move
**  command packet with a target filename that already exists
**
**  Value: 173
*/
#define FM_MOVE_TGT_EXIST_ERR_EID (FM_MOVE_TGT_BASE_EID + FM_FNAME_EXIST_EID_OFFSET)

/** \brief <tt> 'Move File Error: filename is a directory: name = \%s'  </tt>
**  \event <tt> 'Move File error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Move
**  command packet with a target filename that is a directory.
**
**  Value: 174
*/
#define FM_MOVE_TGT_ISDIR_ERR_EID (FM_MOVE_TGT_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/** \brief <tt> 'Move File error: filename exists as an open file: name = \%s' </tt>
**  \event <tt> 'Move File error: filename exists as an open file: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Move
**  command packet with a target filename that is open.
**
**  Value: 175
*/
#define FM_MOVE_TGT_ISOPEN_ERR_EID (FM_MOVE_TGT_BASE_EID + FM_FNAME_ISOPEN_EID_OFFSET)

/** \brief <tt> 'Move File error: filename has unknown state: name = \%s, state = \%d' </tt>
**  \event <tt> 'Move File error: filename has unknown state: name = \%s, state = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Copy
**  command packet with a target file with unknown state..
**
**  Value: 177
*/
#define FM_MOVE_TGT_UNKNOWN_ERR_EID (FM_MOVE_TGT_BASE_EID + FM_FNAME_UNKNOWN_EID_OFFSET)

/** \brief <tt> 'Move File Error: Child Task' </tt>
**  \event <tt> 'Move File error: child task is disabled' </tt>
**  \event <tt> 'Move File error: child task queue is full' </tt>
**  \event <tt> 'Move File error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This is the base for any of several messages that are  generated when
**  the FM child task command queue interface cannot be used.
**
**  Value: 178
*/
#define FM_MOVE_CHILD_BASE_EID (FM_MOVE_TGT_BASE_EID + FM_FNAME_NUM_OFFSETS)

/** \brief <tt> 'Move File Error: child task is disabled' </tt>
**  \event <tt> 'Move File error: child task is disabled' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task is disabled.
**
**  Value: 178
*/
#define FM_MOVE_CHILD_DISABLED_ERR_EID (FM_MOVE_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/** \brief <tt> 'Move File Error: child task queue is full' </tt>
**  \event <tt> 'Move File error: child task queue is full' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task comand queue is full.
**
**  If the child task command queue is full, the problem may be temporary,
**  caused by sending too many FM commands too quickly.  If the command
**  queue does not empty itself within a reasonable amount of time then
**  the child task may be hung. It may be possible to use CFE commands to
**  terminate the child task, which should then cause FM to process all
**  commands in the main task.
**
**  Value: 179
*/
#define FM_MOVE_CHILD_FULL_ERR_EID (FM_MOVE_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/** \brief <tt> 'Move File Error: child task interface is broken: count = \%d, index = \%d' </tt>
**  \event <tt> 'Move File error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the interface between the main task
**  and child task is broken.
**
**  If the child task queue is broken then either the handshake interface
**  logic is flawed, or there has been some sort of data corruption that
**  affected the interface control variables.  In either case, it may be
**  necessary to restart the FM application to resync the interface.
**
**  Value: 180
**
*/
#define FM_MOVE_CHILD_BROKEN_ERR_EID (FM_MOVE_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/** \brief <tt> 'Rename File Error: Source Filename' </tt>
**  \event <tt> 'Rename File error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Rename File error: file does not exist: name = \%s' </tt>
**  \event <tt> 'Rename File error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Rename
**  command packet with a source filename that is unusable for one
**  of several reasons.
**
**  Value: 181
*/
#define FM_RENAME_SRC_BASE_EID (FM_MOVE_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/** \brief <tt> 'Rename File Error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Rename File error: filename is invalid: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Rename
**  command packet with an invalid source filename.
**
**  Value: 181
*/
#define FM_RENAME_SRC_INVALID_ERR_EID (FM_RENAME_SRC_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/** \brief <tt> 'Rename File Error: file does not exist: name = \%s' </tt>
**  \event <tt> 'Rename File error: file does not exist: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Rename
**  command packet with a source filename that does not exist.
**
**  Value: 182
*/
#define FM_RENAME_SRC_DNE_ERR_EID (FM_RENAME_SRC_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/** \brief <tt> 'Rename File Error: filename is a directory: name = \%s'  </tt>
**  \event <tt> 'Rename File error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Rename
**  command packet with a source filename that is a directory.
**
**  Value: 183
*/
#define FM_RENAME_SRC_ISDIR_ERR_EID (FM_RENAME_SRC_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/** \brief <tt> 'Rename File Error: Target Filename' </tt>
**  \event <tt> 'Rename File error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Rename File error: file does not exist: name = \%s' </tt>
**  \event <tt> 'Rename File error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Rename
**  command packet with a target filename that is unusable for one
**  of several reasons.
**
**  Value: 187
*/
#define FM_RENAME_TGT_BASE_EID (FM_RENAME_SRC_BASE_EID + FM_FNAME_NUM_OFFSETS)

/** \brief <tt> 'Rename File Error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Rename File error: filename is invalid: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Rename
**  command packet with an invalid target filename.
**
**  Value: 187
*/
#define FM_RENAME_TGT_INVALID_ERR_EID (FM_RENAME_TGT_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/** \brief <tt> 'Rename File Error: file already exists: name = \%s' </tt>
**  \event <tt> 'Rename File error: file already exists: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Rename
**  command packet with a target filename that already exists.
**
**  Value: 188
*/
#define FM_RENAME_TGT_EXIST_ERR_EID (FM_RENAME_TGT_BASE_EID + FM_FNAME_EXIST_EID_OFFSET)

/** \brief <tt> 'Rename File Error: filename is a directory: name = \%s'  </tt>
**  \event <tt> 'Rename File error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Rename
**  command packet with a target filename that is a directory.
**
**  Value: 189
*/
#define FM_RENAME_TGT_ISDIR_ERR_EID (FM_RENAME_TGT_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/** \brief <tt> 'Rename File error: filename exists as an open file: name = \%s' </tt>
**  \event <tt> 'Rename File error: filename exists as an open file: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Rename
**  command packet with a target filename that is open.
**
**  Value: 190
*/
#define FM_RENAME_TGT_ISOPEN_ERR_EID (FM_RENAME_TGT_BASE_EID + FM_FNAME_ISOPEN_EID_OFFSET)

/** \brief <tt> 'Rename File error: filename has unknown state: name = \%s, state = \%d' </tt>
**  \event <tt> 'Rename File error: filename has unknown state: name = \%s, state = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Rename
**  command packet with a target file with unknown state.
**
**  Value: 192
*/
#define FM_RENAME_TGT_UNKNOWN_ERR_EID (FM_RENAME_TGT_BASE_EID + FM_FNAME_UNKNOWN_EID_OFFSET)

/** \brief <tt> 'Rename File Error: Child Task' </tt>
**  \event <tt> 'Rename File error: child task is disabled' </tt>
**  \event <tt> 'Rename File error: child task queue is full' </tt>
**  \event <tt> 'Rename File error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This is the base for any of several messages that are  generated when
**  the FM child task command queue interface cannot be used.
**
**  Value: 193
*/
#define FM_RENAME_CHILD_BASE_EID (FM_RENAME_TGT_BASE_EID + FM_FNAME_NUM_OFFSETS)

/** \brief <tt> 'Rename File Error: child task is disabled' </tt>
**  \event <tt> 'Rename File error: child task is disabled' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task is disabled.
**
**  Value: 193
*/
#define FM_RENAME_CHILD_DISABLED_ERR_EID (FM_RENAME_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/** \brief <tt> 'Rename File Error: child task queue is full' </tt>
**  \event <tt> 'Rename File error: child task queue is full' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task comand queue is full.
**
**  If the child task command queue is full, the problem may be temporary,
**  caused by sending too many FM commands too quickly.  If the command
**  queue does not empty itself within a reasonable amount of time then
**  the child task may be hung. It may be possible to use CFE commands to
**  terminate the child task, which should then cause FM to process all
**  commands in the main task.
**
**  Value: 194
*/
#define FM_RENAME_CHILD_FULL_ERR_EID (FM_RENAME_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/** \brief <tt> 'Rename File Error: child task interface is broken: count = \%d, index = \%d' </tt>
**  \event <tt> 'Rename File error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the interface between the main task
**  and child task is broken.
**
**  If the child task queue is broken then either the handshake interface
**  logic is flawed, or there has been some sort of data corruption that
**  affected the interface control variables.  In either case, it may be
**  necessary to restart the FM application to resync the interface.
**
**  Value: 195
**
*/
#define FM_RENAME_CHILD_BROKEN_ERR_EID (FM_RENAME_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/** \brief <tt> 'Delete File Error: Source Filename' </tt>
**  \event <tt> 'Delete File error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Delete File error: file does not exist: name = \%s' </tt>
**  \event <tt> 'Delete File error: filename is a directory: name = \%s' </tt>
**  \event <tt> 'Delete File error: file is already open: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Delete
**  command packet with a source filename that is unusable for one
**  of several reasons.
**
**  Value: 196
*/
#define FM_DELETE_SRC_BASE_EID (FM_RENAME_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/** \brief <tt> 'Delete File Error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Delete File error: filename is invalid: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Delete
**  command packet with an invalid source filename.
**
**  Value: 196
*/
#define FM_DELETE_SRC_INVALID_ERR_EID (FM_DELETE_SRC_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/** \brief <tt> 'Delete File Error: file does not exist: name = \%s' </tt>
**  \event <tt> 'Delete File error: file does not exist: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Delete
**  command packet with a source filename that does not exist.
**
**  Value: 197
*/
#define FM_DELETE_SRC_DNE_ERR_EID (FM_DELETE_SRC_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/** \brief <tt> 'Delete File Error: filename is a directory: name = \%s'  </tt>
**  \event <tt> 'Delete File error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Delete
**  command packet with a source filename that is a directory.
**
**  Value: 198
*/
#define FM_DELETE_SRC_ISDIR_ERR_EID (FM_DELETE_SRC_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/** \brief <tt> 'Delete File Error: file is already open: name = \%s'  </tt>
**  \event <tt> 'Delete File error: file is already open: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Delete
**  command packet with a source filename that is already open.
**
**  Value: 199
*/
#define FM_DELETE_SRC_OPEN_ERR_EID (FM_DELETE_SRC_BASE_EID + FM_FNAME_ISOPEN_EID_OFFSET)

/** \brief <tt> 'Delete File Error: Child Task' </tt>
**  \event <tt> 'Delete File error: child task is disabled' </tt>
**  \event <tt> 'Delete File error: child task queue is full' </tt>
**  \event <tt> 'Delete File error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This is the base for any of several messages that are  generated when
**  the FM child task command queue interface cannot be used.
**
**  Value: 202
*/
#define FM_DELETE_CHILD_BASE_EID (FM_DELETE_SRC_BASE_EID + FM_FNAME_NUM_OFFSETS)

/** \brief <tt> 'Delete File Error: child task is disabled' </tt>
**  \event <tt> 'Delete File error: child task is disabled' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task is disabled.
**
**  Value: 202
*/
#define FM_DELETE_CHILD_DISABLED_ERR_EID (FM_DELETE_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/** \brief <tt> 'Delete File Error: child task queue is full' </tt>
**  \event <tt> 'Delete File error: child task queue is full' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task comand queue is full.
**
**  If the child task command queue is full, the problem may be temporary,
**  caused by sending too many FM commands too quickly.  If the command
**  queue does not empty itself within a reasonable amount of time then
**  the child task may be hung. It may be possible to use CFE commands to
**  terminate the child task, which should then cause FM to process all
**  commands in the main task.
**
**  Value: 203
*/
#define FM_DELETE_CHILD_FULL_ERR_EID (FM_DELETE_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/** \brief <tt> 'Delete File Error: child task interface is broken: count = \%d, index = \%d' </tt>
**  \event <tt> 'Delete File error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the interface between the main task
**  and child task is broken.
**
**  If the child task queue is broken then either the handshake interface
**  logic is flawed, or there has been some sort of data corruption that
**  affected the interface control variables.  In either case, it may be
**  necessary to restart the FM application to resync the interface.
**
**  Value: 204
**
*/
#define FM_DELETE_CHILD_BROKEN_ERR_EID (FM_DELETE_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/** \brief <tt> 'Delete All Files Error: Directory name' </tt>
**  \event <tt> 'Delete All Files error: directory name is invalid: name = \%s' </tt>
**  \event <tt> 'Delete All Files error: directory does not exist: name = \%s' </tt>
**  \event <tt> 'Delete All Files error: directory name exists as a file: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_DeleteAll
**  command packet with a directory name that is unusable for one
**  of several reasons.
**
**  Value: 205
*/
#define FM_DELETE_ALL_SRC_BASE_EID (FM_DELETE_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/** \brief <tt> 'Delete File Error: directory name is invalid: name = \%s' </tt>
**  \event <tt> 'Delete File error: directory name is invalid: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Delete
**  command packet with an invalid source filename.
**
**  Value: 205
*/
#define FM_DELETE_ALL_SRC_INVALID_ERR_EID (FM_DELETE_ALL_SRC_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/** \brief <tt> 'Delete File Error: directory does not exist: name = \%s' </tt>
**  \event <tt> 'Delete File error: directory does not exist: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_DeleteAll
**  command packet with a directory name that does not exist.
**
**  Value: 206
*/
#define FM_DELETE_ALL_SRC_DNE_ERR_EID (FM_DELETE_ALL_SRC_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/** \brief <tt> 'Delete File Error: directory name exists as a file: name = \%s'  </tt>
**  \event <tt> 'Delete File error: directory name exists as a file: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Delete
**  command packet with a directory name that is a file.
**
**  Value: 207
*/
#define FM_DELETE_ALL_SRC_FILE_ERR_EID (FM_DELETE_ALL_SRC_BASE_EID + FM_FNAME_ISFILE_EID_OFFSET)

/** \brief <tt> 'Delete All Files Error: Child Task' </tt>
**  \event <tt> 'Delete All Files error: child task is disabled' </tt>
**  \event <tt> 'Delete All Files error: child task queue is full' </tt>
**  \event <tt> 'Delete All Files error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This is the base for any of several messages that are  generated when
**  the FM child task command queue interface cannot be used.
**
**  Value: 211
*/
#define FM_DELETE_ALL_CHILD_BASE_EID (FM_DELETE_ALL_SRC_BASE_EID + FM_FNAME_NUM_OFFSETS)

/** \brief <tt> 'Delete All Files Error: child task is disabled' </tt>
**  \event <tt> 'Delete All Files error: child task is disabled' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task is disabled.
**
**  Value: 211
*/
#define FM_DELETE_ALL_CHILD_DISABLED_ERR_EID (FM_DELETE_ALL_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/** \brief <tt> 'Delete All Files Error: child task queue is full' </tt>
**  \event <tt> 'Delete All Files error: child task queue is full' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task comand queue is full.
**
**  If the child task command queue is full, the problem may be temporary,
**  caused by sending too many FM commands too quickly.  If the command
**  queue does not empty itself within a reasonable amount of time then
**  the child task may be hung. It may be possible to use CFE commands to
**  terminate the child task, which should then cause FM to process all
**  commands in the main task.
**
**  Value: 212
*/
#define FM_DELETE_ALL_CHILD_FULL_ERR_EID (FM_DELETE_ALL_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/** \brief <tt> 'Delete All Files Error: child task interface is broken: count = \%d, index = \%d' </tt>
**  \event <tt> 'Delete All Files error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the interface between the main task
**  and child task is broken.
**
**  If the child task queue is broken then either the handshake interface
**  logic is flawed, or there has been some sort of data corruption that
**  affected the interface control variables.  In either case, it may be
**  necessary to restart the FM application to resync the interface.
**
**  Value: 213
**
*/
#define FM_DELETE_ALL_CHILD_BROKEN_ERR_EID (FM_DELETE_ALL_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/** \brief <tt> 'Decompress File Error: Source Filename' </tt>
**  \event <tt> 'Decompress File error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Decompress File error: file does not exist: name = \%s' </tt>
**  \event <tt> 'Decompress File error: filename is a directory: name = \%s' </tt>
**  \event <tt> 'Decompress File error: file is already open: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Decompress
**  command packet with a source filename that is unusable for one
**  of several reasons.
**
**  Value: 214
*/
#define FM_DECOM_SRC_BASE_EID (FM_DELETE_ALL_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/** \brief <tt> 'Decompress File Error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Decompress File error: filename is invalid: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Decompress
**  command packet with an invalid source filename.
**
**  Value: 214
*/
#define FM_DECOM_SRC_INVALID_ERR_EID (FM_DECOM_SRC_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/** \brief <tt> 'Decompress File Error: file does not exist: name = \%s' </tt>
**  \event <tt> 'Decompress File error: file does not exist: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Decompress
**  command packet with a source filename that does not exist.
**
**  Value: 215
*/
#define FM_DECOM_SRC_DNE_ERR_EID (FM_DECOM_SRC_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/** \brief <tt> 'Decompress File Error: filename is a directory: name = \%s'  </tt>
**  \event <tt> 'Decompress File error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Decompress
**  command packet with a source filename that is a directory.
**
**  Value: 216
*/
#define FM_DECOM_SRC_ISDIR_ERR_EID (FM_DECOM_SRC_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/** \brief <tt> 'Decompress File Error: file is already open: name = \%s'  </tt>
**  \event <tt> 'Decompress File error: file is already open: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Decompress
**  command packet with a source filename that is already open.
**
**  Value: 217
*/
#define FM_DECOM_SRC_OPEN_ERR_EID (FM_DECOM_SRC_BASE_EID + FM_FNAME_ISOPEN_EID_OFFSET)

/** \brief <tt> 'Decompress File Error: Target Filename' </tt>
**  \event <tt> 'Decompress File error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Decompress File error: file does not exist: name = \%s' </tt>
**  \event <tt> 'Decompress File error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Decompress
**  command packet with a target filename that is unusable for one
**  of several reasons.
**
**  Value: 220
*/
#define FM_DECOM_TGT_BASE_EID (FM_DECOM_SRC_BASE_EID + FM_FNAME_NUM_OFFSETS)

/** \brief <tt> 'Decompress File Error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Decompress File error: filename is invalid: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Decompress
**  command packet with an invalid target filename.
**
**  Value: 220
*/
#define FM_DECOM_TGT_INVALID_ERR_EID (FM_DECOM_TGT_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/** \brief <tt> 'Decompress File Error: file already exists: name = \%s' </tt>
**  \event <tt> 'Decompress File error: file already exists: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Decompress
**  command packet with a target filename that already exists.
**
**  Value: 221
*/
#define FM_DECOM_TGT_EXIST_ERR_EID (FM_DECOM_TGT_BASE_EID + FM_FNAME_EXIST_EID_OFFSET)

/** \brief <tt> 'Decompress File Error: filename is a directory: name = \%s'  </tt>
**  \event <tt> 'Decompress File error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Decompress
**  command packet with a target filename that is a directory.
**
**  Value: 222
*/
#define FM_DECOM_TGT_ISDIR_ERR_EID (FM_DECOM_TGT_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/** \brief <tt> 'Decompress File Error: Child Task' </tt>
**  \event <tt> 'Decompress File error: child task is disabled' </tt>
**  \event <tt> 'Decompress File error: child task queue is full' </tt>
**  \event <tt> 'Decompress File error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This is the base for any of several messages that are  generated when
**  the FM child task command queue interface cannot be used.
**
**  Value: 226
*/
#define FM_DECOM_CHILD_BASE_EID (FM_DECOM_TGT_BASE_EID + FM_FNAME_NUM_OFFSETS)

/** \brief <tt> 'Decompress File Error: child task is disabled' </tt>
**  \event <tt> 'Decompress File error: child task is disabled' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task is disabled.
**
**  Value: 226
*/
#define FM_DECOM_CHILD_DISABLED_ERR_EID (FM_DECOM_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/** \brief <tt> 'Decompress File Error: child task queue is full' </tt>
**  \event <tt> 'Decompress File error: child task queue is full' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task comand queue is full.
**
**  If the child task command queue is full, the problem may be temporary,
**  caused by sending too many FM commands too quickly.  If the command
**  queue does not empty itself within a reasonable amount of time then
**  the child task may be hung. It may be possible to use CFE commands to
**  terminate the child task, which should then cause FM to process all
**  commands in the main task.
**
**  Value: 227
*/
#define FM_DECOM_CHILD_FULL_ERR_EID (FM_DECOM_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/** \brief <tt> 'Decompress File Error: child task interface is broken: count = \%d, index = \%d' </tt>
**  \event <tt> 'Decompress File error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the interface between the main task
**  and child task is broken.
**
**  If the child task queue is broken then either the handshake interface
**  logic is flawed, or there has been some sort of data corruption that
**  affected the interface control variables.  In either case, it may be
**  necessary to restart the FM application to resync the interface.
**
**  Value: 228
**
*/
#define FM_DECOM_CHILD_BROKEN_ERR_EID (FM_DECOM_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/** \brief <tt> 'Concat Files Error: Source 1 Filename' </tt>
**  \event <tt> 'Concat Files error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Concat Files error: file does not exist: name = \%s' </tt>
**  \event <tt> 'Concat Files error: filename is a directory: name = \%s' </tt>
**  \event <tt> 'Concat Files error: file is already open: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Concat
**  command packet with a source 1 filename that is unusable for one
**  of several reasons.
**
**  Value: 229
*/
#define FM_CONCAT_SRC1_BASE_EID (FM_DECOM_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/** \brief <tt> 'Concat Files Error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Concat Files error: filename is invalid: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Concat
**  command packet with an invalid source 1 filename.
**
**  Value: 229
*/
#define FM_CONCAT_SRC1_INVALID_ERR_EID (FM_CONCAT_SRC1_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/** \brief <tt> 'Concat Files Error: file does not exist: name = \%s' </tt>
**  \event <tt> 'Concat Files error: file does not exist: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Concat
**  command packet with a source 1 filename that does not exist.
**
**  Value: 230
*/
#define FM_CONCAT_SRC1_DNE_ERR_EID (FM_CONCAT_SRC1_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/** \brief <tt> 'Concat Files Error: filename is a directory: name = \%s'  </tt>
**  \event <tt> 'Concat Files error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Concat
**  command packet with a source filename that is a directory.
**
**  Value: 231
*/
#define FM_CONCAT_SRC1_ISDIR_ERR_EID (FM_CONCAT_SRC1_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/** \brief <tt> 'Concat File Error: file is already open: name = \%s'  </tt>
**  \event <tt> 'Concat File error: file is already open: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Concat
**  command packet with a source filename that is already open.
**
**  Value: 232
*/
#define FM_CONCAT_SRC1_OPEN_ERR_EID (FM_CONCAT_SRC1_BASE_EID + FM_FNAME_ISOPEN_EID_OFFSET)

/** \brief <tt> 'Concat Files Error: Source 2 Filename' </tt>
**  \event <tt> 'Concat Files error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Concat Files error: file does not exist: name = \%s' </tt>
**  \event <tt> 'Concat Files error: filename is a directory: name = \%s' </tt>
**  \event <tt> 'Concat Files error: file is already open: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Concat
**  command packet with a source 2 filename that is unusable for one
**  of several reasons.
**
**  Value: 235
*/
#define FM_CONCAT_SRC2_BASE_EID (FM_CONCAT_SRC1_BASE_EID + FM_FNAME_NUM_OFFSETS)

/** \brief <tt> 'Concat Files Error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Concat Files error: filename is invalid: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Concat
**  command packet with an invalid source 2 filename.
**
**  Value: 235
*/
#define FM_CONCAT_SRC2_INVALID_ERR_EID (FM_CONCAT_SRC2_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/** \brief <tt> 'Concat Files Error: file does not exist: name = \%s' </tt>
**  \event <tt> 'Concat Files error: file does not exist: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Concat
**  command packet with a source 2 filename that does not exist.
**
**  Value: 236
*/
#define FM_CONCAT_SRC2_DNE_ERR_EID (FM_CONCAT_SRC2_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/** \brief <tt> 'Concat Files Error: filename is a directory: name = \%s'  </tt>
**  \event <tt> 'Concat Files error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Concat
**  command packet with a source filename that is a directory.
**
**  Value: 237
*/
#define FM_CONCAT_SRC2_ISDIR_ERR_EID (FM_CONCAT_SRC2_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/** \brief <tt> 'Concat File Error: file is already open: name = \%s'  </tt>
**  \event <tt> 'Concat File error: file is already open: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Concat
**  command packet with a source filename that is already open.
**
**  Value: 238
*/
#define FM_CONCAT_SRC2_OPEN_ERR_EID (FM_CONCAT_SRC2_BASE_EID + FM_FNAME_ISOPEN_EID_OFFSET)

/** \brief <tt> 'Concat Files Error: Target Filename' </tt>
**  \event <tt> 'Concat Files error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Concat Files error: file already exists: name = \%s' </tt>
**  \event <tt> 'Concat Files error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Concat
**  command packet with a target filename that is unusable for one
**  of several reasons.
**
**  Value: 241
*/
#define FM_CONCAT_TGT_BASE_EID (FM_CONCAT_SRC2_BASE_EID + FM_FNAME_NUM_OFFSETS)

/** \brief <tt> 'Concat File Error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Concat File error: filename is invalid: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Concat
**  command packet with an invalid target filename.
**
**  Value: 241
*/
#define FM_CONCAT_TGT_INVALID_ERR_EID (FM_CONCAT_TGT_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/** \brief <tt> 'Concat Files Error: file already exists: name = \%s' </tt>
**  \event <tt> 'Concat Files error: file already exists: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Concat
**  command packet with a target filename that already exists.
**
**  Value: 242
*/
#define FM_CONCAT_TGT_EXIST_ERR_EID (FM_CONCAT_TGT_BASE_EID + FM_FNAME_EXIST_EID_OFFSET)

/** \brief <tt> 'Concat File Error: filename is a directory: name = \%s'  </tt>
**  \event <tt> 'Concat File error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Concat
**  command packet with a target filename that is a directory.
**
**  Value: 243
*/
#define FM_CONCAT_TGT_ISDIR_ERR_EID (FM_CONCAT_TGT_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/** \brief <tt> 'Concat Files Error: Child Task' </tt>
**  \event <tt> 'Concat Files error: child task is disabled' </tt>
**  \event <tt> 'Concat Files error: child task queue is full' </tt>
**  \event <tt> 'Concat Files error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This is the base for any of several messages that are generated when
**  the FM child task command queue interface cannot be used.
**
**  Value: 247
*/
#define FM_CONCAT_CHILD_BASE_EID (FM_CONCAT_TGT_BASE_EID + FM_FNAME_NUM_OFFSETS)

/** \brief <tt> 'Concat Files Error: child task is disabled' </tt>
**  \event <tt> 'Concat Files error: child task is disabled' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task is disabled.
**
**  Value: 247
*/
#define FM_CONCAT_CHILD_DISABLED_ERR_EID (FM_CONCAT_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/** \brief <tt> 'Concat Files Error: child task queue is full' </tt>
**  \event <tt> 'Concat Files error: child task queue is full' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task comand queue is full.
**
**  If the child task command queue is full, the problem may be temporary,
**  caused by sending too many FM commands too quickly.  If the command
**  queue does not empty itself within a reasonable amount of time then
**  the child task may be hung. It may be possible to use CFE commands to
**  terminate the child task, which should then cause FM to process all
**  commands in the main task.
**
**  Value: 248
*/
#define FM_CONCAT_CHILD_FULL_ERR_EID (FM_CONCAT_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/** \brief <tt> 'Concat File Error: child task interface is broken: count = \%d, index = \%d' </tt>
**  \event <tt> 'Concat File error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the interface between the main task
**  and child task is broken.
**
**  If the child task queue is broken then either the handshake interface
**  logic is flawed, or there has been some sort of data corruption that
**  affected the interface control variables.  In either case, it may be
**  necessary to restart the FM application to resync the interface.
**
**  Value: 249
**
*/
#define FM_CONCAT_CHILD_BROKEN_ERR_EID (FM_CONCAT_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/** \brief <tt> 'Get File Info Error: Child Task' </tt>
**  \event <tt> 'Get File Info error: child task is disabled' </tt>
**  \event <tt> 'Get File Info error: child task queue is full' </tt>
**  \event <tt> 'Get File Info error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This is the base for any of several messages that are generated when
**  the FM child task command queue interface cannot be used.
**
**  Value: 250
*/
#define FM_FILE_INFO_CHILD_BASE_EID (FM_CONCAT_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/** \brief <tt> 'Get File Info Error: child task is disabled' </tt>
**  \event <tt> 'Get File Info error: child task is disabled' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task is disabled.
**
**  Value: 250
*/
#define FM_FILE_INFO_CHILD_DISABLED_ERR_EID (FM_FILE_INFO_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/** \brief <tt> 'Get File Info Error: child task queue is full' </tt>
**  \event <tt> 'Get File Info error: child task queue is full' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task comand queue is full.
**
**  If the child task command queue is full, the problem may be temporary,
**  caused by sending too many FM commands too quickly.  If the command
**  queue does not empty itself within a reasonable amount of time then
**  the child task may be hung. It may be possible to use CFE commands to
**  terminate the child task, which should then cause FM to process all
**  commands in the main task.
**
**  Value: 251
*/
#define FM_FILE_INFO_CHILD_FULL_ERR_EID (FM_FILE_INFO_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/** \brief <tt> 'Get File Info Error: child task interface is broken: count = \%d, index = \%d' </tt>
**  \event <tt> 'Get File Info error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the interface between the main task
**  and child task is broken.
**
**  If the child task queue is broken then either the handshake interface
**  logic is flawed, or there has been some sort of data corruption that
**  affected the interface control variables.  In either case, it may be
**  necessary to restart the FM application to resync the interface.
**
**  Value: 252
**
*/
#define FM_FILE_INFO_CHILD_BROKEN_ERR_EID (FM_FILE_INFO_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/** \brief <tt> 'Create Directory Error: Directory Name' </tt>
**  \event <tt> 'Create Directory error: directory name is invalid: name = \%s' </tt>
**  \event <tt> 'Create Directory error: directory name exists as a file: name = \%s' </tt>
**  \event <tt> 'Create Directory error: directory already exists: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Create
**  command packet with a directory name that is unusable for one
**  of several reasons.
**
**  Value: 253
*/
#define FM_CREATE_DIR_SRC_BASE_EID (FM_FILE_INFO_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/** \brief <tt> 'Create Directory error: directory name is invalid: name = \%s' </tt>
**  \event <tt> 'Create Directory error: directory name is invalid: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Create
**  command packet with an invalid directory name.
**
**  Value: 253
*/
#define FM_CREATE_DIR_SRC_INVALID_ERR_EID (FM_CREATE_DIR_SRC_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/** \brief <tt> 'Create Directory error: directory name exists as a file: name = \%s' </tt>
**  \event <tt> 'Create Directory error: directory name exists as a file: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Create
**  command packet with a directory name that exists as a file.
**
**  Value: 254
*/
#define FM_CREATE_DIR_SRC_DNE_ERR_EID (FM_CREATE_DIR_SRC_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/** \brief <tt> 'Create Directory Error: directory already exists: name = \%s'  </tt>
**  \event <tt> 'Create Directory error: directory already exists: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_Create
**  command pasket with a directory that already exists.
**
**  Value: 255
*/
#define FM_CREATE_DIR_SRC_ISDIR_ERR_EID (FM_CREATE_DIR_SRC_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/** \brief <tt> 'Create Directory Error: Child Task' </tt>
**  \event <tt> 'Create Directory error: child task is disabled' </tt>
**  \event <tt> 'Create Directory error: child task queue is full' </tt>
**  \event <tt> 'Create Directory error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This is the base for any of several messages that are generated when
**  the FM child task command queue interface cannot be used.
**
**  Value: 259
*/
#define FM_CREATE_DIR_CHILD_BASE_EID (FM_CREATE_DIR_SRC_BASE_EID + FM_FNAME_NUM_OFFSETS)

/** \brief <tt> 'Create Directory error: child task is disabled' </tt>
**  \event <tt> 'Create Directory error: child task is disabled' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task is disabled.
**
**  Value: 259
*/
#define FM_CREATE_DIR_CHILD_DISABLED_ERR_EID (FM_CREATE_DIR_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/** \brief <tt> 'Create Directory Error: child task queue is full' </tt>
**  \event <tt> 'Create Directory error: child task queue is full' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task comand queue is full.
**
**  If the child task command queue is full, the problem may be temporary,
**  caused by sending too many FM commands too quickly.  If the command
**  queue does not empty itself within a reasonable amount of time then
**  the child task may be hung. It may be possible to use CFE commands to
**  terminate the child task, which should then cause FM to process all
**  commands in the main task.
**
**  Value: 260
*/
#define FM_CREATE_DIR_CHILD_FULL_ERR_EID (FM_CREATE_DIR_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/** \brief <tt> 'Create Directory Error: child task interface is broken: count = \%d, index = \%d' </tt>
**  \event <tt> 'Create Directory error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the interface between the main task
**  and child task is broken.
**
**  If the child task queue is broken then either the handshake interface
**  logic is flawed, or there has been some sort of data corruption that
**  affected the interface control variables.  In either case, it may be
**  necessary to restart the FM application to resync the interface.
**
**  Value: 261
**
*/
#define FM_CREATE_DIR_CHILD_BROKEN_ERR_EID (FM_CREATE_DIR_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/** \brief <tt> 'Delete Directory Error: Directory Name' </tt>
**  \event <tt> 'Delete Directory error: directory name is invalid: name = \%s' </tt>
**  \event <tt> 'Delete Directory error: directory name exists as a file: name = \%s' </tt>
**  \event <tt> 'Delete Directory error: directory already exists: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_DeleteDir
**  command packet with a directory name that is unusable for one
**  of several reasons.
**
**  Value: 262
*/
#define FM_DELETE_DIR_SRC_BASE_EID (FM_CREATE_DIR_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/** \brief <tt> 'Delete Directory error: directory name is invalid: name = \%s' </tt>
**  \event <tt> 'Delete Directory error: directory name is invalid: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_DeleteDir
**  command packet with an invalid directory name.
**
**  Value: 262
*/
#define FM_DELETE_DIR_SRC_INVALID_ERR_EID (FM_DELETE_DIR_SRC_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/** \brief <tt> 'Delete Directory error: directory name exists as a file: name = \%s' </tt>
**  \event <tt> 'Delete Directory error: directory name exists as a file: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_DeleteDir
**  command packet with a directory name that exists as a file.
**
**  Value: 263
*/
#define FM_DELETE_DIR_SRC_DNE_ERR_EID (FM_DELETE_DIR_SRC_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/** \brief <tt> 'Delete Directory Error: directory already exists: name = \%s'  </tt>
**  \event <tt> 'Delete Directory error: directory already exists: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_DeleteDir
**  command pasket with a directory that already exists.
**
**  Value: 264
*/
#define FM_DELETE_DIR_SRC_ISDIR_ERR_EID (FM_DELETE_DIR_SRC_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/** \brief <tt> 'Delete Directory Error: Child Task' </tt>
**  \event <tt> 'Delete Directory error: child task is disabled' </tt>
**  \event <tt> 'Delete Directory error: child task queue is full' </tt>
**  \event <tt> 'Delete Directory error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This is the base for any of several messages that are generated when
**  the FM child task command queue interface cannot be used.
**
**  Value: 268
*/
#define FM_DELETE_DIR_CHILD_BASE_EID (FM_DELETE_DIR_SRC_BASE_EID + FM_FNAME_NUM_OFFSETS)

/** \brief <tt> 'Delete Directory error: child task is disabled' </tt>
**  \event <tt> 'Delete Directory error: child task is disabled' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task is disabled.
**
**  Value: 268
*/
#define FM_DELETE_DIR_CHILD_DISABLED_ERR_EID (FM_DELETE_DIR_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/** \brief <tt> 'Delete Directory Error: child task queue is full' </tt>
**  \event <tt> 'Delete Directory error: child task queue is full' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task comand queue is full.
**
**  If the child task command queue is full, the problem may be temporary,
**  caused by sending too many FM commands too quickly.  If the command
**  queue does not empty itself within a reasonable amount of time then
**  the child task may be hung. It may be possible to use CFE commands to
**  terminate the child task, which should then cause FM to process all
**  commands in the main task.
**
**  Value: 269
*/
#define FM_DELETE_DIR_CHILD_FULL_ERR_EID (FM_DELETE_DIR_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/** \brief <tt> 'Delete Directory Error: child task interface is broken: count = \%d, index = \%d' </tt>
**  \event <tt> 'Delete Directory error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the interface between the main task
**  and child task is broken.
**
**  If the child task queue is broken then either the handshake interface
**  logic is flawed, or there has been some sort of data corruption that
**  affected the interface control variables.  In either case, it may be
**  necessary to restart the FM application to resync the interface.
**
**  Value: 270
**
*/
#define FM_DELETE_DIR_CHILD_BROKEN_ERR_EID (FM_DELETE_DIR_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/** \brief <tt> 'Directory List to File Error: Source Filename' </tt>
**  \event <tt> 'Directory List to File error: directory name is invalid: name = \%s' </tt>
**  \event <tt> 'Directory List to File error: directory does not exist: name = \%s' </tt>
**  \event <tt> 'Directory List to File error: directory name is a file: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_GetDirFile
**  command packet with a source directory name that is unusable for one
**  of several reasons.
**
**  Value: 271
*/
#define FM_GET_DIR_FILE_SRC_BASE_EID (FM_DELETE_DIR_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/** \brief <tt> 'Directory List to File Error: directory name is invalid: name = \%s' </tt>
**  \event <tt> 'Directory List to File error: directory name is invalid: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_GetDirFile
**  command packet with an invalid source directory name.
**
**  Value: 271
*/
#define FM_GET_DIR_FILE_SRC_INVALID_ERR_EID (FM_GET_DIR_FILE_SRC_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/** \brief <tt> 'Directory List to File Error: directory does not exist: name = \%s' </tt>
**  \event <tt> 'Directory List to File error: directory does not exist: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_GetDirFile
**  command packet with a source directory name that does not exist.
**
**  Value: 272
*/
#define FM_GET_DIR_FILE_SRC_DNE_ERR_EID (FM_GET_DIR_FILE_SRC_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/** \brief <tt> 'Directory List to File Error: directory name is a file: name = \%s'  </tt>
**  \event <tt> 'Directory List to File error: directory name is a file: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_GetDirFile
**  command packet with a source directory name that is a file.
**
**  Value: 273
*/
#define FM_GET_DIR_FILE_SRC_ISDIR_ERR_EID (FM_GET_DIR_FILE_SRC_BASE_EID + FM_FNAME_ISFILE_EID_OFFSET)

/** \brief <tt> 'Directory List to File Error: Output Filename' </tt>
**  \event <tt> 'Directory List to File error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Directory List to File error: file does not exist: name = \%s' </tt>
**  \event <tt> 'Directory List to File error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_GetDirFile
**  command packet with a target filename that is unusable for one
**  of several reasons.
**
**  Value: 277
*/
#define FM_GET_DIR_FILE_TGT_BASE_EID (FM_GET_DIR_FILE_SRC_BASE_EID + FM_FNAME_NUM_OFFSETS)

/** \brief <tt> 'Directory List to File Error: filename is invalid: name = \%s' </tt>
**  \event <tt> 'Directory List to File error: filename is invalid: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_GetDirFile
**  command packet with an invalid target directory name.
**
**  Value: 277
*/
#define FM_GET_DIR_FILE_TGT_INVALID_ERR_EID (FM_GET_DIR_FILE_TGT_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/** \brief <tt> 'Directory List to File Error: file does not exist: name = \%s' </tt>
**  \event <tt> 'Directory List to File error: file does not exist: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_GetDirFile
**  command packet with a target directory name that does not exist.
**
**  Value: 278
*/
#define FM_GET_DIR_FILE_TGT_DNE_ERR_EID (FM_GET_DIR_FILE_TGT_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/** \brief <tt> 'Directory List to File Error: filename is a directory: name = \%s'  </tt>
**  \event <tt> 'Directory List to File error: filename is a directory: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_GetDirFile
**  command packet with a target filename that is a directory.
**
**  Value: 279
*/
#define FM_GET_DIR_FILE_TGT_ISDIR_ERR_EID (FM_GET_DIR_FILE_TGT_BASE_EID + FM_FNAME_ISDIR_EID_OFFSET)

/** \brief <tt> 'Directory List to File Error: Child Task' </tt>
**  \event <tt> 'Directory List to File error: child task is disabled' </tt>
**  \event <tt> 'Directory List to File error: child task queue is full' </tt>
**  \event <tt> 'Directory List to File error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This is the base for any of several messages that are  generated when
**  the FM child task command queue interface cannot be used.
**
**  Value: 283
*/
#define FM_GET_DIR_FILE_CHILD_BASE_EID (FM_GET_DIR_FILE_TGT_BASE_EID + FM_FNAME_NUM_OFFSETS)

/** \brief <tt> 'Directory List to File Error: child task is disabled' </tt>
**  \event <tt> 'Directory List to File error: child task is disabled' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task is disabled.
**
**  Value: 283
*/
#define FM_GET_DIR_FILE_CHILD_DISABLED_ERR_EID (FM_GET_DIR_FILE_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/** \brief <tt> 'Directory List to File Error: child task queue is full' </tt>
**  \event <tt> 'Directory List to File error: child task queue is full' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task comand queue is full.
**
**  If the child task command queue is full, the problem may be temporary,
**  caused by sending too many FM commands too quickly.  If the command
**  queue does not empty itself within a reasonable amount of time then
**  the child task may be hung. It may be possible to use CFE commands to
**  terminate the child task, which should then cause FM to process all
**  commands in the main task.
**
**  Value: 284
*/
#define FM_GET_DIR_FILE_CHILD_FULL_ERR_EID (FM_GET_DIR_FILE_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/** \brief <tt> 'Directory List to File Error: child task interface is broken: count = \%d, index = \%d' </tt>
**  \event <tt> 'Directory List to File error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the interface between the main task
**  and child task is broken.
**
**  If the child task queue is broken then either the handshake interface
**  logic is flawed, or there has been some sort of data corruption that
**  affected the interface control variables.  In either case, it may be
**  necessary to restart the FM application to resync the interface.
**
**  Value: 285
**
*/
#define FM_GET_DIR_FILE_CHILD_BROKEN_ERR_EID (FM_GET_DIR_FILE_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

/** \brief <tt> 'Directory List to Packet Error: Source Filename' </tt>
**  \event <tt> 'Directory List to Packet error: directory name is invalid: name = \%s' </tt>
**  \event <tt> 'Directory List to Packet error: directory does not exist: name = \%s' </tt>
**  \event <tt> 'Directory List to Packet error: directory name is a file: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_GetDirPkt
**  command packet with a source directory name that is unusable for one
**  of several reasons.
**
**  Value: 286
*/
#define FM_GET_DIR_PKT_SRC_BASE_EID (FM_GET_DIR_FILE_CHILD_BASE_EID + FM_CHILD_NUM_OFFSETS)

/** \brief <tt> 'Directory List to Packet Error: directory name is invalid: name = \%s' </tt>
**  \event <tt> 'Directory List to Packet error: directory name is invalid: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_GetDirPkt
**  command packet with an invalid source directory name.
**
**  Value: 286
*/
#define FM_GET_DIR_PKT_SRC_INVALID_ERR_EID (FM_GET_DIR_PKT_SRC_BASE_EID + FM_FNAME_INVALID_EID_OFFSET)

/** \brief <tt> 'Directory List to Packet Error: directory does not exist: name = \%s' </tt>
**  \event <tt> 'Directory List to Packet error: directory does not exist: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_GetDirPkt
**  command packet with a source directory name that does not exist.
**
**  Value: 287
*/
#define FM_GET_DIR_PKT_SRC_DNE_ERR_EID (FM_GET_DIR_PKT_SRC_BASE_EID + FM_FNAME_DNE_EID_OFFSET)

/** \brief <tt> 'Directory List to Packet Error: directory name is a file: name = \%s'  </tt>
**  \event <tt> 'Directory List to Packet error: directory name is a file: name = \%s' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated upon receipt of a /FM_GetDirPkt
**  command packet with a source directory name that is a file.
**
**  Value: 288
*/
#define FM_GET_DIR_PKT_SRC_ISDIR_ERR_EID (FM_GET_DIR_PKT_SRC_BASE_EID + FM_FNAME_ISFILE_EID_OFFSET)

/** \brief <tt> 'Directory List to Packet Error: Child Task' </tt>
**  \event <tt> 'Directory List to Packet error: child task is disabled' </tt>
**  \event <tt> 'Directory List to Packet error: child task queue is full' </tt>
**  \event <tt> 'Directory List to Packet error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This is the base for any of several messages that are  generated when
**  the FM child task command queue interface cannot be used.
**
**  Value: 292
*/
#define FM_GET_DIR_PKT_CHILD_BASE_EID (FM_GET_DIR_PKT_SRC_BASE_EID + FM_FNAME_NUM_OFFSETS)

/** \brief <tt> 'Directory List to Packet Error: child task is disabled' </tt>
**  \event <tt> 'Directory List to Packet error: child task is disabled' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task is disabled.
**
**  Value: 292
*/
#define FM_GET_DIR_PKT_CHILD_DISABLED_ERR_EID (FM_GET_DIR_PKT_CHILD_BASE_EID + FM_CHILD_DISABLED_EID_OFFSET)

/** \brief <tt> 'Directory List to Packet Error: child task queue is full' </tt>
**  \event <tt> 'Directory List to Packet error: child task queue is full' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the child task comand queue is full.
**
**  If the child task command queue is full, the problem may be temporary,
**  caused by sending too many FM commands too quickly.  If the command
**  queue does not empty itself within a reasonable amount of time then
**  the child task may be hung. It may be possible to use CFE commands to
**  terminate the child task, which should then cause FM to process all
**  commands in the main task.
**
**  Value: 293
*/
#define FM_GET_DIR_PKT_CHILD_FULL_ERR_EID (FM_GET_DIR_PKT_CHILD_BASE_EID + FM_CHILD_Q_FULL_EID_OFFSET)

/** \brief <tt> 'Directory List to Packet Error: child task interface is broken: count = \%d, index = \%d' </tt>
**  \event <tt> 'Directory List to Packet error: child task interface is broken: count = \%d, index = \%d' </tt>
**
**  \par Type: ERROR
**
**  \par Cause
**
**  This event message is generated when the FM child task command queue
**  interface cannot be used because the interface between the main task
**  and child task is broken.
**
**  If the child task queue is broken then either the handshake interface
**  logic is flawed, or there has been some sort of data corruption that
**  affected the interface control variables.  In either case, it may be
**  necessary to restart the FM application to resync the interface.
**
**  Value: 294
**
*/
#define FM_GET_DIR_PKT_CHILD_BROKEN_ERR_EID (FM_GET_DIR_PKT_CHILD_BASE_EID + FM_CHILD_BROKEN_EID_OFFSET)

#endif /* _fm_events_h_ */

/************************/
/*  End of File Comment */
/************************/
