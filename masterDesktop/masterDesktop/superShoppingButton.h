#ifndef SUPERSHOPPINGBUTTON_H
#define SUPERSHOPPINGBUTTON_H

#include <QPushButton>

class superShoppingButton : public QPushButton
{
	Q_OBJECT

public:
	explicit superShoppingButton(QWidget *parent);
	~superShoppingButton();

	void setCheckedButton(bool isChecked);
	void setData(int totalNum, double totalPrice);
protected:
	void paintEvent(QPaintEvent *event);
private:
	bool m_isChecked;
	int m_totalNum;
	double m_totalPrice;
};

#endif // SUPERSHOPPINGBUTTON_H
