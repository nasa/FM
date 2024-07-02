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
 *  File Manager (FM) Child task (low priority command handler)
 */

#include "cfe.h"
#include "fm_msg.h"
#include "fm_msgdefs.h"
#include "fm_msgids.h"
#include "fm_events.h"
#include "fm_app.h"
#include "fm_child.h"
#include "fm_cmds.h"
#include "fm_cmd_utils.h"
#include "fm_perfids.h"
#include "fm_platform_cfg.h"
#include "fm_verify.h"

#include <string.h>

/************************************************************************
** OSAL Compatibility for directory name access
** New OSAL version has an access macro to get the string. If that
** macro is defined, use it, otherwise assume "d_name" structure member.
*************************************************************************/
#ifndef OS_DIRENTRY_NAME
#define OS_DIRENTRY_NAME(x) ((x).d_name)
#endif

#define FM_QUEUE_SEM_NAME "FM_QUEUE_SEM"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task -- startup initialization                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CFE_Status_t FM_ChildInit(void)
{
    int32        TaskTextLen               = OS_MAX_PATH_LEN;
    char         TaskText[OS_MAX_PATH_LEN] = "\0";
    CFE_Status_t Result                    = CFE_SUCCESS;
    uint32       TaskEID                   = 0;

    /* Create counting semaphore (given by parent to wake-up child) */
    Result = OS_CountSemCreate(&FM_GlobalData.ChildSemaphore, FM_CHILD_SEM_NAME, 0, 0);
    if (Result != CFE_SUCCESS)
    {
        TaskEID = FM_CHILD_INIT_SEM_ERR_EID;
        strncpy(TaskText, "create semaphore failed", TaskTextLen - 1);
        TaskText[TaskTextLen - 1] = '\0';
    }
    else
    {
        /* Create mutex semaphore (protect access to ChildQueueCount) */
        Result = OS_MutSemCreate(&FM_GlobalData.ChildQueueCountSem, FM_QUEUE_SEM_NAME, 0);

        if (Result != CFE_SUCCESS)
        {
            TaskEID = FM_CHILD_INIT_QSEM_ERR_EID;
            strncpy(TaskText, "create queue count semaphore failed", TaskTextLen - 1);
            TaskText[TaskTextLen - 1] = '\0';
        }
        else
        {
            /* Create child task (low priority command handler) */
            Result = CFE_ES_CreateChildTask(&FM_GlobalData.ChildTaskID, FM_CHILD_TASK_NAME, FM_ChildTask, 0,
                                            FM_CHILD_TASK_STACK_SIZE, FM_CHILD_TASK_PRIORITY, 0);
            if (Result != CFE_SUCCESS)
            {
                TaskEID = FM_CHILD_INIT_CREATE_ERR_EID;
                strncpy(TaskText, "create task failed", TaskTextLen - 1);
                TaskText[TaskTextLen - 1] = '\0';
            }
        }
    }

    if (Result != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(TaskEID, CFE_EVS_EventType_ERROR, "Child Task initialization error: %s: result = %d",
                          TaskText, (int)Result);
    }

    return Result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task -- task entry point                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildTask(void)
{
    const char *TaskText = "Child Task";

    /*
    ** The child task runs until the parent dies (normal end) or
    **  until it encounters a fatal error (semaphore error, etc.)...
    */
    CFE_EVS_SendEvent(FM_CHILD_INIT_EID, CFE_EVS_EventType_INFORMATION, "%s initialization complete", TaskText);

    /* Child task process loop */
    FM_ChildLoop();

    /* Clear the semaphore ID */
    FM_GlobalData.ChildSemaphore = OS_OBJECT_ID_UNDEFINED;

    /* This call allows cFE to clean-up system resources */
    CFE_ES_ExitChildTask();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task -- main process loop                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildLoop(void)
{
    const char * TaskText = "Child Task termination error: ";
    CFE_Status_t Result   = CFE_SUCCESS;

    while (Result == CFE_SUCCESS)
    {
        /* Pend on the "handshake" semaphore */
        Result = OS_CountSemTake(FM_GlobalData.ChildSemaphore);

        /* Mark the period when this task is active */
        CFE_ES_PerfLogEntry(FM_CHILD_TASK_PERF_ID);

        if (Result == CFE_SUCCESS)
        {
            /* Make sure the parent/child handshake is not broken */
            if (FM_GlobalData.ChildQueueCount == 0)
            {
                FM_GlobalData.ChildCmdErrCounter++;
                CFE_EVS_SendEvent(FM_CHILD_TERM_EMPTYQ_ERR_EID, CFE_EVS_EventType_ERROR, "%s empty queue", TaskText);

                /* Set result that will terminate child task run loop */
                Result = OS_ERROR;
            }
            else if (FM_GlobalData.ChildReadIndex >= FM_CHILD_QUEUE_DEPTH)
            {
                FM_GlobalData.ChildCmdErrCounter++;
                CFE_EVS_SendEvent(FM_CHILD_TERM_QIDX_ERR_EID, CFE_EVS_EventType_ERROR,
                                  "%s invalid queue index: index = %d", TaskText, (int)FM_GlobalData.ChildReadIndex);

                /* Set result that will terminate child task run loop */
                Result = OS_ERROR;
            }
            else
            {
                /* Invoke the child task command handler */
                FM_ChildProcess();
            }
        }
        else
        {
            CFE_EVS_SendEvent(FM_CHILD_TERM_SEM_ERR_EID, CFE_EVS_EventType_ERROR,
                              "%s semaphore take failed: result = %d", TaskText, (int)Result);
        }

        CFE_ES_PerfLogExit(FM_CHILD_TASK_PERF_ID);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task -- interface handshake processor                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildProcess(void)
{
    const char *          TaskText = "Child Task";
    FM_ChildQueueEntry_t *CmdArgs  = &FM_GlobalData.ChildQueue[FM_GlobalData.ChildReadIndex];

    /* Invoke the command-specific handler */
    switch (CmdArgs->CommandCode)
    {
        case FM_COPY_FILE_CC:
            FM_ChildCopyCmd(CmdArgs);
            break;

        case FM_MOVE_FILE_CC:
            FM_ChildMoveCmd(CmdArgs);
            break;

        case FM_RENAME_FILE_CC:
            FM_ChildRenameCmd(CmdArgs);
            break;

        case FM_DELETE_FILE_CC:
            FM_ChildDeleteCmd(CmdArgs);
            break;

        case FM_DELETE_ALL_FILES_CC:
            FM_ChildDeleteAllFilesCmd(CmdArgs);
            break;

        case FM_DECOMPRESS_FILE_CC:
            FM_ChildDecompressFileCmd(CmdArgs);
            break;

        case FM_CONCAT_FILES_CC:
            FM_ChildConcatFilesCmd(CmdArgs);
            break;

        case FM_CREATE_DIRECTORY_CC:
            FM_ChildCreateDirectoryCmd(CmdArgs);
            break;

        case FM_DELETE_DIRECTORY_CC:
            FM_ChildDeleteDirectoryCmd(CmdArgs);
            break;

        case FM_GET_FILE_INFO_CC:
            FM_ChildFileInfoCmd(CmdArgs);
            break;

        case FM_GET_DIR_LIST_FILE_CC:
            FM_ChildDirListFileCmd(CmdArgs);
            break;

        case FM_GET_DIR_LIST_PKT_CC:
            FM_ChildDirListPktCmd(CmdArgs);
            break;

        case FM_SET_PERMISSIONS_CC:
            FM_ChildSetPermissionsCmd(CmdArgs);
            break;

        default:
            FM_GlobalData.ChildCmdErrCounter++;
            CFE_EVS_SendEvent(FM_CHILD_EXE_ERR_EID, CFE_EVS_EventType_ERROR,
                              "%s execution error: invalid command code: cc = %d", TaskText, (int)CmdArgs->CommandCode);
            break;
    }

    /* Update the handshake queue read index */
    FM_GlobalData.ChildReadIndex++;

    if (FM_GlobalData.ChildReadIndex >= FM_CHILD_QUEUE_DEPTH)
    {
        FM_GlobalData.ChildReadIndex = 0;
    }

    /* Prevent parent/child updating queue counter at same time */
    OS_MutSemTake(FM_GlobalData.ChildQueueCountSem);
    FM_GlobalData.ChildQueueCount--;
    OS_MutSemGive(FM_GlobalData.ChildQueueCountSem);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Copy File                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildCopyCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    const char *CmdText   = "Copy File";
    int32       OS_Status = OS_SUCCESS;

    /* Report current child task activity */
    FM_GlobalData.ChildCurrentCC = CmdArgs->CommandCode;

    /* Note the order of the arguments to OS_cp (src,tgt) */
    OS_Status = OS_cp(CmdArgs->Source1, CmdArgs->Target);

    if (OS_Status != OS_SUCCESS)
    {
        FM_GlobalData.ChildCmdErrCounter++;

        /* Send command failure event (error) */
        CFE_EVS_SendEvent(FM_COPY_OS_ERR_EID, CFE_EVS_EventType_ERROR,
                          "%s error: OS_cp failed: result = %d, src = %s, tgt = %s", CmdText, (int)OS_Status,
                          CmdArgs->Source1, CmdArgs->Target);
    }
    else
    {
        FM_GlobalData.ChildCmdCounter++;

        /* Send command completion event (info) */
        CFE_EVS_SendEvent(FM_COPY_CMD_INF_EID, CFE_EVS_EventType_INFORMATION, "%s command: src = %s, tgt = %s", CmdText,
                          CmdArgs->Source1, CmdArgs->Target);
    }

    /* Report previous child task activity */
    FM_GlobalData.ChildPreviousCC = CmdArgs->CommandCode;
    FM_GlobalData.ChildCurrentCC  = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Move File                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildMoveCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    const char *CmdText   = "Move File";
    int32       OS_Status = OS_SUCCESS;

    /* Report current child task activity */
    FM_GlobalData.ChildCurrentCC = CmdArgs->CommandCode;

    OS_Status = OS_mv(CmdArgs->Source1, CmdArgs->Target);

    if (OS_Status != OS_SUCCESS)
    {
        FM_GlobalData.ChildCmdErrCounter++;

        /* Send command failure event (error) */
        CFE_EVS_SendEvent(FM_MOVE_OS_ERR_EID, CFE_EVS_EventType_ERROR,
                          "%s error: OS_mv failed: result = %d, src = %s, tgt = %s", CmdText, (int)OS_Status,
                          CmdArgs->Source1, CmdArgs->Target);
    }
    else
    {
        FM_GlobalData.ChildCmdCounter++;

        /* Send command completion event (info) */
        CFE_EVS_SendEvent(FM_MOVE_CMD_INF_EID, CFE_EVS_EventType_INFORMATION, "%s command: src = %s, tgt = %s", CmdText,
                          CmdArgs->Source1, CmdArgs->Target);
    }

    /* Report previous child task activity */
    FM_GlobalData.ChildPreviousCC = CmdArgs->CommandCode;
    FM_GlobalData.ChildCurrentCC  = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Rename File                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildRenameCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    const char *CmdText   = "Rename File";
    int32       OS_Status = OS_SUCCESS;

    /* Report current child task activity */
    FM_GlobalData.ChildCurrentCC = CmdArgs->CommandCode;

    OS_Status = OS_rename(CmdArgs->Source1, CmdArgs->Target);

    if (OS_Status != OS_SUCCESS)
    {
        FM_GlobalData.ChildCmdErrCounter++;

        /* Send command failure event (error) */
        CFE_EVS_SendEvent(FM_RENAME_OS_ERR_EID, CFE_EVS_EventType_ERROR,
                          "%s error: OS_rename failed: result = %d, src = %s, tgt = %s", CmdText, (int)OS_Status,
                          CmdArgs->Source1, CmdArgs->Target);
    }
    else
    {
        FM_GlobalData.ChildCmdCounter++;

        /* Send command completion event (info) */
        CFE_EVS_SendEvent(FM_RENAME_CMD_INF_EID, CFE_EVS_EventType_INFORMATION, "%s command: src = %s, tgt = %s",
                          CmdText, CmdArgs->Source1, CmdArgs->Target);
    }

    /* Report previous child task activity */
    FM_GlobalData.ChildPreviousCC = CmdArgs->CommandCode;
    FM_GlobalData.ChildCurrentCC  = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Delete File                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildDeleteCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    const char *CmdText   = "Delete File";
    int32       OS_Status = OS_SUCCESS;

    /* Report current child task activity */
    FM_GlobalData.ChildCurrentCC = CmdArgs->CommandCode;

    OS_Status = OS_remove(CmdArgs->Source1);

    if (OS_Status != OS_SUCCESS)
    {
        FM_GlobalData.ChildCmdErrCounter++;

        /* Send command failure event (error) */
        CFE_EVS_SendEvent(FM_DELETE_OS_ERR_EID, CFE_EVS_EventType_ERROR,
                          "%s error: OS_remove failed: result = %d, file = %s", CmdText, (int)OS_Status,
                          CmdArgs->Source1);
    }
    else
    {
        FM_GlobalData.ChildCmdCounter++;

        /* Send command completion event (info) */
        CFE_EVS_SendEvent(FM_DELETE_CMD_INF_EID, CFE_EVS_EventType_INFORMATION, "%s command: file = %s", CmdText,
                          CmdArgs->Source1);
    }

    /* Report previous child task activity */
    FM_GlobalData.ChildPreviousCC = CmdArgs->CommandCode;
    FM_GlobalData.ChildCurrentCC  = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Delete All Files               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildDeleteAllFilesCmd(FM_ChildQueueEntry_t *CmdArgs)
{
    const char *CmdText = "Delete All Files";
    osal_id_t   DirId   = OS_OBJECT_ID_UNDEFINED;
    os_dirent_t DirEntry;
    int32       OS_Status                     = OS_SUCCESS;
    uint32      FilenameState                 = FM_NAME_IS_INVALID;
    uint32      NameLength                    = 0;
    uint32      DeleteCount                   = 0;
    uint32      FilesNotDeletedCount          = 0;
    uint32      DirectoriesSkippedCount       = 0;
    char        Filename[2 * OS_MAX_PATH_LEN] = "";

    /*
    ** Command argument usage for this command:
    **
    **  CmdArgs->CommandCode = FM_DELETE_ALL_FILES_CC
    **  CmdArgs->Source1     = directory name
    **  CmdArgs->Source2     = directory name plus separator
    */
    char *Directory  = CmdArgs->Source1;
    char *DirWithSep = CmdArgs->Source2;

    memset(&DirEntry, 0, sizeof(DirEntry));

    /* Report current child task activity */
    FM_GlobalData.ChildCurrentCC = CmdArgs->CommandCode;

    /* Open directory so that we can read from it */
    OS_Status = OS_DirectoryOpen(&DirId, Directory);

    if (OS_Status != OS_SUCCESS)
    {
        FM_GlobalData.ChildCmdErrCounter++;

        /* Send command failure event (error) */
        CFE_EVS_SendEvent(FM_DELETE_ALL_OS_ERR_EID, CFE_EVS_EventType_ERROR,
                          "%s error: OS_DirectoryOpen failed: dir = %s", CmdText, Directory);
    }
    else
    {
        /* Read each directory entry and delete the files */

        while (OS_DirectoryRead(DirId, &DirEntry) == OS_SUCCESS)
        {
            /*
            ** Ignore the "." and ".." directory entries
            */
            if ((strcmp(OS_DIRENTRY_NAME(DirEntry), FM_THIS_DIRECTORY) != 0) &&
                (strcmp(OS_DIRENTRY_NAME(DirEntry), FM_PARENT_DIRECTORY) != 0))
            {
                /* Construct full path filename */
                NameLength = snprintf(Filename, sizeof(Filename), "%s%s", DirWithSep, OS_DIRENTRY_NAME(DirEntry));

                if (NameLength >= OS_MAX_PATH_LEN)
                {
                    FilesNotDeletedCount++;
                }
                else
                {
                    /* What kind of directory entry is this? */
                    FilenameState = FM_GetFilenameState(Filename, OS_MAX_PATH_LEN, false);

                    /* FilenameState cannot have a value beyond five macros in cases below */
                    switch (FilenameState)
                    {
                        case FM_NAME_IS_INVALID:
                            FilesNotDeletedCount++;
                            break;

                        case FM_NAME_IS_NOT_IN_USE:

                            /* This result is very unlikely - the */
                            /*   name existed a moment ago when   */
                            /*   the directory entry was read but */
                            /*   now the call to OS_stat() failed */
                            /*   implying that the entry is gone  */
                            FilesNotDeletedCount++;
                            break;

                        case FM_NAME_IS_DIRECTORY:
                            DirectoriesSkippedCount++;
                            break;

                        case FM_NAME_IS_FILE_OPEN:
                            FilesNotDeletedCount++;
                            break;

                        case FM_NAME_IS_FILE_CLOSED:
                            if ((OS_Status = OS_remove(Filename)) == OS_SUCCESS)
                            {
                                /*
                                ** After deleting the file, rewind the directory
                                ** to keep the file system from getting confused
                                */
                                OS_DirectoryRewind(DirId);

                                /* Increment delete count */
                                DeleteCount++;
                            }
                            else
                            {
                                FilesNotDeletedCount++;
                            }

                            break;
                        default:
                            FilesNotDeletedCount++;
                            break;
                    } /* end switch statement */
                }
            } /* end if "." or ".." directory entries */
        }     /* End while OS_ReadDirectory */

        OS_DirectoryClose(DirId);

        /* Send command completion event (info) */
        CFE_EVS_SendEvent(FM_DELETE_ALL_CMD_INF_EID, CFE_EVS_EventType_INFORMATION,
                          "%s command: deleted %d files: dir = %s", CmdText, (int)DeleteCount, Directory);
        FM_GlobalData.ChildCmdCounter++;

        if (FilesNotDeletedCount > 0)
        {
            /* If errors occurred, report generic event(s) */
            CFE_EVS_SendEvent(FM_DELETE_ALL_FILES_ND_WARNING_EID, CFE_EVS_EventType_INFORMATION,
                              "%s command: one or more files could not be deleted. Files may be open : dir = %s",
                              CmdText, Directory);
            FM_GlobalData.ChildCmdWarnCounter++;
        }

        if (DirectoriesSkippedCount > 0)
        {
            /* If errors occurred, report generic event(s) */
            CFE_EVS_SendEvent(FM_DELETE_ALL_SKIP_WARNING_EID, CFE_EVS_EventType_INFORMATION,
                              "%s command: one or more directories skipped : dir = %s", CmdText, Directory);
            FM_GlobalData.ChildCmdWarnCounter++;
        }

    } /* end if OS_Status != OS_SUCCESS */

    /* Report previous child task activity */
    FM_GlobalData.ChildPreviousCC = CmdArgs->CommandCode;
    FM_GlobalData.ChildCurrentCC  = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Decompress File                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildDecompressFileCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    const char * CmdText    = "Decompress File";
    CFE_Status_t CFE_Status = CFE_SUCCESS;

    /* Report current child task activity */
    FM_GlobalData.ChildCurrentCC = CmdArgs->CommandCode;

    /* Decompress source file into target file */
    CFE_Status = FM_Decompress_Impl(FM_GlobalData.DecompressorStatePtr, CmdArgs->Source1, CmdArgs->Target);

    if (CFE_Status != CFE_SUCCESS)
    {
        FM_GlobalData.ChildCmdErrCounter++;

        /* Send command failure event (error) */
        CFE_EVS_SendEvent(FM_DECOM_CFE_ERR_EID, CFE_EVS_EventType_ERROR,
                          "%s error: CFE_FS_Decompress failed: result = %d, src = %s, tgt = %s", CmdText,
                          (int)CFE_Status, CmdArgs->Source1, CmdArgs->Target);
    }
    else
    {
        FM_GlobalData.ChildCmdCounter++;

        /* Send command completion event (info) */
        CFE_EVS_SendEvent(FM_DECOM_CMD_INF_EID, CFE_EVS_EventType_INFORMATION, "%s command: src = %s, tgt = %s",
                          CmdText, CmdArgs->Source1, CmdArgs->Target);
    }

    /* Report previous child task activity */
    FM_GlobalData.ChildPreviousCC = CmdArgs->CommandCode;
    FM_GlobalData.ChildCurrentCC  = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Concatenate Files              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildConcatFilesCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    const char *CmdText        = "Concat Files";
    bool        ConcatResult   = false;
    bool        CopyInProgress = false;
    int32       LoopCount      = 0;
    int32       OS_Status      = OS_SUCCESS;
    osal_id_t   FileHandleSrc  = OS_OBJECT_ID_UNDEFINED;
    osal_id_t   FileHandleTgt  = OS_OBJECT_ID_UNDEFINED;
    int32       BytesRead      = 0;
    int32       BytesWritten   = 0;

    /* Report current child task activity */
    FM_GlobalData.ChildCurrentCC = CmdArgs->CommandCode;

    /* Copy source file #1 to the target file */
    OS_Status = OS_cp(CmdArgs->Source1, CmdArgs->Target);

    if (OS_Status != OS_SUCCESS)
    {
        FM_GlobalData.ChildCmdErrCounter++;

        /* Send command failure event (error) */
        CFE_EVS_SendEvent(FM_CONCAT_OSCPY_ERR_EID, CFE_EVS_EventType_ERROR,
                          "%s error: OS_cp failed: result = %d, src = %s, tgt = %s", CmdText, (int)OS_Status,
                          CmdArgs->Source1, CmdArgs->Target);
    }
    else
    {
        /* Open source file #2 */
        OS_Status = OS_OpenCreate(&FileHandleSrc, CmdArgs->Source2, OS_FILE_FLAG_NONE, OS_READ_ONLY);

        if (OS_Status != OS_SUCCESS)
        {
            FM_GlobalData.ChildCmdErrCounter++;

            /* Send command failure event (error) */
            CFE_EVS_SendEvent(FM_CONCAT_OPEN_SRC2_ERR_EID, CFE_EVS_EventType_ERROR,
                              "%s error: OS_OpenCreate failed: result = %d, src2 = %s", CmdText, (int)OS_Status,
                              CmdArgs->Source2);
        }
        else
        {
            /* Open target file */
            OS_Status = OS_OpenCreate(&FileHandleTgt, CmdArgs->Target, OS_FILE_FLAG_NONE, OS_READ_WRITE);

            if (OS_Status != OS_SUCCESS)
            {
                FM_GlobalData.ChildCmdErrCounter++;

                /* Send command failure event (error) */
                CFE_EVS_SendEvent(FM_CONCAT_OPEN_TGT_ERR_EID, CFE_EVS_EventType_ERROR,
                                  "%s error: OS_OpenCreate failed: result = %d, tgt = %s", CmdText, (int)OS_Status,
                                  CmdArgs->Target);
            }
            else
            {
                /* Append source file #2 to target file */
                /* Seek to end of target file */
                OS_lseek(FileHandleTgt, 0, OS_SEEK_END);
                CopyInProgress = true;
                LoopCount      = 0;

                while (CopyInProgress)
                {
                    BytesRead = OS_read(FileHandleSrc, FM_GlobalData.ChildBuffer, FM_CHILD_FILE_BLOCK_SIZE);

                    if (BytesRead == 0)
                    {
                        /* Success - finished reading source file #2 */
                        CopyInProgress = false;
                        ConcatResult   = true;

                        FM_GlobalData.ChildCmdCounter++;

                        /* Send command completion event (info) */
                        CFE_EVS_SendEvent(FM_CONCAT_CMD_INF_EID, CFE_EVS_EventType_INFORMATION,
                                          "%s command: src1 = %s, src2 = %s, tgt = %s", CmdText, CmdArgs->Source1,
                                          CmdArgs->Source2, CmdArgs->Target);
                    }
                    else if (BytesRead < 0)
                    {
                        CopyInProgress = false;
                        FM_GlobalData.ChildCmdErrCounter++;

                        /* Send command failure event (error) */
                        CFE_EVS_SendEvent(FM_CONCAT_OSRD_ERR_EID, CFE_EVS_EventType_ERROR,
                                          "%s error: OS_read failed: result = %d, file = %s", CmdText, (int)BytesRead,
                                          CmdArgs->Source2);
                    }
                    else
                    {
                        /* Write source file #2 to target file */
                        BytesWritten = OS_write(FileHandleTgt, FM_GlobalData.ChildBuffer, BytesRead);

                        if (BytesWritten != BytesRead)
                        {
                            CopyInProgress = false;
                            FM_GlobalData.ChildCmdErrCounter++;

                            /* Send command failure event (error) */
                            CFE_EVS_SendEvent(FM_CONCAT_OSWR_ERR_EID, CFE_EVS_EventType_ERROR,
                                              "%s error: OS_write failed: result = %d, expected = %d", CmdText,
                                              (int)BytesWritten, (int)BytesRead);
                        }

                        /* Avoid CPU hogging */
                        LoopCount++;
                        if (LoopCount == FM_CHILD_FILE_LOOP_COUNT)
                        {
                            /* Give up the CPU */
                            CFE_ES_PerfLogExit(FM_CHILD_TASK_PERF_ID);
                            OS_TaskDelay(FM_CHILD_FILE_SLEEP_MS);
                            CFE_ES_PerfLogEntry(FM_CHILD_TASK_PERF_ID);
                            LoopCount = 0;
                        }
                    }
                }
                /* Close target file */
                OS_close(FileHandleTgt);
            }
            /* Close source file #2 */
            OS_close(FileHandleSrc);
        }

        if (ConcatResult == false)
        {
            /* Remove partial target file after concat error */
            OS_remove(CmdArgs->Target);
        }
    }

    /* Report previous child task activity */
    FM_GlobalData.ChildPreviousCC = CmdArgs->CommandCode;
    FM_GlobalData.ChildCurrentCC  = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Get File Info                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildFileInfoCmd(FM_ChildQueueEntry_t *CmdArgs)
{
    const char *CmdText    = "Get File Info";
    bool        GettingCRC = false;
    uint32      CurrentCRC = 0;
    int32       LoopCount  = 0;
    int32       BytesRead  = 0;
    osal_id_t   FileHandle = OS_OBJECT_ID_UNDEFINED;
    int32       Status     = 0;

    FM_FileInfoPkt_Payload_t *ReportPtr;

    /* Report current child task activity */
    FM_GlobalData.ChildCurrentCC = CmdArgs->CommandCode;

    /*
    ** Command argument usage for this command:
    **
    **  CmdArgs->CommandCode   = FM_GET_DIR_LIST_FILE_CC
    **  CmdArgs->Source1       = name of directory or file
    **  CmdArgs->FileInfoState = state of directory or file
    **  CmdArgs->FileInfoSize  = file size, else zero
    **  CmdArgs->FileInfoTime  = last modify time
    */

    /* Initialize file info packet (set all data to zero) */
    CFE_MSG_Init(CFE_MSG_PTR(FM_GlobalData.FileInfoPkt.TelemetryHeader), CFE_SB_ValueToMsgId(FM_FILE_INFO_TLM_MID),
                 sizeof(FM_FileInfoPkt_t));

    ReportPtr = &FM_GlobalData.FileInfoPkt.Payload;

    /* Report directory or filename state, name, size and time */
    ReportPtr->FileStatus = (uint8)CmdArgs->FileInfoState;
    snprintf(ReportPtr->Filename, OS_MAX_PATH_LEN, "%s", CmdArgs->Source1);

    ReportPtr->FileSize         = CmdArgs->FileInfoSize;
    ReportPtr->LastModifiedTime = CmdArgs->FileInfoTime;
    ReportPtr->Mode             = CmdArgs->Mode;

    /* Validate CRC algorithm */
    if (CmdArgs->FileInfoCRC != FM_IGNORE_CRC)
    {
        if (CmdArgs->FileInfoState != FM_NAME_IS_FILE_CLOSED)
        {
            /* Can only calculate CRC for closed files */
            FM_GlobalData.ChildCmdWarnCounter++;

            CFE_EVS_SendEvent(FM_GET_FILE_INFO_STATE_WARNING_EID, CFE_EVS_EventType_INFORMATION,
                              "%s warning: unable to compute CRC: invalid file state = %d, file = %s", CmdText,
                              (int)CmdArgs->FileInfoState, CmdArgs->Source1);

            CmdArgs->FileInfoCRC = FM_IGNORE_CRC;
        }
        else if ((CmdArgs->FileInfoCRC != CFE_ES_CrcType_CRC_8) && (CmdArgs->FileInfoCRC != CFE_ES_CrcType_CRC_16) &&
                 (CmdArgs->FileInfoCRC != CFE_ES_CrcType_CRC_32))
        {
            /* Can only calculate CRC using known algorithms */
            FM_GlobalData.ChildCmdWarnCounter++;

            CFE_EVS_SendEvent(FM_GET_FILE_INFO_TYPE_WARNING_EID, CFE_EVS_EventType_INFORMATION,
                              "%s warning: unable to compute CRC: invalid CRC type = %d, file = %s", CmdText,
                              (int)CmdArgs->FileInfoCRC, CmdArgs->Source1);

            CmdArgs->FileInfoCRC = FM_IGNORE_CRC;
        }
    }

    /* Compute CRC */
    if (CmdArgs->FileInfoCRC != FM_IGNORE_CRC)
    {
        Status = OS_OpenCreate(&FileHandle, CmdArgs->Source1, OS_FILE_FLAG_NONE, OS_READ_ONLY);

        if (Status != OS_SUCCESS)
        {
            FM_GlobalData.ChildCmdWarnCounter++;

            /* Send CRC failure event (warning) */
            CFE_EVS_SendEvent(FM_GET_FILE_INFO_OPEN_ERR_EID, CFE_EVS_EventType_ERROR,
                              "%s warning: unable to compute CRC: OS_OpenCreate result = %d, file = %s", CmdText,
                              (int)Status, CmdArgs->Source1);

            GettingCRC = false;
        }
        else
        {
            GettingCRC = true;
        }

        while (GettingCRC)
        {
            BytesRead = OS_read(FileHandle, FM_GlobalData.ChildBuffer, FM_CHILD_FILE_BLOCK_SIZE);

            if (BytesRead == 0)
            {
                /* Finished reading file */
                GettingCRC = false;
                OS_close(FileHandle);

                /* Add CRC to telemetry packet */
                ReportPtr->CRC_Computed = true;
                ReportPtr->CRC          = CurrentCRC;
            }
            else if (BytesRead < 0)
            {
                /* Error reading file */
                CurrentCRC = 0;
                GettingCRC = false;
                OS_close(FileHandle);

                /* Send CRC failure event (warning) */
                FM_GlobalData.ChildCmdWarnCounter++;
                CFE_EVS_SendEvent(FM_GET_FILE_INFO_READ_WARNING_EID, CFE_EVS_EventType_INFORMATION,
                                  "%s warning: unable to compute CRC: OS_read result = %d, file = %s", CmdText,
                                  (int)BytesRead, CmdArgs->Source1);
            }
            else
            {
                /* Continue CRC calculation */
                CurrentCRC =
                    CFE_ES_CalculateCRC(FM_GlobalData.ChildBuffer, BytesRead, CurrentCRC, CmdArgs->FileInfoCRC);
            }

            /* Avoid CPU hogging */
            if (GettingCRC)
            {
                LoopCount++;
                if (LoopCount == FM_CHILD_FILE_LOOP_COUNT)
                {
                    /* Give up the CPU */
                    CFE_ES_PerfLogExit(FM_CHILD_TASK_PERF_ID);
                    OS_TaskDelay(FM_CHILD_FILE_SLEEP_MS);
                    CFE_ES_PerfLogEntry(FM_CHILD_TASK_PERF_ID);
                    LoopCount = 0;
                }
            }
        }

        ReportPtr->CRC = CurrentCRC;
    }

    /* Timestamp and send file info telemetry packet */
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(FM_GlobalData.FileInfoPkt.TelemetryHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(FM_GlobalData.FileInfoPkt.TelemetryHeader), true);

    FM_GlobalData.ChildCmdCounter++;

    /* Send command completion event (info) */
    CFE_EVS_SendEvent(FM_GET_FILE_INFO_CMD_INF_EID, CFE_EVS_EventType_INFORMATION, "%s command: file = %s", CmdText,
                      CmdArgs->Source1);

    /* Report previous child task activity */
    FM_GlobalData.ChildPreviousCC = CmdArgs->CommandCode;
    FM_GlobalData.ChildCurrentCC  = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Create Directory               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildCreateDirectoryCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    const char *CmdText   = "Create Directory";
    int32       OS_Status = OS_SUCCESS;

    /* Report current child task activity */
    FM_GlobalData.ChildCurrentCC = CmdArgs->CommandCode;

    OS_Status = OS_mkdir(CmdArgs->Source1, 0);

    if (OS_Status != OS_SUCCESS)
    {
        FM_GlobalData.ChildCmdErrCounter++;

        /* Send command failure event (error) */
        CFE_EVS_SendEvent(FM_CREATE_DIR_OS_ERR_EID, CFE_EVS_EventType_ERROR,
                          "%s error: OS_mkdir failed: result = %d, dir = %s", CmdText, (int)OS_Status,
                          CmdArgs->Source1);
    }
    else
    {
        FM_GlobalData.ChildCmdCounter++;

        /* Send command completion event (info) */
        CFE_EVS_SendEvent(FM_CREATE_DIR_CMD_INF_EID, CFE_EVS_EventType_INFORMATION, "%s command: src = %s", CmdText,
                          CmdArgs->Source1);
    }

    /* Report previous child task activity */
    FM_GlobalData.ChildPreviousCC = CmdArgs->CommandCode;
    FM_GlobalData.ChildCurrentCC  = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Delete Directory               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildDeleteDirectoryCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    const char *CmdText      = "Delete Directory";
    bool        RemoveTheDir = true;
    osal_id_t   DirId        = OS_OBJECT_ID_UNDEFINED;
    os_dirent_t DirEntry;
    int32       OS_Status = OS_SUCCESS;

    memset(&DirEntry, 0, sizeof(DirEntry));

    /* Report current child task activity */
    FM_GlobalData.ChildCurrentCC = CmdArgs->CommandCode;

    /* Open the dir so we can see if it is empty */
    OS_Status = OS_DirectoryOpen(&DirId, CmdArgs->Source1);

    if (OS_Status != OS_SUCCESS)
    {
        CFE_EVS_SendEvent(FM_DELETE_OPENDIR_OS_ERR_EID, CFE_EVS_EventType_ERROR,
                          "%s error: OS_DirectoryOpen failed: dir = %s", CmdText, CmdArgs->Source1);

        RemoveTheDir = false;
        FM_GlobalData.ChildCmdErrCounter++;
    }
    else
    {
        /* Look for a directory entry that is not "." or ".." */
        while ((OS_DirectoryRead(DirId, &DirEntry) == OS_SUCCESS) && (RemoveTheDir == true))
        {
            if ((strcmp(OS_DIRENTRY_NAME(DirEntry), FM_THIS_DIRECTORY) != 0) &&
                (strcmp(OS_DIRENTRY_NAME(DirEntry), FM_PARENT_DIRECTORY) != 0))
            {
                CFE_EVS_SendEvent(FM_DELETE_DIR_EMPTY_ERR_EID, CFE_EVS_EventType_ERROR,
                                  "%s error: directory is not empty: dir = %s", CmdText, CmdArgs->Source1);

                RemoveTheDir = false;
                FM_GlobalData.ChildCmdErrCounter++;
            }
        }

        OS_DirectoryClose(DirId);
    }

    if (RemoveTheDir)
    {
        /* Remove the directory */
        OS_Status = OS_rmdir(CmdArgs->Source1);

        if (OS_Status != OS_SUCCESS)
        {
            /* Send command failure event (error) */
            CFE_EVS_SendEvent(FM_DELETE_RMDIR_OS_ERR_EID, CFE_EVS_EventType_ERROR,
                              "%s error: OS_rmdir failed: result = %d, dir = %s", CmdText, (int)OS_Status,
                              CmdArgs->Source1);

            FM_GlobalData.ChildCmdErrCounter++;
        }
        else
        {
            /* Send command completion event (info) */
            CFE_EVS_SendEvent(FM_DELETE_DIR_CMD_INF_EID, CFE_EVS_EventType_INFORMATION, "%s command: src = %s", CmdText,
                              CmdArgs->Source1);

            FM_GlobalData.ChildCmdCounter++;
        }
    }

    /* Report previous child task activity */
    FM_GlobalData.ChildPreviousCC = CmdArgs->CommandCode;
    FM_GlobalData.ChildCurrentCC  = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Get Directory List (to file)   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildDirListFileCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    const char *CmdText    = "Directory List to File";
    bool        Result     = false;
    osal_id_t   FileHandle = OS_OBJECT_ID_UNDEFINED;
    osal_id_t   DirId      = OS_OBJECT_ID_UNDEFINED;
    int32       Status     = 0;

    /* Report current child task activity */
    FM_GlobalData.ChildCurrentCC = CmdArgs->CommandCode;

    /*
    ** Command argument usage for this command:
    **
    **  CmdArgs->CommandCode = FM_GET_DIR_LIST_FILE_CC
    **  CmdArgs->Source1     = directory name
    **  CmdArgs->Source2     = directory name plus separator
    **  CmdArgs->Target      = output filename
    */

    /* Open directory for reading directory list */
    Status = OS_DirectoryOpen(&DirId, CmdArgs->Source1);

    if (Status != OS_SUCCESS)
    {
        FM_GlobalData.ChildCmdErrCounter++;

        /* Send command failure event (error) */
        CFE_EVS_SendEvent(FM_GET_DIR_FILE_OSOPENDIR_ERR_EID, CFE_EVS_EventType_ERROR,
                          "%s error: OS_DirectoryOpen failed: dir = %s", CmdText, CmdArgs->Source1);
    }
    else
    {
        /* Create output file, write placeholder for statistics, etc. */
        Result = FM_ChildDirListFileInit(&FileHandle, CmdArgs->Source1, CmdArgs->Target);
        if (Result == true)
        {
            /* Read directory listing and write contents to output file */
            FM_ChildDirListFileLoop(DirId, FileHandle, CmdArgs->Source1, CmdArgs->Source2, CmdArgs->Target,
                                    CmdArgs->GetSizeTimeMode);

            /* Close output file */
            OS_close(FileHandle);
        }

        /* Close directory list access handle */
        OS_DirectoryClose(DirId);
    }

    /* Report previous child task activity */
    FM_GlobalData.ChildPreviousCC = CmdArgs->CommandCode;
    FM_GlobalData.ChildCurrentCC  = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Get Directory List (to pkt)    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildDirListPktCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    const char *       CmdText                      = "Directory List to Packet";
    char               LogicalName[OS_MAX_PATH_LEN] = "\0";
    bool               StillProcessing              = true;
    osal_id_t          DirId                        = OS_OBJECT_ID_UNDEFINED;
    os_dirent_t        DirEntry;
    int32              ListIndex      = 0;
    FM_DirListEntry_t *ListEntry      = NULL;
    size_t             PathLength     = 0;
    size_t             EntryLength    = 0;
    int32              FilesTillSleep = FM_CHILD_STAT_SLEEP_FILECOUNT;
    int32              Status;

    FM_DirListPkt_Payload_t *ReportPtr;

    memset(&DirEntry, 0, sizeof(DirEntry));

    /* Report current child task activity */
    FM_GlobalData.ChildCurrentCC = CmdArgs->CommandCode;

    /*
    ** Command argument usage for this command:
    **
    **  CmdArgs->CommandCode   = FM_GET_DIR_LIST_PKT_CC
    **  CmdArgs->Source1       = directory name
    **  CmdArgs->Source2       = directory name plus separator
    **  CmdArgs->DirListOffset = index of 1st reported dir entry
    */
    PathLength = OS_strnlen(CmdArgs->Source2, OS_MAX_PATH_LEN);

    /* Open source directory for reading directory list */
    Status = OS_DirectoryOpen(&DirId, CmdArgs->Source1);

    if (Status != OS_SUCCESS)
    {
        FM_GlobalData.ChildCmdErrCounter++;

        /* Send command failure event (error) */
        CFE_EVS_SendEvent(FM_GET_DIR_PKT_OS_ERR_EID, CFE_EVS_EventType_ERROR,
                          "%s error: OS_DirectoryOpen failed: dir = %s", CmdText, CmdArgs->Source1);
    }
    else
    {
        /* Initialize the directory list telemetry packet */
        CFE_MSG_Init(CFE_MSG_PTR(FM_GlobalData.DirListPkt.TelemetryHeader), CFE_SB_ValueToMsgId(FM_DIR_LIST_TLM_MID),
                     sizeof(FM_DirListPkt_t));

        ReportPtr = &FM_GlobalData.DirListPkt.Payload;

        snprintf(ReportPtr->DirName, OS_MAX_PATH_LEN, "%s", CmdArgs->Source1);
        ReportPtr->FirstFile = CmdArgs->DirListOffset;

        StillProcessing = true;
        while (StillProcessing == true)
        {
            /* Read next directory entry */
            Status = OS_DirectoryRead(DirId, &DirEntry);

            if (Status != OS_SUCCESS)
            {
                /* Stop reading directory - no more entries */
                StillProcessing = false;
            }
            else if ((strcmp(OS_DIRENTRY_NAME(DirEntry), FM_THIS_DIRECTORY) != 0) &&
                     (strcmp(OS_DIRENTRY_NAME(DirEntry), FM_PARENT_DIRECTORY) != 0))
            {
                /* Do not count the "." and ".." directory entries */
                ReportPtr->TotalFiles++;

                /* Start collecting directory entries at command-specified offset */
                /* Stop collecting directory entries when telemetry packet is full */
                if ((ReportPtr->TotalFiles > ReportPtr->FirstFile) &&
                    (ReportPtr->PacketFiles < FM_DIR_LIST_PKT_ENTRIES))
                {
                    /* Create a shorthand access to the packet list entry */
                    ListIndex = ReportPtr->PacketFiles;
                    ListEntry = &ReportPtr->FileList[ListIndex];

                    EntryLength = OS_strnlen(OS_DIRENTRY_NAME(DirEntry), OS_MAX_FILE_NAME);

                    /* Verify combined directory plus filename length */
                    if ((PathLength + EntryLength) < sizeof(LogicalName))
                    {
                        /* Add filename to directory listing telemetry packet */
                        snprintf(ListEntry->EntryName, sizeof(ListEntry->EntryName), "%s", OS_DIRENTRY_NAME(DirEntry));

                        /* Build filename - Directory already has path separator */
                        memcpy(LogicalName, CmdArgs->Source2, PathLength);
                        memcpy(&LogicalName[PathLength], OS_DIRENTRY_NAME(DirEntry), EntryLength);
                        LogicalName[PathLength + EntryLength] = '\0';

                        FM_ChildSleepStat(LogicalName, ListEntry, &FilesTillSleep, CmdArgs->GetSizeTimeMode);

                        /* Add another entry to the telemetry packet */
                        ReportPtr->PacketFiles++;
                    }
                    else
                    {
                        FM_GlobalData.ChildCmdWarnCounter++;

                        /* Send command warning event (info) */
                        CFE_EVS_SendEvent(FM_GET_DIR_PKT_WARNING_EID, CFE_EVS_EventType_INFORMATION,
                                          "%s warning: dir + entry is too long: dir = %s, entry = %s", CmdText,
                                          CmdArgs->Source2, OS_DIRENTRY_NAME(DirEntry));
                    }
                }
            }
        }

        OS_DirectoryClose(DirId);

        /* Timestamp and send directory listing telemetry packet */
        CFE_SB_TimeStampMsg(CFE_MSG_PTR(FM_GlobalData.DirListPkt.TelemetryHeader));
        CFE_SB_TransmitMsg(CFE_MSG_PTR(FM_GlobalData.DirListPkt.TelemetryHeader), true);

        /* Send command completion event (info) */
        CFE_EVS_SendEvent(FM_GET_DIR_PKT_CMD_INF_EID, CFE_EVS_EventType_INFORMATION,
                          "%s command: offset = %d, dir = %s", CmdText, (int)CmdArgs->DirListOffset, CmdArgs->Source1);

        FM_GlobalData.ChildCmdCounter++;
    }

    /* Report previous child task activity */
    FM_GlobalData.ChildPreviousCC = CmdArgs->CommandCode;
    FM_GlobalData.ChildCurrentCC  = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Set File Permissions           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void FM_ChildSetPermissionsCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    int32       OS_Status = OS_SUCCESS;
    const char *CmdText   = "Set Permissions";

    OS_Status = OS_chmod(CmdArgs->Source1, CmdArgs->Mode);

    if (OS_Status == OS_SUCCESS)
    {
        FM_GlobalData.ChildCmdCounter++;

        /* Send command completion event (info) */
        CFE_EVS_SendEvent(FM_SET_PERM_CMD_INF_EID, CFE_EVS_EventType_INFORMATION, "%s command: file = %s, access = %d",
                          CmdText, CmdArgs->Source1, (int)CmdArgs->Mode);
    }
    else
    {
        FM_GlobalData.ChildCmdErrCounter++;

        /* Send OS error message */
        CFE_EVS_SendEvent(FM_SET_PERM_OS_ERR_EID, CFE_EVS_EventType_ERROR,
                          "%s command: OS_chmod error, RC=0x%08X, file = %s, access = %d", CmdText,
                          (unsigned int)OS_Status, CmdArgs->Source1, (int)CmdArgs->Mode);
    }

    /* Report previous child task activity */
    FM_GlobalData.ChildPreviousCC = CmdArgs->CommandCode;
    FM_GlobalData.ChildCurrentCC  = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task utility function -- create dir list output file   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_ChildDirListFileInit(osal_id_t *FileHandlePtr, const char *Directory, const char *Filename)
{
    const char *    CmdText       = "Directory List to File";
    bool            CommandResult = true;
    CFE_FS_Header_t FileHeader;
    osal_id_t       FileHandle   = OS_OBJECT_ID_UNDEFINED;
    int32           BytesWritten = 0;
    int32           Status       = 0;

    /* Initialize the standard cFE File Header for the Directory Listing File */
    CFE_FS_InitHeader(&FileHeader, CmdText, FM_DIR_LIST_FILE_SUBTYPE);

    /* Create directory listing output file */
    Status = OS_OpenCreate(&FileHandle, Filename, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);

    if (Status == OS_SUCCESS)
    {
        /* Write the standard CFE file header */
        BytesWritten = CFE_FS_WriteHeader(FileHandle, &FileHeader);
        if (BytesWritten == sizeof(CFE_FS_Header_t))
        {
            /* Initialize directory statistics structure */
            memset(&FM_GlobalData.DirListFileStats, 0, sizeof(FM_GlobalData.DirListFileStats));
            strncpy(FM_GlobalData.DirListFileStats.DirName, Directory, OS_MAX_PATH_LEN - 1);
            FM_GlobalData.DirListFileStats.DirName[OS_MAX_PATH_LEN - 1] = '\0';

            /* Write blank FM directory statistics structure as a placeholder */
            BytesWritten = OS_write(FileHandle, &FM_GlobalData.DirListFileStats, sizeof(FM_DirListFileStats_t));
            if (BytesWritten == sizeof(FM_DirListFileStats_t))
            {
                /* Return output file handle */
                *FileHandlePtr = FileHandle;
            }
            else
            {
                CommandResult = false;
                FM_GlobalData.ChildCmdErrCounter++;

                /* Send command failure event (error) */
                CFE_EVS_SendEvent(FM_GET_DIR_FILE_WRBLANK_ERR_EID, CFE_EVS_EventType_ERROR,
                                  "%s error: OS_write blank stats failed: result = %d, expected = %u", CmdText,
                                  (int)BytesWritten, (unsigned int)sizeof(FM_DirListFileStats_t));
            }
        }
        else
        {
            CommandResult = false;
            FM_GlobalData.ChildCmdErrCounter++;

            /* Send command failure event (error) */
            CFE_EVS_SendEvent(FM_GET_DIR_FILE_WRHDR_ERR_EID, CFE_EVS_EventType_ERROR,
                              "%s error: CFE_FS_WriteHeader failed: result = %d, expected = %u", CmdText,
                              (int)BytesWritten, (unsigned int)sizeof(CFE_FS_Header_t));
        }

        /* Close output file after write error */
        if (CommandResult == false)
        {
            OS_close(FileHandle);
        }
    }
    else
    {
        CommandResult = false;
        FM_GlobalData.ChildCmdErrCounter++;

        /* Send command failure event (error) */
        CFE_EVS_SendEvent(FM_GET_DIR_FILE_OSCREAT_ERR_EID, CFE_EVS_EventType_ERROR,
                          "%s error: OS_OpenCreate failed: result = %d, file = %s", CmdText, (int)Status, Filename);
    }

    return CommandResult;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task utility function -- write to dir list output file */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildDirListFileLoop(osal_id_t DirId, osal_id_t FileHandle, const char *Directory, const char *DirWithSep,
                             const char *Filename, uint8 getSizeTimeMode)
{
    const char *      CmdText                   = "Directory List to File";
    size_t            WriteLength               = sizeof(FM_DirListEntry_t);
    bool              ReadingDirectory          = true;
    bool              CommandResult             = true;
    uint32            DirEntries                = 0;
    uint32            FileEntries               = 0;
    size_t            EntryLength               = 0;
    size_t            PathLength                = 0;
    int32             BytesWritten              = 0;
    int32             FilesTillSleep            = FM_CHILD_STAT_SLEEP_FILECOUNT;
    int32             Status                    = 0;
    char              TempName[OS_MAX_PATH_LEN] = "\0";
    os_dirent_t       DirEntry;
    FM_DirListEntry_t DirListData;

    memset(&DirEntry, 0, sizeof(DirEntry));

    PathLength = OS_strnlen(DirWithSep, OS_MAX_PATH_LEN);

    /* Until end of directory entries or output file write error */
    while ((CommandResult == true) && (ReadingDirectory == true))
    {
        Status = OS_DirectoryRead(DirId, &DirEntry);

        /* Normal loop end - no more directory entries */
        if (Status != OS_SUCCESS)
        {
            ReadingDirectory = false;
        }
        else if ((strcmp(OS_DIRENTRY_NAME(DirEntry), FM_THIS_DIRECTORY) != 0) &&
                 (strcmp(OS_DIRENTRY_NAME(DirEntry), FM_PARENT_DIRECTORY) != 0))
        {
            /* Do not count the "." and ".." files */
            DirEntries++;

            /* Count all files - write limited number */
            if (FileEntries < FM_DIR_LIST_FILE_ENTRIES)
            {
                EntryLength = OS_strnlen(OS_DIRENTRY_NAME(DirEntry), OS_MAX_FILE_NAME);

                /*
                 * DirListData.EntryName and TempName are both OS_MAX_PATH_LEN, DirEntry name is OS_MAX_FILE_NAME,
                 * so limiting test is PathLength and EntryLength together
                 */
                if ((PathLength + EntryLength) < sizeof(TempName))
                {
                    /* Build qualified directory entry name */
                    memcpy(TempName, DirWithSep, PathLength);
                    memcpy(&TempName[PathLength], OS_DIRENTRY_NAME(DirEntry), EntryLength);
                    TempName[PathLength + EntryLength] = '\0';

                    /*
                     * Populate directory list file entry -
                     * Note this is guaranteed to be null-terminated due to the memset()
                     * this will leave at least one null char after the string.
                     */
                    memset(&DirListData, 0, sizeof(DirListData));
                    strncpy(DirListData.EntryName, OS_DIRENTRY_NAME(DirEntry), sizeof(DirListData.EntryName) - 1);

                    FM_ChildSleepStat(TempName, &DirListData, &FilesTillSleep, getSizeTimeMode);

                    /* Write directory list file entry to output file */
                    BytesWritten = OS_write(FileHandle, &DirListData, WriteLength);

                    if (BytesWritten == WriteLength)
                    {
                        FileEntries++;
                    }
                    else
                    {
                        CommandResult = false;
                        FM_GlobalData.ChildCmdErrCounter++;

                        /* Send command failure event (error) */
                        CFE_EVS_SendEvent(FM_GET_DIR_FILE_WRENTRY_ERR_EID, CFE_EVS_EventType_ERROR,
                                          "%s error: OS_write entry failed: result = %d, expected = %d", CmdText,
                                          (int)BytesWritten, (int)WriteLength);
                    }
                }
                else
                {
                    FM_GlobalData.ChildCmdWarnCounter++;

                    /* Send command failure event (error) */
                    CFE_EVS_SendEvent(FM_GET_DIR_FILE_WARNING_EID, CFE_EVS_EventType_INFORMATION,
                                      "%s error: combined directory and entry name too long: dir = %s, entry = %s",
                                      CmdText, Directory, OS_DIRENTRY_NAME(DirEntry));
                }
            }
        }
    }

    /* Update directory statistics in output file */
    if ((CommandResult == true) && (DirEntries != 0))
    {
        /* Update entries found in directory vs entries written to file */
        FM_GlobalData.DirListFileStats.DirEntries  = DirEntries;
        FM_GlobalData.DirListFileStats.FileEntries = FileEntries;

        /* Back up to the start of the statistics data */
        OS_lseek(FileHandle, sizeof(CFE_FS_Header_t), OS_SEEK_SET);

        /* Write an updated version of the statistics data */
        WriteLength  = sizeof(FM_DirListFileStats_t);
        BytesWritten = OS_write(FileHandle, &FM_GlobalData.DirListFileStats, WriteLength);

        if (BytesWritten != WriteLength)
        {
            CommandResult = false;
            FM_GlobalData.ChildCmdErrCounter++;

            /* Send command failure event (error) */
            CFE_EVS_SendEvent(FM_GET_DIR_FILE_UPSTATS_ERR_EID, CFE_EVS_EventType_ERROR,
                              "%s error: OS_write update stats failed: result = %d, expected = %d", CmdText,
                              (int)BytesWritten, (int)WriteLength);
        }
    }

    /* Send command completion event (info) */
    if (CommandResult == true)
    {
        FM_GlobalData.ChildCmdCounter++;

        CFE_EVS_SendEvent(FM_GET_DIR_FILE_CMD_INF_EID, CFE_EVS_EventType_INFORMATION,
                          "%s command: wrote %d of %d names: dir = %s, filename = %s", CmdText, (int)FileEntries,
                          (int)DirEntries, Directory, Filename);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task utility function -- get dir entry size and time   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 FM_ChildSizeTimeMode(const char *Filename, uint32 *FileSize, uint32 *FileTime, uint32 *FileMode)
{
    int32      Result = OS_SUCCESS;
    os_fstat_t FileStatus;

    memset(&FileStatus, 0, sizeof(FileStatus));

    Result = OS_stat(Filename, &FileStatus);

    if (Result != OS_SUCCESS)
    {
        *FileSize = 0;
        *FileTime = 0;
        *FileMode = 0;
    }
    else
    {
        *FileTime = OS_FILESTAT_TIME(FileStatus);
        *FileSize = OS_FILESTAT_SIZE(FileStatus);
        *FileMode = OS_FILESTAT_MODE(FileStatus);
    }

    return Result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task utility function -- sleep between OS_stat on files*/
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildSleepStat(const char *Filename, FM_DirListEntry_t *DirListData, int32 *FilesTillSleep,
                       bool getSizeTimeMode)
{
    /* Check if command requested size and time */
    if (getSizeTimeMode == true)
    {
        if (*FilesTillSleep <= 0)
        {
            CFE_ES_PerfLogExit(FM_CHILD_TASK_PERF_ID);
            OS_TaskDelay(FM_CHILD_STAT_SLEEP_MS);
            CFE_ES_PerfLogEntry(FM_CHILD_TASK_PERF_ID);
            *FilesTillSleep = FM_CHILD_STAT_SLEEP_FILECOUNT;
        }

        /* Get file size, date, and mode */
        FM_ChildSizeTimeMode(Filename, &(DirListData->EntrySize), &(DirListData->ModifyTime), &(DirListData->Mode));

        (*FilesTillSleep)--;
    }
    else
    {
        DirListData->EntrySize  = 0;
        DirListData->ModifyTime = 0;
        DirListData->Mode       = 0;
    }
}
