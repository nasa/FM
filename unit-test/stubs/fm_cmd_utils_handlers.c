/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
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

/*
 * Includes
 */
#include "osapi.h"
#include "cfe.h"
#include "utstubs.h"
#include "cfe_resourceid.h"
#include "cfe_resourceid_basevalue.h"

static void UT_fm_cmd_utils_bool_conversion(UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    bool  retval;
    int32 status_code;

    if (UT_Stub_GetInt32StatusCode(Context, &status_code))
    {
        retval = status_code;
    }
    else
    {
        retval = false;
    }

    UT_Stub_SetReturnValue(FuncKey, retval);
}

/*------------------------------------------------------------*/
void UT_DefaultHandler_FM_IsValidCmdPktLength(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    UT_fm_cmd_utils_bool_conversion(FuncKey, Context);
}

/*------------------------------------------------------------*/
void UT_DefaultHandler_FM_VerifyChildTask(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    UT_fm_cmd_utils_bool_conversion(FuncKey, Context);
}

/*------------------------------------------------------------*/
void UT_DefaultHandler_FM_VerifyDirExists(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    UT_fm_cmd_utils_bool_conversion(FuncKey, Context);
}

/*------------------------------------------------------------*/
void UT_DefaultHandler_FM_VerifyDirNoExist(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    UT_fm_cmd_utils_bool_conversion(FuncKey, Context);
}

/*------------------------------------------------------------*/
void UT_DefaultHandler_FM_VerifyFileClosed(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    UT_fm_cmd_utils_bool_conversion(FuncKey, Context);
}

/*------------------------------------------------------------*/
void UT_DefaultHandler_FM_VerifyFileExists(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    UT_fm_cmd_utils_bool_conversion(FuncKey, Context);
}

/*------------------------------------------------------------*/
void UT_DefaultHandler_FM_VerifyFileNoExist(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    UT_fm_cmd_utils_bool_conversion(FuncKey, Context);
}

/*------------------------------------------------------------*/
void UT_DefaultHandler_FM_VerifyFileNotOpen(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    UT_fm_cmd_utils_bool_conversion(FuncKey, Context);
}

/*------------------------------------------------------------*/
void UT_DefaultHandler_FM_VerifyFileState(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    UT_fm_cmd_utils_bool_conversion(FuncKey, Context);
}

/*------------------------------------------------------------*/
void UT_DefaultHandler_FM_VerifyOverwrite(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    UT_fm_cmd_utils_bool_conversion(FuncKey, Context);
}
