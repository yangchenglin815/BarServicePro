#ifndef MYDISCOUNTDIALOG_H
#define MYDISCOUNTDIALOG_H

#include <QDialog>
#include "ui_myDiscountDialog.h"

class myDiscountDialog : public QDialog
{
	Q_OBJECT

public:
	myDiscountDialog(QWidget *parent = 0);
	~myDiscountDialog();

private:
	Ui::myDiscountDialog ui;
};

#endif // MYDISCOUNTDIALOG_H
