// DevDlg.cpp : ��@��
//

#include "stdafx.h"
#include "MP819xVC.h"
#include "DevDlg.h"


// DevDlg ��ܤ��

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

// �`�N: �ڭ̥[�J�F�� IID_IDevDlg ���䴩
//  �H�K�q VBA �䴩���O�w��ô���C�� IID �����P .IDL �ɤ��A
//  ���[�ܤ��t���� (Dispinterface) �� GUID �۲šC

// {8313EDCD-FBD5-471C-8B2F-6F601ABFCC3B}
static const IID IID_IDevDlg =
{ 0x8313EDCD, 0xFBD5, 0x471C, { 0x8B, 0x2F, 0x6F, 0x60, 0x1A, 0xBF, 0xCC, 0x3B } };

BEGIN_INTERFACE_MAP(DevDlg, CDialog)
	INTERFACE_PART(DevDlg, IID_IDevDlg, Dispatch)
END_INTERFACE_MAP()


BOOL DevDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// �]�m��������m�bm_tabFrame����d�� 
	CRect rs;
	m_tabFrame.GetClientRect(rs); 
	rs.top+=20; rs.bottom-=4; rs.left+=4; rs.right-=4; 
	
	m_tabFrame.InsertItem(0,_T("ADC Sample"));
	m_AdcPage.Create(Tab_AdcSample::IDD,	&m_tabFrame);
	
	m_tabFrame.InsertItem(1,_T("Reg Dump"));
	m_RegDumpPage.Create(Tab_RegDump::IDD,		&m_tabFrame);

	// �վ��m 
	m_AdcPage.MoveWindow(rs);
	m_RegDumpPage.MoveWindow(rs);
	
	// �]�w�Ĥ@���i��, ��l����
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
	// TODO: �b���[�J����i���B�z�`���{���X
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
