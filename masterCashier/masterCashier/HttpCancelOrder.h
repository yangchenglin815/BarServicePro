#ifndef HTTPCANCELORDER_H
#define HTTPCANCELORDER_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpCancelOrder : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpCancelOrder(QString sBarId, QString sOrderId, QString sPasswd);
	~HttpCancelOrder();
protected:
	void processResponse(QString strResult);
	void processError();
private:
	
};

#endif // HTTPCANCELORDER_H
