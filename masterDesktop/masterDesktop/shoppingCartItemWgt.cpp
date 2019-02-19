#include "shoppingCartItemWgt.h"
#include <QUrl>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "logging.hpp"

shoppingCartItemWgt::shoppingCartItemWgt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	InitConnect();
}

shoppingCartItemWgt::~shoppingCartItemWgt()
{

}

void shoppingCartItemWgt::setData(GoodsInfo_t goodsInfo)
{
	m_goodsInfo = goodsInfo;

	ui.lbl_name->setText(QStringLiteral("%1").arg(m_goodsInfo.sName));
	ui.lbl_standard->setText(QStringLiteral("%1").arg(goodsInfo.sUnit));
	ui.lbl_price->setText(QStringLiteral("¥") + QString::number(goodsInfo.dTotalPrice, 'f', 2));
	ui.lbl_num->setText(QString::number(goodsInfo.nOrderNum));

	QPixmap pixmap;
	pixmap.loadFromData(getNetworkPic(goodsInfo.sImgUrl));
	pixmap.scaled(ui.lbl_icon->width(), ui.lbl_icon->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ui.lbl_icon->setPixmap(pixmap);
	ui.lbl_icon->setScaledContents(true);
}

void shoppingCartItemWgt::slotOnAdd()
{
	int curr_num = ui.lbl_num->text().toInt();
	if (curr_num < m_goodsInfo.nMainStockNum)
	{
		ui.lbl_num->setText(QString::number(++curr_num));
		m_goodsInfo.nOrderNum = curr_num;
		m_goodsInfo.dTotalPrice = m_goodsInfo.dPrice * curr_num;
		ui.lbl_price->setText(QString::number(m_goodsInfo.dTotalPrice, 'f', 2));
		emit sigOnSyncAddGoodsInfo(m_goodsInfo);
	}
}

void shoppingCartItemWgt::slotOnReduce()
{
	int curr_num = ui.lbl_num->text().toInt();
	if (curr_num > 0)
	{
		ui.lbl_num->setText(QString::number(--curr_num));
		m_goodsInfo.nOrderNum = curr_num;
		m_goodsInfo.dTotalPrice = m_goodsInfo.dPrice * curr_num;
		ui.lbl_price->setText(QString::number(m_goodsInfo.dTotalPrice, 'f', 2));
		emit sigOnSyncReduceGoodsInfo(m_goodsInfo);
	}
}

QByteArray shoppingCartItemWgt::getNetworkPic(const QString &szUrl)
{
	QUrl url(szUrl);
	QNetworkAccessManager manager;
	QEventLoop loop;

	Log::Info("reading network pic from : %s", szUrl.toLocal8Bit().data());
	QNetworkReply *reply = manager.get(QNetworkRequest(url));
	QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	loop.exec();

	QByteArray picData = reply->readAll();
	return picData;
}

void shoppingCartItemWgt::InitConnect()
{
	connect(ui.btn_add, SIGNAL(clicked()), this, SLOT(slotOnAdd()));
	connect(ui.btn_reduce, SIGNAL(clicked()), this, SLOT(slotOnReduce()));
}
