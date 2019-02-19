#include "commonFun.h"
#include "constData.h"
#include "enumData.h"
#include "setip.h"

string commonFun::getRequestUrl()
{
	string sUrl;
	int nType = std::stoi(ConfigFileRead("type"));
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