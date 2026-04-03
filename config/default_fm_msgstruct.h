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
 *   Specification for the CFS File Manager (FM) command and telemetry
 *   message data types.
 *
 * @note
 *   Constants and enumerated types related to these message structures
 *   are defined in fm_msgdefs.h.
 */

#ifndef DEFAULT_FM_MSGSTRUCT_H
#define DEFAULT_FM_MSGSTRUCT_H

/* ======== */
/* Includes */
/* ======== */

#include "cfe.h"
#include "fm_msgdefs.h"

/* ======== */
/* Typedefs */
/* ======== */

/**
 * \defgroup cfsfmcmdstructs CFS File Manager Command Structures
 * \{
 */

/* Command Packets */

/**
 *  \brief Housekeeping Request command packet structure
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /** \brief Command header */
} FM_SendHkCmd_t;

/**
 *  \brief No-Operation command packet structure
 *
 *  For command details see #FM_NOOP_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /** \brief Command header */
} FM_NoopCmd_t;

/**
 *  \brief Reset Counters command packet structure
 *
 *  For command details see #FM_RESET_COUNTERS_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /** \brief Command header */
} FM_ResetCountersCmd_t;

/**
 *  \brief Copy File command packet structure
 *
 *  For command details see #FM_COPY_FILE_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t            CommandHeader; /** \brief Command header */
    FM_OverwriteSourceTarget_Payload_t Payload;       /** \brief Command payload */
} FM_CopyFileCmd_t;

/**
 *  \brief Move File command packet structure
 *
 *  For command details see #FM_MOVE_FILE_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t            CommandHeader; /** \brief Command header */
    FM_OverwriteSourceTarget_Payload_t Payload;       /** \brief Command payload */
} FM_MoveFileCmd_t;

/**
 *  \brief Rename File command packet structure
 *
 *  For command details see #FM_RENAME_FILE_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t   CommandHeader; /** \brief Command header */
    FM_SourceTarget_Payload_t Payload;       /** \brief Command payload */
} FM_RenameFileCmd_t;

/**
 *  \brief Delete File command packet structure
 *
 *  For command details see #FM_DELETE_FILE_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /** \brief Command header */
    FM_Path_Payload_t       Payload;       /** \brief Command Payload */
} FM_DeleteFileCmd_t;

/**
 *  \brief Delete All command packet structure
 *
 *  For command details see #FM_DELETE_ALL_FILES_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /** \brief Command header */
    FM_Path_Payload_t       Payload;       /** \brief Command Payload */
} FM_DeleteAllFilesCmd_t;

/**
 *  \brief Decompress File command packet structure
 *
 *  For command details see #FM_DECOMPRESS_FILE_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t   CommandHeader; /** \brief Command header */
    FM_SourceTarget_Payload_t Payload;       /** \brief Command Payload */
} FM_DecompressFileCmd_t;

/**
 *  \brief Concatenate Files command packet structure
 *
 *  For command details see #FM_CONCAT_FILES_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t     CommandHeader; /** \brief Command header */
    FM_ConcatFilesCmd_Payload_t Payload;       /** \brief Command Payload */
} FM_ConcatFilesCmd_t;

/**
 *  \brief Get File Info command packet structure
 *
 *  For command details see #FM_GET_FILE_INFO_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t     CommandHeader; /** \brief Command header */
    FM_GetFileInfoCmd_Payload_t Payload;       /** \brief Command Payload */
} FM_GetFileInfoCmd_t;

/**
 *  \brief Get Open Files command packet structure
 *
 *  For command details see #FM_GET_OPEN_FILES_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /** \brief Command header */
} FM_GetOpenFilesCmd_t;

/**
 *  \brief Create Directory command packet structure
 *
 *  For command details see #FM_CREATE_DIRECTORY_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /** \brief Command header */
    FM_Path_Payload_t       Payload;       /** \brief Command Payload */
} FM_CreateDirectoryCmd_t;

/**
 *  \brief Delete Directory command packet structure
 *
 *  For command details see #FM_DELETE_DIRECTORY_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /** \brief Command header */
    FM_Path_Payload_t       Payload;       /** \brief Command Payload */
} FM_DeleteDirectoryCmd_t;

/**
 *  \brief Get DIR List to File command packet structure
 *
 *  For command details see #FM_GET_DIR_LIST_FILE_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t        CommandHeader; /** \brief Command header */
    FM_GetDirListFileCmd_Payload_t Payload;       /** \brief Command Payload */
} FM_GetDirListFileCmd_t;

/**
 *  \brief Get DIR List to Packet command packet structure
 *
 *  For command details see #FM_GET_DIR_LIST_PKT_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t       CommandHeader; /** \brief Command header */
    FM_GetDirListPktCmd_Payload_t Payload;       /** \brief Command Payload */
} FM_GetDirListPktCmd_t;

/**
 *  \brief Get Free Space command packet structure
 *
 *  For command details see #FM_MONITOR_FILESYSTEM_SPACE_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /** \brief Command header */
} FM_MonitorFilesystemSpaceCmd_t;

/**
 *  \brief Set Table State command packet structure
 *
 *  For command details see #FM_SET_TABLE_STATE_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t       CommandHeader; /** \brief Command header */
    FM_SetTableStateCmd_Payload_t Payload;       /** \brief Command Payload */
} FM_SetTableStateCmd_t;

/**
 *  \brief Set Permissions for a file
 *
 *  For command details see #FM_SET_PERMISSIONS_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t        CommandHeader; /** \brief Command header */
    FM_SetPermissionsCmd_Payload_t Payload;
} FM_SetPermissionsCmd_t;

/**\}*/

/* Telemetry Packets */

/**
 * \defgroup cfsfmtlmstructs CFS File Manager Telemetry Structures
 * \{
 */

/**
 *  \brief Get Directory Listing telemetry packet
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t TelemetryHeader; /** \brief Telemetry Header */
    FM_DirListPkt_Payload_t   Payload;         /** \brief Telemetry Payload */
} FM_DirListPkt_t;

/**
 *  \brief Get File Info telemetry packet
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t TelemetryHeader; /** \brief Telemetry Header */
    FM_FileInfoPkt_Payload_t  Payload;         /** \brief Telemetry Payload */
} FM_FileInfoPkt_t;

/**
 *  \brief Get Open Files telemetry packet
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t TelemetryHeader; /** \brief Telemetry Header */
    FM_OpenFilesPkt_Payload_t Payload;         /** \brief Telemetry Payload */
} FM_OpenFilesPkt_t;

/**
 *  \brief Monitor filesystem telemetry packet
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t     TelemetryHeader; /** \brief Telemetry Header */
    FM_MonitorReportPkt_Payload_t Payload;         /** \brief Telemetry Payload */
} FM_MonitorReportPkt_t;

/**
 * \brief Housekeeping telemetry packet
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t TelemetryHeader; /** \brief Telemetry Header */
    FM_HkTlm_Payload_t        Payload;         /** \brief Telemetry Payload */
} FM_HkTlm_t;

/**\}*/

#endif /* DEFAULT_FM_MSGSTRUCT_H */