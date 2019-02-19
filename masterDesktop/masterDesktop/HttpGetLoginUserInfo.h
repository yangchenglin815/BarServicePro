#ifndef HTTPGETLOGINUSERINFO_H
#define HTTPGETLOGINUSERINFO_H

#include <QObject>
#include "IHttpReqRes.h"
#include "appConfigini.h"

class HttpGetLoginUserInfo : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpGetLoginUserInfo(QString sMac, QString sBarId = appConfigIni::getBarId());
	~HttpGetLoginUserInfo();
protected:
	void processResponse(QString strResult);
	void processError();	
};

#endif // HTTPGETLOGINUSERINFO_H
