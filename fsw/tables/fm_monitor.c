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
 *  File Manager (FM) File System Free Space Table Data
 *
 *  Default table contents
 */

/*************************************************************************
**
** Include section
**
**************************************************************************/
#include "cfe.h"
#include "cfe_tbl_filedef.h"
#include "fm_platform_cfg.h"
#include "fm_msg.h"

/*
** FM file system free space table header
*/
CFE_TBL_FileDef_t CFE_TBL_FileDef = {"FM_MonitorTable", FM_APP_NAME "." FM_TABLE_CFE_NAME, FM_TABLE_DEF_DESC,
                                     FM_TABLE_FILENAME, sizeof(FM_MonitorTable_t)};

/*
** FM file system monitor table data
**
** -- table entries must be enabled or disabled or unused
**
** -- enabled table entries may be disabled by command
** -- disabled table entries may be enabled by command
** -- unused table entries may not be modified by command
**
** -- enabled or disabled entries must have a valid file system name
**
** -- the file system name for unused entries is ignored
*/
FM_MonitorTable_t FM_MonitorTable = {
    {{
         /* - 0 - */
         .Type = FM_MonitorTableEntry_Type_VOLUME_FREE_SPACE, /* Entry Type (unused, volume free, directory estimate) */
         .Enabled = true,
         .Name    = "/ram" /* File system name (logical mount point) */
     },
     {
         /* - 1 - */
         .Type = FM_MonitorTableEntry_Type_VOLUME_FREE_SPACE, /* Entry Type (unused, volume free, directory estimate) */
         .Enabled = false,
         .Name    = "/boot" /* File system name (logical mount point) */
     },
     {
         /* - 2 - */
         .Type =
             FM_MonitorTableEntry_Type_DIRECTORY_ESTIMATE, /* Entry Type (unused, volume free, directory estimate) */
         .Enabled = true,
         .Name    = "/cf" /* File system name (logical mount point) */
     },
     {
         /* - 3 - */
         .Type = FM_MonitorTableEntry_Type_UNUSED /* Entry Type (unused, volume free, directory estimate) */
     },
     {
         /* - 4 - */
         .Type = FM_MonitorTableEntry_Type_UNUSED /* Entry Type (unused, volume free, directory estimate) */
     },
     {
         /* - 5 - */
         .Type = FM_MonitorTableEntry_Type_UNUSED /* Entry Type (unused, volume free, directory estimate) */
     },
     {
         /* - 6 - */
         .Type = FM_MonitorTableEntry_Type_UNUSED /* Entry Type (unused, volume free, directory estimate) */
     },
     {
         /* - 7 - */
         .Type = FM_MonitorTableEntry_Type_UNUSED /* Entry Type (unused, volume free, directory estimate) */
     }}};
