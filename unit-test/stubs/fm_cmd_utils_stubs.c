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
 * Auto-Generated stub implementations for functions defined in fm_cmd_utils header
 */

#include "fm_cmd_utils.h"
#include "utgenstub.h"

void UT_DefaultHandler_FM_IsValidCmdPktLength(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_FM_VerifyChildTask(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_FM_VerifyDirExists(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_FM_VerifyDirNoExist(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_FM_VerifyFileClosed(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_FM_VerifyFileExists(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_FM_VerifyFileNoExist(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_FM_VerifyFileNotOpen(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_FM_VerifyFileState(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_FM_VerifyOverwrite(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for FM_AppendPathSep()
 * ----------------------------------------------------
 */
void FM_AppendPathSep(char *Directory, uint32 BufferSize)
{
    UT_GenStub_AddParam(FM_AppendPathSep, char *, Directory);
    UT_GenStub_AddParam(FM_AppendPathSep, uint32, BufferSize);

    UT_GenStub_Execute(FM_AppendPathSep, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_GetDirectorySpaceEstimate()
 * ----------------------------------------------------
 */
int32 FM_GetDirectorySpaceEstimate(const char *Directory, uint64 *BlockCount, uint64 *ByteCount)
{
    UT_GenStub_SetupReturnBuffer(FM_GetDirectorySpaceEstimate, int32);

    UT_GenStub_AddParam(FM_GetDirectorySpaceEstimate, const char *, Directory);
    UT_GenStub_AddParam(FM_GetDirectorySpaceEstimate, uint64 *, BlockCount);
    UT_GenStub_AddParam(FM_GetDirectorySpaceEstimate, uint64 *, ByteCount);

    UT_GenStub_Execute(FM_GetDirectorySpaceEstimate, Basic, NULL);

    return UT_GenStub_GetReturnValue(FM_GetDirectorySpaceEstimate, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_GetFilenameState()
 * ----------------------------------------------------
 */
uint32 FM_GetFilenameState(char *Filename, uint32 BufferSize, bool FileInfoCmd)
{
    UT_GenStub_SetupReturnBuffer(FM_GetFilenameState, uint32);

    UT_GenStub_AddParam(FM_GetFilenameState, char *, Filename);
    UT_GenStub_AddParam(FM_GetFilenameState, uint32, BufferSize);
    UT_GenStub_AddParam(FM_GetFilenameState, bool, FileInfoCmd);

    UT_GenStub_Execute(FM_GetFilenameState, Basic, NULL);

    return UT_GenStub_GetReturnValue(FM_GetFilenameState, uint32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_GetOpenFilesData()
 * ----------------------------------------------------
 */
uint32 FM_GetOpenFilesData(const FM_OpenFilesEntry_t *OpenFilesData)
{
    UT_GenStub_SetupReturnBuffer(FM_GetOpenFilesData, uint32);

    UT_GenStub_AddParam(FM_GetOpenFilesData, const FM_OpenFilesEntry_t *, OpenFilesData);

    UT_GenStub_Execute(FM_GetOpenFilesData, Basic, NULL);

    return UT_GenStub_GetReturnValue(FM_GetOpenFilesData, uint32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_GetVolumeFreeSpace()
 * ----------------------------------------------------
 */
int32 FM_GetVolumeFreeSpace(const char *FileSys, uint64 *BlockCount, uint64 *ByteCount)
{
    UT_GenStub_SetupReturnBuffer(FM_GetVolumeFreeSpace, int32);

    UT_GenStub_AddParam(FM_GetVolumeFreeSpace, const char *, FileSys);
    UT_GenStub_AddParam(FM_GetVolumeFreeSpace, uint64 *, BlockCount);
    UT_GenStub_AddParam(FM_GetVolumeFreeSpace, uint64 *, ByteCount);

    UT_GenStub_Execute(FM_GetVolumeFreeSpace, Basic, NULL);

    return UT_GenStub_GetReturnValue(FM_GetVolumeFreeSpace, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_InvokeChildTask()
 * ----------------------------------------------------
 */
void FM_InvokeChildTask(void)
{

    UT_GenStub_Execute(FM_InvokeChildTask, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_IsValidCmdPktLength()
 * ----------------------------------------------------
 */
bool FM_IsValidCmdPktLength(const CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength, uint32 EventID, const char *CmdText)
{
    UT_GenStub_SetupReturnBuffer(FM_IsValidCmdPktLength, bool);

    UT_GenStub_AddParam(FM_IsValidCmdPktLength, const CFE_MSG_Message_t *, MsgPtr);
    UT_GenStub_AddParam(FM_IsValidCmdPktLength, size_t, ExpectedLength);
    UT_GenStub_AddParam(FM_IsValidCmdPktLength, uint32, EventID);
    UT_GenStub_AddParam(FM_IsValidCmdPktLength, const char *, CmdText);

    UT_GenStub_Execute(FM_IsValidCmdPktLength, Basic, UT_DefaultHandler_FM_IsValidCmdPktLength);

    return UT_GenStub_GetReturnValue(FM_IsValidCmdPktLength, bool);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_VerifyChildTask()
 * ----------------------------------------------------
 */
bool FM_VerifyChildTask(uint32 EventID, const char *CmdText)
{
    UT_GenStub_SetupReturnBuffer(FM_VerifyChildTask, bool);

    UT_GenStub_AddParam(FM_VerifyChildTask, uint32, EventID);
    UT_GenStub_AddParam(FM_VerifyChildTask, const char *, CmdText);

    UT_GenStub_Execute(FM_VerifyChildTask, Basic, UT_DefaultHandler_FM_VerifyChildTask);

    return UT_GenStub_GetReturnValue(FM_VerifyChildTask, bool);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_VerifyDirExists()
 * ----------------------------------------------------
 */
bool FM_VerifyDirExists(char *Directory, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    UT_GenStub_SetupReturnBuffer(FM_VerifyDirExists, bool);

    UT_GenStub_AddParam(FM_VerifyDirExists, char *, Directory);
    UT_GenStub_AddParam(FM_VerifyDirExists, uint32, BufferSize);
    UT_GenStub_AddParam(FM_VerifyDirExists, uint32, EventID);
    UT_GenStub_AddParam(FM_VerifyDirExists, const char *, CmdText);

    UT_GenStub_Execute(FM_VerifyDirExists, Basic, UT_DefaultHandler_FM_VerifyDirExists);

    return UT_GenStub_GetReturnValue(FM_VerifyDirExists, bool);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_VerifyDirNoExist()
 * ----------------------------------------------------
 */
bool FM_VerifyDirNoExist(char *Name, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    UT_GenStub_SetupReturnBuffer(FM_VerifyDirNoExist, bool);

    UT_GenStub_AddParam(FM_VerifyDirNoExist, char *, Name);
    UT_GenStub_AddParam(FM_VerifyDirNoExist, uint32, BufferSize);
    UT_GenStub_AddParam(FM_VerifyDirNoExist, uint32, EventID);
    UT_GenStub_AddParam(FM_VerifyDirNoExist, const char *, CmdText);

    UT_GenStub_Execute(FM_VerifyDirNoExist, Basic, UT_DefaultHandler_FM_VerifyDirNoExist);

    return UT_GenStub_GetReturnValue(FM_VerifyDirNoExist, bool);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_VerifyFileClosed()
 * ----------------------------------------------------
 */
bool FM_VerifyFileClosed(char *Filename, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    UT_GenStub_SetupReturnBuffer(FM_VerifyFileClosed, bool);

    UT_GenStub_AddParam(FM_VerifyFileClosed, char *, Filename);
    UT_GenStub_AddParam(FM_VerifyFileClosed, uint32, BufferSize);
    UT_GenStub_AddParam(FM_VerifyFileClosed, uint32, EventID);
    UT_GenStub_AddParam(FM_VerifyFileClosed, const char *, CmdText);

    UT_GenStub_Execute(FM_VerifyFileClosed, Basic, UT_DefaultHandler_FM_VerifyFileClosed);

    return UT_GenStub_GetReturnValue(FM_VerifyFileClosed, bool);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_VerifyFileExists()
 * ----------------------------------------------------
 */
bool FM_VerifyFileExists(char *Filename, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    UT_GenStub_SetupReturnBuffer(FM_VerifyFileExists, bool);

    UT_GenStub_AddParam(FM_VerifyFileExists, char *, Filename);
    UT_GenStub_AddParam(FM_VerifyFileExists, uint32, BufferSize);
    UT_GenStub_AddParam(FM_VerifyFileExists, uint32, EventID);
    UT_GenStub_AddParam(FM_VerifyFileExists, const char *, CmdText);

    UT_GenStub_Execute(FM_VerifyFileExists, Basic, UT_DefaultHandler_FM_VerifyFileExists);

    return UT_GenStub_GetReturnValue(FM_VerifyFileExists, bool);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_VerifyFileNoExist()
 * ----------------------------------------------------
 */
bool FM_VerifyFileNoExist(char *Filename, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    UT_GenStub_SetupReturnBuffer(FM_VerifyFileNoExist, bool);

    UT_GenStub_AddParam(FM_VerifyFileNoExist, char *, Filename);
    UT_GenStub_AddParam(FM_VerifyFileNoExist, uint32, BufferSize);
    UT_GenStub_AddParam(FM_VerifyFileNoExist, uint32, EventID);
    UT_GenStub_AddParam(FM_VerifyFileNoExist, const char *, CmdText);

    UT_GenStub_Execute(FM_VerifyFileNoExist, Basic, UT_DefaultHandler_FM_VerifyFileNoExist);

    return UT_GenStub_GetReturnValue(FM_VerifyFileNoExist, bool);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_VerifyFileNotOpen()
 * ----------------------------------------------------
 */
bool FM_VerifyFileNotOpen(char *Filename, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    UT_GenStub_SetupReturnBuffer(FM_VerifyFileNotOpen, bool);

    UT_GenStub_AddParam(FM_VerifyFileNotOpen, char *, Filename);
    UT_GenStub_AddParam(FM_VerifyFileNotOpen, uint32, BufferSize);
    UT_GenStub_AddParam(FM_VerifyFileNotOpen, uint32, EventID);
    UT_GenStub_AddParam(FM_VerifyFileNotOpen, const char *, CmdText);

    UT_GenStub_Execute(FM_VerifyFileNotOpen, Basic, UT_DefaultHandler_FM_VerifyFileNotOpen);

    return UT_GenStub_GetReturnValue(FM_VerifyFileNotOpen, bool);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_VerifyFileState()
 * ----------------------------------------------------
 */
bool FM_VerifyFileState(FM_File_States State, char *Filename, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    UT_GenStub_SetupReturnBuffer(FM_VerifyFileState, bool);

    UT_GenStub_AddParam(FM_VerifyFileState, FM_File_States, State);
    UT_GenStub_AddParam(FM_VerifyFileState, char *, Filename);
    UT_GenStub_AddParam(FM_VerifyFileState, uint32, BufferSize);
    UT_GenStub_AddParam(FM_VerifyFileState, uint32, EventID);
    UT_GenStub_AddParam(FM_VerifyFileState, const char *, CmdText);

    UT_GenStub_Execute(FM_VerifyFileState, Basic, UT_DefaultHandler_FM_VerifyFileState);

    return UT_GenStub_GetReturnValue(FM_VerifyFileState, bool);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_VerifyNameValid()
 * ----------------------------------------------------
 */
uint32 FM_VerifyNameValid(char *Name, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    UT_GenStub_SetupReturnBuffer(FM_VerifyNameValid, uint32);

    UT_GenStub_AddParam(FM_VerifyNameValid, char *, Name);
    UT_GenStub_AddParam(FM_VerifyNameValid, uint32, BufferSize);
    UT_GenStub_AddParam(FM_VerifyNameValid, uint32, EventID);
    UT_GenStub_AddParam(FM_VerifyNameValid, const char *, CmdText);

    UT_GenStub_Execute(FM_VerifyNameValid, Basic, NULL);

    return UT_GenStub_GetReturnValue(FM_VerifyNameValid, uint32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_VerifyOverwrite()
 * ----------------------------------------------------
 */
bool FM_VerifyOverwrite(uint16 Overwrite, uint32 EventID, const char *CmdText)
{
    UT_GenStub_SetupReturnBuffer(FM_VerifyOverwrite, bool);

    UT_GenStub_AddParam(FM_VerifyOverwrite, uint16, Overwrite);
    UT_GenStub_AddParam(FM_VerifyOverwrite, uint32, EventID);
    UT_GenStub_AddParam(FM_VerifyOverwrite, const char *, CmdText);

    UT_GenStub_Execute(FM_VerifyOverwrite, Basic, UT_DefaultHandler_FM_VerifyOverwrite);

    return UT_GenStub_GetReturnValue(FM_VerifyOverwrite, bool);
}
