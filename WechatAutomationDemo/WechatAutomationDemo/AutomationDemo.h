// AutomationDemo.h : main header file for the AUTOMATIONDEMO application
//

#if !defined(AFX_AUTOMATIONDEMO_H__447BC644_ED1F_4569_8E5D_8A500D3E9C0A__INCLUDED_)
#define AFX_AUTOMATIONDEMO_H__447BC644_ED1F_4569_8E5D_8A500D3E9C0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAutomationDemoApp:
// See AutomationDemo.cpp for the implementation of this class
//

class CAutomationDemoApp : public CWinApp
{
public:
	CAutomationDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutomationDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAutomationDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOMATIONDEMO_H__447BC644_ED1F_4569_8E5D_8A500D3E9C0A__INCLUDED_)
