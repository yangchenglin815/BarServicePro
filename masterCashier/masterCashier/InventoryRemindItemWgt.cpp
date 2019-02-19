#include "InventoryRemindItemWgt.h"

InventoryRemindItemWgt::InventoryRemindItemWgt(bool flag, QWidget *parent /*= 0*/)
	: QWidget(parent)
{
	ui.setupUi(this);
	if (flag)
	{
		ui.lbl_logo->setStyleSheet("background-image:url(:/masterCashier/Resources/finish.png);");
	}
	else
	{
		ui.lbl_logo->setStyleSheet("background-image:url(:/masterCashier/Resources/unfinish.png);");
	}
}

InventoryRemindItemWgt::~InventoryRemindItemWgt()
{

}

void InventoryRemindItemWgt::setData(QString nContent, QString nTime)
{
	ui.lbl_name->setText(QStringLiteral("%1").arg(nContent));
	ui.lbl_time->setText(QStringLiteral("%1").arg(nTime));
}

void InventoryRemindItemWgt::setData(OrderInfo_t orderInfo)
{
	m_orderInfo = orderInfo;
	if (m_orderInfo.nTradeType == 1)
	{
		ui.lbl_name->setText(QStringLiteral("销售单"));
		ui.lbl_logo->setStyleSheet("background-image:url(:/masterCashier/Resources/finish.png);");
	}
	else if (m_orderInfo.nTradeType == 2)
	{
		ui.lbl_name->setText(QStringLiteral("退货单"));
		ui.lbl_logo->setStyleSheet("background-image:url(:/masterCashier/Resources/unfinish.png);");
	}
	QString time = QDateTime::fromTime_t(m_orderInfo.sCreateTime.toLongLong()/1000).toString("yyyy-MM-dd hh:mm:ss");
	ui.lbl_time->setText(QStringLiteral("%1").arg(time));
}

OrderInfo_t InventoryRemindItemWgt::getData()
{
	return m_orderInfo;
}
