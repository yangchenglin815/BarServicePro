#include "orderConfirmItemWgt.h"
#include "orderConfirmGoodsItemWgt.h"

orderConfirmItemWgt::orderConfirmItemWgt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	for (int i = 0; i < 3; ++i)
	{
		QListWidgetItem *item = new QListWidgetItem;
		item->setSizeHint(QSize(ui.listWidget->width(), 102));
		orderConfirmGoodsItemWgt *itemWidget = new orderConfirmGoodsItemWgt;
		ui.listWidget->addItem(item);
		ui.listWidget->setItemWidget(item, itemWidget);
	}
}

orderConfirmItemWgt::~orderConfirmItemWgt()
{

}
