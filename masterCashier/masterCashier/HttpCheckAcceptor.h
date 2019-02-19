#ifndef HTTPCHECKACCEPTOR_H
#define HTTPCHECKACCEPTOR_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpCheckAcceptor : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpCheckAcceptor(QString sPhone, QString sPwd);
	~HttpCheckAcceptor();
protected:
	void processResponse(QString strResult);
	void processError();
};

#endif // HTTPCHECKACCEPTOR_H
