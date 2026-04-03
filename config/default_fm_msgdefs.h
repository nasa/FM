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
 * \brief A generic struct containing a path to a source file, a path to a
 *        target file, and a flag to indicate whether an overwrite should occur
 *        that is used by several commands
 *
 * Used by #FM_COPY_FILE_CC, #FM_MOVE_FILE_CC
 */
typedef struct
{
    uint16 Overwrite;                            /** \brief Allow overwrite */
    char   SourcePath[CFE_MISSION_MAX_PATH_LEN]; /** \brief Path to source file */
    char   TargetPath[CFE_MISSION_MAX_PATH_LEN]; /** \brief Path to target file */
} FM_OverwriteSourceTarget_Payload_t;

/**
 * \brief A generic struct containing a path to a source file and a path to a
 *        target file that is used by several commands
 *
 * Used by #FM_RENAME_FILE_CC, #FM_DECOMPRESS_FILE_CC
 */
typedef struct
{
    char SourcePath[CFE_MISSION_MAX_PATH_LEN]; /** \brief Path to source file */
    char TargetPath[CFE_MISSION_MAX_PATH_LEN]; /** \brief Path to target file */
} FM_SourceTarget_Payload_t;

/**
 * \brief A generic struct containing a path to a file used by several commands
 *
 * Used by #FM_DELETE_FILE_CC, #FM_DELETE_ALL_FILES_CC, #FM_CREATE_DIRECTORY_CC,
 * #FM_DELETE_DIRECTORY_CC
 */

typedef struct
{
    char Path[CFE_MISSION_MAX_PATH_LEN]; /** \brief Path to file */
} FM_Path_Payload_t;

/**
 * \brief Concat the file at SourcePath2 onto the file at SourcePath1 and
 *        name the final file TargetPath
 *
 * Used by #FM_CONCAT_FILES_CC
 */
typedef struct
{
    char SourcePath1[CFE_MISSION_MAX_PATH_LEN]; /** \brief Path to source file 1 */
    char SourcePath2[CFE_MISSION_MAX_PATH_LEN]; /** \brief Path to source file 2 */
    char TargetPath[CFE_MISSION_MAX_PATH_LEN];  /** \brief Path to target file */
} FM_ConcatFilesCmd_Payload_t;

/**
 *  \brief Get info about file represented by Path and provide the CRC method
 *
 *  Used by #FM_GET_FILE_INFO_CC
 */
typedef struct
{
    char   Path[CFE_MISSION_MAX_PATH_LEN]; /** \brief Path to file */
    uint32 CRC_Method;                     /** \brief CRC method */
} FM_GetFileInfoCmd_Payload_t;

/**
 *  \brief Get Directory and output to file command payload
 *
 * Contains a directory and output file name, with optional flags
 * Used by #FM_GET_DIR_LIST_FILE_CC
 */
typedef struct
{
    char  DirectoryPath[CFE_MISSION_MAX_PATH_LEN];  /** \brief Path to directory to output */
    char  OutputFilePath[CFE_MISSION_MAX_PATH_LEN]; /** \brief Path to file that holds output */
    uint8 GetSizeTimeMode; /** \brief Option to query size, time, and mode of files (CPU intensive) */
    uint8 Padding[3];      /** \brief Padding to 32*bit boundary */
} FM_GetDirListFileCmd_Payload_t;

/**
 *  \brief Get Directory and output to packet command payload
 *
 * Contains a directory and position offset, with optional flags
 * Used by #FM_GET_DIR_LIST_PKT_CC
 */
typedef struct
{
    char   Path[CFE_MISSION_MAX_PATH_LEN]; /** \brief Path to directory to output */
    uint32 Offset;                         /** \brief Index of first entry to put in packet */
    uint8  GetSizeTimeMode;                /** \brief Option to query size, time, and mode of files (CPU intensive) */
    uint8  Padding[3];                     /** \brief Padding to 32-bit boundary */
} FM_GetDirListPktCmd_Payload_t;

/**
 *  \brief Table Index and State command payload structure
 *
 *  Used by #FM_SET_TABLE_STATE_CC
 */
typedef struct
{
    uint32 Index; /** \brief Table entry index */
    uint32 State; /** \brief New table entry state */
} FM_SetTableStateCmd_Payload_t;

/**
 *  \brief File name and mode command payload structure
 *
 *  Used by #FM_SET_PERMISSIONS_CC
 */
typedef struct
{
    char   Path[CFE_MISSION_MAX_PATH_LEN]; /** \brief Path to file whose permissions are to be set */
    uint32 Permissions;                    /** \brief Permissions for the file */
} FM_SetPermissionsCmd_Payload_t;

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
    char   EntryName[CFE_MISSION_MAX_PATH_LEN]; /** \brief Directory Listing Filename */
    uint32 Size;                                /** \brief Directory Listing File Size */
    uint32 ModifyTime;                          /** \brief Directory Listing File Last Modification Times */
    uint32 Permissions;                         /** \brief Mode of the file (Permissions from #OS_FILESTAT_MODE) */
} FM_DirListEntry_t;

/**
 *  \brief Get Directory Listing telemetry payload
 */
typedef struct
{
    char              DirectoryName[CFE_MISSION_MAX_PATH_LEN]; /** \brief Directory Name */
    uint32            TotalFiles;                              /** \brief Number of files in the directory */
    uint32            FilesInPacket;                           /** \brief Number of files in this packet */
    uint32            FirstFileIndex;                    /** \brief Index into directory files of first packet file */
    FM_DirListEntry_t FileList[FM_DIR_LIST_PKT_ENTRIES]; /** \brief Directory listing file data */
} FM_DirListPkt_Payload_t;

/**
 *  \brief Get File Info telemetry payload
 */
typedef struct
{
    uint8  Status;                         /** \brief Status indicating whether the file is open or closed */
    uint8  CRC_Computed;                   /** \brief Flag indicating whether a CRC was computed or not */
    uint8  Padding[2];                     /** \brief Structure padding */
    uint32 CRC;                            /** \brief CRC value if computed */
    uint32 Size;                           /** \brief File Size */
    uint32 LastModifiedTime;               /** \brief Last Modification Time of File */
    uint32 Permissions;                    /** \brief Mode of the file (Permissions) */
    char   Name[CFE_MISSION_MAX_PATH_LEN]; /** \brief Name of File */
} FM_FileInfoPkt_Payload_t;

/**
 *  \brief Get Open Files list entry structure
 */
typedef struct
{
    char LogicalName[CFE_MISSION_MAX_PATH_LEN]; /** \brief Logical filename */
    char App[CFE_MISSION_MAX_API_LEN];          /** \brief Application that opened file */
} FM_OpenFilesEntry_t;

/**
 *  \brief Get Open Files telemetry payload
 */
typedef struct
{
    uint32              Count;                                    /** \brief Number of files opened via cFE */
    FM_OpenFilesEntry_t OpenFilesList[CFE_MISSION_MAX_NUM_FILES]; /** \brief List of files opened via cFE */
} FM_OpenFilesPkt_Payload_t;

/**
 *  \brief Monitor filesystem list entry structure
 */
typedef struct
{
    uint8  ReportType;
    uint8  Padding1[7];                    /** \brief Padding for alignment */
    char   Path[CFE_MISSION_MAX_PATH_LEN]; /** \brief File system path */
    uint8  Padding2[4];                    /** \brief Padding for alignment */
    uint64 Blocks;                         /** \brief Block count from last check/poll, 0 if unknown */
    uint64 Bytes;                          /** \brief Byte count from last check/poll, 0 if unknown */
} FM_MonitorReportEntry_t;

/**
 *  \brief Monitor filesystem telemetry payload
 */
typedef struct
{
    FM_MonitorReportEntry_t FileSys[FM_TABLE_ENTRY_COUNT]; /** \brief Array of file system free space entries */
} FM_MonitorReportPkt_Payload_t;

/**
 * \brief Housekeeping telemetry payload
 */
typedef struct
{
    uint8 CommandCounter;      /** \brief Application command counter */
    uint8 CommandErrorCounter; /** \brief Application command error counter */

    uint8 OpenFiles; /** \brief Number of open files in the system */

    uint8 ChildCommandCounter;        /** \brief Child task command counter */
    uint8 ChildCommandErrorCounter;   /** \brief Child task command error counter */
    uint8 ChildCommandWarningCounter; /** \brief Child task command warning counter */

    uint8 ChildQueueCount; /** \brief Number of pending commands in queue */

    uint8 ChildCurrentCC;  /** \brief Command code currently executing */
    uint8 ChildPreviousCC; /** \brief Command code previously executed */
    uint8 Padding[3];      /** \brief Explicit padding for packet alignment */
} FM_HkTlm_Payload_t;

/**\}*/

#endif
/* DEFAULT_FM_MSGDEFS_H */
