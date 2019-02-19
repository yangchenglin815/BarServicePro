#include "HttpGetWarningGoodsList.h"
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

HttpGetWarningGoodsList::HttpGetWarningGoodsList(QString sBarId)
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpOrderGetWarningGoodsListAction);
	QString allParam = QString("barId=%1").arg(sBarId);
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpGetWarningGoodsList::~HttpGetWarningGoodsList()
{

}

void HttpGetWarningGoodsList::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		//AppConfigHandler.Info(QStringLiteral("HttpGetWarningGoodsList 返回参数json格式错误").toStdString());
		Log::Info("HttpGetWarningGoodsList 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		//AppConfigHandler.Info(this->getTestUrl().toStdString());
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	//AppConfigHandler.Info("HttpGetWarningGoodsList" + strResult.toStdString());
	Log::Info("HttpGetWarningGoodsList %s", strResult.toStdString().c_str());

	QList<WarningGoods_t> lWarningGoodsList;
	if (sc.property("data").isArray())
	{
		QScriptValueIterator it(sc.property("data"));
		WarningGoods_t goodsInfo;
		while (it.hasNext())
		{
			it.next();
			goodsInfo.sGoodsId = it.value().property("id").toString();
			if (goodsInfo.sGoodsId.isEmpty())
			{
				continue;
			}
			goodsInfo.sGoodsName = it.value().property("name").toString();
			goodsInfo.dPrice = it.value().property("price").toString().toDouble();
			goodsInfo.nInventory = it.value().property("barInventory").toInt32();
			lWarningGoodsList.append(goodsInfo);
		}
	}
	HttpSignal::instance()->sigOnGetWarningGoodsList(nCode, sMsg, lWarningGoodsList);
	this->deleteLater();
}

void HttpGetWarningGoodsList::processError()
{
	QList<WarningGoods_t> lWarningGoodsList;
	HttpSignal::instance()->sigOnGetWarningGoodsList(e_connectServerError, c_sNetError, lWarningGoodsList);
	this->deleteLater();
}
