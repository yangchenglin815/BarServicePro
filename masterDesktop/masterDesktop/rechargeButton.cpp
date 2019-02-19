#include "rechargeButton.h"
#include <QPainter>

rechargeButton::rechargeButton(QWidget *parent, DiscountPkg_t discountPkgs)
	: QPushButton(parent)
	, m_discountPkg(discountPkgs)
	, m_discountFlag(false)
{
	setFixedSize(QSize(120, 60));
	setCheckable(true);

}

rechargeButton::~rechargeButton()
{

}

void rechargeButton::paintEvent(QPaintEvent *event)
{
	int size = 2;                //ÒõÓ°¿í¶È
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	painter.setOpacity(0.94);
	painter.setPen(QColor("#144D46"));
	if (this->isChecked())
	{
		painter.setBrush(QColor("#008782"));
		painter.drawRect(0, 0, this->width(), this->height());

		for (int i = 0; i <= size; i++)
		{
			painter.setPen(QColor("#00DDC2"));
			painter.setBrush(Qt::transparent);
			painter.drawRoundedRect(i, i, this->width() - i * 2, this->height() - i * 2, 0, 0);
		}
	}
	else
	{
		painter.setBrush(QColor("#0D3632"));
		painter.drawRect(0, 0, this->width(), this->height());

		for (int i = 0; i <= size; i++)
		{
			painter.setPen(QColor("#144D46"));
			painter.setBrush(Qt::transparent);
			painter.drawRoundedRect(i, i, this->width() - i * 2, this->height() - i * 2, 0, 0);
		}
	}

	painter.setPen(QColor("#144D46"));
	painter.drawRect(0, 0, this->width(), this->height());

	painter.setPen(QColor("#FFFFFF"));
	QFont font;
	font.setFamily(QStringLiteral("Î¢ÈíÑÅºÚ"));
	font.setPixelSize(18);
	painter.setFont(font);
	painter.drawText(QRect(0, 0, this->width(), this->height()), Qt::AlignCenter, QString::number(m_discountPkg.dOriginalPrice));

	if (m_discountFlag)
	{
		painter.drawPixmap(QRect(this->width() - 46, 0, 46, 20), QPixmap(":/masterDesktop/Resources/coupon.png"));
	}
}
