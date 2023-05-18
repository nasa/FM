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
 * Auto-Generated stub implementations for functions defined in fm_compression header
 */

#include "fm_compression.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for FM_Compress_Impl()
 * ----------------------------------------------------
 */
CFE_Status_t FM_Compress_Impl(FM_Compressor_State_t *State, const char *SrcFileName, const char *DstFileName)
{
    UT_GenStub_SetupReturnBuffer(FM_Compress_Impl, CFE_Status_t);

    UT_GenStub_AddParam(FM_Compress_Impl, FM_Compressor_State_t *, State);
    UT_GenStub_AddParam(FM_Compress_Impl, const char *, SrcFileName);
    UT_GenStub_AddParam(FM_Compress_Impl, const char *, DstFileName);

    UT_GenStub_Execute(FM_Compress_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(FM_Compress_Impl, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_CompressionService_Init()
 * ----------------------------------------------------
 */
CFE_Status_t FM_CompressionService_Init(void)
{
    UT_GenStub_SetupReturnBuffer(FM_CompressionService_Init, CFE_Status_t);

    UT_GenStub_Execute(FM_CompressionService_Init, Basic, NULL);

    return UT_GenStub_GetReturnValue(FM_CompressionService_Init, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_Decompress_Impl()
 * ----------------------------------------------------
 */
CFE_Status_t FM_Decompress_Impl(FM_Decompressor_State_t *State, const char *SrcFileName, const char *DstFileName)
{
    UT_GenStub_SetupReturnBuffer(FM_Decompress_Impl, CFE_Status_t);

    UT_GenStub_AddParam(FM_Decompress_Impl, FM_Decompressor_State_t *, State);
    UT_GenStub_AddParam(FM_Decompress_Impl, const char *, SrcFileName);
    UT_GenStub_AddParam(FM_Decompress_Impl, const char *, DstFileName);

    UT_GenStub_Execute(FM_Decompress_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(FM_Decompress_Impl, CFE_Status_t);
}
