#include "HttpGetOrderList.h"
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>

#include "appconfig.h"
#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"
#include "logging.hpp"

using namespace dm::server;

HttpGetOrderList::HttpGetOrderList()
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpOrderGetOrderListAction);
	QString allParam = QString("pageNum=0&pageSize=0&isNotPay=0&barId=%1").arg(appConfigIni::getBarId());
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpGetOrderList::~HttpGetOrderList()
{

}

void HttpGetOrderList::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		//AppConfigHandler.Info(QStringLiteral("HttpGetOrderList 返回参数json格式错误").toStdString());
		Log::Info("HttpGetOrderList 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		//AppConfigHandler.Info(this->getTestUrl().toStdString());
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	//AppConfigHandler.Info("HttpGetOrderList" + strResult.toStdString());
	Log::Info("HttpGetOrderList %s", strResult.toStdString().c_str());
	QList<OrderInfo_t> orderInfoList;
	if (sc.property("data").property("list").isArray())
	{
		QScriptValueIterator it(sc.property("data").property("list"));
		OrderInfo_t orderInfo;
		while (it.hasNext())
		{
			it.next();
			orderInfo.sOrderId = it.value().property("id").toString();
			if (orderInfo.sOrderId.isEmpty())
			{
				continue;
			}
			orderInfo.sOrderNum = it.value().property("orderNum").toString();
			orderInfo.sCreateTime = it.value().property("createTime").toString();
			orderInfo.sFinishTime = it.value().property("finishTime").toString();
			orderInfo.nTradeType = it.value().property("tradeType").toInt32();
			orderInfo.nPayStatus = it.value().property("payStatus").toInt32();
			orderInfo.nPayType = it.value().property("payType").toInt32();
			orderInfo.sRemark = it.value().property("remark").toString();
			orderInfo.dSum = it.value().property("price").toString().toDouble();
			orderInfo.seatNo = it.value().property("seatNum").toString();

			orderInfoList.append(orderInfo);
		}
	}
	HttpSignal::instance()->sigOnGetOrderList(nCode, sMsg, orderInfoList);
	this->deleteLater();
}

void HttpGetOrderList::processError()
{
	QList<OrderInfo_t> orderInfoList;
	HttpSignal::instance()->sigOnGetOrderList(e_connectServerError, c_sNetError, orderInfoList);
	this->deleteLater();
}
