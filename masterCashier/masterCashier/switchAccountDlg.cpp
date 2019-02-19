#include "switchAccountDlg.h"
#include "HttpAgent.h"
#include "enumData.h"
#include "appconfig.h"
#include "appConfigIni.h"

#include <QMessageBox>
#include <QKeyEvent>
#include <QPainter>

using namespace dm::server;

switchAccountDlg::switchAccountDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);

	init();
	initConnect();
}

switchAccountDlg::~switchAccountDlg()
{

}

void switchAccountDlg::setLogoText(QString text)
{
	ui.lbl_name->setText(QStringLiteral("%1收银端").arg(text));
}

void switchAccountDlg::slotOnSwitch()
{
	ui.lbl_hint->setText("");
	if (ui.lEdit_account->text().isEmpty())
	{
		ui.lEdit_account->setFocus();
		ui.lbl_hint->setText(QStringLiteral("用户名不能为空！"));
		return;
	}
	else if (ui.lEdit_passwd->text().isEmpty())
	{
		ui.lEdit_passwd->setFocus();
		ui.lbl_hint->setText(QStringLiteral("密码不能为空！"));
		return;
	}

	ui.lbl_hint->setText(QStringLiteral("正在登陆中..."));
	m_User = ui.lEdit_account->text();
	m_Passwd = ui.lEdit_passwd->text();
	HttpAgent::instance()->RequestOnLogout(t_switch);
}

void switchAccountDlg::slotOnLogout(int nCode, QString sMsg)
{
	if (nCode != e_success)
	{
		ui.lbl_hint->setText(QStringLiteral("当前账号登出失败:%1").arg(sMsg));
		//AppConfigHandler.Info("Request On HandleLogout failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On HandleLogout success");
		HttpAgent::instance()->RequestOnLogin(m_User, m_Passwd, t_switch);
	}
}

void switchAccountDlg::slotOnHandleSwitch(int nCode, QString sMsg, QString sUser, QString nUserId)
{
	if (nCode != e_success)
	{
		ui.lbl_hint->setText(QStringLiteral("切换账号失败:%1").arg(sMsg));
	//	AppConfigHandler.Info("Request On HandleLogout to login failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
	//	AppConfigHandler.Info("Request On HandleLogout to login success");
		appConfigIni::setUserName(QStringLiteral("%1").arg(sUser));
		appConfigIni::setUserId(nUserId);
		accept();
	}
}

void switchAccountDlg::init()
{
	ui.lEdit_account->setMaxLength(11);
	ui.lEdit_account->setFocus();
	ui.lEdit_passwd->setMaxLength(18);
	ui.lEdit_passwd->setEchoMode(QLineEdit::Password);
}

void switchAccountDlg::initConnect()
{
	connect(ui.btn_login, SIGNAL(clicked()), this, SLOT(slotOnSwitch()));
	connect(HttpAgent::instance(), SIGNAL(sigOnLogout(int, QString)), this, SLOT(slotOnLogout(int, QString)));
	connect(HttpAgent::instance(), SIGNAL(sigOnSwitch(int, QString, QString, QString)), this, SLOT(slotOnHandleSwitch(int, QString, QString, QString)));
}

void switchAccountDlg::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter){
		slotOnSwitch();
	}
}
