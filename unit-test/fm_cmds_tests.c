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

/*
 * UT Assert
 */
#include "fm_test_utils.c"

/*
 * UT includes
 */
#include "uttest.h"
#include "utassert.h"
#include "utstubs.h"

uint8 call_count_CFE_EVS_SendEvent;

/*
**********************************************************************************
**          TEST CASE FUNCTIONS
**********************************************************************************
*/
/******************/
/* Noop Tests     */
/******************/

void Test_FM_NoopCmd_Success(void)
{
    CFE_SB_Buffer_t   DummyBuf;
    int32           strCmpResult;
    char            ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, 
             "%%s command: FM version %%d.%%d.%%d.%%d");

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);

    bool Result = FM_NoopCmd(&DummyBuf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_NoopCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, FM_NOOP_CMD_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType, CFE_EVS_EventType_INFORMATION);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent.Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent.Spec);

}

void Test_FM_NoopCmd_BadLength(void)
{
    CFE_SB_Buffer_t   DummyBuf;
     
    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);

    bool Result = FM_NoopCmd(&DummyBuf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_NoopCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
}

void add_FM_NoopCmd_tests(void)
{
    UtTest_Add(Test_FM_NoopCmd_Success,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_NoopCmd_Success");

    UtTest_Add(Test_FM_NoopCmd_BadLength,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_NoopCmd_BadLength");
}

/****************************/
/* Reset Counters Tests     */
/****************************/

void Test_FM_ResetCountersCmd_Success(void)
{
    CFE_SB_Buffer_t   DummyBuf;
    int32           strCmpResult;
    char            ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, 
             "%%s command");

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);

    FM_GlobalData.CommandCounter = 1;
    FM_GlobalData.CommandErrCounter = 1;
    FM_GlobalData.ChildCmdCounter = 1;
    FM_GlobalData.ChildCmdErrCounter = 1;
    FM_GlobalData.ChildCmdWarnCounter = 1;

    bool Result = FM_ResetCountersCmd(&DummyBuf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_ResetCountersCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, FM_RESET_CMD_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType, CFE_EVS_EventType_DEBUG);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent.Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent.Spec);

    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCmdCounter, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCmdErrCounter, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCmdWarnCounter, 0);
}

void Test_FM_ResetCountersCmd_BadLength(void)
{
    CFE_SB_Buffer_t   DummyBuf;
     
    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);
    
    FM_GlobalData.CommandCounter = 1;
    FM_GlobalData.CommandErrCounter = 1;
    FM_GlobalData.ChildCmdCounter = 1;
    FM_GlobalData.ChildCmdErrCounter = 1;
    FM_GlobalData.ChildCmdWarnCounter = 1;

    bool Result = FM_ResetCountersCmd(&DummyBuf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_NoopCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCmdCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCmdErrCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCmdWarnCounter, 1);

}

void add_FM_ResetCountersCmd_tests(void)
{
    UtTest_Add(Test_FM_ResetCountersCmd_Success,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_ResetCountersCmd_Success");

    UtTest_Add(Test_FM_ResetCountersCmd_BadLength,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_ResetCountersCmd_BadLength");
}

/****************************/
/* Copy File Tests     */
/****************************/

void Test_FM_CopyFileCmd_Success(void)
{
    FM_CopyFileCmd_t  CmdPkt;
    CmdPkt.Overwrite = 0;
    strncpy(CmdPkt.Source, "src1", OS_MAX_PATH_LEN);
    strncpy(CmdPkt.Target, "tgt", OS_MAX_PATH_LEN);
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_CopyFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_CopyFileCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_COPY_CC);
}

void Test_FM_CopyFileCmd_BadLength(void)
{
    FM_CopyFileCmd_t  CmdPkt;
    CmdPkt.Overwrite = 0;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_CopyFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_CopyFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_CopyFileCmd_BadOverwrite(void)
{
    FM_CopyFileCmd_t  CmdPkt;
    CmdPkt.Overwrite = 0;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_CopyFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_CopyFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);

}

void Test_FM_CopyFileCmd_SourceNotExist(void)
{
    FM_CopyFileCmd_t  CmdPkt;
    CmdPkt.Overwrite = 0;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_CopyFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_CopyFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);

}

void Test_FM_CopyFileCmd_NoOverwriteTargetExists(void)
{
    FM_CopyFileCmd_t  CmdPkt;
    CmdPkt.Overwrite = 0;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_CopyFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_CopyFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);

}

void Test_FM_CopyFileCmd_OverwriteFileOpen(void)
{
    FM_CopyFileCmd_t  CmdPkt;
    CmdPkt.Overwrite = 1;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_CopyFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_CopyFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);

}

void Test_FM_CopyFileCmd_NoChildTask(void)
{
    FM_CopyFileCmd_t  CmdPkt;
    CmdPkt.Overwrite = 0;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    bool Result = FM_CopyFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_CopyFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);


}

void add_FM_CopyFileCmd_tests(void)
{
    UtTest_Add(Test_FM_CopyFileCmd_Success,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_CopyFileCmd_Success");

    UtTest_Add(Test_FM_CopyFileCmd_BadLength,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_CopyFileCmd_BadLength");

    UtTest_Add(Test_FM_CopyFileCmd_BadOverwrite,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_CopyFileCmd_BadOverwrite");
    
    UtTest_Add(Test_FM_CopyFileCmd_SourceNotExist,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_CopyFileCmd_SourceNotExist");

    UtTest_Add(Test_FM_CopyFileCmd_NoOverwriteTargetExists,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_CopyFileCmd_NoOverwriteTargetExists");

    UtTest_Add(Test_FM_CopyFileCmd_OverwriteFileOpen,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_CopyFileCmd_OverwriteFileOpen");

    UtTest_Add(Test_FM_CopyFileCmd_NoChildTask,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_CopyFileCmd_NoChildTask");

}

/****************************/
/* Move File Tests          */
/****************************/

void Test_FM_MoveFileCmd_Success(void)
{
    FM_MoveFileCmd_t  CmdPkt;
    CmdPkt.Overwrite = 0;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_MoveFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_MoveFileCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_MOVE_CC);
}

void Test_FM_MoveFileCmd_BadLength(void)
{
    FM_MoveFileCmd_t  CmdPkt;
    CmdPkt.Overwrite = 0;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_MoveFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_MoveFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_MoveFileCmd_BadOverwrite(void)
{
    FM_MoveFileCmd_t  CmdPkt;
    CmdPkt.Overwrite = 0;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_MoveFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_MoveFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);

}

void Test_FM_MoveFileCmd_SourceNotExist(void)
{
    FM_MoveFileCmd_t  CmdPkt;
    CmdPkt.Overwrite = 0;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_MoveFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_MoveFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);

}

void Test_FM_MoveFileCmd_NoOverwriteTargetExists(void)
{
    FM_MoveFileCmd_t  CmdPkt;
    CmdPkt.Overwrite = 0;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_MoveFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_MoveFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);

}

void Test_FM_MoveFileCmd_OverwriteFileOpen(void)
{
    FM_MoveFileCmd_t  CmdPkt;
    CmdPkt.Overwrite = 1;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_MoveFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_MoveFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);

}

void Test_FM_MoveFileCmd_NoChildTask(void)
{
    FM_MoveFileCmd_t  CmdPkt;
    CmdPkt.Overwrite = 0;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    bool Result = FM_MoveFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_MoveFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);


}

void add_FM_MoveFileCmd_tests(void)
{
    UtTest_Add(Test_FM_MoveFileCmd_Success,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_MoveFileCmd_Success");

    UtTest_Add(Test_FM_MoveFileCmd_BadLength,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_MoveFileCmd_BadLength");

    UtTest_Add(Test_FM_MoveFileCmd_BadOverwrite,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_MoveFileCmd_BadOverwrite");
    
    UtTest_Add(Test_FM_MoveFileCmd_SourceNotExist,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_MoveFileCmd_SourceNotExist");

    UtTest_Add(Test_FM_MoveFileCmd_NoOverwriteTargetExists,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_MoveFileCmd_NoOverwriteTargetExists");

    UtTest_Add(Test_FM_MoveFileCmd_OverwriteFileOpen,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_MoveFileCmd_OverwriteFileOpen");

    UtTest_Add(Test_FM_MoveFileCmd_NoChildTask,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_MoveFileCmd_NoChildTask");

}


/****************************/
/* Rename File Tests        */
/****************************/

void Test_FM_RenameFileCmd_Success(void)
{
    FM_RenameFileCmd_t  CmdPkt;
    strncpy(CmdPkt.Source, "src1", OS_MAX_PATH_LEN);
    strncpy(CmdPkt.Target, "tgt", OS_MAX_PATH_LEN);

    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_RenameFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_RenameFileCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_RENAME_CC);
}

void Test_FM_RenameFileCmd_BadLength(void)
{
    FM_RenameFileCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_RenameFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_RenameFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_RenameFileCmd_SourceNotExist(void)
{
    FM_RenameFileCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_RenameFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_RenameFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);

}

void Test_FM_RenameFileCmd_TargetExists(void)
{
    FM_RenameFileCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_MoveFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_RenameFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);

}

void Test_FM_RenameFileCmd_NoChildTask(void)
{
    FM_RenameFileCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    bool Result = FM_RenameFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_RenameFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);


}

void add_FM_RenameFileCmd_tests(void)
{
    UtTest_Add(Test_FM_RenameFileCmd_Success,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_RenameFileCmd_Success");

    UtTest_Add(Test_FM_RenameFileCmd_BadLength,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_RenameFileCmd_BadLength");
    
    UtTest_Add(Test_FM_RenameFileCmd_SourceNotExist,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_RenameFileCmd_SourceNotExist");

    UtTest_Add(Test_FM_RenameFileCmd_TargetExists,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_RenameFileCmd_TargetExists");

    UtTest_Add(Test_FM_RenameFileCmd_NoChildTask,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_RenameFileCmd_NoChildTask");
}

/****************************/
/* Delete File Tests        */
/****************************/

void Test_FM_DeleteFileCmd_Success(void)
{
    FM_DeleteFileCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    CFE_MSG_FcnCode_t forced_CmdCode = FM_DELETE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &forced_CmdCode, sizeof(forced_CmdCode), false);

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_DeleteFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_DeleteFileCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_DELETE_CC);
}

void Test_FM_DeleteFileCmd_BadLength(void)
{
    FM_DeleteFileCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    CFE_MSG_FcnCode_t forced_CmdCode = FM_DELETE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &forced_CmdCode, sizeof(forced_CmdCode), false);

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_DeleteFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DeleteFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_DeleteFileCmd_FileNotClosed(void)
{
    FM_DeleteFileCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    CFE_MSG_FcnCode_t forced_CmdCode = FM_DELETE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &forced_CmdCode, sizeof(forced_CmdCode), false);

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_DeleteFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DeleteFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_DeleteFileCmd_NoChildTask(void)
{
    FM_DeleteFileCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    CFE_MSG_FcnCode_t forced_CmdCode = FM_DELETE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &forced_CmdCode, sizeof(forced_CmdCode), false);

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    bool Result = FM_DeleteFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DeleteFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_DeleteFileCmd_tests(void)
{
    UtTest_Add(Test_FM_DeleteFileCmd_Success,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_DeleteFileCmd_Success");

    UtTest_Add(Test_FM_DeleteFileCmd_BadLength,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_DeleteFileCmd_BadLength");
    
    UtTest_Add(Test_FM_DeleteFileCmd_FileNotClosed,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_DeleteFileCmd_FileNotClosed");

    UtTest_Add(Test_FM_DeleteFileCmd_NoChildTask,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_DeleteFileCmd_NoChildTask");
}

/****************************/
/* Delete All Files Tests   */
/****************************/

void Test_FM_DeleteAllFilesCmd_Success(void)
{
    FM_DeleteAllCmd_t  CmdPkt;
    strncpy(CmdPkt.Directory, "dir", OS_MAX_PATH_LEN);

    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_DeleteAllFilesCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_DeleteAllFilesCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_DELETE_ALL_CC);
}

void Test_FM_DeleteAllFilesCmd_BadLength(void)
{
    FM_DeleteAllCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_DeleteAllFilesCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DeleteAllFilesCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_DeleteAllFilesCmd_DirNoExist(void)
{
    FM_DeleteAllCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_DeleteAllFilesCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DeleteAllFilesCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_DeleteAllFilesCmd_NoChildTask(void)
{
    FM_DeleteAllCmd_t  CmdPkt;
    strncpy(CmdPkt.Directory, "dir", OS_MAX_PATH_LEN);
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    bool Result = FM_DeleteAllFilesCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DeleteAllFilesCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_DeleteAllFilesCmd_tests(void)
{
    UtTest_Add(Test_FM_DeleteAllFilesCmd_Success,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_DeleteAllFilesCmd_Success");

    UtTest_Add(Test_FM_DeleteAllFilesCmd_BadLength,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_DeleteAllFilesCmd_BadLength");
    
    UtTest_Add(Test_FM_DeleteAllFilesCmd_DirNoExist,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_DeleteAllFilesCmd_DirNoExist");

    UtTest_Add(Test_FM_DeleteAllFilesCmd_NoChildTask,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_DeleteAllFilesCmd_NoChildTask");
}

#ifdef FM_INCLUDE_DECOMPRESS
/****************************/
/* Decompress File Test s   */
/****************************/

void Test_FM_DecompressFileCmd_Success(void)
{
    FM_DecompressCmd_t  CmdPkt;

        FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_DecompressFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_DecompressFileCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_DECOMPRESS_CC);
}

void Test_FM_DecompressFileCmd_BadLength(void)
{
    FM_DecompressCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_DecompressFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DecompressFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_DecompressFileCmd_SourceFileOpen(void)
{
    FM_DecompressCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_DecompressFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DecompressFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_DecompressFileCmd_TargetFileExists(void)
{
    FM_DecompressCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_DecompressFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DecompressFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_DecompressFileCmd_NoChildTask(void)
{
    FM_DecompressCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    bool Result = FM_DecompressFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DecompressFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_DecompressFileCmd_tests(void)
{
    UtTest_Add(Test_FM_DecompressFileCmd_Success,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_DecompressFileCmd_Success");

    UtTest_Add(Test_FM_DecompressFileCmd_BadLength,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_DecompressFileCmd_BadLength");
    
    UtTest_Add(Test_FM_DecompressFileCmd_SourceFileOpen,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_DecompressFileCmd_SourceFileOpen");
 
    UtTest_Add(Test_FM_DecompressFileCmd_TargetFileExists,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_DecompressFileCmd_TargetFileExists");

    UtTest_Add(Test_FM_DecompressFileCmd_NoChildTask,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_DecompressFileCmd_NoChildTask");
}

#endif

/****************************/
/* Concat Files Tests       */
/****************************/

void Test_FM_ConcatFilesCmd_Success(void)
{
    FM_ConcatCmd_t  CmdPkt;
    strncpy(CmdPkt.Source1, "src1", OS_MAX_PATH_LEN);
    strncpy(CmdPkt.Source2, "src2", OS_MAX_PATH_LEN);
    strncpy(CmdPkt.Target, "tgt", OS_MAX_PATH_LEN);

    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_ConcatFilesCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_ConcatFilesCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_CONCAT_CC);
}

void Test_FM_ConcatFilesCmd_BadLength(void)
{
    FM_ConcatCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_ConcatFilesCmd((CFE_SB_Buffer_t*)&CmdPkt);
    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_ConcatFilesCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_ConcatFilesCmd_SourceFile1NotClosed(void)
{
    FM_ConcatCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_ConcatFilesCmd((CFE_SB_Buffer_t*)&CmdPkt);
    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_ConcatFilesCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_ConcatFilesCmd_SourceFile2NotClosed(void)
{
    FM_ConcatCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDeferredRetcode(UT_KEY(FM_VerifyFileClosed), 2, false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_ConcatFilesCmd((CFE_SB_Buffer_t*)&CmdPkt);
    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_ConcatFilesCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_ConcatFilesCmd_TargetFileExists(void)
{
    FM_ConcatCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_ConcatFilesCmd((CFE_SB_Buffer_t*)&CmdPkt);
    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_ConcatFilesCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_ConcatFilesCmd_NoChildTask(void)
{
    FM_ConcatCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    bool Result = FM_ConcatFilesCmd((CFE_SB_Buffer_t*)&CmdPkt);
    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_ConcatFilesCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_ConcatFilesCmd_tests(void)
{
    UtTest_Add(Test_FM_ConcatFilesCmd_Success,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_ConcatFilesCmd_Success");

    UtTest_Add(Test_FM_ConcatFilesCmd_BadLength,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_ConcatFilesCmd_BadLength");
    
    UtTest_Add(Test_FM_ConcatFilesCmd_SourceFile1NotClosed,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_ConcatFilesCmd_SourceFile1NotClosed");
 
    UtTest_Add(Test_FM_ConcatFilesCmd_SourceFile2NotClosed,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_ConcatFilesCmd_SourceFile2NotClosed");

    UtTest_Add(Test_FM_ConcatFilesCmd_TargetFileExists,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_ConcatFilesCmd_TargetFileExists");

    UtTest_Add(Test_FM_ConcatFilesCmd_NoChildTask,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_ConcatFilesCmd_NoChildTask");
}

/****************************/
/* Get File Info Tests      */
/****************************/

void Test_FM_GetFileInfoCmd_Success(void)
{
    FM_GetFileInfoCmd_t  CmdPkt;
    strncpy(CmdPkt.Filename, "file", OS_MAX_PATH_LEN);

    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyNameValid), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_GetFileInfoCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_GetFileInfoCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_GET_FILE_INFO_CC);
}

void Test_FM_GetFileInfoCmd_BadLength(void)
{
    FM_GetFileInfoCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyNameValid), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_GetFileInfoCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_GetFileInfoCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_GetFileInfoCmd_InvalidName(void)
{
    FM_GetFileInfoCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyNameValid), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_GetFileInfoCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_GetFileInfoCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_GetFileInfoCmd_NoChildTask(void)
{
    FM_GetFileInfoCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyNameValid), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    bool Result = FM_GetFileInfoCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_GetFileInfoCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_GetFileInfoCmd_tests(void)
{
    UtTest_Add(Test_FM_GetFileInfoCmd_Success,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_GetFileInfoCmd_Success");

    UtTest_Add(Test_FM_GetFileInfoCmd_BadLength,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_GetFileInfoCmd_BadLength");
   
    UtTest_Add(Test_FM_GetFileInfoCmd_InvalidName,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_GetFileInfoCmd_InvalidName");

    UtTest_Add(Test_FM_GetFileInfoCmd_NoChildTask,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_GetFileInfoCmd_NoChildTask");
}


/****************************/
/* Get Open Files Tests     */
/****************************/

void Test_FM_GetOpenFilesCmd_Success(void)
{
    FM_GetOpenFilesCmd_t  CmdPkt;
    int32           strCmpResult;
    char            ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, 
             "%%s command");

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

 
    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);

    bool Result = FM_GetOpenFilesCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_GetOpenFilesCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, FM_GET_OPEN_FILES_CMD_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType, CFE_EVS_EventType_DEBUG);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent.Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent.Spec);


}

void Test_FM_GetOpenFilesCmd_BadLength(void)
{
    FM_GetOpenFilesCmd_t  CmdPkt;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);

    bool Result = FM_GetOpenFilesCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_GetFileInfoCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
}

void add_FM_GetOpenFilesCmd_tests(void)
{
    UtTest_Add(Test_FM_GetOpenFilesCmd_Success,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_GetOpenFilesCmd_Success");

    UtTest_Add(Test_FM_GetOpenFilesCmd_BadLength,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_GetOpenFilesCmd_BadLength");
}


/****************************/
/* Create Directory Tests   */
/****************************/

void Test_FM_CreateDirectoryCmd_Success(void)
{
    FM_CreateDirCmd_t  CmdPkt;

    strncpy(CmdPkt.Directory, "dir", OS_MAX_PATH_LEN);
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_CreateDirectoryCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_CreateDirectoryCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_CREATE_DIR_CC);
}

void Test_FM_CreateDirectoryCmd_BadLength(void)
{
    FM_CreateDirCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_CreateDirectoryCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_CreateDirectoryCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_CreateDirectoryCmd_DirExists(void)
{
    FM_CreateDirCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirNoExist), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_CreateDirectoryCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_CreateDirectoryCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_CreateDirectoryCmd_NoChildTask(void)
{
    FM_CreateDirCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    bool Result = FM_CreateDirectoryCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_CreateDirectoryCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_CreateDirectoryCmd_tests(void)
{
    UtTest_Add(Test_FM_CreateDirectoryCmd_Success,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_CreateDirectoryCmd_Success");

    UtTest_Add(Test_FM_CreateDirectoryCmd_BadLength,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_CreateDirectoryCmd_BadLength");
    
    UtTest_Add(Test_FM_CreateDirectoryCmd_DirExists,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_CreateDirectoryCmd_DirExists");

    UtTest_Add(Test_FM_CreateDirectoryCmd_NoChildTask,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_CreateDirectoryCmd_NoChildTask");
}


/****************************/
/* Delete Directory Tests   */
/****************************/

void Test_FM_DeleteDirectoryCmd_Success(void)
{
    FM_DeleteDirCmd_t  CmdPkt;
    strncpy(CmdPkt.Directory, "dir", OS_MAX_PATH_LEN);
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_DeleteDirectoryCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_DeleteDirectoryCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_DELETE_DIR_CC);
}

void Test_FM_DeleteDirectoryCmd_BadLength(void)
{
    FM_DeleteDirCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_DeleteDirectoryCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DeleteDirectoryCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_DeleteDirectoryCmd_DirNoExist(void)
{
    FM_DeleteDirCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_DeleteDirectoryCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DeleteDirectoryCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_DeleteDirectoryCmd_NoChildTask(void)
{
    FM_DeleteDirCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    bool Result = FM_DeleteDirectoryCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DeleteDirectoryCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_DeleteDirectoryCmd_tests(void)
{
    UtTest_Add(Test_FM_DeleteDirectoryCmd_Success,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_DeleteDirectoryCmd_Success");

    UtTest_Add(Test_FM_DeleteDirectoryCmd_BadLength,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_DeleteDirectoryCmd_BadLength");
    
    UtTest_Add(Test_FM_DeleteDirectoryCmd_DirNoExist,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_DeleteDirectoryCmd_DirNoExist");

    UtTest_Add(Test_FM_DeleteDirectoryCmd_NoChildTask,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_DeleteDirectoryCmd_NoChildTask");
}

/****************************/
/* Get Dir List File Tests  */
/****************************/

void Test_FM_GetDirListFileCmd_Success(void)
{
    FM_GetDirFileCmd_t  CmdPkt;

    strncpy(CmdPkt.Filename, "file", OS_MAX_PATH_LEN);
    strncpy(CmdPkt.Directory, "dir", OS_MAX_PATH_LEN);
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_GetDirListFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_GetDirListFileCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_GET_DIR_FILE_CC);
}

void Test_FM_GetDirListFileCmd_SuccessDefaultPath(void)
{
    FM_GetDirFileCmd_t  CmdPkt;

    strncpy(CmdPkt.Directory, "dir", OS_MAX_PATH_LEN);
    CmdPkt.Filename[0] = '\0';
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_GetDirListFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_GetDirListFileCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_GET_DIR_FILE_CC);
}

void Test_FM_GetDirListFileCmd_BadLength(void)
{
    FM_GetDirFileCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_GetDirListFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_GetDirListFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_GetDirListFileCmd_SourceNotExist(void)
{
    FM_GetDirFileCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_GetDirListFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_GetDirListFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_GetDirListFileCmd_TargetFileOpen(void)
{
    FM_GetDirFileCmd_t  CmdPkt;
    strncpy(CmdPkt.Filename, "file", OS_MAX_PATH_LEN);
    strncpy(CmdPkt.Directory, "dir", OS_MAX_PATH_LEN);

    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_GetDirListFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_GetDirListFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_GetDirListFileCmd_NoChildTask(void)
{
    FM_GetDirFileCmd_t  CmdPkt;
    strncpy(CmdPkt.Filename, "file", OS_MAX_PATH_LEN);
    strncpy(CmdPkt.Directory, "dir", OS_MAX_PATH_LEN);

    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    bool Result = FM_GetDirListFileCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_GetDirListFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_GetDirListFileCmd_tests(void)
{
    UtTest_Add(Test_FM_GetDirListFileCmd_Success,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_GetDirListFileCmd_Success");

    UtTest_Add(Test_FM_GetDirListFileCmd_SuccessDefaultPath,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_GetDirListFileCmd_SuccessDefaultPath");

    UtTest_Add(Test_FM_GetDirListFileCmd_BadLength,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_GetDirListFileCmd_BadLength");
    
    UtTest_Add(Test_FM_GetDirListFileCmd_SourceNotExist,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_GetDirListFileCmd_SourceNotExist");

    UtTest_Add(Test_FM_GetDirListFileCmd_TargetFileOpen,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_GetDirListFileCmd_TargetFileOpen");

    UtTest_Add(Test_FM_GetDirListFileCmd_NoChildTask,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_GetDirListFileCmd_NoChildTask");
}


/****************************/
/* Get Dir List Pkt Tests  */
/****************************/

void Test_FM_GetDirListPktCmd_Success(void)
{
    FM_GetDirPktCmd_t  CmdPkt;
    strncpy(CmdPkt.Directory, "dir", OS_MAX_PATH_LEN);

    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_GetDirListPktCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_GetDirListPktCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_GET_DIR_PKT_CC);
}

void Test_FM_GetDirListPktCmd_BadLength(void)
{
    FM_GetDirPktCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_GetDirListPktCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_GetDirListPktCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_GetDirListPktCmd_SourceNotExist(void)
{
    FM_GetDirPktCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_GetDirListPktCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_GetDirListPktCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_GetDirListPktCmd_NoChildTask(void)
{
    FM_GetDirPktCmd_t  CmdPkt;
    FM_GlobalData.ChildWriteIndex = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    bool Result = FM_GetDirListPktCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_GetDirListPktCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_GetDirListPktCmd_tests(void)
{
    UtTest_Add(Test_FM_GetDirListPktCmd_Success,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_GetDirListPktCmd_Success");

    UtTest_Add(Test_FM_GetDirListPktCmd_BadLength,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_GetDirListPktCmd_BadLength");
    
    UtTest_Add(Test_FM_GetDirListPktCmd_SourceNotExist,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_GetDirListPktCmd_SourceNotExist");

    UtTest_Add(Test_FM_GetDirListPktCmd_NoChildTask,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_GetDirListPktCmd_NoChildTask");
}


/****************************/
/* Get Free Space Tests     */
/****************************/

void Test_FM_GetFreeSpaceCmd_Success(void)
{
    CFE_SB_Buffer_t     DummyBuf;
    int32           strCmpResult;
    char            ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, 
             "%%s command");

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    FM_FreeSpaceTable_t DummyTable;
    OS_statvfs_t        forced_FileStats;
    forced_FileStats.blocks_free = 10;

    for(int i = 0; i < FM_TABLE_ENTRY_COUNT; i++)
    {
        DummyTable.FileSys[i].State = FM_TABLE_ENTRY_DISABLED;
        strncpy(DummyTable.FileSys[i].Name, "sys", OS_MAX_PATH_LEN);
    }
    DummyTable.FileSys[0].State = FM_TABLE_ENTRY_ENABLED;

    FM_GlobalData.FreeSpaceTablePtr = &DummyTable;

    UT_SetDataBuffer(UT_KEY(OS_FileSysStatVolume), &forced_FileStats,
                     sizeof(forced_FileStats), false);

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_GetFreeSpaceCmd(&DummyBuf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, FM_GET_FREE_SPACE_CMD_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType, CFE_EVS_EventType_DEBUG);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent.Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent.Spec);


    uint8 call_count_CFE_SB_TransmitMsg = UT_GetStubCount(UT_KEY(CFE_SB_TransmitMsg));

    /* Assert */
    UtAssert_True(Result == true, "FM_GetFreeSpaceCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(call_count_CFE_SB_TransmitMsg, 1);
    UtAssert_INT32_EQ(FM_GlobalData.FreeSpacePkt.FileSys[0].FreeSpace, 10);
}

void Test_FM_GetFreeSpaceCmd_BadLength(void)
{
    CFE_SB_Buffer_t     DummyBuf;
    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);
    
    bool Result = FM_GetFreeSpaceCmd(&DummyBuf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 call_count_CFE_SB_TransmitMsg = UT_GetStubCount(UT_KEY(CFE_SB_TransmitMsg));

    /* Assert */
    UtAssert_True(Result == false, "FM_GetFreeSpaceCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(call_count_CFE_SB_TransmitMsg, 0);
}

void Test_FM_GetFreeSpaceCmd_NullFreeSpaceTable(void)
{
    CFE_SB_Buffer_t     DummyBuf;
    int32           strCmpResult;
    char            ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, 
             "%%s error: file system free space table is not loaded");

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);


    FM_GlobalData.FreeSpaceTablePtr = NULL;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);

    bool Result = FM_GetFreeSpaceCmd(&DummyBuf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 call_count_CFE_SB_TransmitMsg = UT_GetStubCount(UT_KEY(CFE_SB_TransmitMsg));

    /* Assert */
    UtAssert_True(Result == false, "FM_GetFreeSpaceCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, FM_GET_FREE_SPACE_TBL_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent.Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent.Spec);


    UtAssert_INT32_EQ(call_count_CFE_SB_TransmitMsg, 0);

}

void Test_FM_GetFreeSpaceCmd_OSFileSysStatVolumeFails(void)
{
    CFE_SB_Buffer_t     DummyBuf;
    int32           strCmpResult;
    char            ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    char            ExpectedEventString2[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, 
             "Could not get file system free space for %%s. Returned 0x%%08X");
    snprintf(ExpectedEventString2, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, 
             "%%s command");

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent[2];
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, context_CFE_EVS_SendEvent);

    FM_FreeSpaceTable_t DummyTable;
    
    for(int i = 0; i < FM_TABLE_ENTRY_COUNT; i++)
    {
        DummyTable.FileSys[i].State = FM_TABLE_ENTRY_DISABLED;
        strncpy(DummyTable.FileSys[i].Name, "sys", OS_MAX_PATH_LEN);
    }
    DummyTable.FileSys[0].State = FM_TABLE_ENTRY_ENABLED;

    FM_GlobalData.FreeSpaceTablePtr = &DummyTable;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(OS_FileSysStatVolume), OS_ERROR);

    bool Result = FM_GetFreeSpaceCmd(&DummyBuf);

    /* Assert */
    UtAssert_True(Result == true, "FM_GetFreeSpaceCmd returned true");

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_OS_SYS_STAT_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_GET_FREE_SPACE_CMD_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_DEBUG);

    strCmpResult = strncmp(ExpectedEventString2, context_CFE_EVS_SendEvent[1].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[1].Spec);

    uint8 call_count_CFE_SB_TransmitMsg = UT_GetStubCount(UT_KEY(CFE_SB_TransmitMsg));

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(call_count_CFE_SB_TransmitMsg, 1);
    UtAssert_INT32_EQ(FM_GlobalData.FreeSpacePkt.FileSys[0].FreeSpace, 0);
}

void add_FM_GetFreeSpaceCmd_tests(void)
{
    UtTest_Add(Test_FM_GetFreeSpaceCmd_Success,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_GetFreeSpaceCmd_Success");

    UtTest_Add(Test_FM_GetFreeSpaceCmd_BadLength,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_GetFreeSpaceCmd_BadLength");
    
    UtTest_Add(Test_FM_GetFreeSpaceCmd_NullFreeSpaceTable,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_GetFreeSpaceCmd_NullFreeSpaceTable");

    UtTest_Add(Test_FM_GetFreeSpaceCmd_OSFileSysStatVolumeFails,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_GetFreeSpaceCmd_OSFileSysStatVolumeFails");

}

/****************************/
/* Set Table State Cmd      */
/****************************/

void Test_FM_SetTableStateCmd_Success(void)
{
    FM_SetTableStateCmd_t CmdPkt;
    CmdPkt.TableEntryState = FM_TABLE_ENTRY_ENABLED;
    CmdPkt.TableEntryIndex = 0;

    int32           strCmpResult;
    char            ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, 
             "%%s command: index = %%d, state = %%d");

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
   
    FM_FreeSpaceTable_t DummyTable;

    for(int i = 0; i < FM_TABLE_ENTRY_COUNT; i++)
    {
        DummyTable.FileSys[i].State = FM_TABLE_ENTRY_DISABLED;
    }

    FM_GlobalData.FreeSpaceTablePtr = &DummyTable;

    bool Result = FM_SetTableStateCmd((CFE_SB_Buffer_t*)&CmdPkt);

    /* Assert */
    UtAssert_True(Result == true, "FM_SetTableStateCmd returned true");

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, FM_SET_TABLE_STATE_CMD_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType, CFE_EVS_EventType_INFORMATION);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent.Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent.Spec);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(FM_GlobalData.FreeSpaceTablePtr->FileSys[0].State,
                      FM_TABLE_ENTRY_ENABLED);
}

void Test_FM_SetTableStateCmd_BadLength(void)
{
    FM_SetTableStateCmd_t CmdPkt;
    CmdPkt.TableEntryState = FM_TABLE_ENTRY_ENABLED;
    CmdPkt.TableEntryIndex = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);
    
    bool Result = FM_SetTableStateCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_SetTableStateCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
}

void Test_FM_SetTableStateCmd_NullFreeSpaceTable(void)
{
    FM_SetTableStateCmd_t CmdPkt;
    CmdPkt.TableEntryState = FM_TABLE_ENTRY_ENABLED;
    CmdPkt.TableEntryIndex = 0;

    int32           strCmpResult;
    char            ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, 
             "%%s error: file system free space table is not loaded");

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    FM_GlobalData.FreeSpaceTablePtr = NULL;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);

    bool Result = FM_SetTableStateCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_SetTableStateCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, FM_SET_TABLE_STATE_TBL_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent.Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent.Spec);

}

void Test_FM_SetTableStateCmd_TableEntryIndexTooLarge(void)
{
    FM_SetTableStateCmd_t CmdPkt;
    CmdPkt.TableEntryState = FM_TABLE_ENTRY_ENABLED;
    CmdPkt.TableEntryIndex = FM_TABLE_ENTRY_COUNT;

    int32           strCmpResult;
    char            ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, 
             "%%s error: invalid command argument: index = %%d");

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
   
    FM_FreeSpaceTable_t DummyTable;

    for(int i = 0; i < FM_TABLE_ENTRY_COUNT; i++)
    {
        DummyTable.FileSys[i].State = FM_TABLE_ENTRY_DISABLED;
    }

    FM_GlobalData.FreeSpaceTablePtr = &DummyTable;

    bool Result = FM_SetTableStateCmd((CFE_SB_Buffer_t*)&CmdPkt);

    /* Assert */
    UtAssert_True(Result == false, "FM_SetTableStateCmd returned false");

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, FM_SET_TABLE_STATE_ARG_IDX_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent.Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent.Spec);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
}

void Test_FM_SetTableStateCmd_BadNewState(void)
{
    FM_SetTableStateCmd_t CmdPkt;
    CmdPkt.TableEntryState = FM_TABLE_ENTRY_UNUSED;
    CmdPkt.TableEntryIndex = 0;

    int32           strCmpResult;
    char            ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, 
             "%%s error: invalid command argument: state = %%d");

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
   
    FM_FreeSpaceTable_t DummyTable;

    for(int i = 0; i < FM_TABLE_ENTRY_COUNT; i++)
    {
        DummyTable.FileSys[i].State = FM_TABLE_ENTRY_DISABLED;
    }

    FM_GlobalData.FreeSpaceTablePtr = &DummyTable;

    bool Result = FM_SetTableStateCmd((CFE_SB_Buffer_t*)&CmdPkt);

    /* Assert */
    UtAssert_True(Result == false, "FM_SetTableStateCmd returned false");

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, FM_SET_TABLE_STATE_ARG_STATE_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent.Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent.Spec);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
}

void Test_FM_SetTableStateCmd_BadCurrentState(void)
{
    FM_SetTableStateCmd_t CmdPkt;
    CmdPkt.TableEntryState = FM_TABLE_ENTRY_DISABLED;
    CmdPkt.TableEntryIndex = 0;

    int32           strCmpResult;
    char            ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, 
             "%%s error: cannot modify unused table entry: index = %%d");

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
   
    FM_FreeSpaceTable_t DummyTable;

    for(int i = 0; i < FM_TABLE_ENTRY_COUNT; i++)
    {
        DummyTable.FileSys[i].State = FM_TABLE_ENTRY_DISABLED;
    }

    DummyTable.FileSys[0].State = FM_TABLE_ENTRY_UNUSED;

    FM_GlobalData.FreeSpaceTablePtr = &DummyTable;

    bool Result = FM_SetTableStateCmd((CFE_SB_Buffer_t*)&CmdPkt);

    /* Assert */
    UtAssert_True(Result == false, "FM_SetTableStateCmd returned false");

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, FM_SET_TABLE_STATE_UNUSED_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent.Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent.Spec);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
}

void add_FM_SetTableStateCmd_tests(void)
{
    UtTest_Add(Test_FM_SetTableStateCmd_Success,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_SetTableStateCmd_Success");

    UtTest_Add(Test_FM_SetTableStateCmd_BadLength,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_SetTableStateCmd_BadLength");
    
    UtTest_Add(Test_FM_SetTableStateCmd_NullFreeSpaceTable,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_SetTableStateCmd_NullFreeSpaceTable");

    UtTest_Add(Test_FM_SetTableStateCmd_TableEntryIndexTooLarge,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_SetTableStateCmd_TableEntryIndexTooLarge");

    UtTest_Add(Test_FM_SetTableStateCmd_BadNewState,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_SetTableStateCmd_BadNewState");

    UtTest_Add(Test_FM_SetTableStateCmd_BadCurrentState,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_SetTableStateCmd_BadCurrentState");
}


/****************************/
/* Set Permissions Tests    */
/****************************/

void Test_FM_SetPermissionsCmd_Success(void)
{
    FM_SetPermCmd_t  CmdPkt;

    strncpy(CmdPkt.FileName, "file", OS_MAX_PATH_LEN);
    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyNameValid), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_SetPermissionsCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_SetPermissionsCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_SET_FILE_PERM_CC);
}

void Test_FM_SetPermissionsCmd_BadLength(void)
{
    FM_SetPermCmd_t  CmdPkt;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyNameValid), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_SetPermissionsCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_SetPermissionsCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_SetPermissionsCmd_BadName(void)
{
    FM_SetPermCmd_t  CmdPkt;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyNameValid), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_SetPermissionsCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_SetPermissionsCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_SetPermissionsCmd_NoChildTask(void)
{
    FM_SetPermCmd_t  CmdPkt;

    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyNameValid), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    bool Result = FM_SetPermissionsCmd((CFE_SB_Buffer_t*)&CmdPkt);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_SetPermissionsCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_SetPermissionsCmd_tests(void)
{
    UtTest_Add(Test_FM_SetPermissionsCmd_Success,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_SetPermissionsCmd_Success");

    UtTest_Add(Test_FM_SetPermissionsCmd_BadLength,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_SetPermissionsCmd_BadLength");
 
    UtTest_Add(Test_FM_SetPermissionsCmd_BadName,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_SetPermissionsCmd_BadName");
  
    UtTest_Add(Test_FM_SetPermissionsCmd_NoChildTask,
               fm_tests_Setup, fm_tests_Teardown,
               "Test_FM_SetPermissionsCmd_NoChildTask");
}



/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    add_FM_NoopCmd_tests();
    add_FM_ResetCountersCmd_tests();
    add_FM_CopyFileCmd_tests();
    //add_FM_MoveFileCmd_tests();
    add_FM_RenameFileCmd_tests();
    //add_FM_DeleteFileCmd_tests();
    add_FM_DeleteAllFilesCmd_tests();

#ifdef FM_INCLUDE_DECOMPRESS
    add_FM_DecompressFileCmd_tests();
#endif

    add_FM_ConcatFilesCmd_tests();
    add_FM_GetFileInfoCmd_tests();
    add_FM_GetOpenFilesCmd_tests();
    add_FM_CreateDirectoryCmd_tests();
    add_FM_DeleteDirectoryCmd_tests();
    add_FM_GetDirListFileCmd_tests();
    add_FM_GetDirListPktCmd_tests();
    add_FM_GetFreeSpaceCmd_tests();
    add_FM_SetTableStateCmd_tests();
    add_FM_SetPermissionsCmd_tests();
}
