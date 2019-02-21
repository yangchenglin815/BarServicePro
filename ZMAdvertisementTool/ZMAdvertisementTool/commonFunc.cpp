#include "commonFunc.h"
#include "enumData.h"
#include "constData.h"
#include "appConfigIni.h"

QString commonFunc::getRequestUrl()
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