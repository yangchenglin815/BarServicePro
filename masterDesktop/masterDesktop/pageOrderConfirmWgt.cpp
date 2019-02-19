#include "pageOrderConfirmWgt.h"
#include "orderConfirmItemWgt.h"

pageOrderConfirmWgt::pageOrderConfirmWgt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QListWidgetItem *item = new QListWidgetItem;
	item->setSizeHint(QSize(ui.listWidget->width(), 39 + 102*3));
	orderConfirmItemWgt *itemWidget = new orderConfirmItemWgt;
	ui.listWidget->addItem(item);
	ui.listWidget->setItemWidget(item, itemWidget);
}

pageOrderConfirmWgt::~pageOrderConfirmWgt()
{

}
