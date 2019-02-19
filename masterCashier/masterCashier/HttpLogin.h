#ifndef HTTPLOGIN_H
#define HTTPLOGIN_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpLogin : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpLogin(QString nUser, QString nPasswd, int nType);
	~HttpLogin();
protected:
	void processResponse(QString strResult);
	void processError();
private:
	int m_loginType;
};

#endif // HTTPLOGIN_H
