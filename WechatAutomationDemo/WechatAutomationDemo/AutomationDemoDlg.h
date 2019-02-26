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
#include "CustomListCtrl.h"
#include "MdUserApi.h"
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

	COrderInfo();
	~COrderInfo();
};

struct InstrumentInfo 
{
	char	InstrumentID[31];
	char	InstrumentName[31];
	char	ExchangeID[5];
	int		VolumeMultiple;
	double	LongMarginRateByMoney;
	double	LongMarginRateByVol;
	double	ShortMarginRateByMoney;
	double	ShortMarginRateByVol;
	double	OpenRatioByMoney;
	double	OpenRatioByVol;
	double	CloseRatioByMoney;
	double	CloseRatioByVol;
	double	CloseTodayRatioByMoney;
	double	CloseToayRatioByVol;
};
class CTraderSpi;
class CAutomationDemoDlg : public CDialog
{
// Construction
public:
	CAutomationDemoDlg(CWnd* pParent = NULL);	// standard constructor
	//获取未成交报单
	void AddNewUntradeOrder(CThostFtdcOrderField* pOrder, bool bLast);
public:
	static UINT InsertThread(LPVOID pParam );
// Dialog Data
	//{{AFX_DATA(CAutomationDemoDlg)
	enum { IDD = IDD_AUTOMATIONDEMO_DIALOG };
	CEdit	m_ctrCurTrdVolSell;
	CEdit	m_ctrCurTrdVolBuy;
	CEdit	m_ctrCurTrdVol;
	CCustomListCtrl	m_ctrOrderList;
	CString	m_strAccountName;
	double	m_dClosePrice;
	CString	m_strInstrument;
	double	m_dPrice;
	CString	m_strPassword;
	int		m_nVolumn;
	CString	m_strIP;
	int		m_iPort;
	CString	m_strBrokerID;
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
	afx_msg void OnBtnQryMarginRate();
	afx_msg void OnBtnBuy();
	afx_msg void OnBtnSell();
	afx_msg void OnBtnSellToday();
	afx_msg void OnBtnChangePass();
	afx_msg void OnBtnQryComPos();
	afx_msg void OnBtnQryAccount();
	afx_msg void OnBtnQryInstrument();
	afx_msg void OnBtnQryPos();
	afx_msg void OnBtnQryCommisRate();
	afx_msg void OnBtnQryOrder();
	afx_msg void OnBtnQryTrade();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg LRESULT OnLoginSuccess(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLoginFailure(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRtnOrder(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRtnTrade(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRspQryInstrument(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRspQryInstrumentEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRspQryInstrumentMarginRate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnRspQryInstrumentCommiRate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRspLogoutSuccess(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()	
private:
	COrderInfo* FindOrder(CThostFtdcOrderField* pOrder);
	void ShowTradeRecord( CThostFtdcTradeField* pTrade );
	void FormatLog( int nLevel, CString strDomain, CString strText );
	void RemoveAllUnTradeOrder();
private:
	BOOL								m_bRunning;
	BOOL								m_bOnLine;
	BOOL								m_bLogin;
	CTraderSpi*							m_pTradeApi;
	CMdUserApi*							m_pMDApi;
	FILE*								m_pLog;
	std::vector<COrderInfo*>			m_vecAllOrder;
	std::vector<CThostFtdcTradeField*>	m_vecAllTrade;
	std::map<std::string, PostionInfo*> m_mapPosition;
	std::vector<CThostFtdcOrderField*>  m_vecUntradeOrder;
	std::map<std::string, InstrumentInfo*> m_mapInstrumentInfo;
	int									m_nTradeVolume;
	int									m_nTradeVolBuy;
	int									m_nTradeVolSell;
	CBrush								m_bkBrush;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOMATIONDEMODLG_H__26C48350_7376_4798_A468_53F3668B7FDC__INCLUDED_)
