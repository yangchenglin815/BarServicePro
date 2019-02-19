#ifndef QRCODEGENERATEWGT_H
#define QRCODEGENERATEWGT_H

#include <QWidget>
#include <QPainter>
#include "qrencode.h"

class qrCodeGenerateWgt : public QWidget
{
	Q_OBJECT

public:
	explicit qrCodeGenerateWgt(QWidget *parent = 0);
	~qrCodeGenerateWgt();
	void generateString(QString str);
private:
	void draw(QPainter &painter, int width, int height);
private:
	QString string;
	QRcode *qr;
protected:
	void paintEvent(QPaintEvent *);
	QSize sizeHint() const;
	QSize minimumSizeHint() const;
};

#endif // QRCODEGENERATEWGT_H
