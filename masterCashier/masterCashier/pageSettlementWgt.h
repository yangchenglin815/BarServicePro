#ifndef PAGESETTLEMENTWGT_H
#define PAGESETTLEMENTWGT_H

#include <QWidget>
#include "ui_pageSettlementWgt.h"
#include "httpStructData.h"

class pageSettlementWgt : public QWidget
{
	Q_OBJECT

public:
	pageSettlementWgt(QWidget *parent = 0);
	~pageSettlementWgt();
signals:
	void sigOnFrontFunc();
private slots:
	void slotOnGetShiftInfo(int nCode, QString sMsg, ShiftInfo_t tShiftInfo);
	void slotOnCommitFunc();
	void slotOnIndexChanged(int index);
	void slotOnCheckAcceptorResult(int nCode, QString sMsg);
	void slotOnCommitShiftInfoResult(int nCode, QString sMsg);
	void slotOnShiftLogout(int nCode, QString sMsg);
	void slotOnShiftLogin(int nCode, QString sMsg, QString sUser, QString nUserId);
private:
	void initConnect();
	void initData();
	QString getUserIdByName(QString name);
private:
	Ui::pageSettlementWgt ui;
	ShiftInfo_t m_shiftInfo;
	ShifterInfo_t m_shifterInfo;
	QString m_acceptorId;
	QString m_acceptorPwd;
	QString m_mark;
};

#endif // PAGESETTLEMENTWGT_H
