#include "HttpDealPayment.h"
#include <QScriptEngine>
#include <QScriptValue>

#include "appconfig.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"
#include "logging.hpp"

using namespace dm::server;

HttpDealPayment::HttpDealPayment(const PaymentOrder &paymentOrder)
{
	m_paymentOrder = paymentOrder;
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpOrderGoodsDetialAction);
	QString orderGoodsInfoListJson = createOrderGoodsListJson(paymentOrder.orderGoodsList);
	QString allParam = QString("payType=%1&barId=%2&goodsList=%3&factPrice=%4&changePrice=%5&remark=%6&authCode=%7&orderFrom=%8").arg(m_paymentOrder.nPayType).arg(m_paymentOrder.nBarId).arg(orderGoodsInfoListJson).arg(m_paymentOrder.sFactPay).arg(m_paymentOrder.sChange).arg(m_paymentOrder.sRemark).arg(m_paymentOrder.sAuthCode).arg("2");
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpDealPayment::~HttpDealPayment()
{

}

void HttpDealPayment::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		//AppConfigHandler.Info(QStringLiteral("HttpDealPayment 返回参数json格式错误").toStdString());
		Log::Info("HttpDealPayment 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		//AppConfigHandler.Info(this->getTestUrl().toStdString());
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	//AppConfigHandler.Info("HttpDealPayment" + strResult.toStdString());
	Log::Info("HttpDealPayment %s", strResult.toStdString().c_str());
	QString orderId = sc.property("data").property("id").toString();
	int payStatus = sc.property("data").property("payStatus").toInt32();
	HttpSignal::instance()->sigOnGetOrderResult(nCode, sMsg, orderId, payStatus);
	this->deleteLater();
}

void HttpDealPayment::processError()
{
	HttpSignal::instance()->sigOnGetOrderResult(e_connectServerError, c_sNetError, "", -1);
	this->deleteLater();
}

QString HttpDealPayment::createOrderGoodsListJson(const QList<OrderGoods_t> &orderGoodsInfoList)
{
	QJsonArray jsonArray;
	int nLen = orderGoodsInfoList.length();
	OrderGoods_t pOrderGoodsInfo;
	for (int i = 0; i < nLen; i++)
	{
		pOrderGoodsInfo = orderGoodsInfoList[i];
		QJsonObject dataJson;
		dataJson.insert("goodsId", pOrderGoodsInfo.nGoodsId);
		dataJson.insert("num", pOrderGoodsInfo.nNum);
		dataJson.insert("type", pOrderGoodsInfo.nType);
		dataJson.insert("tasteGroups", createTasteGroupjson(pOrderGoodsInfo.nTasteGroupList));
		dataJson.insert("price", pOrderGoodsInfo.sPrice);
		jsonArray.insert(i, dataJson); //
	}

	QJsonDocument document;
	document.setArray(jsonArray);
	QByteArray byte_array = document.toJson(QJsonDocument::Compact);
	return QString(byte_array);
}

QString HttpDealPayment::createTasteGroupjson(const QList<TasteGroupInfo_t> &tasteGroupList)
{
	QStringList list;
	for (int i = 0; i < tasteGroupList.size(); ++i){
		list.append(tasteGroupList[i].sOpt);
	}
	return list.join("/");
}
