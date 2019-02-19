#include "HttpReportPCInfo.h"
#include <QScriptEngine>
#include <QScriptValue>

#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"
#include "logging.hpp"

HttpReportPCInfo::HttpReportPCInfo(const ReportPCInfo_t &info)
{
	m_info = info;
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpReportPCInfoAction);
	QString allParam = QString("barId=%1&pcIp=%2&pcName=%3&mac=%4").arg(m_info.nBarId).arg(m_info.sIp).arg(m_info.sPCName).arg(m_info.sMac);
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpReportPCInfo::~HttpReportPCInfo()
{

}

void HttpReportPCInfo::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		Log::Info(QStringLiteral("HttpGetBarInfo 返回参数json格式错误").toStdString().c_str());
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	Log::Info("HttpReportPCInfo : %s", strResult.toStdString().c_str());
	HttpSignal::instance()->sigOnReportPCInfoResult(nCode, sMsg);
	this->deleteLater();
}

void HttpReportPCInfo::processError()
{
	HttpSignal::instance()->sigOnReportPCInfoResult(e_connectServerError, c_sNetError);
	this->deleteLater();
}
