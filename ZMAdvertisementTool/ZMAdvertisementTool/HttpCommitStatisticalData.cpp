#include "HttpCommitStatisticalData.h"
#include <QScriptEngine>
#include <QScriptValue>

#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFunc.h"
#include "logging.hpp"

HttpCommitStatisticalData::HttpCommitStatisticalData(int delayTime, QString mac, QString computerName)
{
	setReqUrlAddress(commonFunc::getRequestUrl() + c_sHttpCommitStatisticalDataAction);
	QByteArray array = createJsonData(delayTime, mac, computerName);;
	setReqParams(array);
}

void HttpCommitStatisticalData::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("msg").toString();
	if (!sc.property("code").isValid())
	{
		Log::Info("HttpCommitStatisticalData 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	HttpSignal::instance()->sigOnCommitStatisticalData(nCode, sMsg);
	this->deleteLater();
}

void HttpCommitStatisticalData::processError()
{
	HttpSignal::instance()->sigOnCommitStatisticalData(e_connectServerError, c_sNetError);
	this->deleteLater();
}

QByteArray HttpCommitStatisticalData::createJsonData(int delayTime, QString mac, QString computerName)
{
	QByteArray byte_array;
	QJsonObject jsonObject;
	jsonObject.insert("macid", mac);
	jsonObject.insert("computerName", computerName);
	jsonObject.insert("delayTime", delayTime);

	QJsonDocument document;
	document.setObject(jsonObject);
	byte_array = document.toJson(QJsonDocument::Compact);
	return byte_array;
}
