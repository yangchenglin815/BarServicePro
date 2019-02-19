#include "commonFun.h"
#include "appConfigIni.h"
#include "enumData.h"
#include "constData.h"

QString commonFun::getRequestUrl()
{
	QString sUrl;
	int nType = appConfigIni::getServerType().toInt();
	switch (nType)
	{
	case t_formalServer:
		sUrl = c_sFormalServerUrl;
		break;
	case t_testServer:
		sUrl = c_sTestServerUrl;
		break;
	case t_localServer:
		sUrl = c_sLocalServerUrl;
		break;
	default:
		break;
	}
	return sUrl;
}

bool commonFun::getSearchResult(QString searchText, QString goodsName, QString goodsCode)
{
	if (goodsName.contains(searchText))
	{
		return true;
	}

	QStringList list = goodsCode.split(",");
	if (list.size() > 0)
	{
		for (int i = 0; i < list.size(); ++i)
		{
			if (list[i] == searchText)
			{
				return true;
			}
		}
	}
	return false;
}

QString commonFun::getPayWay(int index)
{
	QString payWay;
	switch (index)
	{
	case t_wxPay:
		payWay = QStringLiteral("微信支付");
		break;
	case t_aliPay:
		payWay = QStringLiteral("支付宝支付");
		break;
	case t_cashPay:
		payWay = QStringLiteral("现金支付");
		break;
	case t_yyPay:
		payWay = QStringLiteral("幽游钱包支付");
		break;
	default:
		break;
	}
	return payWay;
}

QString commonFun::getImageFileName(QString filePath)
{
	QString fileName;
	QStringList list = filePath.split("/");
	if (list.size() > 0)
	{
		fileName = list.at(list.size() - 1);
	}
	return fileName;
}
