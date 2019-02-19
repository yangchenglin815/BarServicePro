#ifndef PAGEORDERCONFIRMWGT_H
#define PAGEORDERCONFIRMWGT_H

#include <QWidget>
#include "ui_pageOrderConfirmWgt.h"

class pageOrderConfirmWgt : public QWidget
{
	Q_OBJECT

public:
	pageOrderConfirmWgt(QWidget *parent = 0);
	~pageOrderConfirmWgt();

private:
	Ui::pageOrderConfirmWgt ui;
};

#endif // PAGEORDERCONFIRMWGT_H
