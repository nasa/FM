/*************************************************************************
** Filename: fm_testrunner.c
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
**   This file contains the unit test runner for the FM.
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

#include "uttest.h"
#include "fm_app_test.h"
#include "fm_child_test.h"
#include "fm_cmds_test.h"
#include "fm_tbl_test.h"
#include "fm_cmd_utils_test.h"

/*
 * Function Definitions
 */

int main(void)
{
    FM_App_Test_AddTestCases();
    FM_Child_Test_AddTestCases();
    FM_Cmds_Test_AddTestCases();
    FM_Tbl_Test_AddTestCases();
    FM_Cmd_Utils_Test_AddTestCases();

    return (UtTest_Run());
} /* end main */

/************************/
/*  End of File Comment */
/************************/
