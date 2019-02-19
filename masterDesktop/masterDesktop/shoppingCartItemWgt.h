#ifndef SHOPPINGCARTITEMWGT_H
#define SHOPPINGCARTITEMWGT_H

#include <QWidget>
#include "ui_shoppingCartItemWgt.h"
#include "httpStructData.h"

class shoppingCartItemWgt : public QWidget
{
	Q_OBJECT

public:
	shoppingCartItemWgt(QWidget *parent = 0);
	~shoppingCartItemWgt();

	void setData(GoodsInfo_t goodsInfo);
	GoodsInfo_t getData(){ return m_goodsInfo; };
signals:
	void sigOnSyncAddGoodsInfo(GoodsInfo_t goodsInfo);
	void sigOnSyncReduceGoodsInfo(GoodsInfo_t goodsInfo);
private slots:
    void slotOnAdd();
    void slotOnReduce();
private:
	QByteArray getNetworkPic(const QString &szUrl);
	void InitConnect();
private:
	Ui::shoppingCartItemWgt ui;
	GoodsInfo_t m_goodsInfo;
};

#endif // SHOPPINGCARTITEMWGT_H
