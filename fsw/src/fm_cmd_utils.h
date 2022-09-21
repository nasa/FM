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
 *   Specification for the CFS File Manager command utility
 *   functions.
 */
#ifndef FM_CMD_UTILS_H
#define FM_CMD_UTILS_H

#include "cfe.h"
#include "fm_msg.h"

/************************************************************************
 * Type Definitions
 ************************************************************************/

/**
 *  \brief  FM enum used for verifying file states
 */
typedef enum
{
    FM_FILE_CLOSED,  /**< \brief FM File Is Closed */
    FM_FILE_EXISTS,  /**< \brief FM File Exists */
    FM_FILE_NOEXIST, /**< \brief FM File Does Not Exist */
    FM_FILE_NOTOPEN, /**< \brief FM File Is Not Open */
    FM_DIR_EXISTS,   /**< \brief FM Directory Exists */
    FM_DIR_NOEXIST   /**< \brief FM Directory Does Not Exist */
} FM_File_States;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler utility function prototypes                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

/**
 *  \brief Verify Target File Overwrite Function
 *
 *  \par Description
 *       This function is invoked from the copy and move file command handlers
 *       to verify the target file overwrite argument.  Acceptable values are
 *       TRUE (one) and FALSE (zero).
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Overwrite Value being tested
 *  \param [in]  EventID   Error event ID (command specific)
 *  \param [in]  CmdText   Error event text (command specific)
 *
 *  \return Boolean valid overwrite response
 *  \retval true  Overwrite argument valid
 *  \retval false Overwrite argument invalid
 *
 *  \sa #FM_COPY_CC, #FM_MOVE_CC
 */
bool FM_VerifyOverwrite(uint16 Overwrite, uint32 EventID, const char *CmdText);

/**
 *  \brief Get Open Files Data Function
 *
 *  \par Description
 *       This function creates a list of open files
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  OpenFilesData pointer to open files data
 *
 *  \return The number of open files
 *
 *  \sa #OS_FDGetInfo
 */
uint32 FM_GetOpenFilesData(const FM_OpenFilesEntry_t *OpenFilesData);

/**
 *  \brief Get Filename State Function
 *
 *  \par Description
 *       This function performs a series of tests on the input filename to
 *       determine first whether the filename is currently in use by
 *       the file system as a file or directory.
 *       If the caller is the Get File Info command handler, the function
 *       also stores the file size and last modified timestamp.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Filename    Pointer to buffer containing filename
 *  \param [in]  BufferSize  Size of filename character buffer
 *  \param [in]  FileInfoCmd Is this for the Get File Info command?
 *
 *  \return File state
 *  \retval #FM_NAME_IS_INVALID     \copydoc FM_NAME_IS_INVALID
 *  \retval #FM_NAME_IS_NOT_IN_USE  \copydoc FM_NAME_IS_NOT_IN_USE
 *  \retval #FM_NAME_IS_FILE_OPEN   \copydoc FM_NAME_IS_FILE_OPEN
 *  \retval #FM_NAME_IS_FILE_CLOSED \copydoc FM_NAME_IS_FILE_CLOSED
 *  \retval #FM_NAME_IS_DIRECTORY   \copydoc FM_NAME_IS_DIRECTORY
 *
 *  \sa #OS_stat, #OS_FDGetInfo
 */
uint32 FM_GetFilenameState(char *Filename, uint32 BufferSize, bool FileInfoCmd);

/**
 *  \brief Verify Name Function
 *
 *  \par Description
 *       This function calls the Get Filename State function and generates
 *       an error event if the state is invalid.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Name       Pointer to buffer containing name
 *  \param [in]  BufferSize Size of name character buffer
 *  \param [in]  EventID    Error event ID (command specific)
 *  \param [in]  CmdText    Error event text (command specific)
 *
 *  \return File state
 *  \retval #FM_NAME_IS_INVALID     \copydoc FM_NAME_IS_INVALID
 *  \retval #FM_NAME_IS_NOT_IN_USE  \copydoc FM_NAME_IS_NOT_IN_USE
 *  \retval #FM_NAME_IS_FILE_OPEN   \copydoc FM_NAME_IS_FILE_OPEN
 *  \retval #FM_NAME_IS_FILE_CLOSED \copydoc FM_NAME_IS_FILE_CLOSED
 *  \retval #FM_NAME_IS_DIRECTORY   \copydoc FM_NAME_IS_DIRECTORY
 *
 *  \sa #FM_GetFilenameState
 */
uint32 FM_VerifyNameValid(char *Name, uint32 BufferSize, uint32 EventID, const char *CmdText);

/**
 *  \brief Verify File State Function
 *
 *  \par Description
 *       This function calls the Get Filename State function and generates
 *       an error event if the state is anything other than the given state.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  State      State of file to verify
 *  \param [in]  Filename   Pointer to buffer containing filename
 *  \param [in]  BufferSize Size of filename character buffer
 *  \param [in]  EventID    Error event ID (command specific)
 *  \param [in]  CmdText    Error event text (command specific)
 *
 *  \return Boolean file state response
 *  \retval true  File is in the given state
 *  \retval false File is not in the given state
 *
 *  \sa #FM_GetFilenameState
 */
bool FM_VerifyFileState(FM_File_States State, char *Filename, uint32 BufferSize, uint32 EventID, const char *CmdText);

/**
 *  \brief Verify File is Closed Function
 *
 *  \par Description
 *       This function calls the Verify File State function and generates
 *       an error event if the state is anything other than a closed file.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Filename   Pointer to buffer containing filename
 *  \param [in]  BufferSize Size of filename character buffer
 *  \param [in]  EventID    Error event ID (command specific)
 *  \param [in]  CmdText    Error event text (command specific)
 *
 *  \return Boolean file closed response
 *  \retval true  File is closed
 *  \retval false File is not closed
 *
 *  \sa #FM_VerifyFileState
 */
bool FM_VerifyFileClosed(char *Filename, uint32 BufferSize, uint32 EventID, const char *CmdText);

/**
 *  \brief Verify File Exists Function
 *
 *  \par Description
 *       This function calls the Verify File State function and generates
 *       an error event if the state is anything other than an open file or
 *       a closed file.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Filename   Pointer to buffer containing filename
 *  \param [in]  BufferSize Size of filename character buffer
 *  \param [in]  EventID    Error event ID (command specific)
 *  \param [in]  CmdText    Error event text (command specific)
 *
 *  \return Boolean file exists response
 *  \retval true  File exists
 *  \retval false File does not exist
 *
 *  \sa #FM_VerifyFileState
 */
bool FM_VerifyFileExists(char *Filename, uint32 BufferSize, uint32 EventID, const char *CmdText);

/**
 *  \brief Verify File Does Not Exist Function
 *
 *  \par Description
 *       This function calls the Verify File State function and generates
 *       an error event if the state is anything other than the name is
 *       unused the name is not a file and is not a directory.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Filename   Pointer to buffer containing name
 *  \param [in]  BufferSize Size of name character buffer
 *  \param [in]  EventID    Error event ID (command specific)
 *  \param [in]  CmdText    Error event text (command specific)
 *
 *  \return Boolean file does not exist response
 *  \retval true  File does not exist
 *  \retval false File exists
 *
 *  \sa #FM_VerifyFileState
 */
bool FM_VerifyFileNoExist(char *Filename, uint32 BufferSize, uint32 EventID, const char *CmdText);

/**
 *  \brief Verify File Is Not Open Function
 *
 *  \par Description
 *       This function calls the Verify File State function and generates
 *       an error event if the state is a directory or an open file.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Filename   Pointer to buffer containing name
 *  \param [in]  BufferSize Size of name character buffer
 *  \param [in]  EventID    Error event ID (command specific)
 *  \param [in]  CmdText    Error event text (command specific)
 *
 *  \return Boolean file not open or not in use response
 *  \retval true  File is not open or not in use
 *  \retval false File anything other than closed or not in use
 *
 *  \sa #FM_VerifyFileState
 */
bool FM_VerifyFileNotOpen(char *Filename, uint32 BufferSize, uint32 EventID, const char *CmdText);

/**
 *  \brief Verify Directory Exists Function
 *
 *  \par Description
 *       This function calls the Verify File State function and generates
 *       an error event if the state is not an existing directory.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Directory  Pointer to buffer containing directory name
 *  \param [in]  BufferSize Size of directory name character buffer
 *  \param [in]  EventID    Error event ID (command specific)
 *  \param [in]  CmdText    Error event text (command specific)
 *
 *  \return Boolean directory exists response
 *  \retval true  Directory exists
 *  \retval false Directory does not exist
 *
 *  \sa #FM_VerifyFileState
 */
bool FM_VerifyDirExists(char *Directory, uint32 BufferSize, uint32 EventID, const char *CmdText);

/**
 *  \brief Verify Directory Does Not Exist Function
 *
 *  \par Description
 *       This function calls the Verify File State function and generates
 *       an error event if the state is an existing file or directory.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Name       Pointer to buffer containing directory name
 *  \param [in]  BufferSize Size of directory name character buffer
 *  \param [in]  EventID    Error event ID (command specific)
 *  \param [in]  CmdText    Error event text (command specific)
 *
 *  \return Boolean directory does not exist response
 *  \retval true  Directory does not exist
 *  \retval false Directory exists
 *
 *  \sa #FM_VerifyFileState
 */
bool FM_VerifyDirNoExist(char *Name, uint32 BufferSize, uint32 EventID, const char *CmdText);

/**
 *  \brief Verify Child Task Interface Function
 *
 *  \par Description
 *       This function verifies that the child task interface queue is
 *       not full and that the queue index values are within bounds.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  EventID Error event ID (command specific)
 *  \param [in]  CmdText Error event text (command specific)
 *
 *  \return Boolean child task queue available response
 *  \retval true  Child task queue available
 *  \retval false Child task queue not available
 */
bool FM_VerifyChildTask(uint32 EventID, const char *CmdText);

/**
 *  \brief Invoke Child Task Function
 *
 *  \par Description
 *       This function is called after the caller has loaded the next
 *       available entry in the child task queue with the arguments for
 *       the current command.  The function updates the queue access
 *       index and then verifies that the Child Task is operational.
 *       If the Child Task is operational then it is signaled via
 *       handshake semaphore to process the next command from the queue.
 *       If instead, the Child Task is not operational, the Child Task
 *       queue processor function is called directly to process the
 *       current command.  The difference between the two methods is in
 *       which execution thread is active when the command is processed.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \sa #OS_CountSemGive, #FM_ChildProcess
 */
void FM_InvokeChildTask(void);

/**
 *  \brief Append Path Separator Function
 *
 *  \par Description
 *       This function appends a path separator character (slash) to
 *       a directory name in advance of combining the directory name
 *       with directory entry names to create qualified filenames.  The
 *       function will only append the separator character if there is
 *       room in the buffer for another character.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  Directory  Pointer to buffer containing directory name
 *  \param [in]  BufferSize Size of directory name character buffer
 *
 *  \sa #FM_GetFilenameState
 */
void FM_AppendPathSep(char *Directory, uint32 BufferSize);

#endif
