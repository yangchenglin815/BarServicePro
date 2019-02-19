#ifndef PERSONALCENTERDLG_H
#define PERSONALCENTERDLG_H

#include <QDialog>
#include "ui_personalCenterDlg.h"
#include "httpStructData.h"

class personalCenterDlg : public QDialog
{
	Q_OBJECT

public:
	personalCenterDlg(QWidget *parent = 0);
	~personalCenterDlg();

	void InitData(LoginUserInfo_t userInfo);
private slots:
    void slotOnGetDiscountPkg(int nCode, QString sMsg, QList<DiscountPkg_t> lDiscountPkgList);
private:
	void InitConnect();
	void InitUi();
private:
	Ui::personalCenterDlg ui;
	LoginUserInfo_t m_loginUserInfo;
	QList<DiscountPkg_t> m_discountPkgList;
};

#endif // PERSONALCENTERDLG_H
