// AutomationDemoDlg.cpp : implementation file
//
#include "StdAfx.h"
#include "AutomationDemo.h"
#include "AutomationDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutomationDemoDlg dialog

CAutomationDemoDlg::CAutomationDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutomationDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutomationDemoDlg)
	m_strAccountName = _T("");
	m_dClosePrice = 0.0;
	m_strInstrument = _T("");
	m_dPrice = 0.0;
	m_strPassword = _T("");
	m_nVolumn = 0;
	m_strIP = _T("");
	m_iPort = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_pTradeApi = NULL;
/*	m_pReceiveApi = NULL;*/
	m_bOnLine = FALSE;
	m_bRunning = FALSE;
	m_bLogin = FALSE;
	m_pThread = NULL;
	m_pThreadAccount = NULL;
	m_pThreadPosition = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAutomationDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutomationDemoDlg)
	DDX_Control(pDX, IDC_LIST_ORDER_LIST, m_ctrOrderList);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT_NAME, m_strAccountName);
	DDX_Text(pDX, IDC_EDIT_CLOSE_PRICE, m_dClosePrice);
	DDX_Text(pDX, IDC_EDIT_INSTRUMENT, m_strInstrument);
	DDX_Text(pDX, IDC_EDIT_OPEN_PRICE, m_dPrice);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_EDIT_VOLUMN, m_nVolumn);
	DDX_Text(pDX, IDC_EDIT_IP_ADDRESS, m_strIP);
	DDX_Text(pDX, IDC_EDIT_PORT, m_iPort);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAutomationDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CAutomationDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LOGIN, OnBtnLogin)
	ON_BN_CLICKED(IDC_BTN_BEGIN, OnBtnBegin)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_QRY_POSITION, OnBtnQryPosition)
	ON_BN_CLICKED(IDC_BTN_QRY_UNTRADE, OnBtnQryUntrade)
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_MSG_LOGIN_SUCCESS, OnLoginSuccess)
	ON_MESSAGE(WM_MSG_LOGIN_FAILURE, OnLoginFailure)
	ON_MESSAGE(WM_MSG_RTN_ORDER, OnRtnOrder)
	ON_MESSAGE(WM_MSG_RTN_TRADE, OnRtnTrade)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutomationDemoDlg message handlers

BOOL CAutomationDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CFont* pFont = new CFont();
	pFont->CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
   "宋体");
	HFONT hFont = (HFONT)(pFont->GetSafeHandle()); 
	
	SendMessageToDescendants(WM_SETFONT,(WPARAM)hFont,TRUE,TRUE,FALSE); 



	// TODO: Add extra initialization here
	m_pLog = fopen("./runLog.log", "w");
	CButton* pBtn = (CButton*)GetDlgItem(IDC_BTN_BEGIN);
	pBtn->EnableWindow(FALSE);
	pBtn = (CButton*)GetDlgItem(IDC_BTN_QRY_POSITION);
	pBtn->EnableWindow(FALSE);
	m_ctrOrderList.SetExtendedStyle(m_ctrOrderList.GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ctrOrderList.InsertColumn(0, _T("成交编号"), LVCFMT_CENTER, 80);
	m_ctrOrderList.InsertColumn(1, _T("合约"), LVCFMT_CENTER, 80);
	m_ctrOrderList.InsertColumn(2, _T("手数"), LVCFMT_CENTER, 40);
	m_ctrOrderList.InsertColumn(3, _T("买卖"), LVCFMT_CENTER, 40);
	m_ctrOrderList.InsertColumn(4, _T("开平"), LVCFMT_CENTER, 40);
	m_ctrOrderList.InsertColumn(5, _T("时间"), LVCFMT_CENTER, 120);
	m_ctrOrderList.SetRowHight(20);

	char szBuf[255];
	GetPrivateProfileString("Server", "ip", "127.0.0.1", szBuf, 255, "./Config.ini");
	m_strIP = szBuf;
	m_iPort = GetPrivateProfileInt("Server", "port", 7001, "./Config.ini");
	GetPrivateProfileString("Server", "user", "TestDemo1", szBuf, 255, "./Config.ini");
	m_strAccountName = szBuf;
	GetPrivateProfileString("Symbol", "id", "", szBuf, 255, "./Config.ini");
	m_strInstrument = szBuf;
	GetPrivateProfileString("Symbol", "open", "0.0", szBuf, 255, "./Config.ini");
	m_dPrice = atof(szBuf);
	GetPrivateProfileString("Symbol", "close", "0.0", szBuf, 255, "./Config.ini");
	m_dClosePrice = atof(szBuf);
	m_nVolumn = GetPrivateProfileInt("Symbol", "volume", 1, "./Config.ini");
	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAutomationDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAutomationDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAutomationDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAutomationDemoDlg::OnBtnLogin() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CButton* pBtn = (CButton*)GetDlgItem(IDC_BTN_LOGIN);
	if (!m_bOnLine && !m_bLogin)
	{
		if (m_strAccountName.IsEmpty() || m_strIP.IsEmpty() || m_strPassword.IsEmpty() || m_iPort <= 0)
		{
			MessageBox("登陆信息填写不正确");
			return;
		}
		if (m_pTradeApi == NULL)
		{
			m_pTradeApi = new CTraderSpi();
			char szAddr[100];
			sprintf(szAddr, "tcp://%s:%d", m_strIP, m_iPort);
			m_pTradeApi->Init(m_pLog, szAddr);
			m_pTradeApi->SetRspDlg(this);

// 			m_pReceiveApi = new CReceiveSpi();
// 			m_pReceiveApi->Init(m_pLog, szAddr);
// 			m_pReceiveApi->SetRspDlg(this);
		}
		else
		{
			m_pTradeApi->ReqUserLogin();
/*			m_pReceiveApi->ReqUserLogin();*/
		}
		m_bLogin = TRUE;
		pBtn->SetWindowText("正在登陆...");
	}
	else
	{
		delete m_pTradeApi;
		m_pTradeApi = NULL;
/*		m_pReceiveApi = NULL;*/
		m_bOnLine = FALSE;
		m_bLogin = FALSE;
		pBtn->SetWindowText("未登陆");
		pBtn = (CButton*)GetDlgItem(IDC_BTN_BEGIN);
		pBtn->SetWindowText("开始");
		pBtn->EnableWindow(FALSE);
	}
}

void CAutomationDemoDlg::OnBtnBegin() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_strInstrument.IsEmpty() || m_dPrice < 0.00001 || m_dClosePrice < 0.00001)
	{
		MessageBox("下单数据填写不正确");
		return;
	}
	CButton* pBtn = (CButton*)GetDlgItem(IDC_BTN_BEGIN);
	if (!m_bRunning)
	{
		m_bRunning = TRUE;
		m_pTradeApi->Start();
		pBtn->SetWindowText("暂停");
		m_pTradeApi->ReqOrderInsert(m_strInstrument.GetBuffer(m_strInstrument.GetLength()),
				m_dPrice, THOST_FTDC_OF_Open, THOST_FTDC_D_Buy, m_nVolumn);
		SetTimer(1, 1000, NULL);
	}
	else
	{
		KillTimer(1);
		m_bRunning = FALSE;
		m_pTradeApi->Stop();
		pBtn->SetWindowText("开始");
	}
/*	CButton* pBtn = (CButton*)GetDlgItem(IDC_BTN_BEGIN);
	if (!m_bRunning)
	{
		m_bRunning = TRUE;
		SetTimer(1, 200, NULL);
		pBtn->SetWindowText("暂停");
		if (!m_pThread)
		{
			m_pThread = AfxBeginThread(InsertThread, (LPVOID)this);
			m_pThreadAccount = AfxBeginThread(QryAccountThread, (LPVOID)this);
			m_pThreadPosition = AfxBeginThread(QryPositionThread, (LPVOID)this);
		}
		else
		{
			m_pThread->ResumeThread();
			m_pThreadAccount->ResumeThread();
			m_pThreadPosition->ResumeThread();
		}
	}
	else
	{
		m_bRunning = FALSE;
		KillTimer(1);
		pBtn->SetWindowText("开始");
		m_pThread->SuspendThread();
	}
*/
}

LRESULT CAutomationDemoDlg::OnLoginSuccess( WPARAM wParam, LPARAM lParam )
{
	m_bLogin = FALSE;
	CButton* pBtn = (CButton*)GetDlgItem(IDC_BTN_LOGIN);
	pBtn->SetWindowText("已登陆");
	m_bOnLine = TRUE;
	pBtn = (CButton*)GetDlgItem(IDC_BTN_BEGIN);
	pBtn->EnableWindow(TRUE);
	pBtn = (CButton*)GetDlgItem(IDC_BTN_QRY_POSITION);
	pBtn->EnableWindow(TRUE);
	UpdateData(TRUE);

	char szBuf[255];
	WritePrivateProfileString("Server", "ip", m_strIP, "./Config.ini");
	sprintf(szBuf, "%d", m_iPort);
	WritePrivateProfileString("Server", "port", szBuf, "./Config.ini");
	WritePrivateProfileString("Server", "user", m_strAccountName, "./Config.ini");
	WritePrivateProfileString("Symbol", "id", m_strInstrument, "./Config.ini");
	sprintf(szBuf, "%f", m_dPrice);
	WritePrivateProfileString("Server", "open", szBuf, "./Config.ini");
	sprintf(szBuf, "%f", m_dClosePrice);
	WritePrivateProfileString("Server", "close", szBuf, "./Config.ini");
	sprintf(szBuf, "%d", m_nVolumn);
	WritePrivateProfileString("Server", "volume", szBuf, "./Config.ini");
	return 0;
}

LRESULT CAutomationDemoDlg::OnLoginFailure( WPARAM wParam, LPARAM lParam )
{
	m_bLogin = FALSE;
	CButton* pBtn = (CButton*)GetDlgItem(IDC_BTN_LOGIN);
	pBtn->SetWindowText("未登陆");
	CThostFtdcRspInfoField *pRspField = (CThostFtdcRspInfoField*)wParam;
	CString strTmp = "登陆失败!!";
	if (pRspField)
	{
		strTmp.Format("登陆失败, ErrCode = %d, ErrMsg = %s", pRspField->ErrorID, pRspField->ErrorMsg);
		delete pRspField;
	}
	MessageBox(strTmp);
	return 0;
}

LRESULT CAutomationDemoDlg::OnRtnOrder( WPARAM wParam, LPARAM lParam )
{
	CThostFtdcOrderField* pOrderField = (CThostFtdcOrderField*)wParam;
	if (pOrderField)
	{
		COrderInfo* pOrderInfo = FindOrder(pOrderField);
		if (pOrderInfo)
		{
			pOrderInfo->m_pOrder->LimitPrice = pOrderField->LimitPrice;
			if(strlen(pOrderInfo->m_pOrder->OrderSysID) == 0)
			{
				strcpy(pOrderInfo->m_pOrder->OrderSysID,pOrderField->OrderSysID);
			}
			pOrderInfo->m_pOrder->OrderStatus = pOrderField->OrderStatus;
			strcpy(pOrderInfo->m_pOrder->StatusMsg,pOrderField->StatusMsg);
			strcpy(pOrderInfo->m_pOrder->UpdateTime,pOrderField->UpdateTime);
			strcpy(pOrderInfo->m_pOrder->InsertTime,pOrderField->InsertTime);

			if (pOrderInfo->m_pOrder->OrderStatus == THOST_FTDC_OST_AllTraded ||
				pOrderInfo->m_pOrder->OrderStatus == THOST_FTDC_OST_Canceled )
			{
				pOrderInfo->m_isEnd = true;
			}
			delete pOrderField;
		}
		else
		{
			pOrderInfo= new COrderInfo;
			pOrderInfo->m_isEnd = false;
			pOrderInfo->m_pOrder = pOrderField;
			if (pOrderInfo->m_pOrder->OrderStatus == THOST_FTDC_OST_AllTraded ||
				pOrderInfo->m_pOrder->OrderStatus == THOST_FTDC_OST_Canceled )
			{
				pOrderInfo->m_isEnd = true;
			}
			m_vecAllOrder.push_back(pOrderInfo);
		}
	}
	return 0;
}

LRESULT CAutomationDemoDlg::OnRtnTrade( WPARAM wParam, LPARAM lParam )
{
	CThostFtdcTradeField* pTrade = (CThostFtdcTradeField*)wParam;
	if (pTrade)
	{
		std::map<std::string, PostionInfo*>::iterator itMap = m_mapPosition.find(pTrade->InstrumentID);
		if (itMap != m_mapPosition.end())
		{
			switch(pTrade->OffsetFlag)
			{
			case THOST_FTDC_OF_Open:
				if (pTrade->Direction== THOST_FTDC_D_Buy)
				{
					//开多仓
					if (itMap->second->m_PositionType == 1)
					{
						itMap->second->m_nPosition += pTrade->Volume;
						FormatLog(LOG_APP_LEVEL, "OnRtnTrade", "继续开多仓");
					}
				}
				else if (pTrade->Direction == THOST_FTDC_D_Sell)
				{
					//开空仓
					if (itMap->second->m_PositionType == -1)
					{
						itMap->second->m_nPosition += pTrade->Volume;
						FormatLog(LOG_APP_LEVEL, "OnRtnTrade", "继续开空仓");
					}
				}
				break;
			case THOST_FTDC_OF_Close:
			case THOST_FTDC_OF_CloseToday:
			case THOST_FTDC_OF_CloseYesterday:
				if (pTrade->Direction== THOST_FTDC_D_Buy)
				{
					//平空仓
					if (itMap->second->m_PositionType == -1)
					{
						itMap->second->m_nPosition -= pTrade->Volume;
						if (itMap->second->m_nPosition <= 0)
						{
							delete itMap->second;
							m_mapPosition.erase(itMap);
						}
						FormatLog(LOG_APP_LEVEL, "OnRtnTrade", "平空仓");
					}
				}
				else if (pTrade->Direction == THOST_FTDC_D_Sell)
				{
					//平多仓
					if (itMap->second->m_PositionType == 1)
					{
						itMap->second->m_nPosition -= pTrade->Volume;
						if (itMap->second->m_nPosition <= 0)
						{
							delete itMap->second;
							m_mapPosition.erase(itMap);
						}
						FormatLog(LOG_APP_LEVEL, "OnRtnTrade", "平多仓");
					}
				}
				break;
			}
		}
		else
		{
			//只有开仓的时候才加进来
			if (pTrade->OffsetFlag == THOST_FTDC_OF_Open)
			{
				PostionInfo* pNewTrade = new PostionInfo;
				if (pTrade->Direction== THOST_FTDC_D_Buy)
				{
					pNewTrade->m_PositionType = 1;
					FormatLog(LOG_APP_LEVEL, "OnRtnTrade", "开多仓成功");
				}
				else
				{
					pNewTrade->m_PositionType = -1;
					FormatLog(LOG_APP_LEVEL, "OnRtnTrade", "开空仓成功");
				}
				pNewTrade->m_nPosition = pTrade->Volume;
				m_mapPosition[pTrade->InstrumentID] = pNewTrade;
			}
		}
		ShowTradeRecord(pTrade);
	}
	return 0;
}

COrderInfo* CAutomationDemoDlg::FindOrder( CThostFtdcOrderField* pField )
{
	COrderInfo* pInfo = NULL;
	for (int i=0; i<m_vecAllOrder.size(); i++)
	{
		pInfo = m_vecAllOrder[i];
		if (pInfo->m_isEnd)
		{
			continue;
		}
		//第一次报单回报, SysID为空, 通过OrderRef, frontID, SessionID唯一判断一个报单
		if (strlen(pField->OrderSysID) == 0 || strlen(pInfo->m_pOrder->OrderSysID) == 0)
		{
			if (strcmp(pInfo->m_pOrder->OrderRef,pField->OrderRef) == 0 &&
				pInfo->m_pOrder->FrontID == pField->FrontID &&
				pInfo->m_pOrder->SessionID == pField->SessionID)
			{
				return pInfo;
			}
		}
		//报单编号都不为空是比较报单编号
		else
		{
			if (strcmp(pInfo->m_pOrder->OrderSysID,pField->OrderSysID) == 0)
			{
				return pInfo;
			}
		}
	}
	return NULL;
}

void CAutomationDemoDlg::ShowTradeRecord( CThostFtdcTradeField* pTrade )
{
	m_ctrOrderList.InsertItem(0, pTrade->OrderSysID);
	m_ctrOrderList.SetItemText(0, 1, pTrade->InstrumentID);
	CString strFormat;
	strFormat.Format("%d", pTrade->Volume);
	m_ctrOrderList.SetItemText(0, 2, strFormat);
	if (pTrade->Direction == THOST_FTDC_D_Buy)
	{
		//m_ctrOrderList.SetTextColor(RGB(255, 0, 0));
		m_ctrOrderList.SetItemText(0, 3, "买");
	}
	else
	{
		//m_ctrOrderList.SetTextColor(RGB(0, 128, 0));
		m_ctrOrderList.SetItemText(0, 3, "卖");
	}
	if (pTrade->OffsetFlag == THOST_FTDC_OF_Open)
	{
		m_ctrOrderList.SetItemText(0, 4, "开仓");
	}
	else
	{
		m_ctrOrderList.SetItemText(0, 4, "平仓");
	}
	m_ctrOrderList.SetItemText(0, 5, pTrade->TradeTime);
}

void CAutomationDemoDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	UpdateData(TRUE);
	CDialog::OnTimer(nIDEvent);
}

void CAutomationDemoDlg::FormatLog( int nLevel, CString strDomain, CString strText )
{
	CString strLog, strTmp;
	SYSTEMTIME nowTime;
	GetLocalTime(&nowTime);
	strLog.Format("%d-%02d-%02d %02d:%02d:%02d %03d   ", nowTime.wYear, nowTime.wMonth, nowTime.wDay, nowTime.wHour, nowTime.wMinute, nowTime.wSecond, nowTime.wMilliseconds);
	switch(nLevel)
	{
	case LOG_APP_LEVEL:
		strTmp.Format("CAutomationDemoDlg::%s\t [Info] %s", strDomain, strText);
		break;
	case LOG_DEBUG_LEVEL:
		strTmp.Format("CAutomationDemoDlg::%s\t [Debug] %s", strDomain, strText);
		break;
	case LOG_WARN_LEVEL:
		strTmp.Format("CAutomationDemoDlg::%s\t [Warn] %s", strDomain, strText);
		break;
	case LOG_ERROR_LEVEL:
		strTmp.Format("CAutomationDemoDlg::%s\t [Err] %s", strDomain, strText);
		break;
	}
	if (m_pLog)
	{
		strLog += strTmp;
		fprintf(m_pLog, "%s\n", strLog);
		fflush(m_pLog);
	}
}

void CAutomationDemoDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_pLog)
	{
		fclose(m_pLog);
	}
	if (m_pTradeApi)
	{
		delete m_pTradeApi;
	}
	for (int i=0; i<m_vecAllOrder.size(); i++)
	{
		delete m_vecAllOrder[i]->m_pOrder;
		delete m_vecAllOrder[i];
	}
	m_vecAllOrder.clear();
	for (i=0; i<m_vecAllTrade.size(); i++)
	{
		delete m_vecAllTrade[i];
	}
	m_vecAllTrade.clear();

	std::map<std::string, PostionInfo*>::iterator itMap;
	for (itMap = m_mapPosition.begin(); itMap != m_mapPosition.end(); itMap++)
	{
		delete itMap->second;
	}
	m_mapPosition.clear();
}

void CAutomationDemoDlg::OnBtnQryPosition() 
{
	// TODO: Add your control notification handler code here
	CThostFtdcQryInvestorPositionDetailField *pQryPosDetail = new CThostFtdcQryInvestorPositionDetailField;
	memset(pQryPosDetail, 0, sizeof(CThostFtdcQryInvestorPositionDetailField));
	strncpy(pQryPosDetail->BrokerID, "RohonDemo", 10);
	strncpy(pQryPosDetail->InvestorID, m_strAccountName.GetBuffer(m_strAccountName.GetLength()), 12);
	m_strAccountName.ReleaseBuffer();
	m_pTradeApi->ReqQryInvestorPositionDetail(pQryPosDetail);
	delete pQryPosDetail;
}

void CAutomationDemoDlg::OnBtnQryUntrade() 
{
	// TODO: Add your control notification handler code here
	CThostFtdcQryOrderField *pOrderField = new CThostFtdcQryOrderField;
	memset(pOrderField, 0, sizeof(CThostFtdcQryOrderField));
	strncpy(pOrderField->BrokerID,  "RohonDemo", 10);
	strncpy(pOrderField->InvestorID, m_strAccountName.GetBuffer(m_strAccountName.GetLength()), 12);
	m_strAccountName.ReleaseBuffer();
	m_pTradeApi->ReqQryOrder(pOrderField);
	delete pOrderField;
}

void CAutomationDemoDlg::AddNewUntradeOrder( CThostFtdcOrderField* pOrder, bool bLast)
{
	if (pOrder == NULL)
	{
		if (bLast)
		{
			if (IDYES == MessageBox("要撤掉所有报单吗?", "提示", MB_YESNO))
			{
				RemoveAllUnTradeOrder();
			}
		}
		return;
	}
	m_vecUntradeOrder.push_back(pOrder);
	CString strText;
	strText.Format("Receive new untrade order SysID:%s, UnTradeVol:%d, Price:%02f", pOrder->OrderSysID, pOrder->VolumeTotal, pOrder->LimitPrice);
	FormatLog(LOG_APP_LEVEL, "UntradeOrder", strText);
}

void CAutomationDemoDlg::RemoveAllUnTradeOrder()
{
	CThostFtdcOrderField* pOrder = NULL;
	for (int i=0; i<m_vecUntradeOrder.size(); i++)
	{
		pOrder = m_vecUntradeOrder[i];
		CThostFtdcInputOrderActionField InputOrder;
		memset(&InputOrder, 0, sizeof(CThostFtdcInputOrderActionField));
		InputOrder.ActionFlag = THOST_FTDC_AF_Delete;
		strcpy(InputOrder.OrderSysID, pOrder->OrderSysID);
		strcpy(InputOrder.InstrumentID, pOrder->InstrumentID);
		CString strText;
		strText.Format("撤单 OrderSysID:%s", InputOrder.InvestorID, InputOrder.InstrumentID, InputOrder.OrderSysID, InputOrder.OrderRef);
		FormatLog(LOG_APP_LEVEL, "RemoveOrder", strText);
		m_pTradeApi->ReqOrderAction(&InputOrder);
		delete pOrder;
	}
	m_vecUntradeOrder.clear();
}

UINT CAutomationDemoDlg::InsertThread( LPVOID pParam )
{
	CAutomationDemoDlg* pDlg = (CAutomationDemoDlg*)pParam;
	bool bOpen = true;
	while(true)
	{
		if (bOpen)
		{
			pDlg->m_pTradeApi->ReqOrderInsert(pDlg->m_strInstrument.GetBuffer(pDlg->m_strInstrument.GetLength()),
				pDlg->m_dPrice, THOST_FTDC_OF_Open, THOST_FTDC_D_Buy, pDlg->m_nVolumn);
			bOpen = false;
		}
		else
		{
			pDlg->m_pTradeApi->ReqOrderInsert(pDlg->m_strInstrument.GetBuffer(pDlg->m_strInstrument.GetLength()),
				pDlg->m_dClosePrice, THOST_FTDC_OF_Close, THOST_FTDC_D_Sell, pDlg->m_nVolumn);
			bOpen = true;
		}
		Sleep(1000);
	}
	return 0;
}

UINT CAutomationDemoDlg::QryAccountThread( LPVOID pParam )
{
	CAutomationDemoDlg* pDlg = (CAutomationDemoDlg*)pParam;
	while(true)
	{
		pDlg->FormatLog(LOG_APP_LEVEL, "AccountThread", "Begin query account");
		pDlg->m_pTradeApi->ReqQryTradingAccount();
		Sleep(3000);
	}
	return 0;
}

UINT CAutomationDemoDlg::QryPositionThread( LPVOID pParam )
{
	CAutomationDemoDlg* pDlg = (CAutomationDemoDlg*)pParam;
	CThostFtdcQryInvestorPositionDetailField* pDetail = new CThostFtdcQryInvestorPositionDetailField;
	strcpy(pDetail->BrokerID, "3030");
	strcpy(pDetail->InvestorID, "haosc");
	pDetail->InstrumentID[0] = 0;
	while(true)
	{
		pDlg->FormatLog(LOG_APP_LEVEL, "PosThread", "Begin query user position");
		pDlg->m_pTradeApi->ReqQryInvestorPositionDetail(pDetail);
		Sleep(1000);
	}
	return 0;
}

