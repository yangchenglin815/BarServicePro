#include "personalCenterDlg.h"
#include "commonFun.h"
#include "HttpAgent.h"
#include "logging.hpp"
#include "QFlowLayout.h"
#include "superButton.h"

#include <QMessageBox>

personalCenterDlg::personalCenterDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_TranslucentBackground, true);

	InitConnect();
}

personalCenterDlg::~personalCenterDlg()
{

}

void personalCenterDlg::InitData(LoginUserInfo_t userInfo)
{
	m_loginUserInfo = userInfo;
	HttpAgent::instance()->RequestOnGetDiscountPkg(t_personalCentrePage);
}

void personalCenterDlg::slotOnGetDiscountPkg(int nCode, QString sMsg, QList<DiscountPkg_t> lDiscountPkgList)
{
	if (nCode != e_success)
	{
		Log::Info("Request On GetDiscountPkg failed : %s", sMsg.toLocal8Bit().data());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		Log::Info("Request On GetDiscountPkg success");
		m_discountPkgList = lDiscountPkgList;
		InitUi();
	}
}

void personalCenterDlg::InitConnect()
{
	connect(ui.btn_close, SIGNAL(clicked()), this, SLOT(accept()));

	qRegisterMetaType<QList<DiscountPkg_t> >("QList<DiscountPkg_t>");
	connect(HttpAgent::instance(), SIGNAL(sigOnGetDiscountPkgForPersongalCentrePage(int, QString, QList<DiscountPkg_t>)), this, SLOT(slotOnGetDiscountPkg(int, QString, QList<DiscountPkg_t>)));
}

void personalCenterDlg::InitUi()
{
	ui.lbl_user->setText(QStringLiteral("%1").arg(m_loginUserInfo.sName));
	ui.lbl_card->setText(QStringLiteral("身份证号：%1").arg(m_loginUserInfo.sIdCard));
	ui.lbl_wallet_value->setText(QStringLiteral("¥") + QString::number(m_loginUserInfo.dWalletFee, 'f', 2));
	ui.lbl_balance_value->setText(QStringLiteral("¥") + QString::number(m_loginUserInfo.dNetFee, 'f', 2));
	ui.lbl_time_value->setText(QString::number(commonFun::secondsToMinutes(m_loginUserInfo.nDiffTime) + 1));

	QFlowLayout *layout = new QFlowLayout(QMargins(97, 21, 97, 21), 37, 20);
	for (int i = 0; i < m_discountPkgList.size(); ++i)
	{
		superButton *button = new superButton(this, m_discountPkgList[i]);
		button->setFixedSize(QSize(200, 48));
		layout->addWidget(button);
	}
	ui.scrollArea->setLayout(layout);
}
