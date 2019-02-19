#ifndef PAGEORDERPAYWGT_H
#define PAGEORDERPAYWGT_H

#include <QWidget>
#include "ui_pageOrderPayWgt.h"

class pageOrderPayWgt : public QWidget
{
	Q_OBJECT

public:
	pageOrderPayWgt(QWidget *parent = 0);
	~pageOrderPayWgt();

private:
	Ui::pageOrderPayWgt ui;
};

#endif // PAGEORDERPAYWGT_H
