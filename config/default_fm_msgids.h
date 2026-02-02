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
 *   CFS File Manager (FM) Application Message IDs
 */
#ifndef DEFAULT_FM_MSGIDS_H
#define DEFAULT_FM_MSGIDS_H

/* ======== */
/* Includes */
/* ======== */

#include "cfe_core_api_base_msgids.h"
#include "fm_msgid_values.h"

/* ====== */
/* Macros */
/* ====== */

/**
 * \defgroup cfsfmcmdmid CFS File Manager Command Message IDs
 * \{
 */

#define FM_CMD_MID     FM_CMD_PLATFORM_MIDVAL(CMD)
#define FM_SEND_HK_MID FM_CMD_PLATFORM_MIDVAL(SEND_HK)

/**\}*/

/**
 * \defgroup cfsfmtlmmid CFS File Manager Telemetry Message IDs
 * \{
 */

#define FM_HK_TLM_MID         FM_TLM_PLATFORM_MIDVAL(HK_TLM)
#define FM_FILE_INFO_TLM_MID  FM_TLM_PLATFORM_MIDVAL(FILE_INFO_TLM)
#define FM_DIR_LIST_TLM_MID   FM_TLM_PLATFORM_MIDVAL(DIR_LIST_TLM)
#define FM_OPEN_FILES_TLM_MID FM_TLM_PLATFORM_MIDVAL(OPEN_FILES_TLM)
#define FM_MONITOR_TLM_MID    FM_TLM_PLATFORM_MIDVAL(MONITOR_TLM)

/**\}*/

#endif /* DEFAULT_FM_MSGIDS_H */
