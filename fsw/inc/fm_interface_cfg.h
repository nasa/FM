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
 *   CFS File Manager (FM) Application Public Definitions
 *
 * @note This file may be overridden/superceded by mission-provided defintions
 * either by overriding this header or by generating definitions from a command/data
 * dictionary tool.
 */

#ifndef FM_INTERFACE_CFG_H
#define FM_INTERFACE_CFG_H

/* ======== */
/* Includes */
/* ======== */

#include "fm_interface_cfg_values.h"

/* ====== */
/* Macros */
/* ====== */

/**
 * \defgroup cfsfmmissioncfg CFS File Manager Interface Configuration
 * \{
 */

/**
 * \brief Directory List Telemetry Packet Entry Count
 *
 *  \par Description:
 *       This definition sets the number of directory entries contained
 *       in the Directory List telemetry packet.  The command handler will
 *       read directory entries until reaching the index of the start entry
 *       (set via command argument) and then continue to read
 *       directory entries and populate the telemtry packet until there are
 *       either no more unread directory entries or until the telemetry
 *       packet is full.
 *
 *  \par Limits:
 *       The FM application limits this value to be no less than 10 and
 *       and no greater than 100. The number of directory entries in the
 *       telemetry packet will in large part determine the packet size.
 */
#define FM_DIR_LIST_PKT_ENTRIES                   FM_INTERFACE_CFGVAL(DIR_LIST_PKT_ENTRIES)
#define DEFAULT_FM_INTERFACE_DIR_LIST_PKT_ENTRIES 20

/**
 * \brief Maximum Directory List Output File Entries
 *
 *  \par Description:
 *       This definition sets the upper limit for the number of directory
 *       entries that may be written to a Directory List output file.
 *       Directory List files are variable length, based on the number of
 *       directory entries actually written to the file.  There may zero
 *       entries written to the file if the directory is empty.  For most
 *       environments, this definition will play no role at all, as it
 *       will be set to a number much larger than the count of files that
 *       will ever exist in any directory at one time.
 *
 *  \par Limits:
 *       The FM application limits this value to be no less than 100 and
 *       no greater than 10000.
 */
#define FM_DIR_LIST_FILE_ENTRIES                   FM_INTERFACE_CFGVAL(DIR_LIST_FILE_ENTRIES)
#define DEFAULT_FM_INTERFACE_DIR_LIST_FILE_ENTRIES 3000

/**
 * \brief Directory List Output File Header Sub-Type
 *
 *  \par Description:
 *       This definition sets the cFE File Header sub-type value for FM
 *       Directory List data files.  The value may be used to differentiate
 *       FM Directory List files from other data files.
 *
 *  \par Limits:
 *       The FM application places no limits on this unsigned 32 bit value.
 */
#define FM_DIR_LIST_FILE_SUBTYPE                   FM_INTERFACE_CFGVAL(DIR_LIST_FILE_SUBTYPE)
#define DEFAULT_FM_INTERFACE_DIR_LIST_FILE_SUBTYPE 12345

/**
 * \brief FM argument to not calculate CRC during Get File Info command
 */
#define FM_IGNORE_CRC                   FM_INTERFACE_CFGVAL(IGNORE_CRC)
#define DEFAULT_FM_INTERFACE_IGNORE_CRC 0

/**
 * \brief Number of Free Space Table Entries
 *
 *  \par Description:
 *       This value defines the number of entries in both the FM file system
 *       free space table and the FM file system free space telemetry packet.
 *       Note: this value does not define the number of file systems present
 *       or supported by the CFE-OSAL, the value only defines the number of
 *       file systems for which FM may be enabled to report free space data.
 *
 *  \par Limits:
 *       FM limits this value to be not less than 1 and not greater than 32.
 */
#define FM_TABLE_ENTRY_COUNT                   FM_INTERFACE_CFGVAL(TABLE_ENTRY_COUNT)
#define DEFAULT_FM_INTERFACE_TABLE_ENTRY_COUNT 8

/**\}*/

#endif // FM_INTERFACE_CFG_H