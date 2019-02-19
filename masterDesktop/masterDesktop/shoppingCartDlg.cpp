#include "shoppingCartDlg.h"
#include "shoppingCartItemWgt.h"
#include "ViewSignal.h"
#include <QListWidgetItem>

shoppingCartDlg::shoppingCartDlg(QWidget *parent)
	: QDialog(parent)
	, m_goodsTotal(0)
	, m_goodsTotalPrice(0.00)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground, true);

	InitConnect();
}

shoppingCartDlg::~shoppingCartDlg()
{

}

void shoppingCartDlg::addToShoppingCart(GoodsInfo_t goodsInfo)
{
	bool isExist = false;
	for (int i = 0; i < m_goodsInfoList.size(); ++i)
	{
		if (m_goodsInfoList[i].nId == goodsInfo.nId)
		{
			isExist = true;
			m_goodsInfoList[i].nOrderNum++;
			m_goodsInfoList[i].dTotalPrice = m_goodsInfoList[i].dPrice * m_goodsInfoList[i].nOrderNum;
			break;
		}
	}

	if (!isExist)
	{
		goodsInfo.nOrderNum = 1;
		m_goodsInfoList.append(goodsInfo);
	}
	m_goodsTotal ++;
	m_goodsTotalPrice += goodsInfo.dPrice;

	InitShoppingCart();
}

void shoppingCartDlg::reduceToShoppongCart(GoodsInfo_t goodsInfo)
{
	bool isExist = false;
	int index = -1;
	for (int i = 0; i < m_goodsInfoList.size(); ++i)
	{
		if (m_goodsInfoList[i].nId == goodsInfo.nId)
		{
			isExist = true;
			m_goodsInfoList[i].nOrderNum--;
			m_goodsInfoList[i].dTotalPrice = m_goodsInfoList[i].dPrice * m_goodsInfoList[i].nOrderNum;
			if (m_goodsInfoList[i].nOrderNum == 0)
			{
				index = i;
			}
			break;
		}
	}

	if (!isExist)
	{
		return;
	}
	m_goodsTotal--;
	m_goodsTotalPrice -= goodsInfo.dPrice;

	if (index != -1)
	{
		m_goodsInfoList.removeAt(index);
	}

	InitShoppingCart();
}

void shoppingCartDlg::slotOnClearShoppingCart()
{
	m_goodsTotal = 0;
	m_goodsTotalPrice = 0.00;
	m_goodsInfoList.clear();

	InitShoppingCart();
	ViewSignal::instance()->sigOnClearShoppingCart();
}

void shoppingCartDlg::slotOnSyncAddGoodsInfo(GoodsInfo_t goodsInfo)
{
	int goodsNumDiff = 0;
	double priceDiff = 0.00;
	for (int i = 0; i < m_goodsInfoList.size(); ++i)
	{
		if (m_goodsInfoList[i].nId == goodsInfo.nId)
		{
			goodsNumDiff = goodsInfo.nOrderNum - m_goodsInfoList[i].nOrderNum;
			priceDiff = goodsInfo.dTotalPrice - m_goodsInfoList[i].dTotalPrice;
			m_goodsInfoList[i].dTotalPrice = goodsInfo.dTotalPrice;
			m_goodsInfoList[i].nOrderNum = goodsInfo.nOrderNum;
			break;
		}
	}

	m_goodsTotal += goodsNumDiff;
	m_goodsTotalPrice += priceDiff;

	ui.lbl_sum->setText(QStringLiteral("¥") + QString::number(m_goodsTotalPrice, 'f', 2));
	ui.lbl_num->setText(QStringLiteral("共计%1件商品").arg(m_goodsTotal));
	emit sigOnShoppingCartChanged(m_goodsTotal, m_goodsTotalPrice);
	ViewSignal::instance()->sigOnGoodsNumChanged(goodsInfo);
}

void shoppingCartDlg::slotOnSyncReduceGoodsInfo(GoodsInfo_t goodsInfo)
{
	int goodsNumDiff = 0;
	double priceDiff = 0.00;
	int index = -1;
	for (int i = 0; i < m_goodsInfoList.size(); ++i)
	{
		if (m_goodsInfoList[i].nId == goodsInfo.nId)
		{
			if (goodsInfo.nOrderNum == 0)
			{
				index = i;
			}
			goodsNumDiff = m_goodsInfoList[i].nOrderNum - goodsInfo.nOrderNum;
			priceDiff = m_goodsInfoList[i].dTotalPrice - goodsInfo.dTotalPrice;
			m_goodsInfoList[i].dTotalPrice = goodsInfo.dTotalPrice;
			m_goodsInfoList[i].nOrderNum = goodsInfo.nOrderNum;
			break;
		}
	}
	
	m_goodsTotal -= goodsNumDiff;
	m_goodsTotalPrice -= priceDiff;
	ViewSignal::instance()->sigOnGoodsNumChanged(goodsInfo);

	if (index == -1)
	{
		ui.lbl_sum->setText(QStringLiteral("¥") + QString::number(m_goodsTotalPrice, 'f', 2));
		ui.lbl_num->setText(QStringLiteral("共计%1件商品").arg(m_goodsTotal));
		return;
	}

	m_goodsInfoList.removeAt(index);
	InitShoppingCart();
}

void shoppingCartDlg::InitShoppingCart()
{
	ui.listWidget->clear();
 	for (int i = 0; i < m_goodsInfoList.size(); ++i)
	{
		QListWidgetItem *item = new QListWidgetItem;
		item->setSizeHint(QSize(460, 76));
		shoppingCartItemWgt *itemWidget = new shoppingCartItemWgt;
		connect(itemWidget, SIGNAL(sigOnSyncAddGoodsInfo(GoodsInfo_t)), this, SLOT(slotOnSyncAddGoodsInfo(GoodsInfo_t)));
		connect(itemWidget, SIGNAL(sigOnSyncReduceGoodsInfo(GoodsInfo_t)), this, SLOT(slotOnSyncReduceGoodsInfo(GoodsInfo_t)));
		itemWidget->setData(m_goodsInfoList[i]);
		ui.listWidget->addItem(item);
		ui.listWidget->setItemWidget(item, itemWidget);
	}
	ui.lbl_sum->setText(QStringLiteral("¥") + QString::number(m_goodsTotalPrice, 'f', 2));
	ui.lbl_num->setText(QStringLiteral("共计%1件商品").arg(m_goodsTotal));
	
	emit sigOnShoppingCartChanged(m_goodsTotal, m_goodsTotalPrice);
}

void shoppingCartDlg::InitConnect()
{
	connect(ui.btn_clearCart, SIGNAL(clicked()), this, SLOT(slotOnClearShoppingCart()));
}
