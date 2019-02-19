#ifndef HTTPREFUND_H
#define HTTPREFUND_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpRefund : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpRefund(QString sBarId, QString sOrderId, QString sRemark);
	~HttpRefund();
protected:
	void processResponse(QString strResult);
	void processError();
private:
	
};

#endif // HTTPREFUND_H
