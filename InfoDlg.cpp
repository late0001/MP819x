// InfoDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "MP819xVC.h"
#include "InfoDlg.h"
#include "MP819xVCDlg.h"


// InfoDlg 對話方塊

// ===================================================================
// Step 1: 儲存 Dialog Resource template  & 儲存建構者的位址

IMPLEMENT_DYNAMIC(InfoDlg, CDialog)
InfoDlg::InfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(InfoDlg::IDD, pParent)
{
	m_pParent=pParent;
	this->m_nID=InfoDlg::IDD;
}

// Step 2: Create Modeless Dialog
BOOL InfoDlg::Create(void)
{
	// 呼叫 CDialog::Create 建立 Modeless Dialog 視窗
    // 指定 m_nID ==> Dialog Resource template ID ,與 parent point
	         //return CDialog::Create(m_nID,m_pParent);
	return CDialog::Create(m_nID,NULL);
}

// Step 4: 當程式結束時, MFC 會呼叫這個 method delete 自己
void InfoDlg::PostNcDestroy(void)
{
	delete this;
}

// ===================================================================

InfoDlg::~InfoDlg()
{
}

void InfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INFO_TEXTBOX, m_InfoTextBox);
	
}


BEGIN_MESSAGE_MAP(InfoDlg, CDialog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// InfoDlg 訊息處理常式


BOOL InfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	InitInfoTextBox();
	bInfoDlgExist = 1;
	return TRUE;
}

void InfoDlg::OnClose()
{
	((CMP819xVCDlg*) m_pParent)->ModlessDlgReturn();
	EndDialog(TRUE);
}

void InfoDlg::InitInfoTextBox()
{
	CString Str1;
	Str1.Format("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",

		"  80MHz\r\n",
		"    +-----------------------------------------------+\r\n",
		"    |                       0                       |\r\n",
		"    +-----------------------+-----------------------+\r\n",
		"    |          1 0          |           9           |\r\n",
		"    +-----------+-----------+-----------+-----------+\r\n",
		"    |     4     |     2     |     1     |     3     |\r\n",
		"    +-----------+-----------+-----------+-----------+\r\n\r\n",

		"  40MHz\r\n",
		"    +-----------------------+\r\n",
		"    |           0           |\r\n",
		"    +-----------+-----------+\r\n",
		"    |     2     |     1     |\r\n",
		"    +-----------+-----------+\r\n\r\n",

		"  20MHz\r\n",
		"    +-----------+\r\n",
		"    |     0     |\r\n",
		"    +-----------+\r\n"
		);

	m_InfoTextBox.SetFont(&NormalFont, TRUE);
	m_InfoTextBox.SetWindowText(Str1);

}

