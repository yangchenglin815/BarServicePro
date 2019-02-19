#include "PageSettingsWidget.h"

PageSettingsWidget::PageSettingsWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.btn_pageSettings->setChecked(true);
	ui.stackedWidget->setCurrentWidget(ui.btn_pageSettings);
}

PageSettingsWidget::~PageSettingsWidget()
{

}
