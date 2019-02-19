#include "RemindContentItemWgt.h"

RemindContentItemWgt::RemindContentItemWgt(QWidget *parent)
: QWidget(parent)
{
	ui.setupUi(this);
}

RemindContentItemWgt::~RemindContentItemWgt()
{

}

void RemindContentItemWgt::setData(WarningGoods_t warningGoodsInfo)
{
	m_warningGoodsInfo = warningGoodsInfo;
	ui.lbl_goodsName->setText(m_warningGoodsInfo.sGoodsName);
	ui.lbl_goodsNum->setText(QString::number(m_warningGoodsInfo.nInventory));
	ui.lbl_price->setText(QString::number(m_warningGoodsInfo.dPrice));
}

void RemindContentItemWgt::setData(OrderDetialInfo_t orderDetialInfo)
{
	m_orderDetialInfo = orderDetialInfo;
	ui.lbl_goodsName->setText(m_orderDetialInfo.sGoodsName);
	ui.lbl_goodsNum->setText(QString::number(m_orderDetialInfo.nOrderNum));
	ui.lbl_price->setText(QString::number(m_orderDetialInfo.dPrice));
}

OrderDetialInfo_t RemindContentItemWgt::getData()
{
	return m_orderDetialInfo;
}
