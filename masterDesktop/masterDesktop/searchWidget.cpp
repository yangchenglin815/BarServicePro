#include "searchWidget.h"
#include <QPainter>

searchWidget::searchWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

searchWidget::~searchWidget()
{

}

void searchWidget::paintEvent(QPaintEvent *event)
{
	int size = 2;                //ÒõÓ°¿í¶È
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	painter.setPen(QColor("#144D46"));
	painter.setBrush(QColor("#002E2D"));
	painter.drawRect(0, 0, this->width(), this->height());

	for (int i = 0; i <= size; i++)
	{
		painter.setPen(QColor("rgba(8,1,3,0.65)"));
		painter.setBrush(Qt::transparent);
		painter.drawRoundedRect(i, i, this->width() - i * 2, this->height() - i * 2, 0, 0);
	}

	painter.setPen(QColor("#144D46"));
	painter.drawRect(0, 0, this->width(), this->height());
}
