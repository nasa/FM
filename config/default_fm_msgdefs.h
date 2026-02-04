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
 *   message payload content definitions.
 */
#ifndef DEFAULT_FM_MSGDEFS_H
#define DEFAULT_FM_MSGDEFS_H

/* ======== */
/* Includes */
/* ======== */

#include "fm_interface_cfg.h" /* For FM_DIR_LIST_PKT_ENTRIES */
#include "fm_fcncodes.h"      /* For command codes and any other definitions for message interpretation */
#include "fm_tbldefs.h"       /* For FM_TABLE_ENTRY_COUNT */
#include "fm_extern_typedefs.h"

/* ======== */
/* Typedefs */
/* ======== */

/* Command Payloads */

/**
 * \defgroup cfsfmcmd CFS File Manager Commands
 * \{
 */

/**
 * \brief Copy/Move File command payload structure
 *
 * Contains a source and target file name and an overwrite flag
 *
 * Used by #FM_COPY_FILE_CC, #FM_MOVE_FILE_CC
 */
typedef struct
{
    uint16 Overwrite;                        /**< \brief Allow overwrite */
    char   Source[CFE_MISSION_MAX_PATH_LEN]; /**< \brief Source filename */
    char   Target[CFE_MISSION_MAX_PATH_LEN]; /**< \brief Target filename */
} FM_OvwSourceTargetFilename_Payload_t;

/**
 *  \brief Source and Target filename command payload structure
 *
 *  Used by #FM_RENAME_FILE_CC, #FM_DECOMPRESS_FILE_CC
 */
typedef struct
{
    char Source[CFE_MISSION_MAX_PATH_LEN]; /**< \brief Source filename */
    char Target[CFE_MISSION_MAX_PATH_LEN]; /**< \brief Target filename */
} FM_SourceTargetFileName_Payload_t;

/**
 *  \brief Single filename command payload structure
 *
 *  Used by #FM_DELETE_FILE_CC
 */
typedef struct
{
    char Filename[CFE_MISSION_MAX_PATH_LEN]; /**< \brief Delete filename */
} FM_SingleFilename_Payload_t;

/**
 *  \brief Single directory command payload structure
 *
 *  Used by #FM_DELETE_ALL_FILES_CC, #FM_CREATE_DIRECTORY_CC, #FM_DELETE_DIRECTORY_CC
 */
typedef struct
{
    char Directory[CFE_MISSION_MAX_PATH_LEN]; /**< \brief Directory name */
} FM_DirectoryName_Payload_t;

/**
 * \brief Two source, one target filename command payload structure
 *
 * Used by #FM_CONCAT_FILES_CC
 */
typedef struct
{
    char Source1[CFE_MISSION_MAX_PATH_LEN]; /**< \brief Source 1 filename */
    char Source2[CFE_MISSION_MAX_PATH_LEN]; /**< \brief Source 2 filename */
    char Target[CFE_MISSION_MAX_PATH_LEN];  /**< \brief Target filename */
} FM_TwoSourceOneTarget_Payload_t;

/**
 *  \brief Filename and CRC command payload structure
 *
 *  Used by #FM_GET_FILE_INFO_CC
 */
typedef struct
{
    char   Filename[CFE_MISSION_MAX_PATH_LEN]; /**< \brief Filename */
    uint32 FileInfoCRC;                        /**< \brief File info CRC method */
} FM_FilenameAndCRC_Payload_t;

/**
 *  \brief Get Directory and output to file command payload
 *
 * Contains a directory and output file name, with optional flags
 * Used by #FM_GET_DIR_LIST_FILE_CC
 */
typedef struct
{
    char  Directory[CFE_MISSION_MAX_PATH_LEN]; /**< \brief Directory name */
    char  Filename[CFE_MISSION_MAX_PATH_LEN];  /**< \brief Filename */
    uint8 GetSizeTimeMode; /**< \brief Option to query size, time, and mode of files (CPU intensive) */
    uint8 Spare01[3];      /**< \brief Padding to 32 bit boundary */
} FM_GetDirectoryToFile_Payload_t;

/**
 *  \brief Get Directory and output to message command payload
 *
 * Contains a directory and position offset, with optional flags
 * Used by #FM_GET_DIR_LIST_PKT_CC
 */
typedef struct
{
    char   Directory[CFE_MISSION_MAX_PATH_LEN]; /**< \brief Directory name */
    uint32 DirListOffset;                       /**< \brief Index of 1st dir entry to put in packet */
    uint8  GetSizeTimeMode; /**< \brief Option to query size, time, and mode of files (CPU intensive) */
    uint8  Spare01[3];      /**< \brief Padding to 32 bit boundary */
} FM_GetDirectoryToPkt_Payload_t;

/**
 *  \brief Table Index and State command payload structure
 *
 *  Used by #FM_SET_TABLE_STATE_CC
 */
typedef struct
{
    uint32 TableEntryIndex; /**< \brief Table entry index */
    uint32 TableEntryState; /**< \brief New table entry state */
} FM_TableIndexAndState_Payload_t;

/**
 *  \brief File name and mode command payload structure
 *
 *  Used by #FM_SET_PERMISSIONS_CC
 */
typedef struct
{
    char   FileName[CFE_MISSION_MAX_PATH_LEN]; /**< \brief File name of the permissions to set */
    uint32 Mode;                               /**< \brief Permissions, passed directly to OS_chmod */
} FM_FilenameAndMode_Payload_t;

/**\}*/

/* Telemetry Payloads */

/**
 * \defgroup cfsfmtlm CFS File Manager Telemetry
 * \{
 */

/**
 *  \brief Get Directory Listing entry structure
 */
typedef struct
{
    char   EntryName[CFE_MISSION_MAX_PATH_LEN]; /**< \brief Directory Listing Filename */
    uint32 EntrySize;                           /**< \brief Directory Listing File Size */
    uint32 ModifyTime;                          /**< \brief Directory Listing File Last Modification Times */
    uint32 Mode;                                /**< \brief Mode of the file (Permissions from #OS_FILESTAT_MODE) */
} FM_DirListEntry_t;

/**
 *  \brief Get Directory Listing telemetry payload
 */
typedef struct
{
    char              DirName[CFE_MISSION_MAX_PATH_LEN]; /**< \brief Directory Name */
    uint32            TotalFiles;                        /**< \brief Number of files in the directory */
    uint32            PacketFiles;                       /**< \brief Number of files in this packet */
    uint32            FirstFile;                         /**< \brief Index into directory files of first packet file */
    FM_DirListEntry_t FileList[FM_DIR_LIST_PKT_ENTRIES]; /**< \brief Directory listing file data */
} FM_DirListPkt_Payload_t;

/**
 *  \brief Get File Info telemetry payload
 */
typedef struct
{
    uint8  FileStatus;                         /**< \brief Status indicating whether the file is open or closed */
    uint8  CRC_Computed;                       /**< \brief Flag indicating whether a CRC was computed or not */
    uint8  Spare[2];                           /**< \brief Structure padding */
    uint32 CRC;                                /**< \brief CRC value if computed */
    uint32 FileSize;                           /**< \brief File Size */
    uint32 LastModifiedTime;                   /**< \brief Last Modification Time of File */
    uint32 Mode;                               /**< \brief Mode of the file (Permissions) */
    char   Filename[CFE_MISSION_MAX_PATH_LEN]; /**< \brief Name of File */
} FM_FileInfoPkt_Payload_t;

/**
 *  \brief Get Open Files list entry structure
 */
typedef struct
{
    char LogicalName[CFE_MISSION_MAX_PATH_LEN]; /**< \brief Logical filename */
    char AppName[CFE_MISSION_MAX_API_LEN];      /**< \brief Application that opened file */
} FM_OpenFilesEntry_t;

/**
 *  \brief Get Open Files telemetry payload
 */
typedef struct
{
    uint32              NumOpenFiles;                             /**< \brief Number of files opened via cFE */
    FM_OpenFilesEntry_t OpenFilesList[CFE_MISSION_MAX_NUM_FILES]; /**< \brief List of files opened via cFE */
} FM_OpenFilesPkt_Payload_t;

/**
 *  \brief Monitor filesystem list entry structure
 */
typedef struct
{
    uint8  ReportType;
    uint8  Padding[7]; /**< \brief Padding to align Name (and subsequent members) to 64-bit boundaries */
    char   Name[CFE_MISSION_MAX_PATH_LEN]; /**< \brief File system name */
    uint64 Blocks;                         /**< \brief Block count from last check/poll, 0 if unknown */
    uint64 Bytes;                          /**< \brief Byte count from last check/poll, 0 if unknown */
} FM_MonitorReportEntry_t;

/**
 *  \brief Monitor filesystem telemetry payload
 */
typedef struct
{
    FM_MonitorReportEntry_t FileSys[FM_TABLE_ENTRY_COUNT]; /**< \brief Array of file system free space entries */
} FM_MonitorReportPkt_Payload_t;

/**
 * \brief Housekeeping telemetry payload
 */
typedef struct
{
    uint8 CommandCounter;    /**< \brief Application command counter */
    uint8 CommandErrCounter; /**< \brief Application command error counter */
    uint8 Spare;             /**< \brief Placeholder for unused command warning counter */

    uint8 NumOpenFiles; /**< \brief Number of open files in the system */

    uint8 ChildCmdCounter;     /**< \brief Child task command counter */
    uint8 ChildCmdErrCounter;  /**< \brief Child task command error counter */
    uint8 ChildCmdWarnCounter; /**< \brief Child task command warning counter */

    uint8 ChildQueueCount; /**< \brief Number of pending commands in queue */

    uint8 ChildCurrentCC;  /**< \brief Command code currently executing */
    uint8 ChildPreviousCC; /**< \brief Command code previously executed */
} FM_HkTlm_Payload_t;

/**\}*/

#endif
/* DEFAULT_FM_MSGDEFS_H */
