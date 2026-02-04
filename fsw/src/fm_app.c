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
 *  Core Flight System (CFS) File Manager (FM) Application
 *
 *  The File Manager (FM) Application provides onboard file system
 *  management services by processing commands for copying and moving
 *  files, decompressing files, concatenating files, creating directories,
 *  deleting files and directories, and providing file and directory status.
 *  When the File Manager application receives a housekeeping request
 *  (scheduled within the scheduler application), FM  reports it's housekeeping
 *  status values via telemetry messaging.
 */

#include "cfe.h"
#include "fm_msg.h"
#include "fm_msgdefs.h"
#include "fm_msgids.h"
#include "fm_app.h"
#include "fm_table_utils.h"
#include "fm_child.h"
#include "fm_cmds.h"
#include "fm_cmd_utils.h"
#include "fm_dispatch.h"
#include "fm_eventids.h"
#include "fm_perfids.h"
#include "fm_platform_cfg.h"
#include "fm_version.h"
#include "fm_verify.h"
#include "fm_compression.h"

#include <string.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM application global data                                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

FM_AppData_t FM_AppData;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM application -- entry point and main loop processor           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void FM_AppMain(void)
{
    uint32           RunStatus = CFE_ES_RunStatus_APP_RUN;
    CFE_SB_Buffer_t *BufPtr    = NULL;
    CFE_Status_t     Result    = CFE_SUCCESS;

    /* Performance Log (start time counter) */
    CFE_ES_PerfLogEntry(FM_APPMAIN_PERF_ID);

    /*
    ** Perform application specific initialization...
    */
    Result = FM_AppInit();

    /*
    ** Check for start-up error...
    */
    if (Result != CFE_SUCCESS)
    {
        /*
        ** Set request to terminate main loop...
        */
        RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    /*
    ** Main process loop...
    */
    while (CFE_ES_RunLoop(&RunStatus) == true)
    {
        /* Performance Log (stop time counter) */
        CFE_ES_PerfLogExit(FM_APPMAIN_PERF_ID);

        /* Wait for the next Software Bus message */
        Result = CFE_SB_ReceiveBuffer(&BufPtr, FM_AppData.CmdPipe, FM_SB_TIMEOUT);

        /* Performance Log (start time counter) */
        CFE_ES_PerfLogEntry(FM_APPMAIN_PERF_ID);

        if (Result == CFE_SUCCESS)
        {
            if (BufPtr != NULL)
            {
                /* Process Software Bus message */
                FM_TaskPipe(BufPtr);
            }
            else
            {
                /* Software Bus thought it succeeded but provided a bad pointer */
                CFE_EVS_SendEvent(FM_SB_RECEIVE_NULL_PTR_ERR_EID,
                                  CFE_EVS_EventType_ERROR,
                                  "Main loop error: SB returned NULL pointer on success");

                /* Set request to terminate main loop */
                RunStatus = CFE_ES_RunStatus_APP_ERROR;
            }
        }
        else if (Result != CFE_SB_TIME_OUT && Result != CFE_SB_NO_MESSAGE)
        {
            /* Process Software Bus error */
            CFE_EVS_SendEvent(FM_SB_RECEIVE_ERR_EID,
                              CFE_EVS_EventType_ERROR,
                              "Main loop error: SB receive: result = 0x%08X",
                              (unsigned int)Result);

            /* Set request to terminate main loop */
            RunStatus = CFE_ES_RunStatus_APP_ERROR;
        }
    }

    /*
    ** Send an event describing the reason for the termination...
    */
    CFE_EVS_SendEvent(FM_EXIT_ERR_EID,
                      CFE_EVS_EventType_ERROR,
                      "Application terminating: result = 0x%08X",
                      (unsigned int)Result);

    /*
    ** In case cFE Event Services is not working...
    */
    CFE_ES_WriteToSysLog("FM application terminating: result = 0x%08X\n", (unsigned int)Result);

    /*
    ** Performance Log (stop time counter)...
    */
    CFE_ES_PerfLogExit(FM_APPMAIN_PERF_ID);

    /*
    ** Let cFE kill the task (and any child tasks)...
    */
    CFE_ES_ExitApp(RunStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM application -- startup initialization processor              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CFE_Status_t FM_AppInit(void)
{
    const char  *ErrText = "Initialization error:";
    CFE_Status_t Result  = CFE_SUCCESS;

    /* Initialize global data  */
    memset(&FM_AppData, 0, sizeof(FM_AppData));

    /* Initialize housekeeping telemetry message */
    CFE_MSG_Init(CFE_MSG_PTR(FM_AppData.HkTlm.TelemetryHeader), CFE_SB_ValueToMsgId(FM_HK_TLM_MID), sizeof(FM_HkTlm_t));

    /* Register for event services */
    Result = CFE_EVS_Register(NULL, 0, CFE_EVS_EventFilter_BINARY);

    if (Result != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("FM App: Error registering for Event Services, RC = 0x%08X\n", (unsigned int)Result);
    }
    else
    {
        /* Create Software Bus message pipe */
        Result = CFE_SB_CreatePipe(&FM_AppData.CmdPipe, FM_APP_PIPE_DEPTH, FM_APP_PIPE_NAME);
        if (Result != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(FM_CR_PIPE_ERR_EID,
                              CFE_EVS_EventType_ERROR,
                              "%s create SB input pipe: result = 0x%08X",
                              ErrText,
                              (unsigned int)Result);
        }
        else
        {
            /* Subscribe to Housekeeping request commands */
            Result = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(FM_SEND_HK_MID), FM_AppData.CmdPipe);

            if (Result != CFE_SUCCESS)
            {
                CFE_EVS_SendEvent(FM_STARTUP_SUBSCRIB_HK_ERR_EID,
                                  CFE_EVS_EventType_ERROR,
                                  "%s subscribe to HK request: result = 0x%08X",
                                  ErrText,
                                  (unsigned int)Result);
            }
        }
    }

    /* Keep indentation from getting too deep */
    if (Result == CFE_SUCCESS)
    {
        /* Subscribe to FM ground command packets */
        Result = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(FM_CMD_MID), FM_AppData.CmdPipe);

        if (Result != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(FM_STARTUP_SUBSCRIB_GCMD_ERR_EID,
                              CFE_EVS_EventType_ERROR,
                              "%s subscribe to FM commands: result = 0x%08X",
                              ErrText,
                              (unsigned int)Result);
        }
        else
        {
            /* Initialize FM tables */
            Result = FM_TableInit();

            if (Result != CFE_SUCCESS)
            {
                CFE_EVS_SendEvent(FM_STARTUP_TABLE_INIT_ERR_EID,
                                  CFE_EVS_EventType_ERROR,
                                  "%s register free space table: result = 0x%08X",
                                  ErrText,
                                  (unsigned int)Result);
            }
            else
            {
                /* Create low priority child task */
                FM_ChildInit();

                /* Application startup event message */
                CFE_EVS_SendEvent(FM_INIT_INF_EID,
                                  CFE_EVS_EventType_INFORMATION,
                                  "Initialization complete: version %d.%d.%d.%d",
                                  FM_MAJOR_VERSION,
                                  FM_MINOR_VERSION,
                                  FM_REVISION,
                                  FM_MISSION_REV);
            }
        }
    }

    FM_CompressionService_Init();

    return Result;
}
