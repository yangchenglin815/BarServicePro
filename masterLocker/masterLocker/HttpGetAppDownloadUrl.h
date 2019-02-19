#ifndef HTTPGETAPPDOWNLOADURL_H
#define HTTPGETAPPDOWNLOADURL_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpGetAppDownloadUrl : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpGetAppDownloadUrl(QString nAction);
	~HttpGetAppDownloadUrl();
protected:
	void processResponse(QString strResult);
	void processError();
};

#endif // HTTPGETAPPDOWNLOADURL_H
