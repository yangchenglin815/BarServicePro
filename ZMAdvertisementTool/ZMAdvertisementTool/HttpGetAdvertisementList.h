#ifndef HTTPGETADVERTISEMENTLIST_H
#define HTTPGETADVERTISEMENTLIST_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpGetAdvertisementList : public IHttpReqRes
{
	Q_OBJECT
public:
	explicit HttpGetAdvertisementList();
protected:
	void processResponse(QString strResult);
	void processError();
};

#endif // HTTPGETADVERTISEMENTLIST_H
