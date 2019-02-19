#ifndef ORDERGOODSDIALOG_H
#define ORDERGOODSDIALOG_H

#include <QDialog>
#include "ui_orderGoodsDialog.h"

enum PageType{
	t_OrderOnline = 1,
	t_OrderConfirm = 2,
	t_OrderCommit = 3,
	t_OrderPay = 4,
	t_GoodsSearch = 5,
};

class shoppingCartDlg;
class orderGoodsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit orderGoodsDialog(QWidget *parent = 0);
	~orderGoodsDialog();

	void InitPage(PageType pageType);
private slots:
	void slotOnShowShoppingDlg();
	void slotOnAddGoodsItem(GoodsInfo_t goodsInfo);
	void slotOnReduceGoodsItem(GoodsInfo_t goodsInfo);
	void slotOnShoppingCartChanged(int totalNum, double totalPrice);
private:
	void InitMask();
	void InitConnect();
private:
	Ui::orderGoodsDialog ui;
	QWidget *m_maskWgt;
	superShoppingButton *m_shoppingCart_btn;
	shoppingCartDlg *m_shoppingCart_dlg;
};

#endif // ORDERGOODSDIALOG_H
