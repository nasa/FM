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
 *
 * Auto-Generated stub implementations for functions defined in fm_child header
 */

#include "fm_child.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ChildConcatFilesCmd()
 * ----------------------------------------------------
 */
void FM_ChildConcatFilesCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_GenStub_AddParam(FM_ChildConcatFilesCmd, const FM_ChildQueueEntry_t *, CmdArgs);

    UT_GenStub_Execute(FM_ChildConcatFilesCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ChildCopyCmd()
 * ----------------------------------------------------
 */
void FM_ChildCopyCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_GenStub_AddParam(FM_ChildCopyCmd, const FM_ChildQueueEntry_t *, CmdArgs);

    UT_GenStub_Execute(FM_ChildCopyCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ChildCreateDirectoryCmd()
 * ----------------------------------------------------
 */
void FM_ChildCreateDirectoryCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_GenStub_AddParam(FM_ChildCreateDirectoryCmd, const FM_ChildQueueEntry_t *, CmdArgs);

    UT_GenStub_Execute(FM_ChildCreateDirectoryCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ChildDecompressFileCmd()
 * ----------------------------------------------------
 */
void FM_ChildDecompressFileCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_GenStub_AddParam(FM_ChildDecompressFileCmd, const FM_ChildQueueEntry_t *, CmdArgs);

    UT_GenStub_Execute(FM_ChildDecompressFileCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ChildDeleteAllFilesCmd()
 * ----------------------------------------------------
 */
void FM_ChildDeleteAllFilesCmd(FM_ChildQueueEntry_t *CmdArgs)
{
    UT_GenStub_AddParam(FM_ChildDeleteAllFilesCmd, FM_ChildQueueEntry_t *, CmdArgs);

    UT_GenStub_Execute(FM_ChildDeleteAllFilesCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ChildDeleteCmd()
 * ----------------------------------------------------
 */
void FM_ChildDeleteCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_GenStub_AddParam(FM_ChildDeleteCmd, const FM_ChildQueueEntry_t *, CmdArgs);

    UT_GenStub_Execute(FM_ChildDeleteCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ChildDeleteDirectoryCmd()
 * ----------------------------------------------------
 */
void FM_ChildDeleteDirectoryCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_GenStub_AddParam(FM_ChildDeleteDirectoryCmd, const FM_ChildQueueEntry_t *, CmdArgs);

    UT_GenStub_Execute(FM_ChildDeleteDirectoryCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ChildDirListFileCmd()
 * ----------------------------------------------------
 */
void FM_ChildDirListFileCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_GenStub_AddParam(FM_ChildDirListFileCmd, const FM_ChildQueueEntry_t *, CmdArgs);

    UT_GenStub_Execute(FM_ChildDirListFileCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ChildDirListFileInit()
 * ----------------------------------------------------
 */
bool FM_ChildDirListFileInit(osal_id_t *FileHandlePtr, const char *Directory, const char *Filename)
{
    UT_GenStub_SetupReturnBuffer(FM_ChildDirListFileInit, bool);

    UT_GenStub_AddParam(FM_ChildDirListFileInit, osal_id_t *, FileHandlePtr);
    UT_GenStub_AddParam(FM_ChildDirListFileInit, const char *, Directory);
    UT_GenStub_AddParam(FM_ChildDirListFileInit, const char *, Filename);

    UT_GenStub_Execute(FM_ChildDirListFileInit, Basic, NULL);

    return UT_GenStub_GetReturnValue(FM_ChildDirListFileInit, bool);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ChildDirListFileLoop()
 * ----------------------------------------------------
 */
void FM_ChildDirListFileLoop(osal_id_t DirId, osal_id_t FileHandle, const char *Directory, const char *DirWithSep,
                             const char *Filename, uint8 GetSizeTimeMode)
{
    UT_GenStub_AddParam(FM_ChildDirListFileLoop, osal_id_t, DirId);
    UT_GenStub_AddParam(FM_ChildDirListFileLoop, osal_id_t, FileHandle);
    UT_GenStub_AddParam(FM_ChildDirListFileLoop, const char *, Directory);
    UT_GenStub_AddParam(FM_ChildDirListFileLoop, const char *, DirWithSep);
    UT_GenStub_AddParam(FM_ChildDirListFileLoop, const char *, Filename);
    UT_GenStub_AddParam(FM_ChildDirListFileLoop, uint8, GetSizeTimeMode);

    UT_GenStub_Execute(FM_ChildDirListFileLoop, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ChildDirListPktCmd()
 * ----------------------------------------------------
 */
void FM_ChildDirListPktCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_GenStub_AddParam(FM_ChildDirListPktCmd, const FM_ChildQueueEntry_t *, CmdArgs);

    UT_GenStub_Execute(FM_ChildDirListPktCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ChildFileInfoCmd()
 * ----------------------------------------------------
 */
void FM_ChildFileInfoCmd(FM_ChildQueueEntry_t *CmdArgs)
{
    UT_GenStub_AddParam(FM_ChildFileInfoCmd, FM_ChildQueueEntry_t *, CmdArgs);

    UT_GenStub_Execute(FM_ChildFileInfoCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ChildInit()
 * ----------------------------------------------------
 */
CFE_Status_t FM_ChildInit(void)
{
    UT_GenStub_SetupReturnBuffer(FM_ChildInit, CFE_Status_t);

    UT_GenStub_Execute(FM_ChildInit, Basic, NULL);

    return UT_GenStub_GetReturnValue(FM_ChildInit, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ChildLoop()
 * ----------------------------------------------------
 */
void FM_ChildLoop(void)
{

    UT_GenStub_Execute(FM_ChildLoop, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ChildMoveCmd()
 * ----------------------------------------------------
 */
void FM_ChildMoveCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_GenStub_AddParam(FM_ChildMoveCmd, const FM_ChildQueueEntry_t *, CmdArgs);

    UT_GenStub_Execute(FM_ChildMoveCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ChildProcess()
 * ----------------------------------------------------
 */
void FM_ChildProcess(void)
{

    UT_GenStub_Execute(FM_ChildProcess, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ChildRenameCmd()
 * ----------------------------------------------------
 */
void FM_ChildRenameCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_GenStub_AddParam(FM_ChildRenameCmd, const FM_ChildQueueEntry_t *, CmdArgs);

    UT_GenStub_Execute(FM_ChildRenameCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ChildSetPermissionsCmd()
 * ----------------------------------------------------
 */
void FM_ChildSetPermissionsCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_GenStub_AddParam(FM_ChildSetPermissionsCmd, const FM_ChildQueueEntry_t *, CmdArgs);

    UT_GenStub_Execute(FM_ChildSetPermissionsCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ChildSizeTimeMode()
 * ----------------------------------------------------
 */
int32 FM_ChildSizeTimeMode(const char *Filename, uint32 *FileSize, uint32 *FileTime, uint32 *FileMode)
{
    UT_GenStub_SetupReturnBuffer(FM_ChildSizeTimeMode, int32);

    UT_GenStub_AddParam(FM_ChildSizeTimeMode, const char *, Filename);
    UT_GenStub_AddParam(FM_ChildSizeTimeMode, uint32 *, FileSize);
    UT_GenStub_AddParam(FM_ChildSizeTimeMode, uint32 *, FileTime);
    UT_GenStub_AddParam(FM_ChildSizeTimeMode, uint32 *, FileMode);

    UT_GenStub_Execute(FM_ChildSizeTimeMode, Basic, NULL);

    return UT_GenStub_GetReturnValue(FM_ChildSizeTimeMode, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ChildSleepStat()
 * ----------------------------------------------------
 */
void FM_ChildSleepStat(const char *Filename, FM_DirListEntry_t *DirListData, int32 *FilesTillSleep,
                       bool GetSizeTimeMode)
{
    UT_GenStub_AddParam(FM_ChildSleepStat, const char *, Filename);
    UT_GenStub_AddParam(FM_ChildSleepStat, FM_DirListEntry_t *, DirListData);
    UT_GenStub_AddParam(FM_ChildSleepStat, int32 *, FilesTillSleep);
    UT_GenStub_AddParam(FM_ChildSleepStat, bool, GetSizeTimeMode);

    UT_GenStub_Execute(FM_ChildSleepStat, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ChildTask()
 * ----------------------------------------------------
 */
void FM_ChildTask(void)
{

    UT_GenStub_Execute(FM_ChildTask, Basic, NULL);
}
