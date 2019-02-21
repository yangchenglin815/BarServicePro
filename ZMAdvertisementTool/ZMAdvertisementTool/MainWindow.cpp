#include "mainwindow.h"
#include <QGraphicsDropShadowEffect>
#include <QDesktopWidget>
#include <QDebug>
#include <QProcess>

#include <QFile>
#include <QImage>
#include <QBuffer>
#include <QByteArray>
#include <QTimer>
#include <QLayout>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <QDir>
#include <QPropertyAnimation>
#include "HttpAgent.h"
#include "enumdata.h"
#include "logging.hpp"
#include "appConfigIni.h"

#define WIN

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
, m_interval(0)
, m_mouseIn(false)
, m_startTime(QDateTime::currentDateTime())
{
	ui->setupUi(this);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setWindowTitle("ZMAdvertisementTool");

	//开机自启
	QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
	QString strAppPath = QDir::toNativeSeparators(QApplication::applicationFilePath());
	reg.setValue("ZMAdvertisementTool", strAppPath);

	//实例阴影shadow
	m_shadow = new QGraphicsDropShadowEffect(this);
	//设置阴影距离
	m_shadow->setOffset(0, 0);
	//设置阴影颜色
	m_shadow->setColor(QColor("#444444"));
	//设置阴影圆角
	m_shadow->setBlurRadius(30);
	//给嵌套QWidget设置阴影
	ui->widget->setGraphicsEffect(m_shadow);

	QString path = QApplication::applicationDirPath() + "/config/config.ini";
	appConfigIni::setIniPath(path);

	Init();
	InitConnect();

	HttpAgent::instance()->RequestOnGetAdvertisementList();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::slotOnClose()
{
	commitStatisticalData();
}

void MainWindow::anchorClickedSlot(const QUrl &url)
{
	QString cmd;
#ifdef MAC
	cmd = QString("open %1").arg(url.toString());
#endif

#ifdef WIN
	cmd = QString("cmd /c start %1").arg(url.toString());
#endif

#ifdef LINUX
	cmd = QString("x-www-browser %1").arg(url.toString());
#endif
	Log::Info("cmd /c start %s", url.toString().toStdString().c_str());
	QProcess::execute(cmd);
}

void MainWindow::slotOnGetAdvertisementList(QList<AdvertisementInfo_t> advertisementInfoList, int nCode, QString sMsg)
{
	if (nCode != e_success)
	{
		Log::Info("Request On GetAdvertisementList failed : " + sMsg.toStdString());
		HttpAgent::instance()->RequestOnGetAdvertisementList();
	}
	else
	{
		Log::Info("Request On GetAdvertisementList success");
		m_advertisementInfoList = advertisementInfoList;
		InitUi();
	}
}

void MainWindow::slotOnCommitStatisticalData(int nCode, QString sMsg)
{
	if (nCode != e_success)
	{
		Log::Info("Request On CommitStatisticalData failed : " + sMsg.toStdString());
	}
	else
	{
		Log::Info("Request On CommitStatisticalData success");
	}
}


void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
	Q_UNUSED(event)
	qreal x = event->localPos().x() - 24;
	qreal y = event->localPos().y() - 24;
	qreal x1 = ui->wgt_bottom->pos().rx();
	qreal y1 = ui->wgt_bottom->pos().ry();
	qreal x2 = ui->btn_close->pos().rx();
	qreal y2 = ui->btn_close->pos().ry();
	if (x > x1 && x < x1 + ui->wgt_bottom->width() && y > y1 && y < y1 + ui->wgt_bottom->height()) {
		setCursor(Qt::PointingHandCursor);
		m_mouseIn = true;
	}
	else if (x > x2 && x < x2 + ui->btn_close->width() && y > y2 && y < y2 + ui->btn_close->height()){
		setCursor(Qt::PointingHandCursor);
		m_mouseIn = false;
	}
	else {
		setCursor(Qt::ArrowCursor);
		m_mouseIn = false;
	}
	QMainWindow::mouseMoveEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
	Q_UNUSED(event)
	if (event->button() == Qt::LeftButton && m_mouseIn) {
		if (!m_jumpUrl.isEmpty()){
			anchorClickedSlot(QUrl(m_jumpUrl));
			commitStatisticalData();
		}
	}
	QMainWindow::mousePressEvent(event);
}

void MainWindow::Init()
{
	getScreenInfo();
	move(g_nActScreenW - width() + 10, g_nActScreenH - height());

	ui->btn_close->hide();
	setMouseTracking(true);
	ui->centralWidget->setMouseTracking(true);
	ui->widget->setMouseTracking(true);
	ui->wgt_top->setMouseTracking(true);
	ui->wgt_bottom->setMouseTracking(true);
	ui->lbl_image->setMouseTracking(true);
	ui->btn_close->setMouseTracking(true);

	m_timer = new QTimer(this);
	m_timer->setInterval(1000);
	connect(m_timer, &QTimer::timeout, [&](){
		ui->lbl_time->setText(QString::number(m_interval--) + QStringLiteral("秒"));
		if (m_interval == -1){
			ui->lbl_time->hide();
			ui->btn_close->show();
			QLayout *layout = ui->wgt_top->layout();
			layout->setContentsMargins(10, 0, 0, 0);
			ui->wgt_top->setLayout(layout);
			Log::Info("stop the timer.");
			m_timer->stop();
		}
	});
}

void MainWindow::InitConnect()
{
	connect(ui->btn_close, SIGNAL(clicked()), this, SLOT(slotOnClose()));

	qRegisterMetaType<QList<AdvertisementInfo_t> >("QList<AdvertisementInfo_t>");
	connect(HttpAgent::instance(), SIGNAL(sigOnGetAdvertisementList(QList<AdvertisementInfo_t>, int, QString)), this, SLOT(slotOnGetAdvertisementList(QList<AdvertisementInfo_t>, int, QString)));
	connect(HttpAgent::instance(), SIGNAL(sigOnCommitStatisticalData(int, QString)), this, SLOT(slotOnCommitStatisticalData(int, QString)));
}

void MainWindow::getScreenInfo()
{
	QDesktopWidget* desktopWidget = QApplication::desktop();
	QRect screenRect = desktopWidget->screenGeometry();
	g_nActScreenW = screenRect.width();
	g_nActScreenH = screenRect.height();
}

void MainWindow::InitUi()
{
	if (m_advertisementInfoList.size() > 0) {
		AdvertisementInfo_t advertisementInfo = m_advertisementInfoList[0];
		if (advertisementInfo.sTitle.size() > 10)
		{
			ui->lbl_title->setText(QStringLiteral("%1").arg(advertisementInfo.sTitle.left(10) + "..."));
		}
		else
		{
			ui->lbl_title->setText(QStringLiteral("%1").arg(advertisementInfo.sTitle));
		}
		ui->lbl_content->setText(QStringLiteral("%1").arg(advertisementInfo.sDetial));
		m_jumpUrl = advertisementInfo.sJumpUrl;
		m_interval = advertisementInfo.nDelayTime;

		QPixmap pixmap;
		pixmap.loadFromData(getNetworkPic(advertisementInfo.sImage));
		pixmap.scaled(ui->lbl_image->width(), ui->lbl_image->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		ui->lbl_image->setPixmap(pixmap);
		ui->lbl_image->setScaledContents(true);

		QPropertyAnimation *an = new QPropertyAnimation(this, "windowOpacity");
		an->setDuration(1000);
		an->setStartValue(0);
		an->setEndValue(1);
		an->start();
		this->show();

		m_startTime = QDateTime::currentDateTime();
		Log::Info("start the timer.");
		m_timer->start();
		return;
	}
	Log::Info("don't find everything for advertisement!");
}

QByteArray MainWindow::getNetworkPic(const QString &szUrl)
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

void MainWindow::commitStatisticalData()
{
	qint64 diffTime = m_startTime.secsTo(QDateTime::currentDateTime());
	HttpAgent::instance()->RequestOnCommitStatisticalData(diffTime);
	this->hide();
}

void MainWindow::picToData(QString fromPic, QString toData, QString myformat)
{
	QImage img(fromPic);
	QByteArray ba;
	QBuffer buf(&ba);
	img.save(&buf, "JPG");//按照JPG解码保存数据
	QByteArray cc = qCompress(ba, 1);
	QByteArray hh;
	if (myformat == "Hex")
	{
		hh = cc.toHex();//16进制数据
	}
	else
	{
		hh = cc.toBase64();//base64数据
	}
	QString str(hh);
	QFile file(toData);
	if (file.open(QFile::WriteOnly))
	{
		file.write(str.toLatin1());
		file.close();
	}
}

void MainWindow::dataToPic(QString fromData, QString toPic, QString myformat)
{
	QFile file(fromData);
	if (file.open(QFile::ReadOnly))
	{
		QByteArray read = file.readAll();
		file.close();
		QString ss = QString::fromLatin1(read.data(), read.size());
		QByteArray rc;
		if (myformat == "Hex")
		{
			rc = QByteArray::fromHex(ss.toLatin1());
		}
		else
		{
			rc = QByteArray::fromBase64(ss.toLatin1());
		}
		QByteArray rdc = qUncompress(rc);

		QImage img;
		img.loadFromData(rdc);
		img.save(toPic);
	}
}

