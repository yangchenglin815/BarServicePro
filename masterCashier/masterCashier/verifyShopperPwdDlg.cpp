#include "verifyShopperPwdDlg.h"
#include <QKeyEvent>
#include <QMessageBox>
#include "HttpAgent.h"
#include "appConfigIni.h"
#include "appconfig.h"
#include "ViewSignal.h"
#include "enumData.h"

using namespace dm::server;

verifyShopperPwdDlg::verifyShopperPwdDlg(QString sOrderId, int nPageType, QWidget *parent)
	: QDialog(parent)
	, m_orderId(sOrderId)
	, m_curr_pageType(nPageType)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);

	ui.lineEdit->setPlaceholderText(QStringLiteral("请输入店长密码"));
	connect(ui.btn_close, SIGNAL(clicked()), this, SLOT(slotOnClose()));
	connect(HttpAgent::instance(), SIGNAL(sigOnGetCancelOrderRespond(int, QString)), this, SLOT(slotOnGetCancelOrderRespond(int, QString)));
}

verifyShopperPwdDlg::~verifyShopperPwdDlg()
{

}

void verifyShopperPwdDlg::keyPressEvent(QKeyEvent *event)
{
	Q_UNUSED(event);
	if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter){
		QString shopperPwd = ui.lineEdit->text();
		if (!shopperPwd.isEmpty())
		{
			HttpAgent::instance()->RequestOnCancelOrder(appConfigIni::getBarId(), m_orderId, shopperPwd);
		}
	}
}

void verifyShopperPwdDlg::slotOnClose()
{
	if (m_curr_pageType == t_salePage)
	{
		ViewSignal::instance()->sigOnReturnWaitingPay();
	}
	accept();
}

void verifyShopperPwdDlg::slotOnGetCancelOrderRespond(int nCode, QString sMsg)
{
	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On GetCancelOrderRespond failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On GetCancelOrderRespond success");
		QMessageBox::warning(NULL, QStringLiteral("操作成功"), QStringLiteral("撤销订单操作已成功！"));
		if (m_curr_pageType == t_salePage)
		{
			ViewSignal::instance()->sigOnCancelOrderSuccess();
		}
	}
}
