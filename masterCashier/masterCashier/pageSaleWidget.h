#ifndef PAGESALEWIDGET_H
#define PAGESALEWIDGET_H

#include <QWidget>
#include "ui_pageSaleWidget.h"

class pageSaleWidget : public QWidget
{
	Q_OBJECT

public:
	pageSaleWidget(QWidget *parent = 0);
	~pageSaleWidget();

private:
	Ui::pageSaleWidget ui;
};

#endif // PAGESALEWIDGET_H
