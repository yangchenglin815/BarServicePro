#ifndef ORDERRECORDGOODSITEM_H
#define ORDERRECORDGOODSITEM_H

#include <QWidget>
#include "ui_orderRecordGoodsItem.h"

class orderRecordGoodsItem : public QWidget
{
	Q_OBJECT

public:
	orderRecordGoodsItem(QWidget *parent = 0);
	~orderRecordGoodsItem();

private:
	Ui::orderRecordGoodsItem ui;
};

#endif // ORDERRECORDGOODSITEM_H
