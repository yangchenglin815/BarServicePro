#ifndef ORDERCONFIRMGOODSITEMWGT_H
#define ORDERCONFIRMGOODSITEMWGT_H

#include <QWidget>
#include "ui_orderConfirmGoodsItemWgt.h"

class orderConfirmGoodsItemWgt : public QWidget
{
	Q_OBJECT

public:
	orderConfirmGoodsItemWgt(QWidget *parent = 0);
	~orderConfirmGoodsItemWgt();

private:
	Ui::orderConfirmGoodsItemWgt ui;
};

#endif // ORDERCONFIRMGOODSITEMWGT_H
