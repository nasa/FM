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
 *   Specification for the CFS File Manager (FM) table related
 *   constant definitions.
 *
 * @note
 *   These Macro definitions have been put in this file (instead of
 *   fm_tbl.h)
 */

#ifndef DEFAULT_FM_TBLDEFS_H
#define DEFAULT_FM_TBLDEFS_H

/* ======== */
/* Includes */
/* ======== */

#include "cfe.h"

/* ========== */
/* Constants  */
/* ========== */

enum
{
    /**
     * Table entry is not used, these entries are ignored
     */
    FM_MonitorTableEntry_Type_UNUSED = 0,

    /**
     * Monitor the free space on given volume
     *
     * The given path will be passed to OS_FileSysStatVolume() and the results
     * will be reported in the generated TLM entry.
     */
    FM_MonitorTableEntry_Type_VOLUME_FREE_SPACE = 1,

    /**
     * Estimate the sum of space used by files within specified directory
     *
     * The given path will be opened as a directory.  The size of each regular
     * file present in that directory will be summed to produce an estimate of the
     * total space associated with that directory.
     *
     * Note that this yields only an estimate, as there can be discrepancies
     * between the file size as observed by this method and the actual disk blocks
     * used by a given file.
     */
    FM_MonitorTableEntry_Type_DIRECTORY_ESTIMATE = 2
};

/* ======== */
/* Payloads */
/* ======== */

/**
 *  \brief Monitor table entry
 */
typedef struct
{
    /**
     * Table entry type.
     *
     * This should be one of the FM_MonitorTableEntry_Type_* enumeration values.
     * It is defined as a uint8 in this table to ensure a consistent size.
     */
    uint8_t Type;

    /**
     * Boolean flag indicating whether this entry is active or not
     */
    uint8_t Enabled;

    /**
     * Location to monitor
     *
     * The interpretation of this string depends on Type
     * See the FM_MonitorTableEntry_Type_* descriptions for how this is to be set
     */
    char Name[CFE_MISSION_MAX_PATH_LEN];
} FM_MonitorTableEntry_t;

#endif /* DEFAULT_FM_TBLDEFS_H */