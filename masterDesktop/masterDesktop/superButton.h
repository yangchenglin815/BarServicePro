#ifndef SUPERBUTTON_H
#define SUPERBUTTON_H

#include <QPushButton>
#include "httpStructData.h"

class superButton : public QPushButton
{
	Q_OBJECT

public:
	explicit superButton(QWidget *parent, DiscountPkg_t discountPkg);
	~superButton();

	DiscountPkg_t getData(){ return m_discountPkg; };
protected:
	void paintEvent(QPaintEvent *event);
private:
	QString m_text;
	DiscountPkg_t m_discountPkg;
};

#endif // SUPERBUTTON_H
