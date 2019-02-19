#ifndef HTTPGETSIGNINLIST_H
#define HTTPGETSIGNINLIST_H

#include <QObject>
#include "IHttpReqRes.h"
#include "httpStructData.h"
#include "appConfigIni.h"

class HttpGetSignInList : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpGetSignInList(LoginUserInfo userInfo, QString sBarId = appConfigIni::getBarId());
	~HttpGetSignInList();
protected:
	void processResponse(QString strResult);
	void processError();
};

#endif // HTTPGETSIGNINLIST_H
