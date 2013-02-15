/*
 *  telephony-emulator
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: 
 * Sooyoung Ha <yoosah.ha@samsung.com>
 * Sungmin Ha <sungmin82.ha@samsung.com>
 * YeongKyoon Lee <yeongkyoon.lee@samsung.com>
 * 
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Contributors:
 * - S-Core Co., Ltd
 * 
 */

/*
/////////////////////////////////////////////////////////////////////
// at_recv.c
 */

#include <assert.h>

#include "at_tx_func.h"
#include "at_recv.h"
#include "at_rx_data.h"
#include "at_rx_sms.h"
#include "at_rx_gprs.h"
#include "at_gen_resp.h"
#include "at_rx_call.h"
#include "at_rx_ss.h"
#include "at_rx_network.h"
#include "at_rx_display.h"
#include "at_rx_security.h"
#include "at_rx_phonebook.h"
#include "at_rx_snd.h"
#include "at_rx_sap.h"
#include "at_rx_sat.h"
#include "at_rx_misc.h"
#include "at_rx_gps.h"
#include "at_rx_func.h"
#include "at_func.h"
#include "server_tx_gprs.h"
#include "at_tx_call.h"

void NOT_SUPPORTED_AT(char* atmsg)
{
	TRACE(MSGL_WARN," THIS AT IS NOT SUPPROTED..\n");
}

void at_unimplemented(const char* api)
{
	TRACE(MSGL_WARN,"%s is unsupported\n",api);
}

#define unimplemented_case(X) case X: \
	TRACE(MSGL_WARN, #X " is unimplemented\n"); \
break;

int at_recv_misc(char* cmd, char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "\n");
	if( !strcmp( cmd, "+CGMR") )
	{
		return at_rx_misc_me_version(atmsg);
	}
	else if( !strcmp( cmd, "+CGSN") )
	{
		return at_rx_misc_me_serial(atmsg);
	}
	else if( !strcmp( cmd, "+CIMI") )
	{
		return at_rx_sim_sec_imsi(atmsg);
	}
	// return at_rx_misc_time_info_get(atmsg);

	NOT_SUPPORTED_AT(atmsg);
	return 1;
}

int at_recv_power(char* cmd, char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "cmd:%s\n", cmd);
	if ( !strcmp( cmd, "+CPAS" ))
	{
		return at_rx_power_up_exec(atmsg);
	}
	else if ( !strcmp( cmd, "!+CFUN=" ))
	{
		at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
		return at_rx_online_exec(atmsg);
	}
	NOT_SUPPORTED_AT(cmd);
	return 1;
}

int at_recv_call(char* cmd, char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "\n");
	if( !strcmp( cmd, "!D") )	// outgoing
	{
		return at_rx_call_originate_exec(atmsg);
	}
	else if( !strcmp( cmd, "A") )	// answer
	{
		at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
		at_rx_call_answer_exec(atmsg);
		return at_tx_call_connect_noti(atmsg, strlen(atmsg));
	}
	else if( !strcmp( cmd, "+CLCC") )	// get list
	{
		return at_rx_call_list_get(atmsg);
	}
	else if( !strcmp( cmd, "!H") )
	{	
		at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
                return at_rx_call_release_exec(atmsg);
	}
	// return at_rx_call_line_id_set(atmsg);
	// return at_rx_call_line_id_get(atmsg);
	// return at_rx_call_burst_dtmf_exec(atmsg);

	NOT_SUPPORTED_AT(atmsg);	
	return 1;
}

int at_recv_data(char* cmd, char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "\n");
	NOT_SUPPORTED_AT(atmsg);
	return 1;
}

int at_recv_sms(char* cmd, char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "%s\n", atmsg);
	if( !strcmp(cmd, "!+CMMS=") )
	{
		return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
	}
	else if( !strcmp(cmd, "!+CMGS=") )
	{
                return at_rx_sms_SendMsg(atmsg);
	}
	else if( !strcmp(cmd, "!+CPMS") )
	{
		return at_rx_sms_stored_msg_count_get(atmsg);
	}
	else if( !strcmp(cmd, "!+CNMA=") )	// deliver report
	{
                return at_rx_sms_deliver_report_exec(atmsg);
	}
	// return at_rx_sms_param_count_get(atmsg);
	// return at_rx_sms_param_set(atmsg);
	// return at_rx_sms_param_get(atmsg);
	// return at_rx_sms_svc_option_set(atmsg);
	// return at_rx_sms_mem_status_set(atmsg);
	// return at_rx_sms_cbconfig(atmsg);

	NOT_SUPPORTED_AT(atmsg);
	return 1;
}

int at_recv_security(char* cmd, char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "\n");
	char data[256];
        strcpy(data, atmsg);
        char* tdata = strchr(atmsg, '=');
	char token[] = "\"";
        char* ret = NULL;

	if( !strcmp(cmd, "!+CRSM=") )
	{
		return at_rx_sim_sec_rsim_access(atmsg);	
	}
	else if( !strcmp(cmd, "!+CLCK=") )
	{
		return at_rx_sim_sec_phone_lock_get(atmsg);
	}
	else if( !strcmp(cmd, "!%SCCT") )
	{
		return at_rx_sim_sec_card_type(atmsg);
	}
	else if( !strcmp(cmd, "!CPINR=") )
	{
		return at_rx_sim_sec_lock_info(atmsg);
	}
	else if( !strcmp(cmd, "!+CPIN=") )
	{
		ret = strtok(tdata+1, token);
                ret = strtok(NULL, token);
		ret = strtok(NULL, token);
	        if(ret)
        	{
			return at_rx_sim_sec_change_password(data);
		}
		else
			return at_rx_sim_sec_check_password(data);
	}
	// return at_rx_sim_sec_pin_status(atmsg);

	NOT_SUPPORTED_AT(atmsg);
	return 1;
}

int at_recv_phonebook(char* cmd, char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "\n");
	NOT_SUPPORTED_AT(atmsg);
	return 1;
}

int at_recv_display(char*cmd, char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "\n");
	// return at_rx_display_icon_info_get(atmsg);
	NOT_SUPPORTED_AT(atmsg);
	return 1;
}

int at_recv_network(char* cmd, char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "%s\n", cmd);
	if( !strcmp(cmd, "+CNUM") )
	{
		return at_rx_net_subscriber_num_get(atmsg);
	}
	else if( !strcmp(cmd, "+COPS?") )	// plmn sel or current plmn get or plmn selection get
	{
		return at_rx_net_current_plmn_get();
	}
	else if( !strcmp(cmd, "+COPS=?") )
	{
		return at_rx_net_plmn_list_get();
	}
	else if( !strcmp(cmd, "!+COPS=") )
	{
		return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
	}
	else if( !strcmp(cmd, "+CREG?") )
	{
		return at_rx_net_reg_get();
	}
	else if( !strcmp(cmd, "!+CREG=") )
	{
		return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
	}
	// return at_rx_net_band_get(atmsg);
	// return at_rx_net_band_set(atmsg);
	// return at_rx_net_service_domain_get(atmsg);
	NOT_SUPPORTED_AT(atmsg);
	return 1;
}

int at_recv_ss(char* cmd, char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "%s\n", atmsg);
	char data[256];
        strcpy(data, atmsg);
	char* tdata = strchr(atmsg, '=');
        char* ret = NULL;

	if( !strcmp(cmd, "!+CCWA=") )	// call waiting set
	{
		// mode check
                ret = strtok(tdata+1, TOKEN);
                ret = strtok(NULL, TOKEN);
                if(atoi(ret) == 2)              // get
                {
			return at_rx_ss_cw_get(data);
		}
		else				// set
		{
			at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
	                return at_rx_ss_cw_set(data);
		}
	}
	else if( !strcmp(cmd, "+CCWA=?") )	// call waiting list get
	{
		return at_rx_ss_cw_get(atmsg);
	}
	else if( !strcmp(cmd, "!+CHLD=") )	// call hold
	{
		at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
                return at_rx_ss_manage_call_set(atmsg);	
	}
	else if( !strcmp(cmd, "!+CCFC=") )
	{
		ret = strtok(tdata+1, TOKEN);
		ret = strtok(NULL, TOKEN);
		TRACE(MSGL_VGSM_INFO, "FW MODE:%s\n", ret);

		if(atoi(ret) == AT_CALL_FW_MODE_QUERY)		// query status
		{
			return at_rx_ss_cf_get(data);
		}
		else						// mode set
		{
			at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
                        return at_rx_ss_cf_set(data);
		}
	}
	else if( !strcmp(cmd, "!+CLCK=") )
	{
		char token[] = "\"";
                ret = strtok(tdata+1, token);
		TRACE(MSGL_VGSM_INFO, "%s\n", ret);
		
		if( !strcmp(ret, "PS") || !strcmp(ret, "SC") || !strcmp(ret, "FD") )	// for security
		{
			return at_recv_security(cmd, data);
		}
		else
		{
			ret = strtok(NULL, TOKEN);
			TRACE(MSGL_VGSM_INFO, "%s\n", ret);
                	if(atoi(ret) == 0 || atoi(ret) == 1)              // set
                	{
                       		return at_rx_ss_cb_set(data);
                	}
                	else if(atoi(ret) == 2)         // get
                	{
                       		return at_rx_ss_cb_get(data);
                	}
                	else            
                	{       
                        	NOT_SUPPORTED_AT(atmsg);
                        	return -1;
                	}
		}
	}

	NOT_SUPPORTED_AT(atmsg);
	return 1;
}

//GSM_GPRS_CMD GPRS(General Packet Radio Service)
int at_recv_gprs(char* cmd, char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "\n");
	if( !strcmp(cmd, "!+CGDCONT=") )
	{
		at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
		return at_rx_gprs_SetPDPContext(atmsg);
	}
	else if( !strcmp (cmd, "+CGDCONT?") )
	{
                server_tx_gprs_IPConfigurationNotify(0);
		return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
	}
	else if( !strcmp(cmd, "!+CGACT=") )
	{
		at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
		return at_rx_gprs_SetPSActDeact(atmsg);
	}
	else if( !strcmp(cmd, "!D*99***1#") )
	{
		at_gen_resp_send(AT_GEN_ERR_CONNECT);
		return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
	}
	
	return at_rx_gprs_DefaultGPRS(atmsg);
}

int at_recv_sat(char* cmd, char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "\n");
	NOT_SUPPORTED_AT(atmsg);
	return 1;
}

int at_recv_snd(char* cmd, char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "..\n");
	if( !strcmp(cmd, "!%SCMUT=") )		// set SND MIC MUTE CTRL
	{
		at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
                return at_rx_snd_mute_control_set(atmsg);
	}
	else if( !strcmp(cmd, "%SCMUT?") )	// get SND AUDIO PATH CTRL
	{
		return at_rx_snd_mute_control_get(atmsg);
	}
	else if( !strcmp(cmd, "!%SCAUD=") )	// set SND AUDIO PATH CTRL
	{
		at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
                return at_rx_snd_audio_path_set(atmsg);
	}
	else if( !strcmp(cmd, "!%SCCLK=") )	// exec SND CLOCK CTRL
	{
		at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
                return at_rx_snd_clock_ctrl_exec(atmsg);
	}

	NOT_SUPPORTED_AT(atmsg);
	return 1;
}

int at_recv_sap(char* cmd, char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "\n");
	NOT_SUPPORTED_AT(atmsg);
	return 1;
}

int at_recv_svc(char* cmd, char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "\n");
	NOT_SUPPORTED_AT(atmsg);
	return 1;
}

int at_recv_cfg(char* cmd, char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "\n");
	NOT_SUPPORTED_AT(atmsg);
	return 1;
}

int at_recv_gps(char* cmd, char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "\n");
	NOT_SUPPORTED_AT(atmsg);
	return 1;
}

