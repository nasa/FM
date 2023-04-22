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
 *  File Manager (FM) non-implemented compression API
 *
 * This returns "CFE_STATUS_NOT_IMPLEMENTED" for the internal compression API
 * calls, and is used when compression features are not required.
 */

#include <common_types.h>
#include <cfe_error.h>

#include "fm_compression.h"

CFE_Status_t FM_CompressionService_Init(void)
{
    return CFE_SUCCESS;
}

CFE_Status_t FM_Decompress_Impl(FM_Decompressor_State_t *State, const char *SrcFileName, const char *DstFileName)
{
    return CFE_STATUS_NOT_IMPLEMENTED;
}

CFE_Status_t FM_Compress_Impl(FM_Compressor_State_t *State, const char *SrcFileName, const char *DstFileName)
{
    return CFE_STATUS_NOT_IMPLEMENTED;
}
