#include "masterDesktop.h"
#include <QDateTime>
#include <QTextCursor>
#include <QTextBlockFormat>
#include <QPropertyAnimation>
#include <QMessageBox>

#include "enumData.h"
#include "commonFun.h"
#include "personalCenterDlg.h"
#include "attendanceDialog.h"
#include "rechargeDialog.h"
#include "orderGoodsDialog.h"
#include "callServiceDialog.h"
#include "wifiConnectionDlg.h"
#include "complaintDialog.h"
#include "EventHandler.h"
#include "HttpAgent.h"
#include "appConfigIni.h"
#include "logging.hpp"

masterDesktop::masterDesktop(QWidget *parent)
: QMainWindow(parent)
, m_personalCenterDlg(NULL)
, m_attendanceDlg(NULL)
, m_rechargeDlg(NULL)
, m_orderGoodsDlg(NULL)
, m_callDlg(NULL)
, m_wifiDlg(NULL)
, m_complaintDlg(NULL)
, m_startTime(QDateTime::currentDateTime())
, m_diffTime(0)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_TranslucentBackground, true);
	setWindowTitle("masterDesktop");

	m_timer = new QTimer;
	m_timer->setInterval(60 * 1000);
	connect(m_timer, &QTimer::timeout, [&](){
		m_diffTime = m_startTime.secsTo(QDateTime::currentDateTime());
		HttpAgent::instance()->RequestOnGetLoginUserInfo("70-8B-CD-7D-F3-6C");
	});
	m_timer->start();

	InitUI();
	Initconnect(); 

	QString path = QApplication::applicationDirPath() + "/config/config.ini";
	appConfigIni::setIniPath(path);

	HttpAgent::instance()->RequestOnGetLoginUserInfo("70-8B-CD-7D-F3-6C");
}

masterDesktop::~masterDesktop()
{
	m_timer->stop();
	m_timer->deleteLater();
}

void masterDesktop::slotOnSendMessage()
{
	QString message = ui.lEd_content->text();
	InitChatMessage(t_local, "", message);
	ui.lEd_content->clear();
}

void masterDesktop::slotOnShowPersonalCenter()
{
	if (m_personalCenterDlg == NULL)
	{
		m_personalCenterDlg = new personalCenterDlg;
	}

	m_personalCenterDlg->InitData(m_loginUserInfo);
	m_animation->setTargetObject(m_personalCenterDlg);
	m_animation->start();
	m_personalCenterDlg->exec();
}

void masterDesktop::slotOnShowAttendance()
{
	if (m_attendanceDlg == NULL)
	{
		m_attendanceDlg = new attendanceDialog;
	}

	m_attendanceDlg->InitSignIn(m_loginUserInfo);
	m_animation->setTargetObject(m_attendanceDlg);
	m_animation->start();
	m_attendanceDlg->exec();
}

void masterDesktop::slotOnShowRecharge()
{
	if (m_rechargeDlg == NULL)
	{
		m_rechargeDlg = new rechargeDialog;
	}

	m_rechargeDlg->InitDiscountPkg(m_loginUserInfo);
	m_animation->setTargetObject(m_rechargeDlg);
	m_animation->start();
	m_rechargeDlg->exec();
}

void masterDesktop::slotOnShowOrderPage()
{
	if (m_orderGoodsDlg == NULL)
	{
		m_orderGoodsDlg = new orderGoodsDialog;
	}
	m_orderGoodsDlg->InitPage(PageType::t_OrderOnline);

	m_animation->setTargetObject(m_orderGoodsDlg);
	m_animation->start();
	m_orderGoodsDlg->exec();
}

void masterDesktop::slotOnShowCallPage()
{
	if (m_callDlg == NULL)
	{
		m_callDlg = new callServiceDialog;
	}

	m_callDlg->InitCall(m_loginUserInfo);
	m_animation->setTargetObject(m_callDlg);
	m_animation->start();
	m_callDlg->exec();
}

void masterDesktop::slotOnShowComplaintsPage()
{
	if (m_complaintDlg == NULL)
	{
		m_complaintDlg = new complaintDialog;
	}

	m_animation->setTargetObject(m_complaintDlg);
	m_animation->start();
	m_complaintDlg->exec();
}

void masterDesktop::slotOnRebootFunc()
{
	dm::server::EventHandler::GetInstance().rebootClientHandler();
}

void masterDesktop::slotOnShutDownFunc()
{
	dm::server::EventHandler::GetInstance().shutdownClientHandler();
}

void masterDesktop::slotOnGetLoginUserInfo(int nCode, QString sMsg, LoginUserInfo loginUserInfo)
{
	if (nCode != e_success)
	{
		Log::Info("Request On GetLoginUserInfo failed : %s", sMsg.toLocal8Bit().data());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		Log::Info("Request On GetLoginUserInfo success");
		m_loginUserInfo = loginUserInfo;
		m_loginUserInfo.nDiffTime = m_diffTime;
		Log::Info("time diff : %d", m_diffTime);
		InitLoginUserInfo();
	}
}

void masterDesktop::slotOnShowWifiPage()
{
	if (m_wifiDlg == NULL)
	{
		m_wifiDlg = new wifiConnectionDlg;
	}

	m_wifiDlg->InitData();
	m_animation->setTargetObject(m_wifiDlg);
	m_animation->start();
	m_wifiDlg->exec();
}

void masterDesktop::Initconnect()
{
	connect(ui.btn_close, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.btn_send, SIGNAL(clicked()), this, SLOT(slotOnSendMessage()));
	connect(ui.btn_personal, SIGNAL(clicked()), this, SLOT(slotOnShowPersonalCenter()));
	connect(ui.btn_attendance, SIGNAL(clicked()), this, SLOT(slotOnShowAttendance()));
	connect(ui.btn_recharge, SIGNAL(clicked()), this, SLOT(slotOnShowRecharge()));
	connect(ui.btn_order, SIGNAL(clicked()), this, SLOT(slotOnShowOrderPage()));
	connect(ui.btn_tel, SIGNAL(clicked()), this, SLOT(slotOnShowCallPage()));
	connect(ui.btn_wifi, SIGNAL(clicked()), this, SLOT(slotOnShowWifiPage()));
	connect(ui.btn_complaints, SIGNAL(clicked()), this, SLOT(slotOnShowComplaintsPage()));
	connect(ui.btn_reboot, SIGNAL(clicked()), this, SLOT(slotOnRebootFunc()));
	connect(ui.btn_shutdown, SIGNAL(clicked()), this, SLOT(slotOnShutDownFunc()));

	qRegisterMetaType<LoginUserInfo>("LoginUserInfo");
	connect(HttpAgent::instance(), SIGNAL(sigOnGetLoginUserInfo(int, QString, LoginUserInfo)), this, SLOT(slotOnGetLoginUserInfo(int, QString, LoginUserInfo)));
}

void masterDesktop::InitUI()
{
	ui.textBrowser->setFocusPolicy(Qt::NoFocus);
	QTextCursor textCursor = ui.textBrowser->textCursor();
	QTextBlockFormat textBlockFormat;
	textBlockFormat.setLineHeight(25, QTextBlockFormat::FixedHeight);//设置固定行高
	textCursor.setBlockFormat(textBlockFormat);
	ui.textBrowser->setTextCursor(textCursor);

	InitChatMessage(t_admin, "", QStringLiteral("欢迎光临全游电竞馆"));
	ui.lbl_pcNo->setText(QStringLiteral("%1").arg(commonFun::getLocalmachineName()));

	m_animation = new QPropertyAnimation;
	m_animation->setPropertyName("windowOpacity");
	m_animation->setDuration(1000);
	m_animation->setStartValue(0);
	m_animation->setEndValue(1);
}

void masterDesktop::InitChatMessage(int nType, QString sUser, QString sMessage)
{
	switch (nType){
	case t_admin:
		sUser = QStringLiteral("系统消息");
		break;
	case t_local:
		sUser = QStringLiteral("我");
		break;
	case t_user:
		break;
	}

	ui.textBrowser->setTextColor(QColor("#22B4B0"));
	ui.textBrowser->setCurrentFont(QFont(QStringLiteral("微软雅黑"), 12));
	ui.textBrowser->append(QString("[ %1 ] ").arg(sUser) + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
	ui.textBrowser->setTextColor(QColor("#DEDEDE"));
	ui.textBrowser->setCurrentFont(QFont(QStringLiteral("微软雅黑"), 10));
	ui.textBrowser->append(QString("     %1").arg(sMessage));
}

void masterDesktop::InitLoginUserInfo()
{
	appConfigIni::setUserId(m_loginUserInfo.sUserId);

	ui.lbl_user->setText(QStringLiteral("%1").arg(m_loginUserInfo.sName));
	ui.lbl_wallet->setText(QStringLiteral("钱包： %1").arg(m_loginUserInfo.dWalletFee, 0, 'f', 2));
	ui.lbl_fee->setText(QStringLiteral("网费： %1").arg(m_loginUserInfo.dNetFee, 0, 'f', 2));

	if (m_personalCenterDlg != NULL)
	{
		m_personalCenterDlg->InitData(m_loginUserInfo);
	}
}
