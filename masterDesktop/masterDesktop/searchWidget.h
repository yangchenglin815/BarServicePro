#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include "ui_searchWidget.h"

class searchWidget : public QWidget
{
	Q_OBJECT

public:
	searchWidget(QWidget *parent = 0);
	~searchWidget();
protected:
	void paintEvent(QPaintEvent *event);
private:
	Ui::searchWidget ui;
};

#endif // SEARCHWIDGET_H
