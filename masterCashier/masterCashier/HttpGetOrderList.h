#ifndef HTTPGETORDERLIST_H
#define HTTPGETORDERLIST_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpGetOrderList : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpGetOrderList();
	~HttpGetOrderList();
protected:
	void processResponse(QString strResult);
	void processError();
	
};

#endif // HTTPGETORDERLIST_H
