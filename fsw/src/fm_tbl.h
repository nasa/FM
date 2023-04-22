/************************************************************************
 * NASA Docket No. GSC-18,918-1, and identified as “Core Flight
 * Software System (cFS) File Manager Application Version 2.6.1”
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
 *   Unit specification for the CFS File Manager table structures.
 */
#ifndef FM_TBL_H
#define FM_TBL_H

#include "cfe.h"
#include "fm_msg.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM table global function prototypes                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 *  \brief Table Initialization Function
 *
 *  \par Description
 *       This function is invoked during FM application startup initialization to
 *       create and initialize the FM file system free space table.  The purpose
 *       for the table is to define the list of file systems for which free space
 *       must be reported.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \return Execution status, see \ref CFEReturnCodes
 *  \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 *
 *  \sa /FM_AppInit
 */
CFE_Status_t FM_TableInit(void);

/**
 *  \brief Table Verification Function
 *
 *  \par Description
 *       This function is called from the CFE Table Services as part of the
 *       initial table load, and later inresponse to a Table Validate command.
 *       The function verifies that the table data is acceptable to populate the
 *       FM file system free space table.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  TablePtr - Pointer to table data for verification.
 *
 *  \return Validation status
 *  \retval #CFE_SUCCESS             \copydoc CFE_SUCCESS
 *  \retval #FM_TABLE_VALIDATION_ERR \copybrief FM_TABLE_VALIDATION_ERR
 *
 *  \sa /FM_AppInit
 */
CFE_Status_t FM_ValidateTable(FM_MonitorTable_t *TablePtr);

/**
 *  \brief Acquire Table Data Pointer Function
 *
 *  \par Description
 *       This function is invoked to acquire a pointer to the FM file system free
 *       space table data.  The pointer is maintained in the FM global data
 *       structure.  Note that the table data pointer will be set to NULL if the
 *       table has not yet been successfully loaded.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \sa #FM_GlobalData_t
 */
void FM_AcquireTablePointers(void);

/**
 *  \brief Release Table Data Pointer Function
 *
 *  \par Description
 *       This function is invoked to release the pointer to the FM file system free
 *       space table data.  The pointer is maintained in the FM global data
 *       structure.  The table data pointer must be periodically released to allow
 *       CFE Table Services an opportunity to load or dump the table without risk
 *       of interfering with users of the table data.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \sa #FM_GlobalData_t
 */
void FM_ReleaseTablePointers(void);

#endif
