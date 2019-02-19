#ifndef PAGEPOINTLISTWGT_H
#define PAGEPOINTLISTWGT_H

#include <QWidget>
#include <QButtonGroup>
#include "ui_pagePointListWgt.h"
#include "httpStructData.h"

class pagePointListWgt : public QWidget
{
	Q_OBJECT

public:
	pagePointListWgt(QWidget *parent = 0);
	~pagePointListWgt();
signals:
	void sigOnNextFunc();
private slots:
	void slotOnGetAllGoodsInfo(int nCode, QString sMsg, QList<GoodsInfo_t> goodsInfoList);
	void slotOnGetClassifyGroups(int nCode, QString sMsg, QList<ClassifyGroupInfo_t> classifyGroupList, int nPage);
	void slotOnGoodsCheckUpResult(int nCode, QString sMsg);
	void slotOnCurrentPageChanged(int page);
	void slotOnNextFunc();
	void slotOnUpdateCheckNum(GoodsInfo_t goodsInfo);
	void slotOnSearchGoods(QString sIndex);
protected:
	void keyPressEvent(QKeyEvent *event);
private:
	void initConnect();
	void initData(QList<GoodsInfo_t> goodsInfoList, int pageNum = 1);
	QString getClassifyNameById(QString id);
private:
	Ui::pagePointListWgt ui;
	QList<GoodsInfo_t> m_goodsInfoList;
	QList<ClassifyGroupInfo_t> m_classifyGroupList;
	int m_totalPage;
	int m_currPage;
	int m_itemNum;
};

#endif // PAGEPOINTLISTWGT_H
