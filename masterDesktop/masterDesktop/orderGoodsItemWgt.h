#ifndef ORDERGOODSITEMWGT_H
#define ORDERGOODSITEMWGT_H

#include <QWidget>
#include "ui_orderGoodsItemWgt.h"
#include "httpStructData.h"

class orderGoodsItemWgt : public QWidget
{
	Q_OBJECT

public:
	orderGoodsItemWgt(QWidget *parent = 0);
	~orderGoodsItemWgt();

	void setData(const GoodsInfo_t &goodsInfo);
	GoodsInfo_t getData(){ return m_goodsInfo; };
	void setNum(int num);
signals:
	void sigOnAddGoodsItem(GoodsInfo_t goodsInfo);
	void sigOnReduceGoodsItem(GoodsInfo_t goodsInfo);
private slots:
    void slotOnAdd();
	void slotOnReduce();
private:
	Ui::orderGoodsItemWgt ui;
	GoodsInfo_t m_goodsInfo;
private:
	QByteArray getNetworkPic(const QString &szUrl);
	void InitConnect();
};

#endif // ORDERGOODSITEMWGT_H
