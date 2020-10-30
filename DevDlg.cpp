// DevDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "MP819xVC.h"
#include "DevDlg.h"


// DevDlg 對話方塊

IMPLEMENT_DYNAMIC(DevDlg, CDialog)

DevDlg::DevDlg(CWnd* pParent /*=NULL*/)
	: CDialog(DevDlg::IDD, pParent)
{
}

DevDlg::~DevDlg()
{
}



void DevDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BB_TAB_FRAME, m_tabFrame);
}


BEGIN_MESSAGE_MAP(DevDlg, CDialog)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_BB_TAB_FRAME, OnTcnSelchangeTabFrame)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(DevDlg, CDialog)
END_DISPATCH_MAP()

// 注意: 我們加入了對 IID_IDevDlg 的支援
//  以便從 VBA 支援型別安全繫結。此 IID 必須與 .IDL 檔中，
//  附加至分配介面 (Dispinterface) 的 GUID 相符。

// {8313EDCD-FBD5-471C-8B2F-6F601ABFCC3B}
static const IID IID_IDevDlg =
{ 0x8313EDCD, 0xFBD5, 0x471C, { 0x8B, 0x2F, 0x6F, 0x60, 0x1A, 0xBF, 0xCC, 0x3B } };

BEGIN_INTERFACE_MAP(DevDlg, CDialog)
	INTERFACE_PART(DevDlg, IID_IDevDlg, Dispatch)
END_INTERFACE_MAP()


BOOL DevDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// 設置頁面的位置在m_tabFrame控制項範圍內 
	CRect rs;
	m_tabFrame.GetClientRect(rs); 
	rs.top+=20; rs.bottom-=4; rs.left+=4; rs.right-=4; 
	
	m_tabFrame.InsertItem(0,_T("ADC Sample"));
	m_AdcPage.Create(Tab_AdcSample::IDD,	&m_tabFrame);
	
	m_tabFrame.InsertItem(1,_T("Reg Dump"));
	m_RegDumpPage.Create(Tab_RegDump::IDD,		&m_tabFrame);

	// 調整位置 
	m_AdcPage.MoveWindow(rs);
	m_RegDumpPage.MoveWindow(rs);
	
	// 設定第一頁可見, 其餘隱藏
	m_AdcPage.ShowWindow(TRUE); 
	m_RegDumpPage.ShowWindow(FALSE); 
	m_tabFrame.SetCurSel(0); 

	return TRUE;
}

void DevDlg::OnDestroy()
{

	m_AdcPage.DestroyWindow();
	delete m_AdcPage;

	m_RegDumpPage.DestroyWindow();
	delete m_RegDumpPage;

	CDialog::OnDestroy();
}




void DevDlg::OnTcnSelchangeTabFrame(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此加入控制項告知處理常式程式碼
	int CurSel; 
	CurSel = m_tabFrame.GetCurSel(); 
	switch (CurSel) 
	{ 
	case 0: 
		m_AdcPage.ShowWindow(TRUE); 
		m_RegDumpPage.ShowWindow(FALSE); 
		break; 
	case 1: 
		m_AdcPage.ShowWindow(FALSE); 
		m_RegDumpPage.ShowWindow(TRUE); 
		break; 
	default: ; 
	} 

	*pResult = 0;
}
