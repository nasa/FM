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
 * Auto-Generated stub implementations for functions defined in fm_tbl header
 */

#include "fm_tbl.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for FM_AcquireTablePointers()
 * ----------------------------------------------------
 */
void FM_AcquireTablePointers(void)
{

    UT_GenStub_Execute(FM_AcquireTablePointers, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ReleaseTablePointers()
 * ----------------------------------------------------
 */
void FM_ReleaseTablePointers(void)
{

    UT_GenStub_Execute(FM_ReleaseTablePointers, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_TableInit()
 * ----------------------------------------------------
 */
CFE_Status_t FM_TableInit(void)
{
    UT_GenStub_SetupReturnBuffer(FM_TableInit, CFE_Status_t);

    UT_GenStub_Execute(FM_TableInit, Basic, NULL);

    return UT_GenStub_GetReturnValue(FM_TableInit, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ValidateTable()
 * ----------------------------------------------------
 */
CFE_Status_t FM_ValidateTable(FM_MonitorTable_t *TablePtr)
{
    UT_GenStub_SetupReturnBuffer(FM_ValidateTable, CFE_Status_t);

    UT_GenStub_AddParam(FM_ValidateTable, FM_MonitorTable_t *, TablePtr);

    UT_GenStub_Execute(FM_ValidateTable, Basic, NULL);

    return UT_GenStub_GetReturnValue(FM_ValidateTable, CFE_Status_t);
}
