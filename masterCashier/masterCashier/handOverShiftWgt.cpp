#include "handOverShiftWgt.h"

handOverShiftWgt::handOverShiftWgt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.page_pointList, SIGNAL(sigOnNextFunc()), this, SLOT(slotOnNextFunc()));
	connect(ui.page_settlement, SIGNAL(sigOnFrontFunc()), this, SLOT(slotOnFrontFunc()));
}

handOverShiftWgt::~handOverShiftWgt()
{

}

void handOverShiftWgt::slotOnNextFunc()
{
	ui.stackedWidget->setCurrentWidget(ui.page_settlement);
}

void handOverShiftWgt::slotOnFrontFunc()
{
	ui.stackedWidget->setCurrentWidget(ui.page_pointList);
}
