#include "HttpGoodsCheckUp.h"
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


HttpGoodsCheckUp::HttpGoodsCheckUp(int stockType, const QList<GoodsCheckUpInfo_t> &goodsCheckUpInfoList, QString mark)
{
	m_goodsCheckUpInfoList = goodsCheckUpInfoList;
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpOrderGoodsCheckUpAction);
	QString orderGoodsInfoListJson = createGoodsGroupjson(m_goodsCheckUpInfoList);
	QString allParam = QString("target=%1&datas=%2&mark=%3").arg(stockType).arg(orderGoodsInfoListJson).arg(mark);
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpGoodsCheckUp::~HttpGoodsCheckUp()
{

}

void HttpGoodsCheckUp::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		//AppConfigHandler.Info(QStringLiteral("HttpGoodsCheckUp 返回参数json格式错误").toStdString());
		Log::Info("HttpGoodsCheckUp 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		//AppConfigHandler.Info(this->getTestUrl().toStdString());
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	//AppConfigHandler.Info("HttpGoodsCheckUp" + strResult.toStdString());
	Log::Info("HttpGoodsCheckUp %s", strResult.toStdString().c_str());
	HttpSignal::instance()->sigOnGetGoodsCheckUpResult(nCode, sMsg);
	this->deleteLater();
}

void HttpGoodsCheckUp::processError()
{
	HttpSignal::instance()->sigOnGetGoodsCheckUpResult(e_connectServerError, c_sNetError);
	this->deleteLater();
}

QString HttpGoodsCheckUp::createGoodsGroupjson(const QList<GoodsCheckUpInfo_t> &goodsCheckUpInfoList)
{
	QJsonArray jsonArray;
	int nLen = goodsCheckUpInfoList.length();
	GoodsCheckUpInfo_t pGoodsInfo;
	for (int i = 0; i < nLen; i++)
	{
		pGoodsInfo = goodsCheckUpInfoList[i];
		QJsonObject dataJson;
		dataJson.insert("goodsId", pGoodsInfo.nGoodsId);
		dataJson.insert("mainInventory", pGoodsInfo.nMainStock);
		dataJson.insert("barInventory", pGoodsInfo.nBarStock);
		dataJson.insert("price", pGoodsInfo.dPrice);
		dataJson.insert("checkInventory", pGoodsInfo.nCheckStock);
		jsonArray.insert(i, dataJson); //
	}

	QJsonDocument document;
	document.setArray(jsonArray);
	QByteArray byte_array = document.toJson(QJsonDocument::Compact);
	return QString(byte_array);
}
