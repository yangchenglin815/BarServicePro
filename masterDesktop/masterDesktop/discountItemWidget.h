#ifndef DISCOUNTITEMWIDGET_H
#define DISCOUNTITEMWIDGET_H

#include <QWidget>
#include "ui_discountItemWidget.h"

class discountItemWidget : public QWidget
{
	Q_OBJECT

public:
	discountItemWidget(QWidget *parent = 0);
	~discountItemWidget();

	void setData(QString text);
	void setChecked(bool isChecked);
	bool isChecked();
private:
	Ui::discountItemWidget ui;
	bool m_isChecked;
};

#endif // DISCOUNTITEMWIDGET_H
