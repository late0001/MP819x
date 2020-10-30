#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <tester.h>
#include <testerCommon.h>
#include <Windows.h>

using namespace std;

class wt208_vsa
{
public:
	wt208_vsa();
	~wt208_vsa(void);
	bool init();
	void destory();
	bool initParam(int connect_id);
	void set_vsa_port(int port);
	bool exec();
	void setAutorange();
	void startCapture();
	void stopCapture();
	bool terminate();
protected:
	
	bool data_capture();
	int set_vsa_Autorange();
	static DWORD WINAPI ThreadVSAFun(LPVOID pM);
public:
	
	DWORD m_ThreadId;
private:
	VsaParameter m_VsaParameter;
	VsgParameter m_VsgParameter;
	int m_connect_id;
	double m_external_gain;
	double m_freq;
	int m_repeat;
	bool is_wifi_vsg;
	int m_port;
	string m_print_string;
	bool m_bTerminate;
	double m_dut_peek_power;
	HANDLE m_handle_VSG;
	HANDLE m_handle_VSA;
	int vsa_loop;

	bool m_vsg_runing;
};

