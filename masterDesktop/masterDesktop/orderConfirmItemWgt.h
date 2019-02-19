#ifndef ORDERCONFIRMITEMWGT_H
#define ORDERCONFIRMITEMWGT_H

#include <QWidget>
#include "ui_orderConfirmItemWgt.h"

class orderConfirmItemWgt : public QWidget
{
	Q_OBJECT

public:
	orderConfirmItemWgt(QWidget *parent = 0);
	~orderConfirmItemWgt();

private:
	Ui::orderConfirmItemWgt ui;
};

#endif // ORDERCONFIRMITEMWGT_H
