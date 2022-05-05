/*
** $Id: fm_child.c 1.16.1.5 2017/01/24 23:53:32EST mdeschu Exp  $
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
** Purpose: File Manager (FM) Child task (low priority command handler)
**
** Author: Scott Walling (Microtel)
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
#include "fm_child.h"
#include "fm_cmds.h"
#include "fm_cmd_utils.h"
#include "fm_perfids.h"
#include "fm_platform_cfg.h"
#include "fm_verify.h"
#include "fm_test_utils.h"

#include "uttest.h"
#include "utassert.h"
#include "utstubs.h"

#include <string.h>

#ifdef FM_INCLUDE_DECOMPRESS
#include "cfs_fs_lib.h"
#endif

/************************************************************************
** OSAL Compatibility for directory name access
** New OSAL version have an access macro to get the string.  If that
** macro is defined, use it, otherwise assume "d_name" structure member.
*************************************************************************/
#ifndef OS_DIRENTRY_NAME
#define OS_DIRENTRY_NAME(x)     ((x).d_name)
#endif


#define FM_QUEUE_SEM_NAME "FM_QUEUE_SEM"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task -- startup initialization                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 FM_ChildInit(void)
{
   return UT_DEFAULT_IMPL(FM_ChildInit); 
} /* End of FM_ChildInit() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task -- task entry point                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildTask(void)
{
    unimplemented(__func__, __FILE__, __LINE__);
} /* End of FM_ChildTask() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task -- main process loop                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildLoop(void)
{
    UT_DEFAULT_IMPL(FM_ChildLoop);
} /* End of FM_ChildLoop() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task -- interface handshake processor                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildProcess(void)
{
    UT_DEFAULT_IMPL(FM_ChildProcess);
} /* End of FM_ChildProcess() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Copy File                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildCopyCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_Stub_RegisterContext(UT_KEY(FM_ChildCopyCmd),CmdArgs);
    UT_DEFAULT_IMPL(FM_ChildCopyCmd);
} /* End of FM_ChildCopyCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Move File                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildMoveCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_DEFAULT_IMPL(FM_ChildMoveCmd);
} /* End of FM_ChildMoveCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Rename File                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildRenameCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_DEFAULT_IMPL(FM_ChildRenameCmd);
} /* End of FM_ChildRenameCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Delete File                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildDeleteCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_DEFAULT_IMPL(FM_ChildDeleteCmd);
} /* End of FM_ChildDeleteCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Delete All Files               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildDeleteAllCmd(FM_ChildQueueEntry_t *CmdArgs)
{
    UT_DEFAULT_IMPL(FM_ChildDeleteAllCmd);
} /* End of FM_ChildDeleteAllCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Decompress File                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef FM_INCLUDE_DECOMPRESS

void FM_ChildDecompressCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_DEFAULT_IMPL(FM_ChildDecompressCmd);
} /* End of FM_ChildDecompressCmd() */

#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Concatenate Files              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildConcatCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_DEFAULT_IMPL(FM_ChildConcatCmd);
} /* End of FM_ChildConcatCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Get File Info                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildFileInfoCmd(FM_ChildQueueEntry_t *CmdArgs)
{
    UT_DEFAULT_IMPL(FM_ChildFileInfoCmd);
} /* End of FM_ChildFileInfoCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Create Directory               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildCreateDirCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_DEFAULT_IMPL(FM_ChildCreateDirCmd);
} /* End of FM_ChildCreateDirCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Delete Directory               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildDeleteDirCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_DEFAULT_IMPL(FM_ChildDeleteDirCmd);

} /* End of FM_ChildDeleteDirCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Get Directory List (to file)   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildDirListFileCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_DEFAULT_IMPL(FM_ChildDeleteDirCmd);
} /* End of FM_ChildDirListFileCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Get Directory List (to pkt)    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildDirListPktCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_DEFAULT_IMPL(FM_ChildDirListPktCmd);
} /* End of FM_ChildDirListPktCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task command handler -- Set File Permissions           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void FM_ChildSetPermissionsCmd(const FM_ChildQueueEntry_t *CmdArgs)
{
    UT_DEFAULT_IMPL(FM_ChildSetPermissionsCmd);
}  /* End of FM_ChildSetPermissionsCmd() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task utility function -- create dir list output file   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_ChildDirListFileInit(int32 *FileHandlePtr,const char *Directory,const char *Filename)
{
    return UT_DEFAULT_IMPL(FM_ChildDirListFileInit);
} /* End FM_ChildDirListFileInit */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task utility function -- write to dir list output file */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildDirListFileLoop(uint32 DirId, int32 FileHandle,
                             const char *Directory, const char *DirWithSep,
                             const char *Filename, uint8 getSizeTimeMode)
{
    unimplemented(__func__, __FILE__, __LINE__);
} /* End of FM_ChildDirListFileLoop */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task utility function -- get dir entry size and time   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 FM_ChildSizeTimeMode(const char *Filename, uint32 *FileSize, uint32 *FileTime, uint32 *FileMode)
{
    return UT_DEFAULT_IMPL(FM_ChildSizeTimeMode);
} /* End of FM_ChildSizeTimeMode */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task utility function -- sleep between OS_stat on files*/
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_ChildSleepStat(const char *Filename, FM_DirListEntry_t *DirListData, int32 *FilesTillSleep, bool getSizeTimeMode)
{
    UT_Stub_RegisterContext(UT_KEY(FM_ChildSleepStat), Filename);
    UT_Stub_RegisterContext(UT_KEY(FM_ChildSleepStat), DirListData);
    UT_Stub_RegisterContext(UT_KEY(FM_ChildSleepStat), FilesTillSleep);
    UT_Stub_RegisterContextGenericArg(UT_KEY(FM_ChildSleepStat), getSizeTimeMode);
    UT_DEFAULT_IMPL(FM_ChildSleepStat);
} /* FM_ChildSleepStat */

/************************/
/*  End of File Comment */
/************************/

