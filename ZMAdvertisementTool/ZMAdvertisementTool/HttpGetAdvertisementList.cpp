#include "HttpGetAdvertisementList.h"
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>

#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFunc.h"
#include "appConfigIni.h"
#include "HttpStructData.h"
#include "logging.hpp"

HttpGetAdvertisementList::HttpGetAdvertisementList()
{
	setReqUrlAddress(commonFunc::getRequestUrl() + c_sHttpGetAdvertisementListAction);
}

void HttpGetAdvertisementList::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("msg").toString();
	if (!sc.property("code").isValid())
	{
		Log::Info("HttpGetAdvertisementList 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	QList<AdvertisementInfo_t> advertisementInfoList;
	if (sc.property("datas").isArray()) {
		QScriptValueIterator it(sc.property("datas"));
		AdvertisementInfo_t advertisementInfo;
		while (it.hasNext()) {
			it.next();
			advertisementInfo.sId = it.value().property("id").toString();
			if (advertisementInfo.sId.isEmpty()) {
				continue;
			}
			advertisementInfo.sTitle = it.value().property("title").toString();
			advertisementInfo.sDetial = it.value().property("description").toString();
			advertisementInfo.sImage = it.value().property("indexImg").toString();
			advertisementInfo.sJumpUrl = it.value().property("jumpUrl").toString();
			advertisementInfo.nDelayTime = it.value().property("delayTime").toInt32();
			advertisementInfoList.append(advertisementInfo);
		}
	}
	HttpSignal::instance()->sigOnGetAdvertisementList(advertisementInfoList, nCode, sMsg);
	this->deleteLater();
}

void HttpGetAdvertisementList::processError()
{
	QList<AdvertisementInfo_t> advertisementInfoList;
	HttpSignal::instance()->sigOnGetAdvertisementList(advertisementInfoList, e_connectServerError, c_sNetError);
	this->deleteLater();
}
