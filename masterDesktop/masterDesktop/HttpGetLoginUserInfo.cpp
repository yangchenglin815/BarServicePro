#include "HttpGetLoginUserInfo.h"
#include <QScriptEngine>
#include <QScriptValue>

#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"
#include "logging.hpp"


HttpGetLoginUserInfo::HttpGetLoginUserInfo(QString sMac, QString sBarId /*= appConfigIni::getBarId()*/)
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpGetLoginUserInfoAction);
	QString allParam = QString("barId=%1&mac=%2").arg(sBarId).arg(sMac);
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpGetLoginUserInfo::~HttpGetLoginUserInfo()
{

}

void HttpGetLoginUserInfo::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		Log::Info("HttpGetBarInfo 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	Log::Info("HttpGetBarInfo %s", strResult.toStdString().c_str());

	LoginUserInfo loginUserInfo;
	loginUserInfo.sName = sc.property("data").property("realName").toString();
	loginUserInfo.sPhone = sc.property("data").property("phone").toString();
	loginUserInfo.sIdCard = sc.property("data").property("idCard").toString();
	loginUserInfo.sUserId = sc.property("data").property("userId").toString();
	loginUserInfo.dNetFee = sc.property("data").property("netMoney").toString().toDouble();
	loginUserInfo.dWalletFee = sc.property("data").property("walletMoney").toString().toDouble();
	HttpSignal::instance()->sigOnGetLoginUserInfo(nCode, sMsg, loginUserInfo);
	this->deleteLater();
}

void HttpGetLoginUserInfo::processError()
{
	LoginUserInfo loginUserInfo;
	HttpSignal::instance()->sigOnGetLoginUserInfo(e_connectServerError, c_sNetError, loginUserInfo);
	this->deleteLater();
}
