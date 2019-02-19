#ifndef PAGESETTINGS_H
#define PAGESETTINGS_H

#include <QWidget>
#include "ui_PageSettings.h"

class PageSettings : public QWidget
{
	Q_OBJECT

public:
	PageSettings(QWidget *parent = 0);
	~PageSettings();

private:
	Ui::PageSettings ui;
};

#endif // PAGESETTINGS_H
