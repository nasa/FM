PROC $sc_$cpu_fm_tableloadfile
;*******************************************************************************
;  Name:  fm_tableloadfile
;
;  Test Description
;	This procedure populates the CFS File Manager free space table and 
;	generates a table load image for that table. This table image can be
;	loaded via the CFE_TBL_LOAD command.
;
;  Change History
;	Date	   Name		Description
;	12/10/08   W. Moleski	Original Procedure.
;	02/01/10   W. Moleski	Updated for FM 2.1.0.0
;       03/01/11   W. Moleski   Added variables for App name and ram directory
;       01/19/17   W. Moleski   Updated for FM 2.5.0.0 using CPU1 for commanding
;                               and added a hostCPU variable for the utility
;                               procs to connect to the proper host IP address.
;
;  Arguments
;	None.
;
;  Procedures Called
;	Name			Description
;**********************************************************************

local logging = %liv (log_procedure)
%liv (log_procedure) = FALSE

#include "ut_statusdefs.h"
#include "fm_platform_cfg.h"
#include "fm_events.h"
#include "fm_defs.h"

%liv (log_procedure) = logging

;**********************************************************************
; Define local variables
;**********************************************************************
local FMAppName = FM_APP_NAME
local ramDir = "/ram"
local hostCPU = "$CPU"
local freeSpaceTblName = FMAppName & "." & FM_TABLE_CFE_NAME

LOCAL tblAppId, tblPktId

;;; Set the pkt and app IDs for the tables based upon the cpu being used
;;; Right now, the pktIDs are not used
;; CPU1 is the default
tblAppId = "0FBA"
tblPktId = 4026

write ";*********************************************************************"
write ";  Create & upload the Free Space Table file."
write ";********************************************************************"
;; States are 0=Disabled; 1=Enabled;
$SC_$CPU_FM_FreeSpaceTBL[0].State = FM_TABLE_ENTRY_ENABLED
$SC_$CPU_FM_FreeSpaceTBL[0].Name = ramDir

$SC_$CPU_FM_FreeSpaceTBL[1].State = FM_TABLE_ENTRY_ENABLED
$SC_$CPU_FM_FreeSpaceTBL[1].Name = "/cf"

for i = 2 to FM_TABLE_ENTRY_COUNT-1 do
  $SC_$CPU_FM_FreeSpaceTBL[i].State = FM_TABLE_ENTRY_UNUSED
  $SC_$CPU_FM_FreeSpaceTBL[i].Name = ""
enddo

local maxIndex = FM_TABLE_ENTRY_COUNT-1
local endmnemonic = "$SC_$CPU_FM_FreeSpaceTBL[" & maxIndex & "]"

;; Create the Table Load file
s create_tbl_file_from_cvt (hostCPU,tblAppId,"FM FreeSpace Table Load 1","$cpu_fmdevtbl_ld_1",freeSpaceTblName,"$SC_$CPU_FM_FreeSpaceTBL[0].State",endmnemonic)

ENDPROC
