// Tab_AdcSample.cpp : 實作檔
//

#include "stdafx.h"
#include "Tab_AdcSample.h"

// Tab_AdcSample 對話方塊

IMPLEMENT_DYNAMIC(Tab_AdcSample, CDialog)

Tab_AdcSample::Tab_AdcSample()
	: CDialog(Tab_AdcSample::IDD)
{
}

Tab_AdcSample::~Tab_AdcSample()
{
}

void Tab_AdcSample::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEV_TRIGSIG_SEL, m_Dev_TriggerSignalSel);
	DDX_Control(pDX, IDC_DEV_TRIG_TIME, m_Dev_TriggerTime);
	DDX_Control(pDX, IDC_DEV_DMADATA_SEL, m_Dev_DmaDataSel);
	DDX_Control(pDX, IDC_DEV_POLL_TIME, m_Dev_TriggerPollTime);
	DDX_Control(pDX, IDC_DEV_TEXT_FIELD, m_Dev_Text_Field);
	DDX_Control(pDX, IDC_TAB_ADCSTATUS1, m_Dev_Status_1);
	DDX_Control(pDX, IDC_TAB_ADCSTATUS2, m_Dev_Status_2);
	DDX_Control(pDX, IDC_TRIG_HINT, m_Dev_Trigger_Hint);
	DDX_Control(pDX, IDC_DEV_MAC_TRIG, m_Dev_Trigger_MAC_Mode);
	DDX_Control(pDX, IDC_DEV_BB_TRIG, m_Dev_Trigger_BB_Mode);
	DDX_Control(pDX, IDC_DEV_ADCSMP_BTN, m_Button_AdcSample);
	DDX_Control(pDX, IDC_ADC_SAVE_FILE_LOCATION, m_Save_File_Location);
	DDX_Control(pDX, IDC_ADC_SAVE_FILE_BTN, m_Button_Save_File);
}


BEGIN_MESSAGE_MAP(Tab_AdcSample, CDialog)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_DEV_ADCSMP_BTN, OnBnClicked_AdcSample)
	ON_BN_CLICKED(IDC_DEV_MAC_TRIG, OnBnClickedDevMacTrig)
	ON_BN_CLICKED(IDC_DEV_BB_TRIG, OnBnClickedDevBbTrig)
	ON_BN_CLICKED(IDC_ADC_SAVE_FILE_BTN, OnAdcSaveFile)

END_MESSAGE_MAP()

void Tab_AdcSample::PollingForAdcSmpResult()
{
	UINT	sec, minCount, min, hrCount, hr, day;
	UINT	i = 0, RetStatus;
	UINT	BufferSize = 32768; // use 128K buffer for 8822B later IC, suggested by BB Stanley.

	UINT	*Buffer;
	CString	CurStringData, NextStringData, strFormatData;
	
	if(IS_HARDWARE_SERIES(8814A))
		BufferSize = 16384; // 64K buffer

	Buffer = (UINT*)malloc(BufferSize*sizeof(UINT));

	timerCount++;
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	sec			= timerCount%60;
	minCount	= timerCount/60;
	min			= minCount%60;
	hrCount		= minCount/60;
	hr			= hrCount%24;
	day			= hrCount/24;
	strFormatData.Format("Elapsed: %dd,%dh,%dm,%ds", day, hr, min, sec);
	m_Dev_Status_1.SetWindowText(strFormatData);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	RetStatus = ADC_GetResult((LPADAPTER)(Adapter.hFile), Buffer);
	
	if(RetStatus == TRUE)
	{
		// 8 bytes per row
		for(i = 0; i < BufferSize; i+=2)
		{
			CurStringData.Format("%08x", (UINT)Buffer[i]);
			NextStringData += CurStringData;
			CurStringData.Format("%08x\r\n", (UINT)Buffer[i+1]);
			NextStringData += CurStringData;
		}
		
		// Print on Text Field
		m_Dev_Text_Field.SetWindowText(NextStringData);

		// Reset to idle state
		ResetAdcState();

		// Save to file
		SaveAdcResult(NextStringData);
	}

	free(Buffer);

}

void Tab_AdcSample::SaveAdcResult(CString Result)
{
	CString Child_Str, Parent_Str;
	CString	Str1, Str2, Str3;
	TCHAR	Buf[10];

	Child_Str.Format("");
	Str2.Format("// LA Mode Setting\r\n");
	//"Trigger Mode: "
	if (m_Dev_Trigger_MAC_Mode.GetCheck())
		Str1.Format("MAC Trigger");
	else
		Str1.Format("BB Trigger");

	Child_Str.Format("%s%s%s", Str2, "Trigger Mode: ", Str1);
	Str2.Format("%s\r\n", Child_Str);

	//"Trigger Signal: "
	m_Dev_TriggerSignalSel.GetWindowText(Buf, 10);
	Child_Str.Format("%s%s%s", Str2, "Trigger Signal: ", Buf);
	Str2.Format("%s\r\n", Child_Str);

	//"DMA Data: "
	m_Dev_DmaDataSel.GetWindowText(Buf, 10);
	Child_Str.Format("%s%s%s", Str2, "DMA Data: ", Buf);
	Str2.Format("%s\r\n", Child_Str);

	//"Trigger Time: "
	m_Dev_TriggerTime.GetWindowText(Buf, 10);
	Child_Str.Format("%s%s%s", Str2, "Trigger Time: ", Buf);
	Str2.Format("%s\r\n", Child_Str);

	//"Polling Time: "
	m_Dev_TriggerPollTime.GetWindowText(Buf, 10);
	Child_Str.Format("%s%s%s\r\n", Str2, "Polling Time: ", Buf);

	Str2.Format("\r\n// PHY Setting\r\n");
	Parent_Str.Format("%s%s\r\n", Str2, BBDev_Str);

	Str3.Format("%s%s%s%s", Child_Str, Parent_Str, "\r\n/******************/\r\n", Result);

	SaveAdcResultToFile(Str3);
}

void Tab_AdcSample::SaveAdcResultToFile(CString Result)
{
	TCHAR	currentDir[MAX_PATH];
	CFile	myFile;
	CString	str1, str2;
	TCHAR	Buf[260];

	GetCurrentDirectory(MAX_PATH, currentDir);
	m_Save_File_Location.GetWindowText(Buf,260);
	str1.Format("%s", Buf);
	str2.Format("%s\\ADC_Result.txt", currentDir);
	
	if ( myFile.Open( str1, CFile::modeCreate |   
		   CFile::modeReadWrite ) )
	{
		myFile.Write( Result, Result.GetLength() * sizeof(TCHAR) );
		myFile.Flush();
	}
	else if ( myFile.Open( str2, CFile::modeCreate |   
		   CFile::modeReadWrite ) )
	{
		myFile.Write( Result, Result.GetLength() * sizeof(TCHAR) );
		myFile.Flush();
		MessageBox(
			TEXT("Using default path \".\\ADC_Result.txt\"."),
			TEXT("The specified path is not available!"),			
			MB_OK
			);
	}
	else
	{
		MessageBox(
			TEXT("Please make sure the file path exists."),
			TEXT("Save to file FAIL!"),			
			MB_OK
			);
	}
	myFile.Close();
}

void Tab_AdcSample::ResetAdcState()
{
	m_Button_AdcSample.SetWindowText("Start");
	timerCount = 0;
	KillTimer(DEV_BB_ADCSMP_POLLING);
	ADC_Stop((LPADAPTER)(Adapter.hFile));
	EnableControls(TRUE);
}

// Tab_AdcSample 訊息處理常式

BOOL Tab_AdcSample::OnInitDialog()
{
	CString strFormatData;
	CDialog::OnInitDialog();
	timerCount = 0;

	UpdateAdcStatus();

	m_Dev_Text_Field.SetFont(&NormalFont,TRUE);
	m_Dev_TriggerSignalSel.SetFont(&NormalFont,TRUE);
	m_Dev_DmaDataSel.SetFont(&NormalFont,TRUE);
	m_Dev_TriggerTime.SetFont(&NormalFont,TRUE);
	m_Dev_TriggerPollTime.SetFont(&NormalFont,TRUE);
	m_Dev_Status_1.SetFont(&SmallFont,TRUE);
	m_Dev_Status_2.SetFont(&SmallFont,TRUE);

	m_Dev_Trigger_MAC_Mode.SetCheck(TRUE);

	strFormatData.Format("%d", 3);
	m_Dev_TriggerSignalSel.SetWindowText(strFormatData);
	strFormatData.Format("%d", 0);
	m_Dev_DmaDataSel.SetWindowText(strFormatData);
	strFormatData.Format("%d", 96);
	m_Dev_TriggerTime.SetWindowText(strFormatData);
	strFormatData.Format("%d", 200);
	m_Dev_TriggerPollTime.SetWindowText(strFormatData);

	m_Save_File_Location.SetFont(&NormalFont,TRUE);

	return TRUE;
}

void Tab_AdcSample::UpdateAdcStatus()
{
	CString strFormatData;
	UINT Result = 0;
	Result = RegisterRead(0x8FC);
	if (Result < 0)
	{
		strFormatData.Format("[8FC]= N/A");
		m_Dev_Status_2.SetWindowText(strFormatData);
	}
	else
	{
		strFormatData.Format("[8FC]= 0x%x", Result);
		m_Dev_Status_2.SetWindowText(strFormatData);
	}
}

void Tab_AdcSample::EnableControls(BOOL bEnable)
{
	m_Dev_TriggerSignalSel.EnableWindow(bEnable);
	m_Dev_TriggerTime.EnableWindow(bEnable);
	m_Dev_DmaDataSel.EnableWindow(bEnable);
	m_Dev_TriggerPollTime.EnableWindow(bEnable);
	m_Dev_Trigger_MAC_Mode.EnableWindow(bEnable);
	m_Dev_Trigger_BB_Mode.EnableWindow(bEnable);
}


void Tab_AdcSample::OnBnClicked_AdcSample()
{
	ULONG	Param[2];
	CString	strFormatData;
	
	UINT	SignalSel = 0;
	UINT	TriggerMode = 0;
	UINT	TriggerTime = 0;
	UINT	DmaDataSel = 0;
	UINT	PollTime = 0;
	UINT	i = 0;
	
	TCHAR	Buf[260];
	
	m_Button_AdcSample.GetWindowText(Buf,260);
	strFormatData.Format("%s",Buf);
	if (strFormatData == "Start")
	{
		m_Button_AdcSample.SetWindowText("Stop");
		m_Dev_Text_Field.SetWindowText("");
		EnableControls(FALSE);
	}
	else
	{
		ResetAdcState();
		return;
	}

	// 1.
	// SignalSel(TRIG_SIG_SEL)
	//	預設為     3	;	範圍		0-3		如果 TRIG_SEL	為 1
	//	預設為     31	;	範圍		0-31		如果 TRIG_SEL	為 0
	SignalSel = getCurTriggerSignalSel();

	// 2.
	// DmaDataSel(DMA_DATA_SIG_SEL)
	// 預設為		0
	// 範圍		0-15
	DmaDataSel = getCurDmaDataType();

	// 3.
	// TriggerTime(TRIG_TIME_SEL)
	// 預設為		96
	// 單位為		us
	// 範圍		0- 2048
	TriggerTime = getCurTriggerTime();
	
	// 4.
	// PollTime(TRIG_POLL_TIME)
	// 預設為		200
	// 單位為		us
	// 範圍		200- 10000
	PollTime = getCurPollTime();

	if (m_Dev_Trigger_MAC_Mode.GetCheck())
		TriggerMode  = 1;
	else
		TriggerMode  = 0;

	SignalSel &= 0x1f;
	TriggerMode &= 0x01;
	TriggerTime &= 0xffff;
	DmaDataSel &= 0x0f;
	PollTime &= 0xffff;

	Param[0] = (TriggerTime<<16) | (DmaDataSel<<8) | (TriggerMode<<7) |SignalSel;
	Param[1] = (PollTime);
	strFormatData.Format("%x\r\n ADC Sample Triggered!", Param);
	m_Dev_Text_Field.SetWindowText(strFormatData);

	//BYTE 0[4:0]	(SignalSel)
	//BYTE 0[7]		(TriggerMode)
	//BYTE 1[3:0]	(DmaDataSel)
	//BYTE 2-3		(TriggerTime)
	//BYTE 4-5		(PollTime)
	ADC_Trigger((LPADAPTER)(Adapter.hFile), Param);

	// Enable Polling Timer
	
	SetTimer(DEV_BB_ADCSMP_POLLING, 1000, NULL);

}


UINT Tab_AdcSample::getCurTriggerSignalSel()
{
	CString strFormatData;
	char str1[10];
	UINT SignalSel = 0;
	UINT MaxValue  = 3;

	// SignalSel
	//	預設為     3	;	範圍		0-3		如果 TRIG_SEL	為 1
	//	預設為     31	;	範圍		0-31		如果 TRIG_SEL	為 0
	m_Dev_TriggerSignalSel.GetWindowText(str1, 10);
	strFormatData.Format("%s", str1);

	if (m_Dev_Trigger_MAC_Mode.GetCheck())
		MaxValue  = 3;
	else
		MaxValue  = 31;

	if (strFormatData != ""){
		SignalSel = atoi((const char *)strFormatData);
		if (SignalSel > MaxValue || SignalSel < 0)
			SignalSel = MaxValue;
	}
	else
		SignalSel = MaxValue;

	strFormatData.Format("%d", SignalSel);
	m_Dev_TriggerSignalSel.SetWindowText(strFormatData);

	return SignalSel;
}

UINT Tab_AdcSample::getCurDmaDataType()
{
	CString strFormatData;
	char str1[10];
	UINT DmaDataSel = 0;

	// DmaDataSel
	// 預設為		0 
	// 範圍		0-15
	m_Dev_DmaDataSel.GetWindowText(str1, 10);
	strFormatData.Format("%s", str1);
	if (strFormatData != ""){
		DmaDataSel = atoi((const char *)strFormatData);
		if (DmaDataSel > 15 || DmaDataSel < 0)
			DmaDataSel = 0;
	}
	else
		DmaDataSel = 0;
	strFormatData.Format("%d", DmaDataSel);
	m_Dev_DmaDataSel.SetWindowText(strFormatData);

	return DmaDataSel;
}

UINT Tab_AdcSample::getCurTriggerTime()
{
	CString strFormatData;
	char str1[10];
	UINT TriggerTime = 0;
	
	// TriggerTime
	// 預設為		96
	// 單位為		us
	// 範圍		0- 2048
	m_Dev_TriggerTime.GetWindowText(str1, 10);
	strFormatData.Format("%s", str1);
	TriggerTime = atoi((const char *)strFormatData);
	if (strFormatData != ""){
		TriggerTime = atoi((const char *)strFormatData);
		if (TriggerTime > 2048 || TriggerTime < 0)
			TriggerTime = 96;
	}
	else
		TriggerTime = 96;
	strFormatData.Format("%d", TriggerTime);
	m_Dev_TriggerTime.SetWindowText(strFormatData);

	return TriggerTime;
}

UINT Tab_AdcSample::getCurPollTime()
{
	CString strFormatData;
	char str1[10];
	UINT PollTime = 0;
	
	// PollTime
	// 預設為		200
	// 單位為		us
	// 範圍		200- 10000
	m_Dev_TriggerPollTime.GetWindowText(str1, 10);
	strFormatData.Format("%s", str1);
	PollTime = atoi((const char *)strFormatData);
	if (strFormatData != ""){
		PollTime = atoi((const char *)strFormatData);
		if (PollTime > 10000 || PollTime < 200)
			PollTime = 200;
	}
	else
		PollTime = 200;
	strFormatData.Format("%d", PollTime);
	m_Dev_TriggerPollTime.SetWindowText(strFormatData);

	return PollTime;
}


void Tab_AdcSample::OnBnClickedDevMacTrig()
{
	CString strFormatData;
	strFormatData.Format("%s", "(0~3)");
	m_Dev_Trigger_Hint.SetWindowText(strFormatData);
	strFormatData.Format("%d", 3);
	m_Dev_TriggerSignalSel.SetWindowText(strFormatData);
}


void Tab_AdcSample::OnBnClickedDevBbTrig()
{
	CString strFormatData;
	strFormatData.Format("%s", "(0~31)");
	m_Dev_Trigger_Hint.SetWindowText(strFormatData);
	strFormatData.Format("%d", 31);
	m_Dev_TriggerSignalSel.SetWindowText(strFormatData);
}

void Tab_AdcSample::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
    {
		case DEV_BB_ADCSMP_POLLING:
			PollingForAdcSmpResult();
			break;
	    default:
	        break;
    }

    CDialog::OnTimer(nIDEvent);
}

void Tab_AdcSample::OnClose()
{
	KillTimer(DEV_BB_ADCSMP_POLLING);
}

void Tab_AdcSample::OnAdcSaveFile()
{
	CString szFilter = "ADC Result(*.txt)|*.txt||";
			
	CFileDialog *fd = new CFileDialog(TRUE,"txt","ADC_Result.txt",OFN_HIDEREADONLY,szFilter,this);
	TCHAR currentDir[MAX_PATH];
	CString strFormatData;

	GetCurrentDirectory( MAX_PATH, currentDir );
			
	fd->m_ofn.lpstrInitialDir = currentDir;
			
	if(fd->DoModal() == IDOK) {
		m_Save_File_Location.SetWindowText(fd->GetPathName());
	}
	delete fd;
}



