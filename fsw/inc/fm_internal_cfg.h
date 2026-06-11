/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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
 *   CFS File Manager (FM) Application Private Config Definitions
 *
 * This provides default values for configurable items that are internal
 * to this module and do NOT affect the interface(s) of this module.  Changes
 * to items in this file only affect the local module and will be transparent
 * to external entities that are using the public interface(s).
 *
 * @note This file may be overridden/superceded by mission-provided defintions
 * either by overriding this header or by generating definitions from a command/data
 * dictionary tool.
 */

#ifndef FM_INTERNAL_CFG_H
#define FM_INTERNAL_CFG_H

/* ======== */
/* Includes */
/* ======== */

#include "cfe.h"
#include "fm_internal_cfg_values.h"

/* ====== */
/* Macros */
/* ====== */

/**
 * \defgroup cfsfmplatformcfg CFS File Manager Internal Configuration
 * \{
 */

/**
 * \brief FM directory entry definitions
 */
#define FM_THIS_DIRECTORY                    FM_INTERNAL_CFGVAL(THIS_DIRECTORY)
#define DEFAULT_FM_INTERNAL_THIS_DIRECTORY   "."
#define FM_PARENT_DIRECTORY                  FM_INTERNAL_CFGVAL(PARENT_DIRECTORY)
#define DEFAULT_FM_INTERNAL_PARENT_DIRECTORY ".."

/**
 * \brief File Manager Application Name
 *
 *  \par Description:
 *       This definition must match the name used at startup by the cFE
 *       Executive Services when creating the FM application.  Note that
 *       application names are also an argument to certain cFE commands.
 *       For example, the application name is needed to access tables
 *       via cFE Table Services commands.
 *
 *  \par Limits:
 *       FM requires that this name be defined, but otherwise places
 *       no limits on the definition.  Refer to CFE Executive Services
 *       for specific information on limits related to application names.
 */
#define FM_APP_NAME                  FM_INTERNAL_CFGVAL(APP_NAME)
#define DEFAULT_FM_INTERNAL_APP_NAME "FM"

/**
 * \brief File Manager Command Pipe Name
 *
 *  \par Description:
 *       This definition is the name used at startup when creating a cFE
 *       Software Bus command pipe for the FM application.
 *
 *  \par Limits:
 *       FM requires that this name be defined, but otherwise places
 *       no limits on the definition.  Refer to CFE Software Bus Services
 *       for specific information on limits related to pipe names.
 */
#define FM_APP_PIPE_NAME                  FM_INTERNAL_CFGVAL(APP_PIPE_NAME)
#define DEFAULT_FM_INTERNAL_APP_PIPE_NAME "FM_CMD_PIPE"

/**
 * \brief File Manager Command Pipe Depth
 *
 *  \par Description:
 *       This definition sets the total number of packets that may queue
 *       in the FM command pipe.  The limit for individual message types
 *       in the queue is the default cFE Software Bus subscription
 *       limit.
 *
 *  \par Limits:
 *       It is recommended that this value be no less than 4 and
 *       no greater than 20 packets, but this is not enforced by FM.
 */
#define FM_APP_PIPE_DEPTH                  FM_INTERNAL_CFGVAL(APP_PIPE_DEPTH)
#define DEFAULT_FM_INTERNAL_APP_PIPE_DEPTH 10

/**
 * \brief Mission specific version number for FM application
 *
 *  \par Description:
 *       An application version number consists of four parts:
 *       major version number, minor version number, revision
 *       number and mission specific revision number. The mission
 *       specific revision number is defined here and the other
 *       parts are defined in "fm_version.h".
 *
 *  \par Limits:
 *       Must be defined as a numeric value that is greater than
 *       or equal to zero.
 */
#define FM_MISSION_REV                  FM_INTERNAL_CFGVAL(MISSION_REV)
#define DEFAULT_FM_INTERNAL_MISSION_REV 0xFF

/**
 * \brief Child file stat sleep
 *
 *  \par Description:
 *       OS_stat is a CPU intensive call. FM uses the OS_stat call to query a
 *       file’s size, date, and mode when setting up directory listings.
 *       Querying a large number of files and/or files large in size when
 *       processing directory listing commands can cause FM to hog the CPU. To
 *       mitigate this, options to sleep a configurable number of milliseconds
 *       between calls to OS_stat for a configurable number of files
 *       in a directory listing is provided. A large sleep cycle will not hang the CPU
 *       but it may take a long time for directory listing to complete. A shorter
 *       sleep cycle will speed up the directory listing commands but may cause
 *       FM to hog the CPU.
 *
 *       FM_CHILD_STAT_SLEEP_MS: The number of milliseconds to sleep each
 *       cycle. One cycle is FM_CHILD_STAT_SLEEP_FILECOUNT.
 *
 *       FM_CHILD_STAT_SLEEP_FILECOUNT: The number of files to process (OS_stat) before
 *       sleeping FM_CHILD_STAT_SLEEP_MS.
 *       Works in tandem with FM_CHILD_STAT_SLEEP_MS to reduce CPU hogging
 *       while allowing slightly more customization to balance time the operator is waiting to
 *       get data back from a directory listing versus FM hogging the CPU with calls to OS_stat
 *
 *       In short:
 *       High SLEEP_MS means less CPU hogging by FM but a longer time to process a dir listing command
 *       Low SLEEP_MS means more potential CPU hogging by FM but shorter time to process a dir listing command
 *       High FILECOUNT means more potential CPU hogging by FM but a shorter time to process a dir listing command
 *       Low FILECOUNT means less CPU hogging by FM but longer time to process a dir listing command
 *  \par Limits:
 *       The default is zero unless the mission needs require them to be changed.
 *
 */
#define FM_CHILD_STAT_SLEEP_MS                         FM_INTERNAL_CFGVAL(CHILD_STAT_SLEEP_MS)
#define DEFAULT_FM_INTERNAL_CHILD_STAT_SLEEP_MS        0
#define FM_CHILD_STAT_SLEEP_FILECOUNT                  FM_INTERNAL_CFGVAL(CHILD_STAT_SLEEP_FILECOUNT)
#define DEFAULT_FM_INTERNAL_CHILD_STAT_SLEEP_FILECOUNT 0

/**
 * \brief Child Task Command Queue Entry Count
 *
 *  \par Description:
 *       This definition sets the array depth for the command arguments queue in
 *       the FM main task to FM child task handshake interface.  The value sets
 *       the upper limit for the number of commands that can be waiting in the
 *       queue to be processed by the low priority FM child task.  A multi-entry
 *       command queue prevents the occasional slow command from being rejected
 *       because the child task has not yet completed the previous slow command.
 *
 *  \par Limits:
 *       The FM application limits this value to be no less than 1 and no greater
 *       than 10.  There must be at least one because this is the method for
 *       passing command arguments from the parent to the child task.  The upper
 *       limit is arbitrary.
 */
#define FM_CHILD_QUEUE_DEPTH                  FM_INTERNAL_CFGVAL(CHILD_QUEUE_DEPTH)
#define DEFAULT_FM_INTERNAL_CHILD_QUEUE_DEPTH 3

/**
 * \brief Child Task Name - cFE object name
 *
 *  \par Description:
 *       This definition sets the FM child task object name.  The task object
 *       name is required during child task creation by cFE Executive Services.
 *
 *  \par Limits:
 *       FM requires that this name be defined, but otherwise places
 *       no limits on the definition.  Refer to CFE Executive Services
 *       for specific information on limits related to object names.
 */
#define FM_CHILD_TASK_NAME                  FM_INTERNAL_CFGVAL(CHILD_TASK_NAME)
#define DEFAULT_FM_INTERNAL_CHILD_TASK_NAME "FM_CHILD_TASK"

/**
 * \brief Child Task Stack Size
 *
 *  \par Description:
 *       This definition sets the size in bytes of the FM child task
 *       stack.  It is highly recommended that this assignment be made
 *       by someone familiar with the system requirements for tasks
 *       running on the target platform.
 *
 *  \par Limits:
 *       The FM application limits this value to be no less than 2048
 *       and no greater than 20480.  These limits are purely arbitrary
 *       and may need to be modified for specific platforms.
 */
#define FM_CHILD_TASK_STACK_SIZE                  FM_INTERNAL_CFGVAL(CHILD_TASK_STACK_SIZE)
#define DEFAULT_FM_INTERNAL_CHILD_TASK_STACK_SIZE 20480

/**
 * \brief Child Task Execution Priority
 *
 *  \par Description:
 *       This parameter sets the execution priority for the FM child task.
 *       It is highly recommended that this assignment be made by someone
 *       familiar with the system requirements for tasks running on the
 *       target platform. Note: This parameter is VxWorks® specific. Not
 *       all operating systems set task priority this way.
 *
 *  \par Limits:
 *       Value to be no less than 1 and no greater than 255.
 *
 *  \par Priority Values:
 *       Note that a small value has higher priority than a large value.
 *       Thus, 100 is higher priority than 150. It is also necessary to
 *       ensure that a child task has lower priority than its parent.
 *       It should be clear that a child task that runs ahead of its
 *       parent defeats the purpose of having a child task to run in
 *       the background.
 */
#define FM_CHILD_TASK_PRIORITY                  FM_INTERNAL_CFGVAL(CHILD_TASK_PRIORITY)
#define DEFAULT_FM_INTERNAL_CHILD_TASK_PRIORITY 205

/**
 * \brief Child Task Semaphore Name - cFE object name
 *
 *  \par Description:
 *       This definition sets the FM child task semaphore object name.
 *       The semaphore object name is required during semaphore creation
 *       by cFE Executive Services.
 *
 *  \par Limits:
 *       FM requires that this name be defined, but otherwise places
 *       no limits on the definition.  Refer to CFE Executive Services
 *       for specific information on limits related to object names.
 */
#define FM_CHILD_SEM_NAME                  FM_INTERNAL_CFGVAL(CHILD_SEM_NAME)
#define DEFAULT_FM_INTERNAL_CHILD_SEM_NAME "FM_CHILD_SEM"

/**
 * \brief Child Task File I/O Control Settings
 *
 *  \par Description:
 *       These definitions control the amount of file data that the FM child task
 *       will process before giving up the CPU to allow other tasks time to run.
 *
 *       FM_CHILD_FILE_BLOCK_SIZE defines the size of each block of file data that
 *       the FM child task will read or write.  This value also defines the size
 *       of the FM child task I/O buffer that exists in global memory.
 *
 *       FM_CHILD_FILE_LOOP_COUNT defines the number of file data blocks that may
 *       be processed before the FM child task sleeps (gives up the CPU).
 *
 *       FM_CHILD_FILE_SLEEP_MS defines the length of time (in milli-secs) before
 *       the FM child task wakes (re-acquires the CPU).  Note that many platforms
 *       will limit the precision of this value.
 *
 *       Thus the combination of the 3 values control CPU use by the FM child task.
 *       Using a smaller block size minimizes the amount of RAM used by the file
 *       I/O buffer, but at the expense of file efficiency.  Adjust each of the
 *       values such that the combination is appropriate for the target platform.
 *
 *       For example, if the block size is 2048 and the loop count is 16 and the
 *       sleep time is 20, then while processing a 1 Mbyte file there will be
 *       32 sleep cycles of 20ms each, for a total task delay of 0.64 seconds.
 *
 *  \par Limits:
 *       FM_CHILD_FILE_BLOCK_SIZE: The FM application limits this value to be no
 *       less than 256 bytes and no greater than 32KB.
 *
 *       FM_CHILD_FILE_LOOP_COUNT: The FM application limits this value to be
 *       non-zero.  There is no upper limit - a very large number effectively
 *       means that the FM child task will not surrender the CPU to other lower
 *       priority tasks.
 *
 *       FM_CHILD_FILE_SLEEP_MS: The FM application limits this value to be no
 *       no greater than 100 ms.  The value zero generally means a very short
 *       task delay - refer to the target platform documentation for specifics.
 */
#define FM_CHILD_FILE_BLOCK_SIZE                  FM_INTERNAL_CFGVAL(CHILD_FILE_BLOCK_SIZE)
#define DEFAULT_FM_INTERNAL_CHILD_FILE_BLOCK_SIZE 2048
#define FM_CHILD_FILE_LOOP_COUNT                  FM_INTERNAL_CFGVAL(CHILD_FILE_LOOP_COUNT)
#define DEFAULT_FM_INTERNAL_CHILD_FILE_LOOP_COUNT 16
#define FM_CHILD_FILE_SLEEP_MS                    FM_INTERNAL_CFGVAL(CHILD_FILE_SLEEP_MS)
#define DEFAULT_FM_INTERNAL_CHILD_FILE_SLEEP_MS   20

/**
 *  \brief Wakeup for FM
 *
 *  \par Description
 *      Wakes up FM every 1 second for routine maintenance whether a
 *      message was received or not.
 */
#define FM_SB_TIMEOUT                  FM_INTERNAL_CFGVAL(SB_TIMEOUT)
#define DEFAULT_FM_INTERNAL_SB_TIMEOUT 1000

/**
 * \brief Monitor Table Name - cFE object name
 *
 *  \par Description:
 *       Table object name is required during table creation.
 *
 *  \par Limits:
 *       FM requires that this name be defined, but otherwise places
 *       no limits on the definition.  Refer to CFE Table Services
 *       for specific information on limits related to table names.
 */
#define FM_TABLE_CFE_NAME                  FM_INTERNAL_CFGVAL(TABLE_CFE_NAME)
#define DEFAULT_FM_INTERNAL_TABLE_CFE_NAME "Monitor"

/**
 * \brief Monitor Table Name - filename with path
 *
 *  \par Description:
 *       Table name with path is required to load table at startup.
 *
 *  \par Limits:
 *       FM requires that this name be defined, but otherwise places
 *       no limits on the definition.  If the named table does not
 *       exist or fails validation, the table load will fail.
 */
#define FM_TABLE_DEF_NAME                  FM_INTERNAL_CFGVAL(TABLE_DEF_NAME)
#define DEFAULT_FM_INTERNAL_TABLE_DEF_NAME "/cf/fm_monitor.tbl"

/**
 * \brief Monitor Table Name - filename without path
 *
 *  \par Description:
 *       Table name without path defines the output name for the table
 *       file created during the table make process.
 *
 *  \par Limits:
 *       FM requires that this name be defined, but otherwise places
 *       no limits on the definition.  If the table name is not
 *       valid then the make process may fail, or the table file may
 *       be unloadable to the target hardware.
 */
#define FM_TABLE_FILENAME                  FM_INTERNAL_CFGVAL(TABLE_FILENAME)
#define DEFAULT_FM_INTERNAL_TABLE_FILENAME "fm_monitor.tbl"

/**
 * \brief Free Space Table Description
 *
 *  \par Description:
 *       Table files contain headers that include descriptive text.
 *       This text will be put into the file header during the table
 *       make process.
 *
 *  \par Limits:
 *       FM requires that this name be defined, but otherwise places
 *       no limits on the definition.  Refer to cFE Table Services
 *       for limits related to table descriptive text.
 */
#define FM_TABLE_DEF_DESC                  FM_INTERNAL_CFGVAL(TABLE_DEF_DESC)
#define DEFAULT_FM_INTERNAL_TABLE_DEF_DESC "FM File System Free Space Table"

/**
 * \brief Default Directory List Output Filename
 *
 *  \par Description:
 *       This definition is the default output filename used by the Get
 *       Directory List to File command handler when the output filename
 *       is not provided.  The default filename is used whenever the
 *       commanded output filename is the empty string.
 *
 *  \par Limits:
 *       The FM application does not place a limit on this configuration
 *       parameter, however the symbol must be defined and the name will
 *       be subject to the same verification tests as a commanded output
 *       filename.  Set this parameter to the empty string if no default
 *       filename is desired.
 */
#define FM_DIR_LIST_FILE_DEFNAME                  FM_INTERNAL_CFGVAL(DIR_LIST_FILE_DEFNAME)
#define DEFAULT_FM_INTERNAL_DIR_LIST_FILE_DEFNAME "/ram/fm_dirlist.out"

/**\}*/

#endif // FM_INTERNAL_CFG_H