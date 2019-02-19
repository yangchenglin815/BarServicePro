#ifndef ORDERRECORDITEMWIDGET_H
#define ORDERRECORDITEMWIDGET_H

#include <QWidget>
#include "ui_orderRecordItemWidget.h"

class orderRecordItemWidget : public QWidget
{
	Q_OBJECT

public:
	orderRecordItemWidget(QWidget *parent = 0);
	~orderRecordItemWidget();

private:
	Ui::orderRecordItemWidget ui;
};

#endif // ORDERRECORDITEMWIDGET_H
