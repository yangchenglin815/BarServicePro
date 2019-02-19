#ifndef HTTPGETALLGOODSINFO_H
#define HTTPGETALLGOODSINFO_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpGetAllGoodsInfo : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpGetAllGoodsInfo();
	~HttpGetAllGoodsInfo();
protected:
	void processResponse(QString strResult);
	void processError();
};

#endif // HTTPGETALLGOODSINFO_H
