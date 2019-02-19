#include "pageOrderCommitWgt.h"
#include "orderCommitGoodsItemWgt.h"

pageOrderCommitWgt::pageOrderCommitWgt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	for (int i = 0; i < 2; ++i)
	{
		QListWidgetItem *item = new QListWidgetItem;
		item->setSizeHint(QSize(ui.listWidget->width(), 102));
		orderCommitGoodsItemWgt *itemWidget = new orderCommitGoodsItemWgt;
		ui.listWidget->addItem(item);
		ui.listWidget->setItemWidget(item, itemWidget);
	}
}

pageOrderCommitWgt::~pageOrderCommitWgt()
{

}
