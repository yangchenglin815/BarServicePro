#ifndef PAYMENTDIALOG_H
#define PAYMENTDIALOG_H

#include <QDialog>
#include "ui_paymentDialog.h"
#include "httpStructData.h"

class paymentDialog : public QDialog
{
	Q_OBJECT

public:
	paymentDialog(QWidget *parent = 0);
	~paymentDialog();

	void Init(DiscountPkg_t discountPkg, QString feeCode);
signals:
	void sigOnExit();
private:
	void InitConnect();
private:
	Ui::paymentDialog ui;
	DiscountPkg_t m_discountPkg;
	QString m_feeCode;
};

#endif // PAYMENTDIALOG_H
