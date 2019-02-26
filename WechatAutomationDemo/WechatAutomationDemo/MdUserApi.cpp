// MdUserApi.cpp: implementation of the CMdUserApi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "automationdemo.h"
#include "MdUserApi.h"
#include "AutomationDemoDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMdUserApi::CMdUserApi()
{
	m_pMDApi = NULL;
	m_nRequestID = 1;
	m_pMainDlg = NULL;
}

CMdUserApi::~CMdUserApi()
{

}

void CMdUserApi::OnFrontConnected()
{
	CThostFtdcReqUserLoginField field;
	memset(&field, 0, sizeof(CThostFtdcReqUserLoginField));
	strcpy(field.BrokerID, m_pMainDlg->m_strBrokerID);
	strcpy(field.UserID, m_pMainDlg->m_strAccountName);
	strcpy(field.Password, m_pMainDlg->m_strPassword);
	m_pMDApi->ReqUserLogin(&field, m_nRequestID++);
}

void CMdUserApi::OnFrontDisconnected( int nReason )
{
	FormatLog(LOG_WARN_LEVEL, "DisCon", "OnFrontDisconnected");
}

void CMdUserApi::OnRspUserLogin( CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	CString strText;
	if (pRspInfo && pRspInfo->ErrorID != 0)
	{
		strText.Format("ErrorID=%d; ErrorMsg=%s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		FormatLog(LOG_ERROR_LEVEL, "RspLogin", strText);
	}
	else
	{
		FormatLog(LOG_APP_LEVEL, "RspLogin", "Success");
	}
}

void CMdUserApi::OnRspUserLogout( CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if (pRspInfo && pRspInfo->ErrorID != 0)
	{
		CString strText;
		strText.Format("ErrorID=%d; ErrorMsg=%s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		FormatLog(LOG_ERROR_LEVEL, "RspLogout", strText);
	}
}

void CMdUserApi::OnRspError( CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if (pRspInfo && pRspInfo->ErrorID != 0)
	{
		CString strText;
		strText.Format("ErrorID=%d; ErrorMsg=%s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		FormatLog(LOG_ERROR_LEVEL, "RspError", strText);
	}
}

void CMdUserApi::OnRspSubMarketData( CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	
}

void CMdUserApi::OnRspUnSubMarketData( CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	
}

void CMdUserApi::OnRtnDepthMarketData( CThostFtdcDepthMarketDataField *pDepthMarketData )
{
	CString strText;
	strText.Format("[%s], ×îÐÂ:%f, Âò1:%f, Âô1:%f", 
		pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice, pDepthMarketData->BidPrice1, pDepthMarketData->AskPrice1);
	FormatLog(LOG_APP_LEVEL, "OnDepthMarket", strText);
}

void CMdUserApi::Init(FILE* pLog, char* szAddress )
{
	m_pLog = pLog;
	if (m_pMDApi != NULL)
	{
		m_pMDApi->Release();
		m_pMDApi = NULL;
	}
	m_pMDApi = CThostFtdcMdApi::CreateFtdcMdApi();
	m_pMDApi->RegisterSpi(this);
	m_pMDApi->RegisterFront(szAddress);
	m_pMDApi->Init();
}

void CMdUserApi::SubscribeMarketData( char *pInstrumentID)
{
	char** ppInstrumentID = new char*[1];
	ppInstrumentID[0] = pInstrumentID;
	m_pMDApi->SubscribeMarketData(ppInstrumentID, 1);
}

void CMdUserApi::FormatLog( int nLevel, CString strDomain, CString strText )
{
	CString strLog, strTmp;
	SYSTEMTIME nowTime;
	GetLocalTime(&nowTime);
	strLog.Format("%d-%02d-%02d %02d:%02d:%02d %03d   ", nowTime.wYear, nowTime.wMonth, nowTime.wDay, nowTime.wHour, nowTime.wMinute, nowTime.wSecond, nowTime.wMilliseconds);
	switch(nLevel)
	{
	case LOG_APP_LEVEL:
		strTmp.Format("CTraderSpi::%s\t [Info] %s", strDomain, strText);
		break;
	case LOG_DEBUG_LEVEL:
		strTmp.Format("CTraderSpi::%s\t [Debug] %s", strDomain, strText);
		break;
	case LOG_WARN_LEVEL:
		strTmp.Format("CTraderSpi::%s\t [Warn] %s", strDomain, strText);
		break;
	case LOG_ERROR_LEVEL:
		strTmp.Format("CTraderSpi::%s\t [Err] %s", strDomain, strText);
		break;
	}
	if (m_pLog)
	{
		strLog += strTmp;
		fprintf(m_pLog, "%s\n", strLog);
		fflush(m_pLog);
	}
}