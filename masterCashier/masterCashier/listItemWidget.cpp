#include "listItemWidget.h"

listItemWidget::listItemWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	initConnect();
}

listItemWidget::~listItemWidget()
{

}

void listItemWidget::setData(const GoodsInfo_t &goodsInfo)
{
	m_goodsInfo = goodsInfo;
	ui.lbl_goodsName->setText(QStringLiteral("%1").arg(m_goodsInfo.sName));
	ui.lbl_stockNum->setText(QString::number(m_goodsInfo.nStockNum));
	ui.lbl_num->setText(QString::number(m_goodsInfo.nOrderNum));
	m_goodsInfo.dTotalPrice = m_goodsInfo.nOrderNum * m_goodsInfo.dPrice;
	ui.lbl_sum->setText(QString::number(m_goodsInfo.dTotalPrice));
}

const GoodsInfo_t& listItemWidget::getData()
{
	return m_goodsInfo;
}

void listItemWidget::slotOnAddNum()
{
	if (m_goodsInfo.nOrderNum < m_goodsInfo.nStockNum || m_goodsInfo.nType == 2)
	{
		m_goodsInfo.nOrderNum++;
		m_goodsInfo.dTotalPrice = m_goodsInfo.nOrderNum * m_goodsInfo.dPrice;
		ui.lbl_num->setText(QString::number(m_goodsInfo.nOrderNum));
		ui.lbl_sum->setText(QString::number(m_goodsInfo.dTotalPrice));

		emit sigOnAddNum(m_goodsInfo.dPrice);
	}
}

void listItemWidget::slotOnReduceNum()
{
	if (m_goodsInfo.nOrderNum >1)
	{
		m_goodsInfo.nOrderNum--;
		m_goodsInfo.dTotalPrice = m_goodsInfo.nOrderNum * m_goodsInfo.dPrice;
		ui.lbl_num->setText(QString::number(m_goodsInfo.nOrderNum));
		ui.lbl_sum->setText(QString::number(m_goodsInfo.dTotalPrice));

		emit sigOnReduceNum(m_goodsInfo.dPrice);
	}
}

void listItemWidget::slotOnDeleteItem()
{
	emit sigOnDeleteItem(m_goodsInfo.nId);
}

void listItemWidget::initConnect()
{
	connect(ui.btn_add, SIGNAL(clicked()), this, SLOT(slotOnAddNum()));
	connect(ui.btn_delete, SIGNAL(clicked()), this, SLOT(slotOnDeleteItem()));
	connect(ui.btn_reduce, SIGNAL(clicked()), this, SLOT(slotOnReduceNum()));
}
