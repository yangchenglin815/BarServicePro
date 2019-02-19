#ifndef MASTERLOCKER_H
#define MASTERLOCKER_H

#include <QtWidgets/QMainWindow>
#include "ui_masterlocker.h"
#include "socketRequest.h"
#include "KillExe.h"
#include <QProcess>
#include <QTimer>

class QPushButton;
class QPaintEvent;
class QShowEvent;
class masterLocker : public QMainWindow
{
	Q_OBJECT

public:
	masterLocker(QWidget *parent = 0);
	~masterLocker();
protected:
	void paintEvent(QPaintEvent *event);
	void showEvent(QShowEvent *event);

	void keyPressEvent(QKeyEvent *event);
	void closeEvent(QCloseEvent *event);
public slots:
	void kill();
	void startLock();
	void stopLock();

private slots:
    void slotOnGetBookSeatStatus(int nCode, QString sMsg, int nStatus);
	void slotOnGetVerifyUnlockCode(int nCode, QString sMsg);
	void slotOnReportOnlineStatus(int nCode, QString sMsg);
	void slotOnGetDownloadUrl(int nCode, QString sMsg, QString nUrl);
	void slotOnGetBarName(int nCode, QString sMsg, QString nBarName);
	void slotOnConfirm();
private:
	void initUI();
	void initConnect();
	void handleBookSeatStatus(int nStatus);
signals:
	void run();
private:
	Ui::masterLockerClass ui;
	KillExe kill_exe_;
	int g_nActScreenW;
	int g_nActScreenH;

	QProcess process;
	bool isRuning;
	QTimer *m_timer;

	QString m_nPCName;
	QString m_nMac;
	QString m_nBarId;
};

#endif // MASTERLOCKER_H
