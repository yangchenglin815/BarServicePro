#include "childItemWidget.h"

childItemWidget::childItemWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

childItemWidget::~childItemWidget()
{

}

void childItemWidget::setData(const GoodsInfo_t &goodsInfo)
{
	m_goodsInfo = goodsInfo;
	ui.lbl_child->setText(QStringLiteral("%1").arg(m_goodsInfo.sName));
}

const GoodsInfo_t& childItemWidget::getData()
{
	return m_goodsInfo;
}
