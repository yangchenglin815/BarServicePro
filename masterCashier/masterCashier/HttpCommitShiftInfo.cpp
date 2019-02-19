#include "HttpCommitShiftInfo.h"
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

HttpCommitShiftInfo::HttpCommitShiftInfo(const ShiftInfo_t &tShiftInfo, QString sUser, QString sMark)
{
	m_shiftInfo = tShiftInfo;
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpOrderCommitShiftInfoAction);
	QString salesObject = createSalesInfoJson(m_shiftInfo);
	QString goodsObject = createGoodsInfoJson(m_shiftInfo);
	QString othersObject = createOthersInfoJson(m_shiftInfo);
	QString allParam = QString("sales=%1&goods=%2&others=%3&user=%4&mark=%5&endTime=%6").arg(salesObject).arg(goodsObject).arg(othersObject).arg(sUser).arg(sMark).arg(m_shiftInfo.nEndTime);
	if (!tShiftInfo.nParentId.isEmpty())
	{
		allParam.append(QString("&parentId=%1").arg(tShiftInfo.nParentId));
	}

	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpCommitShiftInfo::~HttpCommitShiftInfo()
{

}

void HttpCommitShiftInfo::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		//AppConfigHandler.Info(QStringLiteral("HttpCommitShiftInfo 返回参数json格式错误").toStdString());
		Log::Info("HttpCommitShiftInfo 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		//AppConfigHandler.Info(this->getTestUrl().toStdString());
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}
	//AppConfigHandler.Info("HttpCommitShiftInfo" + strResult.toStdString());
	Log::Info("HttpCommitShiftInfo %s", strResult.toStdString().c_str());

	HttpSignal::instance()->sigOnCommitShiftInfoResult(nCode, sMsg);
	this->deleteLater();
}

void HttpCommitShiftInfo::processError()
{
	HttpSignal::instance()->sigOnCommitShiftInfoResult(e_connectServerError, c_sNetError);
	this->deleteLater();
}

QString HttpCommitShiftInfo::createSalesInfoJson(const ShiftInfo_t &info)
{
	QJsonObject dataJson;
	dataJson.insert("cashIn", info.dCashSale);
	dataJson.insert("cashOut", info.dCashReturn); 
	dataJson.insert("orderTotal", info.dCodeSale); 
	dataJson.insert("total", info.dTotalSale); 
	dataJson.insert("cashTotal", info.dCash);

	QJsonDocument document;
	document.setObject(dataJson);
	QByteArray byte_array = document.toJson(QJsonDocument::Compact);
	QString byte_str = QString::fromUtf8(byte_array);

	return byte_str;
}

QString HttpCommitShiftInfo::createGoodsInfoJson(const ShiftInfo_t &info)
{
	QJsonObject dataJson;
	dataJson.insert("operationTotal", info.dOperationTotal);
	dataJson.insert("operationIn", info.dOperationIn);
	dataJson.insert("operationOut", info.dOperationOut);
	dataJson.insert("checkTotal", info.dCheckTotal);
	dataJson.insert("checkLoss", info.dCheckLoss);
	dataJson.insert("checkProfit", info.dCheckProfit);

	QJsonDocument document;
	document.setObject(dataJson);
	QByteArray byte_array = document.toJson(QJsonDocument::Compact);
	QString byte_str = QString::fromUtf8(byte_array);

	return byte_str;
}

QString HttpCommitShiftInfo::createOthersInfoJson(const ShiftInfo_t &info)
{
	QJsonObject dataJson;
	dataJson.insert("otherIn", info.dOtherIn);
	dataJson.insert("otherOut", info.dOtherOut);
	dataJson.insert("cashIn", info.dCashIn);
	dataJson.insert("cashOut", info.dCashOut);
	dataJson.insert("cashTotal", info.dCashTotal);

	QJsonDocument document;
	document.setObject(dataJson);
	QByteArray byte_array = document.toJson(QJsonDocument::Compact);
	QString byte_str = QString::fromUtf8(byte_array);

	return byte_str;
}
