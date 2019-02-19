#ifndef REMINDCONTENTITEMWGT_H
#define REMINDCONTENTITEMWGT_H

#include <QWidget>
#include "httpStructData.h"
#include "ui_RemindContentItemWgt.h"

class RemindContentItemWgt : public QWidget
{
	Q_OBJECT

public:
	RemindContentItemWgt(QWidget *parent = 0);
	~RemindContentItemWgt();

	void setData(WarningGoods_t warningGoodsInfo);
	void setData(OrderDetialInfo_t orderDetialInfo);
	OrderDetialInfo_t getData();
private:
	Ui::RemindContentItemWgt ui;
private:
	WarningGoods_t m_warningGoodsInfo;
	OrderDetialInfo_t m_orderDetialInfo;
};

#endif // REMINDCONTENTITEMWGT_H
