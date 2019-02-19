#ifndef SWITCHACCOUNTDLG_H
#define SWITCHACCOUNTDLG_H

#include <QDialog>
#include "ui_switchAccountDlg.h"

class switchAccountDlg : public QDialog
{
	Q_OBJECT

public:
	switchAccountDlg(QWidget *parent = 0);
	~switchAccountDlg();

	void setLogoText(QString text);
private slots:
    void slotOnSwitch();
	void slotOnLogout(int nCode, QString sMsg);
	void slotOnHandleSwitch(int nCode, QString sMsg, QString sUser, QString nUserId);
protected:
	void keyPressEvent(QKeyEvent *event);
private:
	void init();
	void initConnect();
private:
	Ui::switchAccountDlg ui;
	QString m_User;
	QString m_Passwd;
};

#endif // SWITCHACCOUNTDLG_H
