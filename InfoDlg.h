#pragma once


// InfoDlg 對話方塊

class InfoDlg : public CDialog
{
	// ========================================
	 CWnd *m_pParent;
	 int m_nID;
	// ========================================	
	DECLARE_DYNAMIC(InfoDlg)

public:
	InfoDlg(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~InfoDlg();

// 對話方塊資料
	enum { IDD = IDD_INFODLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnClose();
	void InitInfoTextBox();

private:
	void PostNcDestroy(void);

public:
	BOOL Create(void);

	CStatic	m_InfoTextBox;

};
