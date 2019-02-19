#ifndef VERIFYSHOPPERPWDDLG_H
#define VERIFYSHOPPERPWDDLG_H

#include <QDialog>
#include "ui_verifyShopperPwdDlg.h"

class verifyShopperPwdDlg : public QDialog
{
	Q_OBJECT

public:
	explicit verifyShopperPwdDlg(QString sOrderId, int nPageType, QWidget *parent = 0);
	~verifyShopperPwdDlg();
protected:
	void keyPressEvent(QKeyEvent *event);
private slots:
    void slotOnClose();
	void slotOnGetCancelOrderRespond(int nCode, QString sMsg);
private:
	Ui::verifyShopperPwdDlg ui;
	QString m_orderId;
	int m_curr_pageType;
};

#endif // VERIFYSHOPPERPWDDLG_H
