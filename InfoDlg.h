#pragma once


// InfoDlg ��ܤ��

class InfoDlg : public CDialog
{
	// ========================================
	 CWnd *m_pParent;
	 int m_nID;
	// ========================================	
	DECLARE_DYNAMIC(InfoDlg)

public:
	InfoDlg(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~InfoDlg();

// ��ܤ�����
	enum { IDD = IDD_INFODLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
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
