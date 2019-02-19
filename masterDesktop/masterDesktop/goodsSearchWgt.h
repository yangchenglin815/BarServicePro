#ifndef GOODSSEARCHWGT_H
#define GOODSSEARCHWGT_H

#include <QWidget>
#include "ui_goodsSearchWgt.h"

class goodsSearchWgt : public QWidget
{
	Q_OBJECT

public:
	goodsSearchWgt(QWidget *parent = 0);
	~goodsSearchWgt();

private:
	Ui::goodsSearchWgt ui;
};

#endif // GOODSSEARCHWGT_H
