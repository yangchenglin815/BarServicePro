#include "callServiceDialog.h"
#include "HttpAgent.h"
#include "logging.hpp"
#include <QMessageBox>

callServiceDialog::callServiceDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint /*| Qt::WindowStaysOnTopHint*/);
	setAttribute(Qt::WA_TranslucentBackground, true);

	InitConnect();
}

callServiceDialog::~callServiceDialog()
{

}

void callServiceDialog::InitCall(LoginUserInfo userInfo)
{
	HttpAgent::instance()->RequestOnCallService(userInfo);
}

void callServiceDialog::slotOnCallService(int nCode, QString sMsg)
{
	if (nCode != e_success)
	{
		Log::Info("Request On CallService failed : %s", sMsg.toLocal8Bit().data());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		Log::Info("Request On CallService success");
	}
}

void callServiceDialog::InitConnect()
{
	connect(ui.btn_close, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui.btn_cancel, SIGNAL(clicked()), this, SLOT(accept()));
	connect(HttpAgent::instance(), SIGNAL(sigOnCallService(int, QString)), this, SLOT(slotOnCallService(int, QString)));
}
