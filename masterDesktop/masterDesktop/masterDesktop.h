#ifndef MASTERDESKTOP_H
#define MASTERDESKTOP_H

#include <QtWidgets/QMainWindow>
#include "ui_masterDesktop.h"
#include "httpStructData.h"
#include <QDateTime>
#include <QTimer>

class personalCenterDlg;
class attendanceDialog;
class rechargeDialog;
class orderGoodsDialog;
class QPropertyAnimation;
class callServiceDialog;
class wifiConnectionDlg;
class complaintDialog;

class masterDesktop : public QMainWindow
{
	Q_OBJECT

public:
	masterDesktop(QWidget *parent = 0);
	~masterDesktop();
private slots:
    void slotOnSendMessage();
	void slotOnShowPersonalCenter();
	void slotOnShowAttendance();
	void slotOnShowRecharge();
	void slotOnShowOrderPage();
	void slotOnShowCallPage();
	void slotOnShowWifiPage();
	void slotOnShowComplaintsPage();
	void slotOnRebootFunc();
	void slotOnShutDownFunc();
	void slotOnGetLoginUserInfo(int nCode, QString sMsg, LoginUserInfo loginUserInfo);
private:
	void Initconnect();
	void InitUI();
	void InitChatMessage(int nType, QString sUser, QString sMessage);
	void InitLoginUserInfo();
private:
	Ui::masterDesktopClass ui;
	QPropertyAnimation *m_animation;
	personalCenterDlg *m_personalCenterDlg;
	attendanceDialog *m_attendanceDlg;
	rechargeDialog *m_rechargeDlg;
	orderGoodsDialog *m_orderGoodsDlg;
	callServiceDialog *m_callDlg;
	wifiConnectionDlg *m_wifiDlg;
	complaintDialog *m_complaintDlg;
	LoginUserInfo m_loginUserInfo;

	QDateTime m_startTime;
	QTimer *m_timer;
	int m_diffTime;
};

#endif // MASTERDESKTOP_H
