#pragma once


// Tab_RegDump ��ܤ��

class Tab_RegDump : public CDialog
{
	DECLARE_DYNAMIC(Tab_RegDump)

public:
	Tab_RegDump(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~Tab_RegDump();

// ��ܤ�����
	enum { IDD = IDD_TAB_REGDUMP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

	DECLARE_MESSAGE_MAP()
};
