#include "HttpGetNetBagList.h"
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>

#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"
#include "logging.hpp"
#include "httpStructData.h"

HttpGetNetBagList::HttpGetNetBagList(RequestPageType type, QString sBarId /*= appConfigIni::getBarId()*/)
{
	m_pageType = type;
	setReqUrlAddress(commonFun::getRequestUrl() + c_SHttpGetNetBagListAction);
	QString allParam = QString("barId=%1&userId=%2").arg(sBarId).arg(appConfigIni::getUserId());
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpGetNetBagList::~HttpGetNetBagList()
{

}

void HttpGetNetBagList::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		Log::Info("HttpGetNetBagList 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	Log::Info("HttpGetNetBagList %s", strResult.toStdString().c_str());

	QList<DiscountPkg_t> discountPkgList;
	if (sc.property("data").isArray())
	{
		QScriptValueIterator it(sc.property("data"));
		while (it.hasNext())
		{
			it.next();
			DiscountPkg_t discountPkg;
			discountPkg.sId = it.value().property("id").toString();
			if (discountPkg.sId.isEmpty())
			{
				continue;
			}
			discountPkg.dOriginalPrice = it.value().property("coupPrice").toString().toDouble();
			discountPkgList.append(discountPkg);
		}
	}

	if (m_pageType == t_rechargePage)
	{
		HttpSignal::instance()->sigOnGetDiscountPkgForRechargePage(nCode, sMsg, discountPkgList);
	}
	else if (m_pageType = t_personalCentrePage)
	{
		HttpSignal::instance()->sigOnGetDiscountPkgForPersongalCentrePage(nCode, sMsg, discountPkgList);
	}
	this->deleteLater();
}

void HttpGetNetBagList::processError()
{
	QList<DiscountPkg_t> discountPkgList;
	if (m_pageType == t_rechargePage)
	{
		HttpSignal::instance()->sigOnGetDiscountPkgForRechargePage(e_connectServerError, c_sNetError, discountPkgList);
	}
	else if (m_pageType = t_personalCentrePage)
	{
		HttpSignal::instance()->sigOnGetDiscountPkgForPersongalCentrePage(e_connectServerError, c_sNetError, discountPkgList);
	}
	this->deleteLater();
}
