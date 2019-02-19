#ifndef HTTPSIGNIN_H
#define HTTPSIGNIN_H

#include <QObject>
#include "IHttpReqRes.h"
#include "httpStructData.h"
#include "appConfigIni.h"

class HttpSignIn : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpSignIn(LoginUserInfo userInfo, QString sBarId = appConfigIni::getBarId());
	~HttpSignIn();
protected:
	void processResponse(QString strResult);
	void processError();
};

#endif // HTTPSIGNIN_H
