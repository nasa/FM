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
 *   Prototypes for child task functions.
 */
#ifndef FM_CHILD_H
#define FM_CHILD_H

#include "cfe.h"
#include "fm_msg.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task global function prototypes                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 *  \brief Child Task Initialization Function
 *
 *  \par Description
 *       This function is invoked during FM application startup initialization to
 *       create and initialize the FM Child Task.  The purpose for the child task
 *       is to process FM application commands that take too long to execute within
 *       the main task.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 *
 *  \sa #FM_AppInit
 */
CFE_Status_t FM_ChildInit(void);

/**
 *  \brief Child Task Entry Point Function
 *
 *  \par Description
 *       This function is the entry point for the FM application child task.  The
 *       function registers with CFE as a child task, creates the semaphore to
 *       interface with the parent task and calls the child task main loop function.
 *       Should the main loop function return due to a breakdown in the interface
 *       handshake with the parent task, this function will self delete as a child
 *       task with CFE. There is no return from #CFE_ES_DeleteChildTask.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \sa #CFE_ES_DeleteChildTask, #FM_ChildLoop
 */
void FM_ChildTask(void);

/**
 *  \brief Child Task Main Loop Processor Function
 *
 *  \par Description
 *       This function is the main loop for the FM application child task.  The
 *       function waits indefinitely for the parent task to grant the handshake
 *       semaphore, which is the signal that there are fresh command arguments in
 *       the child task handshake queue.  The function will remain in this loop
 *       until the child task is terminated by the CFE, or until a fatal error
 *       occurs which causes the child task to terminate itself.  Fatal errors are
 *       defined as any error returned by #OS_CountSemTake or if the handshake
 *       queue is empty, or if the read index for the handshake queue is invalid.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \sa #FM_ChildQueueEntry_t, #FM_ChildProcess
 */
void FM_ChildLoop(void);

/**
 *  \brief Child Task Command Queue Processor Function
 *
 *  \par Description
 *       This function routes control to the appropriate child task command
 *       handler.  After the command handler has finished, this function then
 *       updates the queue access variables to point to the next queue entry.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \sa #FM_ChildQueueEntry_t, #FM_ChildTask
 */
void FM_ChildProcess(void);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handlers                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 *  \brief Child Task Copy File Command Handler
 *
 *  \par Description
 *       This function is invoked when the FM child task has been granted the child
 *       task handshake semaphore and the child task command queue contains arguments
 *       that signal a copy file command.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in] CmdArgs A pointer to an entry in the child task handshake command
 *       queue which contains the arguments necessary to process this command.
 *
 *  \sa #FM_ChildQueueEntry_t, #FM_CopyFileCmd_t
 */
void FM_ChildCopyCmd(const FM_ChildQueueEntry_t *CmdArgs);

/**
 *  \brief Child Task Move File Command Handler
 *
 *  \par Description
 *       This function is invoked when the FM child task has been granted the child
 *       task handshake semaphore and the child task command queue contains arguments
 *       that signal a move file command.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in] CmdArgs A pointer to an entry in the child task handshake command
 *       queue which contains the arguments necessary to process this command.
 *
 *  \sa #FM_ChildQueueEntry_t, #FM_MoveFileCmd_t
 */
void FM_ChildMoveCmd(const FM_ChildQueueEntry_t *CmdArgs);

/**
 *  \brief Child Task Rename File Command Handler
 *
 *  \par Description
 *       This function is invoked when the FM child task has been granted the child
 *       task handshake semaphore and the child task command queue contains arguments
 *       that signal a rename file command.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in] CmdArgs A pointer to an entry in the child task handshake command
 *       queue which contains the arguments necessary to process this command.
 *
 *  \sa #FM_ChildQueueEntry_t, #FM_RenameFileCmd_t
 */
void FM_ChildRenameCmd(const FM_ChildQueueEntry_t *CmdArgs);

/**
 *  \brief Child Task Delete File Command Handler
 *
 *  \par Description
 *       This function is invoked when the FM child task has been granted the child
 *       task handshake semaphore and the child task command queue contains arguments
 *       that signal a delete file command.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in] CmdArgs A pointer to an entry in the child task handshake command
 *       queue which contains the arguments necessary to process this command.
 *
 *  \sa #FM_ChildQueueEntry_t, #FM_DeleteFileCmd_t
 */
void FM_ChildDeleteCmd(const FM_ChildQueueEntry_t *CmdArgs);

/**
 *  \brief Child Task Delete All Files Command Handler
 *
 *  \par Description
 *       This function is invoked when the FM child task has been granted the child
 *       task handshake semaphore and the child task command queue contains arguments
 *       that signal a delete all files from a directory command.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in] CmdArgs A pointer to an entry in the child task handshake command
 *       queue which contains the arguments necessary to process this command.
 *
 *  \sa #FM_ChildQueueEntry_t, #FM_DeleteAllFilesCmd_t
 */
void FM_ChildDeleteAllFilesCmd(FM_ChildQueueEntry_t *CmdArgs);

/**
 *  \brief Child Task Decompress File Command Handler
 *
 *  \par Description
 *       This function is invoked when the FM child task has been granted the child
 *       task handshake semaphore and the child task command queue contains arguments
 *       that signal a decompress file command.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in] CmdArgs A pointer to an entry in the child task handshake command
 *       queue which contains the arguments necessary to process this command.
 *
 *  \sa #FM_ChildQueueEntry_t, #FM_DecompressFileCmd_t
 */
void FM_ChildDecompressFileCmd(const FM_ChildQueueEntry_t *CmdArgs);

/**
 *  \brief Child Task Concatenate Files Command Handler
 *
 *  \par Description
 *       This function is invoked when the FM child task has been granted the child
 *       task handshake semaphore and the child task command queue contains arguments
 *       that signal a concatenate files command.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in] CmdArgs A pointer to an entry in the child task handshake command
 *       queue which contains the arguments necessary to process this command.
 *
 *  \sa #FM_ChildQueueEntry_t, #FM_ConcatFilesCmd_t
 */
void FM_ChildConcatFilesCmd(const FM_ChildQueueEntry_t *CmdArgs);

/**
 *  \brief Child Task Get File Info Command Handler
 *
 *  \par Description
 *       This function is invoked when the FM child task has been granted the child
 *       task handshake semaphore and the child task command queue contains arguments
 *       that signal a get file info command.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in] CmdArgs A pointer to an entry in the child task handshake command
 *       queue which contains the arguments necessary to process this command.
 *
 *  \sa #FM_ChildQueueEntry_t, #FM_GetFileInfoCmd_t
 */
void FM_ChildFileInfoCmd(FM_ChildQueueEntry_t *CmdArgs);

/**
 *  \brief Child Task Create Directory Command Handler
 *
 *  \par Description
 *       This function is invoked when the FM child task has been granted the child
 *       task handshake semaphore and the child task command queue contains arguments
 *       that signal a create directory command.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in] CmdArgs A pointer to an entry in the child task handshake command
 *       queue which contains the arguments necessary to process this command.
 *
 *  \sa #FM_ChildQueueEntry_t, #FM_CreateDirectoryCmd_t
 */
void FM_ChildCreateDirectoryCmd(const FM_ChildQueueEntry_t *CmdArgs);

/**
 *  \brief Child Task Delete Directory Command Handler
 *
 *  \par Description
 *       This function is invoked when the FM child task has been granted the child
 *       task handshake semaphore and the child task command queue contains arguments
 *       that signal a delete directory command.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in] CmdArgs A pointer to an entry in the child task handshake command
 *       queue which contains the arguments necessary to process this command.
 *
 *  \sa #FM_ChildQueueEntry_t, #FM_DeleteDirectoryCmd_t
 */
void FM_ChildDeleteDirectoryCmd(const FM_ChildQueueEntry_t *CmdArgs);

/**
 *  \brief Child Task Get Dir List to File Command Handler
 *
 *  \par Description
 *       This function is invoked when the FM child task has been granted the child
 *       task handshake semaphore and the child task command queue contains arguments
 *       that signal a get directory listing to a file command.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in] CmdArgs A pointer to an entry in the child task handshake command
 *       queue which contains the arguments necessary to process this command.
 *
 *  \sa #FM_ChildQueueEntry_t, #FM_GetDirListFileCmd_t
 */
void FM_ChildDirListFileCmd(const FM_ChildQueueEntry_t *CmdArgs);

/**
 *  \brief Child Task Get Dir List to Packet Command Handler
 *
 *  \par Description
 *       This function is invoked when the FM child task has been granted the child
 *       task handshake semaphore and the child task command queue contains arguments
 *       that signal a get directory listing to a packet command.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in] CmdArgs A pointer to an entry in the child task handshake command
 *       queue which contains the arguments necessary to process this command.
 *
 *  \sa #FM_ChildQueueEntry_t, #FM_GetDirListPktCmd_t
 */
void FM_ChildDirListPktCmd(const FM_ChildQueueEntry_t *CmdArgs);

/**
 *  \brief Child Task Set Permissions Command Handler
 *
 *  \par Description
 *       This function is invoked when the FM child task has been granted the child
 *       task handshake semaphore and the child task command queue contains arguments
 *       that signal a set permissions / mode on the source1 file or directory
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in] CmdArgs A pointer to an entry in the child task handshake command
 *       queue which contains the arguments necessary to process this command.
 *
 *  \sa #FM_ChildQueueEntry_t, #FM_SetPermissionsCmd_t
 */
void FM_ChildSetPermissionsCmd(const FM_ChildQueueEntry_t *CmdArgs);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task utility functions                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 *  \brief Child Task Get Dir List to File Initialization Function
 *
 *  \par Description
 *       This function creates the output file and then writes both the CFE file header
 *       and a blank copy of the directory list statistics structure to the output file.
 *       At the end of the command, software will re-write the statistics structure,
 *       this time with up to date values.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [out] FileHandlePtr A pointer to a file handle variable which is modified to
 *       contain the newly created output file handle.
 *  \param [in] Directory      A pointer to a buffer containing the directory name.
 *  \param [in] Filename       A pointer to a buffer containing the output filename.
 *
 *  \return Execution status, see \ref CFEReturnCodes and \ref OSReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
bool FM_ChildDirListFileInit(osal_id_t *FileHandlePtr, const char *Directory, const char *Filename);

/**
 *  \brief Child Task Get Dir List to File Loop Processor Function
 *
 *  \par Description
 *       This function reads each directory entry, determines the last modify time
 *       size and mode for each entry, and writes the entry data to the output file.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in] DirId           Directory ID, a handle used to read directory entries.
 *  \param [in] FileHandle      Output file handle.
 *  \param [in] Directory       Pointer to a buffer containing the directory name.
 *  \param [in] DirWithSep      Pointer to directory name with path separator appended.
 *  \param [in] Filename        Pointer to a buffer containing the output filename.
 *  \param [in] GetSizeTimeMode Option to call OS_stat for size, time, mode of files
 */
void FM_ChildDirListFileLoop(osal_id_t DirId, osal_id_t FileHandle, const char *Directory, const char *DirWithSep,
                             const char *Filename, uint8 GetSizeTimeMode);

/**
 *  \brief Child Task File Size Time and Mode Utility Function
 *
 *  \par Description
 *       This function is invoked to query the last modify time, current size and mode (permissions) for
 *       each directory entry when processing either the Get Directory List to File
 *       or Get Directory List to Packet commands.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in] Filename  Pointer to the combined directory and entry names.
 *  \param [out] FileSize Pointer to the number containing the current entry size.
 *  \param [out] FileTime Pointer to the number containing the last modify time.
 *  \param [out] FileMode Pointer to the value containing the mode (permissions).
 *
 *  \return Execution status, see \ref CFEReturnCodes and \ref OSReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
int32 FM_ChildSizeTimeMode(const char *Filename, uint32 *FileSize, uint32 *FileTime, uint32 *FileMode);

/**
 *  \brief Child Task Sleep and Stat Utility Function
 *
 *  \par Description
 *       This function is invoked to query the last modify time, current size and mode (permissions) for
 *       each directory entry when processing either the Get Directory List to File
 *       or Get Directory List to Packet commands.
 *       However it only will sleep if FM_CHILD_STAT_SLEEP_FILECOUNT reaches zero and call FM_ChildSizeTimeMode if
 *       getSizeTimeMode is TRUE, otherwise this function has no effect
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in] Filename        Pointer to the combined directory and entry names.
 *  \param [out] DirListData    Pointer to the data containing the current entry size, last modify time, and mode
 *  \param [out] FilesTillSleep If this is zero the function will sleep for #FM_CHILD_STAT_SLEEP_MS and reset it to
 *                              #FM_CHILD_STAT_SLEEP_FILECOUNT. Otherwise it will subtract 1
 *  \param [in] GetSizeTimeMode Whether this function should call FM_ChildSizeTimeMode
 */
void FM_ChildSleepStat(const char *Filename, FM_DirListEntry_t *DirListData, int32 *FilesTillSleep,
                       bool GetSizeTimeMode);

#endif
