// CustomListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CustomListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomListCtrl

CCustomListCtrl::CCustomListCtrl()
{
}

CCustomListCtrl::~CCustomListCtrl()
{
}


BEGIN_MESSAGE_MAP(CCustomListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CCustomListCtrl)
	ON_WM_MEASUREITEM_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomListCtrl message handlers

void CCustomListCtrl::SetRowHight(int nHeight)
{
	m_nRowHeight = nHeight;
	CRect rcWin;
	GetWindowRect(&rcWin);
	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rcWin.Width();
	wp.cy = rcWin.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
}

void CCustomListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	lpMeasureItemStruct->itemHeight = m_nRowHeight;
	//CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CCustomListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	TCHAR lpBuffer[512];
	LV_ITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.iItem = lpDrawItemStruct->itemID;
	lvi.iSubItem = 0;
	lvi.pszText = lpBuffer;
	lvi.cchTextMax = sizeof(lpBuffer);
	VERIFY(GetItem(&lvi));
	LV_COLUMN lvc, lvcprev;
	::ZeroMemory(&lvc, sizeof(LV_COLUMN));
	::ZeroMemory(&lvcprev, sizeof(LV_COLUMN));
	lvc.mask = LVCF_WIDTH | LVCF_FMT;
	lvcprev.mask = LVCF_WIDTH | LVCF_FMT;
	for ( int nCol=0; GetColumn(nCol, &lvc); nCol++)
	{
		if (nCol > 0)
		{
			GetColumn(nCol-1, &lvcprev);
			lpDrawItemStruct->rcItem.left += lvcprev.cx+3;
			lpDrawItemStruct->rcItem.right += lpDrawItemStruct->rcItem.left;
		}
		::ZeroMemory(&lvi, sizeof(lvi));
		lvi.iItem = lpDrawItemStruct->itemID;
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iSubItem = nCol;
		lvi.pszText = lpBuffer;
		lvi.cchTextMax = sizeof(lpBuffer);
		VERIFY(GetItem(&lvi));
		CDC* pDC;
		pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		CPen* pOldPen = NULL;
		if ( lpDrawItemStruct->itemState & ODS_SELECTED )
		{
			pDC->FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_HIGHLIGHT));
			pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
			
		}
		else
		{
			pDC->FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_WINDOW));
			pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		}
		pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));
		UINT uFormat = DT_LEFT;
		::DrawText(lpDrawItemStruct->hDC, lpBuffer, strlen(lpBuffer), &lpDrawItemStruct->rcItem, uFormat); 
		pDC->SelectStockObject(SYSTEM_FONT);
		if (pOldPen != NULL)
		{
			pDC->SelectObject(pOldPen);
		}
	}
	//CListCtrl::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
