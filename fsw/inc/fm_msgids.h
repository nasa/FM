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
 *   Specification for the CFS FM application software bus
 *   message identifiers
 */
#ifndef FM_MSGIDS_H
#define FM_MSGIDS_H

/**
 * \defgroup cfsfmcmdmid CFS File Manager Command Message IDs
 * \{
 */

#define FM_CMD_MID     0x188C /** < \brief FM ground commands */
#define FM_SEND_HK_MID 0x188D /** < \brief FM send housekeeping */

/**\}*/

/**
 * \defgroup cfsfmtlmmid CFS File Manager Telemetry Message IDs
 * \{
 */

#define FM_HK_TLM_MID         0x088A /** < \brief FM housekeeping */
#define FM_FILE_INFO_TLM_MID  0x088B /** < \brief FM get file info */
#define FM_DIR_LIST_TLM_MID   0x088C /** < \brief FM get dir list */
#define FM_OPEN_FILES_TLM_MID 0x088D /** < \brief FM get open files */
#define FM_FREE_SPACE_TLM_MID 0x088E /** < \brief FM get free space */

/**\}*/

#endif
