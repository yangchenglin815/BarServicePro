#ifndef INVENTORYREMINDWGT_H
#define INVENTORYREMINDWGT_H

#include <QWidget>
#include "httpStructData.h"
#include "ui_InventoryRemindWgt.h"

class InventoryRemindWgt : public QWidget
{
	Q_OBJECT

public:
	InventoryRemindWgt(QWidget *parent = 0);
	~InventoryRemindWgt();
private:
	void InitWarningOrderList();
	void InitWarningGoodsList();
	void InitConnect();
private slots:
	void slotOnGetWarningGoodsList(int nCode, QString sMsg, QList<WarningGoods_t> lWarningGoodsList);
private:
	Ui::InventoryRemindWgt ui;
	QList<WarningGoods_t> m_warningGoodsList;
};

#endif // INVENTORYREMINDWGT_H
