#include "HttpNetFeeRecharge.h"
#include <QScriptEngine>
#include <QScriptValue>

#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"
#include "logging.hpp"

HttpNetFeeRecharge::HttpNetFeeRecharge(LoginUserInfo_t userInfo, DiscountPkg_t discountPkg, QString sBarId /*= appConfigIni::getBarId()*/)
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpNetFeeRechargeAction);
	QString allParam = QString("barId=%1&userId=%2&idCard=%3&unitPrice=%4&bagFee=%5&seatNum=%6&type=%7").arg(sBarId).arg(userInfo.sUserId).arg(userInfo.sIdCard).arg(discountPkg.dOriginalPrice).arg(discountPkg.dOriginalPrice).arg(commonFun::getLocalmachineName()).arg(discountPkg.sId);
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpNetFeeRecharge::~HttpNetFeeRecharge()
{

}

void HttpNetFeeRecharge::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		Log::Info("HttpNetFeeRecharge 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	Log::Info("HttpNetFeeRecharge %s", strResult.toStdString().c_str());

	QString codeData = sc.property("data").toString();
	HttpSignal::instance()->sigOnNetFeeRecharge(nCode, sMsg, codeData);
	this->deleteLater();
}

void HttpNetFeeRecharge::processError()
{
	HttpSignal::instance()->sigOnNetFeeRecharge(e_connectServerError, c_sNetError, "");
	this->deleteLater();
}
