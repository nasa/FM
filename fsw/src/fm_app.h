/*
** Filename: fm_app.h
**
** NASA Docket No. GSC-18,475-1, identified as “Core Flight Software System (CFS)
** File Manager Application Version 2.5.3
**
** Copyright © 2020 United States Government as represented by the Administrator of
** the National Aeronautics and Space Administration. All Rights Reserved. 
**
** Licensed under the Apache License, Version 2.0 (the "License"); 
** you may not use this file except in compliance with the License. 
**  
** You may obtain a copy of the License at 
** http://www.apache.org/licenses/LICENSE-2.0 
**
** Unless required by applicable law or agreed to in writing, software 
** distributed under the License is distributed on an "AS IS" BASIS, 
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
** See the License for the specific language governing permissions and 
** limitations under the License. 
**
** Title: CFS File Manager (FM) Application Header File
**
** Purpose: Unit specification for the CFS File Manager Application.
**
** Notes:
**
** References:
**    Flight Software Branch C Coding Standard Version 1.0a
**
*/

#ifndef _fm_app_h_
#define _fm_app_h_

#include "cfe.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM application global function prototypes                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
**  \brief Application entry point and main process loop
**
**  \par Description
**
**       Register FM as a CFE application.
**       Invoke FM application initialization function.
**       Enter FM main process loop.
**         Pend (forever) on next Software Bus command packet.
**         Process received Software Bus command packet.
**         Repeat main process loop.
**       Allow CFE to terminate the FM application.
**
**  \par Assumptions, External Events, and Notes: None
**
**  \sa #FM_AppInit, #CFE_ES_RunLoop, #CFE_SB_RcvMsg, #FM_ProcessPkt
**/
void FM_AppMain(void);


/**
**  \brief FM Application Initialization Function
**
**  \par Description
**
**       Initialize FM global data structure.
**       Register FM application for CFE Event Services.
**       Create Software Bus input pipe.
**       Subscribe to FM housekeeping request command packet.
**       Subscribe to FM ground command packet.
**       Invoke FM table initialization function.
**       Invoke FM child task initialization function.
**
**  \par Assumptions, External Events, and Notes: None
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS
**  \endcode
**  \retstmt Return codes from #CFE_EVS_Register, #CFE_SB_CreatePipe, #CFE_SB_Subscribe
**  \endcode
**  \endreturns
**
**  \sa #CFE_EVS_Register, #CFE_SB_CreatePipe, #CFE_SB_Subscribe
**/
int32 FM_AppInit(void);


/**
**  \brief Process Input Command Packets
**
**  \par Description
**
**       Branch to appropriate input packet handler: HK request or FM commands.
**
**  \par Assumptions, External Events, and Notes: None
**
**  \param [in]  MessagePtr - Pointer to Software Bus command packet.
**
**  \sa #FM_ReportHK, #FM_ProcessCmd
**/
void FM_ProcessPkt(CFE_SB_MsgPtr_t MessagePtr);


/**
**  \brief Process FM Ground Commands
**
**  \par Description
**
**       Branch to the command specific handlers for FM ground commands.
**
**  \par Assumptions, External Events, and Notes: None
**
**  \param [in]  MessagePtr - Pointer to Software Bus command packet.
**
**  \sa #FM_Noop, #FM_Reset, #FM_Copy, #FM_Move, #FM_Rename, #FM_Delete,
**      #FM_DeleteAll, #FM_Decompress, #FM_Concat, #FM_GetFileInfo,
**      #FM_GetOpenFiles, #FM_CreateDir, #FM_DeleteDir, #FM_GetDirFile,
**      #FM_GetDirPkt, #FM_GetFreeSpace
**/
void FM_ProcessCmd(CFE_SB_MsgPtr_t MessagePtr);


/**
**  \brief Housekeeping Request Command Handler
**
**  \par Description
**
**       Allow CFE Table Services the opportunity to manage the File System
**       Free Space Table.  This provides a mechanism to receive table updates.
**       
**       Populate the FM application Housekeeping Telemetry packet.  Timestamp
**       the packet and send it to ground via the Software Bus.
**
**  \par Assumptions, External Events, and Notes: None
**
**  \param [in]  MessagePtr - Pointer to Software Bus command packet.
**
**  \sa #FM_HousekeepingCmd_t, #FM_HousekeepingPkt_t
**/
void FM_ReportHK(CFE_SB_MsgPtr_t MessagePtr);


#endif /* _fm_app_h_ */

/************************/
/*  End of File Comment */
/************************/
