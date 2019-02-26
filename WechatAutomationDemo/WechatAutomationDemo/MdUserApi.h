// MdUserApi.h: interface for the CMdUserApi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDUSERAPI_H__EB15FB7E_0B26_46DA_A48C_4D66C5007855__INCLUDED_)
#define AFX_MDUSERAPI_H__EB15FB7E_0B26_46DA_A48C_4D66C5007855__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ThostFtdcMdApi.h"

class CAutomationDemoDlg;
class CMdUserApi : public CThostFtdcMdSpi  
{
public:
	CMdUserApi();
	virtual ~CMdUserApi();
public:
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();
	
	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason);
	
	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///登出请求响应
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///订阅行情应答
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///取消订阅行情应答
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///深度行情通知
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
public:
	void Init(FILE* pLog, char* szAddress);
	void SubscribeMarketData(char *pInstrumentID);
	void FormatLog( int nLevel, CString strDomain, CString strText );
	void SetMainDlg(CAutomationDemoDlg* pDlg) { m_pMainDlg = pDlg; }
private:
	CThostFtdcMdApi*			m_pMDApi;
	FILE*						m_pLog;
	int							m_nRequestID;
	CAutomationDemoDlg*			m_pMainDlg;
};

#endif // !defined(AFX_MDUSERAPI_H__EB15FB7E_0B26_46DA_A48C_4D66C5007855__INCLUDED_)
