#ifndef HTTPOPERMAININVENTORY_H
#define HTTPOPERMAININVENTORY_H

#include <QObject>
#include "IHttpReqRes.h"
#include "httpStructData.h"

class HttpOperMainInventory : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpOperMainInventory(int nType, QString sRemarks, QList<ReturnGoodsInfo_t> lReturnGoodsInfoList);
	~HttpOperMainInventory();
protected:
	void processResponse(QString strResult);
	void processError();
private:
	QString createReturnGoodsListJson(const QList<ReturnGoodsInfo_t> &returnGoodsInfoList);
};

#endif // HTTPOPERMAININVENTORY_H
