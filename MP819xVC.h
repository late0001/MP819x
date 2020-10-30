// MP819xVC.h : main header file for the MP819xVC application
//

#if !defined(AFX_MP819xVC_H__26469B77_0034_403E_8E9E_D511A5DA8133__INCLUDED_)
#define AFX_MP819xVC_H__26469B77_0034_403E_8E9E_D511A5DA8133__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMP819xVCApp:
// See MP819xVC.cpp for the implementation of this class
//

class CMP819xVCApp : public CWinApp
{
public:
	CMP819xVCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMP819xVCApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMP819xVCApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MP819xVC_H__26469B77_0034_403E_8E9E_D511A5DA8133__INCLUDED_)
