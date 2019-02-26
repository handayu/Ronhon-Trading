#if !defined(AFX_CHANGEPASSWORDDLG_H__6B9ED3E6_6EE6_40BB_8C65_B06E88A2C3E5__INCLUDED_)
#define AFX_CHANGEPASSWORDDLG_H__6B9ED3E6_6EE6_40BB_8C65_B06E88A2C3E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChangePasswordDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChangePasswordDlg dialog

class CChangePasswordDlg : public CDialog
{
// Construction
public:
	CChangePasswordDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChangePasswordDlg)
	enum { IDD = IDD_DIALOG_CHANGE_PASSWORD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChangePasswordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChangePasswordDlg)
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString				m_OldPassword;
	CString				m_NewPassword;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANGEPASSWORDDLG_H__6B9ED3E6_6EE6_40BB_8C65_B06E88A2C3E5__INCLUDED_)
