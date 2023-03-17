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
 * Auto-Generated stub implementations for functions defined in fm_dispatch header
 */

#include "fm_dispatch.h"
#include "utgenstub.h"

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

    UT_GenStub_Execute(FM_IsValidCmdPktLength, Basic, NULL);

    return UT_GenStub_GetReturnValue(FM_IsValidCmdPktLength, bool);
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
void FM_ProcessPkt(const CFE_SB_Buffer_t *BufPtr)
{
    UT_GenStub_AddParam(FM_ProcessPkt, const CFE_SB_Buffer_t *, BufPtr);

    UT_GenStub_Execute(FM_ProcessPkt, Basic, NULL);
}
