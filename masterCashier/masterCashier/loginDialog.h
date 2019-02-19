#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "ui_loginDialog.h"

class loginDialog : public QDialog
{
	Q_OBJECT

public:
	loginDialog(QWidget *parent = 0);
	~loginDialog();
private:
	void initConnect();
	void init();
signals:
	void sigOnGetBarInfo(QString nBarName);
protected:
	void keyPressEvent(QKeyEvent *event);
private slots:
    void slotOnLogin();
	void slotOnHandleLogin(int nCode, QString sMsg, QString sUser, QString nUserId);
	void slotOnGetBarInfo(int nCode, QString sMsg, QString nBarName);
private:
	Ui::loginDialog ui;
	QString m_user;
};

#endif // LOGINDIALOG_H
