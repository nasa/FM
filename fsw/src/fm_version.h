/*
** Filename: fm_version.h
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
** Title: File Manager (FM) Version Information Header File
**
** Purpose: Specification for the CFS FM application version label
**          definitions
**
** Notes:
**
** References:
**    Flight Software Branch C Coding Standard Version 1.0a
**    CFS Flight Software Development Standards Version 0.11
**
*/

#ifndef _fm_version_h_
#define _fm_version_h_

/*************************************************************************
**
** Macro definitions
**
**************************************************************************/

/*
**  Application Version Information:
**
**  Major.Minor.Revision.Mission_Rev
**
**  Major: Major update.  This would include major changes or new functionality.
**         Most likely will include database schema changes and interface changes.
**         Probably not backwards compatible with older versions
**
**	Minor: Minor change, may introduce new features, but backwards compatibility is mostly
**         retained.  Likely will include schema changes.
**
**  Revision: Minor bug fixes, no significant new features implemented, though a few small
**            improvements may be included.  May include a schema change.
**
**  Mission_Rev:  Used by users of the applications (nominally missions) to denote changes made
**                by the mission.  Releases from the Flight Softare Reuse Library (FSRL) should
**                use Mission_Rev zero (0).
**
*/
#define FM_MAJOR_VERSION     2
#define FM_MINOR_VERSION     5
#define FM_REVISION          3

#endif /* _fm_version_h_ */

/************************/
/*  End of File Comment */
/************************/

