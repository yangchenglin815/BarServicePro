#ifndef HTTPGETORDERDETIALLIST_H
#define HTTPGETORDERDETIALLIST_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpGetOrderDetialList : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpGetOrderDetialList(QString sSerialId);
	~HttpGetOrderDetialList();
protected:
	void processResponse(QString strResult);
	void processError();
private:
	
};

#endif // HTTPGETORDERDETIALLIST_H
