#if !defined(AFX_CUSTOMLISTCTRL_H__2C2F088F_FB94_4526_AE05_7892AB904CE2__INCLUDED_)
#define AFX_CUSTOMLISTCTRL_H__2C2F088F_FB94_4526_AE05_7892AB904CE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustomListCtrl window

class CCustomListCtrl : public CListCtrl
{
// Construction
public:
	CCustomListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetRowHight(int nHeight);
	virtual ~CCustomListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCustomListCtrl)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
private:
	int m_nRowHeight;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMLISTCTRL_H__2C2F088F_FB94_4526_AE05_7892AB904CE2__INCLUDED_)
