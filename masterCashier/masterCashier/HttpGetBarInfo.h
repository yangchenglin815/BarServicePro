#ifndef HTTPGETBARINFO_H
#define HTTPGETBARINFO_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpGetBarInfo : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpGetBarInfo(QString nBarId);
	~HttpGetBarInfo();
protected:
	void processResponse(QString strResult);
	void processError();
};

#endif // HTTPGETBARINFO_H
