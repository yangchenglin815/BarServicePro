#include "shiftListItemWidget.h"

shiftListItemWidget::shiftListItemWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.lEd_shiftNum, SIGNAL(editingFinished()), this, SLOT(slotOnEditFinished()));
}

shiftListItemWidget::~shiftListItemWidget()
{

}

void shiftListItemWidget::setData(GoodsInfo_t goodsInfo)
{
	m_goodsInfo = goodsInfo;

	ui.lbl_no->setText(QString::number(m_goodsInfo.nFortId));
	ui.lbl_goodsName->setText(QStringLiteral("%1").arg(m_goodsInfo.sName));
	ui.lbl_classify->setText(QStringLiteral("%1").arg(m_goodsInfo.sClassifyName));
	ui.lbl_unit->setText(QStringLiteral("%1").arg(m_goodsInfo.sUnit));
	ui.lbl_price->setText(QString::number(m_goodsInfo.dPrice));
	ui.lbl_stock->setText(QString::number(m_goodsInfo.nStockNum));
	if (!m_goodsInfo.nCheckNum.isEmpty())
	{
		ui.lEd_shiftNum->setText(m_goodsInfo.nCheckNum);
	}
}

void shiftListItemWidget::slotOnEditFinished()
{
	QString text = ui.lEd_shiftNum->text();
	m_goodsInfo.nCheckNum = text;
	emit sigOnEditFinished(m_goodsInfo);
}
