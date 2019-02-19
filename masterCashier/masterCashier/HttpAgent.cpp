#include "HttpAgent.h"
#include "ThreadManager.h"
#include "HttpClient.h"

HttpAgent* HttpAgent::m_pInstance = NULL;

HttpAgent *HttpAgent::instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new HttpAgent;
	}
	return m_pInstance;
}

void HttpAgent::RequestOnGetBarInfo(QString nBarId)
{
	HttpClient::instance()->RequestOnGetBarInfo(nBarId);
}

void HttpAgent::RequestOnLogin(QString nUser, QString nPasswd, int nType)
{
	HttpClient::instance()->RequestOnLogin(nUser, nPasswd, nType);
}

void HttpAgent::RequestOnLogout(int nType)
{
	HttpClient::instance()->RequestOnLogout(nType);
}

void HttpAgent::RequestOnGetClassifyGroups(QString nBarId, int nPage)
{
	HttpClient::instance()->RequestOnGetClassifyGroups(nBarId, nPage);
}

void HttpAgent::RequestOnGetGoodsInfo(QString nBarId, QString sName, int nPage)
{
	HttpClient::instance()->RequestOnGetGoodsInfo(nBarId, sName, nPage);
}

void HttpAgent::RequestOnGetAllGoodsInfo()
{
	HttpClient::instance()->RequestOnGetAllGoodsInfo();
}

void HttpAgent::RequestOnDealPayment(PaymentOrder orderInfo)
{
	HttpClient::instance()->RequestOnDealPayment(orderInfo);
}

void HttpAgent::RequestOnGoodsCheckUp(int stockType, QList<GoodsCheckUpInfo_t> goodsCheckUpList, QString mark)
{
	HttpClient::instance()->RequestOnGoodsCheckUp(stockType, goodsCheckUpList, mark);
}

void HttpAgent::RequestOnGetShiftInfo(QString nEndTime)
{
	HttpClient::instance()->RequestOnGetShiftInfo(nEndTime);
}

void HttpAgent::RequestOnCommitShiftInfo(ShiftInfo_t tShiftInfo, QString sUser, QString sMark)
{
	HttpClient::instance()->RequestOnCommitShiftInfo(tShiftInfo, sUser, sMark);
}

void HttpAgent::RequestOnCheckAcceptor(QString sPhone, QString sPwd)
{
	HttpClient::instance()->RequestOnCheckAcceptor(sPhone, sPwd);
}

void HttpAgent::RequestOnGetWarningGoodsList(QString sBarId)
{
	HttpClient::instance()->RequestOnGetWarningGoodsList(sBarId);
}

void HttpAgent::RequestOnRefund(QString sBarId, QString sOrderId, QString sRemark)
{
	HttpClient::instance()->RequestOnRefund(sBarId, sOrderId, sRemark);
}

void HttpAgent::RequestOnCancelOrder(QString sBarId, QString sOrderId, QString sPasswd)
{
	HttpClient::instance()->RequestOnCancelOrder(sBarId, sOrderId, sPasswd);
}

void HttpAgent::RequestOnGetOrderDetialList(QString sSerialId)
{
	HttpClient::instance()->RequestOnGetOrderDetialList(sSerialId);
}

void HttpAgent::RequestOnGetOrderList()
{
	HttpClient::instance()->RequestOnGetOrderList();
}

void HttpAgent::RequestOnFaceDetection(QString filePath)
{
	HttpClient::instance()->RequestOnFaceDetection(filePath);
}

void HttpAgent::RequestOnCommitFaceData(QString sIdCard, QString sName)
{
	HttpClient::instance()->RequestOnCommitFaceData(sIdCard, sName);
}

HttpAgent::HttpAgent(QObject *parent)
	: QObject(parent)
{
	initConnect();
	moveToThread(ThreadManager::getAgentThread());
	ThreadManager::getAgentThread()->start();
}

HttpAgent::~HttpAgent()
{

}

void HttpAgent::initConnect()
{
	connect(HttpSignal::instance(), SIGNAL(sigOnGetBarInfo(int, QString, QString)), this, SIGNAL(sigOnGetBarInfo(int, QString, QString)));
	connect(HttpSignal::instance(), SIGNAL(sigOnLogin(int, QString, QString, QString)), this, SIGNAL(sigOnLogin(int, QString, QString, QString)));
	connect(HttpSignal::instance(), SIGNAL(sigOnLogout(int, QString)), this, SIGNAL(sigOnLogout(int, QString)));
	connect(HttpSignal::instance(), SIGNAL(sigOnShiftLogout(int, QString)), this, SIGNAL(sigOnShiftLogout(int, QString)));
	connect(HttpSignal::instance(), SIGNAL(sigOnSwitch(int, QString, QString, QString)), this, SIGNAL(sigOnSwitch(int, QString, QString, QString)));
	connect(HttpSignal::instance(), SIGNAL(sigOnShiftLogin(int, QString, QString, QString)), this, SIGNAL(sigOnShiftLogin(int, QString, QString, QString)));
	connect(HttpSignal::instance(), SIGNAL(sigOnGetOrderResult(int, QString, QString, int)), SIGNAL(sigOnGetOrderResult(int, QString, QString, int)));
	connect(HttpSignal::instance(), SIGNAL(sigOnGetGoodsCheckUpResult(int, QString)), this, SIGNAL(sigOnGoodsCheckUpResult(int, QString)));
	connect(HttpSignal::instance(), SIGNAL(sigOnCommitShiftInfoResult(int, QString)), this, SIGNAL(sigOnCommitShiftInfoResult(int, QString)));
	connect(HttpSignal::instance(), SIGNAL(sigOnCheckAcceptorResult(int, QString)), this, SIGNAL(sigOnCheckAcceptorResult(int, QString)));
	connect(HttpSignal::instance(), SIGNAL(sigOnGetRefundRespond(int, QString)), this, SIGNAL(sigOnGetRefundRespond(int, QString)));
	connect(HttpSignal::instance(), SIGNAL(sigOnGetCancelOrderRespond(int, QString)), this, SIGNAL(sigOnGetCancelOrderRespond(int, QString)));
	connect(HttpSignal::instance(), SIGNAL(sigOnGetFaceDetectionRespond(int, QString, int)), this, SIGNAL(sigOnGetFaceDetectionRespond(int, QString, int)));
	connect(HttpSignal::instance(), SIGNAL(sigOnGetCommitFaceDataRespond(int, QString, int)), this, SIGNAL(sigOnGetCommitFaceDataRespond(int, QString, int)));

	qRegisterMetaType<QList<ClassifyGroupInfo_t> >("QList<ClassifyGroupInfo_t>");
	connect(HttpSignal::instance(), SIGNAL(sigOnGetClassifyGroups(int, QString, QList<ClassifyGroupInfo_t>, int)), this, SIGNAL(sigOnGetClassifyGroups(int, QString, QList<ClassifyGroupInfo_t>, int)));

	qRegisterMetaType<QList<GoodsInfo_t> >("QList<GoodsInfo_t>");
	connect(HttpSignal::instance(), SIGNAL(sigOnGetGoodsInfo(int, QString, QList<GoodsInfo_t>)), this, SIGNAL(sigOnGetGoodsInfo(int, QString, QList<GoodsInfo_t>)));
	connect(HttpSignal::instance(), SIGNAL(sigOnGetReturnGoodsInfo(int, QString, QList<GoodsInfo_t>)), this, SIGNAL(sigOnGetReturnGoodsInfo(int, QString, QList<GoodsInfo_t>)));
	connect(HttpSignal::instance(), SIGNAL(sigOnGetAllGoodsInfo(int, QString, QList<GoodsInfo_t>)), this, SIGNAL(sigOnGetAllGoodsInfo(int, QString, QList<GoodsInfo_t>)));

	qRegisterMetaType<ShiftInfo_t>("ShiftInfo_t");
	connect(HttpSignal::instance(), SIGNAL(sigOnGetShiftInfo(int, QString, ShiftInfo_t)), this, SIGNAL(sigOnGetShiftInfo(int, QString, ShiftInfo_t)));

	qRegisterMetaType<QList<WarningGoods_t> >("QList<WarningGoods_t>");
	connect(HttpSignal::instance(), SIGNAL(sigOnGetWarningGoodsList(int, QString, QList<WarningGoods_t>)), this, SIGNAL(sigOnGetWarningGoodsList(int, QString, QList<WarningGoods_t>)));

	qRegisterMetaType<QList<OrderDetialInfo_t> >("QList<OrderDetialInfo_t>");
	connect(HttpSignal::instance(), SIGNAL(sigOnGetOrderDetialList(int, QString, QList<OrderDetialInfo_t>)), this, SIGNAL(sigOnGetOrderDetialList(int, QString, QList<OrderDetialInfo_t>)));

	qRegisterMetaType<QList<OrderInfo_t> >("QList<OrderInfo_t>");
	connect(HttpSignal::instance(), SIGNAL(sigOnGetOrderList(int, QString, QList<OrderInfo_t>)), this, SIGNAL(sigOnGetOrderList(int, QString, QList<OrderInfo_t>)));
}
