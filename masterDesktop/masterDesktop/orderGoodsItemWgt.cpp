#include "orderGoodsItemWgt.h"
#include <QUrl>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "logging.hpp"

orderGoodsItemWgt::orderGoodsItemWgt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.btn_reduce->hide();
	ui.lbl_num->hide();
	ui.lbl_num->setText("0");

	InitConnect();
}

orderGoodsItemWgt::~orderGoodsItemWgt()
{

}

void orderGoodsItemWgt::setData(const GoodsInfo_t &goodsInfo)
{
	m_goodsInfo = goodsInfo;
	m_goodsInfo.dTotalPrice = m_goodsInfo.dPrice;
	ui.lbl_name->setText(QStringLiteral("%1").arg(m_goodsInfo.sName));
	ui.lbl_price->setText(QStringLiteral("¥") + QString::number(m_goodsInfo.dPrice, 'f', 2));
	setNum(goodsInfo.nOrderNum);

	QPixmap pixmap;
	pixmap.loadFromData(getNetworkPic(goodsInfo.sImgUrl));
	pixmap.scaled(ui.lbl_icon->width(), ui.lbl_icon->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	ui.lbl_icon->setPixmap(pixmap);
	ui.lbl_icon->setScaledContents(true);
}

void orderGoodsItemWgt::setNum(int num)
{
	if (num == 0)
	{
		ui.btn_reduce->hide();
		ui.lbl_num->hide();
	}
	else
	{
		ui.btn_reduce->show();
		ui.lbl_num->show();
	}
	ui.lbl_num->setText(QString::number(num));
}

void orderGoodsItemWgt::slotOnAdd()
{
	int curr_num = ui.lbl_num->text().toInt();
	if (curr_num < m_goodsInfo.nMainStockNum)
	{
		if (curr_num == 0)
		{
			ui.btn_reduce->show();
			ui.lbl_num->show();
		}
		ui.lbl_num->setText(QString::number(++curr_num));
		emit sigOnAddGoodsItem(m_goodsInfo);
	}
}

void orderGoodsItemWgt::slotOnReduce()
{
	int curr_num = ui.lbl_num->text().toInt();
	if (curr_num > 0)
	{
		if (curr_num == 1)
		{
			ui.btn_reduce->hide();
			ui.lbl_num->hide();
		}
		ui.lbl_num->setText(QString::number(--curr_num));
		emit sigOnReduceGoodsItem(m_goodsInfo);
	}
}

QByteArray orderGoodsItemWgt::getNetworkPic(const QString &szUrl)
{
	QUrl url(szUrl);
	QNetworkAccessManager manager;
	QEventLoop loop;

	QNetworkReply *reply = manager.get(QNetworkRequest(url));
	QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	loop.exec();

	QByteArray picData = reply->readAll();
	return picData;
}

void orderGoodsItemWgt::InitConnect()
{
	connect(ui.btn_add, SIGNAL(clicked()), this, SLOT(slotOnAdd()));
	connect(ui.btn_reduce, SIGNAL(clicked()), this, SLOT(slotOnReduce()));
}
