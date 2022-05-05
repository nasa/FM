PROC scx_cpu1_fm_badtblloadfile
;*******************************************************************************
;  Name:  fm_badtblloadfile
;
;  Test Description
;	This procedure populates the CFS File Manager free space table and 
;	generates a table load image for that table. This table image containd
;	all possible validation errors. This image can be loaded via the
;	CFE_TBL_LOAD command.
;
;  Change History
;
;	Date	   Name		Description
;	05/11/10   W. Moleski	Initial creation
;       03/03/11   W. Moleski   Added variables for App name and ram directory
;       01/18/17   W. Moleski   Added variable to specify the Host being used
;
;  Arguments
;	None.
;
;  Procedures Called
;	Name			Description
;
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
local freeSpaceTblName = FMAppName & "." & FM_TABLE_CFE_NAME
local hostCPU = "CPU3"

LOCAL tblAppId, tblPktId

;;; Set the pkt and app IDs for the tables based upon the cpu being used
;;; Right now, the pktIDs are not used
;; CPU1 is the default
tblAppId = "0FBA"
tblPktId = 4026

write ";*********************************************************************"
write ";  Create & upload the Free Space Table file."
write ";********************************************************************"
;; States are 0=Unused; 1=Enabled; 2=Disabled
SCX_CPU1_FM_FreeSpaceTBL[0].State =FM_TABLE_ENTRY_ENABLED 
SCX_CPU1_FM_FreeSpaceTBL[0].Name = "/ram"

SCX_CPU1_FM_FreeSpaceTBL[1].State = FM_TABLE_ENTRY_ENABLED
SCX_CPU1_FM_FreeSpaceTBL[1].Name = "/cf"

SCX_CPU1_FM_FreeSpaceTBL[2].State = FM_TABLE_ENTRY_DISABLED
SCX_CPU1_FM_FreeSpaceTBL[2].Name = "/cf/apps-rc"

SCX_CPU1_FM_FreeSpaceTBL[3].State = FM_TABLE_ENTRY_DISABLED
SCX_CPU1_FM_FreeSpaceTBL[3].Name = ""

SCX_CPU1_FM_FreeSpaceTBL[4].State = 3
SCX_CPU1_FM_FreeSpaceTBL[4].Name = ""

for i = 5 to FM_TABLE_ENTRY_COUNT-1 do
  SCX_CPU1_FM_FreeSpaceTBL[i].State = FM_TABLE_ENTRY_UNUSED
  SCX_CPU1_FM_FreeSpaceTBL[i].Name = ""
enddo

local maxIndex = FM_TABLE_ENTRY_COUNT-1
local endmnemonic = "SCX_CPU1_FM_FreeSpaceTBL[" & maxIndex & "]"

;; Create the Table Load file
s create_tbl_file_from_cvt (hostCPU,tblAppId,"FM FreeSpace Table Invalid Load 1","cpu1_fmbadtbl_ld_1",freeSpaceTblName,"SCX_CPU1_FM_FreeSpaceTBL[0].State",endmnemonic)

ENDPROC
