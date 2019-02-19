#ifndef HTTPGOODSCHECKUP_H
#define HTTPGOODSCHECKUP_H

#include <QObject>
#include "IHttpReqRes.h"
#include "httpStructData.h"

class HttpGoodsCheckUp : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpGoodsCheckUp(int stockType, const QList<GoodsCheckUpInfo_t> &goodsCheckUpInfoList, QString mark);
	~HttpGoodsCheckUp();
protected:
	void processResponse(QString strResult);
	void processError();
private:
	QString createGoodsGroupjson(const QList<GoodsCheckUpInfo_t> &goodsCheckUpInfoList);
private:
	QList<GoodsCheckUpInfo_t> m_goodsCheckUpInfoList;
};

#endif // HTTPGOODSCHECKUP_H
