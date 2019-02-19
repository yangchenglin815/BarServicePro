#include "HttpGetOrderDetialList.h"
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

HttpGetOrderDetialList::HttpGetOrderDetialList(QString sSerialId)
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpOrderGetOrderDetialAction);
	QString allParam = QString("id=%1&barId=%2").arg(sSerialId).arg(appConfigIni::getBarId());
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpGetOrderDetialList::~HttpGetOrderDetialList()
{

}

void HttpGetOrderDetialList::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		//AppConfigHandler.Info(QStringLiteral("HttpGetOrderDetialList 返回参数json格式错误").toStdString());
		Log::Info("HttpGetOrderDetialList 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		//AppConfigHandler.Info(this->getTestUrl().toStdString());
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	//AppConfigHandler.Info("HttpGetOrderDetialList" + strResult.toStdString());
	Log::Info("HttpGetOrderDetialList %s", strResult.toStdString().c_str());
	QList<OrderDetialInfo_t> orderDetialList;
	if (sc.property("data").property("detailList").isArray())
	{
		QScriptValueIterator it(sc.property("data").property("detailList"));
		OrderDetialInfo_t orderDetialInfo;
		while (it.hasNext())
		{
			it.next();
			orderDetialInfo.sGoodsId = it.value().property("goodsId").toString();
			if (orderDetialInfo.sGoodsId.isEmpty())
			{
				continue;
			}
			orderDetialInfo.sGoodsName = it.value().property("name").toString();
			orderDetialInfo.nOrderNum = it.value().property("num").toInt32();
			orderDetialInfo.dPrice = it.value().property("price").toString().toDouble();
			orderDetialList.append(orderDetialInfo);
		}
	}

	HttpSignal::instance()->sigOnGetOrderDetialList(nCode, sMsg, orderDetialList);
	this->deleteLater();
}

void HttpGetOrderDetialList::processError()
{
	QList<OrderDetialInfo_t> orderDetialList;
	HttpSignal::instance()->sigOnGetOrderDetialList(e_connectServerError, c_sNetError, orderDetialList);
	this->deleteLater();
}
