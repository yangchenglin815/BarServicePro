#ifndef HTTPGETBARNAME_H
#define HTTPGETBARNAME_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpGetBarName : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpGetBarName(QString nBarId);
	~HttpGetBarName();
protected:
	void processResponse(QString strResult);
	void processError();
};

#endif // HTTPGETBARNAME_H
