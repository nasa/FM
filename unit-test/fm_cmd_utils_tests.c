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
#include "fm_cmd_utils.h"
#include "fm_app.h"
#include "fm_child.h"
#include "fm_perfids.h"
#include "fm_events.h"

/*
 * UT Assert
 */
#include "fm_test_utils.h"

/*
 * UT includes
 */
#include "uttest.h"
#include "utassert.h"
#include "utstubs.h"

/*********************************************************************************
 *          TEST CASE FUNCTIONS
 *********************************************************************************/

/* **************************
 * VerifyOverwrite Tests
 * *************************/
void Test_FM_VerifyOverwrite(void)
{
    uint32 eventid = 1;

    /* Overwrite true (aka 1) */
    UtAssert_BOOL_TRUE(FM_VerifyOverwrite(1, eventid, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);

    /* Overwrite false (aka 0) */
    UtAssert_BOOL_TRUE(FM_VerifyOverwrite(0, eventid, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);

    /* Overwrite not true or false */
    UtAssert_BOOL_FALSE(FM_VerifyOverwrite(3, eventid, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, eventid);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
}

/* **************************
 * GetOpenFilesData Tests
 * *************************/
void Test_FM_GetOpenFilesData(void)
{
    osal_id_t           id = OS_OBJECT_ID_UNDEFINED;
    FM_OpenFilesEntry_t files_entry;
    OS_task_prop_t      task_prop;
    OS_file_prop_t      file_prop;

    memset(&files_entry, 0, sizeof(files_entry));
    memset(&task_prop, 0, sizeof(task_prop));
    memset(&file_prop, 0, sizeof(file_prop));

    strncpy(file_prop.Path, "FilePath", sizeof(file_prop.Path));
    strncpy(task_prop.name, "AppName", sizeof(task_prop.name));

    /* NULL without any id's for OS_ForEachObject */
    UtAssert_UINT32_EQ(FM_GetOpenFilesData(NULL), 0);

    /* Undefined object id */
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &id, sizeof(id), false);
    UtAssert_UINT32_EQ(FM_GetOpenFilesData(NULL), 0);

    /* NULL with OS_STREAM id */
    OS_OpenCreate(&id, NULL, 0, 0);
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &id, sizeof(id), false);
    UtAssert_UINT32_EQ(FM_GetOpenFilesData(NULL), 1);
    UtAssert_STUB_COUNT(OS_FDGetInfo, 0);

    /* Fail OS_FDGetInfo */
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &id, sizeof(id), false);
    UT_SetDeferredRetcode(UT_KEY(OS_FDGetInfo), 1, !OS_SUCCESS);
    UtAssert_UINT32_EQ(FM_GetOpenFilesData(&files_entry), 1);
    UtAssert_STUB_COUNT(OS_FDGetInfo, 1);

    /* Fail OS_TaskGetInfo */
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &id, sizeof(id), false);
    UT_SetDataBuffer(UT_KEY(OS_FDGetInfo), &file_prop, sizeof(file_prop), false);
    UT_SetDeferredRetcode(UT_KEY(OS_TaskGetInfo), 1, !OS_SUCCESS);
    UtAssert_UINT32_EQ(FM_GetOpenFilesData(&files_entry), 1);
    UtAssert_STUB_COUNT(OS_TaskGetInfo, 1);
    UtAssert_STRINGBUF_EQ(files_entry.LogicalName, sizeof(files_entry.LogicalName), file_prop.Path,
                          sizeof(file_prop.Path));

    /* All pass */
    memset(&files_entry, 0, sizeof(files_entry));
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &id, sizeof(id), false);
    UT_SetDataBuffer(UT_KEY(OS_FDGetInfo), &file_prop, sizeof(file_prop), false);
    UT_SetDataBuffer(UT_KEY(OS_TaskGetInfo), &task_prop, sizeof(task_prop), false);
    UtAssert_UINT32_EQ(FM_GetOpenFilesData(&files_entry), 1);
    UtAssert_STRINGBUF_EQ(files_entry.LogicalName, sizeof(files_entry.LogicalName), file_prop.Path,
                          sizeof(file_prop.Path));
    UtAssert_STRINGBUF_EQ(files_entry.AppName, sizeof(files_entry.AppName), task_prop.name, sizeof(task_prop.name));
}

/* **************************
 * GetFilenameState Tests
 * *************************/
void Test_FM_GetFilenameState(void)
{
    char           filename[OS_MAX_FILE_NAME] = {0};
    os_fstat_t     fstat;
    osal_id_t      id = OS_OBJECT_ID_UNDEFINED;
    OS_file_prop_t file_prop;

    memset(&fstat, 0, sizeof(fstat));
    memset(&file_prop, 0, sizeof(file_prop));

    /* NULL filename */
    UtAssert_UINT32_EQ(FM_GetFilenameState(NULL, 0, false), FM_NAME_IS_INVALID);

    /* Empty string */
    UtAssert_UINT32_EQ(FM_GetFilenameState(filename, 1, false), FM_NAME_IS_INVALID);

    /* Unterminated string */
    strncpy(filename, "File", sizeof(filename));
    UtAssert_UINT32_EQ(FM_GetFilenameState(filename, 1, false), FM_NAME_IS_INVALID);

    /* OS_stat failure, file info false */
    UT_SetDeferredRetcode(UT_KEY(OS_stat), 1, !OS_SUCCESS);
    FM_GlobalData.FileStatSize = 1;
    UtAssert_UINT32_EQ(FM_GetFilenameState(filename, sizeof(filename), false), FM_NAME_IS_NOT_IN_USE);
    UtAssert_UINT32_EQ(FM_GlobalData.FileStatSize, 1);

    /* OS_stat failure, file info true */
    UT_SetDeferredRetcode(UT_KEY(OS_stat), 1, !OS_SUCCESS);
    UtAssert_UINT32_EQ(FM_GetFilenameState(filename, sizeof(filename), true), FM_NAME_IS_NOT_IN_USE);
    UtAssert_UINT32_EQ(FM_GlobalData.FileStatSize, 0);

    /* File is directory, file info true */
    fstat.FileModeBits = OS_FILESTAT_MODE_DIR;
    fstat.FileSize     = 2;
    UT_SetDataBuffer(UT_KEY(OS_stat), &fstat, sizeof(fstat), false);
    UtAssert_UINT32_EQ(FM_GetFilenameState(filename, sizeof(filename), true), FM_NAME_IS_DIRECTORY);
    UtAssert_UINT32_EQ(FM_GlobalData.FileStatSize, 2);

    /* File is file, file info false, no objects */
    UtAssert_UINT32_EQ(FM_GetFilenameState(filename, sizeof(filename), false), FM_NAME_IS_FILE_CLOSED);
    UtAssert_UINT32_EQ(FM_GlobalData.FileStatSize, 2);

    /* File is file, undefined object */
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &id, sizeof(id), false);
    UtAssert_UINT32_EQ(FM_GetFilenameState(filename, sizeof(filename), false), FM_NAME_IS_FILE_CLOSED);

    /* File is file, OS_FDGetInfo fail */
    OS_OpenCreate(&id, NULL, 0, 0);
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &id, sizeof(id), false);
    UT_SetDeferredRetcode(UT_KEY(OS_FDGetInfo), 1, !OS_SUCCESS);
    UtAssert_UINT32_EQ(FM_GetFilenameState(filename, sizeof(filename), false), FM_NAME_IS_FILE_CLOSED);

    /* File is file, strcmp fail */
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &id, sizeof(id), false);
    UtAssert_UINT32_EQ(FM_GetFilenameState(filename, sizeof(filename), false), FM_NAME_IS_FILE_CLOSED);

    /* File is file, strcmp match */
    strncpy(file_prop.Path, filename, sizeof(file_prop.Path));
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &id, sizeof(id), false);
    UT_SetDataBuffer(UT_KEY(OS_FDGetInfo), &file_prop, sizeof(file_prop), false);
    UtAssert_UINT32_EQ(FM_GetFilenameState(filename, sizeof(filename), false), FM_NAME_IS_FILE_OPEN);
}

/* **************************
 * VerifyNameValid Tests
 * *************************/
void Test_FM_VerifyNameValid(void)
{
    char   filename[OS_MAX_FILE_NAME] = "Filename";
    uint32 eventid                    = 1;

    /* Filename not in use */
    UT_SetDeferredRetcode(UT_KEY(OS_stat), 1, !OS_SUCCESS);
    UtAssert_UINT32_EQ(FM_VerifyNameValid(filename, sizeof(filename), 0, NULL), FM_NAME_IS_NOT_IN_USE);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);

    /* Invalid filename */
    UtAssert_UINT32_EQ(FM_VerifyNameValid(filename, 1, eventid, "Cmd text"), FM_NAME_IS_INVALID);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, eventid);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
}

/* **************************
 * FM_VerifyFileState Tests
 * *************************/
void Test_FM_VerifyFileState(void)
{
    char           filename[OS_MAX_FILE_NAME] = "Filename";
    osal_id_t      id                         = OS_OBJECT_ID_UNDEFINED;
    OS_file_prop_t file_prop;

    memset(&file_prop, 0, sizeof(file_prop));
    strncpy(file_prop.Path, filename, sizeof(file_prop.Path));
    OS_OpenCreate(&id, NULL, 0, 0);

    /* FM_NAME_IS_CLOSED */
    UtAssert_BOOL_TRUE(FM_VerifyFileState(FM_FILE_CLOSED, filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);

    /* FM_NAME_IS_OPEN */
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &id, sizeof(id), false);
    UT_SetDataBuffer(UT_KEY(OS_FDGetInfo), &file_prop, sizeof(file_prop), false);
    UtAssert_BOOL_FALSE(FM_VerifyFileState(FM_FILE_CLOSED, filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_FNAME_ISOPEN_EID_OFFSET);
}

/* **************************
 * VerifyFileClosed Tests
 * *************************/
void Test_FM_VerifyFileClosed(void)
{
    char           filename[OS_MAX_FILE_NAME] = "Filename";
    os_fstat_t     fstat                      = {.FileModeBits = OS_FILESTAT_MODE_DIR};
    osal_id_t      id                         = OS_OBJECT_ID_UNDEFINED;
    OS_file_prop_t file_prop;

    memset(&file_prop, 0, sizeof(file_prop));
    strncpy(file_prop.Path, filename, sizeof(file_prop.Path));
    OS_OpenCreate(&id, NULL, 0, 0);

    /* FM_NAME_IS_NOT_IN_USE */
    UT_SetDeferredRetcode(UT_KEY(OS_stat), 1, !OS_SUCCESS);
    UtAssert_BOOL_FALSE(FM_VerifyFileClosed(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_FNAME_DNE_EID_OFFSET);

    /* FM_NAME_IS_DIRECTORY */
    UT_SetDataBuffer(UT_KEY(OS_stat), &fstat, sizeof(fstat), false);
    UtAssert_BOOL_FALSE(FM_VerifyFileClosed(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_FNAME_ISDIR_EID_OFFSET);

    /* FM_NAME_IS_CLOSED */
    UtAssert_BOOL_TRUE(FM_VerifyFileClosed(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);

    /* FM_NAME_IS_OPEN */
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &id, sizeof(id), false);
    UT_SetDataBuffer(UT_KEY(OS_FDGetInfo), &file_prop, sizeof(file_prop), false);
    UtAssert_BOOL_FALSE(FM_VerifyFileClosed(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 3);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[2].EventID, FM_FNAME_ISOPEN_EID_OFFSET);

    /* FM_NAME_IS_INVALID */
    UtAssert_BOOL_FALSE(FM_VerifyFileClosed(filename, 1, 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 4);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[3].EventID, FM_FNAME_INVALID_EID_OFFSET);
}

/* **************************
 * VerifyFileExists Tests
 * *************************/
void Test_FM_VerifyFileExists(void)
{
    char           filename[OS_MAX_FILE_NAME] = "Filename";
    os_fstat_t     fstat                      = {.FileModeBits = OS_FILESTAT_MODE_DIR};
    osal_id_t      id                         = OS_OBJECT_ID_UNDEFINED;
    OS_file_prop_t file_prop;

    memset(&file_prop, 0, sizeof(file_prop));
    strncpy(file_prop.Path, filename, sizeof(file_prop.Path));
    OS_OpenCreate(&id, NULL, 0, 0);

    /* FM_NAME_IS_NOT_IN_USE */
    UT_SetDeferredRetcode(UT_KEY(OS_stat), 1, !OS_SUCCESS);
    UtAssert_BOOL_FALSE(FM_VerifyFileExists(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_FNAME_DNE_EID_OFFSET);

    /* FM_NAME_IS_DIRECTORY */
    UT_SetDataBuffer(UT_KEY(OS_stat), &fstat, sizeof(fstat), false);
    UtAssert_BOOL_FALSE(FM_VerifyFileExists(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_FNAME_ISDIR_EID_OFFSET);

    /* FM_NAME_IS_CLOSED */
    UtAssert_BOOL_TRUE(FM_VerifyFileExists(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);

    /* FM_NAME_IS_OPEN */
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &id, sizeof(id), false);
    UT_SetDataBuffer(UT_KEY(OS_FDGetInfo), &file_prop, sizeof(file_prop), false);
    UtAssert_BOOL_TRUE(FM_VerifyFileExists(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);

    /* FM_NAME_IS_INVALID */
    UtAssert_BOOL_FALSE(FM_VerifyFileExists(filename, 1, 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 3);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[2].EventID, FM_FNAME_INVALID_EID_OFFSET);
}

/* **************************
 * VerifyFileNoExist Tests
 * *************************/
void Test_FM_VerifyFileNoExist(void)
{
    char           filename[OS_MAX_FILE_NAME] = "Filename";
    os_fstat_t     fstat                      = {.FileModeBits = OS_FILESTAT_MODE_DIR};
    osal_id_t      id                         = OS_OBJECT_ID_UNDEFINED;
    OS_file_prop_t file_prop;

    memset(&file_prop, 0, sizeof(file_prop));
    strncpy(file_prop.Path, filename, sizeof(file_prop.Path));
    OS_OpenCreate(&id, NULL, 0, 0);

    /* FM_NAME_IS_NOT_IN_USE */
    UT_SetDeferredRetcode(UT_KEY(OS_stat), 1, !OS_SUCCESS);
    UtAssert_BOOL_TRUE(FM_VerifyFileNoExist(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);

    /* FM_NAME_IS_DIRECTORY */
    UT_SetDataBuffer(UT_KEY(OS_stat), &fstat, sizeof(fstat), false);
    UtAssert_BOOL_FALSE(FM_VerifyFileNoExist(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_FNAME_ISDIR_EID_OFFSET);

    /* FM_NAME_IS_CLOSED */
    UtAssert_BOOL_FALSE(FM_VerifyFileNoExist(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_FNAME_EXIST_EID_OFFSET);

    /* FM_NAME_IS_OPEN */
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &id, sizeof(id), false);
    UT_SetDataBuffer(UT_KEY(OS_FDGetInfo), &file_prop, sizeof(file_prop), false);
    UtAssert_BOOL_FALSE(FM_VerifyFileNoExist(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 3);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[2].EventID, FM_FNAME_EXIST_EID_OFFSET);

    /* FM_NAME_IS_INVALID */
    UtAssert_BOOL_FALSE(FM_VerifyFileNoExist(filename, 1, 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 4);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[3].EventID, FM_FNAME_INVALID_EID_OFFSET);
}

/* **************************
 * VerifyFileNotOpen Tests
 * *************************/
void Test_FM_VerifyFileNotOpen(void)
{
    char           filename[OS_MAX_FILE_NAME] = "Filename";
    os_fstat_t     fstat                      = {.FileModeBits = OS_FILESTAT_MODE_DIR};
    osal_id_t      id                         = OS_OBJECT_ID_UNDEFINED;
    OS_file_prop_t file_prop;

    memset(&file_prop, 0, sizeof(file_prop));
    strncpy(file_prop.Path, filename, sizeof(file_prop.Path));
    OS_OpenCreate(&id, NULL, 0, 0);

    /* FM_NAME_IS_NOT_IN_USE */
    UT_SetDeferredRetcode(UT_KEY(OS_stat), 1, !OS_SUCCESS);
    UtAssert_BOOL_TRUE(FM_VerifyFileNotOpen(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);

    /* FM_NAME_IS_DIRECTORY */
    UT_SetDataBuffer(UT_KEY(OS_stat), &fstat, sizeof(fstat), false);
    UtAssert_BOOL_FALSE(FM_VerifyFileNotOpen(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_FNAME_ISDIR_EID_OFFSET);

    /* FM_NAME_IS_CLOSED */
    UtAssert_BOOL_TRUE(FM_VerifyFileNotOpen(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);

    /* FM_NAME_IS_OPEN */
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &id, sizeof(id), false);
    UT_SetDataBuffer(UT_KEY(OS_FDGetInfo), &file_prop, sizeof(file_prop), false);
    UtAssert_BOOL_FALSE(FM_VerifyFileNotOpen(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_FNAME_ISOPEN_EID_OFFSET);

    /* FM_NAME_IS_INVALID */
    UtAssert_BOOL_FALSE(FM_VerifyFileNotOpen(filename, 1, 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 3);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[2].EventID, FM_FNAME_INVALID_EID_OFFSET);
}

/* **************************
 * VerifyDirExist Tests
 * *************************/
void Test_FM_VerifyDirExists(void)
{
    char           filename[OS_MAX_FILE_NAME] = "Filename";
    os_fstat_t     fstat                      = {.FileModeBits = OS_FILESTAT_MODE_DIR};
    osal_id_t      id                         = OS_OBJECT_ID_UNDEFINED;
    OS_file_prop_t file_prop;

    memset(&file_prop, 0, sizeof(file_prop));
    strncpy(file_prop.Path, filename, sizeof(file_prop.Path));
    OS_OpenCreate(&id, NULL, 0, 0);

    /* FM_NAME_IS_NOT_IN_USE */
    UT_SetDeferredRetcode(UT_KEY(OS_stat), 1, !OS_SUCCESS);
    UtAssert_BOOL_FALSE(FM_VerifyDirExists(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_FNAME_DNE_EID_OFFSET);

    /* FM_NAME_IS_DIRECTORY */
    UT_SetDataBuffer(UT_KEY(OS_stat), &fstat, sizeof(fstat), false);
    UtAssert_BOOL_TRUE(FM_VerifyDirExists(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);

    /* FM_NAME_IS_CLOSED */
    UtAssert_BOOL_FALSE(FM_VerifyDirExists(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_FNAME_ISFILE_EID_OFFSET);

    /* FM_NAME_IS_OPEN */
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &id, sizeof(id), false);
    UT_SetDataBuffer(UT_KEY(OS_FDGetInfo), &file_prop, sizeof(file_prop), false);
    UtAssert_BOOL_FALSE(FM_VerifyDirExists(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 3);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[2].EventID, FM_FNAME_ISFILE_EID_OFFSET);

    /* FM_NAME_IS_INVALID */
    UtAssert_BOOL_FALSE(FM_VerifyDirExists(filename, 1, 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 4);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[3].EventID, FM_FNAME_INVALID_EID_OFFSET);
}

/* **************************
 * VerifyDirNoExist Tests
 * *************************/
void Test_FM_VerifyDirNoExist(void)
{
    char           filename[OS_MAX_FILE_NAME] = "Filename";
    os_fstat_t     fstat                      = {.FileModeBits = OS_FILESTAT_MODE_DIR};
    osal_id_t      id                         = OS_OBJECT_ID_UNDEFINED;
    OS_file_prop_t file_prop;

    memset(&file_prop, 0, sizeof(file_prop));
    strncpy(file_prop.Path, filename, sizeof(file_prop.Path));
    OS_OpenCreate(&id, NULL, 0, 0);

    /* FM_NAME_IS_NOT_IN_USE */
    UT_SetDeferredRetcode(UT_KEY(OS_stat), 1, !OS_SUCCESS);
    UtAssert_BOOL_TRUE(FM_VerifyDirNoExist(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);

    /* FM_NAME_IS_DIRECTORY */
    UT_SetDataBuffer(UT_KEY(OS_stat), &fstat, sizeof(fstat), false);
    UtAssert_BOOL_FALSE(FM_VerifyDirNoExist(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_FNAME_ISDIR_EID_OFFSET);

    /* FM_NAME_IS_CLOSED */
    UtAssert_BOOL_FALSE(FM_VerifyDirNoExist(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_FNAME_DNE_EID_OFFSET);

    /* FM_NAME_IS_OPEN */
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &id, sizeof(id), false);
    UT_SetDataBuffer(UT_KEY(OS_FDGetInfo), &file_prop, sizeof(file_prop), false);
    UtAssert_BOOL_FALSE(FM_VerifyDirNoExist(filename, sizeof(filename), 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 3);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[2].EventID, FM_FNAME_DNE_EID_OFFSET);

    /* FM_NAME_IS_INVALID */
    UtAssert_BOOL_FALSE(FM_VerifyDirNoExist(filename, 1, 0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 4);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[3].EventID, FM_FNAME_INVALID_EID_OFFSET);
}

/* **********************
 * VerifyChildTask tests
 * *********************/
void Test_FM_VerifyChildTask(void)
{
    /* ChildSemaphore not defined */
    UtAssert_BOOL_FALSE(FM_VerifyChildTask(0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CHILD_DISABLED_EID_OFFSET);

    /* LocalQueueCount equal to FM_CHILD_QUEUE_DEPTH */
    FM_GlobalData.ChildSemaphore  = FM_UT_OBJID_1;
    FM_GlobalData.ChildQueueCount = FM_CHILD_QUEUE_DEPTH;
    UtAssert_BOOL_FALSE(FM_VerifyChildTask(0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_CHILD_Q_FULL_EID_OFFSET);

    /* LocalQueueCount greater than FM_CHILD_QUEUE_DEPTH */
    FM_GlobalData.ChildQueueCount = FM_CHILD_QUEUE_DEPTH + 1;
    UtAssert_BOOL_FALSE(FM_VerifyChildTask(0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 3);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[2].EventID, FM_CHILD_BROKEN_EID_OFFSET);

    /* ChildWriteIndex equal to FM_CHILD_QUEUE_DEPTH */
    FM_GlobalData.ChildQueueCount = FM_CHILD_QUEUE_DEPTH - 1;
    FM_GlobalData.ChildWriteIndex = FM_CHILD_QUEUE_DEPTH;
    UtAssert_BOOL_FALSE(FM_VerifyChildTask(0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 4);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[3].EventID, FM_CHILD_BROKEN_EID_OFFSET);

    /* Success */
    FM_GlobalData.ChildWriteIndex = FM_CHILD_QUEUE_DEPTH - 1;
    UtAssert_BOOL_TRUE(FM_VerifyChildTask(0, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 4);
}

/* **********************
 * InvokeChildTask tests
 * *********************/
void Test_FM_InvokeChildTask(void)
{
    /* Conditions true */
    FM_GlobalData.ChildWriteIndex = FM_CHILD_QUEUE_DEPTH - 1;
    FM_GlobalData.ChildSemaphore  = FM_UT_OBJID_1;
    UtAssert_VOIDCALL(FM_InvokeChildTask());
    UtAssert_INT32_EQ(FM_GlobalData.ChildWriteIndex, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueueCount, 1);
    UtAssert_STUB_COUNT(OS_CountSemGive, 1);

    /* Conditions false */
    FM_GlobalData.ChildSemaphore = OS_OBJECT_ID_UNDEFINED;
    UtAssert_VOIDCALL(FM_InvokeChildTask());
    UtAssert_INT32_EQ(FM_GlobalData.ChildWriteIndex, 1);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueueCount, 2);
    UtAssert_STUB_COUNT(OS_CountSemGive, 1);
}

/* **********************
 * AppendPathSep Tests
 * *********************/
void Test_FM_AppendPathSep(void)
{
    char directory[3] = "";

    /* Empty directory */
    UtAssert_VOIDCALL(FM_AppendPathSep(directory, sizeof(directory)));
    UtAssert_UINT32_EQ(directory[0], 0);

    /* Ends with directory separator */
    strncpy(directory, "/", sizeof(directory));
    UtAssert_VOIDCALL(FM_AppendPathSep(directory, sizeof(directory)));
    UtAssert_UINT32_EQ(strncmp(directory, "/", sizeof(directory)), 0);

    /* No room */
    strncpy(directory, "a", sizeof(directory));
    UtAssert_VOIDCALL(FM_AppendPathSep(directory, sizeof(directory) - 1));
    UtAssert_UINT32_EQ(strncmp(directory, "a", sizeof(directory)), 0);

    UtAssert_VOIDCALL(FM_AppendPathSep(directory, sizeof(directory)));
    UtAssert_UINT32_EQ(strncmp(directory, "a/", sizeof(directory)), 0);
}

void Test_FM_GetVolumeFreeSpace(void)
{
    /*
     * Test case for:
     * int32 FM_GetVolumeFreeSpace(const char *FileSys, uint64 *BlockCount, uint64 *ByteCount)
     */
    uint64       bytes;
    uint64       blocks;
    OS_statvfs_t statbuf;

    statbuf.block_size   = 44;
    statbuf.blocks_free  = 55;
    statbuf.total_blocks = 66;

    UT_SetDataBuffer(UT_KEY(OS_FileSysStatVolume), &statbuf, sizeof(statbuf), false);

    /* Nominal */
    UtAssert_INT32_EQ(FM_GetVolumeFreeSpace("test", &blocks, &bytes), CFE_SUCCESS);

    UtAssert_UINT32_EQ(blocks, statbuf.blocks_free);
    UtAssert_UINT32_EQ(bytes, statbuf.block_size * statbuf.blocks_free);

    /* Failure in OS_FileSysStatVolume */
    UT_SetDefaultReturnValue(UT_KEY(OS_FileSysStatVolume), OS_ERROR);
    UtAssert_INT32_EQ(FM_GetVolumeFreeSpace("test", &blocks, &bytes), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_OS_SYS_STAT_ERR_EID);
}

void Test_FM_GetDirectorySpaceEstimate(void)
{
    /*
     * Test case for:
     * int32 FM_GetDirectorySpaceEstimate(const char *Directory, uint64 *BlockCount, uint64 *ByteCount)
     */

    uint64      bytes;
    uint64      blocks;
    os_dirent_t direntry = {.FileName = "f1"};
    os_fstat_t  fstat;
    char        longname[OS_MAX_PATH_LEN + 5];

    memset(&fstat, 0, sizeof(fstat));

    /* Nominal */
    fstat.FileSize = 1234;
    blocks         = 0;
    bytes          = 0;
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, OS_ERROR);
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);
    UT_SetDataBuffer(UT_KEY(OS_stat), &fstat, sizeof(fstat), false);

    UtAssert_INT32_EQ(FM_GetDirectorySpaceEstimate("test", &blocks, &bytes), CFE_SUCCESS);
    UtAssert_ZERO(blocks); /* not reported via OS_stat, so left unchanged */
    UtAssert_UINT32_EQ(bytes, fstat.FileSize);

    /* Skip subdirectories */
    fstat.FileModeBits = OS_FILESTAT_MODE_DIR;
    blocks             = 0;
    bytes              = 0;
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, OS_ERROR);
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);
    UT_SetDataBuffer(UT_KEY(OS_stat), &fstat, sizeof(fstat), false);

    UtAssert_INT32_EQ(FM_GetDirectorySpaceEstimate("test", &blocks, &bytes), CFE_SUCCESS);
    UtAssert_ZERO(blocks);
    UtAssert_ZERO(bytes);

    /* OS_stat failed for single file (this still returns success overall, but generates an event) */
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, OS_ERROR);
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat), OS_ERROR);
    UtAssert_INT32_EQ(FM_GetDirectorySpaceEstimate("test", &blocks, &bytes), CFE_SUCCESS);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DIRECTORY_ESTIMATE_ERR_EID);

    /* Directory name string too long (should not cause segfault) */
    UT_ResetState(UT_KEY(OS_DirectoryRead));
    UT_ResetState(UT_KEY(OS_DirectoryOpen));
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_ERROR);
    memset(longname, 'a', sizeof(longname) - 1);
    longname[sizeof(longname) - 1] = 0;
    UtAssert_INT32_EQ(FM_GetDirectorySpaceEstimate(longname, &blocks, &bytes), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_DIRECTORY_ESTIMATE_ERR_EID);
    UtAssert_STUB_COUNT(OS_DirectoryOpen, 0);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 0);

    /* OS_DirectoryOpen failed */
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_ERROR);
    UtAssert_INT32_EQ(FM_GetDirectorySpaceEstimate("test", &blocks, &bytes), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 3);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[2].EventID, FM_DIRECTORY_ESTIMATE_ERR_EID);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 0);
}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    UtTest_Add(Test_FM_VerifyOverwrite, FM_Test_Setup, FM_Test_Teardown, "Test_FM_VerifyOverwrite");
    UtTest_Add(Test_FM_GetOpenFilesData, FM_Test_Setup, FM_Test_Teardown, "Test_FM_GetOpenFilesData");
    UtTest_Add(Test_FM_GetFilenameState, FM_Test_Setup, FM_Test_Teardown, "Test_FM_GetFilenameState");
    UtTest_Add(Test_FM_VerifyNameValid, FM_Test_Setup, FM_Test_Teardown, "Test_FM_VerifyNameValid");
    UtTest_Add(Test_FM_VerifyFileState, FM_Test_Setup, FM_Test_Teardown, "Test_FM_VerifyFileState");
    UtTest_Add(Test_FM_VerifyFileClosed, FM_Test_Setup, FM_Test_Teardown, "Test_FM_VerifyFileClosed");
    UtTest_Add(Test_FM_VerifyFileExists, FM_Test_Setup, FM_Test_Teardown, "Test_FM_VerifyFileExists");
    UtTest_Add(Test_FM_VerifyFileNoExist, FM_Test_Setup, FM_Test_Teardown, "Test_FM_VerifyFileNoExist");
    UtTest_Add(Test_FM_VerifyFileNotOpen, FM_Test_Setup, FM_Test_Teardown, "Test_FM_VerifyFileNotOpen");
    UtTest_Add(Test_FM_VerifyDirExists, FM_Test_Setup, FM_Test_Teardown, "Test_FM_VerifyDirExists");
    UtTest_Add(Test_FM_VerifyDirNoExist, FM_Test_Setup, FM_Test_Teardown, "Test_FM_VerifyDirNoExist");
    UtTest_Add(Test_FM_VerifyChildTask, FM_Test_Setup, FM_Test_Teardown, "Test_FM_VerifyChildTask");
    UtTest_Add(Test_FM_InvokeChildTask, FM_Test_Setup, FM_Test_Teardown, "Test_FM_InvokeChildTask");
    UtTest_Add(Test_FM_AppendPathSep, FM_Test_Setup, FM_Test_Teardown, "Test_FM_AppendPathSep");
    UtTest_Add(Test_FM_GetVolumeFreeSpace, FM_Test_Setup, FM_Test_Teardown, "Test_FM_GetVolumeFreeSpace");
    UtTest_Add(Test_FM_GetDirectorySpaceEstimate, FM_Test_Setup, FM_Test_Teardown, "Test_FM_GetDirectorySpaceEstimate");
}
