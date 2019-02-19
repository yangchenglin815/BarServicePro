#ifndef SHOPPINGCARTDLG_H
#define SHOPPINGCARTDLG_H

#include <QDialog>
#include "ui_shoppingCartDlg.h"
#include "httpStructData.h"

class shoppingCartDlg : public QDialog
{
	Q_OBJECT

public:
	shoppingCartDlg(QWidget *parent = 0);
	~shoppingCartDlg();

	void addToShoppingCart(GoodsInfo_t goodsInfo);
	void reduceToShoppongCart(GoodsInfo_t goodsInfo);
signals:
	void sigOnShoppingCartChanged(int totalNum, double totalPrice);
private slots:
    void slotOnClearShoppingCart();
	void slotOnSyncAddGoodsInfo(GoodsInfo_t goodsInfo);
	void slotOnSyncReduceGoodsInfo(GoodsInfo_t goodsInfo);
private:
	void InitShoppingCart();
	void InitConnect();
private:
	Ui::shoppingCartDlg ui;
	QList<GoodsInfo_t> m_goodsInfoList;
	int m_goodsTotal;
	double m_goodsTotalPrice;
};

#endif // SHOPPINGCARTDLG_H
