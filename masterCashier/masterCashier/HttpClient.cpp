#include "HttpClient.h"
#include "HttpGetBarInfo.h"
#include "HttpLogin.h"
#include "HttpLogout.h"
#include "HttpGetClassifyGroup.h"
#include "HttpGetGoodsInfo.h"
#include "HttpDealPayment.h"
#include "HttpGetAllGoodsInfo.h"
#include "HttpGoodsCheckUp.h"
#include "HttpGetShiftInfo.h"
#include "HttpCommitShiftInfo.h"
#include "HttpCheckAcceptor.h"
#include "HttpGetWarningGoodsList.h"
#include "HttpRefund.h"
#include "HttpCancelOrder.h"
#include "HttpGetOrderDetialList.h"
#include "HttpGetOrderList.h"
#include "HttpFaceDetection.h"
#include "HttpCommitFaceData.h"

HttpClient* HttpClient::m_pInstance = NULL;

HttpClient *HttpClient::instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new HttpClient;
	}
	return m_pInstance;
}

HttpClient::HttpClient(QObject *parent)
	: QObject(parent)
{
	m_pManager = new QNetworkAccessManager();
}

HttpClient::~HttpClient()
{

}

void HttpClient::setToken(QString sToken)
{
	m_sToken = sToken;
}

void HttpClient::RequestOnGetBarInfo(QString nBarId)
{
	HttpGetBarInfo *p = new HttpGetBarInfo(nBarId);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnLogin(QString nUser, QString nPasswd, int nType)
{
	HttpLogin *p = new HttpLogin(nUser, nPasswd, nType);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnLogout(int nType)
{
	HttpLogout *p = new HttpLogout(nType);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnGetClassifyGroups(QString nBarId, int nPage)
{
	HttpGetClassifyGroup *p = new HttpGetClassifyGroup(nBarId, nPage);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnGetGoodsInfo(QString nBarId, QString sName, int nPage)
{
	HttpGetGoodsInfo *p = new HttpGetGoodsInfo(nBarId, sName, nPage);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnGetAllGoodsInfo()
{
	HttpGetAllGoodsInfo *p = new HttpGetAllGoodsInfo();
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnDealPayment(PaymentOrder orderInfo)
{
	HttpDealPayment *p = new HttpDealPayment(orderInfo);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnGoodsCheckUp(int stockType, QList<GoodsCheckUpInfo_t> goodsCheckUpList, QString mark)
{
	HttpGoodsCheckUp *p = new HttpGoodsCheckUp(stockType, goodsCheckUpList, mark);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnGetShiftInfo(QString nEndTime)
{
	HttpGetShiftInfo *p = new HttpGetShiftInfo(nEndTime);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnCommitShiftInfo(ShiftInfo_t tShiftInfo, QString sUser, QString sMark)
{
	HttpCommitShiftInfo *p = new HttpCommitShiftInfo(tShiftInfo, sUser, sMark);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnCheckAcceptor(QString sPhone, QString sPwd)
{
	HttpCheckAcceptor *p = new HttpCheckAcceptor(sPhone, sPwd);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnGetWarningGoodsList(QString sBarId)
{
	HttpGetWarningGoodsList *p = new HttpGetWarningGoodsList(sBarId);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnRefund(QString sBarId, QString sOrderId, QString sRemark)
{
	HttpRefund *p = new HttpRefund(sBarId, sOrderId, sRemark);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnCancelOrder(QString sBarId, QString sOrderId, QString sPasswd)
{
	HttpCancelOrder *p = new HttpCancelOrder(sBarId, sOrderId, sPasswd);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnGetOrderDetialList(QString sSerialId)
{
	HttpGetOrderDetialList *p = new HttpGetOrderDetialList(sSerialId);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnGetOrderList()
{
	HttpGetOrderList *p = new HttpGetOrderList();
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnFaceDetection(QString filePath)
{
	HttpFaceDetection *p = new HttpFaceDetection(filePath);
	p->setNetManager(m_pManager);
	p->postFaceTection();
}

void HttpClient::RequestOnCommitFaceData(QString sIdCard, QString sName)
{
	HttpCommitFaceData *p = new HttpCommitFaceData();
	p->setNetManager(m_pManager);
	p->postFaceData(sIdCard, sName);
}
