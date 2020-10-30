#include "StdAfx.h"
#include "wt208_vsa.h"
#include <iostream>
#include "common.h"
#include <assert.h>
#include <Windows.h>
#include "MP819xVCDlg.h"
//#include "Mutex.h"
//#include "bt_demo.h"

static volatile unsigned int g_print_rc = 0;
extern CMP819xVCDlg *gDlg;
static      CRITICAL_SECTION     cs;
void print_debug_info(const char *buf)
{

//	ENTER_LOCK(g_print_rc);
	//myprintf("%s", buf);

    memcpy(vsaThData.buf, buf,2048);
	::PostMessage((HWND) gDlg->m_hWnd, WM_UPDATECAPDATA, (WPARAM) 0, (LPARAM) 0 );//数据处理完毕，通知主线程
// 	if(gDlg->m_hWnd!= NULL){
// 		gDlg->m_redit1.SetWindowTextW(_T(""));
// 	
// 		gDlg->m_redit1.SetSel(-1,-1);
// 		gDlg->m_redit1.ReplaceSel(buf1);
// 	}
//	EXIT_LOCK(g_print_rc);
}

wt208_vsa::wt208_vsa()
	: m_handle_VSG(INVALID_HANDLE_VALUE)
	, m_handle_VSA(INVALID_HANDLE_VALUE)
	, m_ThreadId(NULL)
	, m_external_gain(0.0)
	, m_dut_peek_power(-8.0)
	, m_freq(2412 * 1e6)
	, m_repeat(10000)
	, m_vsg_runing(false)
	//, m_connect_id(-1)
	, m_port(-1)
	, m_bTerminate(false)
	, vsa_loop(0)
{
	memset(&m_VsaParameter, 0, sizeof(VsaParameter));
	memset(&m_VsgParameter, 0, sizeof(VsgParameter));
}

void wt208_vsa::destory()
{
	::PostThreadMessage(m_ThreadId, WM_STOPTHREAD, 0, 0);
	Sleep(2);
	::PostThreadMessage(m_ThreadId, WM_QUIT, 0, 0);
	if(m_handle_VSA != INVALID_HANDLE_VALUE)
	{
		WaitForSingleObject(m_handle_VSA, INFINITE);
		CloseHandle(m_handle_VSA);
	}
	if(m_handle_VSG != INVALID_HANDLE_VALUE)
	{
		WaitForSingleObject(m_handle_VSG, INFINITE);
		CloseHandle(m_handle_VSG);
	}
	::DeleteCriticalSection(&cs);
	if(m_connect_id != -1)
	{
		WT_DisConnect(m_connect_id);
		printf("[%d] disconnect tester\n", m_connect_id);
	}
}

wt208_vsa::~wt208_vsa(void)
{
	destory();
}

void wt208_vsa::set_vsa_port(int port)
{
	m_port = WT_PORT_OFF + port;
}

bool wt208_vsa::init()
{
	bool ret;
	::InitializeCriticalSection(&cs);
	m_handle_VSA = CreateThread(NULL, 0, ThreadVSAFun, this, 0, &m_ThreadId);
	return (m_handle_VSA != INVALID_HANDLE_VALUE) ;
	//return true;
}
bool wt208_vsa::initParam(int connect_id)
{
	int ret = WT_ERR_CODE_OK;
	m_connect_id = connect_id;
	// 	if(false == demo_Multi_connect_tester(&m_connect_id))
	// 	{
	// 		return false;
	// 	}
	if(m_connect_id == -1) return false;
	ret = WT_GetDefaultParameter(&m_VsaParameter, &m_VsgParameter);
	assert(WT_ERR_CODE_OK == ret);

	ret = WT_SetExternalGain(m_connect_id, m_external_gain);
	assert(WT_ERR_CODE_OK == ret);
	myprintf("[%d]set tester external cable loss %lf dB\n",m_connect_id,m_external_gain);
	return WT_ERR_CODE_OK == ret;
}
int wt208_vsa::set_vsa_Autorange()
{
	int ret = 0;
	int rf_port = m_port;
	if(rf_port <= 0)
	{
		rf_port = get_rf_vsa_config();
	}

	ret = WT_SetExternalGain(m_connect_id, m_external_gain);
	// vsaParam max_power = target dut power + (14~18dB)
	// if not sure DUT Max power input ,we should run WT_SetVSA_AutoRange to get vsaParam max_power
	// here we don't know DUT Max power input, so just set max_power=0
#if 0
	m_VsaParameter.max_power = 0.0;
	
		//freq
		m_VsaParameter.freq = m_freq;
		//WIFI standard
		m_VsaParameter.demod = WT_DEMOD_11AG;
		// rf port
		m_VsaParameter.rfPort = WT_PORT_OFF + rf_port;
		//sample time
		m_VsaParameter.smp_time = 2000;
		//timeout waiting
		m_VsaParameter.TimeoutWaiting = 8;
		//trigger pretime
		m_VsaParameter.trig_pretime = 20;
		//trigger level
		m_VsaParameter.trig_level = -28.0;
		//trigger type
		m_VsaParameter.trig_type = WT_TRIG_TYPE_IF;
		//trigger timeout.
		//if trig_timeout > TimeoutWaiting maybe cause WT_SetVSA_AutoRange return WT_ERR_CODE_TIMEOUT
		m_VsaParameter.trig_timeout = 2;
		//gDlg->m_pageVSA.GetVsaParameter(&m_VsaParameter);
#else
	m_VsaParameter.max_power = 0.0;
	
	//freq
	m_VsaParameter.freq = vsaThData.vsaParam.freq;
	//WIFI standard
	m_VsaParameter.demod = vsaThData.vsaParam.demod;
	// rf port
	m_VsaParameter.rfPort =  rf_port;
	//sample time
	m_VsaParameter.smp_time = vsaThData.vsaParam.smp_time;
	//timeout waiting
	m_VsaParameter.TimeoutWaiting = 8;
	//trigger pretime
	m_VsaParameter.trig_pretime = 20;
	//trigger level
	m_VsaParameter.trig_level = vsaThData.vsaParam.trig_level;
	//trigger type
	m_VsaParameter.trig_type = vsaThData.vsaParam.trig_type;
	//trigger timeout.
	//if trig_timeout > TimeoutWaiting maybe cause WT_SetVSA_AutoRange return WT_ERR_CODE_TIMEOUT
	m_VsaParameter.trig_timeout = vsaThData.vsaParam.trig_timeout;
#endif
	ret = WT_SetVSA_AutoRange(m_connect_id, &m_VsaParameter);

	char buf[256] = {0};
	sprintf(buf, "[%d]WT_SetVSA_AutoRange RF_Port:%d %s\n", m_connect_id, rf_port, (ret == WT_ERR_CODE_OK ? "OK" : "FAIL"));
	m_print_string += string(buf);

	return ret;
}

bool wt208_vsa::data_capture()
{
	int ret = 0;
	
	char buf[2048] = {0};
	
	if(vsa_loop++ > 65535) vsa_loop = 0;
	//while(vsa_loop++ < 400 && !m_bTerminate)
	//{
		sprintf(buf, "========Connect[%d] loop %d=============\n", m_connect_id, vsa_loop);
		m_print_string += string(buf);

		memset(buf, 0, sizeof(buf));
 		ret = set_vsa_Autorange();
 		if(WT_ERR_CODE_OK == ret)
 		{
			memset(buf, 0, sizeof(buf));
			ret = WT_DataCapture(m_connect_id);
			sprintf(buf, "[%d]WT_DataCapture %s [ret: %d]\n", m_connect_id, (WT_ERR_CODE_OK == ret ? "OK" : "FAIL"), ret);
			m_print_string += string(buf);

			if(WT_ERR_CODE_OK == ret)
			{
				memset(buf, 0, sizeof(buf));
				print_power_2(m_connect_id, buf);
				m_print_string += string(buf);

				memset(buf, 0, sizeof(buf));
				print_evm_2(m_connect_id, buf);
				m_print_string += string(buf);

				memset(buf, 0, sizeof(buf));
				print_freq_err_2(m_connect_id, buf);
				m_print_string += string(buf);
				memset(buf, 0, sizeof(buf));
				print_datarate_2(m_connect_id, buf);
				m_print_string += string(buf);
			}else {
				memset(buf, 0, sizeof(buf));
				print_power_3(buf);
				m_print_string += string(buf);

				memset(buf, 0, sizeof(buf));
				print_evm_3(buf);
				m_print_string += string(buf);

				memset(buf, 0, sizeof(buf));
				print_freq_err_3( buf);
				m_print_string += string(buf);
			}
		}
		print_debug_info(m_print_string.c_str());
		m_print_string.clear();
		memset(buf, 0, sizeof(buf));

		Sleep(100);
	//}
	//ret = WT_StopDataCapture(m_connect_id);
	return WT_ERR_CODE_OK == ret;
}
CEvent stopCapEvent;

DWORD WINAPI wt208_vsa::ThreadVSAFun(LPVOID pM)
{
	wt208_vsa *pThis = (wt208_vsa *)(LPVOID)pM;
	bool b_run_or_stop = false;//是否启动数据处理
	int ret;
	MSG msg;//线程的消息

	while(1){
		if(b_run_or_stop)//如果启动数据处理
		{
			EnterCriticalSection(&cs);//进入临界区
			pThis->data_capture();
			LeaveCriticalSection(&cs);//离开临界区

		}
		if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{//检查线程的消息队列，是否有下面的自定义消息
			switch(msg.message)
			{
			case WM_QUIT://退出从线程
				ExitThread(0);
			case WM_STARTTHREAD:
				//启动数据处理,从线程从消息中提取其将要处理的数据
				b_run_or_stop = true;
				break;
			case WM_STOPTHREAD://暂停数据处理
				b_run_or_stop = false;
				stopCapEvent.SetEvent();
				break;
			case  WM_AUTORANGE:
				ret = pThis->set_vsa_Autorange();
				if(WT_ERR_CODE_OK == ret)
				{

				}
				break;
			default:
				break;

			}

		}
	}
	return 1;
}

bool wt208_vsa::terminate()
{
	m_bTerminate = true;
	return true;
}

bool wt208_vsa::exec()
{
	if(init())
	{
		//m_handle_VSA = CreateThread(NULL, 0, ThreadVSAFun, this, 0, &m_ThreadId);

		return true;
	}
	return false;
}

void wt208_vsa::setAutorange()
{
	::PostThreadMessage(m_ThreadId, WM_AUTORANGE,  (WPARAM) 0,  0);
}

void wt208_vsa::startCapture()
{
	if(m_ThreadId == NULL) return;
	::PostThreadMessage(m_ThreadId, WM_STARTTHREAD,  (WPARAM) 0,  0);
	int x = GetLastError();
}

void wt208_vsa::stopCapture()
{
	if(m_ThreadId == NULL) return;
	::PostThreadMessage(m_ThreadId, WM_STOPTHREAD,  (WPARAM) 0,  0);
	DWORD dwRet = 0;  
	MSG msg;  
	while (TRUE)  
	{  
		//wait for m_hThread to be over，and wait for  
		//QS_ALLINPUT（Any message is in the queue）  
		dwRet = MsgWaitForMultipleObjects (1, &stopCapEvent.m_hObject,   FALSE, INFINITE, QS_ALLINPUT);  
		switch(dwRet)  
		{  
		case WAIT_OBJECT_0:   
			break; //break the loop  
		case WAIT_OBJECT_0 + 1:  
			//get the message from Queue  
			//and dispatch it to specific window  
			PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);  
			DispatchMessage(&msg);   
			continue;  
		default:  
			break; // unexpected failure  
		}  
		break;  
	}  
	stopCapEvent.ResetEvent();
}
