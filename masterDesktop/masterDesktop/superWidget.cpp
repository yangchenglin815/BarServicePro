#include "superWidget.h"
#include <QPainter>

superWidget::superWidget(QWidget *parent)
	: QWidget(parent)
{

}

superWidget::~superWidget()
{

}

void superWidget::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)
	int size = 2;                //ÒõÓ°¿í¶È
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	painter.setPen(QColor("#008272"));
    painter.setBrush(QColor("#0C2F2B"));
	painter.drawRect(0, 0, this->width(), this->height());

	for (int i = 0; i <= size; i++)
	{
		painter.setPen(QColor("rgba(0,0,0,1)"));
		painter.setBrush(Qt::transparent);
		painter.drawRoundedRect(i, i, this->width() - i * 2, this->height() - i * 2, 0, 0);
	}

	painter.setPen(QColor("#008272"));
	painter.drawRect(0, 0, this->width(), this->height());
}
