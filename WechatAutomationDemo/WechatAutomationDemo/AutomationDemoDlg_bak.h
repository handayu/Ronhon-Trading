// AutomationDemoDlg.h : header file
//

#if !defined(AFX_AUTOMATIONDEMODLG_H__26C48350_7376_4798_A468_53F3668B7FDC__INCLUDED_)
#define AFX_AUTOMATIONDEMODLG_H__26C48350_7376_4798_A468_53F3668B7FDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdafx.h"
#include <VECTOR>
#include <map>
#include <string>
#include "TraderSpi.h"
#include "ReceiveSpi.h"
#include "CustomListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CAutomationDemoDlg dialog
struct PostionInfo
{
	int		m_nPosition;
	int		m_PositionType; //1: 多仓, -1: 空仓
};

struct COrderInfo
{
	bool					m_isEnd;
	CThostFtdcOrderField*	m_pOrder;
};

class CTraderSpi;
class CReceiveSpi;
class CAutomationDemoDlg : public CDialog
{
// Construction
public:
	CAutomationDemoDlg(CWnd* pParent = NULL);	// standard constructor
	//获取未成交报单
	void AddNewUntradeOrder(CThostFtdcOrderField* pOrder, bool bLast);
	void FormatLog( int nLevel, CString strDomain, CString strText );
public:
	static UINT InsertThread(LPVOID pParam );
	static UINT QryAccountThread(LPVOID pParam);
	static UINT QryPositionThread(LPVOID pParam);
// Dialog Data
	//{{AFX_DATA(CAutomationDemoDlg)
	enum { IDD = IDD_AUTOMATIONDEMO_DIALOG };
	CCustomListCtrl	m_ctrOrderList;
	CString	m_strAccountName;
	double	m_dClosePrice;
	CString	m_strInstrument;
	double	m_dPrice;
	CString	m_strPassword;
	int		m_nVolumn;
	CString	m_strIP;
	int		m_iPort;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutomationDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAutomationDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnLogin();
	afx_msg void OnBtnBegin();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBtnQryPosition();
	afx_msg void OnBtnQryUntrade();
	//}}AFX_MSG
	afx_msg LRESULT OnLoginSuccess(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLoginFailure(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRtnOrder(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRtnTrade(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()	
private:
	COrderInfo* FindOrder(CThostFtdcOrderField* pOrder);
	void ShowTradeRecord( CThostFtdcTradeField* pTrade );
	
	void RemoveAllUnTradeOrder();
public:
	CTraderSpi*							m_pTradeApi;
	CReceiveSpi*						m_pReceiveApi;
private:
	BOOL								m_bRunning;
	BOOL								m_bOnLine;
	BOOL								m_bLogin;
	FILE*								m_pLog;
	std::vector<COrderInfo*>			m_vecAllOrder;
	std::vector<CThostFtdcTradeField*>	m_vecAllTrade;
	std::map<std::string, PostionInfo*> m_mapPosition;
	std::vector<CThostFtdcOrderField*>  m_vecUntradeOrder;
	CWinThread*							m_pThread;
	CWinThread*							m_pThreadAccount;
	CWinThread*							m_pThreadPosition;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOMATIONDEMODLG_H__26C48350_7376_4798_A468_53F3668B7FDC__INCLUDED_)
