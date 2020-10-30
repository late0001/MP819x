#pragma once
#include "RtlFunc.h"

// Tab_AdcSample 對話方塊



class Tab_AdcSample : public CDialog
{
	DECLARE_DYNAMIC(Tab_AdcSample)

public:
	Tab_AdcSample();
	virtual ~Tab_AdcSample();

// Declarations
	
	CEdit	m_Dev_TriggerSignalSel;	// Mode
	CEdit	m_Dev_TriggerTime;		// WaitPeriod
	CEdit	m_Dev_DmaDataSel;		// DataType
	CEdit	m_Dev_TriggerPollTime;	// TimeWaitBetweenPolling
	CEdit	m_Dev_Text_Field;
	CEdit	m_Save_File_Location;
	CStatic m_Dev_Status_1;
	CStatic m_Dev_Status_2;
	CStatic m_Dev_Trigger_Hint;
	CButton	m_Dev_Trigger_MAC_Mode;
	CButton	m_Dev_Trigger_BB_Mode;
	CButton m_Button_AdcSample;
	CButton	m_Button_Save_File;

// 對話方塊資料
	enum { IDD = IDD_TAB_ADCSAMPLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClicked_AdcSample();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	
	UINT	timerCount;

	void
	PollingForAdcSmpResult(
		);

	void
	EnableControls(
		BOOL bEnable
		);

	void
	ResetAdcState(
		);

	void 
	SaveAdcResult(
		CString Result
		);


	void 
	SaveAdcResultToFile(
		CString Result
		);

	void
	UpdateAdcStatus(
		);

	UINT
	getCurTriggerSignalSel(
		);

	UINT
	getCurDmaDataType(
		);

	UINT
	getCurTriggerTime(
		);

	UINT
	getCurPollTime(
		);

	public:
	afx_msg void OnBnClickedDevMacTrig();
	afx_msg void OnBnClickedDevBbTrig();
	afx_msg void OnAdcSaveFile();
	
};
