#pragma once

#include <QString>
#include <QMap>
#include <QList>
#include <QVariant>

typedef struct ProductInfo
{
	QString nProductName;            //商品名称
	QString nProductNum;             //商品数量
	QString nPrice;                        //商品单价
	QString sTasteGroup;        //口味组
}ProductInfo_t;
Q_DECLARE_METATYPE(ProductInfo)

typedef struct ReceiptInfo
{
	QString sBarName;         //网吧名
	QString nSeatNo;              //座位号
	QString nPaymentNo;        //订单编号
	QString nAccount;            //用户名
	QString strSum;               //订单金额
	QList<ProductInfo> nProductList;       //订单商品列表
}ReceiptInfo_t;
Q_DECLARE_METATYPE(ReceiptInfo)