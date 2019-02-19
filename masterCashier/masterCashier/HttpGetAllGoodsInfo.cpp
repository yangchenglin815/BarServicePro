#include "HttpGetAllGoodsInfo.h"
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

HttpGetAllGoodsInfo::HttpGetAllGoodsInfo()
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpGetOrderAllGoodsListAction);
}

HttpGetAllGoodsInfo::~HttpGetAllGoodsInfo()
{

}

void HttpGetAllGoodsInfo::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		//AppConfigHandler.Info(QStringLiteral("HttpGetAllGoodsInfo 返回参数json格式错误").toStdString());
		Log::Info("HttpGetAllGoodsInfo 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		//AppConfigHandler.Info(this->getTestUrl().toStdString());
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	//AppConfigHandler.Info("HttpGetAllGoodsInfo" + strResult.toStdString());
	Log::Info("HttpGetAllGoodsInfo %s", strResult.toStdString().c_str());
	QList<GoodsInfo_t>goodsInfoList;
	if (sc.property("data").isArray())
	{
		QScriptValueIterator it(sc.property("data"));
		GoodsInfo_t goodsInfo;
		while (it.hasNext())
		{
			it.next();
			goodsInfo.nId = it.value().property("id").toString();
			if (goodsInfo.nId.isEmpty())
			{
				continue;
			}
			goodsInfo.sName = it.value().property("name").toString();
			goodsInfo.nType = it.value().property("type").toInt32();
			goodsInfo.dPrice = it.value().property("price").toString().toDouble();
			goodsInfo.nClassifyId = it.value().property("classify").toString();
			goodsInfo.sCode = it.value().property("code").toString();
			goodsInfo.sUnit = it.value().property("unit").toString();
			goodsInfo.sImgUrl = it.value().property("imgUrl").toString();
			goodsInfo.nStockNum = it.value().property("barInventory").toInt32();
			goodsInfo.nMainStockNum = it.value().property("mainInventory").toInt32();
			QList<TasteGroupInfo_t> tasteGroupList;
			goodsInfo.nTasteGroupList = tasteGroupList;
			goodsInfoList.append(goodsInfo);
		}
	}
	HttpSignal::instance()->sigOnGetAllGoodsInfo(nCode, sMsg, goodsInfoList);
	this->deleteLater();
}

void HttpGetAllGoodsInfo::processError()
{
	QList<GoodsInfo_t> goodsInfoList;
	HttpSignal::instance()->sigOnGetAllGoodsInfo(e_connectServerError, c_sNetError, goodsInfoList);
	this->deleteLater();
}
