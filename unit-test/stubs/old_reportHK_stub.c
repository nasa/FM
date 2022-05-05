#include "cfe.h"
#include "fm_msg.h"
#include "fm_msgdefs.h"
#include "fm_msgids.h"
#include "fm_app.h"
#include "fm_tbl.h"
#include "fm_child.h"
#include "fm_cmds.h"
#include "fm_cmd_utils.h"
#include "fm_events.h"
#include "fm_perfids.h"
#include "fm_platform_cfg.h"
#include "fm_version.h"
#include "fm_verify.h"
#include "fm_test_utils.h"

#include <string.h>

#include "utstubs.h"
#include "uttools.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM application global data                                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

FM_GlobalData_t  FM_GlobalData;

void FM_ReportHK(const CFE_MSG_CommandHeader_t* Msg)
{
//	unimplemented(__func__,__FILE__,__LINE__);
	printf("Size of Msg is %lu and it should be %lu  \n",sizeof(Msg), sizeof(FM_HousekeepingCmd_t));
	const char *CmdText = "HK Request";
	bool Result = FM_IsValidCmdPktLength(&Msg->Msg, sizeof(FM_HousekeepingCmd_t),FM_HK_REQ_ERR_EID,CmdText);

	printf("The result is %u\n",Result);

	UT_Stub_RegisterContextGenericArg(UT_KEY(FM_ReportHK), Msg);

	int32 status;
	va_list va;
	
	status = UT_DEFAULT_IMPL_VARARGS(FM_ReportHK, va);
	if (status > 0){
	    UT_Stub_CopyFromLocal(UT_KEY(FM_ReportHK), Msg, sizeof(Msg));
    	}	
}
