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
 *   File Manager (FM) Application Topic IDs
 */

#ifndef FM_TOPICIDS_H
#define FM_TOPICIDS_H

/* ======== */
/* Includes */
/* ======== */

#include "fm_topicid_values.h"

/* ====== */
/* Macros */
/* ====== */

#define FM_MISSION_CMD_TOPICID             FM_MISSION_TIDVAL(CMD)
#define DEFAULT_FM_MISSION_CMD_TOPICID     0x8C
#define FM_MISSION_SEND_HK_TOPICID         FM_MISSION_TIDVAL(SEND_HK)
#define DEFAULT_FM_MISSION_SEND_HK_TOPICID 0x8D

#define FM_MISSION_HK_TLM_TOPICID                 FM_MISSION_TIDVAL(HK_TLM)
#define DEFAULT_FM_MISSION_HK_TLM_TOPICID         0x8A
#define FM_MISSION_FILE_INFO_TLM_TOPICID          FM_MISSION_TIDVAL(FILE_INFO_TLM)
#define DEFAULT_FM_MISSION_FILE_INFO_TLM_TOPICID  0x8B
#define FM_MISSION_DIR_LIST_TLM_TOPICID           FM_MISSION_TIDVAL(DIR_LIST_TLM)
#define DEFAULT_FM_MISSION_DIR_LIST_TLM_TOPICID   0x8C
#define FM_MISSION_OPEN_FILES_TLM_TOPICID         FM_MISSION_TIDVAL(OPEN_FILES_TLM)
#define DEFAULT_FM_MISSION_OPEN_FILES_TLM_TOPICID 0x8D
#define FM_MISSION_MONITOR_TLM_TOPICID            FM_MISSION_TIDVAL(MONITOR_TLM)
#define DEFAULT_FM_MISSION_MONITOR_TLM_TOPICID    0x8E

#endif // FM_TOPICIDS_H