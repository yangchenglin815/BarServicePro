#include "HttpGetShiftInfo.h"
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

HttpGetShiftInfo::HttpGetShiftInfo(QString endTime)
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpOrderGetShiftInfoAction);
	QString allParam = QString("endTime=%1").arg(endTime);
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpGetShiftInfo::~HttpGetShiftInfo()
{

}

void HttpGetShiftInfo::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		//AppConfigHandler.Info(QStringLiteral("HttpGetShiftInfo 返回参数json格式错误").toStdString());
		Log::Info("HttpGetShiftInfo 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		//AppConfigHandler.Info(this->getTestUrl().toStdString());
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}
	//AppConfigHandler.Info("HttpGetShiftInfo" + strResult.toStdString());
	Log::Info("HttpGetShiftInfo %s", strResult.toStdString().c_str());

	ShiftInfo shiftInfo;
	shiftInfo.dCashSale = sc.property("data").property("sales").property("cashIn").toString().toDouble();
	shiftInfo.dCashReturn = sc.property("data").property("sales").property("cashOut").toString().toDouble();
	shiftInfo.dCodeSale = sc.property("data").property("sales").property("orderTotal").toString().toDouble();
	shiftInfo.dTotalSale = sc.property("data").property("sales").property("total").toString().toDouble();
	shiftInfo.dCash = sc.property("data").property("sales").property("cashTotal").toString().toDouble();

	shiftInfo.dOperationTotal = sc.property("data").property("goods").property("operationTotal").toString().toDouble();
	shiftInfo.dOperationIn = sc.property("data").property("goods").property("operationIn").toString().toDouble();
	shiftInfo.dOperationOut = sc.property("data").property("goods").property("operationOut").toString().toDouble();
	shiftInfo.dCheckTotal = sc.property("data").property("goods").property("checkTotal").toString().toDouble();
	shiftInfo.dCheckLoss = sc.property("data").property("goods").property("checkLoss").toString().toDouble();
	shiftInfo.dCheckProfit = sc.property("data").property("goods").property("checkProfit").toString().toDouble();

	shiftInfo.dOtherIn = sc.property("data").property("others").property("otherIn").toString().toDouble();
	shiftInfo.dOtherOut = sc.property("data").property("others").property("otherOut").toString().toDouble();
	shiftInfo.dCashIn = sc.property("data").property("others").property("cashIn").toString().toDouble();
	shiftInfo.dCashOut = sc.property("data").property("others").property("cashOut").toString().toDouble();
	shiftInfo.dCashTotal = sc.property("data").property("others").property("cashTotal").toString().toDouble();

	shiftInfo.nEndTime = sc.property("data").property("endTime").toString();
	shiftInfo.nParentId = sc.property("data").property("parentId").toString();

	QList<ShifterInfo_t> shifterList;
	if (sc.property("data").property("users").isArray())
	{
		QScriptValueIterator it(sc.property("data").property("users"));
		ShifterInfo_t shifterInfo;
		while (it.hasNext())
		{
			it.next();
			shifterInfo.nShiftId = it.value().property("id").toString();
			if (shifterInfo.nShiftId.isEmpty())
			{
				continue;
			}
			shifterInfo.nUserName = it.value().property("name").toString();
			shifterInfo.nAccount = it.value().property("phone").toString();
			shifterList.append(shifterInfo);
		}
	}
	shiftInfo.nShifterList = shifterList;

	HttpSignal::instance()->sigOnGetShiftInfo(nCode, sMsg, shiftInfo);
	this->deleteLater();
}

void HttpGetShiftInfo::processError()
{
	ShiftInfo_t shiftInfo;
	HttpSignal::instance()->sigOnGetShiftInfo(e_connectServerError, c_sNetError, shiftInfo);
	this->deleteLater();
}
