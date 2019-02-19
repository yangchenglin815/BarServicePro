#ifndef RECHARGEDIALOG_H
#define RECHARGEDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include "ui_rechargeDialog.h"
#include "httpStructData.h"
#include "paymentDialog.h"

class QListWidget;
class QListWidgetItem;
class rechargeDialog : public QDialog
{
	Q_OBJECT

public:
	rechargeDialog(QWidget *parent = 0);
	~rechargeDialog();

	void InitDiscountPkg(LoginUserInfo_t userInfo);
private slots:
	void slotOnItemClicked(QListWidgetItem *item);
	void slotOnShowSelectDiscount();
	void slotOnGetDiscountPkg(int nCode, QString sMsg, QList<DiscountPkg_t> lDiscountPkgList);
	void slotOnNetFeeRecharge(int nCode, QString sMsg, QString sFeeCode);
	void slotOnPayNetFee();
	void slotOnDiscountPkgChanged(int id, bool checked);
private:
	void InitUI();
	void InitConnect();
	void InitData();
private:
	Ui::rechargeDialog ui;
	QListWidget *m_listWidget;
	QList<DiscountPkg_t> m_discountPkgList;
	LoginUserInfo_t m_LoginUserInfo;
	DiscountPkg_t m_curr_discountPkg;
	QButtonGroup *m_buttonGroup;
	paymentDialog *m_paymentDlg;
};

#endif // RECHARGEDIALOG_H
