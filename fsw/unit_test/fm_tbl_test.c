/*************************************************************************
** Filename: fm_tbl_test.c
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
**
** Purpose:
**   This file contains unit test cases for the functions contained in the file fm_tbl.c.
**
** References:
**   Flight Software Branch C Coding Standard Version 1.2
**   CFS Development Standards Document
** Notes:
**
*************************************************************************/

/*
 * Includes
 */

#include "fm_tbl_test.h"
#include "fm_app.h"
#include "fm_tbl.h"
#include "fm_msg.h"
#include "fm_msgdefs.h"
#include "fm_msgids.h"
#include "fm_events.h"
#include "fm_version.h"
#include "fm_test_utils.h"
#include "cfe_tbl_internal.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * Function Definitions
 */

void FM_TableInit_Test(void)
{
    int32 Result;

    /* Execute the function being tested */
    Result = FM_TableInit();

    /* Verify results */

    UtAssert_True(Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end FM_TableInit_Test */

void FM_ValidateTable_Test_NominalTableEntryEnabled(void)
{
    int32               Result;
    FM_FreeSpaceTable_t TableData;
    uint16              i;
    char                ExpectedEventText[CFE_EVS_MAX_MESSAGE_LENGTH];
    uint32              GoodEntries   = 0;
    uint32              BadEntries    = 0;
    uint32              UnusedEntries = FM_TABLE_ENTRY_COUNT;

    for (i = 0; i < FM_TABLE_ENTRY_COUNT; i++)
    {
        TableData.FileSys[i].State = FM_TABLE_ENTRY_UNUSED;
    }

    TableData.FileSys[0].State = FM_TABLE_ENTRY_ENABLED;
    GoodEntries                = 1; /* Since one entry is assumed to be good, adjust the entry counts accordingly */

    UnusedEntries = UnusedEntries - GoodEntries - BadEntries;

    strncpy(TableData.FileSys[0].Name, "filename", OS_MAX_PATH_LEN);

    /* Execute the function being tested */
    Result = FM_ValidateTable(&TableData);

    /* Verify results */
    UtAssert_True(Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    sprintf(ExpectedEventText, "Free Space Table verify results: good entries = %d, bad = %d, unused = %d",
            (int)GoodEntries, (int)BadEntries, (int)UnusedEntries);

    UtAssert_True(Ut_CFE_EVS_EventSent(FM_TABLE_VERIFY_EID, CFE_EVS_INFORMATION, ExpectedEventText), ExpectedEventText);

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end FM_ValidateTable_Test_NominalTableEntryEnabled */

void FM_ValidateTable_Test_NominalTableEntryDisabled(void)
{
    int32               Result;
    FM_FreeSpaceTable_t TableData;
    uint16              i;
    char                ExpectedEventText[CFE_EVS_MAX_MESSAGE_LENGTH];
    uint32              GoodEntries   = 0;
    uint32              BadEntries    = 0;
    uint32              UnusedEntries = FM_TABLE_ENTRY_COUNT;

    for (i = 0; i < FM_TABLE_ENTRY_COUNT; i++)
    {
        TableData.FileSys[i].State = FM_TABLE_ENTRY_UNUSED;
    }

    TableData.FileSys[0].State = FM_TABLE_ENTRY_DISABLED;

    GoodEntries   = 1; /* Since one entry is assumed to be good, adjust the entry counts accordingly */
    UnusedEntries = UnusedEntries - GoodEntries - BadEntries;

    strncpy(TableData.FileSys[0].Name, "filename", OS_MAX_PATH_LEN);

    /* Execute the function being tested */
    Result = FM_ValidateTable(&TableData);

    /* Verify results */
    UtAssert_True(Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    sprintf(ExpectedEventText, "Free Space Table verify results: good entries = %d, bad = %d, unused = %d",
            (int)GoodEntries, (int)BadEntries, (int)UnusedEntries);

    UtAssert_True(Ut_CFE_EVS_EventSent(FM_TABLE_VERIFY_EID, CFE_EVS_INFORMATION, ExpectedEventText), ExpectedEventText);

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 1, "Ut_CFE_EVS_GetEventQueueDepth() == 1");

} /* end FM_ValidateTable_Test_NominalTableEntryDisabled */

void FM_ValidateTable_Test_InvalidState(void)
{
    int32               Result;
    FM_FreeSpaceTable_t TableData;
    char                ExpectedEventText[CFE_EVS_MAX_MESSAGE_LENGTH];
    uint32              GoodEntries   = 0;
    uint32              BadEntries    = 0;
    uint32              UnusedEntries = FM_TABLE_ENTRY_COUNT;

    memset(&TableData, 0, sizeof(TableData));

    TableData.FileSys[0].State = 99;

    BadEntries    = FM_TABLE_ENTRY_COUNT; /* Setting all entries bad, adjust unused accordingly */
    UnusedEntries = UnusedEntries - GoodEntries - BadEntries;

    /* Execute the function being tested */
    Result = FM_ValidateTable(&TableData);

    /* Verify results */
    UtAssert_True((int32)Result == (int32)FM_TABLE_VALIDATION_ERR, "Result == FM_TABLE_VALIDATION_ERR");

    /* Since index and state are explicitly set above, there is no dependency on external values,
       and therefore no need for an sprintf for this test. */
    UtAssert_True(Ut_CFE_EVS_EventSent(FM_TABLE_VERIFY_BAD_STATE_ERR_EID, CFE_EVS_ERROR,
                                       "Table verify error: index = 0, invalid state = 99"),
                  "Table verify error: index = 0, invalid state = 99");

    sprintf(ExpectedEventText, "Free Space Table verify results: good entries = %d, bad = %d, unused = %d",
            (int)GoodEntries, (int)BadEntries, (int)UnusedEntries);

    UtAssert_True(Ut_CFE_EVS_EventSent(FM_TABLE_VERIFY_EID, CFE_EVS_INFORMATION, ExpectedEventText), ExpectedEventText);

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end FM_ValidateTable_Test_InvalidState */

void FM_ValidateTable_Test_EmptyNameString(void)
{
    int32               Result;
    FM_FreeSpaceTable_t TableData;
    uint16              i;
    char                ExpectedEventText[CFE_EVS_MAX_MESSAGE_LENGTH];
    uint32              GoodEntries   = 0;
    uint32              BadEntries    = 0;
    uint32              UnusedEntries = FM_TABLE_ENTRY_COUNT;

    for (i = 0; i < FM_TABLE_ENTRY_COUNT; i++)
    {
        TableData.FileSys[i].State = FM_TABLE_ENTRY_UNUSED;
    }

    TableData.FileSys[0].State = FM_TABLE_ENTRY_ENABLED;

    BadEntries    = 1; /* Since one entry is assumed to be bad, adjust the entry counts accordingly */
    UnusedEntries = UnusedEntries - GoodEntries - BadEntries;

    strncpy(TableData.FileSys[0].Name, "", OS_MAX_PATH_LEN);

    /* Execute the function being tested */
    Result = FM_ValidateTable(&TableData);

    /* Verify results */
    UtAssert_True((int32)Result == (int32)FM_TABLE_VALIDATION_ERR, "Result == FM_TABLE_VALIDATION_ERR");

    UtAssert_True(Ut_CFE_EVS_EventSent(FM_TABLE_VERIFY_EMPTY_ERR_EID, CFE_EVS_ERROR,
                                       "Free Space Table verify error: index = 0, empty name string"),
                  "Free Space Table verify error: index = 0, empty name string");

    sprintf(ExpectedEventText, "Free Space Table verify results: good entries = %d, bad = %d, unused = %d",
            (int)GoodEntries, (int)BadEntries, (int)UnusedEntries);

    UtAssert_True(Ut_CFE_EVS_EventSent(FM_TABLE_VERIFY_EID, CFE_EVS_INFORMATION, ExpectedEventText), ExpectedEventText);

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end FM_ValidateTable_Test_EmptyNameString */

void FM_ValidateTable_Test_NameTooLong(void)
{
    int32               Result;
    FM_FreeSpaceTable_t TableData;
    uint16              i;
    char                ExpectedEventText[CFE_EVS_MAX_MESSAGE_LENGTH];
    uint32              GoodEntries   = 0;
    uint32              BadEntries    = 0;
    uint32              UnusedEntries = FM_TABLE_ENTRY_COUNT;

    for (i = 0; i < OS_MAX_PATH_LEN; i++)
    {
        TableData.FileSys[0].Name[i] = 'a';
    }

    TableData.FileSys[0].Name[OS_MAX_PATH_LEN] = '\0';

    for (i = 0; i < FM_TABLE_ENTRY_COUNT; i++)
    {
        TableData.FileSys[i].State = FM_TABLE_ENTRY_UNUSED;
    }

    TableData.FileSys[0].State = FM_TABLE_ENTRY_ENABLED;

    BadEntries    = 1; /* Since one entry is assumed to be bad, adjust the entry counts accordingly */
    UnusedEntries = UnusedEntries - GoodEntries - BadEntries;

    /* Execute the function being tested */
    Result = FM_ValidateTable(&TableData);

    /* Verify results */
    UtAssert_True((int32)Result == (int32)FM_TABLE_VALIDATION_ERR, "Result == FM_TABLE_VALIDATION_ERR");

    UtAssert_True(Ut_CFE_EVS_EventSent(FM_TABLE_VERIFY_TOOLONG_ERR_EID, CFE_EVS_ERROR,
                                       "Free Space Table verify error: index = 0, name too long"),
                  "Free Space Table verify error: index = 0, name too long");

    sprintf(ExpectedEventText, "Free Space Table verify results: good entries = %d, bad = %d, unused = %d",
            (int)GoodEntries, (int)BadEntries, (int)UnusedEntries);

    UtAssert_True(Ut_CFE_EVS_EventSent(FM_TABLE_VERIFY_EID, CFE_EVS_INFORMATION, ExpectedEventText), ExpectedEventText);

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end FM_ValidateTable_Test_NameTooLong */

void FM_ValidateTable_Test_InvalidName(void)
{
    int32               Result;
    FM_FreeSpaceTable_t TableData;
    uint16              i;
    char                ExpectedEventText[CFE_EVS_MAX_MESSAGE_LENGTH];
    uint32              GoodEntries   = 0;
    uint32              BadEntries    = 0;
    uint32              UnusedEntries = FM_TABLE_ENTRY_COUNT;

    for (i = 0; i < FM_TABLE_ENTRY_COUNT; i++)
    {
        TableData.FileSys[i].State = FM_TABLE_ENTRY_UNUSED;
    }

    TableData.FileSys[0].State = FM_TABLE_ENTRY_ENABLED;

    strncpy(TableData.FileSys[0].Name, "*12345", OS_MAX_PATH_LEN);

    BadEntries    = 1; /* Since one entry is assumed to be bad, adjust the entry counts accordingly */
    UnusedEntries = UnusedEntries - GoodEntries - BadEntries;

    /* Execute the function being tested */
    Result = FM_ValidateTable(&TableData);

    /* Verify results */
    UtAssert_True((int32)Result == (int32)FM_TABLE_VALIDATION_ERR, "Result == FM_TABLE_VALIDATION_ERR");

    UtAssert_True(Ut_CFE_EVS_EventSent(FM_TABLE_VERIFY_INVALID_ERR_EID, CFE_EVS_ERROR,
                                       "Free Space Table verify error: index = 0, invalid name = *12345"),
                  "Free Space Table verify error: index = 0, invalid name = *12345");

    sprintf(ExpectedEventText, "Free Space Table verify results: good entries = %d, bad = %d, unused = %d",
            (int)GoodEntries, (int)BadEntries, (int)UnusedEntries);

    UtAssert_True(Ut_CFE_EVS_EventSent(FM_TABLE_VERIFY_EID, CFE_EVS_INFORMATION, ExpectedEventText), ExpectedEventText);

    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 2, "Ut_CFE_EVS_GetEventQueueDepth() == 2");

} /* end FM_ValidateTable_Test_InvalidName */

void FM_AcquireTablePointers_TestNeverLoaded(void)
{
    /* Set to satisfy condition "Status == CFE_TBL_ERR_NEVER_LOADED" */
    Ut_CFE_TBL_SetReturnCode(UT_CFE_TBL_GETADDRESS_INDEX, CFE_TBL_ERR_NEVER_LOADED, 1);

    /* Execute the function being tested */
    FM_AcquireTablePointers();

    /* Verify results */
    UtAssert_True(Ut_CFE_EVS_GetEventQueueDepth() == 0, "Ut_CFE_EVS_GetEventQueueDepth() == 0");

} /* end FM_AcquireTablePointers_TestNeverLoaded */

/* No need to test FM_ReleaseTablePointers - already fully covered and tested by other tests */

void FM_Tbl_Test_AddTestCases(void)
{
    UtTest_Add(FM_TableInit_Test, FM_Test_Setup, FM_Test_TearDown, "FM_TableInit_Test");
    UtTest_Add(FM_ValidateTable_Test_NominalTableEntryEnabled, FM_Test_Setup, FM_Test_TearDown,
               "FM_ValidateTable_Test_NominalTableEntryEnabled");
    UtTest_Add(FM_ValidateTable_Test_NominalTableEntryDisabled, FM_Test_Setup, FM_Test_TearDown,
               "FM_ValidateTable_Test_NominalTableEntryDisabled");
    UtTest_Add(FM_ValidateTable_Test_InvalidState, FM_Test_Setup, FM_Test_TearDown,
               "FM_ValidateTable_Test_InvalidState");
    UtTest_Add(FM_ValidateTable_Test_EmptyNameString, FM_Test_Setup, FM_Test_TearDown,
               "FM_ValidateTable_Test_EmptyNameString");
    UtTest_Add(FM_ValidateTable_Test_NameTooLong, FM_Test_Setup, FM_Test_TearDown, "FM_ValidateTable_Test_NameTooLong");
    UtTest_Add(FM_ValidateTable_Test_InvalidName, FM_Test_Setup, FM_Test_TearDown, "FM_ValidateTable_Test_InvalidName");
    UtTest_Add(FM_AcquireTablePointers_TestNeverLoaded, FM_Test_Setup, FM_Test_TearDown,
               "FM_AcquireTablePointers_TestNeverLoaded");
} /* end FM_Tbl_Test_AddTestCases */

/************************/
/*  End of File Comment */
/************************/
