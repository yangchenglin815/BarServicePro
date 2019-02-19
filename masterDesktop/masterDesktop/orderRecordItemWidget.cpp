#include "orderRecordItemWidget.h"
#include "orderRecordGoodsItem.h"

orderRecordItemWidget::orderRecordItemWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	orderRecordGoodsItem *item = new orderRecordGoodsItem(ui.wgt_top);
}

orderRecordItemWidget::~orderRecordItemWidget()
{

}
