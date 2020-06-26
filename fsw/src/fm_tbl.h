/*
** Filename: fm_tbl.h 
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
** Title: File Manager (FM) Table Definitions
**
** Purpose: Unit specification for the CFS File Manager table structures.
**
** Notes:
**
** References:
**    Flight Software Branch C Coding Standard Version 1.0a
**
**
*/

#ifndef _fm_tbl_h_
#define _fm_tbl_h_

#include "cfe.h"
#include "fm_msg.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM table global function prototypes                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
**  \brief Table Initialization Function
**
**  \par Description
**       This function is invoked during FM application startup initialization to
**       create and initialize the FM file system free space table.  The purpose
**       for the table is to define the list of file systems for which free space
**       must be reported.
**
**  \par Assumptions, External Events, and Notes:
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS  \endcode
**  \retstmt Error return codes from #CFE_TBL_Register    \endcode
**  \endreturns
**
**  \sa /FM_AppInit
**/
int32 FM_TableInit(void);


/**
**  \brief Table Verification Function
**
**  \par Description
**       This function is called from the CFE Table Services as part of the
**       initial table load, and later inresponse to a Table Validate command.
**       The function verifies that the table data is acceptable to populate the
**       FM file system free space table.
**
**  \par Assumptions, External Events, and Notes:
**
**  \param [in]  TableData - Pointer to table data for verification.
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt #FM_TABLE_VALIDATION_ERR if table validation fails \endcode
**  \endreturns
**
**  \sa /FM_AppInit
**/
int32 FM_ValidateTable(FM_FreeSpaceTable_t *TableData);


/**
**  \brief Acquire Table Data Pointer Function
**
**  \par Description
**       This function is invoked to acquire a pointer to the FM file system free
**       space table data.  The pointer is maintained in the FM global data
**       structure.  Note that the table data pointer will be set to NULL if the
**       table has not yet been successfully loaded.
**
**  \par Assumptions, External Events, and Notes:
**
**  \returns
**  \retcode (none) \endcode
**  \endreturns
**
**  \sa #FM_GlobalData_t
**/
void  FM_AcquireTablePointers(void);


/**
**  \brief Release Table Data Pointer Function
**
**  \par Description
**       This function is invoked to release the pointer to the FM file system free
**       space table data.  The pointer is maintained in the FM global data
**       structure.  The table data pointer must be periodically released to allow
**       CFE Table Services an opportunity to load or dump the table without risk
**       of interfering with users of the table data.
**
**  \par Assumptions, External Events, and Notes:
**
**  \returns
**  \retcode (none) \endcode
**  \endreturns
**
**  \sa #FM_GlobalData_t
**/
void  FM_ReleaseTablePointers(void);


#endif /* _fm_tbl_h_ */

/************************/
/*  End of File Comment */
/************************/
