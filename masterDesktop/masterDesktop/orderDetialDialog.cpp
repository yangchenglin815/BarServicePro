#include "orderDetialDialog.h"
#include "orderRecordItemWidget.h"

orderDetialDialog::orderDetialDialog(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	orderRecordItemWidget *wgt = new orderRecordItemWidget(ui.scrollAreaWidgetContents);
}

orderDetialDialog::~orderDetialDialog()
{

}
