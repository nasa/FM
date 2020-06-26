/*
** Filename: fm_defs.h
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
*
** Title: CFS File Manager (FM) Macro Definitions File
**
** Purpose: Value definitions
**
** Notes:
**
** References:
**    Flight Software Branch C Coding Standard Version 1.0a
**
*/

#ifndef _fm_defs_h_
#define _fm_defs_h_


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM child task semaphore does not exist                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define FM_CHILD_SEM_INVALID        0xFFFFFFFF


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM argument to not calculate CRC during Get File Info command   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define FM_IGNORE_CRC               0


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM directory entry definitions                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define FM_THIS_DIRECTORY           "."
#define FM_PARENT_DIRECTORY         ".."


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM filename status definitions                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define FM_NAME_IS_INVALID          0
#define FM_NAME_IS_NOT_IN_USE       1
#define FM_NAME_IS_FILE_OPEN        2
#define FM_NAME_IS_FILE_CLOSED      3
#define FM_NAME_IS_DIRECTORY        4


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM free space table entry state definitions                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define FM_TABLE_ENTRY_DISABLED     0
#define FM_TABLE_ENTRY_ENABLED      1
#define FM_TABLE_ENTRY_UNUSED       2


#endif /* _fm_defs_h_ */

/************************/
/*  End of File Comment */
/************************/

