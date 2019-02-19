#ifndef HTTPGETGOODSINFO_H
#define HTTPGETGOODSINFO_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpGetGoodsInfo : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpGetGoodsInfo(QString sName, int nPage);
	~HttpGetGoodsInfo();
protected:
	void processResponse(QString strResult);
	void processError();
private:
	int m_page;
};

#endif // HTTPGETGOODSINFO_H
