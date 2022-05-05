/*
** $Id: fm_child.c 1.16.1.5 2017/01/24 23:53:32EST mdeschu Exp  $
**
**  Copyright (c) 2007-2014 United States Government as represented by the 
**  Administrator of the National Aeronautics and Space Administration. 
**  All Other Rights Reserved.  
**
**  This software was created at NASA's Goddard Space Flight Center.
**  This software is governed by the NASA Open Source Agreement and may be 
**  used, distributed and modified only pursuant to the terms of that 
**  agreement.
**
** Purpose: File Manager (FM) Child task (low priority command handler)
**
** Author: Scott Walling (Microtel)
**
** Notes:
**
*/

#include "cfe.h"
#include "fm_cmd_utils.h"
#include "fm_msg.h"
#include "fm_child.h"
#include "fm_perfids.h"
#include "fm_events.h"

/*
 * UT Assert
 */
#include "fm_test_utils.h"
#include "cfs_utils.h"

/*
 * UT includes
 */
#include "uttest.h"
#include "utassert.h"
#include "utstubs.h"

/*
**********************************************************************************
**          TEST CASE FUNCTIONS
**********************************************************************************
*/
/* ****************
 * IsValidCmdPktLength Tests
 * ***************/
void Test_FM_IsValidCmdPktLength_ActualLengthSuccess(void)
{
    // Arrange
    CFE_MSG_Message_t *dummy_msgptr = NULL;
    size_t dummy_length = UT_Utils_Any_uint8();
    size_t expected_length = dummy_length;
    uint32 dummy_eventid = 1;
    const char *CmdText = "Text";

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize),&dummy_length,sizeof(dummy_length),false);

    // Act
    bool function_result = FM_IsValidCmdPktLength(dummy_msgptr, expected_length,dummy_eventid, CmdText);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(function_result, true);
}

void Test_FM_IsValidCmdPktLength_ActualLengthNotExpected(void)
{
    // Arrange
    CFE_MSG_Message_t *dummy_msgptr = NULL;
    size_t dummy_length = UT_Utils_Any_uint8();
    size_t expected_length = dummy_length+1;
    uint32 dummy_eventid = UT_Utils_Any_uint32();
    const char *CmdText = "dummy_text";
    
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &dummy_length, sizeof(dummy_length), false);

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    // Act
    bool function_result = FM_IsValidCmdPktLength(dummy_msgptr, expected_length, dummy_eventid, CmdText);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, (uint16) dummy_eventid);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(function_result, false);
}


/* **************************
 * VerifyOverwrite Tests
 * *************************/
void Test_FM_VerifyOverwrite_OverwriteIsNeitherFalseNorTrue(void)
{
    // Arrange
    uint16 dummy_overwrite = 3;
    uint32 eventid = 1;
    const char *dummy_text = "Text";

    // Act
    bool function_result = FM_VerifyOverwrite(dummy_overwrite, eventid, dummy_text);

    uint8 call_count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    // Assert
    UtAssert_True(call_count_SendEvent == 1, "CFE_EVS_SendEvent was called %u time(s) and hsould be 1", call_count_SendEvent);
    UtAssert_True(function_result == false, "FM_VerifyOverwrite returned %s and should be false", function_result ? "true":"false");
}

void Test_FM_VerifyOverwrite_OverwriteIsTrue(void)
{
    // Arrange
    uint16 dummy_overwrite = 1;
    uint32 eventid = 1;
    const char *dummy_text = "Text";

    // Act
    bool function_result = FM_VerifyOverwrite(dummy_overwrite, eventid, dummy_text);

    uint8 call_count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    // Assert
    UtAssert_True(call_count_SendEvent == 0 , "CFE_EVS_SendEvent was called %u time(s) and hsould be 0", call_count_SendEvent);
    UtAssert_True(function_result == true, "FM_VerifyOverwrite returned %s and should be true", function_result ? "true":"false");
}

void Test_FM_VerifyOverwrite_OverwriteIsFalse(void)
{
    // Arrange
    uint16 dummy_overwrite = 0;
    uint32 eventid = 1;
    const char *dummy_text = "Text";

    // Act
    bool function_result = FM_VerifyOverwrite(dummy_overwrite, eventid, dummy_text);

    uint8 call_count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    // Assert
    UtAssert_True(call_count_SendEvent == 0 , "CFE_EVS_SendEvent was called %u time(s) and hsould be 0", call_count_SendEvent);
    UtAssert_True(function_result == true, "FM_VerifyOverwrite returned %s and should be true", function_result ? "true":"false");
}

/* **************************
 * LoadOpenFileData
 * *************************/
/*
void Test_LoadOpenFileData_OpenFileCountUnchanged(void)
{
    // Arrange
    uint32 dummy_id = 333;
    void * dummy_args = NULL;

    uint32 count_openfile_before = OpenFileCount;

    // Act
    LoadOpenFileData(dummy_id, dummy_args);

    uint32 count_openfile_after = OpenFileCount;

    // Assert
    UtAssert_True(count_openfile_after == count_openfile_before,
		    "Openfile value is %u and should be %u",count_openfile_after,count_openfile_before);
}

void Test_LoadOpenFileData_OpenFileCountIncrease(void)
{
    // Arrange
    uint32 dummy_id = 333;
    void * dummy_args = NULL;
    
    UT_SetDefaultReturnValue(UT_KEY(OS_IdentifyObject),OS_OBJECT_TYPE_OS_STREAM);
    uint32 count_openfile_before = OpenFileCount;

    // Act
    LoadOpenFileData(dummy_id, dummy_args);

    uint32 count_openfile_after = OpenFileCount;

    // Assert
    UtAssert_True(count_openfile_after != count_openfile_before,
		    "Openfile value is %u and should be %u", count_openfile_after, count_openfile_before +1);
}

void Test_LoadOpenFileData_OpenFilesDataNotNull(void)
{
    // Arrange
    uint32 dummy_id = 333;
    FM_OpenFilesEntry_t dummy_arg = {.LogicalName="LogicName", .AppName="AppName"};
    FM_OpenFilesEntry_t*dummy_ptr_arg = &dummy_arg;
    
    UT_SetDefaultReturnValue(UT_KEY(OS_IdentifyObject),OS_OBJECT_TYPE_OS_STREAM);
    UT_SetDefaultReturnValue(UT_KEY(OS_FDGetInfo),!OS_SUCCESS);

    uint32 count_openfile_before = OpenFileCount;

    // Act
    LoadOpenFileData(dummy_id, dummy_ptr_arg);

    uint32 count_openfile_after = OpenFileCount;
    uint8 call_count_FDGetInfo = UT_GetStubCount(UT_KEY(OS_FDGetInfo));
    uint8 call_count_GetTaskInfo = UT_GetStubCount(UT_KEY(CFE_ES_GetTaskInfo));


    // Assert
    UtAssert_True(count_openfile_before != count_openfile_after,
		    "OpenFile value is %u and should not be equal to %u",count_openfile_after,count_openfile_before);
    UtAssert_True(call_count_FDGetInfo == 1, "OS_FDGetInfo was called %u time(s) and should be 1",call_count_FDGetInfo);
    UtAssert_True(call_count_GetTaskInfo == 0, "CFE_ES_GetTaskInfo was called %u time(s) and should be 0", call_count_GetTaskInfo);
}

void Test_LoadOpenFileData_FDGetInfoSuccess(void)
{
    // Arrange
    OpenFileCount = 0;
    uint32 dummy_id = 333;
    FM_OpenFilesEntry_t dummy_arg = {.LogicalName="LogicName", .AppName="AppName"};
    FM_OpenFilesEntry_t*dummy_ptr_arg = &dummy_arg;
    OS_file_prop_t dummy_fdprop = {.Path="CheckMessage",.User=30000};

    UT_SetDefaultReturnValue(UT_KEY(OS_IdentifyObject),OS_OBJECT_TYPE_OS_STREAM);
    UT_SetDefaultReturnValue(UT_KEY(OS_FDGetInfo),OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_GetTaskInfo),!CFE_SUCCESS);

    UT_SetDataBuffer(UT_KEY(OS_FDGetInfo),&dummy_fdprop,sizeof(dummy_fdprop),false);

    uint32 count_openfile_before = OpenFileCount;

    // Act
    LoadOpenFileData(dummy_id, dummy_ptr_arg);

    uint32 count_openfile_after = OpenFileCount;
    uint8 call_count_FDGetInfo = UT_GetStubCount(UT_KEY(OS_FDGetInfo));
    uint8 call_count_GetTaskInfo = UT_GetStubCount(UT_KEY(CFE_ES_GetTaskInfo));

    // Assert
    UtAssert_True(count_openfile_before != count_openfile_after,
                    "OpenFile value is %u and should not be equal to %u",count_openfile_after,count_openfile_before);
    UtAssert_True(call_count_FDGetInfo == 1, "OS_FDGetInfo was called %u time(s) and should be 1",call_count_FDGetInfo);
    UtAssert_True(call_count_GetTaskInfo == 1, "CFE_ES_GetTaskInfo was called %u time(s) and should be 1", call_count_GetTaskInfo);
    UtAssert_True(strcmp(dummy_fdprop.Path,dummy_arg.LogicalName) == 0,"The Logical filename is %s and should be %s"
		    ,dummy_arg.LogicalName,dummy_fdprop.Path);
}

void Test_LoadOpenFileData_GetTaskInfoReturnsSuccess(void)
{
    // Arrange
    OpenFileCount = 0;
    uint32 dummy_id = 333;
    FM_OpenFilesEntry_t dummy_arg = {.LogicalName="LogicName", .AppName="AppName"};
    FM_OpenFilesEntry_t*dummy_ptr_arg = &dummy_arg;

    OS_file_prop_t dummy_fdprop = {.Path="CheckMessageFdProp",.User=30000};
    CFE_ES_TaskInfo_t dummy_taskinfo = {.AppName="CheckMes"};

    UT_SetDataBuffer(UT_KEY(OS_FDGetInfo),&dummy_fdprop,sizeof(dummy_fdprop),false);
    UT_SetDataBuffer(UT_KEY(CFE_ES_GetTaskInfo),&dummy_taskinfo,sizeof(dummy_taskinfo),false);

    UT_SetDefaultReturnValue(UT_KEY(OS_IdentifyObject),OS_OBJECT_TYPE_OS_STREAM);
    UT_SetDefaultReturnValue(UT_KEY(OS_FDGetInfo),OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_GetTaskInfo),CFE_SUCCESS);
    
    uint32 count_openfile_before = OpenFileCount;

    // Act
    LoadOpenFileData(dummy_id, dummy_ptr_arg);

    uint32 count_openfile_after = OpenFileCount;
    uint8 call_count_FDGetInfo = UT_GetStubCount(UT_KEY(OS_FDGetInfo));
    uint8 call_count_GetTaskInfo = UT_GetStubCount(UT_KEY(CFE_ES_GetTaskInfo));

    // Assert
    UtAssert_True(count_openfile_before != count_openfile_after,
                    "OpenFile value is %u and should not be equal to %u",count_openfile_after,count_openfile_before);
    UtAssert_True(call_count_FDGetInfo == 1, "OS_FDGetInfo was called %u time(s) and should be 1",call_count_FDGetInfo);
    UtAssert_True(call_count_GetTaskInfo == 1, "CFE_ES_GetTaskInfo was called %u time(s) and should be 1", call_count_GetTaskInfo);
    UtAssert_True(strcmp(dummy_fdprop.Path,dummy_arg.LogicalName) == 0,"The Logical filename is %s and should be %s",
		    dummy_arg.LogicalName,dummy_fdprop.Path);
    UtAssert_True(strcmp(dummy_taskinfo.AppName,dummy_arg.AppName) == 0,"The application name is %s and should be %s",
		    dummy_arg.AppName,dummy_taskinfo.AppName);
}
*/

/* **************************
 * GetOpenFilesData Tests
 * *************************/
void Test_FM_GetOpenFilesData_OSForEachObjectCalled(void)
{
    // Arrange
    FM_OpenFilesEntry_t *dummy_files_data = NULL;

    // Act
    uint32 open_file_count = FM_GetOpenFilesData(dummy_files_data);

    // Assert
    UtAssert_True(open_file_count == 0, "OpenFileCount is %u and should be 0", open_file_count);
}

/* **************************
 * GetFilenameState Tests
 * *************************/
void Test_FM_GetFilenameState_FileNameIsNull(void)
{
    // Arrange
    char *dummy_filename = NULL;
    uint32 dummy_bufsize = 32;
    bool dummy_fileinfo = false;

    // Act
    uint32 filename_state = FM_GetFilenameState(dummy_filename,dummy_bufsize,dummy_fileinfo);

    // Assert
    UtAssert_True(filename_state == FM_NAME_IS_INVALID, "The FilenameState returned is %u and should be %u",
		   filename_state, FM_NAME_IS_INVALID);
}

void Test_FM_GetFilenameState_FileNameNotNullButNotValid(void)
{
    // Arrange
    uint32 dummy_bufsize = 16;
    char dummy_filename[16] = "FileName_Test";
    bool dummy_fileinfo = false;

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),FM_NAME_IS_INVALID);

    // Act
    uint32 filename_state = FM_GetFilenameState(dummy_filename, dummy_bufsize, dummy_fileinfo);

    // Assert
    UtAssert_True(filename_state == FM_NAME_IS_INVALID, "The FilenameState returned is %u and should be %u",
		    filename_state, FM_NAME_IS_INVALID);
}

void Test_FM_GetFilenameState_StringLengthZero(void)
{
    // Arrange
    uint32 dummy_bufsize = 16;
    char dummy_filename[16] = "";
    bool dummy_fileinfo = false;

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename), false);

    // Act
    uint32 filename_state = FM_GetFilenameState(dummy_filename, dummy_bufsize, dummy_fileinfo);

    // Assert
    UtAssert_INT32_EQ(filename_state, FM_NAME_IS_INVALID);
    UtAssert_INT32_EQ(UT_GetStubCount(UT_KEY(CFS_IsValidFilename)), 0);
}

void Test_FM_GetFilenameState_FileNameIsValidButNotInUse_FileInfoCmdFalse(void)
{
    // Arrange
    uint32 dummy_bufsize = 16;
    char dummy_filename[16] = "FileName_Test";
    bool dummy_fileinfo = false;
    //os_fstat_t dummy_fstat;

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),999);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),!OS_SUCCESS);
    //UT_SetDataBuffer(UT_KEY(OS_stat),&dummy_fstat,sizeof(dummy_fstat),false);

    // Act
    uint32 filename_state = FM_GetFilenameState(dummy_filename, dummy_bufsize, dummy_fileinfo);

    // Assert
    UtAssert_True(filename_state == FM_NAME_IS_NOT_IN_USE, "The FilenameState returned is %u and should be %u",
                    filename_state, FM_NAME_IS_NOT_IN_USE);
}

void Test_FM_GetFilenameState_FileNameIsValidButNOtINUse_FileINfoCmdTrue(void)
{
    // Arrange
    uint32 dummy_bufsize = 16;
    char dummy_filename[16] = "FileName_Test";
    bool dummy_fileinfo = true;
    //os_fstat_t dummy_fstat;

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),999);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),!OS_SUCCESS);
    

    // Act
    uint32 filename_state = FM_GetFilenameState(dummy_filename, dummy_bufsize, dummy_fileinfo);

    // Assert
    UtAssert_True(filename_state == FM_NAME_IS_NOT_IN_USE, "The FilenameState returned is %u and should be %u",
                    filename_state, FM_NAME_IS_NOT_IN_USE);
    /* add ut_assert for FM_GlobalData reset */
}

void Test_FM_GetFilenameState_FilenameInUse_OSFilestatDefine_FILESTAT_ISDIR(void)
{
    // Arrange
    uint32 dummy_bufsize = 16;
    char dummy_filename[16] = "FileName_Test";
    bool dummy_fileinfo = false;
    os_fstat_t dummy_fstat = {.FileModeBits=0x10000};
    
    //#define OS_FILESTAT_ISDIR

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),999);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat),&dummy_fstat,sizeof(dummy_fstat),false);

    // Act 
    uint32 filename_state = FM_GetFilenameState(dummy_filename, dummy_bufsize, dummy_fileinfo);

    // Assert
    UtAssert_True(filename_state == FM_NAME_IS_DIRECTORY, "The FilenameState returned is %u and should be %u",
                    filename_state, FM_NAME_IS_DIRECTORY);
}


void Test_FM_GetFilenameState_FilenameInUse_OSFilestatNOTDefine_FileInfoFalse(void)
{
    // Arrange
    uint32 dummy_bufsize = 16;
    char dummy_filename[16] = "FileName_Test";
    bool dummy_fileinfo = false;
    os_fstat_t dummy_fstat = {.FileModeBits=0x10000};
    
    #undef OS_FILESTAT_ISDIR

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),999);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat),&dummy_fstat,sizeof(dummy_fstat),false);

    // Act 
    uint32 filename_state = FM_GetFilenameState(dummy_filename, dummy_bufsize, dummy_fileinfo);

    // Assert
    UtAssert_MIR("Need to figure out how to undefine OS_FILESTAT_ISDIR to check other if statement %u",filename_state);
}

void Test_FM_GetFilenameState_FileNameInUseandValidNotDirectory_FileInfoCmdFalse(void)
{
    // Arrange
    uint32 dummy_bufsize = 16;
    char dummy_filename[16] = "FileName_Test";
    bool dummy_fileinfo = false;
    os_fstat_t dummy_fstat = {.FileModeBits=0x00000};

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),999);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat), OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat),&dummy_fstat,sizeof(dummy_fstat),false);

    // Act
    uint32 filename_state = FM_GetFilenameState(dummy_filename, dummy_bufsize, dummy_fileinfo);

    // Assert
    UtAssert_True(filename_state == FM_NAME_IS_FILE_CLOSED, "The FilenameState returned is %u and should be %u",
		    filename_state, FM_NAME_IS_FILE_CLOSED);
    UtAssert_True(dummy_fileinfo == false, "The FileInfo should be false and it is %s",dummy_fileinfo ? "true":"false");
}

void Test_FM_GetFilenameState_FileNameInUseandValidNotDirectory_FileInfoCmdTrue(void)
{
    // Arrange
    uint32 dummy_bufsize = 16;
    char dummy_filename[16] = "FileName_Test";
    bool dummy_fileinfo = true;
    os_fstat_t dummy_fstat = {.FileModeBits=0x00000};

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),999);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat), OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat),&dummy_fstat,sizeof(dummy_fstat),false);

    // Act
    uint32 filename_state = FM_GetFilenameState(dummy_filename, dummy_bufsize, dummy_fileinfo);

    // Assert
    UtAssert_True(filename_state == FM_NAME_IS_FILE_CLOSED, "The FilenameState returned is %u and should be %u",
                    filename_state, FM_NAME_IS_FILE_CLOSED);
    UtAssert_True(dummy_fileinfo == true, "The FileInfo should be false and it is %s",dummy_fileinfo ? "true":"false");
#ifdef OS_FILESTAT_TIME
    UtAssert_True(FM_GlobalData.FileStatTime == OS_FILESTAT_TIME(dummy_fstat), "The file system should be this?");
#else
    UtAssert_True(FM_GlobalData.FileStatTime == FileStatus.st_mtime, "THis is what it should be");
#endif
}



/* **************************
 * VerifyNameValid Tests
 * *************************/
void Test_FM_VerifyNameValid_FilenameStateNotValid(void)
{
    // Arrange
    #define filename_size 16
    uint32 dummy_bufsize = filename_size;
    uint32 dummy_eventid = 999;
    char dummy_name[filename_size] = "Dummy_Name"; 
    const char dummy_cmdtext;

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename), false);

    // Act
    uint32 filename_state = FM_VerifyNameValid(dummy_name,dummy_bufsize,dummy_eventid,&dummy_cmdtext);

    uint8 count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    // Assert
    UtAssert_True(filename_state == FM_NAME_IS_INVALID, "The FilenameState returned is %u and should be %u",
		    filename_state, FM_NAME_IS_INVALID);
    UtAssert_True(count_SendEvent == 1, "SendEvent was called %u time(s) and should be 1", count_SendEvent);
}

void Test_FM_VerifyNameValid_FilenameStateIsValid(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = 999;
    char dummy_name[32] = "Dummy_Name";
    const char dummy_cmdtext;

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename), true);

    // Act
    uint32 filename_state = FM_VerifyNameValid(dummy_name,dummy_bufsize,dummy_eventid,&dummy_cmdtext);

    uint8 count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    // Assert
    UtAssert_True(filename_state != FM_NAME_IS_INVALID, "The FilenameState returned is not FM_NAME_IS_INVALID");
    UtAssert_True(count_SendEvent == 0, "SendEvent was called %u time(s) and should be 0", count_SendEvent);
}

/* **************************
 * VerifyFileClosed Tests
 * *************************/
void Test_FM_VerifyFileClosed_FilenameInvalid(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    uint32 eventid_before = dummy_eventid;

    // Act
    bool result_fileclosed = FM_VerifyFileClosed(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat = UT_GetStubCount(UT_KEY(OS_stat));
    
    // Assert
    UtAssert_INT32_EQ(result_fileclosed, false);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osstat, 0);
    UtAssert_INT32_EQ(count_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, eventid_before+FM_FNAME_INVALID_EID_OFFSET);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType,CFE_EVS_EventType_ERROR);
}

void Test_FM_VerifyFileClosed_FilenameNotInUse(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "dummy_filename";
    const char dummy_cmdtext;

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename), true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat), !OS_SUCCESS);

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    uint32 eventid_before = dummy_eventid;

    // Act
    bool result_fileclosed = FM_VerifyFileClosed(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent       = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat          = UT_GetStubCount(UT_KEY(OS_stat));
    
    // Assert
    UtAssert_INT32_EQ(result_fileclosed, false);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, eventid_before+FM_FNAME_DNE_EID_OFFSET);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType,CFE_EVS_EventType_ERROR);
}


void Test_FM_VerifyFileClosed_FilenameOpen(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;
    bool fileopen = true;
    os_fstat_t filestat = {.FileModeBits=0x00000};

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat), &filestat, sizeof(filestat),false);
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &fileopen, sizeof(fileopen), false);

    // Act
    bool result_fileclosed = FM_VerifyFileClosed(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent       = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat          = UT_GetStubCount(UT_KEY(OS_stat));
    uint8 count_osforeachobject = UT_GetStubCount(UT_KEY(OS_ForEachObject));
    //uint8 count_osidentify      = UT_GetStubCount(UT_KEY(OS_IdentifyObject));
    
    // Assert
    UtAssert_INT32_EQ(result_fileclosed, true);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osforeachobject, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 0);
    //UtAssert_INT32_EQ(count_osidentify, 1);
    UtAssert_MIR("Unable to stub helper function, LoadOpenFileData, in fm_cmd_utils.c");
}

void Test_FM_VerifyFileClosed_FilenameClosed(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;
    os_fstat_t filestat = {.FileModeBits=0x00000};

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat),&filestat,sizeof(filestat),false);

    // Act
    bool result_fileclosed = FM_VerifyFileClosed(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent       = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat          = UT_GetStubCount(UT_KEY(OS_stat));
    uint8 count_osforeachobject = UT_GetStubCount(UT_KEY(OS_ForEachObject));
    
    // Assert
    UtAssert_INT32_EQ(result_fileclosed, true);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osforeachobject, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 0);
}

void Test_FM_VerifyFileClosed_FilenameIsDirectory(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;
    os_fstat_t filestat = {.FileModeBits=0x10000};

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat),&filestat,sizeof(filestat),false);

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    uint32 eventid_before = dummy_eventid;

    // Act
    bool result_fileclosed = FM_VerifyFileClosed(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat = UT_GetStubCount(UT_KEY(OS_stat));
    
    // Assert
    UtAssert_INT32_EQ(result_fileclosed, false);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, eventid_before+FM_FNAME_ISDIR_EID_OFFSET);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType,CFE_EVS_EventType_ERROR);   
}

void Test_FM_VerifyFileClosed_FilenameDefault(void)
{
    UtAssert_MIR("Not sure how to forcefail function in the same file, otherwise this will not happen");
}

/* **********************
 * VerifyChildTask tests
 * *********************/
void Test_FM_VerifyChildTask_ChildSemaphoreIsInvalid(void)
{
    // Arrange
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    const char* dummy_textptr = "dummy_text";
    FM_GlobalData.ChildSemaphore = FM_CHILD_SEM_INVALID;

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);


    // Act
    bool result_verifychildtask = FM_VerifyChildTask(dummy_eventid, dummy_textptr);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_pspmemset = UT_GetStubCount(UT_KEY(CFE_PSP_MemSet));

    // Assert
    UtAssert_INT32_EQ(result_verifychildtask, false);
    UtAssert_INT32_EQ(count_pspmemset, 0);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(dummy_eventid + FM_CHILD_DISABLED_EID_OFFSET, context_CFE_EVS_SendEvent.EventID);
    UtAssert_INT32_EQ(CFE_EVS_EventType_ERROR, context_CFE_EVS_SendEvent.EventType);
}

void Test_FM_VerifyChildTask_LocalQueueCountEqualToQueueDepth(void)
{
    // Arrange
    uint32 dummy_eventid            = UT_Utils_Any_uint8();
    const char* dummy_textptr       = "dummy_text";
    FM_GlobalData.ChildSemaphore    = FM_CHILD_SEM_INVALID + 1;
    FM_GlobalData.ChildQueueCount   = FM_CHILD_QUEUE_DEPTH;

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    // Act
    bool result_verifychildtask = FM_VerifyChildTask(dummy_eventid, dummy_textptr);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_pspmemset = UT_GetStubCount(UT_KEY(CFE_PSP_MemSet));

    // Assert
    UtAssert_INT32_EQ(result_verifychildtask, false);
    UtAssert_INT32_EQ(count_pspmemset, 0);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(dummy_eventid + FM_CHILD_Q_FULL_EID_OFFSET, context_CFE_EVS_SendEvent.EventID);
    UtAssert_INT32_EQ(CFE_EVS_EventType_ERROR, context_CFE_EVS_SendEvent.EventType);
}

void Test_FM_VerifyChildTask_LocalQueueCountGreaterQueueDepth(void)
{
    // Arrange
    uint32 dummy_eventid            = UT_Utils_Any_uint8();
    const char* dummy_textptr       = "dummy_text";
    FM_GlobalData.ChildSemaphore    = FM_CHILD_SEM_INVALID + 1;
    FM_GlobalData.ChildQueueCount   = FM_CHILD_QUEUE_DEPTH + 1;

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    // Act
    bool result_verifychildtask = FM_VerifyChildTask(dummy_eventid, dummy_textptr);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_pspmemset = UT_GetStubCount(UT_KEY(CFE_PSP_MemSet));

    // Assert
    UtAssert_INT32_EQ(result_verifychildtask, false);
    UtAssert_INT32_EQ(count_pspmemset, 0);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(dummy_eventid + FM_CHILD_BROKEN_EID_OFFSET, context_CFE_EVS_SendEvent.EventID);
    UtAssert_INT32_EQ(CFE_EVS_EventType_ERROR, context_CFE_EVS_SendEvent.EventType); 
}

void Test_FM_VerifyChildTask_ChildWriteIndexGreaterChildQDepth(void)
{
    // Arrange
    uint32 dummy_eventid            = UT_Utils_Any_uint8();
    const char* dummy_textptr       = "dummy_text";
    FM_GlobalData.ChildSemaphore    = FM_CHILD_SEM_INVALID + 1;
    FM_GlobalData.ChildQueueCount   = FM_CHILD_QUEUE_DEPTH - 1;
    FM_GlobalData.ChildWriteIndex   = FM_CHILD_QUEUE_DEPTH + 1;

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    // Act
    bool result_verifychildtask = FM_VerifyChildTask(dummy_eventid, dummy_textptr);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_pspmemset = UT_GetStubCount(UT_KEY(CFE_PSP_MemSet));

    // Assert
    UtAssert_INT32_EQ(result_verifychildtask, false);
    UtAssert_INT32_EQ(count_pspmemset, 0);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(dummy_eventid + FM_CHILD_BROKEN_EID_OFFSET, context_CFE_EVS_SendEvent.EventID);
    UtAssert_INT32_EQ(CFE_EVS_EventType_ERROR, context_CFE_EVS_SendEvent.EventType); 
}

void Test_FM_VerifyChildTask_ChildWriteIndexEqualChildQDepth(void)
{
    // Arrange
    uint32 dummy_eventid            = UT_Utils_Any_uint8();
    const char* dummy_textptr       = "dummy_text";
    FM_GlobalData.ChildSemaphore    = FM_CHILD_SEM_INVALID + 1;
    FM_GlobalData.ChildQueueCount   = FM_CHILD_QUEUE_DEPTH - 1;
    FM_GlobalData.ChildWriteIndex   = FM_CHILD_QUEUE_DEPTH;

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    // Act
    bool result_verifychildtask = FM_VerifyChildTask(dummy_eventid, dummy_textptr);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_pspmemset = UT_GetStubCount(UT_KEY(CFE_PSP_MemSet));

    // Assert
    UtAssert_INT32_EQ(result_verifychildtask, false);
    UtAssert_INT32_EQ(count_pspmemset, 0);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(dummy_eventid + FM_CHILD_BROKEN_EID_OFFSET, context_CFE_EVS_SendEvent.EventID);
    UtAssert_INT32_EQ(CFE_EVS_EventType_ERROR, context_CFE_EVS_SendEvent.EventType);   
}

void Test_FM_VerifyChildTask_Default(void)
{
    // Arrange
    uint32 dummy_eventid            = UT_Utils_Any_uint8();
    const char* dummy_textptr       = "dummy_text";
    FM_GlobalData.ChildSemaphore    = FM_CHILD_SEM_INVALID + 1;
    FM_GlobalData.ChildQueueCount   = FM_CHILD_QUEUE_DEPTH - 1;
    FM_GlobalData.ChildWriteIndex   = FM_CHILD_QUEUE_DEPTH - 1;

    // Act
    bool result_verifychildtask = FM_VerifyChildTask(dummy_eventid, dummy_textptr);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_pspmemset = UT_GetStubCount(UT_KEY(CFE_PSP_MemSet));

    // Assert
    UtAssert_INT32_EQ(result_verifychildtask, true);
    UtAssert_INT32_EQ(count_pspmemset, 1);
    UtAssert_INT32_EQ(count_sendevent, 0);
}

/* **********************
 * InvokeChildTask tests
 * *********************/
void FM_InvokeChildTask_WriteIndexGreaterThanChildQueue(void)
{
    FM_GlobalData.ChildWriteIndex = FM_CHILD_QUEUE_DEPTH;
    FM_GlobalData.ChildQueueCount = 1;
    FM_GlobalData.ChildSemaphore = 1; /* not FM_CHILD_SEM_INVALID */

    FM_InvokeChildTask();

    UtAssert_INT32_EQ(FM_GlobalData.ChildWriteIndex, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueueCount, 2);

    uint8 call_count_OS_CountSemGive = UT_GetStubCount(UT_KEY(OS_CountSemGive));

    UtAssert_INT32_EQ(call_count_OS_CountSemGive, 1);
}

void FM_InvokeChildTask_ChildSemaphoreNotEqualToSEMINVALID(void)
{
    FM_GlobalData.ChildWriteIndex = 1;
    FM_GlobalData.ChildQueueCount = 1;
    FM_GlobalData.ChildSemaphore = FM_CHILD_SEM_INVALID;

    FM_InvokeChildTask();

    UtAssert_INT32_EQ(FM_GlobalData.ChildWriteIndex, 2);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueueCount, 2);

    uint8 call_count_OS_CountSemGive = UT_GetStubCount(UT_KEY(OS_CountSemGive));

    UtAssert_INT32_EQ(call_count_OS_CountSemGive, 0);

}

/* **********************
 * VerifyFileExists Tests
 * *********************/
void Test_FM_VerifyFileExists_FilenameInvalid(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    uint32 eventid_before = dummy_eventid;

    // Act
    bool result_fileexist = FM_VerifyFileExists(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat = UT_GetStubCount(UT_KEY(OS_stat));
    
    // Assert
    UtAssert_INT32_EQ(result_fileexist, false);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osstat, 0);
    UtAssert_INT32_EQ(count_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, eventid_before+FM_FNAME_INVALID_EID_OFFSET);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType,CFE_EVS_EventType_ERROR);
}

void Test_FM_VerifyFileExists_FilenameNotInUse(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "dummy_filename";
    const char dummy_cmdtext;

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename), true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat), !OS_SUCCESS);

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    uint32 eventid_before = dummy_eventid;

    // Act
    bool result_fileexist = FM_VerifyFileExists(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent       = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat          = UT_GetStubCount(UT_KEY(OS_stat));
    
    // Assert
    UtAssert_INT32_EQ(result_fileexist, false);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, eventid_before+FM_FNAME_DNE_EID_OFFSET);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType,CFE_EVS_EventType_ERROR);
}

void Test_FM_VerifyFileExists_FilenameOpen(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;
    bool fileopen = true;
    os_fstat_t filestat = {.FileModeBits=0x00000};

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat), &filestat, sizeof(filestat),false);
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &fileopen, sizeof(fileopen), false);

    // Act
    bool result_fileexist = FM_VerifyFileExists(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    /*
    uint8 count_SendEvent       = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat          = UT_GetStubCount(UT_KEY(OS_stat));
    uint8 count_osforeachobject = UT_GetStubCount(UT_KEY(OS_ForEachObject));
    uint8 count_osidentify      = UT_GetStubCount(UT_KEY(OS_IdentifyObject));
    
    // Assert
    UtAssert_INT32_EQ(result_fileexist, true);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osforeachobject, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 0);
    UtAssert_INT32_EQ(count_osidentify, 1);
    */
    UtAssert_INT32_EQ(result_fileexist, true);
    UtAssert_MIR("Unable to stub helper function, LoadOpenFileData, in fm_cmd_utils.c");
}

void Test_FM_VerifyFileExists_FilenameClosed(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;
    os_fstat_t filestat = {.FileModeBits=0x00000};

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat),&filestat,sizeof(filestat),false);

    // Act
    bool result_fileexist = FM_VerifyFileExists(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent       = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat          = UT_GetStubCount(UT_KEY(OS_stat));
    uint8 count_osforeachobject = UT_GetStubCount(UT_KEY(OS_ForEachObject));
    
    // Assert
    UtAssert_INT32_EQ(result_fileexist, true);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osforeachobject, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 0);
}

void Test_FM_VerifyFileExists_FilenameIsDirectory(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;
    os_fstat_t filestat = {.FileModeBits=0x10000};

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat),&filestat,sizeof(filestat),false);

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    uint32 eventid_before = dummy_eventid;

    // Act
    bool result_fileexist = FM_VerifyFileExists(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat = UT_GetStubCount(UT_KEY(OS_stat));
    
    // Assert
    UtAssert_INT32_EQ(result_fileexist, false);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, eventid_before+FM_FNAME_ISDIR_EID_OFFSET);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType,CFE_EVS_EventType_ERROR);   
}

void Test_FM_VerifyFileExists_FilenameDefault(void)
{
    UtAssert_MIR("Unable to stub helper function, LoadOpenFileData, in fm_cmd_utils.c");
}

/* **********************
 * VerifyFileNoExist Tests
 * *********************/
void Test_FM_VerifyFileNoExist_FilenameInvalid(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    uint32 eventid_before = dummy_eventid;

    // Act
    bool result_filenoexist = FM_VerifyFileNoExist(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat = UT_GetStubCount(UT_KEY(OS_stat));
    
    // Assert
    UtAssert_INT32_EQ(result_filenoexist, false);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osstat, 0);
    UtAssert_INT32_EQ(count_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, eventid_before+FM_FNAME_INVALID_EID_OFFSET);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType,CFE_EVS_EventType_ERROR);
}

void Test_FM_VerifyFileNoExist_FilenameNotInUse(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "dummy_filename";
    const char dummy_cmdtext;

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename), true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat), !OS_SUCCESS);

    // Act
    bool result_filenoexist = FM_VerifyFileNoExist(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent       = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat          = UT_GetStubCount(UT_KEY(OS_stat));
    
    // Assert
    UtAssert_INT32_EQ(result_filenoexist, true);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 0);
}

void Test_FM_VerifyFileNoExist_FilenameOpen(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;
    bool fileopen = true;
    os_fstat_t filestat = {.FileModeBits=0x00000};

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat), &filestat, sizeof(filestat),false);
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &fileopen, sizeof(fileopen), false);

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    //uint32 eventid_before = dummy_eventid;

    // Act
    bool result_filenoexist = FM_VerifyFileNoExist(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    /*
    uint8 count_SendEvent       = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat          = UT_GetStubCount(UT_KEY(OS_stat));
    uint8 count_osforeachobject = UT_GetStubCount(UT_KEY(OS_ForEachObject));
    uint8 count_osidentify      = UT_GetStubCount(UT_KEY(OS_IdentifyObject));
    
    // Assert
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osforeachobject, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 1);
    UtAssert_INT32_EQ(count_osidentify, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, eventid_before+FM_FNAME_EXIST_EID_OFFSET);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType,CFE_EVS_EventType_ERROR);
    */
    UtAssert_INT32_EQ(result_filenoexist, false);
    UtAssert_MIR("Unable to stub helper function, LoadOpenFileData, in fm_cmd_utils.c");
}

void Test_FM_VerifyFileNoExist_FilenameClosed(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;
    os_fstat_t filestat = {.FileModeBits=0x00000};

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat),&filestat,sizeof(filestat),false);

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    uint32 eventid_before = dummy_eventid;

    // Act
    bool result_filenoexist = FM_VerifyFileNoExist(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent       = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat          = UT_GetStubCount(UT_KEY(OS_stat));
    uint8 count_osforeachobject = UT_GetStubCount(UT_KEY(OS_ForEachObject));
    
    // Assert
    UtAssert_INT32_EQ(result_filenoexist, false);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osforeachobject, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, eventid_before+FM_FNAME_EXIST_EID_OFFSET);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType,CFE_EVS_EventType_ERROR); 
}

void Test_FM_VerifyFileNoExist_FilenameIsDirectory(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;
    os_fstat_t filestat = {.FileModeBits=0x10000};

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat),&filestat,sizeof(filestat),false);

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    uint32 eventid_before = dummy_eventid;

    // Act
    bool result_filenoexist = FM_VerifyFileNoExist(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat = UT_GetStubCount(UT_KEY(OS_stat));
    
    // Assert
    UtAssert_INT32_EQ(result_filenoexist, false);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, eventid_before+FM_FNAME_ISDIR_EID_OFFSET);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType,CFE_EVS_EventType_ERROR);   
}

void Test_FM_VerifyFileNoExist_FilenameDefault(void)
{
    UtAssert_MIR("Unable to stub helper function, LoadOpenFileData, in fm_cmd_utils.c");
}

/* **********************
 * VerifyFileNotOpen Tests
 * *********************/
void Test_FM_VerifyFileNotOpen_FilenameInvalid(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    uint32 eventid_before = dummy_eventid;

    // Act
    bool result_filenotopen = FM_VerifyFileNotOpen(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat = UT_GetStubCount(UT_KEY(OS_stat));
    
    // Assert
    UtAssert_INT32_EQ(result_filenotopen, false);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osstat, 0);
    UtAssert_INT32_EQ(count_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, eventid_before+FM_FNAME_INVALID_EID_OFFSET);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType,CFE_EVS_EventType_ERROR);
}

void Test_FM_VerifyFileNotOpen_FilenameNotInUse(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "dummy_filename";
    const char dummy_cmdtext;

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename), true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat), !OS_SUCCESS);

    // Act
    bool result_filenotopen = FM_VerifyFileNotOpen(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent       = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat          = UT_GetStubCount(UT_KEY(OS_stat));
    
    // Assert
    UtAssert_INT32_EQ(result_filenotopen, true);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 0);
}

void Test_FM_VerifyFileNotOpen_FilenameOpen(void)
{
    // Arrange
    /*uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;
    bool fileopen = true;
    os_fstat_t filestat = {.FileModeBits=0x00000};

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat), &filestat, sizeof(filestat),false);
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &fileopen, sizeof(fileopen), false);

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);
    */
    //uint32 eventid_before = dummy_eventid;

    // Act
    /*
    bool result_filenotopen = FM_VerifyFileNotOpen(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);
    */
    /*
    uint8 count_SendEvent       = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat          = UT_GetStubCount(UT_KEY(OS_stat));
    uint8 count_osforeachobject = UT_GetStubCount(UT_KEY(OS_ForEachObject));
    uint8 count_osidentify      = UT_GetStubCount(UT_KEY(OS_IdentifyObject));
    
    // Assert
    UtAssert_INT32_EQ(result_filenotopen, false);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osforeachobject, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 1);
    UtAssert_INT32_EQ(count_osidentify, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, eventid_before+FM_FNAME_ISOPEN_EID_OFFSET);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType,CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(result_filenotopen, false);
    */
    UtAssert_MIR("Unable to stub helper function, LoadOpenFileData, in fm_cmd_utils.c");
}

void Test_FM_VerifyFileNotOpen_FilenameClosed(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;
    os_fstat_t filestat = {.FileModeBits=0x00000};

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat),&filestat,sizeof(filestat),false);

    // Act
    bool result_filenotopen = FM_VerifyFileNotOpen(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent       = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat          = UT_GetStubCount(UT_KEY(OS_stat));
    uint8 count_osforeachobject = UT_GetStubCount(UT_KEY(OS_ForEachObject));
    
    // Assert
    UtAssert_INT32_EQ(result_filenotopen, true);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osforeachobject, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 0);
}

void Test_FM_VerifyFileNotOpen_FilenameIsDirectory(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;
    os_fstat_t filestat = {.FileModeBits=0x10000};

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat),&filestat,sizeof(filestat),false);

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    uint32 eventid_before = dummy_eventid;

    // Act
    bool result_filenotopen = FM_VerifyFileNotOpen(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat = UT_GetStubCount(UT_KEY(OS_stat));
    
    // Assert
    UtAssert_INT32_EQ(result_filenotopen, false);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, eventid_before+FM_FNAME_ISDIR_EID_OFFSET);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType,CFE_EVS_EventType_ERROR);   
}

void Test_FM_VerifyFileNotOpen_FilenameDefault(void)
{
    UtAssert_MIR("Unable to stub helper function, LoadOpenFileData, in fm_cmd_utils.c");
}

/* **********************
 * VerifyDirExists Tests
 * *********************/
void Test_FM_VerifyDirExists_FilenameInvalid(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    uint32 eventid_before = dummy_eventid;

    // Act
    bool result_direxists = FM_VerifyDirExists(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat = UT_GetStubCount(UT_KEY(OS_stat));
    
    // Assert
    UtAssert_INT32_EQ(result_direxists, false);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osstat, 0);
    UtAssert_INT32_EQ(count_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, eventid_before+FM_FNAME_INVALID_EID_OFFSET);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType,CFE_EVS_EventType_ERROR);
}

void Test_FM_VerifyDirExists_FilenameNotInUse(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "dummy_filename";
    const char dummy_cmdtext;

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename), true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat), !OS_SUCCESS);

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    uint32 eventid_before = dummy_eventid;

    // Act
    bool result_direxists = FM_VerifyDirExists(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent       = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat          = UT_GetStubCount(UT_KEY(OS_stat));
    
    // Assert
    UtAssert_INT32_EQ(result_direxists, false);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, eventid_before+FM_FNAME_DNE_EID_OFFSET);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType,CFE_EVS_EventType_ERROR); 
}

void Test_FM_VerifyDirExists_FilenameOpen(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;
    bool fileopen = true;
    os_fstat_t filestat = {.FileModeBits=0x00000};

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat), &filestat, sizeof(filestat),false);
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &fileopen, sizeof(fileopen), false);

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    //uint32 eventid_before = dummy_eventid;

    // Act
    bool result_direxists = FM_VerifyDirExists(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    /*
    uint8 count_SendEvent       = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat          = UT_GetStubCount(UT_KEY(OS_stat));
    uint8 count_osforeachobject = UT_GetStubCount(UT_KEY(OS_ForEachObject));
    uint8 count_osidentify      = UT_GetStubCount(UT_KEY(OS_IdentifyObject));
    
    // Assert
    UtAssert_INT32_EQ(result_direxists, false);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osforeachobject, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 1);
    UtAssert_INT32_EQ(count_osidentify, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, eventid_before+FM_FNAME_ISFILE_EID_OFFSET);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType,CFE_EVS_EventType_ERROR);
    */
    UtAssert_INT32_EQ(result_direxists, false);
    UtAssert_MIR("Unable to stub helper function, LoadOpenFileData, in fm_cmd_utils.c");
}

void Test_FM_VerifyDirExists_FilenameClosed(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;
    os_fstat_t filestat = {.FileModeBits=0x00000};

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat),&filestat,sizeof(filestat),false);

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    uint32 eventid_before = dummy_eventid;

    // Act
    bool result_direxists = FM_VerifyDirExists(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent       = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat          = UT_GetStubCount(UT_KEY(OS_stat));
    uint8 count_osforeachobject = UT_GetStubCount(UT_KEY(OS_ForEachObject));
    
    // Assert
    UtAssert_INT32_EQ(result_direxists, false);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osforeachobject, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, eventid_before+FM_FNAME_ISFILE_EID_OFFSET);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType,CFE_EVS_EventType_ERROR);   
}

void Test_FM_VerifyDirExists_FilenameIsDirectory(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;
    os_fstat_t filestat = {.FileModeBits=0x10000};

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat),&filestat,sizeof(filestat),false);

    // Act
    bool result_direxists = FM_VerifyDirExists(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat = UT_GetStubCount(UT_KEY(OS_stat));
    
    // Assert
    UtAssert_INT32_EQ(result_direxists, true);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 0);
}

void Test_FM_VerifyDirExists_FilenameDefault(void)
{
    UtAssert_MIR("Unable to stub helper function, LoadOpenFileData, in fm_cmd_utils.c");
}

/* **********************
 * VerifyDirNoExist Tests
 * *********************/
void Test_FM_VerifyDirNoExist_FilenameInvalid(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    uint32 eventid_before = dummy_eventid;

    // Act
    bool result_dirnoexist = FM_VerifyDirNoExist(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat = UT_GetStubCount(UT_KEY(OS_stat));
    
    // Assert
    UtAssert_INT32_EQ(result_dirnoexist, false);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osstat, 0);
    UtAssert_INT32_EQ(count_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, eventid_before+FM_FNAME_INVALID_EID_OFFSET);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType,CFE_EVS_EventType_ERROR);
}

void Test_FM_VerifyDirNoExist_FilenameNotInUse(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "dummy_filename";
    const char dummy_cmdtext;

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename), true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat), !OS_SUCCESS);

    // Act
    bool result_dirnoexist = FM_VerifyDirNoExist(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent       = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat          = UT_GetStubCount(UT_KEY(OS_stat));
    
    // Assert
    UtAssert_INT32_EQ(result_dirnoexist, true);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 0);
}

void Test_FM_VerifyDirNoExist_FilenameOpen(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;
    bool fileopen = true;
    os_fstat_t filestat = {.FileModeBits=0x00000};

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat), &filestat, sizeof(filestat),false);
    UT_SetDataBuffer(UT_KEY(OS_ForEachObject), &fileopen, sizeof(fileopen), false);

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    //uint32 eventid_before = dummy_eventid;

    // Act
    bool result_dirnoexist = FM_VerifyDirNoExist(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    /*
    uint8 count_SendEvent       = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat          = UT_GetStubCount(UT_KEY(OS_stat));
    uint8 count_osforeachobject = UT_GetStubCount(UT_KEY(OS_ForEachObject));
    uint8 count_osidentify      = UT_GetStubCount(UT_KEY(OS_IdentifyObject));
    
    // Assert
    
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osforeachobject, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 1);
    UtAssert_INT32_EQ(count_osidentify, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, eventid_before+FM_FNAME_DNE_EID_OFFSET);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType,CFE_EVS_EventType_ERROR);
    */
    UtAssert_INT32_EQ(result_dirnoexist, false);
    UtAssert_MIR("Unable to stub helper function, LoadOpenFileData, in fm_cmd_utils.c");
}

void Test_FM_VerifyDirNoExist_FilenameClosed(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;
    os_fstat_t filestat = {.FileModeBits=0x00000};

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat),&filestat,sizeof(filestat),false);

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    uint32 eventid_before = dummy_eventid;

    // Act
    bool result_dirnoexist = FM_VerifyDirNoExist(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent       = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat          = UT_GetStubCount(UT_KEY(OS_stat));
    uint8 count_osforeachobject = UT_GetStubCount(UT_KEY(OS_ForEachObject));
    
    // Assert
    UtAssert_INT32_EQ(result_dirnoexist, false);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osforeachobject, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, eventid_before+FM_FNAME_DNE_EID_OFFSET);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType,CFE_EVS_EventType_ERROR);   
}

void Test_FM_VerifyDirNoExist_FilenameIsDirectory(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    uint32 dummy_eventid = UT_Utils_Any_uint8();
    char dummy_name[32] = "Dummy_Message";
    const char dummy_cmdtext;
    os_fstat_t filestat = {.FileModeBits=0x10000};

    UT_SetDefaultReturnValue(UT_KEY(CFS_IsValidFilename),true);
    UT_SetDefaultReturnValue(UT_KEY(OS_stat),OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_stat),&filestat,sizeof(filestat),false);

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    uint32 eventid_before = dummy_eventid;

    // Act
    bool result_dirnoexist = FM_VerifyDirNoExist(dummy_name, dummy_bufsize, dummy_eventid, &dummy_cmdtext);

    uint8 count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_isvalidfilename = UT_GetStubCount(UT_KEY(CFS_IsValidFilename));
    uint8 count_osstat = UT_GetStubCount(UT_KEY(OS_stat));
    
    // Assert
    UtAssert_INT32_EQ(result_dirnoexist, false);
    UtAssert_INT32_EQ(count_isvalidfilename, 1);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(count_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, eventid_before+FM_FNAME_ISDIR_EID_OFFSET);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType,CFE_EVS_EventType_ERROR);  
}

void Test_FM_VerifyDirNoExist_FilenameDefault(void)
{
    UtAssert_MIR("Unable to stub helper function, LoadOpenFileData, in fm_cmd_utils.c");
}

/* **********************
 * AppendPathSep Tests
 * *********************/
void FM_AppendPathSep_StringLengthZero(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    char dummy_directory[32] = "";

    // Act
    FM_AppendPathSep(dummy_directory, dummy_bufsize);
    
    // Assert
    uint32 StringLength = strlen(dummy_directory);
    UtAssert_INT32_EQ(StringLength, 0);
}

void FM_AppendPathSep_StringLengthSmallerThanBuffer(void)
{
    // Arrange
    uint32 dummy_bufsize = 1;
    char dummy_directory[32] = "dummy_directory";

    uint32 StringLength_before = strlen(dummy_directory);

    // Act
    FM_AppendPathSep(dummy_directory, dummy_bufsize);

    uint32 StringLength = strlen(dummy_directory);
    
    // Assert
    UtAssert_INT32_EQ(StringLength_before, StringLength);
    UtAssert_INT32_EQ(dummy_directory[StringLength-1] == '/', false);
}

void FM_AppendPathSep_StringEndWithPathSeparator(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    char dummy_directory[32] = "dummy_directory/";

    uint32 StringLength_before = strlen(dummy_directory);

    // Act
    FM_AppendPathSep(dummy_directory, dummy_bufsize);

    uint32 StringLength     = strlen(dummy_directory);
    
    // Assert
    UtAssert_INT32_EQ(StringLength_before, StringLength);
    UtAssert_INT32_EQ(dummy_directory[StringLength-1], '/');
}

void FM_AppendPathSep_StringEndWithoutSeparator(void)
{
    // Arrange
    uint32 dummy_bufsize = 32;
    char dummy_directory[32] = "dummy_directory";

    uint32 StringLength_before = strlen(dummy_directory);

    // Act
    FM_AppendPathSep(dummy_directory, dummy_bufsize);

    uint32 StringLength     = strlen(dummy_directory);
    
    // Assert
    UtAssert_INT32_EQ(StringLength_before != StringLength, true);
    UtAssert_INT32_EQ(dummy_directory[StringLength-1], '/');
}

/*
 * Setup function prior to every test
 */
void fm_UT_Setup(void)
{
        UT_ResetState(0);
}

/*
 * Teardown function after every test
 */
void fm_UT_TearDown(void)
{
        /* do nothing */
}


/* * * * * * * * * * * * * * 
 * Add Method Tests
 * * * * * * * * * * * * * */
void add_FM_IsValidCmdPktLength_tests(void)
{
    UtTest_Add(Test_FM_IsValidCmdPktLength_ActualLengthSuccess,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_IsValidCmdPktLength_ActualLengthSuccess");

    UtTest_Add(Test_FM_IsValidCmdPktLength_ActualLengthNotExpected,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_FM_IsValidCmdPktLength_ActualLengthSuccess");
}

void add_FM_VerifyOverwrite_tests(void)
{
    UtTest_Add(Test_FM_VerifyOverwrite_OverwriteIsNeitherFalseNorTrue,
	fm_UT_Setup, fm_UT_TearDown,
       "Test_FM_VerifyOverwrite_OverwriteIsNeitherFalseNorTrue");	
    
    UtTest_Add(Test_FM_VerifyOverwrite_OverwriteIsTrue,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_FM_VerifyOverwrite_OverwriteIsTrue");

    UtTest_Add(Test_FM_VerifyOverwrite_OverwriteIsFalse,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyOverwite_OverwriteIsFalse");
}

/*
void add_LoadOpenFileData_tests(void)
{
    UtTest_Add(Test_LoadOpenFileData_OpenFileCountUnchanged,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_LoadOpenFileData_OpenFileCountUnchanged");

    UtTest_Add(Test_LoadOpenFileData_OpenFileCountIncrease,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_LoadOpenFileData_OpenFileCountIncrease");

    UtTest_Add(Test_LoadOpenFileData_OpenFilesDataNotNull,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_LoadOpenFileData_OpenFilesDataNotNull");

    UtTest_Add(Test_LoadOpenFileData_FDGetInfoSuccess,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_LoadOpenFileData_FDGetInfoSuccess");

    UtTest_Add(Test_LoadOpenFileData_GetTaskInfoReturnsSuccess,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_LoadOpenFileData_GetTaskInfoReturnsSuccess");
}
*/

void add_FM_GetOpenFilesData_tests(void)
{
    UtTest_Add(Test_FM_GetOpenFilesData_OSForEachObjectCalled,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_FM_GetOpenFilesData_OSForEachObjectCalled");
}

void add_FM_GetFilenameState_tests(void)
{
    UtTest_Add(Test_FM_GetFilenameState_FileNameIsNull,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_FM_GetFilenameState_FileNameIsNull");

    UtTest_Add(Test_FM_GetFilenameState_FileNameNotNullButNotValid,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_FM_GetFilenameState_FileNameNotNullButNotValid");

    UtTest_Add(Test_FM_GetFilenameState_StringLengthZero,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_GetFilenameState_StringLengthZero");

    UtTest_Add(Test_FM_GetFilenameState_FileNameIsValidButNotInUse_FileInfoCmdFalse,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_FM_GetFilenameState_FileNameIsValidButNotInUse_FileInfoCmdFalse");

    UtTest_Add(Test_FM_GetFilenameState_FileNameIsValidButNOtINUse_FileINfoCmdTrue,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_FM_GetFilenameState_FileNameIsValidButNOtINUse_FileINfoCmdTrue");

    UtTest_Add(Test_FM_GetFilenameState_FilenameInUse_OSFilestatDefine_FILESTAT_ISDIR,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_FM_GetFilenameState_FilenameInUse_OSFilestatDefine_FILESTAT_ISDIR");

    UtTest_Add( Test_FM_GetFilenameState_FilenameInUse_OSFilestatNOTDefine_FileInfoFalse,
	fm_UT_Setup, fm_UT_TearDown,
	" Test_FM_GetFilenameState_FilenameInUse_OSFilestatNOTDefine_FileInfoFalse");

    UtTest_Add(Test_FM_GetFilenameState_FileNameInUseandValidNotDirectory_FileInfoCmdFalse,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_FM_GetFilenameState_FileNameInUseandValidNotDirectory_FileInfoCmdFalse");

    UtTest_Add(Test_FM_GetFilenameState_FileNameInUseandValidNotDirectory_FileInfoCmdTrue,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_FM_GetFilenameState_FileNameInUseandValidNotDirectory_FileInfoCmdTrue");
}

void add_FM_VerifyNameValid_tests(void)
{
    UtTest_Add(Test_FM_VerifyNameValid_FilenameStateNotValid,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_FM_VerifyNameValid_FilenameStateNotValid");

    UtTest_Add(Test_FM_VerifyNameValid_FilenameStateIsValid,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_FM_VerifyNameValid_FilenameStateIsValid");
}

void add_FM_VerifyFileClosed_tests(void)
{
    UtTest_Add(Test_FM_VerifyFileClosed_FilenameInvalid,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyFileClosed_FilenameInvalid");

    UtTest_Add(Test_FM_VerifyFileClosed_FilenameNotInUse,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_FM_VerifyFileClosed_FilenameNotInUse");

    UtTest_Add(Test_FM_VerifyFileClosed_FilenameOpen,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_FM_VerifyFileClosed_FilenameOpen");

    UtTest_Add(Test_FM_VerifyFileClosed_FilenameClosed,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_FM_VerifyFileClosed_FilenameClosed");

    UtTest_Add(Test_FM_VerifyFileClosed_FilenameIsDirectory,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_FM_VerifyFileClosed_FilenameIsDirectory");

    UtTest_Add(Test_FM_VerifyFileClosed_FilenameDefault,
	fm_UT_Setup, fm_UT_TearDown,
	"Test_FM_VerifyFileClosed_FilenameDefault");
}

void add_FM_VerifyFileExists_tests(void)
{
    UtTest_Add(Test_FM_VerifyFileExists_FilenameInvalid,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyFileExists_FilenameInvalid");

    UtTest_Add(Test_FM_VerifyFileExists_FilenameNotInUse,
    fm_UT_Setup, fm_UT_TearDown,
    "Test_FM_VerifyFileExists_FilenameNotInUse");

    UtTest_Add(Test_FM_VerifyFileExists_FilenameOpen,
    fm_UT_Setup, fm_UT_TearDown,
    "Test_FM_VerifyFileExists_FilenameOpen");

    UtTest_Add(Test_FM_VerifyFileExists_FilenameClosed,
    fm_UT_Setup, fm_UT_TearDown,
    "Test_FM_VerifyFileExists_FilenameClosed");

    UtTest_Add(Test_FM_VerifyFileExists_FilenameIsDirectory,
    fm_UT_Setup, fm_UT_TearDown,
    "Test_FM_VerifyFileExists_FilenameIsDirectory");

    UtTest_Add(Test_FM_VerifyFileExists_FilenameDefault,
    fm_UT_Setup, fm_UT_TearDown,
    "Test_FM_VerifyFileExists_FilenameDefault");
}

void add_FM_VerifyFileNoExist_tests(void)
{
    UtTest_Add(Test_FM_VerifyFileNoExist_FilenameInvalid,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyFileNoExist_FilenameInvalid");

    UtTest_Add(Test_FM_VerifyFileNoExist_FilenameNotInUse,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyFileNoExist_FilenameNotInUse");

    UtTest_Add(Test_FM_VerifyFileNoExist_FilenameOpen,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyFileNoExist_FilenameOpen");

    UtTest_Add(Test_FM_VerifyFileNoExist_FilenameClosed,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyFileNoExist_FilenameClosed");

    UtTest_Add(Test_FM_VerifyFileNoExist_FilenameIsDirectory,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyFileNoExist_FilenameIsDirectory");

    UtTest_Add(Test_FM_VerifyFileNoExist_FilenameDefault,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyFileNoExist_FilenameDefault");
}

void add_FM_VerifyFileNotOpen_tests(void)
{
    UtTest_Add(Test_FM_VerifyFileNotOpen_FilenameInvalid,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyFileNotOpen_FilenameInvalid");

    UtTest_Add(Test_FM_VerifyFileNotOpen_FilenameNotInUse,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyFileNotOpen_FilenameNotInUse");

    UtTest_Add(Test_FM_VerifyFileNotOpen_FilenameOpen,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyFileNotOpen_FilenameOpen");

    UtTest_Add(Test_FM_VerifyFileNotOpen_FilenameClosed,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyFileNotOpen_FilenameClosed");

    UtTest_Add(Test_FM_VerifyFileNotOpen_FilenameIsDirectory,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyFileNotOpen_FilenameIsDirectory");

    UtTest_Add(Test_FM_VerifyFileNotOpen_FilenameDefault,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyFileNotOpen_FilenameDefault");
}

void add_FM_VerifyDirExists_tests(void)
{
    UtTest_Add(Test_FM_VerifyDirExists_FilenameInvalid,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyDirExists_FilenameInvalid");

    UtTest_Add(Test_FM_VerifyDirExists_FilenameNotInUse,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyDirExists_FilenameNotInUse");

    UtTest_Add(Test_FM_VerifyDirExists_FilenameOpen,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyDirExists_FilenameOpen");

    UtTest_Add(Test_FM_VerifyDirExists_FilenameClosed,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyDirExists_FilenameClosed");

    UtTest_Add(Test_FM_VerifyDirExists_FilenameIsDirectory,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyDirExists_FilenameIsDirectory");

    UtTest_Add(Test_FM_VerifyDirExists_FilenameDefault,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyDirExists_FilenameDefault");
}

void add_FM_VerifyDirNoExist_tests(void)
{
    UtTest_Add(Test_FM_VerifyDirNoExist_FilenameInvalid,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyDirNoExist_FilenameInvalid");

    UtTest_Add(Test_FM_VerifyDirNoExist_FilenameNotInUse,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyDirNoExist_FilenameNotInUse");

    UtTest_Add(Test_FM_VerifyDirNoExist_FilenameOpen,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyDirNoExist_FilenameOpen");

    UtTest_Add(Test_FM_VerifyDirNoExist_FilenameClosed,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyDirNoExist_FilenameClosed");

    UtTest_Add(Test_FM_VerifyDirNoExist_FilenameIsDirectory,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyDirNoExist_FilenameIsDirectory");

    UtTest_Add(Test_FM_VerifyDirNoExist_FilenameDefault,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyDirNoExist_FilenameDefault");
}

void add_FM_VerifyChildTask_tests(void)
{
    UtTest_Add(Test_FM_VerifyChildTask_ChildSemaphoreIsInvalid,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyChildTask_ChildSemaphoreIsInvalid");
    
    UtTest_Add(Test_FM_VerifyChildTask_LocalQueueCountEqualToQueueDepth,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyChildTask_LocalQueueCountEqualToQueueDepth");

    UtTest_Add(Test_FM_VerifyChildTask_LocalQueueCountGreaterQueueDepth,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyChildTask_LocalQueueCountGreaterQueueDepth");

    UtTest_Add(Test_FM_VerifyChildTask_ChildWriteIndexGreaterChildQDepth,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyChildTask_ChildWriteIndexGreaterChildQDepth");

    UtTest_Add(Test_FM_VerifyChildTask_ChildWriteIndexEqualChildQDepth,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyChildTask_ChildWriteIndexEqualChildQDepth");

    UtTest_Add(Test_FM_VerifyChildTask_Default,
        fm_UT_Setup, fm_UT_TearDown,
        "Test_FM_VerifyChildTask_Default");
}

void add_FM_InvokeChildTask_tests(void)
{
    UtTest_Add(FM_InvokeChildTask_WriteIndexGreaterThanChildQueue,
        fm_UT_Setup, fm_UT_TearDown,
        "FM_InvokeChildTask_WriteIndexGreaterThanChildQueue");
   
    UtTest_Add(FM_InvokeChildTask_ChildSemaphoreNotEqualToSEMINVALID,
	    fm_UT_Setup, fm_UT_TearDown,
	    "FM_InvokeChildTask_ChildSemaphoreNotEqualToSEMINVALID");

}

void add_FM_AppendPathSep_tests(void)
{
    UtTest_Add(FM_AppendPathSep_StringLengthZero,
        fm_UT_Setup, fm_UT_TearDown,
        "FM_AppendPathSep_StringLengthZero");	

    UtTest_Add(FM_AppendPathSep_StringLengthSmallerThanBuffer,
	    fm_UT_Setup, fm_UT_TearDown,
    	"FM_AppendPathSep_StringLengthSmallerThanBuffer");

    UtTest_Add(FM_AppendPathSep_StringEndWithPathSeparator,
	    fm_UT_Setup, fm_UT_TearDown,
    	"FM_AppendPathSep_StringEndWithPathSeparator");

    UtTest_Add(FM_AppendPathSep_StringEndWithoutSeparator,
        fm_UT_Setup, fm_UT_TearDown,
        "FM_AppendPathSep_StringEndWithoutSeparator");
}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    add_FM_IsValidCmdPktLength_tests();
    add_FM_VerifyOverwrite_tests();
    //add_LoadOpenFileData_tests();
    add_FM_GetOpenFilesData_tests();
    add_FM_GetOpenFilesData_tests();
    add_FM_GetFilenameState_tests();
    add_FM_VerifyNameValid_tests();
    add_FM_VerifyFileClosed_tests();
    add_FM_VerifyFileExists_tests();
    add_FM_VerifyFileNoExist_tests();
    add_FM_VerifyFileNotOpen_tests();
    add_FM_VerifyDirExists_tests();
    add_FM_VerifyDirNoExist_tests();
    add_FM_VerifyChildTask_tests();
    add_FM_InvokeChildTask_tests();
    add_FM_AppendPathSep_tests();
}

