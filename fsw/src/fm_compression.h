/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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
 *   FM internal data compression API.  These functions may be unimplemented,
 *   or they may map to external/3rd party data compression libraries such
 *   as zLib.  In the latter case, an adapter layer is used to map the FM
 *   compression/decompression tasks to the library of choice.
 */

#ifndef FM_COMPRESSION_H
#define FM_COMPRESSION_H

/* ======== */
/* Includes */
/* ======== */

#include "cfe.h"
#include "fm_internal_cfg.h"

/**
 * @brief The state object for a compressor
 *
 * This is an abstract object at this level, the definition of this
 * object depends on the selected implementation.
 */
typedef struct FM_Compressor_State FM_Compressor_State_t;

/**
 * @brief The state object for a decompressor
 *
 * This is an abstract object at this level, the definition of this
 * object depends on the selected implementation.
 */
typedef struct FM_Decompressor_State FM_Decompressor_State_t;

/* =================== */
/* Function Prototypes */
/* =================== */

/**
 * @brief Initialize the compression/decompression service
 *
 * Called during FM init to prepare service(s).  This may be a no-op
 * if compression/decompression services are not enabled.
 *
 * @returns Status code
 * @retval #CFE_SUCCESS if successful
 */
CFE_Status_t FM_CompressionService_Init(void);

/**
 * @brief Abstract file decompression routine
 *
 * Decompresses the file specified by SrcFileName and stores
 * the uncompressed data in the file specified by DstFileName
 *
 * @param State the decompressor state object
 * @param SrcFileName the compressed input file
 * @param DstFileName the uncompressed output file
 *
 * @returns Status code
 * @retval #CFE_SUCCESS if decompression was successful
 */
CFE_Status_t FM_Decompress_Impl(FM_Decompressor_State_t *State, const char *SrcFileName, const char *DstFileName);

/**
 * @brief Abstract file compression routine
 *
 * Compresses the file specified by SrcFileName and stores
 * the compressed data in the file specified by DstFileName
 *
 * @param State the compressor state object
 * @param SrcFileName the uncompressed input file
 * @param DstFileName the compressed output file
 *
 * @returns Status code
 * @retval #CFE_SUCCESS if compression was successful
 */
CFE_Status_t FM_Compress_Impl(FM_Compressor_State_t *State, const char *SrcFileName, const char *DstFileName);

#endif
