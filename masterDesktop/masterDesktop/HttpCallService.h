#ifndef HTTPCALLSERVICE_H
#define HTTPCALLSERVICE_H

#include <QObject>
#include "IHttpReqRes.h"
#include "appConfigIni.h"
#include "httpStructData.h"

class HttpCallService : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpCallService(LoginUserInfo userInfo, QString sBarId = appConfigIni::getBarId());
	~HttpCallService();
protected:
	void processResponse(QString strResult);
	void processError();
};

#endif // HTTPCALLSERVICE_H
