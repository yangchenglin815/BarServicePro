#include "PageReturnGoodsWgt.h"
#include "HttpAgent.h"
#include "appConfigIni.h"
#include "appconfig.h"
#include "commonFun.h"

#include <QMessageBox>
#include <QKeyEvent>
#include "rootItemWidget.h"
#include "childItemWidget.h"
#include "listItemWidget.h"
#include "ViewSignal.h"

using namespace dm::server;

PageReturnGoodsWgt::PageReturnGoodsWgt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.treeWidget->setRootIsDecorated(false);
	ui.treeWidget->setFrameStyle(QFrame::NoFrame);
	ui.treeWidget->setFocusPolicy(Qt::NoFocus);
	ui.chk_print->setChecked(true);
	InitConnect();
}

PageReturnGoodsWgt::~PageReturnGoodsWgt()
{

}

void PageReturnGoodsWgt::InitData()
{
	ui.lineEdit->setFocus();
	HttpAgent::instance()->RequestOnGetClassifyGroups(appConfigIni::getBarId(), t_returnPage);
}

void PageReturnGoodsWgt::InitConnect()
{
	qRegisterMetaType<QList<ClassifyGroupInfo_t> >("QList<ClassifyGroupInfo_t>");
	connect(HttpAgent::instance(), SIGNAL(sigOnGetClassifyGroups(int, QString, QList<ClassifyGroupInfo_t>, int)), this, SLOT(slotOnGetClassifyGroups(int, QString, QList<ClassifyGroupInfo_t>, int)));

	qRegisterMetaType<QList<GoodsInfo_t> >("QList<GoodsInfo_t>");
	connect(HttpAgent::instance(), SIGNAL(sigOnGetReturnGoodsInfo(int, QString, QList<GoodsInfo_t>)), this, SLOT(slotOnGetReturnGoodsInfo(int, QString, QList<GoodsInfo_t>)));

	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotOnTreeItemClicked(QTreeWidgetItem*, int)));
	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(slotOnAddGoodsListItem(QTreeWidgetItem*, int)));
	connect(ui.lineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotOnSearchGoods(QString)));
	connect(ui.btn_cancel, SIGNAL(clicked()), this, SLOT(slotOnCancel()));
	connect(ui.btn_finish, SIGNAL(clicked()), this, SLOT(slotOnFinishOrder()));
}

void PageReturnGoodsWgt::InitTreeRoot()
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

void PageReturnGoodsWgt::InitTreeChild()
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

void PageReturnGoodsWgt::InitListData(const GoodsInfo_t &goodsInfo)
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

void PageReturnGoodsWgt::keyPressEvent(QKeyEvent *event)
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
							InitListData(goodsInfo);
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

void PageReturnGoodsWgt::slotOnGetClassifyGroups(int nCode, QString sMsg, QList<ClassifyGroupInfo_t> classifyGroupList, int nPage)
{
	if (nPage != t_returnPage)
	{
		return;
	}
	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On returnPage GetClassifyGroups failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On returnPage GetClassifyGroups success");
		m_classifyGroupList = classifyGroupList;
		InitTreeRoot();
		HttpAgent::instance()->RequestOnGetGoodsInfo(appConfigIni::getBarId(), "", t_returnPage);
	}
}

void PageReturnGoodsWgt::slotOnGetReturnGoodsInfo(int nCode, QString sMsg, QList<GoodsInfo_t> goodsInfoList)
{
	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On GetReturnGoodsInfo failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On GetReturnGoodsInfo success");
		m_goodsInfoList = goodsInfoList;
		InitTreeChild();
	}
}

void PageReturnGoodsWgt::slotOnTreeItemClicked(QTreeWidgetItem *item, int column)
{
	item->setExpanded(!item->isExpanded());
	rootItemWidget *m_itemWidget = qobject_cast<rootItemWidget*>(ui.treeWidget->itemWidget(item, column));
	if (m_itemWidget)
	{
		m_itemWidget->setButtonChecked(item->isExpanded());
	}
}

void PageReturnGoodsWgt::slotOnAddGoodsListItem(QTreeWidgetItem *dItem, int column)
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
			InitListData(goodsInfo);
		}
	}
}

void PageReturnGoodsWgt::slotOnSearchGoods(QString sText)
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

void PageReturnGoodsWgt::slotOnDeleteItem(QString nId)
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

void PageReturnGoodsWgt::slotOnCloseAddDlg()
{
	if (m_addGoodsDlg != NULL)
	{
		m_addGoodsDlg->close();
		m_addGoodsDlg->deleteLater();
		m_addGoodsDlg = NULL;

		ViewSignal::instance()->onMask(false);
	}
}

void PageReturnGoodsWgt::slotOnFinishAddGoods(GoodsInfo_t goodsInfo)
{
	if (m_addGoodsDlg != NULL)
	{
		InitListData(goodsInfo);
		m_addGoodsDlg->accept();
		m_addGoodsDlg->deleteLater();
		m_addGoodsDlg = NULL;
	}
}

void PageReturnGoodsWgt::slotOnAddItem(double sum)
{
	ui.lbl_num->setText(QString::number(ui.lbl_num->text().toInt() + 1));
	ui.lbl_sum->setText(QString::number(ui.lbl_sum->text().toDouble() + sum));
}

void PageReturnGoodsWgt::slotOnReduceItem(double sum)
{
	ui.lbl_num->setText(QString::number(ui.lbl_num->text().toInt() - 1));
	ui.lbl_sum->setText(QString::number(ui.lbl_sum->text().toDouble() - sum));
}

void PageReturnGoodsWgt::slotOnCancel()
{
	ui.listWidget->clear();
	ui.lbl_num->setText(QString::number(0));
	ui.lbl_sum->setText("0.00");
	ui.lbl_change->setText("0.00");
	ui.lEdit_pay->clear();
	ui.lEdit_remark->clear();
}

void PageReturnGoodsWgt::slotOnFinishOrder()
{

}
