#ifndef PAYMENTDLG_H
#define PAYMENTDLG_H

#include <QDialog>
#include "ui_paymentDlg.h"

class paymentDlg : public QDialog
{
	Q_OBJECT

public:
	explicit paymentDlg(QWidget *parent);
	~paymentDlg();

	void setModelData(int type, QString msg, bool isMobliePay);
signals:
	void sigOnScanFinished(QString code);
	void sigOnCloseOrder();
protected:
	void keyPressEvent(QKeyEvent *event);
public slots:
    void releaseDlg();
    void contrlDlg();
	void slotOnScanFinished();
protected:
	void showEvent(QShowEvent *event);
private:
	void initUI();
private:
	Ui::paymentDlg ui;
	int m_type;
	QString m_msg;
};

#endif // PAYMENTDLG_H
