#include "myDiscountDialog.h"

myDiscountDialog::myDiscountDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_TranslucentBackground, true);
}

myDiscountDialog::~myDiscountDialog()
{

}
