/************************************************************************
 * NASA Docket No. GSC-18,918-1, and identified as “Core Flight
 * Software System (cFS) File Manager Application Version 2.6.0”
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM application global function prototypes                       */
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
int32 FM_AppInit(void);

/**
 *  \brief Process Input Command Packets
 *
 *  \par Description
 *
 *       Branch to appropriate input packet handler: HK request or FM commands.
 *
 *  \par Assumptions, External Events, and Notes: None
 *
 *  \param [in] MessagePtr Pointer to Software Bus message buffer.
 *
 *  \sa #FM_ReportHK, #FM_ProcessCmd
 */
void FM_ProcessPkt(const CFE_SB_Buffer_t *MessagePtr);

/**
 *  \brief Process FM Ground Commands
 *
 *  \par Description
 *
 *       Branch to the command specific handlers for FM ground commands.
 *
 *  \par Assumptions, External Events, and Notes: None
 *
 *  \param [in]  BufPtr Pointer to Software Bus message buffer.
 */
void FM_ProcessCmd(const CFE_SB_Buffer_t *BufPtr);

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
 *  \param [in]  Msg Pointer to Software Bus command packet.
 *
 *  \sa #FM_HousekeepingCmd_t, #FM_HousekeepingPkt_t
 */
void FM_ReportHK(const CFE_MSG_CommandHeader_t *Msg);

#endif
