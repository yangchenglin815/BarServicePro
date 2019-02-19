#include "superShoppingButton.h"
#include <QPainter>

superShoppingButton::superShoppingButton(QWidget *parent)
	: QPushButton(parent)
	, m_totalNum(0)
	, m_totalPrice(0.00)
{
	m_isChecked = false;
}

superShoppingButton::~superShoppingButton()
{

}

void superShoppingButton::setCheckedButton(bool isChecked)
{
	m_isChecked = isChecked;
	update();
}

void superShoppingButton::setData(int totalNum, double totalPrice)
{
	m_totalNum = totalNum;
	m_totalPrice = totalPrice;

	update();
}

void superShoppingButton::paintEvent(QPaintEvent *event)
{
	int size = 2;                //阴影宽度
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	painter.setPen(QColor("#144D46"));
	if (m_isChecked)
	{
		painter.setBrush(QColor("#006966"));
	}
	else
	{
		painter.setBrush(QColor("#002E2D"));
	}
	painter.drawRect(0, 0, this->width(), this->height());

	for (int i = 0; i <= size; i++)
	{
		painter.setPen(QColor("rgba(8,1,3,0.65)"));
		painter.setBrush(Qt::transparent);
		painter.drawRoundedRect(i, i, this->width() - i * 2, this->height() - i * 2, 0, 0);
	}

	painter.setPen(QColor("#144D46"));
	painter.drawRect(0, 0, this->width(), this->height());

	painter.setPen(QColor("#FFFFFF"));
	QFont font;
	font.setFamily(QStringLiteral("微软雅黑"));
	font.setPixelSize(18);
	painter.setFont(font);
	painter.drawText(QRect(0, 0, 70, height()), Qt::AlignCenter, QStringLiteral("¥") + QString::number(m_totalPrice, 'f', 2));

	if (m_isChecked)
	{
		painter.drawPixmap(QRect(76, 11, 32, 29), QPixmap(":/masterDesktop/Resources/icon_shopping cart_selected.png"));
	}
	else
	{
		painter.drawPixmap(QRect(76, 11, 32, 29), QPixmap(":/masterDesktop/Resources/icon_shopping cart_default.png"));
	}

	if (m_totalNum != 0)
	{
		painter.setPen(QColor("#A2000D"));
		painter.setBrush(QColor("#A2000D"));
		painter.drawEllipse(70, 5, 16, 16);

		painter.setPen(QColor("#EEEEEE"));
		font.setPixelSize(14);
		painter.setFont(font);
		painter.drawText(QRect(70, 5, 16, 16), Qt::AlignCenter, QString::number(m_totalNum));
	}
}
