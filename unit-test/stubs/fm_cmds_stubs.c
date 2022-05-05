/*
** $Id: fm_cmds.c 1.8.1.2 2017/01/23 21:52:56EST sstrege Exp  $
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
** Title: File Manager (FM) Application Ground Commands
**
** Purpose: Provides functions for the execution of the FM ground commands
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
#include "fm_events.h"
#include "fm_app.h"
#include "fm_cmds.h"
#include "fm_cmd_utils.h"
#include "fm_perfids.h"
#include "fm_platform_cfg.h"
#include "fm_version.h"
#include "fm_verify.h"
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
/* FM command handler -- NOOP                                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_NoopCmd(const CFE_SB_Buffer_t* BufPtr)
{
    return UT_DEFAULT_IMPL(FM_NoopCmd) != 0;
} /* End of FM_NoopCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Reset Counters                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_ResetCountersCmd(const CFE_SB_Buffer_t* BufPtr)
{
    return UT_DEFAULT_IMPL(FM_ResetCountersCmd) != 0;
} /* End of FM_ResetCountersCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Copy File                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_CopyFileCmd(const CFE_SB_Buffer_t* BufPtr)
{
    return UT_DEFAULT_IMPL(FM_CopyFileCmd) != 0;
} /* End of FM_CopyFileCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Move File                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_MoveFileCmd(const CFE_SB_Buffer_t* BufPtr)
{
    return UT_DEFAULT_IMPL(FM_MoveFileCmd) != 0;
} /* End of FM_MoveFileCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Rename File                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_RenameFileCmd(const CFE_SB_Buffer_t* BufPtr)
{
    return UT_DEFAULT_IMPL(FM_RenameFileCmd) != 0;
} /* End of FM_RenameFileCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Delete File                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_DeleteFileCmd(const CFE_SB_Buffer_t* BufPtr)
{
    return UT_DEFAULT_IMPL(FM_DeleteFileCmd) != 0;
} /* End of FM_DeleteFileCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Delete All Files                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_DeleteAllFilesCmd(const CFE_SB_Buffer_t* BufPtr)
{
    return UT_DEFAULT_IMPL(FM_DeleteAllFilesCmd) != 0;
} /* End of FM_DeleteAllFilesCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Decompress File                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifdef FM_INCLUDE_DECOMPRESS

bool FM_DecompressFileCmd(const CFE_SB_Buffer_t* BufPtr)
{
    return UT_DEFAULT_IMPL(FM_DecompressFileCmd) != 0;
} /* End of FM_DecompressFileCmd() */

#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Concatenate Files                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_ConcatFilesCmd(const CFE_SB_Buffer_t* BufPtr)
{
    return UT_DEFAULT_IMPL(FM_ConcatFilesCmd) != 0;
} /* End of FM_ConcatFilesCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Get File Info                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_GetFileInfoCmd(const CFE_SB_Buffer_t* BufPtr)
{
    return UT_DEFAULT_IMPL(FM_GetFileInfoCmd) != 0;
} /* End of FM_GetFileInfoCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Get List of Open Files                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_GetOpenFilesCmd(const CFE_SB_Buffer_t* BufPtr)
{
    return UT_DEFAULT_IMPL(FM_GetOpenFilesCmd) != 0;
} /* End of FM_GetOpenFilesCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Create Directory                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_CreateDirectoryCmd(const CFE_SB_Buffer_t* BufPtr)
{
    return UT_DEFAULT_IMPL(FM_CreateDirectoryCmd);
} /* End of FM_CreateDirectoryCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Delete Directory                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_DeleteDirectoryCmd(const CFE_SB_Buffer_t* BufPtr)
{
    return UT_DEFAULT_IMPL(FM_DeleteDirectoryCmd) != 0;
} /* End of FM_DeleteDirectoryCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Get List of Directory Entries (to file)   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_GetDirListFileCmd(const CFE_SB_Buffer_t* BufPtr)
{
    return UT_DEFAULT_IMPL(FM_GetDirListFileCmd) != 0;
} /* End of FM_GetDirListFileCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Get List of Directory Entries (to pkt)    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_GetDirListPktCmd(const CFE_SB_Buffer_t* BufPtr)
{
    return UT_DEFAULT_IMPL(FM_GetDirListPktCmd) != 0;
} /* End of FM_GetDirListPktCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Get File System Free Space                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_GetFreeSpaceCmd(const CFE_SB_Buffer_t* BufPtr)
{
    return UT_DEFAULT_IMPL(FM_GetFreeSpaceCmd) != 0;
} /* End of FM_GetFreeSpaceCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Set Table Entry Enable/Disable State      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_SetTableStateCmd(const CFE_SB_Buffer_t* BufPtr)
{
    return UT_DEFAULT_IMPL(FM_SetTableStateCmd) != 0;
} /* End of FM_SetTableStateCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Set Permissions for a file                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_SetPermissionsCmd(const CFE_SB_Buffer_t* BufPtr)
{
    return UT_DEFAULT_IMPL(FM_SetPermissionsCmd) != 0;
} /* End of FM_SetPermissionsCmd() */

/************************/
/*  End of File Comment */
/************************/

