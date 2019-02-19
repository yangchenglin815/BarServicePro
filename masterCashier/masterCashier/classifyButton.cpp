#include "classifyButton.h"

classifyButton::classifyButton(QWidget *parent)
	: QPushButton(parent)
{
	ui.setupUi(this);
}

classifyButton::~classifyButton()
{

}

void classifyButton::initButton(const QString &text, const QString &style)
{
	ui.lbl_classifyName->setText(text);
	ui.lbl_icon->setStyleSheet(style);
}
