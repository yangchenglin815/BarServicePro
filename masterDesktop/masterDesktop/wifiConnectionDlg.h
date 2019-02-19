#ifndef WIFICONNECTIONDLG_H
#define WIFICONNECTIONDLG_H

#include <QDialog>
#include "ui_wifiConnectionDlg.h"

class wifiConnectionDlg : public QDialog
{
	Q_OBJECT

public:
	wifiConnectionDlg(QWidget *parent = 0);
	~wifiConnectionDlg();

	void InitData();
private slots:
    void slotOnGetWifiInfo(int nCode, QString sMsg, QString sWifiName, QString sWifiKey);
private:
	void InitConnect();
	void InitUi();
private:
	Ui::wifiConnectionDlg ui;
	QString m_wifiName;
	QString m_wifiKey;
};

#endif // WIFICONNECTIONDLG_H
