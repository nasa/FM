/*
** $Id: fm_cmd_utils.c 1.3.1.2 2017/01/23 21:53:21EST sstrege Exp  $
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
** Title: File Manager (FM) Command Utility Functions
**
** Purpose: Provides file manager utility function definitions for
**          processing file manager commands
**
** Author: Susanne L. Strege, Code 582 NASA GSFC
**
** Notes:
**
*/

#include "cfe.h"
#include "fm_msg.h"
#include "fm_cmd_utils.h"
#include "fm_child.h"
#include "fm_perfids.h"
#include "fm_events.h"
#include "cfs_utils.h"
#include "fm_test_utils.h"

/************************************************************************
** UT Includes
*************************************************************************/
#include "uttest.h"
#include "utassert.h"
#include "utstubs.h"

#include <string.h>
#include <ctype.h>

/* UT includes */
#include "uttest.h"
#include "utstubs.h"

// static uint32 OpenFileCount = 0;
// static bool FileIsOpen = false;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- verify command packet length             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_IsValidCmdPktLength(const CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength, 
					uint32 EventID, const char *CmdText)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(FM_IsValidCmdPktLength), MsgPtr);
    UT_Stub_RegisterContextGenericArg(UT_KEY(FM_IsValidCmdPktLength), ExpectedLength);
    UT_Stub_RegisterContextGenericArg(UT_KEY(FM_IsValidCmdPktLength), EventID);
    UT_Stub_RegisterContext(UT_KEY(FM_IsValidCmdPktLength), CmdText);

    bool status;

    status = UT_DEFAULT_IMPL(FM_IsValidCmdPktLength);

    return status;
} /* FM_IsValidCmdPktLength */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- verify state is not invalid              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_VerifyOverwrite(uint16 Overwrite, uint32 EventID, const char *CmdText)
{
    return UT_DEFAULT_IMPL(FM_VerifyOverwrite);
} /* End FM_VerifyOverwrite */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- get open files data                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
static void LoadOpenFileData(uint32 ObjId, void* CallbackArg)
{
    unimplemented(__func__, __FILE__, __LINE__);
} // End LoadOpenFileData() 
*/

uint32 FM_GetOpenFilesData(const FM_OpenFilesEntry_t *OpenFilesData)
{
    return UT_DEFAULT_IMPL(FM_GetOpenFilesData);
} /* End FM_GetOpenFilesData */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- query filename state                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
static void SearchOpenFileData(uint32 ObjId, void* CallbackArg)
{
    unimplemented(__func__, __FILE__, __LINE__);
} // End SearchOpenFileData() 
*/


uint32 FM_GetFilenameState(char *Filename, uint32 BufferSize, bool FileInfoCmd)
{
    UT_Stub_RegisterContext(UT_KEY(FM_GetFilenameState),Filename);
    UT_Stub_RegisterContextGenericArg(UT_KEY(FM_GetFilenameState),BufferSize);
    UT_Stub_RegisterContextGenericArg(UT_KEY(FM_GetFilenameState),FileInfoCmd);

    return UT_DEFAULT_IMPL(FM_GetFilenameState);
} /* End FM_GetFilenameState */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- verify state is not invalid              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint32 FM_VerifyNameValid(char *Name, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    return UT_DEFAULT_IMPL(FM_VerifyNameValid);
} /* End FM_VerifyNameValid */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- verify state is closed file              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_VerifyFileClosed(char *Filename, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    return UT_DEFAULT_IMPL(FM_VerifyFileClosed);
} /* End FM_VerifyFileClosed */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- verify state is open or closed file      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_VerifyFileExists(char *Filename, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    return UT_DEFAULT_IMPL(FM_VerifyFileExists);
} /* End FM_VerifyFileExists */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- verify state is unused                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_VerifyFileNoExist(char *Filename, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    return UT_DEFAULT_IMPL(FM_VerifyFileNoExist);
} /* End FM_VerifyFileNoExist */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- verify state is unused or closed file    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_VerifyFileNotOpen(char *Filename, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    return UT_DEFAULT_IMPL(FM_VerifyFileNotOpen);
} /* End FM_VerifyFileNotOpen */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- verify state is directory                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_VerifyDirExists(char *Directory, uint32 BufferSize, uint32 EventID, const char *CmdText)
{
    return UT_DEFAULT_IMPL(FM_VerifyDirExists);
} /* End FM_VerifyDirExists */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- verify state is unused                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_VerifyDirNoExist(char *Name, uint32 BufferSize, uint32 EventID, const char *CmdText)
{

    return UT_DEFAULT_IMPL(FM_VerifyDirNoExist);
} /* End FM_VerifyDirNoExist */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- verify child task interface is alive     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_VerifyChildTask(uint32 EventID, const char *CmdText)
{

    return UT_DEFAULT_IMPL(FM_VerifyChildTask);
} /* End FM_VerifyChildTask */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- invoke child task command processor      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_InvokeChildTask(void)
{

    UT_DEFAULT_IMPL(FM_InvokeChildTask);
} /* End of FM_InvokeChildTask */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM utility function -- add path separator to directory name     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_AppendPathSep(char *Directory, uint32 BufferSize)
{
    UT_DEFAULT_IMPL(FM_AppendPathSep);
} /* End of FM_AppendPathSep */


/************************/
/*  End of File Comment */
/************************/
