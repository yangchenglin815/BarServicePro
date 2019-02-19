#ifndef ORDERDETIALDIALOG_H
#define ORDERDETIALDIALOG_H

#include <QWidget>
#include "ui_orderDetialDialog.h"

class orderDetialDialog : public QWidget
{
	Q_OBJECT

public:
	orderDetialDialog(QWidget *parent = 0);
	~orderDetialDialog();

private:
	Ui::orderDetialDialog ui;
};

#endif // ORDERDETIALDIALOG_H
