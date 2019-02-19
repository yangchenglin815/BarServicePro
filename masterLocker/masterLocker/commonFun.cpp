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

QString commonFun::getBookingErrMsg(int nCode)
{
	QString sMsg;
	switch (nCode)
	{
	case e_bookingExpired:
		sMsg = c_sBookingExpiredErr;
		break;
	case e_bookingCodeErr:
		sMsg = c_sBookingCodeErr;
		break;
	case e_bookingUnlocked:
		sMsg = c_sBookingUnlockedErr;
		break;
	default:
		break;
	}
	return sMsg;
}
