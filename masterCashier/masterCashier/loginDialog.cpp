#include "loginDialog.h"
#include "FramelessHelper.h"
#include "HttpAgent.h"
#include "appConfigIni.h"
#include "enumData.h"
#include "appconfig.h"

#include <QMessageBox>
#include <QKeyEvent>

using namespace dm::server;

loginDialog::loginDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	setWindowTitle(QStringLiteral("登录"));
	ui.btn_max->hide();

	FramelessHelper *helper = new FramelessHelper;
	helper->activateOn(this);
	helper->setDbClickOn(false);
	helper->setWidgetResizable(false);

	init();
	initConnect();
}

loginDialog::~loginDialog()
{

}

void loginDialog::initConnect()
{
	connect(ui.btn_min, SIGNAL(clicked()), this, SLOT(showMinimized()));
	connect(ui.btn_max, SIGNAL(clicked()), this, SLOT(showMaximized()));
	connect(ui.btn_close, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.btn_login, SIGNAL(clicked()), this, SLOT(slotOnLogin()));
	connect(HttpAgent::instance(), SIGNAL(sigOnGetBarInfo(int, QString, QString)), this, SLOT(slotOnGetBarInfo(int, QString, QString)));
	connect(HttpAgent::instance(), SIGNAL(sigOnLogin(int, QString, QString, QString)), this, SLOT(slotOnHandleLogin(int, QString, QString, QString)));
}

void loginDialog::init()
{
	ui.lEdit_account->setMaxLength(11);
	ui.lEdit_account->setFocus();
	ui.lEdit_passwd->setMaxLength(18);
	ui.lEdit_passwd->setEchoMode(QLineEdit::Password);

	QString barId = appConfigIni::getBarId();
	HttpAgent::instance()->RequestOnGetBarInfo(barId);
}

void loginDialog::slotOnLogin()
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
	m_user = ui.lEdit_account->text();
	QString passwd = ui.lEdit_passwd->text();
	HttpAgent::instance()->RequestOnLogin(m_user, passwd, t_login);
}

void loginDialog::slotOnHandleLogin(int nCode, QString sMsg, QString sUser, QString nUserId)
{
	if (nCode != e_success)
	{
		ui.lbl_hint->setText(QStringLiteral("登录失败：%1").arg(sMsg));
		//AppConfigHandler.Info("Request On HandleLogin failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On HandleLogin success");
		appConfigIni::setUserName(QStringLiteral("%1").arg(sUser));
		appConfigIni::setUserId(nUserId);
		accept();
	}
}

void loginDialog::slotOnGetBarInfo(int nCode, QString sMsg, QString nBarName)
{
	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On GetBarInfo failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On GetBarInfo success");
		ui.lbl_name->setText(QStringLiteral("%1收银端").arg(nBarName));
		appConfigIni::setBarName(nBarName);
		emit sigOnGetBarInfo(nBarName);
	}
}


void loginDialog::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter){
		slotOnLogin();
	}
}