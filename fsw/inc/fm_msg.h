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
 *   Specification for the CFS FM command and telemetry messages.
 */
#ifndef FM_MSG_H
#define FM_MSG_H

#include "cfe.h"
#include "fm_platform_cfg.h"
#include "fm_extern_typedefs.h"

#ifdef FM_INCLUDE_DECOMPRESS
#include "cfs_fs_lib.h"
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM -- command packet structures                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * \defgroup cfsfmcmdstructs CFS File Manager Command Structures
 * \{
 */

/**
 *  \brief Housekeeping Request command packet structure
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */
} FM_HousekeepingCmd_t;

/**
 *  \brief No-Operation command packet structure
 *
 *  For command details see #FM_NOOP_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */
} FM_NoopCmd_t;

/**
 *  \brief Reset Counters command packet structure
 *
 *  For command details see #FM_RESET_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */
} FM_ResetCmd_t;

/**
 *  \brief Copy File command packet structure
 *
 *  For command details see #FM_COPY_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */

    uint16 Overwrite;               /**< \brief Allow overwrite */
    char   Source[OS_MAX_PATH_LEN]; /**< \brief Source filename */
    char   Target[OS_MAX_PATH_LEN]; /**< \brief Target filename */
} FM_CopyFileCmd_t;

/**
 *  \brief Move File command packet structure
 *
 *  For command details see #FM_MOVE_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */

    uint16 Overwrite;               /**< \brief Allow overwrite */
    char   Source[OS_MAX_PATH_LEN]; /**< \brief Source filename */
    char   Target[OS_MAX_PATH_LEN]; /**< \brief Target filename */
} FM_MoveFileCmd_t;

/**
 *  \brief Rename File command packet structure
 *
 *  For command details see #FM_RENAME_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */

    char Source[OS_MAX_PATH_LEN]; /**< \brief Source filename */
    char Target[OS_MAX_PATH_LEN]; /**< \brief Target filename */
} FM_RenameFileCmd_t;

/**
 *  \brief Delete File command packet structure
 *
 *  For command details see #FM_DELETE_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader;                 /**< \brief Command header */
    char                    Filename[OS_MAX_PATH_LEN]; /**< \brief Delete filename */
} FM_DeleteFileCmd_t;

/**
 *  \brief Delete All command packet structure
 *
 *  For command details see #FM_DELETE_ALL_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader;                  /**< \brief Command header */
    char                    Directory[OS_MAX_PATH_LEN]; /**< \brief Directory name */
} FM_DeleteAllCmd_t;

/**
 *  \brief Decompress File command packet structure
 *
 *  For command details see #FM_DECOMPRESS_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader;               /**< \brief Command header */
    char                    Source[OS_MAX_PATH_LEN]; /**< \brief Source filename */
    char                    Target[OS_MAX_PATH_LEN]; /**< \brief Target filename */
} FM_DecompressCmd_t;

/**
 *  \brief Concatenate Files command packet structure
 *
 *  For command details see #FM_CONCAT_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */

    char Source1[OS_MAX_PATH_LEN]; /**< \brief Source 1 filename */
    char Source2[OS_MAX_PATH_LEN]; /**< \brief Source 2 filename */
    char Target[OS_MAX_PATH_LEN];  /**< \brief Target filename */
} FM_ConcatCmd_t;

/**
 *  \brief Get File Info command packet structure
 *
 *  For command details see #FM_GET_FILE_INFO_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */

    char   Filename[OS_MAX_PATH_LEN]; /**< \brief Filename */
    uint32 FileInfoCRC;               /**< \brief File info CRC method */
} FM_GetFileInfoCmd_t;

/**
 *  \brief Get Open Files command packet structure
 *
 *  For command details see #FM_GET_OPEN_FILES_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */
} FM_GetOpenFilesCmd_t;

/**
 *  \brief Create Directory command packet structure
 *
 *  For command details see #FM_CREATE_DIR_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */

    char Directory[OS_MAX_PATH_LEN]; /**< \brief Directory name */
} FM_CreateDirCmd_t;

/**
 *  \brief Delete Directory command packet structure
 *
 *  For command details see #FM_DELETE_DIR_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */

    char Directory[OS_MAX_PATH_LEN]; /**< \brief Directory name */
} FM_DeleteDirCmd_t;

/**
 *  \brief Get DIR List to File command packet structure
 *
 *  For command details see #FM_GET_DIR_FILE_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */

    char  Directory[OS_MAX_PATH_LEN]; /**< \brief Directory name */
    char  Filename[OS_MAX_PATH_LEN];  /**< \brief Filename */
    uint8 GetSizeTimeMode;            /**< \brief Option to query size, time, and mode of files (CPU intensive) */
    uint8 Spare01[3];                 /**< \brief Padding to 32 bit boundary */
} FM_GetDirFileCmd_t;

/**
 *  \brief Get DIR List to Packet command packet structure
 *
 *  For command details see #FM_GET_DIR_PKT_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */

    char   Directory[OS_MAX_PATH_LEN]; /**< \brief Directory name */
    uint32 DirListOffset;              /**< \brief Index of 1st dir entry to put in packet */
    uint8  GetSizeTimeMode;            /**< \brief Option to query size, time, and mode of files (CPU intensive) */
    uint8  Spare01[3];                 /**< \brief Padding to 32 bit boundary */
} FM_GetDirPktCmd_t;

/**
 *  \brief Get Free Space command packet structure
 *
 *  For command details see #FM_MONITOR_FILESYSTEM_SPACE_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */
} FM_MonitorFilesystemSpaceCmd_t;

/**
 *  \brief Set Table State command packet structure
 *
 *  For command details see #FM_SET_TABLE_STATE_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */

    uint32 TableEntryIndex; /**< \brief Table entry index */
    uint32 TableEntryState; /**< \brief New table entry state */
} FM_SetTableStateCmd_t;

/**
 *  \brief Set Permissions for a file
 *
 *  For command details see #FM_SET_FILE_PERM_CC
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */

    char   FileName[OS_MAX_PATH_LEN]; /**< \brief File name of the permissions to set */
    uint32 Mode;                      /**< \brief Permissions, passed directly to OS_chmod */
} FM_SetPermCmd_t;

/**\}*/

/**
 * \defgroup cfsfmtlm CFS File Manager Telemetry
 * \{
 */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM -- get directory listing telemetry structures                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 *  \brief Get Directory Listing entry structure
 */
typedef struct
{
    char   EntryName[OS_MAX_PATH_LEN]; /**< \brief Directory Listing Filename */
    uint32 EntrySize;                  /**< \brief Directory Listing File Size */
    uint32 ModifyTime;                 /**< \brief Directory Listing File Last Modification Times */
    uint32 Mode;                       /**< \brief Mode of the file (Permissions from #OS_FILESTAT_MODE) */
} FM_DirListEntry_t;

/**
 *  \brief Get Directory Listing telemetry packet
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t TlmHeader; /**< \brief Telemetry Header */

    char              DirName[OS_MAX_PATH_LEN];          /**< \brief Directory Name */
    uint32            TotalFiles;                        /**< \brief Number of files in the directory */
    uint32            PacketFiles;                       /**< \brief Number of files in this packet */
    uint32            FirstFile;                         /**< \brief Index into directory files of first packet file */
    FM_DirListEntry_t FileList[FM_DIR_LIST_PKT_ENTRIES]; /**< \brief Directory listing file data */
} FM_DirListPkt_t;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM -- get directory listing to file structures                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 *  \brief Get Directory Listing file statistics structure
 */
typedef struct
{
    char   DirName[OS_MAX_PATH_LEN]; /**< \brief Directory name */
    uint32 DirEntries;               /**< \brief Number of entries in the directory */
    uint32 FileEntries;              /**< \brief Number of entries written to output file */
} FM_DirListFileStats_t;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM -- get file information telemetry structure                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 *  \brief Get File Info telemetry packet
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t TlmHeader; /**< \brief Telemetry Header */

    uint8  FileStatus;                /**< \brief Status indicating whether the file is open or closed */
    uint8  CRC_Computed;              /**< \brief Flag indicating whether a CRC was computed or not */
    uint8  Spare[2];                  /**< \brief Structure padding */
    uint32 CRC;                       /**< \brief CRC value if computed */
    uint32 FileSize;                  /**< \brief File Size */
    uint32 LastModifiedTime;          /**< \brief Last Modification Time of File */
    uint32 Mode;                      /**< \brief Mode of the file (Permissions) */
    char   Filename[OS_MAX_PATH_LEN]; /**< \brief Name of File */
} FM_FileInfoPkt_t;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM -- get open files list telemetry structures                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 *  \brief Get Open Files list entry structure
 */
typedef struct
{
    char LogicalName[OS_MAX_PATH_LEN]; /**< \brief Logical filename */
    char AppName[OS_MAX_API_NAME];     /**< \brief Application that opened file */
} FM_OpenFilesEntry_t;

/**
 *  \brief Get Open Files telemetry packet
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t TlmHeader; /**< \brief Telemetry Header */

    uint32              NumOpenFiles;                         /**< \brief Number of files opened via cFE */
    FM_OpenFilesEntry_t OpenFilesList[OS_MAX_NUM_OPEN_FILES]; /**< \brief List of files opened via cFE */
} FM_OpenFilesPkt_t;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM -- monitor filesystem telemetry structures                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 *  \brief Monitor filesystem list entry structure
 */
typedef struct
{
    uint8  ReportType;
    char   Name[OS_MAX_PATH_LEN]; /**< \brief File system name */
    uint64 Blocks;                /**< \brief Block count from last check/poll, 0 if unknown */
    uint64 Bytes;                 /**< \brief Byte count from last check/poll, 0 if unknown */
} FM_MonitorReportEntry_t;

/**
 *  \brief Monitor filesystem telemetry packet
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t TlmHeader; /**< \brief Telemetry Header */

    FM_MonitorReportEntry_t FileSys[FM_TABLE_ENTRY_COUNT]; /**< \brief Array of file system free space entries */
} FM_MonitorReportPkt_t;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM -- housekeeping telemetry structure                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * \brief Housekeeping telemetry packet
 */
typedef struct
{
    CFE_MSG_TelemetryHeader_t TlmHeader; /**< \brief Telemetry Header */

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
} FM_HousekeepingPkt_t;

/**\}*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM -- monitor filesyste table structures                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef enum
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

} FM_MonitorTableEntry_Type_t;

/**
 *  \brief Monitor table entry
 */
typedef struct
{
    /**
     * Table entry type.
     *
     * This should be one of the enumeration values in FM_MonitorTableEntry_Type_t.
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
     * See description of the FM_MonitorTableEntry_Type_t for how this is to be set
     */
    char Name[OS_MAX_PATH_LEN];

} FM_MonitorTableEntry_t;

/**
 *  \brief Get Free Space table definition
 */
typedef struct
{
    FM_MonitorTableEntry_t Entries[FM_TABLE_ENTRY_COUNT]; /**< \brief One entry for each monitor */
} FM_MonitorTable_t;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM -- child task interface command queue entry                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 *  \brief Child Task Interface command queue entry structure
 */
typedef struct
{
    CFE_MSG_FcnCode_t CommandCode;              /**< \brief Command code - identifies the command */
    uint32            DirListOffset;            /**< \brief Starting entry for dir list commands */
    uint32            FileInfoState;            /**< \brief File info state */
    uint32            FileInfoSize;             /**< \brief File info size */
    uint32            FileInfoTime;             /**< \brief File info time */
    uint32            FileInfoCRC;              /**< \brief File info CRC method */
    char              Source1[OS_MAX_PATH_LEN]; /**< \brief First source file or directory name command argument */
    char              Source2[OS_MAX_PATH_LEN]; /**< \brief Second source filename command argument */
    char              Target[OS_MAX_PATH_LEN];  /**< \brief Target filename command argument */
    uint8             GetSizeTimeMode; /**< \brief Whether to invoke stat call for size and time (CPU intensive) */
    uint32            Mode;            /**< \brief File Mode */
} FM_ChildQueueEntry_t;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM -- application global data structure                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 *  \brief Application global data structure
 */
typedef struct
{
    FM_MonitorTable_t *MonitorTablePtr;    /**< \brief File System Table Pointer */
    CFE_TBL_Handle_t   MonitorTableHandle; /**< \brief File System Table Handle */

    CFE_SB_PipeId_t CmdPipe; /**< \brief cFE software bus command pipe */

    CFE_ES_TaskId_t ChildTaskID;        /**< \brief Child task ID */
    osal_id_t       ChildSemaphore;     /**< \brief Child task wakeup counting semaphore */
    osal_id_t       ChildQueueCountSem; /**< \brief Child queue counter mutex semaphore */

    uint8 ChildCmdCounter;     /**< \brief Child task command success counter */
    uint8 ChildCmdErrCounter;  /**< \brief Child task command error counter */
    uint8 ChildCmdWarnCounter; /**< \brief Child task command warning counter */

    uint8 ChildWriteIndex; /**< \brief Array index for next write to command args */
    uint8 ChildReadIndex;  /**< \brief Array index for next read from command args */
    uint8 ChildQueueCount; /**< \brief Number of pending commands in queue */

    uint8 CommandCounter;    /**< \brief Application command success counter */
    uint8 CommandErrCounter; /**< \brief Application command error counter */
    uint8 Spare8a;           /**< \brief Placeholder for unused command warning counter */

    uint8 ChildCurrentCC;  /**< \brief Command code currently executing */
    uint8 ChildPreviousCC; /**< \brief Command code previously executed */
    uint8 Spare8b;         /**< \brief Structure alignment spare */

    uint32 FileStatTime; /**< \brief Modify time from most recent OS_stat */
    uint32 FileStatSize; /**< \brief File size from most recent OS_stat */
    uint32 FileStatMode; /**< \brief File mode from most recent OS_stat (OS_FILESTAT_MODE) */

    FM_DirListFileStats_t DirListFileStats; /**< \brief Get dir list to file statistics structure */

    FM_DirListPkt_t DirListPkt; /**< \brief Get dir list to packet telemetry packet */

    FM_MonitorReportPkt_t
        MonitorReportPkt; /**< \brief Telemetry packet reporting status of items in the monitor table */

    FM_FileInfoPkt_t FileInfoPkt; /**< \brief Get file info telemetry packet */

    FM_OpenFilesPkt_t OpenFilesPkt; /**< \brief Get open files telemetry packet */

    FM_HousekeepingPkt_t HousekeepingPkt; /**< \brief Application housekeeping telemetry packet */

    char ChildBuffer[FM_CHILD_FILE_BLOCK_SIZE]; /**< \brief Child task file I/O buffer */

    FM_ChildQueueEntry_t ChildQueue[FM_CHILD_QUEUE_DEPTH]; /**< \brief Child task command queue */

#ifdef FM_INCLUDE_DECOMPRESS
    FS_LIB_Decompress_State_t DecompressState;

#endif
} FM_GlobalData_t;

/** \brief File Manager global */
extern FM_GlobalData_t FM_GlobalData;

#endif
