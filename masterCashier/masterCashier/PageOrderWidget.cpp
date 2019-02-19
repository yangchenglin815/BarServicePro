#include "PageOrderWidget.h"
#include "HttpAgent.h"
#include "appconfig.h"
#include "InventoryRemindItemWgt.h"
#include "RemindContentItemWgt.h"
#include "commonFun.h"
#include "appConfigIni.h"
#include "ViewSignal.h"
#include <QMessageBox>
#include <myProgressDlg.h>

using namespace dm::server;

PageOrderWidget::PageOrderWidget(QWidget *parent)
	: QWidget(parent)
	, m_progressDlg(new myProgressDlg(this))
{
	ui.setupUi(this);

	InitUI();
	InitConnect();
}

PageOrderWidget::~PageOrderWidget()
{

}

void PageOrderWidget::InitOrderList()
{
	ui.lwt_order->clear();
	for (int i = 0; i < m_orderInfoList.size(); ++i)
	{
		InventoryRemindItemWgt *itemWidget = new InventoryRemindItemWgt(true);
		itemWidget->setData(m_orderInfoList[i]);
		QListWidgetItem *cItem = new QListWidgetItem();
		cItem->setSizeHint(QSize(ui.lwt_order->width() - 2, 36));
		ui.lwt_order->addItem(cItem);
		ui.lwt_order->setItemWidget(cItem, itemWidget);
	}
	m_progressDlg->accept();
}

void PageOrderWidget::InitDetialList(const QList<OrderDetialInfo_t> &orderDetialInfoList)
{
	ui.listWidget->clear();

	int index = 0;
	for (int i = 0; i < orderDetialInfoList.size(); ++i)
	{
		RemindContentItemWgt *itemWidget = new RemindContentItemWgt();
		itemWidget->setData(orderDetialInfoList[i]);
		index += orderDetialInfoList[i].nOrderNum;
		QListWidgetItem *cItem = new QListWidgetItem();
		cItem->setFlags(cItem->flags() & ~Qt::ItemIsSelectable);
		cItem->setSizeHint(QSize(ui.listWidget->width() - 2, 36));
		ui.listWidget->addItem(cItem);
		ui.listWidget->setItemWidget(cItem, itemWidget);
	}

	if (m_curr_orderInfo.nPayStatus == s_pay || m_curr_orderInfo.nPayStatus == s_return)
	{
		ui.wgt_input->setCurrentWidget(ui.page_finished);
		ui.lbl_num->setText(QString::number(index));
		ui.lbl_orderId->setText(m_curr_orderInfo.sOrderNum);
		ui.lbl_orderTime->setText(QDateTime::fromTime_t(m_curr_orderInfo.sCreateTime.toULongLong()/1000).toString("yyyy-MM-dd hh:mm:ss"));
		ui.lbl_payWay->setText(QStringLiteral("%1").arg(commonFun::getPayWay(m_curr_orderInfo.nPayType)));
		ui.lbl_sum->setText(QString::number(m_curr_orderInfo.dSum));
		ui.lbl_remark->setText(QStringLiteral("【%1】").arg(m_curr_orderInfo.seatNo) + m_curr_orderInfo.sRemark);
	}
	else if (m_curr_orderInfo.nPayStatus == s_unPay)
	{
		ui.wgt_input->setCurrentWidget(ui.page_unfinished);
		ui.lbl_sum_2->setText(QString::number(m_curr_orderInfo.dSum));
		ui.lbl_num_2->setText(QString::number(index));
	}
}

void PageOrderWidget::InitData()
{
	ViewSignal::instance()->onMask(true);
	HttpAgent::instance()->RequestOnGetOrderList();

	m_progressDlg->exec();
	ViewSignal::instance()->onMask(false);
}

void PageOrderWidget::InitUI()
{
	QButtonGroup *buttonGroup = new QButtonGroup();
	buttonGroup->addButton(ui.btn_allOrder);
	buttonGroup->addButton(ui.btn_finishedOrder);
	buttonGroup->addButton(ui.btn_ordering);
	ui.btn_allOrder->setChecked(true);
	ui.chk_pay->setChecked(true);
	ui.chk_print->setChecked(true);
}

void PageOrderWidget::InitConnect()
{
	connect(ui.lwt_order, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slotOnItemClicked(QListWidgetItem*)));
	connect(ui.btn_allOrder, SIGNAL(clicked()), this, SLOT(slotOnShowAllOrder()));
	connect(ui.btn_finishedOrder, SIGNAL(clicked()), this, SLOT(slotOnShowPayFinishedOrder()));
	connect(ui.btn_ordering, SIGNAL(clicked()), this, SLOT(slotOnShowPayUnFinishedOrder()));
	connect(ui.btn_return, SIGNAL(clicked()), this, SLOT(slotOnReturn()));
	connect(ui.btn_cancel, SIGNAL(clicked()), this, SLOT(slotOnCancalOrder()));

	connect(HttpAgent::instance(), SIGNAL(sigOnGetRefundRespond(int, QString)), this, SLOT(slotOnGetRefundRespond(int, QString)));

	qRegisterMetaType<QList<OrderInfo_t> >("QList<OrderInfo_t>");
	connect(HttpAgent::instance(), SIGNAL(sigOnGetOrderList(int, QString, QList<OrderInfo_t>)), this, SLOT(slotOnGetOrderList(int, QString, QList<OrderInfo_t>)));

	qRegisterMetaType<QList<OrderDetialInfo_t> >("QList<OrderDetialInfo_t>");
	connect(HttpAgent::instance(), SIGNAL(sigOnGetOrderDetialList(int, QString, QList<OrderDetialInfo_t>)), this, SLOT(slotOnGetOrderDetialList(int, QString, QList<OrderDetialInfo_t>)));
}

void PageOrderWidget::slotOnGetOrderList(int nCode, QString sMsg, QList<OrderInfo_t> lOrderInfoList)
{
	if (nCode != e_success)
	{
		m_progressDlg->accept();
		//AppConfigHandler.Info("Request On GetOrderList failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On GetOrderList success");
		m_orderInfoList = lOrderInfoList;
		InitOrderList();
	}
}

void PageOrderWidget::slotOnGetOrderDetialList(int nCode, QString sMsg, QList<OrderDetialInfo_t> lOrderDetialInfoList)
{
	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On GetOrderDetialList failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On GetOrderDetialList success");
		InitDetialList(lOrderDetialInfoList);
	}
}

void PageOrderWidget::slotOnItemClicked(QListWidgetItem *cItem)
{
	InventoryRemindItemWgt *itemWidget = qobject_cast<InventoryRemindItemWgt*>(ui.lwt_order->itemWidget(cItem));
	if (itemWidget)
	{
		m_curr_orderInfo = itemWidget->getData();
		//AppConfigHandler.Info("current payStatus: " + QString::number(m_curr_orderInfo.nPayStatus).toStdString());
		HttpAgent::instance()->RequestOnGetOrderDetialList(m_curr_orderInfo.sOrderId);
	}
}

void PageOrderWidget::slotOnShowAllOrder()
{
	for (int i = 0; i < ui.lwt_order->count(); ++i)
	{
		QListWidgetItem *item = ui.lwt_order->item(i);
		item->setHidden(false);
	}
}

void PageOrderWidget::slotOnShowPayFinishedOrder()
{
	for (int i = 0; i < ui.lwt_order->count(); ++i)
	{
		QListWidgetItem *item = ui.lwt_order->item(i);
		InventoryRemindItemWgt *itemWidget = qobject_cast<InventoryRemindItemWgt*>(ui.lwt_order->itemWidget(item));
		if (itemWidget)
		{
			OrderInfo_t orderInfo = itemWidget->getData();
			if (orderInfo.nPayStatus == s_unPay)
			{
				item->setHidden(true);
			}
			else if (orderInfo.nPayStatus == s_pay || orderInfo.nPayStatus == s_return)
			{
				item->setHidden(false);
			}
		}
	}
}

void PageOrderWidget::slotOnShowPayUnFinishedOrder()
{
	for (int i = 0; i < ui.lwt_order->count(); ++i)
	{
		QListWidgetItem *item = ui.lwt_order->item(i);
		InventoryRemindItemWgt *itemWidget = qobject_cast<InventoryRemindItemWgt*>(ui.lwt_order->itemWidget(item));
		if (itemWidget)
		{
			OrderInfo_t orderInfo = itemWidget->getData();
			if (orderInfo.nPayStatus == s_unPay)
			{
				item->setHidden(false);
			}
			else if (orderInfo.nPayStatus == s_pay || orderInfo.nPayStatus == s_return)
			{
				item->setHidden(true);
			}
		}
	}
}

void PageOrderWidget::slotOnReturn()
{
	QString orderId = m_curr_orderInfo.sOrderId;
	if (!orderId.isEmpty())
	{
		HttpAgent::instance()->RequestOnRefund(appConfigIni::getBarId(), orderId, ui.lbl_remark->text());
	}
}

void PageOrderWidget::slotOnGetRefundRespond(int nCode, QString sMsg)
{
	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On GetRefundRespond failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On GetRefundRespond success");
		QMessageBox::warning(NULL, QStringLiteral("操作成功"), QStringLiteral("退款操作已成功！"));
	}
}

void PageOrderWidget::slotOnCancalOrder()
{
	if (m_curr_orderInfo.sOrderId.isEmpty())
	{
		QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("该订单的订单id不存在！"));
		return;
	}
	ViewSignal::instance()->sigOnShowVerifyDlg(m_curr_orderInfo.sOrderId, t_orderPage);
}
