#ifndef HTTPGETWARNINGGOODSLIST_H
#define HTTPGETWARNINGGOODSLIST_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpGetWarningGoodsList : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpGetWarningGoodsList(QString sBarId);
	~HttpGetWarningGoodsList();
protected:
	void processResponse(QString strResult);
	void processError();	
};

#endif // HTTPGETWARNINGGOODSLIST_H
