// Tab_RegDump.cpp : 實作檔
//

#include "stdafx.h"
#include "MP819xVC.h"
#include "Tab_RegDump.h"


// Tab_RegDump 對話方塊

IMPLEMENT_DYNAMIC(Tab_RegDump, CDialog)

Tab_RegDump::Tab_RegDump(CWnd* pParent /*=NULL*/)
	: CDialog(Tab_RegDump::IDD, pParent)
{

}

Tab_RegDump::~Tab_RegDump()
{
}

void Tab_RegDump::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Tab_RegDump, CDialog)
END_MESSAGE_MAP()


// Tab_RegDump 訊息處理常式
