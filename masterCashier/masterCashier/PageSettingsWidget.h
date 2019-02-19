#ifndef PAGESETTINGSWIDGET_H
#define PAGESETTINGSWIDGET_H

#include <QWidget>
#include "ui_PageSettingsWidget.h"

class PageSettingsWidget : public QWidget
{
	Q_OBJECT

public:
	PageSettingsWidget(QWidget *parent = 0);
	~PageSettingsWidget();

private:
	Ui::PageSettingsWidget ui;
};

#endif // PAGESETTINGSWIDGET_H
