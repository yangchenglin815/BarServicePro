#ifndef HTTPOPERATEBARINVENTORY_H
#define HTTPOPERATEBARINVENTORY_H

#include <QObject>
#include "IHttpReqRes.h"
#include "httpStructData.h"

class HttpOperateBarInventory : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpOperateBarInventory(int nType, QString sRemarks, QList<ReturnGoodsInfo_t> lReturnGoodsInfoList);
	~HttpOperateBarInventory();
protected:
	void processResponse(QString strResult);
	void processError();
private:
	QString createReturnGoodsListJson(const QList<ReturnGoodsInfo_t> &returnGoodsInfoList);
};

#endif // HTTPOPERATEBARINVENTORY_H
