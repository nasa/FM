/*
** $Id: fm_tbl.c 1.3.1.2 2017/01/23 21:53:22EST sstrege Exp  $
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
** Title: File Manager (FM) Application Table Definitions
**
** Purpose: Provides functions for the initialization, validation, and
**          management of the FM File System Free Space Table
**
** Author: Susanne L. Strege, Code 582 NASA GSFC
**
** Notes:
**
**
*/

#include "cfe.h"
#include "fm_platform_cfg.h"
#include "fm_msg.h"
#include "fm_tbl.h"
#include "fm_events.h"
#include "cfs_utils.h"
#include "fm_perfids.h"
#include "fm_child.h"
#include "fm_test_utils.h"

/************************************************************************
** UT Includes
*************************************************************************/
#include "uttest.h"
#include "utassert.h"
#include "utstubs.h"

#include <string.h>


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM table function -- startup initialization                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 FM_TableInit(void)
{
    int32 status;
    status = UT_DEFAULT_IMPL(FM_TableInit);
    return status;
} /* End FM_TableInit */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM table function -- table data verification                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 FM_ValidateTable(FM_FreeSpaceTable_t *TablePtr)
{
    int32 status;
    status = UT_DEFAULT_IMPL(FM_ValidateTable);
    return status;
} /* End FM_ValidateTable */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM table function -- acquire table data pointer                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_AcquireTablePointers(void)
{
    UT_DEFAULT_IMPL(FM_AcquireTablePointers);
} /* End FM_AcquireTablePointers */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM table function -- release table data pointer                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ReleaseTablePointers(void)
{
    UT_DEFAULT_IMPL(FM_ReleaseTablePointers);
} /* End FM_ReleaseTablePointers */


/************************/
/*  End of File Comment */
/************************/

