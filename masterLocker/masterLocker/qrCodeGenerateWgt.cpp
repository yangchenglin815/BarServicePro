#include "qrCodeGenerateWgt.h"

qrCodeGenerateWgt::qrCodeGenerateWgt(QWidget *parent)
	: QWidget(parent)
{
	qr = NULL;
}

qrCodeGenerateWgt::~qrCodeGenerateWgt()
{
	if (qr != NULL)
	{
		QRcode_free(qr);
	}
}

void qrCodeGenerateWgt::generateString(QString str)
{
	string = str;
	if (qr != NULL)
	{
		QRcode_free(qr);
	}
	qr = QRcode_encodeString(string.toStdString().c_str(),
		1,
		QR_ECLEVEL_L,
		QR_MODE_8,
		1);
	update();
}

void qrCodeGenerateWgt::draw(QPainter &painter, int width, int height)
{
	QColor foreground(Qt::black);
	painter.setBrush(foreground);
	painter.scale(qreal(220) / this->width(), qreal(220) / this->height()); //·Å´ó±¶Êý
	const int qr_width = qr->width > 0 ? qr->width : 1;
	double scale_x = width / qr_width;
	double scale_y = height / qr_width;
	for (int y = 0; y < qr_width; y++)
	{
		for (int x = 0; x < qr_width; x++)
		{
			unsigned char b = qr->data[y * qr_width + x];
			if (b & 0x01)
			{
				QRectF r(x * scale_x, y * scale_y, scale_x, scale_y);
				painter.drawRects(&r, 1);
			}
		}
	}
}

void qrCodeGenerateWgt::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	QColor background(Qt::white);
	painter.setBrush(background);
	painter.setPen(Qt::NoPen);
	painter.drawRect(0, 0, width(), height());
	if (qr != NULL)
	{
		draw(painter, width(), height());
	}
}

QSize qrCodeGenerateWgt::sizeHint() const
{
	QSize s;
	if (qr != NULL)
	{
		int qr_width = qr->width > 0 ? qr->width : 1;
		s = QSize(qr_width * 4, qr_width * 4);
	}
	else
	{
		s = QSize(50, 50);
	}
	return s;
}

QSize qrCodeGenerateWgt::minimumSizeHint() const
{
	QSize s;
	if (qr != NULL)
	{
		int qr_width = qr->width > 0 ? qr->width : 1;
		s = QSize(qr_width, qr_width);
	}
	else
	{
		s = QSize(50, 50);
	}
	return s;
}
