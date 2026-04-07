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

#ifndef DEFAULT_FM_EXTERN_TYPEDEFS_H
#define DEFAULT_FM_EXTERN_TYPEDEFS_H

/* ======== */
/* Includes */
/* ======== */

#include "cfe.h"

/**
 * \brief FM free space table entry state definitions
 */
#define FM_TABLE_ENTRY_DISABLED 0
#define FM_TABLE_ENTRY_ENABLED  1

/* ======== */
/* Typedefs */
/* ======== */

/**
 * \brief FM enum used for verifying file name states
 */
typedef enum
{
    FM_FileNameStates_INVALID     = 0,
    FM_FileNameStates_NOT_IN_USE  = 1,
    FM_FileNameStates_FILE_OPEN   = 2,
    FM_FileNameStates_FILE_CLOSED = 3,
    FM_FileNameStates_DIRECTORY   = 4,
} FM_FileNameStates_Enum_t;

/**
 *  \brief  FM enum used for verifying file states
 */
typedef enum
{
    FM_FileStates_CLOSED      = 0, /**< \brief FM File Is Closed */
    FM_FileStates_EXISTS      = 1, /**< \brief FM File Exists */
    FM_FileStates_NOEXIST     = 2, /**< \brief FM File Does Not Exist */
    FM_FileStates_NOTOPEN     = 3, /**< \brief FM File Is Not Open */
    FM_FileStates_DIR_EXISTS  = 4, /**< \brief FM Directory Exists */
    FM_FileStates_DIR_NOEXIST = 5, /**< \brief FM Directory Does Not Exist */
} FM_FileStates_Enum_t;

#endif // DEFAULT_FM_EXTERN_TYPEDEFS_H
