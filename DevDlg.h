#pragma once
#include "RtlFunc.h"
#include "Tab_AdcSample.h"
#include "Tab_RegDump.h"

// DevDlg ��ܤ��

class DevDlg : public CDialog
{
	DECLARE_DYNAMIC(DevDlg)

public:
	DevDlg(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~DevDlg();
	afx_msg void OnDestroy();


// ��ܤ�����
	enum { IDD = IDD_DEV_PANEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
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
