#ifndef PAGEORDERWIDGET_H
#define PAGEORDERWIDGET_H

#include <QWidget>
#include "ui_PageOrderWidget.h"
#include "httpStructData.h"

class myProgressDlg;
class PageOrderWidget : public QWidget
{
	Q_OBJECT

public:
	explicit PageOrderWidget(QWidget *parent = 0);
	~PageOrderWidget();

	void InitOrderList();
	void InitDetialList(const QList<OrderDetialInfo_t> &orderDetialInfoList);
	void InitData();
private:
	void InitUI();
	void InitConnect();
private slots:
	void slotOnGetOrderList(int nCode, QString sMsg, QList<OrderInfo_t> lOrderInfoList);
	void slotOnGetOrderDetialList(int nCode, QString sMsg, QList<OrderDetialInfo_t> lOrderDetialInfoList);
	void slotOnItemClicked(QListWidgetItem *cItem);
	void slotOnShowAllOrder();
	void slotOnShowPayFinishedOrder();
	void slotOnShowPayUnFinishedOrder();
	void slotOnReturn();
	void slotOnGetRefundRespond(int nCode, QString sMsg);
	void slotOnCancalOrder();
private:
	Ui::PageOrderWidget ui;
	QList<OrderInfo_t> m_orderInfoList;
	OrderInfo_t m_curr_orderInfo;
	myProgressDlg *const m_progressDlg;
};

#endif // PAGEORDERWIDGET_H
