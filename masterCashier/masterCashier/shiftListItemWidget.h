#ifndef SHIFTLISTITEMWIDGET_H
#define SHIFTLISTITEMWIDGET_H

#include <QWidget>
#include "ui_shiftListItemWidget.h"
#include "httpStructData.h"

class shiftListItemWidget : public QWidget
{
	Q_OBJECT

public:
	shiftListItemWidget(QWidget *parent = 0);
	~shiftListItemWidget();

	void setData(GoodsInfo_t goodsInfo);
signals:
	void sigOnEditFinished(GoodsInfo_t goodsInfo);
private slots :
	void slotOnEditFinished();
private:
	Ui::shiftListItemWidget ui;
	GoodsInfo_t m_goodsInfo;
};

#endif // SHIFTLISTITEMWIDGET_H
