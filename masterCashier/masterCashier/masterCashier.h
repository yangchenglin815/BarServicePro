#ifndef MASTERCASHIER_H
#define MASTERCASHIER_H

#include <QtWidgets/QMainWindow>
#include "ui_masterCashier.h"
#include <faceTectionDlg.h>

class QTimer;
class loginDialog;
class switchAccountDlg;
class verifyShopperPwdDlg;
class masterCashier : public QMainWindow
{
	Q_OBJECT

public:
	masterCashier(QWidget *parent = 0);
	~masterCashier();
private:
	void initUI();
	void initConnect();
private slots:
    void slotOnInitLogin();
	void slotOnGetBarInfo(QString nBarName);
	void slotOnSwitchAccount();
	void slotOnShowShiftPage();
	void slotOnShowSalePage();
	void slotOnShowSettingsPage();
	void OnMask(bool isMask);
	void slotOnShowHomePage();
	void slotOnGetWarningGoodsList();
	void slotOnShowVerifyDlg(QString sOrderId, int nPageType);
	void slotOnFaceTection(QString sIdCard, QString sName);
protected:
	void showEvent(QShowEvent *event);
private:
	Ui::masterCashierClass ui;
	loginDialog *m_loginDlg;
	switchAccountDlg *m_switchDlg;
	QString m_sBarName;
	QWidget *m_maskWgt;
	QTimer *m_warningGoods_timer;
	verifyShopperPwdDlg *m_verifyDlg;
	QLabel *m_cameraLabel;
	faceTectionDlg *m_faceTectionDlg;
};

#endif // MASTERCASHIER_H
