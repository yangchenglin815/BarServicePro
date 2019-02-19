#ifndef HANDOVERSHIFTWGT_H
#define HANDOVERSHIFTWGT_H

#include <QWidget>
#include "ui_handOverShiftWgt.h"

class handOverShiftWgt : public QWidget
{
	Q_OBJECT

public:
	explicit handOverShiftWgt(QWidget *parent = 0);
	~handOverShiftWgt();
public slots:
	void slotOnNextFunc();
	void slotOnFrontFunc();
private:
	Ui::handOverShiftWgt ui;
};

#endif // HANDOVERSHIFTWGT_H
