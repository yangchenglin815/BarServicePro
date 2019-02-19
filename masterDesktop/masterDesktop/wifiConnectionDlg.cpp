#include "wifiConnectionDlg.h"
#include "HttpAgent.h"
#include "logging.hpp"
#include <QMessageBox>

wifiConnectionDlg::wifiConnectionDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint /*| Qt::WindowStaysOnTopHint*/);
	setAttribute(Qt::WA_TranslucentBackground, true);

	InitConnect();
}

wifiConnectionDlg::~wifiConnectionDlg()
{

}

void wifiConnectionDlg::InitData()
{
	HttpAgent::instance()->RequestOnGetWifiInfo();
}

void wifiConnectionDlg::slotOnGetWifiInfo(int nCode, QString sMsg, QString sWifiName, QString sWifiKey)
{
	if (nCode != e_success)
	{
		Log::Info("Request On GetWifiInfo failed : %s", sMsg.toLocal8Bit().data());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		Log::Info("Request On GetWifiInfo success");
		m_wifiName = sWifiName;
		m_wifiKey = sWifiKey;
		InitUi();
	}
}

void wifiConnectionDlg::InitConnect()
{
	connect(ui.btn_close, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(HttpAgent::instance(), SIGNAL(sigOnGetWifiInfo(int, QString, QString, QString)), this, SLOT(slotOnGetWifiInfo(int, QString, QString, QString)));
}

void wifiConnectionDlg::InitUi()
{
	ui.lbl_wifiName->setText(QStringLiteral("WIFI名称： %1").arg(m_wifiName));
	ui.lbl_wifi->setText(QStringLiteral("WIFI密码： %1").arg(m_wifiKey));
}
