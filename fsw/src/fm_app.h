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
 *   Unit specification for the CFS File Manager Application.
 */
#ifndef FM_APP_H
#define FM_APP_H

#include "cfe.h"
#include "fm_msg.h"
#include "fm_compression.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM -- application global constants                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 *  \brief Wakeup for FM
 *
 *  \par Description
 *      Wakes up FM every 1 second for routine maintenance whether a
 *      message was received or not.
 */
#define FM_SB_TIMEOUT 1000

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

    /**
     * \brief State of the embedded decompression routine
     * This depends on the decompression option and may be NULL
     */
    FM_Decompressor_State_t *DecompressorStatePtr;

    /**
     * \brief State of the embedded compression routine
     * This depends on the compression option and may be NULL
     */
    FM_Compressor_State_t *CompressorStatePtr;
} FM_GlobalData_t;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM application global function prototypes                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 *  \brief Application entry point and main process loop
 *
 *  \par Description
 *
 *       Register FM as a CFE application.
 *       Invoke FM application initialization function.
 *       Enter FM main process loop.
 *         Pend (forever) on next Software Bus command packet.
 *         Process received Software Bus command packet.
 *         Repeat main process loop.
 *       Allow CFE to terminate the FM application.
 *
 *  \par Assumptions, External Events, and Notes: None
 *
 *  \sa #FM_AppInit, #CFE_ES_RunLoop, #FM_ProcessPkt
 */
void FM_AppMain(void);

/**
 *  \brief FM Application Initialization Function
 *
 *  \par Description
 *
 *       Initialize FM global data structure.
 *       Register FM application for CFE Event Services.
 *       Create Software Bus input pipe.
 *       Subscribe to FM housekeeping request command packet.
 *       Subscribe to FM ground command packet.
 *       Invoke FM table initialization function.
 *       Invoke FM child task initialization function.
 *
 *  \par Assumptions, External Events, and Notes: None
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 *
 *  \sa #CFE_EVS_Register, #CFE_SB_CreatePipe, #CFE_SB_Subscribe
 */
CFE_Status_t FM_AppInit(void);

/**
 *  \brief Housekeeping Request Command Handler
 *
 *  \par Description
 *
 *       Allow CFE Table Services the opportunity to manage the File System
 *       Free Space Table.  This provides a mechanism to receive table updates.
 *
 *       Populate the FM application Housekeeping Telemetry packet.  Timestamp
 *       the packet and send it to ground via the Software Bus.
 *
 *  \par Assumptions, External Events, and Notes: None
 *
 *  \param [in]  BufPtr Pointer to Software Bus command packet.
 *
 *  \sa #FM_SendHkCmd_t, #FM_HousekeepingPkt_t
 */
void FM_SendHkCmd(const CFE_SB_Buffer_t *BufPtr);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM application global data structure instance                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/** \brief File Manager global */
extern FM_GlobalData_t FM_GlobalData;

#endif
