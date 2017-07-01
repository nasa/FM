proc $sc_$cpu_fm_dirtlmdisplay (directoryName, offsetGiven, expResult, statFlag)
;==============================================================================
;
; Purpose: The purpose of this procedure is to perform the Dir Listing to Tlm
;          cmd and then write the contents of the packet to the log.
;
; Waring:  It is advised that before running a fresh procedure that calls this
;          to initialize the telemetry points P08Cscnt, P18Cscnt, & P28Cscnt 
;          to 0 so that a previous proc run does not interfere with whether
;          the packet is viewed as stale or not 
;
; History:
;
;	Date	  Name		Description
;	08/05/08  D. Stewart	Initial development of this proc.
; 	09/22/09  W. Moleski	Updated to make CPU1 the default.
;	01/13/10  W. Moleski	Updated to work with FM 2.1.0.0
;       01/18/17  W. Moleski    Updated for FM 2.5.0.0 using CPU1 for commanding
;                               and added new GetStatFlag argument to command.
;==============================================================================

#include "fm_platform_cfg.h"

;; If the optional argument is not specified, expect Pass
if (%nargs = 2) then
  expResult = "Pass"
  statFlag = 0
elseif (%nargs = 3) then
  statFlag = 0
endif

local cmdctr
local errcnt
local seqCount
local writeToLog = 1
local currentDis
local numToDisplay
local maxFilesToDisplay = FM_DIR_LIST_PKT_ENTRIES
local actualResult

;****************************************
;  start of sub-proc
;****************************************
; grab the current sequence counter for the packet
; CPU1 is the default
seqCount = P08Cscnt

write "  Packet Sequence Counter before cmd = ", seqCount

; issue the command
cmdctr = $SC_$CPU_FM_CMDPC + 1
errcnt = $SC_$CPU_FM_CMDEC + 1

/$SC_$CPU_FM_DirListTlm DirName=directoryName Offset=offsetGiven GetStatFlag=statFlag

wait until (($SC_$CPU_FM_CMDPC = cmdctr) OR ($SC_$CPU_FM_CMDEC = errcnt))   
if ($SC_$CPU_FM_CMDPC = cmdctr) then
  actualResult = "Pass"
  if (expResult = "Pass") then
    write "<*> Passed - Dir Listing to Tlm command accepted."
  else
    write "<!> Failed - Dir Listing to Tlm command rejected when success was expected."
  endif
else
  actualResult = "Fail"
  if (expResult = "Fail") then
    write "<*> Passed - Dir Listing to Tlm command rejected."
  else
    write "<!> Failed - Dir Listing to Tlm command accepted when failure was expected."
  endif
endif

wait 5

if (actualResult = "Pass") then
  ;; Write contents of the packet to the log
  write ""

  local newSeqCount = P08Cscnt

  if (seqCount = newSeqCount) then
    write "<!> WARNING - Info Packet not received"
    writeToLog = 0
  else
    write "<*> New Info Packet received"
  endif

  write ""
  write "Sequence Counter = ",newSeqCount 

  if (writeToLog = 1) then
    if ($SC_$CPU_FM_DirOffset > $SC_$CPU_FM_TotalFiles) then
      numToDisplay = 0
    elseif (($SC_$CPU_FM_TotalFiles - $SC_$CPU_FM_DirOffset) < maxFilesToDisplay) then
      numToDisplay = ($SC_$CPU_FM_TotalFiles - $SC_$CPU_FM_DirOffset)
    else
      numToDisplay = maxFilesToDisplay
    endif
    write ""
    write "Contents of the Directory Listing to Telemetry Packet:"
    write ""
    write " Directory Stats:"
    write "  Directory Name        = ", $SC_$CPU_FM_DirName
    write "  Directory Offset      = ", $SC_$CPU_FM_DirOffset
    write "  Total Files in Dir    = ", $SC_$CPU_FM_TotalFiles
    write "  Files in Packet       = ", $SC_$CPU_FM_PktFiles
    FOR currentDis = 1 to numToDisplay DO
      if (currentDis = 1) then
        write ""
        write " File Listing: "
      endif
      write "  File " & currentDis & " Name          = ",$SC_$CPU_FM_DirList[currentDis].name
      write "  File " & currentDis & " Size          = ",$SC_$CPU_FM_DirList[currentDis].FileSize
      write "  File " & currentDis & " Last Mod Time = ",$SC_$CPU_FM_DirList[currentDis].LastModTime
      write "  File " & currentDis & " Permissions   = ",$SC_$CPU_FM_DirList[currentDis].FilePerms
    ENDDO
    write ""
  endif
endif

ENDPROC
