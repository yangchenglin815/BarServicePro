#ifndef INVENTORYREMINDITEMWGT_H
#define INVENTORYREMINDITEMWGT_H

#include <QWidget>
#include "ui_InventoryRemindItemWgt.h"
#include "httpStructData.h"

class InventoryRemindItemWgt : public QWidget
{
	Q_OBJECT

public:
	InventoryRemindItemWgt(bool flag, QWidget *parent = 0);
	~InventoryRemindItemWgt();

	void setData(QString nContent, QString nTime);
	void setData(OrderInfo_t orderInfo);
	OrderInfo_t getData();
private:
	Ui::InventoryRemindItemWgt ui;
	OrderInfo_t m_orderInfo;
};

#endif // INVENTORYREMINDITEMWGT_H
