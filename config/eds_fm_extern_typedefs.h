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

#ifndef EDS_FM_EXTERN_TYPEDEFS_H
#define EDS_FM_EXTERN_TYPEDEFS_H

#include "common_types.h"
#include "fm_eds_typedefs.h"

#define FM_NAME_IS_INVALID     EdsLabel_FM_FileNameStates_IS_INVALID
#define FM_NAME_IS_NOT_IN_USE  EdsLabel_FM_FileNameStates_IS_NOT_IN_USE
#define FM_NAME_IS_FILE_OPEN   EdsLabel_FM_FileNameStates_IS_FILE_OPEN
#define FM_NAME_IS_FILE_CLOSED EdsLabel_FM_FileNameStates_IS_FILE_CLOSED
#define FM_NAME_IS_DIRECTORY   EdsLabel_FM_FileNameStates_IS_DIRECTORY

#define FM_FILE_CLOSED  EdsLabel_FM_FileStates_FILE_CLOSED
#define FM_FILE_EXISTS  EdsLabel_FM_FileStates_FILE_EXISTS
#define FM_FILE_NOEXIST EdsLabel_FM_FileStates_FILE_NOEXIST
#define FM_FILE_NOTOPEN EdsLabel_FM_FileStates_FILE_NOTOPEN
#define FM_DIR_EXISTS   EdsLabel_FM_FileStates_DIR_EXISTS
#define FM_DIR_NOEXIST  EdsLabel_FM_FileStates_DIR_NOEXIST

#define FM_TABLE_ENTRY_DISABLED EdsLabel_FM_TableEntryState_DISABLED
#define FM_TABLE_ENTRY_ENABLED  EdsLabel_FM_TableEntryState_ENABLED

#endif /* EDS_FM_EXTERN_TYPEDEFS_H */