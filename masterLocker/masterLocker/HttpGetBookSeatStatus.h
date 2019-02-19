#ifndef HTTPGETBOOKSEATSTATUS_H
#define HTTPGETBOOKSEATSTATUS_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpGetBookSeatStatus : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpGetBookSeatStatus(QString sPcName, QString sBarID, QString sMac);
	~HttpGetBookSeatStatus();
protected:
	void processResponse(QString strResult);
	void processError();
};

#endif // HTTPGETBOOKSEATSTATUS_H
