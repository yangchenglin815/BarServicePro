#ifndef PAGERETURNGOODSWGT_H
#define PAGERETURNGOODSWGT_H

#include <QWidget>
#include "ui_PageReturnGoodsWgt.h"
#include "httpStructData.h"
#include "addGoodsDlg.h"

class PageReturnGoodsWgt : public QWidget
{
	Q_OBJECT

public:
	PageReturnGoodsWgt(QWidget *parent = 0);
	~PageReturnGoodsWgt();

	void InitData();
private:
	void InitConnect();
	void InitTreeRoot();
	void InitTreeChild();
	void InitListData(const GoodsInfo_t &goodsInfo);
protected:
	void keyPressEvent(QKeyEvent *event);
private slots:
	void slotOnGetClassifyGroups(int nCode, QString sMsg, QList<ClassifyGroupInfo_t> classifyGroupList, int nPage);
	void slotOnGetReturnGoodsInfo(int nCode, QString sMsg, QList<GoodsInfo_t> goodsInfoList);
	void slotOnTreeItemClicked(QTreeWidgetItem *item, int column);
	void slotOnAddGoodsListItem(QTreeWidgetItem *dItem, int column);
	void slotOnSearchGoods(QString sText);
	void slotOnDeleteItem(QString nId);
	void slotOnCloseAddDlg();
	void slotOnFinishAddGoods(GoodsInfo_t goodsInfo);
	void slotOnAddItem(double sum);
	void slotOnReduceItem(double sum);
	void slotOnCancel();
	void slotOnFinishOrder();
private:
	Ui::PageReturnGoodsWgt ui;
	QList<ClassifyGroupInfo_t> m_classifyGroupList;
	QList<GoodsInfo_t> m_goodsInfoList;
	addGoodsDlg *m_addGoodsDlg;
};

#endif // PAGERETURNGOODSWGT_H
