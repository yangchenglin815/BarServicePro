#include "HttpGetGoodsInfo.h"
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>

#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"
#include "logging.hpp"
#include "appConfigIni.h"


HttpGetGoodsInfo::HttpGetGoodsInfo(QString sName, int nPage)
{
	m_page = nPage;
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpGetOrderGoodsListAction);
	QString allParam = QString("barId=%1&name=%2").arg(appConfigIni::getBarId()).arg(sName);
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpGetGoodsInfo::~HttpGetGoodsInfo()
{

}

void HttpGetGoodsInfo::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		Log::Info("HttpGetGoodsInfo 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	Log::Info(" HttpGetGoodsInfo %s", strResult.toStdString().c_str());
	QList<GoodsInfo_t>goodsInfoList;
	if (sc.property("data").isArray())
	{
		QScriptValueIterator it(sc.property("data"));
		GoodsInfo_t goodsInfo;
		while (it.hasNext())
		{
			it.next();
			goodsInfo.nId = it.value().property("id").toString();
			if (goodsInfo.nId.isEmpty())
			{
				continue;
			}
			goodsInfo.sName = it.value().property("name").toString();
			goodsInfo.nType = it.value().property("type").toInt32();
			goodsInfo.dPrice = it.value().property("price").toString().toDouble();
			goodsInfo.nClassifyId = it.value().property("classify").toString();
			goodsInfo.sCode = it.value().property("code").toString();
			goodsInfo.sUnit = it.value().property("unit").toString();
			goodsInfo.sImgUrl = it.value().property("imgUrl").toString();
			goodsInfo.nStockNum = it.value().property("barInventory").toInt32();

			QList<TasteGroupInfo_t> tasteGroupList;
			if (it.value().property("tastegroups").isArray())
			{
				QScriptValueIterator it_(it.value().property("tastegroups"));
				TasteGroupInfo_t tasteGroupInfo;
				while (it_.hasNext())
				{
					it_.next();
					tasteGroupInfo.id = it_.value().property("id").toString();
					if (tasteGroupInfo.id.isEmpty())
					{
						continue;
					}
					tasteGroupInfo.sName = it_.value().property("name").toString();
					tasteGroupInfo.sOpt = it_.value().property("opts").toString();
					tasteGroupList.append(tasteGroupInfo);
				}
			}
			goodsInfo.nTasteGroupList = tasteGroupList;
			goodsInfoList.append(goodsInfo);
		}
	}

	if (m_page == t_orderPage)
	{
		HttpSignal::instance()->sigOnGetGoodsInfo(nCode, sMsg, goodsInfoList);
	}
	this->deleteLater();
}

void HttpGetGoodsInfo::processError()
{
	QList<GoodsInfo_t> goodsInfoList;
	if (m_page == t_orderPage)
	{
		HttpSignal::instance()->sigOnGetGoodsInfo(e_connectServerError, c_sNetError, goodsInfoList);
	}
	this->deleteLater();
}
