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
 *   Specification for the CFS File Manager (FM) table structures
 *
 * @note
 *   Constants and enumerated types related to these table structures
 *   are defined in fm_tbldefs.h.
 */

#ifndef DEFAULT_FM_TBL_H
#define DEFAULT_FM_TBL_H

/* ======== */
/* Includes */
/* ======== */

#include "fm_tbldefs.h"
#include "fm_tblstruct.h"

/**
 * \brief Table Data Validation Error Code
 *
 *  \par Description:
 *       Table data is verified during the table load process.  Should
 *       the validation process fail, this value will be returned by
 *       FM to cFE Table Services and displayed in an event message.
 *
 *  \par Limits:
 *       FM requires that this value be defined, but otherwise places
 *       no limits on the definition.  Refer to cFE Table Services
 *       for limits related to error return values.
 */
#define FM_TABLE_VALIDATION_ERR (-1)

#endif /* DEFAULT_FM_TBL_H */