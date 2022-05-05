/*
** $Id: fm_app.c 1.5.1.2 2017/01/23 21:53:08EST sstrege Exp  $
**
**  Copyright (c) 2007-2014 United States Government as represented by the 
**  Administrator of the National Aeronautics and Space Administration. 
**  All Other Rights Reserved.  
**
**  This software was created at NASA's Goddard Space Flight Center.
**  This software is governed by the NASA Open Source Agreement and may be 
**  used, distributed and modified only pursuant to the terms of that 
**  agreement.
**
** Title: Core Flight System (CFS) File Manager (FM) Application
**
** Purpose: The File Manager (FM) Application provides onboard file system
**          management services by processing commands for copying and moving
**          files, decompressing files, concatenating files, creating directories,
**          deleting files and directories, and providing file and directory status.
**          When the File Manager application receives a housekeeping request
**          (scheduled within the scheduler application), FM  reports it's housekeeping
**          status values via telemetry messaging.
**
** Author: Susanne L. Strege, Code 582 NASA GSFC
**
** Notes:
**
*/

#include "cfe.h"
#include "fm_msg.h"
#include "fm_msgdefs.h"
#include "fm_msgids.h"
#include "fm_app.h"
#include "fm_tbl.h"
#include "fm_child.h"
#include "fm_cmds.h"
#include "fm_cmd_utils.h"
#include "fm_events.h"
#include "fm_perfids.h"
#include "fm_platform_cfg.h"
#include "fm_version.h"
#include "fm_verify.h"
#include "fm_test_utils.h"

#include <string.h>

/* UT includes */
#include "uttest.h"
#include "utassert.h"
#include "utstubs.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM application global data                                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

FM_GlobalData_t  FM_GlobalData;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM application -- entry point and main loop processor           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_AppMain(void)
{
    unimplemented(__func__, __FILE__, __LINE__);
} /* End FM_AppMain */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM application -- startup initialization processor              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 FM_AppInit(void)
{
    return UT_DEFAULT_IMPL(FM_AppInit); 
} /* End of FM_AppInit() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM application -- input packet processor                        */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ProcessPkt(const CFE_SB_Buffer_t *MessagePtr)
{
    UT_DEFAULT_IMPL(FM_ProcessPkt);
} /* End of FM_ProcessPkt */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM application -- command packet processor                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ProcessCmd(const CFE_SB_Buffer_t *MessagePtr)
{
    UT_DEFAULT_IMPL(FM_ProcessCmd);
    UT_Stub_CopyFromLocal(UT_KEY(FM_ProcessCmd), &MessagePtr, sizeof(MessagePtr));
} /* End of FM_ProcessCmd */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM application -- housekeeping request packet processor         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ReportHK(const CFE_MSG_CommandHeader_t *Msg)
{
    UT_DEFAULT_IMPL(FM_ReportHK);
} /* End of FM_ReportHK */


/************************/
/*  End of File Comment */
/************************/

