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
 *  File Manager (FM) Command Utility Functions
 *
 *  Provides file manager utility function definitions for
 *  processing file manager commands
 */

#include "cfe.h"
#include "fm_msg.h"
#include "fm_cmd_utils.h"
#include "fm_child.h"
#include "fm_perfids.h"
#include "fm_events.h"

#include <string.h>
#include <ctype.h>

static uint32 OpenFileCount = 0;
static bool   FileIsOpen    = false;

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
/* FM utility function -- verify state is not invalid              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_VerifyOverwrite(uint16 Overwrite, uint32 EventID, const char *CmdText)
{
    bool FunctionResult = true;

    /* Acceptable values are true (one) and false (zero) */
    if ((Overwrite != true) && (Overwrite != false))
    {
        CFE_EVS_SendEvent(EventID, CFE_EVS_EventType_ERROR, "%s error: invalid overwrite = %d", CmdText, Overwrite);

        FunctionResult = false;
    }

    return FunctionResult;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- get open files data                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void LoadOpenFileData(osal_id_t ObjId, void *CallbackArg)
{
    FM_OpenFilesEntry_t *OpenFilesData = (FM_OpenFilesEntry_t *)CallbackArg;
    OS_task_prop_t       TaskInfo;
    OS_file_prop_t       FdProp;

    memset(&FdProp, 0, sizeof(FdProp));

    if (OS_IdentifyObject(ObjId) == OS_OBJECT_TYPE_OS_STREAM)
    {
        if (OpenFilesData != (FM_OpenFilesEntry_t *)NULL)
        {
            if (OS_FDGetInfo(ObjId, &FdProp) == OS_SUCCESS)
            {
                strncpy(OpenFilesData[OpenFileCount].LogicalName, FdProp.Path, OS_MAX_PATH_LEN);

                /* Get the name of the application that opened the file */
                memset(&TaskInfo, 0, sizeof(TaskInfo));

                if (OS_TaskGetInfo(FdProp.User, &TaskInfo) == OS_SUCCESS)
                {
                    strncpy(OpenFilesData[OpenFileCount].AppName, (char *)TaskInfo.name, OS_MAX_API_NAME);
                }
            }
        }

        OpenFileCount++;
    }
}

uint32 FM_GetOpenFilesData(const FM_OpenFilesEntry_t *OpenFilesData)
{
    OpenFileCount = 0;

    OS_ForEachObject(OS_OBJECT_CREATOR_ANY, LoadOpenFileData, (void *)OpenFilesData);

    return OpenFileCount;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- query filename state                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void SearchOpenFileData(osal_id_t ObjId, void *CallbackArg)
{
    char *         Fname = (char *)CallbackArg;
    OS_file_prop_t FdProp;

    memset(&FdProp, 0, sizeof(FdProp));

    if (OS_IdentifyObject(ObjId) == OS_OBJECT_TYPE_OS_STREAM)
    {
        /* Get system info for each file descriptor table entry */
        /* If the FD table entry is valid - then the file is open */
        if (OS_FDGetInfo(ObjId, &FdProp) == OS_SUCCESS)
        {
            if (strcmp(Fname, FdProp.Path) == 0)
            {
                FileIsOpen = true;
            }
        }
    }
}

uint32 FM_GetFilenameState(char *Filename, uint32 BufferSize, bool FileInfoCmd)
{
    os_fstat_t FileStatus;
    uint32     FilenameState   = FM_NAME_IS_INVALID;
    bool       FilenameIsValid = false;
    int32      StringLength;

    memset(&FileStatus, 0, sizeof(FileStatus));

    if (Filename != NULL)
    {
        /* Search Filename for a string terminator */
        for (StringLength = 0; StringLength < BufferSize; StringLength++)
        {
            if (Filename[StringLength] == '\0')
            {
                break;
            }
        }

        /* Verify that Filename is not empty and has a terminator */
        if ((StringLength > 0) && (StringLength < BufferSize))
        {
            FilenameIsValid = true;
        }
    }

    /* If Filename is valid, then determine its state */
    if (FilenameIsValid)
    {
        /* Check to see if Filename is in use */
        if (OS_stat(Filename, &FileStatus) == OS_SUCCESS)
        {
            /* Filename is in use, is it also a directory? */
            if (OS_FILESTAT_ISDIR(FileStatus))
            {
                /* Filename is a directory */
                FilenameState = FM_NAME_IS_DIRECTORY;
            }
            else
            {
                /* Filename is a file, but is it open? */
                FilenameState = FM_NAME_IS_FILE_CLOSED;
                FileIsOpen    = false;

                OS_ForEachObject(OS_OBJECT_CREATOR_ANY, SearchOpenFileData, Filename);

                if (FileIsOpen == true)
                {
                    FilenameState = FM_NAME_IS_FILE_OPEN;
                }
            }

            /* Save the last modify time and file size for File Info commands */
            if (FileInfoCmd)
            {
                FM_GlobalData.FileStatTime = OS_FILESTAT_TIME(FileStatus);
                FM_GlobalData.FileStatSize = OS_FILESTAT_SIZE(FileStatus);
                FM_GlobalData.FileStatMode = OS_FILESTAT_MODE(FileStatus);
            }
        }
        else
        {
            /* Cannot get file stat - therefore does not exist */
            FilenameState = FM_NAME_IS_NOT_IN_USE;

            /* Save the last modify time and file size for File Info commands */
            if (FileInfoCmd)
            {
                FM_GlobalData.FileStatSize = 0;
                FM_GlobalData.FileStatTime = 0;
                FM_GlobalData.FileStatMode = 0;
            }
        }
    }

    return FilenameState;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- verify state is not invalid              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint32 FM_VerifyNameValid(char *Name, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    uint32 FilenameState = FM_NAME_IS_INVALID;

    /* Looking for filename state != FM_NAME_IS_INVALID */
    FilenameState = FM_GetFilenameState(Name, BufferSize, true);

    if (FilenameState == FM_NAME_IS_INVALID)
    {
        /* Insert a terminator in case the invalid string did not have one */
        Name[BufferSize - 1] = '\0';
        CFE_EVS_SendEvent(EventID, CFE_EVS_EventType_ERROR, "%s error: invalid name: name = %s", CmdText, Name);
    }

    return FilenameState;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- verify state of file                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_VerifyFileState(FM_File_States State, char *Filename, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    bool   Result                = false;
    uint32 FilenameState         = FM_NAME_IS_INVALID;
    uint32 ErrorCode             = FM_FNAME_INVALID_EID_OFFSET;
    int32  SpecLen               = OS_MAX_PATH_LEN;
    char   Spec[OS_MAX_PATH_LEN] = "\0";

    /* Get state of the filename */
    FilenameState = FM_GetFilenameState(Filename, BufferSize, false);

    switch (FilenameState)
    {
        case FM_NAME_IS_NOT_IN_USE:
            if (State == FM_FILE_NOEXIST || State == FM_FILE_NOTOPEN || State == FM_DIR_NOEXIST)
            {
                Result = true;
            }
            else
            {
                ErrorCode = FM_FNAME_DNE_EID_OFFSET;
                strncpy(Spec, "%s error: file does not exist: name = %s", SpecLen - 1);
            }
            break;
        case FM_NAME_IS_FILE_OPEN:
            if (State == FM_FILE_EXISTS)
            {
                Result = true;
            }
            else if (State == FM_FILE_NOEXIST)
            {
                ErrorCode = FM_FNAME_EXIST_EID_OFFSET;
                strncpy(Spec, "%s error: file already exists: name = %s", SpecLen - 1);
            }
            else if (State == FM_DIR_EXISTS)
            {
                ErrorCode = FM_FNAME_ISFILE_EID_OFFSET;
                strncpy(Spec, "%s error: directory name exists as a file: name %s", SpecLen - 1);
            }
            else if (State == FM_DIR_NOEXIST)
            {
                ErrorCode = FM_FNAME_DNE_EID_OFFSET;
                strncpy(Spec, "%s error: directory name exists as a file: name = %s", SpecLen - 1);
            }
            else
            {
                ErrorCode = FM_FNAME_ISOPEN_EID_OFFSET;
                strncpy(Spec, "%s error: file is already open: name = %s", SpecLen - 1);
            }
            break;
        case FM_NAME_IS_FILE_CLOSED:
            if (State == FM_FILE_CLOSED || State == FM_FILE_EXISTS || State == FM_FILE_NOTOPEN)
            {
                Result = true;
            }
            else if (State == FM_DIR_EXISTS)
            {
                ErrorCode = FM_FNAME_ISFILE_EID_OFFSET;
                strncpy(Spec, "%s error: directory name exists as a file: name %s", SpecLen - 1);
            }
            else if (State == FM_DIR_NOEXIST)
            {
                ErrorCode = FM_FNAME_DNE_EID_OFFSET;
                strncpy(Spec, "%s error: directory name exists as a file: name = %s", SpecLen - 1);
            }
            else
            {
                ErrorCode = FM_FNAME_EXIST_EID_OFFSET;
                strncpy(Spec, "%s error: file already exists: name = %s", SpecLen - 1);
            }
            break;
        case FM_NAME_IS_DIRECTORY:
            if (State == FM_DIR_EXISTS)
            {
                Result = true;
            }
            else
            {
                ErrorCode = FM_FNAME_ISDIR_EID_OFFSET;
                strncpy(Spec, "%s error: filename is a directory: name = %s", SpecLen - 1);
            }
            break;
        default: /* FilenameState == FM_NAME_IS_INVALID */
            /* Insert a terminator in case the invalid string did not have one */
            Filename[BufferSize - 1] = '\0';
            ErrorCode                = FM_FNAME_INVALID_EID_OFFSET;
            strncpy(Spec, "%s error: filename is invalid: name = %s", SpecLen - 1);
            break;
    }

    if (!Result)
    {
        CFE_EVS_SendEvent((EventID + ErrorCode), CFE_EVS_EventType_ERROR, Spec, CmdText, Filename);
    }

    return Result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- verify state is closed file              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_VerifyFileClosed(char *Filename, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    return FM_VerifyFileState(FM_FILE_CLOSED, Filename, BufferSize, EventID, CmdText);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- verify state is open or closed file      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_VerifyFileExists(char *Filename, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    return FM_VerifyFileState(FM_FILE_EXISTS, Filename, BufferSize, EventID, CmdText);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- verify state is unused                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_VerifyFileNoExist(char *Filename, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    return FM_VerifyFileState(FM_FILE_NOEXIST, Filename, BufferSize, EventID, CmdText);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- verify state is unused or closed file    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_VerifyFileNotOpen(char *Filename, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    return FM_VerifyFileState(FM_FILE_NOTOPEN, Filename, BufferSize, EventID, CmdText);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- verify state is directory                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_VerifyDirExists(char *Directory, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    return FM_VerifyFileState(FM_DIR_EXISTS, Directory, BufferSize, EventID, CmdText);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- verify state is unused                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_VerifyDirNoExist(char *Name, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    return FM_VerifyFileState(FM_DIR_NOEXIST, Name, BufferSize, EventID, CmdText);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- verify child task interface is alive     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_VerifyChildTask(uint32 EventID, const char *CmdText)
{
    bool Result = false;

    /* Copy of child queue count that child task cannot change */
    uint8 LocalQueueCount = FM_GlobalData.ChildQueueCount;

    /* Verify child task is active and queue interface is healthy */
    if (!OS_ObjectIdDefined(FM_GlobalData.ChildSemaphore))
    {
        CFE_EVS_SendEvent((EventID + FM_CHILD_DISABLED_EID_OFFSET), CFE_EVS_EventType_ERROR,
                          "%s error: child task is disabled", CmdText);

        /* Child task disabled - cannot add another command */
        Result = false;
    }
    else if (LocalQueueCount == FM_CHILD_QUEUE_DEPTH)
    {
        CFE_EVS_SendEvent((EventID + FM_CHILD_Q_FULL_EID_OFFSET), CFE_EVS_EventType_ERROR,
                          "%s error: child task queue is full", CmdText);

        /* Queue full - cannot add another command */
        Result = false;
    }
    else if ((LocalQueueCount > FM_CHILD_QUEUE_DEPTH) || (FM_GlobalData.ChildWriteIndex >= FM_CHILD_QUEUE_DEPTH))
    {
        CFE_EVS_SendEvent((EventID + FM_CHILD_BROKEN_EID_OFFSET), CFE_EVS_EventType_ERROR,
                          "%s error: child task interface is broken: count = %d, index = %d", CmdText, LocalQueueCount,
                          FM_GlobalData.ChildWriteIndex);

        /* Queue broken - cannot add another command */
        Result = false;
    }
    else
    {
        memset(&FM_GlobalData.ChildQueue[FM_GlobalData.ChildWriteIndex], 0, sizeof(FM_GlobalData.ChildQueue[0]));

        /* OK to add another command to the queue */
        Result = true;
    }

    return Result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- invoke child task command processor      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_InvokeChildTask(void)
{
    /* Update callers queue index */
    FM_GlobalData.ChildWriteIndex++;

    if (FM_GlobalData.ChildWriteIndex >= FM_CHILD_QUEUE_DEPTH)
    {
        FM_GlobalData.ChildWriteIndex = 0;
    }

    /* Prevent parent/child updating queue counter at same time */
    OS_MutSemTake(FM_GlobalData.ChildQueueCountSem);
    FM_GlobalData.ChildQueueCount++;
    OS_MutSemGive(FM_GlobalData.ChildQueueCountSem);

    /* Does the child task still have a semaphore? */
    if (OS_ObjectIdDefined(FM_GlobalData.ChildSemaphore))
    {
        /* Signal child task to call command handler */
        OS_CountSemGive(FM_GlobalData.ChildSemaphore);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- add path separator to directory name     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_AppendPathSep(char *Directory, uint32 BufferSize)
{
    /*
    **  Previous verification tests ensure that the length of
    **   the string is both non-zero and less than the size
    **   of the string buffer.
    */
    uint32 StringLength = 0;

    StringLength = strlen(Directory);

    /* Do nothing if string already ends with a path separator */
    if ((StringLength != 0) && (Directory[StringLength - 1] != '/'))
    {
        /* Verify that string buffer has room for a path separator */
        if (StringLength < (BufferSize - 1))
        {
            /*
             * gcc 8+ will warn on strncat with constant of the same size as n
             * OK to use strcat since available space already confirmed
             */
            strcat(Directory, "/");
        }
    }
}
