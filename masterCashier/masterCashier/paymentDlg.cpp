#include "paymentDlg.h"
#include "enumData.h"
#include "appconfig.h"
#include <ViewSignal.h>
#include <QTimer>
#include <QDesktopWidget>
#include <QKeyEvent>

using namespace dm::server;

paymentDlg::paymentDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	ui.lineEdit->setFocus();
	QRegExp regx("[1-9][0-9]+$");
	QValidator *validator = new QRegExpValidator(regx, ui.lineEdit);
	ui.lineEdit->setValidator(validator);
	ui.lineEdit->setMaxLength(24);
	ui.lbl_tip->adjustSize();
	ui.lbl_tip->setWordWrap(true);
	//connect(ui.lineEdit, SIGNAL(editingFinished()), this, SLOT(slotOnScanFinished()));
	connect(ui.btn_close, SIGNAL(clicked()), this, SIGNAL(sigOnCloseOrder()));

	initUI();
}

paymentDlg::~paymentDlg()
{

}

void paymentDlg::setModelData(int type, QString msg, bool isMobliePay)
{
	m_type = type;
	m_msg = msg;

	ui.label->setHidden(!isMobliePay);
	ui.label_2->setHidden(!isMobliePay);
	ui.lineEdit->setHidden(!isMobliePay);
	ui.btn_close->setHidden(!isMobliePay);

	initUI();
}

void paymentDlg::keyPressEvent(QKeyEvent *event)
{
	Q_UNUSED(event);
	if (event->key() == Qt::Key_Escape){
		releaseDlg();
	}
	else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
	{
		slotOnScanFinished();
	}
}

void paymentDlg::releaseDlg()
{
	accept();
	ViewSignal::instance()->onMask(false);
}

void paymentDlg::contrlDlg()
{

}

void paymentDlg::slotOnScanFinished()
{
	if (!ui.lineEdit->text().isEmpty())
	{
		if (ui.lineEdit->text().length() < 16)
		{
			ui.lbl_tip->setText(QStringLiteral("请输入正确格式的16-24位付款码！"));
			return;
		}
		emit sigOnScanFinished(ui.lineEdit->text());
		ui.lineEdit->clear();
	}
}

void paymentDlg::showEvent(QShowEvent *event)
{
	QDesktopWidget *desktop = QApplication::desktop();
	//获取桌面宽度
	int w = desktop->width();
	//获取桌面高度
	int h = desktop->height();

	this->move((w - width()) / 2, (h - height()) / 2);
}

void paymentDlg::initUI()
{
	switch (m_type)
	{
	case t_payment_success:
		ui.label->hide();
		ui.label_2->hide();
		ui.lineEdit->hide();
		ui.lbl_logo->setStyleSheet("background-image:url(:/masterCashier/Resources/payment_success.png)");
		ui.lbl_tip->setText(QStringLiteral("支付成功"));
		//AppConfigHandler.Info("payment success !");
		QTimer::singleShot(2000, this, SLOT(releaseDlg()));
		break;
	case t_payment_failed:
		ui.lbl_logo->setStyleSheet("background-image:url(:/masterCashier/Resources/payment_failed.png)");
		ui.lbl_tip->setText(QStringLiteral("%1").arg(m_msg));
		//AppConfigHandler.Info("payment failed !");
		//QTimer::singleShot(3000, this, SLOT(releaseDlg()));
		break;
	case t_paymenting:
		ui.lbl_logo->setStyleSheet("background-image:url(:/masterCashier/Resources/payment_ing.png)");
		ui.lbl_tip->setText(QStringLiteral("支付中..."));
		break;
	}
}
