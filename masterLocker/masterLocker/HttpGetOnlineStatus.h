#ifndef HTTPGETONLINESTATUS_H
#define HTTPGETONLINESTATUS_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpGetOnlineStatus : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpGetOnlineStatus(QString sPcName, QString sBarID, QString sMac, QString nOnlineFlag);
	~HttpGetOnlineStatus();
protected:
	void processResponse(QString strResult);
	void processError();
};

#endif // HTTPGETONLINESTATUS_H
