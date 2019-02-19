#include "discountItemWidget.h"

discountItemWidget::discountItemWidget(QWidget *parent)
	: QWidget(parent)
	, m_isChecked(false)
{
	ui.setupUi(this);
}

discountItemWidget::~discountItemWidget()
{

}

void discountItemWidget::setData(QString text)
{
	ui.lbl_content->setText(text);
}

void discountItemWidget::setChecked(bool isChecked)
{
	if (isChecked)
	{
		ui.lbl_check->setText(QStringLiteral("¡Ì"));
	}
	else
	{
		ui.lbl_check->setText("");
	}
	m_isChecked = isChecked;
}

bool discountItemWidget::isChecked()
{
	return m_isChecked;
}
