#ifndef PAGEQYSUPMARKETWGT_H
#define PAGEQYSUPMARKETWGT_H

#include <QWidget>
#include "ui_pageQySupmarketWgt.h"
#include "HttpAgent.h"
#include "addGoodsDlg.h"
#include "receiptData.h"

class paymentDlg;
class pageQySupmarketWgt : public QWidget
{
	Q_OBJECT

public:
	pageQySupmarketWgt(QWidget *parent = 0);
	~pageQySupmarketWgt();

	void initData();
private slots:
    void slotOnTreeItemClicked(QTreeWidgetItem *item, int column);
	void slotOnGetClassifyGroups(int nCode, QString sMsg, QList<ClassifyGroupInfo_t> classifyGroupList, int nPage);
	void slotOnGetGoodsInfo(int nCode, QString sMsg, QList<GoodsInfo_t> goodsInfoList);
	void slotOnSearchGoods(QString sText);
	void slotOnAddGoodsListItem(QTreeWidgetItem *dItem, int column);
	void slotOnDeleteItem(QString nId);
	void slotOnCloseAddDlg();
	void slotOnFinishAddGoods(GoodsInfo_t goodsInfo);
	void slotOnAddItem(double sum);
	void slotOnReduceItem(double sum);
	void slotOnCancel();
	void slotOnGetOrderResult(int nCode, QString sMsg, QString nOrderId, int nPayStatus);
	void slotOnFinishOrder();
	void slotOnScanFinished(QString code);
	void slotOnStartPinter(ReceiptInfo info);
	void slotOnShowInventoryRemindPage();
	void slotOnShowSalePage();
	void slotOnShowReturnPage();
	void slotOnShowOrderPage();
	void slotOnGoodsListChanged();
	void slotOnCloseOrder();
	void slotOnReturnWaitingPay();
	void slotOnPaySuccess();
	void slotOnShowTipLabel(bool isHiden);
protected:
	void keyPressEvent(QKeyEvent *event);
private:
	void init();
	void initConnect();
	void initTreeRoot();
	void initTreeChild();
	void initListData(const GoodsInfo_t &goodsInfo);

	void payInCashFunc();
	void payInMobileFunc();
private:
	Ui::pageQySupmarketWgt ui;
	QList<ClassifyGroupInfo_t> m_classifyGroupList;
	QList<GoodsInfo_t> m_goodsInfoList;
	addGoodsDlg *m_addGoodsDlg;
	paymentDlg *m_paymentDlg;
	QString m_closeOrderId;
	QLabel *m_tipLabel;
};

#endif // PAGEQYSUPMARKETWGT_H
