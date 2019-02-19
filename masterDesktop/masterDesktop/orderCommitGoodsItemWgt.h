#ifndef ORDERCOMMITGOODSITEMWGT_H
#define ORDERCOMMITGOODSITEMWGT_H

#include <QWidget>
#include "ui_orderCommitGoodsItemWgt.h"

class orderCommitGoodsItemWgt : public QWidget
{
	Q_OBJECT

public:
	orderCommitGoodsItemWgt(QWidget *parent = 0);
	~orderCommitGoodsItemWgt();

private:
	Ui::orderCommitGoodsItemWgt ui;
};

#endif // ORDERCOMMITGOODSITEMWGT_H
