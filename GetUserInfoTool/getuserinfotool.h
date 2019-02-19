#ifndef GETUSERINFOTOOL_H
#define GETUSERINFOTOOL_H

#include <QtWidgets/QMainWindow>
#include "ui_getuserinfotool.h"

#include <QNetworkInterface>  
#include <QList>  
#include "httpStructData.h"

class GetUserInfoTool : public QMainWindow
{
	Q_OBJECT

public:
	GetUserInfoTool(QWidget *parent = 0);
	~GetUserInfoTool();
private slots:
     void slotOnReportInfo();
	 void slotOnReportPCInfoResult(int nCode, QString sMsg);
private:
	QString getHostIpAddress();
	QString getHostMacAddress();
	QString getLocalmachineName();
private:
	Ui::GetUserInfoToolClass ui;
	ReportPCInfo_t m_info;
};

#endif // GETUSERINFOTOOL_H
