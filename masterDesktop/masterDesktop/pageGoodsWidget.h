#ifndef PAGEGOODSWIDGET_H
#define PAGEGOODSWIDGET_H

#include <QWidget>
#include "ui_pageGoodsWidget.h"
#include "httpStructData.h"

class orderGoodsItemWgt;
class pageGoodsWidget : public QWidget
{
	Q_OBJECT

public:
	explicit pageGoodsWidget(QWidget *parent = 0);
	~pageGoodsWidget();

	void InitData();
signals:
	void sigOnAddGoodsItem(GoodsInfo_t goodsInfo);
	void sigOnReduceGoodsItem(GoodsInfo_t goodsInfo);
private slots:
	void slotOnGetClassifyGroups(int nCode, QString sMsg, QList<ClassifyGroupInfo_t> classifyGroupList, int nPage);
	void slotOnGetGoodsInfo(int nCode, QString sMsg, QList<GoodsInfo_t> goodsInfoList);
	void slotOnCurrPageChanged(int nPage);
	void slotOnClassifyItemClicked(QListWidgetItem *item);
	void slotOnGoodsNumChanged(GoodsInfo_t goodsInfo);
	void slotOnClearShoppingCart();
private:
	Ui::pageGoodsWidget ui;
	QList<ClassifyGroupInfo_t> m_classifyGroupList;
	QList<GoodsInfo_t> m_goodsInfoList;
	QList<orderGoodsItemWgt*> m_goodItemGroup;
	QString m_classifyId;
	int m_curr_page;
	int m_total_page;
private:
	void InitConnect();
	void InitClassify();
	void InitGoodsList();
};

#endif // PAGEGOODSWIDGET_H
