#include "pageQySupmarketWgt.h"
#include "rootItemWidget.h"
#include "childItemWidget.h"
#include "listItemWidget.h"
#include "appConfigIni.h"
#include "appconfig.h"

#include <QMessageBox>
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>
#include "paymentDlg.h"
#include "ReceiptPrinterMaker.h"
#include "webSocketAgent.h"
#include "ViewSignal.h"
#include "commonFun.h"

using namespace dm::server;

pageQySupmarketWgt::pageQySupmarketWgt(QWidget *parent)
	: QWidget(parent)
	, m_addGoodsDlg(NULL)
	, m_paymentDlg(NULL)
	, m_tipLabel(NULL)
{
	ui.setupUi(this);

	init();
	initConnect();
}

pageQySupmarketWgt::~pageQySupmarketWgt()
{

}

void pageQySupmarketWgt::initData()
{
	QString barId = appConfigIni::getBarId();
	ui.stackedWidget->setCurrentWidget(ui.page_sale);
	ui.btn_sale->setChecked(true);
	HttpAgent::instance()->RequestOnGetClassifyGroups(barId, t_salePage);
}

void pageQySupmarketWgt::slotOnTreeItemClicked(QTreeWidgetItem *item, int column)
{
	item->setExpanded(!item->isExpanded());
	rootItemWidget *m_itemWidget = qobject_cast<rootItemWidget*>(ui.treeWidget->itemWidget(item, column));
	if (m_itemWidget)
	{
		m_itemWidget->setButtonChecked(item->isExpanded());
	}
}

void pageQySupmarketWgt::slotOnGetClassifyGroups(int nCode, QString sMsg, QList<ClassifyGroupInfo_t> classifyGroupList, int nPage)
{
	if (nPage != t_salePage)
	{
		return;
	}

	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On pageSale GetClassifyGroups failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On pageSale GetClassifyGroups success");
		m_classifyGroupList = classifyGroupList;
		initTreeRoot();
		HttpAgent::instance()->RequestOnGetGoodsInfo(appConfigIni::getBarId(), "", t_salePage);
	}
}

void pageQySupmarketWgt::slotOnGetGoodsInfo(int nCode, QString sMsg, QList<GoodsInfo_t> goodsInfoList)
{
	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On GetGoodsInfo failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On GetGoodsInfo success");
		m_goodsInfoList = goodsInfoList;
		initTreeChild();
	}
}

void pageQySupmarketWgt::slotOnSearchGoods(QString sText)
{
	int index = 0;
	for (int i = 0; i < ui.treeWidget->topLevelItemCount(); ++i)
	{
		QTreeWidgetItem *cItem = ui.treeWidget->topLevelItem(i);
		for (int j = 0; j < cItem->childCount(); ++j)
		{
			QTreeWidgetItem *item = cItem->child(j);
			childItemWidget *m_itemWidget = qobject_cast<childItemWidget*>(ui.treeWidget->itemWidget(item, 0));
			GoodsInfo_t goodsInfo = m_itemWidget->getData();
			if (!sText.isEmpty() && commonFun::getSearchResult(sText, goodsInfo.sName, goodsInfo.sCode))
			{
				index++;
				cItem->setExpanded(true);
				cItem->setHidden(false);
			}
			else
			{
				item->setHidden(true);
			}
		}
	}

	if (!index)
	{
		for (int i = 0; i < ui.treeWidget->topLevelItemCount(); ++i)
		{
			QTreeWidgetItem *cItem = ui.treeWidget->topLevelItem(i);
			cItem->setExpanded(false);
			for (int j = 0; j < cItem->childCount(); ++j)
			{
				QTreeWidgetItem *item = cItem->child(j);
				item->setHidden(false);
			}
		}
	}
}

void pageQySupmarketWgt::slotOnAddGoodsListItem(QTreeWidgetItem *dItem, int column)
{
	childItemWidget *m_itemWidget = qobject_cast<childItemWidget*>(ui.treeWidget->itemWidget(dItem, column));
	if (m_itemWidget)
	{
		GoodsInfo_t goodsInfo = m_itemWidget->getData();
		if (goodsInfo.nTasteGroupList.size() > 0)
		{
			if (m_addGoodsDlg == NULL)
			{
				ViewSignal::instance()->onMask(true);
				m_addGoodsDlg = new addGoodsDlg(this);
				connect(m_addGoodsDlg, SIGNAL(sigOnCloseDlg()), this, SLOT(slotOnCloseAddDlg()));
				qRegisterMetaType<GoodsInfo_t>("GoodsInfo_t");
				connect(m_addGoodsDlg, SIGNAL(sigOnFinishAddGoods(GoodsInfo_t)), this, SLOT(slotOnFinishAddGoods(GoodsInfo_t)));
				m_addGoodsDlg->setData(goodsInfo);
				m_addGoodsDlg->exec();
				ViewSignal::instance()->onMask(false);
			}
		}
		else
		{
			initListData(goodsInfo);
		}
	}
}

void pageQySupmarketWgt::slotOnDeleteItem(QString nId)
{
	//AppConfigHandler.Info("delete item goods id : " + nId.toStdString());
	int num = ui.lbl_num->text().toInt();
	double sum = ui.lbl_sum->text().toDouble();
	for (int i = 0; i < ui.listWidget->count(); ++i)
	{
		QListWidgetItem *cItem = ui.listWidget->item(i);
		listItemWidget *itemWidget = qobject_cast<listItemWidget*>(ui.listWidget->itemWidget(cItem));
		if (itemWidget)
		{
			GoodsInfo_t goodsInfo = itemWidget->getData();
			if (goodsInfo.nId == nId)
			{
				num -= goodsInfo.nOrderNum;
				sum -= goodsInfo.dTotalPrice;
				ui.lbl_num->setText(QString::number(num));
				ui.lbl_sum->setText(QString::number(sum));

				ui.listWidget->removeItemWidget(cItem);
				delete cItem;
				return;
			}
		}
	}
}

void pageQySupmarketWgt::slotOnCloseAddDlg()
{
	if (m_addGoodsDlg != NULL)
	{
		m_addGoodsDlg->close();
		m_addGoodsDlg->deleteLater();
		m_addGoodsDlg = NULL;

		ViewSignal::instance()->onMask(false);
	}
}

void pageQySupmarketWgt::slotOnFinishAddGoods(GoodsInfo_t goodsInfo)
{
	if (m_addGoodsDlg != NULL)
	{
		initListData(goodsInfo);
		m_addGoodsDlg->accept();
		m_addGoodsDlg->deleteLater();
		m_addGoodsDlg = NULL;
	}
}

void pageQySupmarketWgt::slotOnAddItem(double sum)
{
	ui.lbl_num->setText(QString::number(ui.lbl_num->text().toInt() + 1));
	ui.lbl_sum->setText(QString::number(ui.lbl_sum->text().toDouble() + sum));
}

void pageQySupmarketWgt::slotOnReduceItem(double sum)
{
	ui.lbl_num->setText(QString::number(ui.lbl_num->text().toInt() - 1));
	ui.lbl_sum->setText(QString::number(ui.lbl_sum->text().toDouble() - sum));
}

void pageQySupmarketWgt::slotOnCancel()
{
	ui.listWidget->clear();
	ui.lbl_num->setText(QString::number(0));
	ui.lbl_sum->setText("0.00");
	ui.lbl_change->setText("0.00");
	ui.lEdit_pay->clear();
	ui.lEdit_remark->clear();

	initData();
}

void pageQySupmarketWgt::slotOnGetOrderResult(int nCode, QString sMsg, QString nOrderId, int nPayStatus)
{
	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On GetOrderResult failed : " + sMsg.toStdString());
		ViewSignal::instance()->onMask(true);
		m_paymentDlg->setModelData(t_payment_failed, sMsg, ui.chk_pay->isChecked());
		QTimer::singleShot(50, this, [&](){
			m_paymentDlg->exec();
		});
	}
	else
	{
		if (nPayStatus == s_unPay)
		{
			m_closeOrderId = nOrderId;
		}
		else if (nPayStatus == s_pay)
		{
			ViewSignal::instance()->onMask(true);
			//AppConfigHandler.Info("Request On GetOrderResult success");
			m_paymentDlg->setModelData(t_payment_success, sMsg, ui.chk_pay->isChecked());
			QTimer::singleShot(50, this, [&](){
				m_paymentDlg->exec();
				slotOnCancel();
			});
		}
	}
}

void pageQySupmarketWgt::slotOnFinishOrder()
{
	if (ui.chk_pay->isChecked())
	{
		payInMobileFunc();
	}
	else
	{
		payInCashFunc();
	}
}

void pageQySupmarketWgt::slotOnScanFinished(QString code)
{
	PaymentOrder_t paymentOrder;
	paymentOrder.nBarId = appConfigIni::getBarId().toInt();
	paymentOrder.nPayType = t_aliPay;
	paymentOrder.sFactPay = ui.lbl_sum->text();
	paymentOrder.sRemark = ui.lEdit_remark->text();
	paymentOrder.sAuthCode = code;
	paymentOrder.sChange = "0";

	//AppConfigHandler.Info("aliy mobile in pay");
	QList<OrderGoods_t> orderGoodsList;
	for (int i = 0; i < ui.listWidget->count(); ++i)
	{
		QListWidgetItem *cItem = ui.listWidget->item(i);
		listItemWidget *itemWidget = qobject_cast<listItemWidget*>(ui.listWidget->itemWidget(cItem));
		if (itemWidget)
		{
			GoodsInfo_t goodsInfo_ = itemWidget->getData();
			OrderGoods_t orderGoods;
			orderGoods.nGoodsId = goodsInfo_.nId;
			orderGoods.nNum = goodsInfo_.nOrderNum;
			orderGoods.nType = goodsInfo_.nType;
			orderGoods.sPrice = QString::number(goodsInfo_.dPrice);
			orderGoods.nTasteGroupList = goodsInfo_.nSelectedTasteList;
			orderGoodsList.append(orderGoods);
		}
	}
	paymentOrder.orderGoodsList = orderGoodsList;
	HttpAgent::instance()->RequestOnDealPayment(paymentOrder);
}

void pageQySupmarketWgt::slotOnStartPinter(ReceiptInfo info)
{
	if (ui.chk_print->isChecked())
	{
		ReceiptPrinterMaker::instance()->printOrderDetialFunc(info);
	}
}

void pageQySupmarketWgt::slotOnShowInventoryRemindPage()
{
	ui.stackedWidget->setCurrentWidget(ui.page_inventory_remind);
}

void pageQySupmarketWgt::slotOnShowSalePage()
{
	ui.stackedWidget->setCurrentWidget(ui.page_sale);
	initData();
}

void pageQySupmarketWgt::slotOnShowReturnPage()
{
	ui.page_return_goods->InitData();
	ui.stackedWidget->setCurrentWidget(ui.page_return_goods);
}

void pageQySupmarketWgt::slotOnShowOrderPage()
{
	ui.page_order->InitData();
	ui.stackedWidget->setCurrentWidget(ui.page_order);
}

void pageQySupmarketWgt::slotOnGoodsListChanged()
{
	initData();
}

void pageQySupmarketWgt::slotOnCloseOrder()
{
	if (m_closeOrderId.isEmpty())
	{
		QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("该订单的订单id不存在！"));
		return;
	}
	m_paymentDlg->releaseDlg();
	ViewSignal::instance()->sigOnShowVerifyDlg(m_closeOrderId, t_salePage);
}

void pageQySupmarketWgt::slotOnReturnWaitingPay()
{
	ViewSignal::instance()->onMask(true);
	m_paymentDlg->setModelData(t_paymenting, "", true);
	m_paymentDlg->exec();
}

void pageQySupmarketWgt::slotOnPaySuccess()
{
	ViewSignal::instance()->onMask(true);
	m_paymentDlg->setModelData(t_payment_success, QStringLiteral("支付成功！"), true);
	m_paymentDlg->exec();
	slotOnCancel();
}

void pageQySupmarketWgt::slotOnShowTipLabel(bool isHiden)
{
	m_tipLabel->setHidden(!isHiden);
}

void pageQySupmarketWgt::keyPressEvent(QKeyEvent *event)
{
	Q_UNUSED(event);
	if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter){
		if (!ui.lineEdit->text().isEmpty() && ui.lineEdit->hasFocus())
		{
			for (int i = 0; i < ui.treeWidget->topLevelItemCount(); ++i)
			{
				QTreeWidgetItem *cItem = ui.treeWidget->topLevelItem(i);
				for (int j = 0; j < cItem->childCount(); ++j)
				{
					QTreeWidgetItem *item = cItem->child(j);
					childItemWidget *m_itemWidget = qobject_cast<childItemWidget*>(ui.treeWidget->itemWidget(item, 0));
					if (!item->isHidden())
					{
						GoodsInfo_t goodsInfo = m_itemWidget->getData();
						if (goodsInfo.nTasteGroupList.size() > 0)
						{
							if (m_addGoodsDlg == NULL)
							{
								ViewSignal::instance()->onMask(true);
								m_addGoodsDlg = new addGoodsDlg(this);
								connect(m_addGoodsDlg, SIGNAL(sigOnCloseDlg()), this, SLOT(slotOnCloseAddDlg()));
								qRegisterMetaType<GoodsInfo_t>("GoodsInfo_t");
								connect(m_addGoodsDlg, SIGNAL(sigOnFinishAddGoods(GoodsInfo_t)), this, SLOT(slotOnFinishAddGoods(GoodsInfo_t)));
								m_addGoodsDlg->setData(goodsInfo);
								m_addGoodsDlg->exec();
								ViewSignal::instance()->onMask(false);
							}
						}
						else
						{
							initListData(goodsInfo);
						}
						break;
					}
				}
			}
		}
		else if (!ui.lEdit_pay->text().isEmpty())
		{
			double fact = ui.lEdit_pay->text().toDouble();
			double sum = ui.lbl_sum->text().toDouble();
			double change = fact - sum;
			if (change < 0 || ui.listWidget->count() == 0)
			{
				QMessageBox::warning(NULL, "warning", QStringLiteral("输入支付金额错误！"));
				return;
			}
			ui.lbl_change->setText(QString::number(change));
		}
	}
}

void pageQySupmarketWgt::init()
{
	ui.btn_sale->setChecked(true);
	ui.stackedWidget->setCurrentWidget(ui.page_sale);

	ui.treeWidget->setRootIsDecorated(false);
	ui.treeWidget->setFrameStyle(QFrame::NoFrame);
	ui.treeWidget->setFocusPolicy(Qt::NoFocus);

	ui.lineEdit->setFocus();
	ui.chk_print->setChecked(true);
	ui.chk_pay->setChecked(true);
	ui.btn_detial->hide();

	m_tipLabel = new QLabel(this);
	m_tipLabel->setGeometry(QRect(725, 6, 16, 16));
	m_tipLabel->setStyleSheet(QString("border-radius:8px;background:red;color:#FFFFFF;font-size:12px;font-family:%1").arg(QStringLiteral("微软雅黑")));
	m_tipLabel->setText("1");
	m_tipLabel->setAlignment(Qt::AlignCenter);
	m_tipLabel->hide();

	if (m_paymentDlg == NULL)
	{
		m_paymentDlg = new paymentDlg(NULL);
		connect(m_paymentDlg, SIGNAL(sigOnScanFinished(QString)), this, SLOT(slotOnScanFinished(QString)));
		connect(m_paymentDlg, SIGNAL(sigOnCloseOrder()), this, SLOT(slotOnCloseOrder()));
	}
}

void pageQySupmarketWgt::initConnect()
{
	qRegisterMetaType<ReceiptInfo>("ReceiptInfo");
	connect(webSocketAgent::instance(), SIGNAL(sigOnPrintTips(ReceiptInfo)), this, SLOT(slotOnStartPinter(ReceiptInfo)));
	connect(webSocketAgent::instance(), SIGNAL(sigOnGoodsListChanged()), this, SLOT(slotOnGoodsListChanged()));
	connect(webSocketAgent::instance(), SIGNAL(sigOnPaySuccess()), this, SLOT(slotOnPaySuccess()));

	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotOnTreeItemClicked(QTreeWidgetItem*, int)));
	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(slotOnAddGoodsListItem(QTreeWidgetItem*, int)));
	connect(ui.lineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotOnSearchGoods(QString)));
	connect(ui.btn_cancel, SIGNAL(clicked()), this, SLOT(slotOnCancel()));
	connect(ui.btn_finish, SIGNAL(clicked()), this, SLOT(slotOnFinishOrder()));
	connect(ui.btn_stock, SIGNAL(clicked()), this, SLOT(slotOnShowInventoryRemindPage()));
	connect(ui.btn_sale, SIGNAL(clicked()), this, SLOT(slotOnShowSalePage()));
	connect(ui.btn_rejected, SIGNAL(clicked()), this, SLOT(slotOnShowReturnPage()));
	connect(ui.btn_order, SIGNAL(clicked()), this, SLOT(slotOnShowOrderPage()));

	connect(HttpAgent::instance(), SIGNAL(sigOnGetOrderResult(int, QString, QString, int)), this, SLOT(slotOnGetOrderResult(int, QString, QString, int)));
	connect(ViewSignal::instance(), SIGNAL(sigOnReturnWaitingPay()), this, SLOT(slotOnReturnWaitingPay()));
	connect(ViewSignal::instance(), SIGNAL(sigOnCancelOrderSuccess()), this, SLOT(slotOnCancel()));
	connect(ViewSignal::instance(), SIGNAL(sigOnShowTipLabel(bool)), this, SLOT(slotOnShowTipLabel(bool)));

	qRegisterMetaType<QList<ClassifyGroupInfo_t> >("QList<ClassifyGroupInfo_t>");
	connect(HttpAgent::instance(), SIGNAL(sigOnGetClassifyGroups(int, QString, QList<ClassifyGroupInfo_t>, int)), this, SLOT(slotOnGetClassifyGroups(int, QString, QList<ClassifyGroupInfo_t>, int)));

	qRegisterMetaType<QList<GoodsInfo_t> >("QList<GoodsInfo_t>");
	connect(HttpAgent::instance(), SIGNAL(sigOnGetGoodsInfo(int, QString, QList<GoodsInfo_t>)), this, SLOT(slotOnGetGoodsInfo(int, QString, QList<GoodsInfo_t>)));
}

void pageQySupmarketWgt::initTreeRoot()
{
	ui.treeWidget->clear();
	for (int i = 0; i < m_classifyGroupList.size(); ++i)
	{
		rootItemWidget *root = new rootItemWidget();
		root->setData(m_classifyGroupList[i]);
		QTreeWidgetItem *rootItem = new QTreeWidgetItem();
		ui.treeWidget->addTopLevelItem(rootItem);
		ui.treeWidget->setItemWidget(rootItem, 0, root);
	}
}

void pageQySupmarketWgt::initTreeChild()
{
	for (int i = 0; i < ui.treeWidget->topLevelItemCount(); ++i)
	{
		QTreeWidgetItem *cItem = ui.treeWidget->topLevelItem(i);
		rootItemWidget *m_itemWidget = qobject_cast<rootItemWidget*>(ui.treeWidget->itemWidget(cItem, 0));
		if (m_itemWidget)
		{
			QString classifyId = m_itemWidget->getClassifyId();
			for (int j = 0; j < m_goodsInfoList.size(); ++j)
			{
				if (classifyId == m_goodsInfoList[j].nClassifyId)
				{
					   QTreeWidgetItem *child = new QTreeWidgetItem();
					   cItem->addChild(child);
					   childItemWidget *childItem = new childItemWidget();
					   childItem->setData(m_goodsInfoList[j]);
					   ui.treeWidget->setItemWidget(child, 0, childItem);
				}
			}
		}
	}
}

void pageQySupmarketWgt::initListData(const GoodsInfo_t &goodsInfo)
{
	if (goodsInfo.nStockNum == 0 && goodsInfo.nType == 1)
	{
		QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("该商品库存不足！"));
		ViewSignal::instance()->onMask(false);
		return;
	}

	int index = -1;
	int num = 0;
	double sum = 0.00;
	for (int i = 0; i < ui.listWidget->count(); ++i)
	{
		QListWidgetItem *cItem = ui.listWidget->item(i);
		listItemWidget *itemWidget = qobject_cast<listItemWidget*>(ui.listWidget->itemWidget(cItem));
		if (itemWidget)
		{
			GoodsInfo_t goodsInfo_ = itemWidget->getData();
			if (goodsInfo_.nId == goodsInfo.nId)
			{
				//已存在该商品，自增数量1
				itemWidget->slotOnAddNum();
				index++;
			}
			num += itemWidget->getData().nOrderNum;
			sum += itemWidget->getData().dTotalPrice;
		}	
	}

	//商品不存在，新建列表项
	if (index == -1)
	{
		listItemWidget *itemWidget = new listItemWidget();
		connect(itemWidget, SIGNAL(sigOnDeleteItem(QString)), this, SLOT(slotOnDeleteItem(QString)));
		connect(itemWidget, SIGNAL(sigOnAddNum(double)), this, SLOT(slotOnAddItem(double)));
		connect(itemWidget, SIGNAL(sigOnReduceNum(double)), this, SLOT(slotOnReduceItem(double)));
		itemWidget->setData(goodsInfo);
		num += goodsInfo.nOrderNum;
		sum += goodsInfo.dPrice;

		QListWidgetItem *listItem = new QListWidgetItem();
		listItem->setSizeHint(QSize(ui.listWidget->width() - 2, 42));
		ui.listWidget->addItem(listItem);
		ui.listWidget->setItemWidget(listItem, itemWidget);
	}

	ui.lbl_num->setText(QString::number(num));
	ui.lbl_sum->setText(QString::number(sum));
}

void pageQySupmarketWgt::payInCashFunc()
{
	if (ui.lEdit_pay->text().isEmpty())
	{
		QMessageBox::warning(NULL, "warning", QStringLiteral("请输入支付金额！"));
		return;
	}

	PaymentOrder_t paymentOrder;
	paymentOrder.nBarId = appConfigIni::getBarId().toInt();
	paymentOrder.nPayType = t_cashPay;
	paymentOrder.sFactPay = ui.lEdit_pay->text();
	paymentOrder.sRemark = ui.lEdit_remark->text();
	paymentOrder.sAuthCode = "";
	
	double fact = ui.lEdit_pay->text().toDouble();
	double sum = ui.lbl_sum->text().toDouble();
	double change = fact - sum;

	if (fact < sum)
	{
		QMessageBox::warning(NULL, "warning", QStringLiteral("输入支付金额错误！"));
		return;
	}

	ui.lbl_change->setText(QString::number(change));
	paymentOrder.sChange = QString::number(change);
	//AppConfigHandler.Info("cash in pay");
	//AppConfigHandler.Info("fact pay : " + QString::number(fact).toStdString());
	//AppConfigHandler.Info("goods sum : " + QString::number(sum).toStdString());
	//AppConfigHandler.Info("change : " + QString::number(change).toStdString());

	QList<OrderGoods_t> orderGoodsList;
	for (int i = 0; i < ui.listWidget->count(); ++i)
	{
		QListWidgetItem *cItem = ui.listWidget->item(i);
		listItemWidget *itemWidget = qobject_cast<listItemWidget*>(ui.listWidget->itemWidget(cItem));
		if (itemWidget)
		{
			GoodsInfo_t goodsInfo_ = itemWidget->getData();
			OrderGoods_t orderGoods;
			orderGoods.nGoodsId = goodsInfo_.nId;
			orderGoods.nNum = goodsInfo_.nOrderNum;
			orderGoods.nType = goodsInfo_.nType;
			orderGoods.sPrice = QString::number(goodsInfo_.dPrice);
			orderGoods.nTasteGroupList = goodsInfo_.nSelectedTasteList;
			orderGoodsList.append(orderGoods);
		}
	}
	paymentOrder.orderGoodsList = orderGoodsList;
	HttpAgent::instance()->RequestOnDealPayment(paymentOrder);
}

void pageQySupmarketWgt::payInMobileFunc()
{

	ViewSignal::instance()->onMask(true);
	m_paymentDlg->setModelData(t_paymenting, "", true);
	QTimer::singleShot(50, this, [&](){
		m_paymentDlg->exec();
	});
}
