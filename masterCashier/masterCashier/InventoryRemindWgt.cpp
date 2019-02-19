#include "InventoryRemindWgt.h"
#include "InventoryRemindItemWgt.h"
#include "RemindContentItemWgt.h"
#include "HttpAgent.h"
#include "appconfig.h"
#include <QMessageBox>
#include <QTime>
#include <ViewSignal.h>

using namespace dm::server;

InventoryRemindWgt::InventoryRemindWgt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.lwgt_contentRemind->setFocusPolicy(Qt::NoFocus);
	ui.lwgt_inventoryRemind->setFocusPolicy(Qt::NoFocus);
	InitConnect();
}

InventoryRemindWgt::~InventoryRemindWgt()
{

}

void InventoryRemindWgt::InitWarningOrderList()
{
	ui.lwgt_inventoryRemind->clear();
	if (m_warningGoodsList.size() == 0)
	{
		ViewSignal::instance()->sigOnShowTipLabel(false);
		return;
	}

	ViewSignal::instance()->sigOnShowTipLabel(true);
	InventoryRemindItemWgt *itemWidget = new InventoryRemindItemWgt(false);
	itemWidget->setData(QStringLiteral("进货提醒"), QTime::currentTime().toString("hh:mm:ss"));
	QListWidgetItem *cItem = new QListWidgetItem();
	cItem->setFlags(cItem->flags() & ~Qt::ItemIsSelectable);
	cItem->setSizeHint(QSize(ui.lwgt_inventoryRemind->width(), 36));
	ui.lwgt_inventoryRemind->addItem(cItem);
	ui.lwgt_inventoryRemind->setItemWidget(cItem, itemWidget);
	InitWarningGoodsList();
}

void InventoryRemindWgt::InitWarningGoodsList()
{
	ui.lwgt_contentRemind->clear();
	for (int i = 0; i < m_warningGoodsList.size(); ++i)
	{
		RemindContentItemWgt *itemWidget = new RemindContentItemWgt();
		itemWidget->setData(m_warningGoodsList[i]);
		QListWidgetItem *cItem = new QListWidgetItem();
		cItem->setFlags(cItem->flags() & ~Qt::ItemIsSelectable);
		cItem->setSizeHint(QSize(ui.lwgt_contentRemind->width(), 36));
		ui.lwgt_contentRemind->addItem(cItem);
		ui.lwgt_contentRemind->setItemWidget(cItem, itemWidget);
	}
}

void InventoryRemindWgt::InitConnect()
{
	qRegisterMetaType<QList<WarningGoods_t> >("QList<WarningGoods_t>");
	connect(HttpAgent::instance(), SIGNAL(sigOnGetWarningGoodsList(int, QString, QList<WarningGoods_t>)), this, SLOT(slotOnGetWarningGoodsList(int, QString, QList<WarningGoods_t>)));
}

void InventoryRemindWgt::slotOnGetWarningGoodsList(int nCode, QString sMsg, QList<WarningGoods_t> lWarningGoodsList)
{
	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On GetWarningGoodsList failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On GetWarningGoodsList success");
		m_warningGoodsList = lWarningGoodsList;
		InitWarningOrderList();
	}
}
