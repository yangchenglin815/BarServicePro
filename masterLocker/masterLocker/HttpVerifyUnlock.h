#ifndef HTTPVERIFYUNLOCK_H
#define HTTPVERIFYUNLOCK_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpVerifyUnlock : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpVerifyUnlock(QString sPcName, QString sBarID, QString sMac, QString nCode);
	~HttpVerifyUnlock();
protected:
	void processResponse(QString strResult);
	void processError();
};

#endif // HTTPVERIFYUNLOCK_H
