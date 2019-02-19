#include "HttpGetClassifyGroup.h"
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>

#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"
#include "logging.hpp"
#include "appConfigIni.h"

HttpGetClassifyGroup::HttpGetClassifyGroup(int nPage)
{
	m_page = nPage;
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpGetOrderClassifyGroupAction);
	QString allParam = QString("barId=%1").arg(appConfigIni::getBarId());
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpGetClassifyGroup::~HttpGetClassifyGroup()
{

}

void HttpGetClassifyGroup::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		Log::Info("HttpGetClassifyGroup 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	Log::Info("HttpGetClassifyGroup %s", strResult.toStdString().c_str());
	QList<ClassifyGroupInfo_t> classifyGroupList;
	if (sc.property("data").isArray())
	{
		QScriptValueIterator it(sc.property("data"));
		ClassifyGroupInfo_t classifyGroupInfo;
		while (it.hasNext())
		{
			it.next();
			classifyGroupInfo.id = it.value().property("id").toString();
			if (classifyGroupInfo.id.isEmpty())
			{
				continue;
			}
			classifyGroupInfo.sName = it.value().property("name").toString();
			classifyGroupInfo.sort = it.value().property("sort").toInt32();
			classifyGroupList.append(classifyGroupInfo);
		}
	}

	HttpSignal::instance()->sigOnGetClassifyGroups(nCode, sMsg, classifyGroupList, m_page);
	this->deleteLater();
}

void HttpGetClassifyGroup::processError()
{
	QList<ClassifyGroupInfo_t> classifyGroupList;
	HttpSignal::instance()->sigOnGetClassifyGroups(e_connectServerError, c_sNetError, classifyGroupList, m_page);
	this->deleteLater();
}
