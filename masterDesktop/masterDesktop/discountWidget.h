#ifndef DISCOUNTWIDGET_H
#define DISCOUNTWIDGET_H

#include <QWidget>
#include "ui_discountWidget.h"

class discountWidget : public QWidget
{
	Q_OBJECT

public:
	discountWidget(QWidget *parent = 0);
	~discountWidget();

private:
	Ui::discountWidget ui;
};

#endif // DISCOUNTWIDGET_H
