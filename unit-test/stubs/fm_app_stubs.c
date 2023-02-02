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
 * Auto-Generated stub implementations for functions defined in fm_app header
 */

#include "fm_app.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for FM_AppInit()
 * ----------------------------------------------------
 */
int32 FM_AppInit(void)
{
    UT_GenStub_SetupReturnBuffer(FM_AppInit, int32);

    UT_GenStub_Execute(FM_AppInit, Basic, NULL);

    return UT_GenStub_GetReturnValue(FM_AppInit, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_AppMain()
 * ----------------------------------------------------
 */
void FM_AppMain(void)
{

    UT_GenStub_Execute(FM_AppMain, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ProcessCmd()
 * ----------------------------------------------------
 */
void FM_ProcessCmd(const CFE_SB_Buffer_t *BufPtr)
{
    UT_GenStub_AddParam(FM_ProcessCmd, const CFE_SB_Buffer_t *, BufPtr);

    UT_GenStub_Execute(FM_ProcessCmd, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ProcessPkt()
 * ----------------------------------------------------
 */
void FM_ProcessPkt(const CFE_SB_Buffer_t *MessagePtr)
{
    UT_GenStub_AddParam(FM_ProcessPkt, const CFE_SB_Buffer_t *, MessagePtr);

    UT_GenStub_Execute(FM_ProcessPkt, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ReportHK()
 * ----------------------------------------------------
 */
void FM_ReportHK(const CFE_MSG_CommandHeader_t *Msg)
{
    UT_GenStub_AddParam(FM_ReportHK, const CFE_MSG_CommandHeader_t *, Msg);

    UT_GenStub_Execute(FM_ReportHK, Basic, NULL);
}
