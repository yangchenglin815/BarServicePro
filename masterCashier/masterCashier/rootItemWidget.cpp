#include "rootItemWidget.h"

rootItemWidget::rootItemWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.lbl_arrow->setStyleSheet("background-image:url(:/masterCashier/Resources/arrow_left.png);");
}

rootItemWidget::~rootItemWidget()
{

}

void rootItemWidget::setButtonChecked(bool checked)
{
	if (checked)
	{
		ui.lbl_arrow->setStyleSheet("background-image:url(:/masterCashier/Resources/arrow_down.png);");
	}
	else
	{
		ui.lbl_arrow->setStyleSheet("background-image:url(:/masterCashier/Resources/arrow_left.png);");
	}
}

void rootItemWidget::setData(const ClassifyGroupInfo_t &classifyGroupInfo)
{
	m_classifyGroupInfo = classifyGroupInfo;
	ui.lbl_root->setText(QStringLiteral("%1").arg(m_classifyGroupInfo.sName));
}

QString rootItemWidget::getClassifyId()
{
	return m_classifyGroupInfo.id;
}
