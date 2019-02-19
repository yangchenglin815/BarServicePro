#include "commonFun.h"
#include "configIni.h"
#include "enumData.h"
#include "constData.h"

QString commonFun::getRequestUrl()
{
	QString sUrl;
	int nType = configIni::getServerType().toInt();
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
	case t_server:
		sUrl = c_sServerUrl;
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
