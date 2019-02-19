#ifndef ATTENDANCEDIALOG_H
#define ATTENDANCEDIALOG_H

#include <QDialog>
#include "ui_attendanceDialog.h"
#include "httpStructData.h"

class QTableWidgetItem;
class attendanceDialog : public QDialog
{
	Q_OBJECT

public:
	attendanceDialog(QWidget *parent = 0);
	~attendanceDialog();

	void InitSignIn(LoginUserInfo userInfo);
private slots:
    void slotOnItemClicked(QTableWidgetItem *item);
	void slotOnGetSignInList(int nCode, QString sMsg, QList<QString> lSignInDateList);
	void slotOnSignIn(int nCode, QString sMsg);
private:
	void InitUI();
	void InitConnect();
	void InitData();
private:
	Ui::attendanceDialog ui;
	QList<QString> m_weekList;
	LoginUserInfo m_loginUserInfo;
	QList<QString> m_signInDateList;
	QTableWidgetItem *m_curr_item;
	QString m_curr_signInDay;
};

#endif // ATTENDANCEDIALOG_H
