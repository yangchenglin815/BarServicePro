#include "HttpGetSignInList.h"
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>

#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"
#include "logging.hpp"

HttpGetSignInList::HttpGetSignInList(LoginUserInfo userInfo, QString sBarId /*= appConfigIni::getBarId()*/)
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpGetSignInListAction);
	QString allParam = QString("barId=%1&userId=%2&idCard=%3").arg(sBarId).arg(userInfo.sUserId).arg(userInfo.sIdCard);
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpGetSignInList::~HttpGetSignInList()
{

}

void HttpGetSignInList::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		Log::Info("HttpGetSignInList 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	Log::Info("HttpGetSignInList %s", strResult.toStdString().c_str());

	QList<QString> signInDataList;
	if (sc.property("data").isArray())
	{
		QScriptValueIterator it(sc.property("data"));
		while (it.hasNext())
		{
			it.next();
			QString data = it.value().property("signDay").toString();
			if (data.isEmpty())
			{
				continue;
			}
			signInDataList.append(data);
		}
	}
	HttpSignal::instance()->sigOnGetSignInList(nCode, sMsg, signInDataList);
	this->deleteLater();
}

void HttpGetSignInList::processError()
{
	QList<QString> signInDataList;
	HttpSignal::instance()->sigOnGetSignInList(e_connectServerError, c_sNetError, signInDataList);
	this->deleteLater();
}
