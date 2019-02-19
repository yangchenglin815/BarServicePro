#ifndef RECHARGEBUTTON_H
#define RECHARGEBUTTON_H

#include <QPushButton>
#include "httpStructData.h"

class rechargeButton : public QPushButton
{
	Q_OBJECT

public:
	explicit rechargeButton(QWidget *parent, DiscountPkg_t discountPkg);
	~rechargeButton();

	DiscountPkg_t getData(){ return m_discountPkg; };
protected:
	void paintEvent(QPaintEvent *event);

private:
	bool m_discountFlag;
	DiscountPkg_t m_discountPkg;
};

#endif // RECHARGEBUTTON_H
