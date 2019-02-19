#include "HttpOperMainInventory.h"
#include <QScriptEngine>
#include <QScriptValue>

#include "appconfig.h"
#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"

using namespace dm::server;

HttpOperMainInventory::HttpOperMainInventory(int nType, QString sRemarks, QList<ReturnGoodsInfo_t> lReturnGoodsInfoList)
{
	QString goodsArray = createReturnGoodsListJson(lReturnGoodsInfoList);
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpOperateMainInventoryAction);
	QString allParam = QString("type=%1&mark=%2&datas=%3").arg(nType).arg(sRemarks).arg(goodsArray);
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpOperMainInventory::~HttpOperMainInventory()
{

}

void HttpOperMainInventory::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		AppConfigHandler.Info(QStringLiteral("HttpOperMainInventory 返回参数json格式错误").toStdString());
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		AppConfigHandler.Info(this->getTestUrl().toStdString());
	}

	AppConfigHandler.Info("HttpOperMainInventory" + strResult.toStdString());
	HttpSignal::instance()->sigOnRespondMainInventory(nCode, sMsg);
	this->deleteLater();
}

void HttpOperMainInventory::processError()
{
	HttpSignal::instance()->sigOnRespondMainInventory(e_connectServerError, c_sNetError);
	this->deleteLater();
}

QString HttpOperMainInventory::createReturnGoodsListJson(const QList<ReturnGoodsInfo_t> &returnGoodsInfoList)
{
	QJsonArray jsonArray;
	int nLen = returnGoodsInfoList.length();
	ReturnGoodsInfo_t pGoodsInfo;
	for (int i = 0; i < nLen; i++)
	{
		pGoodsInfo = returnGoodsInfoList[i];
		QJsonObject dataJson;
		dataJson.insert("goodsId", pGoodsInfo.sGoodsId);
		dataJson.insert("operInventory", pGoodsInfo.nNum);
		dataJson.insert("operPrice", pGoodsInfo.dPrice);
		jsonArray.insert(i, dataJson); //
	}

	QJsonDocument document;
	document.setArray(jsonArray);
	QByteArray byte_array = document.toJson(QJsonDocument::Compact);
	return QString(byte_array);
}
