#ifndef CHILDITEMWIDGET_H
#define CHILDITEMWIDGET_H

#include <QWidget>
#include "ui_childItemWidget.h"
#include "httpStructData.h"

class childItemWidget : public QWidget
{
	Q_OBJECT

public:
	childItemWidget(QWidget *parent = 0);
	~childItemWidget();

	void setData(const GoodsInfo_t &goodsInfo);
	const GoodsInfo_t& getData();
private:
	Ui::childItemWidget ui;
	GoodsInfo_t m_goodsInfo;
};

#endif // CHILDITEMWIDGET_H
