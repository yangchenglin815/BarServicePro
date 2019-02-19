#ifndef RECEIPTPRINTERMAKER_H
#define RECEIPTPRINTERMAKER_H

#include <QObject>
#include <QPainter>
#include "receiptData.h"

class ReceiptPrinterMaker : public QObject
{
	Q_OBJECT

public:
	static ReceiptPrinterMaker* instance();

	void printOrderDetialFunc(const ReceiptInfo &info);
	void saveImageDetialFunc();
private:
	explicit ReceiptPrinterMaker();
	~ReceiptPrinterMaker();
private:
	static ReceiptPrinterMaker* m_pInstance;
	ReceiptInfo m_receiptInfo;
	QPainter* m_pPainter;
	QPixmap* m_pPixmap;
	int m_nPictureMaxHeight; // 图片最大高度
	int m_nZoomWidth; // 打印机的缩放宽度， 打印机的dll会将450的图片缩放到174大小
};

#endif // RECEIPTPRINTERMAKER_H
