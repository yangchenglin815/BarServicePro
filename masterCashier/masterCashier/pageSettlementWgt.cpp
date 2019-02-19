#include "pageSettlementWgt.h"
#include <QStyledItemDelegate>
#include <QMessageBox>
#include <QKeyEvent>

#include "HttpAgent.h"
#include "appconfig.h"
#include "appConfigIni.h"
#include "handOverShiftDlg.h"
#include "ViewSignal.h"

using namespace dm::server;

pageSettlementWgt::pageSettlementWgt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	QStyledItemDelegate *itemDelegate = new QStyledItemDelegate();
	ui.cbx_successor->setItemDelegate(itemDelegate);

	ui.lEd_passwd->setMaxLength(18);
	ui.lEd_passwd->setEchoMode(QLineEdit::Password);

	initConnect();
}

pageSettlementWgt::~pageSettlementWgt()
{

}

void pageSettlementWgt::slotOnGetShiftInfo(int nCode, QString sMsg, ShiftInfo_t tShiftInfo)
{
	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On GetShiftInfo failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("ÍøÂçÇëÇó³öÏÖÒì³£--´íÎóÂë:%1, ´íÎóÐÅÏ¢:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On GetShiftInfo success");
		m_shiftInfo = tShiftInfo;
		appConfigIni::setParentId(m_shiftInfo.nParentId);
		initData();
	}
}

void pageSettlementWgt::slotOnCommitFunc()
{
	if (ui.lEd_extraReceipt->text().isEmpty() || ui.lEd_extraExpenses->text().isEmpty() || ui.lEd_passwd->text().isEmpty())
	{
		QMessageBox::warning(NULL, "warning", QStringLiteral("ÐÅÏ¢ÊäÈë´íÎó£¡"));
		return;
	}

	if (appConfigIni::getUserId() == m_shifterInfo.nShiftId)
	{
		QMessageBox::warning(NULL, "warning", QStringLiteral("½Ó°àÈËÐÅÏ¢´íÎó£¡"));
		return;
	}

	m_shiftInfo.dCashSale = ui.lbl_cashSaleSum->text().toDouble();
	m_shiftInfo.dCashReturn = ui.lbl_cashReturnSum->text().remove("-").toDouble();
	m_shiftInfo.dCodeSale = ui.lbl_codeSaleSum->text().toDouble();
	m_shiftInfo.dTotalSale = ui.lbl_shiftSaleSum->text().toDouble();
	m_shiftInfo.dCash = ui.lbl_cashSum->text().toDouble();

	m_shiftInfo.dOperationTotal = ui.lbl_sumNum->text().toDouble();
	m_shiftInfo.dOperationIn = ui.lbl_receiptNum->text().toDouble();
	m_shiftInfo.dOperationOut = ui.lbl_expensesNum->text().toDouble();
	m_shiftInfo.dCheckTotal = ui.lbl_checkNum->text().toDouble();
	m_shiftInfo.dCheckLoss = ui.lbl_damageNum->text().toDouble();
	m_shiftInfo.dCheckProfit = ui.lbl_overflowingNum->text().toDouble();

	m_shiftInfo.dOtherIn = ui.lEd_extraReceipt->text().toDouble();
	m_shiftInfo.dOtherOut = ui.lEd_extraExpenses->text().toDouble();
	m_shiftInfo.dCashIn = ui.lbl_cashReceipt->text().toDouble();
	m_shiftInfo.dCashOut = ui.lbl_cashExpenses->text().toDouble();
	m_shiftInfo.dCashTotal = ui.lbl_cashReceiptSum->text().toDouble();

	m_shiftInfo.nEndTime = QString::number(QDateTime::currentDateTime().toTime_t()).append("000");
	m_shiftInfo.nParentId = appConfigIni::getParentId();
	m_acceptorId = m_shifterInfo.nShiftId;
	m_acceptorPwd = ui.lEd_passwd->text();
	m_mark = ui.tEd_remark->toPlainText();

	HttpAgent::instance()->RequestOnCheckAcceptor(m_shifterInfo.nAccount, m_acceptorPwd);
}

void pageSettlementWgt::slotOnIndexChanged(int index)
{
	m_shifterInfo = ui.cbx_successor->itemData(index).value<ShifterInfo_t>();
}

void pageSettlementWgt::slotOnCheckAcceptorResult(int nCode, QString sMsg)
{
	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On CheckAcceptorResult failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("ÍøÂçÇëÇó³öÏÖÒì³£--´íÎóÂë:%1, ´íÎóÐÅÏ¢:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On CheckAcceptorResult success");
		HttpAgent::instance()->RequestOnCommitShiftInfo(m_shiftInfo, m_acceptorId, m_mark);
	}
}

void pageSettlementWgt::slotOnCommitShiftInfoResult(int nCode, QString sMsg)
{
	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On CommitShiftInfoResult failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("ÍøÂçÇëÇó³öÏÖÒì³£--´íÎóÂë:%1, ´íÎóÐÅÏ¢:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On CommitShiftInfoResult success");
		HttpAgent::instance()->RequestOnLogout(t_shift);
	}
}

void pageSettlementWgt::slotOnShiftLogout(int nCode, QString sMsg)
{
	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On ShiftLogout failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("ÍøÂçÇëÇó³öÏÖÒì³£--´íÎóÂë:%1, ´íÎóÐÅÏ¢:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On ShiftLogout success");
		HttpAgent::instance()->RequestOnLogin(m_shifterInfo.nAccount, m_acceptorPwd, t_shift);
	}
}

void pageSettlementWgt::slotOnShiftLogin(int nCode, QString sMsg, QString sUser, QString nUserId)
{
	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On ShiftLogin failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("ÍøÂçÇëÇó³öÏÖÒì³£--´íÎóÂë:%1, ´íÎóÐÅÏ¢:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On ShiftLogin success");
		appConfigIni::setUserName(QStringLiteral("%1").arg(sUser));
		appConfigIni::setUserId(nUserId);
		handOverShiftDlg *dlg = new handOverShiftDlg();
		ViewSignal::instance()->onMask(true);
		int r = dlg->exec();
		if (r == QDialog::Accepted)
		{
			ViewSignal::instance()->onMask(false);
			ViewSignal::instance()->sigOnShowHomePage();
		}
	}
}

void pageSettlementWgt::initConnect()
{
	connect(ui.btn_front, SIGNAL(clicked()), this, SIGNAL(sigOnFrontFunc()));
	connect(ui.btn_commit, SIGNAL(clicked()), this, SLOT(slotOnCommitFunc()));
	connect(ui.cbx_successor, SIGNAL(currentIndexChanged(int)), this, SLOT(slotOnIndexChanged(int)));

	connect(HttpAgent::instance(), SIGNAL(sigOnCheckAcceptorResult(int, QString)), this, SLOT(slotOnCheckAcceptorResult(int, QString)));
	connect(HttpAgent::instance(), SIGNAL(sigOnCommitShiftInfoResult(int, QString)), this, SLOT(slotOnCommitShiftInfoResult(int, QString)));
	connect(HttpAgent::instance(), SIGNAL(sigOnShiftLogout(int, QString)), this, SLOT(slotOnShiftLogout(int, QString)));
	connect(HttpAgent::instance(), SIGNAL(sigOnShiftLogin(int, QString, QString, QString)), this, SLOT(slotOnShiftLogin(int, QString, QString, QString)));

	qRegisterMetaType<ShiftInfo_t>("ShiftInfo_t");
	connect(HttpAgent::instance(), SIGNAL(sigOnGetShiftInfo(int, QString, ShiftInfo_t)), this, SLOT(slotOnGetShiftInfo(int, QString, ShiftInfo_t)));
}

void pageSettlementWgt::initData()
{
	ui.lbl_cashSaleSum->setText(QString::number(m_shiftInfo.dCashSale));
	if (m_shiftInfo.dCashReturn == 0)
	{
		ui.lbl_cashReturnSum->setText(QString::number(m_shiftInfo.dCashReturn));
	}
	else
	{
		ui.lbl_cashReturnSum->setText(QString("-%1").arg(m_shiftInfo.dCashReturn));
	}
	ui.lbl_codeSaleSum->setText(QString::number(m_shiftInfo.dCodeSale));
	ui.lbl_shiftSaleSum->setText(QString::number(m_shiftInfo.dTotalSale));
	ui.lbl_cashSum->setText(QString::number(m_shiftInfo.dCash));

	ui.lbl_sumNum->setText(QString::number(m_shiftInfo.dOperationTotal));
	ui.lbl_receiptNum->setText(QString::number(m_shiftInfo.dOperationIn));
	ui.lbl_expensesNum->setText(QString::number(m_shiftInfo.dOperationOut));
	ui.lbl_checkNum->setText(QString::number(m_shiftInfo.dCheckTotal));
	ui.lbl_damageNum->setText(QString::number(m_shiftInfo.dCheckLoss));
	ui.lbl_overflowingNum->setText(QString::number(m_shiftInfo.dCheckProfit));

	ui.lEd_extraReceipt->setText(QString::number(m_shiftInfo.dOtherIn));
	ui.lEd_extraExpenses->setText(QString::number(m_shiftInfo.dOtherOut));
	ui.lbl_cashReceipt->setText(QString::number(m_shiftInfo.dCash));
	ui.lbl_cashExpenses->setText(QString::number(m_shiftInfo.dCashOut));
	ui.lbl_cashReceiptSum->setText(QString::number(m_shiftInfo.dCashTotal));

	ui.lbl_shift->setText(QStringLiteral("%1").arg(appConfigIni::getUserName()));
	ui.cbx_successor->clear();
	for (int i = 0; i < m_shiftInfo.nShifterList.size(); ++i)
	{
		QVariant var;
		var.setValue(m_shiftInfo.nShifterList[i]);
		ui.cbx_successor->addItem(QStringLiteral("%1").arg(m_shiftInfo.nShifterList[i].nUserName), var);
	}
	ui.lEd_passwd->clear();
	ui.tEd_remark->clear();
}

QString pageSettlementWgt::getUserIdByName(QString name)
{
	for (int i = 0; i < m_shiftInfo.nShifterList.size(); ++i)
	{
		if (m_shiftInfo.nShifterList[i].nUserName == name)
		{
			return m_shiftInfo.nShifterList[i].nShiftId;
		}
	}
	return "";
}
