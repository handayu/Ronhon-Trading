// TraderSpi.cpp: implementation of the CTraderSpi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutomationDemo.h"
#include "TraderSpi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTraderSpi::CTraderSpi()
{
	m_pLog = NULL;
	m_pUserApi = NULL;
	m_nRequestId = 0;
	m_bLogin = false;
	m_nRspInsertTime = 0;
// 	m_chDir = THOST_FTDC_D_Buy;			//初始化为买
// 	m_chOffset = THOST_FTDC_OF_Open;	//初始化为开仓
}

CTraderSpi::~CTraderSpi()
{
	if (m_pUserApi)
	{
		m_pUserApi->RegisterSpi(NULL);
		m_pUserApi->Release();
		m_pUserApi = NULL;
	}
}

void CTraderSpi::OnFrontConnected()
{
	///用户登录请求
	ReqUserLogin();
}

void CTraderSpi::ReqUserLogin()
{
	m_bLogin = true;
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_pDlg->m_strBrokerID);
	strcpy(req.UserID, m_pDlg->m_strAccountName);
	strcpy(req.Password, m_pDlg->m_strPassword);
	strcpy(req.UserProductInfo, "RohonStock");
	int iResult = m_pUserApi->ReqUserLogin(&req, ++m_nRequestId);
	if (iResult == 0)
	{
		FormatLog(LOG_APP_LEVEL, "ReqUserLogin", "登陆请求成功");
	}
	else
	{
		m_bLogin = false;
		FormatLog(LOG_ERROR_LEVEL, "ReqUserLogin", "请求发送失败");
		m_pDlg->PostMessage(WM_MSG_LOGIN_FAILURE);
	}
}

void CTraderSpi::OnRspUserLogin( CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	m_bLogin = false;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		m_frontID = pRspUserLogin->FrontID;
		m_sessionID = pRspUserLogin->SessionID;
		m_iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		m_iNextOrderRef++;
		sprintf(m_orderRef, "%d", m_iNextOrderRef);
		FormatLog(LOG_APP_LEVEL, "OnRspUserLogin", "User Login success");
		m_pDlg->PostMessage(WM_MSG_LOGIN_SUCCESS);

		CThostFtdcSettlementInfoConfirmField filed;
		memset(&filed, 0, sizeof(CThostFtdcSettlementInfoConfirmField));
		strcpy(filed.BrokerID, m_pDlg->m_strBrokerID);
		strcpy(filed.InvestorID, m_pDlg->m_strAccountName);
		m_pUserApi->ReqSettlementInfoConfirm(&filed, m_nRequestId++);
	}
	else
	{
		CString strFormat;
		strFormat.Format("User Login Failure ErrCode:%d, ErrMsg:%s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		FormatLog(LOG_APP_LEVEL, "OnRspUserLogin", strFormat);
		CThostFtdcRspInfoField *pRspField = new CThostFtdcRspInfoField;
		memcpy(pRspField, pRspInfo, sizeof(CThostFtdcRspInfoField));
		m_pDlg->PostMessage(WM_MSG_LOGIN_FAILURE, (WPARAM)pRspField);
	}
}

void CTraderSpi::OnRspSettlementInfoConfirm( CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	
}

void CTraderSpi::OnRspQryInstrument( CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	CString strTmp;
	if (IsErrorRspInfo(pRspInfo))
	{
		FormatLog(LOG_ERROR_LEVEL, "OnRspQryInstrument", "Error response errCode:%d, errMsg:%s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else if (pInstrument)
	{
		FormatLog(LOG_APP_LEVEL, "OnRspQryInstrument", "%s, %.2lf, %.2lf, %.2lf, %d", 
			pInstrument->InstrumentID, pInstrument->PriceTick, pInstrument->LongMarginRatio, 
			pInstrument->ShortMarginRatio, pInstrument->VolumeMultiple);
		CThostFtdcInstrumentField *pInst = new CThostFtdcInstrumentField;
		memcpy(pInst, pInstrument, sizeof(CThostFtdcInstrumentField));
		m_pDlg->PostMessage(WM_MSG_RSPQRYINSTRUMENT, (WPARAM)pInst);
	}
	if (bIsLast)
	{
		m_pDlg->PostMessage(WM_MSG_RSPQRYINSTRUMETNEND);
	}
}

void CTraderSpi::OnRspQryTradingAccount( CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if (IsErrorRspInfo(pRspInfo))
	{
		FormatLog(LOG_ERROR_LEVEL, "RspQryTradingAccount", "Error response errCode:%d, errMsg:%s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else if (pTradingAccount)
	{
		FormatLog(LOG_APP_LEVEL, "RspQryTradingAccount", "%s,%s, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %s,%d, %f, %f, %f, %f, %f", 
			pTradingAccount->BrokerID, pTradingAccount->AccountID, pTradingAccount->PreMortgage, pTradingAccount->PreCredit, pTradingAccount->PreDeposit, pTradingAccount->PreBalance, pTradingAccount->PreMargin,
			pTradingAccount->InterestBase, pTradingAccount->Interest, pTradingAccount->Deposit, pTradingAccount->Withdraw, pTradingAccount->FrozenMargin, pTradingAccount->FrozenCash, pTradingAccount->FrozenCommission,
			pTradingAccount->CurrMargin, pTradingAccount->CashIn, pTradingAccount->Commission, pTradingAccount->CloseProfit, pTradingAccount->PositionProfit, pTradingAccount->Balance, pTradingAccount->Available,
			pTradingAccount->WithdrawQuota, pTradingAccount->Reserve, pTradingAccount->TradingDay, pTradingAccount->SettlementID, pTradingAccount->Credit, pTradingAccount->Mortgage, pTradingAccount->ExchangeMargin,
			pTradingAccount->DeliveryMargin, pTradingAccount->ExchangeDeliveryMargin);
	}
}

void CTraderSpi::OnRspQryInvestorPosition( CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if (pRspInfo && pRspInfo->ErrorID != 0)
	{
		FormatLog(LOG_APP_LEVEL, "OnRspQryInvestorPosition", "ErrID=%d, ErrMsg=%s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else if (pInvestorPosition)
	{
		FormatLog(LOG_APP_LEVEL, "RspPosition", "%s,%s,%s,'%c','%c','%c',%d,%d,%d,%d,%.2lf,%.2lf,%d,%d,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%s,%d,%.2lf,%.2lf,%d,%d,%d,%.2lf,%.2lf,%d%.2lf,%.2lf", 
			pInvestorPosition->InstrumentID, pInvestorPosition->BrokerID, pInvestorPosition->InvestorID, pInvestorPosition->PosiDirection, pInvestorPosition->HedgeFlag,
			pInvestorPosition->PositionDate, pInvestorPosition->YdPosition, pInvestorPosition->Position, pInvestorPosition->LongFrozen, pInvestorPosition->ShortFrozen,
			pInvestorPosition->LongFrozenAmount, pInvestorPosition->ShortFrozenAmount, pInvestorPosition->OpenVolume, pInvestorPosition->CloseVolume, pInvestorPosition->OpenAmount,
			pInvestorPosition->CloseAmount, pInvestorPosition->PositionCost, pInvestorPosition->PreMargin, pInvestorPosition->UseMargin, pInvestorPosition->FrozenMargin,
			pInvestorPosition->FrozenCash, pInvestorPosition->FrozenCommission, pInvestorPosition->CashIn, pInvestorPosition->Commission, pInvestorPosition->CloseProfit,
			pInvestorPosition->PositionProfit, pInvestorPosition->PreSettlementPrice, pInvestorPosition->SettlementPrice, pInvestorPosition->TradingDay,pInvestorPosition->SettlementID,
			pInvestorPosition->OpenCost, pInvestorPosition->ExchangeMargin, pInvestorPosition->CombPosition, pInvestorPosition->CombLongFrozen,pInvestorPosition->CombShortFrozen,
			pInvestorPosition->CloseProfitByDate, pInvestorPosition->CloseProfitByTrade, pInvestorPosition->TodayPosition, pInvestorPosition->MarginRateByMoney, pInvestorPosition->MarginRateByVolume);
	}
	if (bIsLast)
	{
		FormatLog(LOG_APP_LEVEL, "RspPosition", "查询结束!");
	}
}

void CTraderSpi::ReqOrderInsert(char* chInstrment, double price, char offsetFlag, char direction, int volumn)
{
	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	///经纪公司代码
	strcpy(req.BrokerID, m_pDlg->m_strBrokerID);
	///投资者代码
	strcpy(req.InvestorID, m_pDlg->m_strAccountName);
	///合约代码
	strcpy(req.InstrumentID, chInstrment);
	///用户代码
	//	TThostFtdcUserIDType	UserID;
	///报单价格条件: 限价
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	//req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
	///买卖方向: 
	req.Direction = direction;
	///组合开平标志: 开仓
	req.CombOffsetFlag[0] = offsetFlag;
	///组合投机套保标志
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///价格
	req.LimitPrice = price;
	///数量: 1
	req.VolumeTotalOriginal = volumn;
	///有效期类型: 当日有效
	req.TimeCondition = THOST_FTDC_TC_GFD;
	//req.TimeCondition = THOST_FTDC_TC_IOC;
	///GTD日期
	//	TThostFtdcDateType	GTDDate;
	///成交量类型: 任何数量
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///最小成交量: 1
	req.MinVolume = 1;
	///触发条件: 立即
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	///止损价
	//	TThostFtdcPriceType	StopPrice;
	///强平原因: 非强平
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///自动挂起标志: 否
	req.IsAutoSuspend = 0;
	///业务单元
	//	TThostFtdcBusinessUnitType	BusinessUnit;
	///请求编号
	//	TThostFtdcRequestIDType	RequestID;
	///用户强评标志: 否
	req.UserForceClose = 0;

	req.RequestID = ++m_nRequestId;
	int iResult = m_pUserApi->ReqOrderInsert(&req, ++m_nRequestId);
	FormatLog(LOG_APP_LEVEL, "ReqOrderInsert", "[%s] [%f] [%d] [SendReqID:%d] [UserReqID:%d]", req.InstrumentID, req.LimitPrice, req.VolumeTotalOriginal, req.RequestID, m_nRequestId);
}

///请求报单录入(用户下单)
void CTraderSpi::OnRspOrderInsert( CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	m_nRspInsertTime++;
	FormatLog(LOG_APP_LEVEL, "OnRspOrderInsert", "m_nRspInsertTime = %d", m_nRspInsertTime);
	if (IsErrorRspInfo(pRspInfo))
	{
		FormatLog(LOG_ERROR_LEVEL, "OnRspOrderInsert", "Error response errCode:%d, errMsg:%s, InputRequestID:%d, RequestID:%d",
			pRspInfo->ErrorID, pRspInfo->ErrorMsg, pInputOrder->RequestID, nRequestID);
	}
	else
	{
		FormatLog(LOG_APP_LEVEL, "OnRspOrderInsert", "InpReqID:%d, nReqstID:%d", pInputOrder->RequestID, nRequestID);	
	}
}

void CTraderSpi::ReqOrderAction( CThostFtdcInputOrderActionField *pOrder )
{
	int iResult = m_pUserApi->ReqOrderAction(pOrder, ++m_nRequestId);
}

void CTraderSpi::OnRspOrderAction( CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if (pRspInfo && pRspInfo->ErrorID != 0)
	{
		CString strFor;
		strFor.Format("撤单失败:[%d-%s]", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		FormatLog(LOG_ERROR_LEVEL, "OnRspOrderAction", strFor);
	}
	else
	{
		FormatLog(LOG_APP_LEVEL, "OnRspOrderAction", "Rsp success");
	}
}

void CTraderSpi::OnRspError( CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	
}

void CTraderSpi::OnFrontDisconnected( int nReason )
{
	FormatLog(LOG_APP_LEVEL, "OnFrontDisconnected", "Reason: %d", nReason);
	if (m_bLogin)
	{
		m_bLogin = false;
		m_pDlg->PostMessage(WM_MSG_LOGIN_FAILURE, NULL, NULL);
	}
// 	if (m_pUserApi)
// 	{
// 		m_pUserApi->RegisterSpi(NULL);
// 		m_pUserApi->Release();
// 		m_pUserApi = NULL;
// 		m_pDlg->PostMessage(WM_MSG_LOGIN_FAILURE, NULL, NULL);
// 	}
}

void CTraderSpi::OnHeartBeatWarning( int nTimeLapse )
{
	
}

void CTraderSpi::OnRtnOrder( CThostFtdcOrderField *pOrder )
{
// 	FormatLog(LOG_APP_LEVEL, "OnRtnOrder", "[%s][%s][%s][%s][%s] '%c''%c'[%s][%s] %.3f  %d '%c'[%s]'%c' %d  '%c'%.3f '%c''%d'[%s]  %d[%s][%s][%s][%s] "
// 		"[%s][%s] %d '%c' %d  [%s] %d [%s]'%c''%c'  '%c' %d %d [%s][%s]  [%s][%s][%s][%s][%s]  [%s] %d %d %d [%s]  [%s] %d [%s] %d [%s]", 
// 		pOrder->BrokerID, pOrder->InvestorID, pOrder->InstrumentID, pOrder->OrderRef, pOrder->UserID, 
// 		pOrder->OrderPriceType, pOrder->Direction, pOrder->CombOffsetFlag, pOrder->CombHedgeFlag, pOrder->LimitPrice,
// 		pOrder->VolumeTotalOriginal, pOrder->TimeCondition, pOrder->GTDDate, pOrder->VolumeCondition, pOrder->MinVolume, 
// 		pOrder->ContingentCondition, pOrder->StopPrice, pOrder->ForceCloseReason, pOrder->IsAutoSuspend, pOrder->BusinessUnit, 
// 		pOrder->RequestID, pOrder->OrderLocalID, pOrder->ExchangeID, pOrder->ParticipantID, pOrder->ClientID, 
// 		pOrder->ExchangeInstID, pOrder->TraderID, pOrder->InstallID, pOrder->OrderSubmitStatus, pOrder->NotifySequence, 
// 		pOrder->TradingDay, pOrder->SettlementID, pOrder->OrderSysID, pOrder->OrderSource, pOrder->OrderStatus, 
// 		pOrder->OrderType, pOrder->VolumeTraded, pOrder->VolumeTotal, pOrder->InsertDate, pOrder->InsertTime, 
// 		pOrder->ActiveTime, pOrder->SuspendTime, pOrder->UpdateTime, pOrder->CancelTime, pOrder->ActiveTraderID, 
// 		pOrder->ClearingPartID, pOrder->SequenceNo, pOrder->FrontID, pOrder->SessionID, pOrder->UserProductInfo, 
// 		pOrder->StatusMsg, pOrder->UserForceClose, pOrder->ActiveUserID, pOrder->BrokerOrderSeq, pOrder->RelativeOrderSysID);
// 	CThostFtdcOrderField* pNewOrder = new CThostFtdcOrderField;
// 	memcpy(pNewOrder, pOrder, sizeof(CThostFtdcOrderField));
// 	m_pDlg->PostMessage(WM_MSG_RTN_ORDER, (WPARAM)pNewOrder);
}

void CTraderSpi::OnRtnTrade( CThostFtdcTradeField *pTrade )
{
// 	FormatLog(LOG_APP_LEVEL, "OnRtnTrade", "[%s][%s][%s][%s][%s] [%s][%s]'%c'[%s][%s] [%s]'%d'[%s]'%c''%c'  %.3f %d [%s][%s]'%d'"
// 		  "'%d'[%s][%s][%s][%s]  %d [%s] %d %d", 
// 		  pTrade->BrokerID, pTrade->InvestorID, pTrade->InstrumentID, pTrade->OrderRef, pTrade->UserID, 
// 		  pTrade->ExchangeID, pTrade->TradeID, pTrade->Direction, pTrade->OrderSysID, pTrade->ParticipantID, 
// 		  pTrade->ClientID, pTrade->TradingRole, pTrade->ExchangeInstID, pTrade->OffsetFlag, pTrade->HedgeFlag, 
// 		  pTrade->Price, pTrade->Volume, pTrade->TradeDate, pTrade->TradeTime, pTrade->TradeType,
// 		  pTrade->PriceSource, pTrade->TraderID, pTrade->OrderLocalID, pTrade->ClearingPartID, pTrade->BusinessUnit, 
// 		pTrade->SequenceNo, pTrade->TradingDay, pTrade->SettlementID, pTrade->BrokerOrderSeq);
	CThostFtdcTradeField* pNewTrade = new CThostFtdcTradeField;
	memcpy(pNewTrade, pTrade, sizeof(CThostFtdcTradeField));
	m_pDlg->PostMessage(WM_MSG_RTN_TRADE, (WPARAM)pNewTrade);
}


void CTraderSpi::ReqSettlementInfoConfirm()
{
	
}

void CTraderSpi::ReqQryInstrument()
{
	CThostFtdcQryInstrumentField filed;
	memset(&filed, 0, sizeof(CThostFtdcQryInstrumentField));
	//strcpy(filed.InstrumentID, "IF1506");
	m_pUserApi->ReqQryInstrument(&filed, m_nRequestId++);
}

void CTraderSpi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField accout;
	memset(&accout, 0, sizeof(CThostFtdcQryTradingAccountField));
	strcpy(accout.BrokerID, m_pDlg->m_strBrokerID);
	strcpy(accout.InvestorID, m_pDlg->m_strAccountName);
	m_pUserApi->ReqQryTradingAccount(&accout, ++m_nRequestId);
	CString strText;
	strText.Format("m_nRequestId = %d", m_nRequestId);
	FormatLog(LOG_APP_LEVEL, "ReqQryTradingAccount", strText);
	FormatLog(LOG_APP_LEVEL, "RspQryTradingAccount", "BrokerID, AccountID, PreMortgage, PreCredit, PreDeposit, PreBalance, PreMargin, InterestBase, Interest, Deposit, Withdraw, FrozenMargin, FrozenCash, FrozenCommission, CurrMargin, CashIn, Commission, CloseProfit, PositionProfit, Balance, Available, WithdrawQuota, Reserve, TradingDay,SettlementID, Credit, Mortgage, ExchangeMargin, DeliveryMargin, ExchangeDeliveryMargin");
}

void CTraderSpi::ReqQryInvestorPosition()
{
	CThostFtdcQryInvestorPositionField pPosition;
	memset(&pPosition, 0, sizeof(CThostFtdcQryInvestorPositionField));
	strcpy(pPosition.BrokerID, m_pDlg->m_strBrokerID);
	strcpy(pPosition.InvestorID, m_pDlg->m_strAccountName);
	m_pUserApi->ReqQryInvestorPosition(&pPosition, ++m_nRequestId);
	FormatLog(LOG_APP_LEVEL, "ReqQryInvestorPosition", "Send...");
	CString strFormat;
	strFormat = "InstrumentID,BrokerID,InvestorID,PosiDirection,HedgeFlag,PositionDate,YdPosition,Position,LongFrozen,ShortFrozen,LongFrozenAmount,ShortFrozenAmount,OpenVolume,";
	strFormat += "CloseVolume,OpenAmount,CloseAmount,PositionCost,PreMargin,UseMargin,FrozenMargin,FrozenCash,FrozenCommission,CashIn,Commission,CloseProfit,PositionProfit,PreSettlementPrice,";
	strFormat += "SettlementPrice,TradingDay,SettlementID,OpenCost,ExchangeMargin,CombPosition,CombLongFrozen,CombShortFrozen,CloseProfitByDate,CloseProfitByTrade,TodayPosition,MarginRateByMoney,MarginRateByVolume";
	FormatLog(LOG_APP_LEVEL, "ReqQryInvestorPosition", strFormat);
}

bool CTraderSpi::IsErrorRspInfo( CThostFtdcRspInfoField *pRspInfo )
{
	return ((pRspInfo) && (pRspInfo->ErrorID != 0));
}

bool CTraderSpi::IsMyOrder( CThostFtdcOrderField *pOrder )
{
	return ((pOrder->FrontID == m_frontID) &&
		(pOrder->SessionID == m_sessionID) &&
			(strcmp(pOrder->OrderRef, m_orderRef) == 0));
}

bool CTraderSpi::IsTradingOrder( CThostFtdcOrderField *pOrder )
{
	return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}

void CTraderSpi::FormatLog( int nLevel, CString strDomain, const char* paramFmt,... )
{
	char szParam[1024+1] = {0} ;
	va_list args;
	va_start(args,paramFmt);
	//sprintf(szParam,paramFmt,args);
	_vsnprintf(szParam,1024,paramFmt,args);
	va_end(args);

	CString strLog, strTmp;
	SYSTEMTIME nowTime;
	GetLocalTime(&nowTime);
	strLog.Format("%d-%02d-%02d %02d:%02d:%02d %03d   ", nowTime.wYear, nowTime.wMonth, nowTime.wDay, nowTime.wHour, nowTime.wMinute, nowTime.wSecond, nowTime.wMilliseconds);
	switch(nLevel)
	{
	case LOG_APP_LEVEL:
		strTmp.Format("CTraderSpi::%s\t [Info] %s", strDomain, szParam);
		break;
	case LOG_DEBUG_LEVEL:
		strTmp.Format("CTraderSpi::%s\t [Debug] %s", strDomain, szParam);
		break;
	case LOG_WARN_LEVEL:
		strTmp.Format("CTraderSpi::%s\t [Warn] %s", strDomain, szParam);
		break;
	case LOG_ERROR_LEVEL:
		strTmp.Format("CTraderSpi::%s\t [Err] %s", strDomain, szParam);
		break;
	}
	if (m_pLog)
	{
		strLog += strTmp;
		fprintf(m_pLog, "%s\n", strLog);
		fflush(m_pLog);
	}
}

void CTraderSpi::Init( FILE* pLog, char* szAddress )
{
	m_pLog = pLog;
	if (m_pUserApi == NULL)
	{
		m_pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
		m_pUserApi->RegisterSpi(this);
		m_pUserApi->RegisterFront(szAddress);
 		m_pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);
 		m_pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);
		m_pUserApi->Init();
	}
}

void CTraderSpi::ReqQryInvestorPositionDetail(CThostFtdcQryInvestorPositionDetailField *pQryInvestorPositionDetail)
{
	FormatLog(LOG_APP_LEVEL, "OnRspQryInvestorPositionDetail", "InstrumentID, BrokerID, InvestorID,HedgeFlag,Direction,OpenDate,TradeID,Volume,OpenPrice,TradingDay,SettlementID,TradeType,CombInstrumentID,ExchangeID,CloseProfitByDate,CloseProfitByTrade,PositionProfitByDate,PositionProfitByTrade,Margin,ExchMargin,MarginRateByMoney,MarginRateByVolume,LastSettlementPrice,SettlementPrice,CloseVolume,CloseAmount");
	m_pUserApi->ReqQryInvestorPositionDetail(pQryInvestorPositionDetail, ++m_nRequestId);
}

void CTraderSpi::OnRspQryInvestorPositionDetail( CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if (pRspInfo && pRspInfo->ErrorID != 0)
	{
		FormatLog(LOG_ERROR_LEVEL, "OnRspQryInvestorPositionDetail", pRspInfo->ErrorMsg);
	}
	else if (pInvestorPositionDetail)
	{
		
		CString strFormat;
		strFormat.Format("%s, %s, %s, %c, %c, %s, %s, %d, %f, %s, %d, %c, %s, %s, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %d, %f",
			pInvestorPositionDetail->InstrumentID, pInvestorPositionDetail->BrokerID, pInvestorPositionDetail->InvestorID, pInvestorPositionDetail->HedgeFlag, pInvestorPositionDetail->Direction,
			pInvestorPositionDetail->OpenDate, pInvestorPositionDetail->TradeID, pInvestorPositionDetail->Volume, pInvestorPositionDetail->OpenPrice, pInvestorPositionDetail->TradingDay,
			pInvestorPositionDetail->SettlementID, pInvestorPositionDetail->TradeType, pInvestorPositionDetail->CombInstrumentID, pInvestorPositionDetail->ExchangeID, pInvestorPositionDetail->CloseProfitByDate,
			pInvestorPositionDetail->CloseProfitByTrade, pInvestorPositionDetail->PositionProfitByDate, pInvestorPositionDetail->PositionProfitByTrade, pInvestorPositionDetail->Margin,
			pInvestorPositionDetail->ExchMargin, pInvestorPositionDetail->MarginRateByMoney, pInvestorPositionDetail->MarginRateByVolume, pInvestorPositionDetail->LastSettlementPrice, pInvestorPositionDetail->SettlementPrice, 
			pInvestorPositionDetail->CloseVolume, pInvestorPositionDetail->CloseAmount);
		FormatLog(LOG_APP_LEVEL, "OnRspQryInvestorPositionDetail", strFormat);
	}
	if (bIsLast)
	{
		FormatLog(LOG_APP_LEVEL, "OnRspQryInvestorPositionDetail", "Query done");
	}
}

void CTraderSpi::ReqQryOrder( CThostFtdcQryOrderField *pQryOrder )
{
	int iResult = m_pUserApi->ReqQryOrder(pQryOrder, ++m_nRequestId);
	if (iResult != 0)
	{
		FormatLog(LOG_APP_LEVEL, "ReqQryOrder", "查询报单失败");
	}
}

void CTraderSpi::OnRspQryOrder( CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	CString strText;
	if (pRspInfo && pRspInfo->ErrorID != 0)
	{
		strText.Format("ErrorID:%d, ErrMsg:%s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		FormatLog(LOG_APP_LEVEL, "RspOrder", strText);
	}
	else
	{
		if (pOrder)
		{
			strText.Format("[%s] [%s] %s %s %.2lf, ALL:%d, TD:%d, UnTD:%d, BrokerOrderSeq:%d", pOrder->OrderSysID, pOrder->InstrumentID, 
				pOrder->Direction == THOST_FTDC_D_Buy ? "买":"卖",
				pOrder->CombOffsetFlag[0]==THOST_FTDC_OF_Open ? "开仓":"平仓",
				pOrder->LimitPrice, pOrder->VolumeTotalOriginal, pOrder->VolumeTraded, pOrder->VolumeTotal, pOrder->BrokerOrderSeq);
			FormatLog(LOG_APP_LEVEL, "OnRspQryOrder", strText);
		}
		if (bIsLast)
		{
			FormatLog(LOG_APP_LEVEL, "OnRspQryOrder", "查询报单结束");
		}
		/*
		if (pOrder->OrderStatus == '1' || pOrder->OrderStatus == '3' || pOrder->OrderStatus == 'b')
		{
			CThostFtdcOrderField* pNewOrder = new CThostFtdcOrderField;
			memcpy(pNewOrder, pOrder, sizeof(CThostFtdcOrderField));
			m_pDlg->AddNewUntradeOrder(pNewOrder, false);
		}
		if (bIsLast)
		{
			m_pDlg->AddNewUntradeOrder(NULL, true);
		}
		*/
	}
}

void CTraderSpi::ReqQryTrade( CThostFtdcQryTradeField *pQryTrade )
{
	int iResult = m_pUserApi->ReqQryTrade(pQryTrade, ++m_nRequestId);
	if (iResult != 0)
	{
		FormatLog(LOG_APP_LEVEL, "ReqQryTrade", "查询成交失败");
	}
}


void CTraderSpi::OnRspQryTrade( CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	CString strText;
	if (pRspInfo && pRspInfo->ErrorID != 0)
	{
		strText.Format("ErrorID:%d, ErrMsg:%s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		FormatLog(LOG_APP_LEVEL, "RspQryTrade", strText);
	}
	else
	{
		if (pTrade)
		{
			strText.Format("[%s] [%s] %s %s %.2lf, %d, Seq:%d", pTrade->OrderSysID, pTrade->InstrumentID, 
				pTrade->Direction == THOST_FTDC_D_Buy ? "买":"卖",
				pTrade->OffsetFlag==THOST_FTDC_OF_Open ? "开仓":"平仓",
				pTrade->Price, pTrade->Volume, pTrade->BrokerOrderSeq);
			FormatLog(LOG_APP_LEVEL, "RspQryTrade", strText);
		}
		if (bIsLast)
		{
			FormatLog(LOG_APP_LEVEL, "RspQryTrade", "查询成交结束");
		}
	}
}

void CTraderSpi::Start()
{
	m_bRunning = true;
	ReqOrderInsert(m_pDlg->m_strInstrument.GetBuffer(m_pDlg->m_strInstrument.GetLength()), m_pDlg->m_dPrice, THOST_FTDC_OF_Open, THOST_FTDC_D_Buy, m_pDlg->m_nVolumn);
}

void CTraderSpi::Stop()
{
	m_bRunning = false;
}

void CTraderSpi::OnRspQryInvestor( CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if (pRspInfo && pRspInfo->ErrorID != 0)
	{
		FormatLog(LOG_APP_LEVEL, "OnRspQryInvestor", "查询投资者信息失败!");
	}
	else
	{
		CString strText;
		strText.Format("Code:%s, Group:%s, Name:%s, CardID:%s, Phone:%s, addr:%s", 
			pInvestor->InvestorID, pInvestor->InvestorGroupID, pInvestor->InvestorName, 
			pInvestor->IdentifiedCardNo, pInvestor->Telephone, pInvestor->Address);
		FormatLog(LOG_APP_LEVEL, "OnRspQryInvestor",  strText);
	}
}


int CTraderSpi::ReqQryInstrumentMarginRate( CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate )
{
	int iResult = m_pUserApi->ReqQryInstrumentMarginRate(pQryInstrumentMarginRate, ++m_nRequestId);
	if (iResult != 0)
	{
		FormatLog(LOG_APP_LEVEL, "ReqQryMargin", "请求查询合约保证金率失败!");
	}
	return 0;
}

int CTraderSpi::ReqQryInstrumentCommissionRate( CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate )
{
	int iResult = m_pUserApi->ReqQryInstrumentCommissionRate(pQryInstrumentCommissionRate, ++m_nRequestId);
	if (iResult != 0)
	{
		FormatLog(LOG_APP_LEVEL, "ReqQryComm", "请求查询合约手续费率失败!");
	}
	return 0;
}

void CTraderSpi::OnRspQryInstrumentMarginRate( CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	CString strText;
	if (pRspInfo && pRspInfo->ErrorID != 0)
	{
		strText.Format("ErrID:%d, ErrMsg:%s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		FormatLog(LOG_ERROR_LEVEL, "InsMarginRate", strText);
	}
	else if (pInstrumentMarginRate)
	{
		strText.Format("[%s] [%s] [%s] [longMoney:%f] [longVol:%f] [shortMoney:%f] [shortVol:%f]",
			pInstrumentMarginRate->InstrumentID,pInstrumentMarginRate->BrokerID,pInstrumentMarginRate->InvestorID, pInstrumentMarginRate->LongMarginRatioByMoney, pInstrumentMarginRate->LongMarginRatioByVolume,
			pInstrumentMarginRate->ShortMarginRatioByMoney, pInstrumentMarginRate->ShortMarginRatioByVolume);
		FormatLog(LOG_APP_LEVEL, "InsMarginRate", strText);

		CThostFtdcInstrumentMarginRateField *pMarginRate = new CThostFtdcInstrumentMarginRateField;
		memcpy(pMarginRate, pInstrumentMarginRate, sizeof(CThostFtdcInstrumentMarginRateField));
		m_pDlg->PostMessage(WM_MSG_RSPQRYINSTRUMENTMARGINRATE, (WPARAM)pMarginRate);
	}
}


void CTraderSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CString strText;
	if (pRspInfo && pRspInfo->ErrorID != 0)
	{
		strText.Format("ErrID:%d, ErrMsg:%s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		FormatLog(LOG_ERROR_LEVEL, "InsCommRate", strText);
	}
	else if(pInstrumentCommissionRate)
	{
		strText.Format("[%s] [%s] [%s] [openMoney:%f] [openVol:%f] [closeMoney:%f] [closevol:%f]",
			pInstrumentCommissionRate->InstrumentID,pInstrumentCommissionRate->BrokerID,pInstrumentCommissionRate->InvestorID, pInstrumentCommissionRate->OpenRatioByMoney, pInstrumentCommissionRate->OpenRatioByVolume,
			pInstrumentCommissionRate->CloseRatioByMoney, pInstrumentCommissionRate->CloseRatioByVolume);
		FormatLog(LOG_APP_LEVEL, "InsCommRate", strText);

		CThostFtdcInstrumentCommissionRateField* pCommRate = new CThostFtdcInstrumentCommissionRateField;
		memcpy(pCommRate, pInstrumentCommissionRate, sizeof(CThostFtdcInstrumentCommissionRateField));
		m_pDlg->PostMessage(WM_MSG_RSPQRYINSTRUMENTCOMMIRATE, (WPARAM)pCommRate);
		if (bIsLast)
		{
			FormatLog(LOG_APP_LEVEL, "InsCommRate", "查询手续费率结束");
		}
	}
	else
	{
		FormatLog(LOG_ERROR_LEVEL, "InsCommRate", "查询手续费率失败!!!");
	}
}


int CTraderSpi::ReqQryDepthMarketData( CThostFtdcQryDepthMarketDataField *pQryDepthMarketData )
{
	FormatLog(LOG_APP_LEVEL, "ReqDepthMarket", pQryDepthMarketData->InstrumentID);
	int iResult = m_pUserApi->ReqQryDepthMarketData(pQryDepthMarketData, ++m_nRequestId);
	if (iResult != 0)
	{
		FormatLog(LOG_APP_LEVEL, "ReqDepthMarket", "请求查询合约行情失败!");
	}
	return iResult;
}

void CTraderSpi::OnRspQryDepthMarketData( CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if (pRspInfo && pRspInfo->ErrorID != 0)
	{
		FormatLog(LOG_APP_LEVEL, "RspDepthMarket", "返回行情数据错误");
	}
	else
	{
		CString strText;
		strText.Format("Ins:[%s], lastPrice:.2l%f", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice);
		FormatLog(LOG_APP_LEVEL, "RspDepthMarket", strText);
	}
}

int CTraderSpi::ReqUserPasswordUpdate( CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate )
{
	int nResult = m_pUserApi->ReqUserPasswordUpdate(pUserPasswordUpdate, ++m_nRequestId);
	if (nResult != 0)
	{
		FormatLog(LOG_APP_LEVEL, "PasswordUpdate", "请求更新密码失败!");
	}
	return 0;
}

void CTraderSpi::OnRspUserPasswordUpdate( CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	CString strFormat;
	if (pRspInfo && pRspInfo->ErrorID != 0)
	{
		strFormat.Format("请求更新密码失败! ErrorID:%d, ErrorMsg:%s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		FormatLog(LOG_APP_LEVEL, "OnRspUserPasswordUpdate", strFormat);
	}
	else if (pUserPasswordUpdate)
	{
		strFormat.Format("User:%s, Old:%s, New:%s", pUserPasswordUpdate->UserID, pUserPasswordUpdate->OldPassword, pUserPasswordUpdate->NewPassword);
		FormatLog(LOG_APP_LEVEL, "OnRspUserPasswordUpdate", strFormat);
	}
}

void CTraderSpi::ReqUserLogout()
{
	CThostFtdcUserLogoutField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_pDlg->m_strBrokerID);
	strcpy(req.UserID, m_pDlg->m_strAccountName);
	int iResult = m_pUserApi->ReqUserLogout(&req, ++m_nRequestId);
	if (iResult == 0)
	{
		FormatLog(LOG_APP_LEVEL, "ReqUserLogout", "登陆请求成功");
	}
	else
	{
		FormatLog(LOG_ERROR_LEVEL, "ReqUserLogout", "请求发送失败");
		m_pDlg->PostMessage(WM_MSG_LOGOUT_FAILURE);
	}
}

void CTraderSpi::OnRspUserLogout( CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if (!pRspInfo || (pRspInfo && pRspInfo->ErrorID == 0))
	{
		FormatLog(LOG_APP_LEVEL, "RspLogout", "Logout success");
		m_pDlg->PostMessage(WM_MSG_LOGOUT_SUCCESS);
	}
	else
	{
		FormatLog(LOG_ERROR_LEVEL, "RspLogout", "Logout Error");
	}
}

int CTraderSpi::ReqQryInvestorPositionCombineDetail( CThostFtdcQryInvestorPositionCombineDetailField *pQryInvestorPositionCombineDetail )
{
	FormatLog(LOG_APP_LEVEL, "OnRspQryInvestorPositionCombineDetail", "BrokerID,CombInstrumentID,ComTradeID,Direction,ExchangeID,ExchMargin,HedgeFlag,InstrumentID,InvestorID,LegID,LegMultiple,Margin,MarginRateByMoney,MarginRateByVolume,OpenDate,SettlementID,TotalAmt,TradeID,TradingDay");
	int nResult = m_pUserApi->ReqQryInvestorPositionCombineDetail(pQryInvestorPositionCombineDetail, ++m_nRequestId);
	if (nResult != 0)
	{
		FormatLog(LOG_APP_LEVEL, "PositionCombineDetail", "请求查询组合持仓失败!");
	}
	return nResult;
}

void CTraderSpi::OnRspQryInvestorPositionCombineDetail( CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	CString strFormat;
	if (pRspInfo && pRspInfo->ErrorID != 0)
	{
		FormatLog(LOG_APP_LEVEL, "OnRspQryInvestorPositionCombineDetail", "查询组合持仓失败! ErrorID:%d, ErrorMsg:%s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else if (pInvestorPositionCombineDetail == NULL)
	{
		FormatLog(LOG_APP_LEVEL, "OnRspQryInvestorPositionCombineDetail", "没有记录.");
	}
	else
	{
		FormatLog(LOG_APP_LEVEL, "OnRspQryInvestorPositionCombineDetail", "%s,%s,%s,%c,%s,%f,%c,%s,%s,%d,%d,%f,%f,%f,%s,%d,%d,%s,%s", 
			pInvestorPositionCombineDetail->BrokerID, pInvestorPositionCombineDetail->CombInstrumentID, pInvestorPositionCombineDetail->ComTradeID,
			pInvestorPositionCombineDetail->Direction, pInvestorPositionCombineDetail->ExchangeID, pInvestorPositionCombineDetail->ExchMargin,
			pInvestorPositionCombineDetail->HedgeFlag, pInvestorPositionCombineDetail->InstrumentID, pInvestorPositionCombineDetail->InvestorID, pInvestorPositionCombineDetail->LegID,
			pInvestorPositionCombineDetail->LegMultiple, pInvestorPositionCombineDetail->Margin, pInvestorPositionCombineDetail->MarginRateByMoney, pInvestorPositionCombineDetail->MarginRateByVolume,
			pInvestorPositionCombineDetail->OpenDate, pInvestorPositionCombineDetail->SettlementID, pInvestorPositionCombineDetail->TotalAmt, pInvestorPositionCombineDetail->TradeID,
			pInvestorPositionCombineDetail->TradingDay);
	}
}

void CTraderSpi::OnRtnInstrumentStatus( CThostFtdcInstrumentStatusField *pInstrumentStatus )
{
	FormatLog(LOG_APP_LEVEL,"OnRtnInstrumentStatus", "[%s] [%s] [%s] [%s] '%c' %d [%s] '%c'", 
		pInstrumentStatus->ExchangeID, pInstrumentStatus->ExchangeInstID, pInstrumentStatus->SettlementGroupID,
		pInstrumentStatus->InstrumentID, pInstrumentStatus->InstrumentStatus, pInstrumentStatus->TradingSegmentSN,
		pInstrumentStatus->EnterTime, pInstrumentStatus->EnterReason);
}

void CTraderSpi::OnRtnFromBankToFutureByFuture( CThostFtdcRspTransferField *pRspTransfer )
{
	ReqQryTradingAccount();
}

void CTraderSpi::OnRtnFromFutureToBankByFuture( CThostFtdcRspTransferField *pRspTransfer )
{
	ReqQryTradingAccount();
}




