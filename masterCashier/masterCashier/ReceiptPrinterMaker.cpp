#include "ReceiptPrinterMaker.h"
#include <QPainter>
#include <QPrinter>  
#include <QPrintDialog>  
#include <QPrintPreviewDialog> 
#include <QTextDocument>
#include <QTime>
#include <QApplication>
#include "appConfig.h"

using namespace dm::server;

ReceiptPrinterMaker* ReceiptPrinterMaker::m_pInstance = NULL;

const int c_sPFontSize = 20;
const int c_sFontSize = 12;
const int c_sMFontSize = 14;

const int c_sTopSpace = 30;
const int c_sLineLeftSpace = 0;
const int c_sLeftSpace = 2;
const int c_sKeyWidth = 60;
const int c_sPValueWidth = 200;
const int c_sPKeyWidth = 125;

ReceiptPrinterMaker* ReceiptPrinterMaker::instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new ReceiptPrinterMaker;
	}
	return m_pInstance;
}

void ReceiptPrinterMaker::printOrderDetialFunc(const ReceiptInfo &info)
{
	m_receiptInfo = info;

	//保存为图片
	saveImageDetialFunc();

	// 创建打印机对象  
	QPrinter printer;
	// 创建打印对话框  
	QString printerName = printer.printerName();
	//AppConfigHandler.Info("the printer name : " + printerName.toStdString());
	if (printerName.size() == 0)
		return;
	printer.setNumCopies(1);
	//AppConfigHandler.Info("begin to print the order !");
// 		QPrintDialog dlg(&printer);
// 		if (dlg.exec() == QDialog::Accepted)
// 		{

			//打印尺寸  
			QSizeF s = QSizeF(printer.logicalDpiX() * (58 / 25.4), printer.logicalDpiY() * (297 / 25.4));
			printer.setPageSizeMM(s);
			printer.setOutputFormat(QPrinter::NativeFormat);

			int nWidth = s.width();
			int nXPos = 0;
			int nYPos = 0;

			QFont font;
			font.setFamily("微软雅黑");
			font.setPixelSize(c_sPFontSize);

			QPainter paint;
			paint.begin(&printer);

			paint.setFont(font);
			paint.setPen(QPen(QColor("#000000")));
			QString sBarName = m_receiptInfo.sBarName;
			int titleHight = sBarName.size() / 7 + 1;
			for (int i = 0; i < titleHight; ++i)
			{
				QRect rect1(0, c_sTopSpace + c_sFontSize*i + 10 * i, nWidth, c_sPFontSize);
				paint.drawText(rect1, Qt::AlignHCenter | Qt::AlignVCenter, QStringLiteral("%1").arg(sBarName.mid(i * 7, 7)));
			}


			QPen pen(QBrush(QColor("#000000")), 1, Qt::DashLine);
			paint.setPen(pen);
			int xPos1 = nXPos + c_sLineLeftSpace;
			int yPos1 = c_sTopSpace + c_sPFontSize * titleHight + 20;
			paint.drawLine(xPos1, yPos1, nWidth - c_sLineLeftSpace, yPos1);

			paint.setPen(QPen(QColor("#000000")));
			font.setPixelSize(c_sFontSize);
			paint.setFont(font);
			int xPos2 = nXPos + c_sLeftSpace;
			int yPos2 = yPos1 + 30;
			QRect rect2(xPos2, yPos2, c_sKeyWidth, c_sFontSize);
			paint.drawText(rect2, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("座位号 :"));

			font.setPixelSize(c_sPFontSize);
			paint.setFont(font);
			int xPos3 = xPos2 + c_sKeyWidth - 6;
			int yPos3 = yPos2 - 10;
			QRect rect3(xPos3, yPos3, c_sPValueWidth, c_sPFontSize);
			paint.drawText(rect3, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("%1").arg(m_receiptInfo.nSeatNo));

			font.setPixelSize(c_sFontSize);
			paint.setFont(font);
			int xPos4 = xPos2;
			int yPos4 = yPos3 + 30;
			QRect rect4(xPos4, yPos4, c_sKeyWidth, c_sFontSize);
			paint.drawText(rect4, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("账单号 :"));

			font.setPixelSize(c_sFontSize);
			font.setBold(true);
			paint.setFont(font);
			int xPos5 = xPos3;
			int yPos5 = yPos4;
			QRect rect5(xPos5, yPos5, c_sPValueWidth, c_sFontSize);
			paint.drawText(rect5, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("%1").arg(m_receiptInfo.nPaymentNo));

			font.setPixelSize(c_sFontSize);
			font.setBold(false);
			paint.setFont(font);
			int xPos6 = xPos2;
			int yPos6 = yPos4 + 24;
			QRect rect6(xPos6, yPos6, c_sPValueWidth, c_sFontSize);
			paint.drawText(rect6, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("服务员 : %1").arg(m_receiptInfo.nAccount));
			int xPos7 = xPos2;
			int yPos7 = yPos6 + 22;
			QRect rect7(xPos7, yPos7, c_sPValueWidth, c_sFontSize);
			paint.drawText(rect7, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("单  位 : 元"));

			paint.setPen(pen);
			int yPos8 = yPos7 + 30;
			paint.drawLine(c_sLineLeftSpace, yPos8, nWidth - c_sLineLeftSpace, yPos8);
			int yPos9 = yPos8 + 10;
			paint.drawLine(c_sLineLeftSpace, yPos9, nWidth - c_sLineLeftSpace, yPos9);

			paint.setPen(QPen(QColor("#000000")));
			int x1 = xPos2;
			int y1 = yPos9 + 14;
			QRect firstRect(x1, y1, c_sKeyWidth, c_sFontSize);
			paint.drawText(firstRect, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("品名"));
			int x2 = x1 + 100;
			int y2 = y1;
			QRect secondRect(x2, y2, c_sKeyWidth, c_sFontSize);
			paint.drawText(secondRect, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("数量"));
			int x3 = x2 + 40;
			int y3 = y1;
			QRect thirdRect(x3, y3, c_sKeyWidth, c_sFontSize);
			paint.drawText(thirdRect, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("金额"));

			paint.setPen(pen);
			int y4 = y3 + 26;
			paint.drawLine(c_sLineLeftSpace, y4, nWidth - c_sLineLeftSpace, y4);

			paint.setPen(QPen(QColor("#000000")));
			font.setPixelSize(c_sFontSize);
			paint.setFont(font);
			int x_ = c_sLeftSpace;
			int y_ = y4 + 20;
			for (int i = 0; i < m_receiptInfo.nProductList.size(); ++i)
			{
				QString strName = m_receiptInfo.nProductList[i].nProductName + QString("(%1)").arg(m_receiptInfo.nProductList[i].sTasteGroup);;
				int num = strName.size();
				for (int i = 0; i < num / 7 + 1; ++i)
				{
					QRect rect1_(x_, y_ + c_sFontSize*i + 5 * i, c_sPKeyWidth, c_sFontSize);
					paint.drawText(rect1_, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("%1").arg(strName.mid(i * 7, 7)));
				}
				QRect rect2_(x_ + 100, y_, c_sKeyWidth, c_sFontSize);
				paint.drawText(rect2_, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("%1").arg(m_receiptInfo.nProductList[i].nProductNum));
				QRect rect3_(x_ + 100 + 40, y_, c_sKeyWidth, c_sFontSize);
				paint.drawText(rect3_, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("%1").arg(m_receiptInfo.nProductList[i].nPrice));
				y_ += (num / 7 + 1) * c_sFontSize;
				y_ += 20;
			}

			paint.setPen(pen);
			int y5_ = y_;
			paint.drawLine(c_sLineLeftSpace, y5_, nWidth - c_sLineLeftSpace, y5_);

			paint.setPen(QPen(QColor("#000000")));
			font.setPixelSize(c_sPFontSize);
			paint.setFont(font);
			int xPos1_ = c_sLeftSpace;
			int yPos1_ = y5_ + 10;
			QRect firstRect_(xPos1_, yPos1_, c_sPKeyWidth, c_sPFontSize);
			paint.drawText(firstRect_, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("合计"));
			QRect secondRect_(xPos1_ + c_sPKeyWidth - 70, yPos1_, c_sPKeyWidth, c_sPFontSize);
			paint.drawText(secondRect_, Qt::AlignRight | Qt::AlignVCenter, QStringLiteral("%1").arg(m_receiptInfo.strSum));

			paint.setPen(pen);
			int yPos2_ = yPos1_ + 30;
			paint.drawLine(c_sLineLeftSpace, yPos2_, nWidth - c_sLineLeftSpace, yPos2_);

			paint.setPen(QPen(QColor("#000000")));
			font.setPixelSize(c_sFontSize);
			paint.setFont(font);
			paint.drawText(QRect(c_sLeftSpace, yPos2_ + 10, nWidth, c_sFontSize), Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("打印时间 : %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm")));

			font.setPixelSize(c_sMFontSize);
			paint.setFont(font);
			paint.drawText(QRect(c_sLeftSpace + 10, yPos2_ + 60, nWidth, c_sMFontSize), Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("谢谢光临，欢迎再次惠顾！"));
			paint.end();
	//	}
}

void ReceiptPrinterMaker::saveImageDetialFunc()
{
	m_pPixmap = new QPixmap(m_nZoomWidth, m_nPictureMaxHeight);
	m_pPixmap->fill(Qt::white);
	m_pPainter = new QPainter(m_pPixmap);

	int nWidth = 210;
	int nXPos = 0;
	int nYPos = 0;

	QFont font;
	font.setFamily("微软雅黑");
	font.setPixelSize(c_sPFontSize);

	m_pPainter->setFont(font);
	m_pPainter->setPen(QPen(QColor("#000000")));
	QString sBarName = m_receiptInfo.sBarName;
	int titleHight = sBarName.size() / 7 + 1;
	for (int i = 0; i < titleHight; ++i)
	{
		QRect rect1(0, c_sTopSpace + c_sFontSize*i + 10 * i, nWidth, c_sPFontSize);
		m_pPainter->drawText(rect1, Qt::AlignHCenter | Qt::AlignVCenter, QStringLiteral("%1").arg(sBarName.mid(i * 7, 7)));
	}


	QPen pen(QBrush(QColor("#000000")), 1, Qt::DashLine);
	m_pPainter->setPen(pen);
	int xPos1 = nXPos + c_sLineLeftSpace;
	int yPos1 = c_sTopSpace + c_sPFontSize * titleHight + 20;
	m_pPainter->drawLine(xPos1, yPos1, nWidth - c_sLineLeftSpace, yPos1);

	m_pPainter->setPen(QPen(QColor("#000000")));
	font.setPixelSize(c_sFontSize);
	m_pPainter->setFont(font);
	int xPos2 = nXPos + c_sLeftSpace;
	int yPos2 = yPos1 + 30;
	QRect rect2(xPos2, yPos2, c_sKeyWidth, c_sFontSize);
	m_pPainter->drawText(rect2, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("座位号 :"));

	font.setPixelSize(c_sPFontSize);
	m_pPainter->setFont(font);
	int xPos3 = xPos2 + c_sKeyWidth - 6;
	int yPos3 = yPos2 - 10;
	QRect rect3(xPos3, yPos3, c_sPValueWidth, c_sPFontSize);
	m_pPainter->drawText(rect3, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("%1").arg(m_receiptInfo.nSeatNo));

	font.setPixelSize(c_sFontSize);
	m_pPainter->setFont(font);
	int xPos4 = xPos2;
	int yPos4 = yPos3 + 30;
	QRect rect4(xPos4, yPos4, c_sKeyWidth, c_sFontSize);
	m_pPainter->drawText(rect4, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("账单号 :"));

	font.setPixelSize(c_sFontSize);
	font.setBold(true);
	m_pPainter->setFont(font);
	int xPos5 = xPos3;
	int yPos5 = yPos4;
	QRect rect5(xPos5, yPos5, c_sPValueWidth, c_sFontSize);
	m_pPainter->drawText(rect5, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("%1").arg(m_receiptInfo.nPaymentNo));

	font.setPixelSize(c_sFontSize);
	font.setBold(false);
	m_pPainter->setFont(font);
	int xPos6 = xPos2;
	int yPos6 = yPos4 + 24;
	QRect rect6(xPos6, yPos6, c_sPValueWidth, c_sFontSize);
	m_pPainter->drawText(rect6, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("服务员 : %1").arg(m_receiptInfo.nAccount));
	int xPos7 = xPos2;
	int yPos7 = yPos6 + 22;
	QRect rect7(xPos7, yPos7, c_sPValueWidth, c_sFontSize);
	m_pPainter->drawText(rect7, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("单  位 : 元"));

	m_pPainter->setPen(pen);
	int yPos8 = yPos7 + 30;
	m_pPainter->drawLine(c_sLineLeftSpace, yPos8, nWidth - c_sLineLeftSpace, yPos8);
	int yPos9 = yPos8 + 10;
	m_pPainter->drawLine(c_sLineLeftSpace, yPos9, nWidth - c_sLineLeftSpace, yPos9);

	m_pPainter->setPen(QPen(QColor("#000000")));
	int x1 = xPos2;
	int y1 = yPos9 + 14;
	QRect firstRect(x1, y1, c_sKeyWidth, c_sFontSize);
	m_pPainter->drawText(firstRect, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("品名"));
	int x2 = x1 + 100;
	int y2 = y1;
	QRect secondRect(x2, y2, c_sKeyWidth, c_sFontSize);
	m_pPainter->drawText(secondRect, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("数量"));
	int x3 = x2 + 40;
	int y3 = y1;
	QRect thirdRect(x3, y3, c_sKeyWidth, c_sFontSize);
	m_pPainter->drawText(thirdRect, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("金额"));

	m_pPainter->setPen(pen);
	int y4 = y3 + 26;
	m_pPainter->drawLine(c_sLineLeftSpace, y4, nWidth - c_sLineLeftSpace, y4);

	m_pPainter->setPen(QPen(QColor("#000000")));
	font.setPixelSize(c_sFontSize);
	m_pPainter->setFont(font);
	int x_ = c_sLeftSpace;
	int y_ = y4 + 20;
	for (int i = 0; i < m_receiptInfo.nProductList.size(); ++i)
	{
		QString strName = m_receiptInfo.nProductList[i].nProductName + QString("(%1)").arg(m_receiptInfo.nProductList[i].sTasteGroup);
		int num = strName.size();
		for (int i = 0; i < num / 7 + 1; ++i)
		{
			QRect rect1_(x_, y_ + c_sFontSize*i + 5 * i, c_sPKeyWidth, c_sFontSize);
			m_pPainter->drawText(rect1_, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("%1").arg(strName.mid(i * 7, 7)));
		}
		QRect rect2_(x_ + 100, y_, c_sKeyWidth, c_sFontSize);
		m_pPainter->drawText(rect2_, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("%1").arg(m_receiptInfo.nProductList[i].nProductNum));
		QRect rect3_(x_ + 100 + 40, y_, c_sKeyWidth, c_sFontSize);
		m_pPainter->drawText(rect3_, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("%1").arg(m_receiptInfo.nProductList[i].nPrice));
		y_ += (num / 7 + 1) * c_sFontSize;
		y_ += 20;
	}

	m_pPainter->setPen(pen);
	int y5_ = y_;
	m_pPainter->drawLine(c_sLineLeftSpace, y5_, nWidth - c_sLineLeftSpace, y5_);

	m_pPainter->setPen(QPen(QColor("#000000")));
	font.setPixelSize(c_sPFontSize);
	m_pPainter->setFont(font);
	int xPos1_ = c_sLeftSpace;
	int yPos1_ = y5_ + 10;
	QRect firstRect_(xPos1_, yPos1_, c_sPKeyWidth, c_sPFontSize);
	m_pPainter->drawText(firstRect_, Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("合计"));
	QRect secondRect_(xPos1_ + c_sPKeyWidth - 70, yPos1_, c_sPKeyWidth, c_sPFontSize);
	m_pPainter->drawText(secondRect_, Qt::AlignRight | Qt::AlignVCenter, QStringLiteral("%1").arg(m_receiptInfo.strSum));

	m_pPainter->setPen(pen);
	int yPos2_ = yPos1_ + 30;
	m_pPainter->drawLine(c_sLineLeftSpace, yPos2_, nWidth - c_sLineLeftSpace, yPos2_);

	m_pPainter->setPen(QPen(QColor("#000000")));
	font.setPixelSize(c_sFontSize);
	m_pPainter->setFont(font);
	m_pPainter->drawText(QRect(c_sLeftSpace, yPos2_ + 10, nWidth, c_sFontSize), Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("打印时间 : %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm")));

	font.setPixelSize(c_sMFontSize);
	m_pPainter->setFont(font);
	m_pPainter->drawText(QRect(c_sLeftSpace + 10, yPos2_ + 60, nWidth, c_sMFontSize), Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("谢谢光临，欢迎再次惠顾！"));

	QPixmap pixmap = m_pPixmap->copy(0, 0, 210, yPos2_ + 60);
	pixmap.save(QApplication::applicationDirPath() + QString("/pic/%1.bmp").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss")));

	m_pPainter->end();
	delete m_pPainter;
	m_pPainter = NULL;
	delete m_pPixmap;
	m_pPixmap = NULL;
}

ReceiptPrinterMaker::ReceiptPrinterMaker()
: m_nPictureMaxHeight(1200)
, m_pPainter(NULL)
, m_pPixmap(NULL)
, m_nZoomWidth(450) // 打印机的缩放宽度， 打印机的dll会将450的图片缩放到174大小
{

}

ReceiptPrinterMaker::~ReceiptPrinterMaker()
{

}
