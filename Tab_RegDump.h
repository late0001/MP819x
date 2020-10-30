#pragma once


// Tab_RegDump 對話方塊

class Tab_RegDump : public CDialog
{
	DECLARE_DYNAMIC(Tab_RegDump)

public:
	Tab_RegDump(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~Tab_RegDump();

// 對話方塊資料
	enum { IDD = IDD_TAB_REGDUMP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
};
