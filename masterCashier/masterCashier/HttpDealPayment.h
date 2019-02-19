#ifndef HTTPDEALPAYMENT_H
#define HTTPDEALPAYMENT_H

#include <QObject>
#include "IHttpReqRes.h"
#include "httpStructData.h"

class HttpDealPayment : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpDealPayment(const PaymentOrder &paymentOrder );
	~HttpDealPayment();
protected:
	void processResponse(QString strResult);
	void processError();
private:
	PaymentOrder m_paymentOrder;
private:
	QString createOrderGoodsListJson(const QList<OrderGoods_t> &orderGoodsInfoList);
	QString createTasteGroupjson(const QList<TasteGroupInfo_t> &tasteGroupList);
};

#endif // HTTPDEALPAYMENT_H
