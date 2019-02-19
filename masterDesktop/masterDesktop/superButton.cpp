#include "superButton.h"
#include <QPainter>

superButton::superButton(QWidget *parent, DiscountPkg_t discountPkg)
	: QPushButton(parent)
	, m_discountPkg(discountPkg)
{
	setCheckable(true);
}

superButton::~superButton()
{

}

void superButton::paintEvent(QPaintEvent *event)
{
	int size = 2;                //ÒõÓ°¿í¶È
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	painter.setPen(QColor("#0D3632"));
	painter.setBrush(QColor("#0D3632"));
	painter.drawRect(0, 0, this->width(), this->height());

	//QColor color("#144D46");
	for (int i = 0; i <= size; i++)
	{
		//color.setAlpha(i * 4);
		painter.setPen(QColor("#144D46"));
		painter.setBrush(Qt::transparent);
		painter.drawRoundedRect(i, i, this->width() - i * 2, this->height() - i * 2, 0, 0);
	}

	painter.setPen(QColor("#FFFFFF"));
	QFont font;
	font.setFamily(QStringLiteral("Î¢ÈíÑÅºÚ"));
	font.setPixelSize(18);
	painter.setFont(font);
	painter.drawText(QRect(0, 0, this->width(), this->height()), Qt::AlignCenter, QString::number(m_discountPkg.dOriginalPrice));
}
