#ifndef LISTITEMWIDGET_H
#define LISTITEMWIDGET_H

#include <QWidget>
#include "ui_listItemWidget.h"
#include "httpStructData.h"

class listItemWidget : public QWidget
{
	Q_OBJECT

public:
	listItemWidget(QWidget *parent = 0);
	~listItemWidget();

	void setData(const GoodsInfo_t &goodsInfo);
	const GoodsInfo_t& getData();
signals:
	void sigOnDeleteItem(QString nId);
	void sigOnAddNum(double sum);
	void sigOnReduceNum(double sum);
public slots:
    void slotOnAddNum();
	void slotOnReduceNum();
	void slotOnDeleteItem();
private:
	void initConnect();
private:
	Ui::listItemWidget ui;
	GoodsInfo_t m_goodsInfo;
};

#endif // LISTITEMWIDGET_H
