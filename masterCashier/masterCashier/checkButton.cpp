#include "checkButton.h"

checkButton::checkButton(QWidget *parent)
	: QPushButton(parent)
{
	this->setFixedSize(QSize(90, 40));
	this->setStyleSheet(QString("QPushButton{font-family:%1;font-size:14px;color:rgba(0,0,0,0.9);border:1px solid #DEEEF2;background:#FFFFFF;}QPushButton:checked{background:#EAFBFF;}").arg(QStringLiteral("Î¢ÈíÑÅºÚ")));
	this->setCheckable(true);
}

checkButton::~checkButton()
{

}

void checkButton::setId(QString id)
{
	m_classId = id;
}

QString checkButton::getId()
{
	return m_classId;
}
