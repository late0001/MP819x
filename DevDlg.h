#pragma once
#include "RtlFunc.h"
#include "Tab_AdcSample.h"
#include "Tab_RegDump.h"

// DevDlg 對話方塊

class DevDlg : public CDialog
{
	DECLARE_DYNAMIC(DevDlg)

public:
	DevDlg(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~DevDlg();
	afx_msg void OnDestroy();


// 對話方塊資料
	enum { IDD = IDD_DEV_PANEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	
public:
	CTabCtrl		m_tabFrame;
	Tab_AdcSample	m_AdcPage;
	Tab_RegDump		m_RegDumpPage;
	afx_msg void OnTcnSelchangeTabFrame(NMHDR *pNMHDR, LRESULT *pResult);
};
