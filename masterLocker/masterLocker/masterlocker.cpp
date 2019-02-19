#include "masterlocker.h"
#include <QPushButton>
#include <QDesktopWidget>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QMessageBox>
#include <QSettings>
#include <QDir>

#include "hookhelper.h"
#include "HttpAgent.h"
#include "enumData.h"
#include "commonFun.h"
#include "HttpAgent.h"
#include "webSocketAgent.h"
#include "constData.h"

masterLocker::masterLocker(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	kill_exe_.KillProcess((L"masterLocker.exe"));
	setAttribute(Qt::WA_TranslucentBackground, true);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	setWindowTitle("masterLocker");
	setWindowIcon(QIcon(":/masterLocker/Resources/masterLocker.ico"));

	QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
	QString strAppPath = QDir::toNativeSeparators(QApplication::applicationFilePath());
	reg.setValue("masterLocker", strAppPath);

	isRuning = false;
	connect(this, SIGNAL(run()), SLOT(kill()));

	QString path = QApplication::applicationDirPath() + "/config/config.ini";
	appConfigIni::setIniPath(path);

	socketRequest::instance()->initTcpSocket();
	connect(ui.btn_confirm, SIGNAL(clicked()), this, SLOT(slotOnConfirm()));

	initUI();
	initConnect();
	webSocketAgent::instance();

	m_nPCName = appConfigIni::getLocalmachineName();
	m_nBarId = appConfigIni::getBarId();
	m_nMac = appConfigIni::getHostMacAddress().replace(":", "-");
//	HttpAgent::instance()->RequestOnGetDownloadUrl();
	ui.lbl_pcno->setText(QStringLiteral("%1号机").arg(m_nPCName));
	HttpAgent::instance()->RequestOnReportOnlineStatus(m_nPCName, m_nBarId, m_nMac, QString::number(s_onLine));

	m_timer = new QTimer(this);
	m_timer->setInterval(20000);
	connect(m_timer, &QTimer::timeout, [&](){
		socketRequest::instance()->requestOnHeartBeat();
	});
	m_timer->start();
}

masterLocker::~masterLocker()
{

}

void masterLocker::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	QImage image;
	image.load(":/masterLocker/Resources/bg.png");
	QPainter draw(this);
	QRect rect(0, 0, this->width(), this->height());
	draw.drawImage(rect, image);
}

void masterLocker::showEvent(QShowEvent *event)
{
	this->setAttribute(Qt::WA_Mapped);

	QMainWindow::showEvent(event);
}

void masterLocker::keyPressEvent(QKeyEvent *event)
{
	event->ignore();
}

void masterLocker::closeEvent(QCloseEvent *event)
{
	if (event->spontaneous())
		event->ignore();
	else event->accept();
}

void masterLocker::initUI()
{
	QDesktopWidget* desktopWidget = QApplication::desktop();
	QRect screenRect = desktopWidget->screenGeometry();
	g_nActScreenW = screenRect.width();
	g_nActScreenH = screenRect.height();
	this->resize(g_nActScreenW, g_nActScreenH);


	ui.leftWidget->setStyleSheet("background-image: url(:/masterLocker/Resources/left_bg.png);");
	ui.leftWidget->resize(571, 341);
	ui.leftWidget->move(-35, -11);

	ui.mainWidget->setStyleSheet("background-image: url(:/masterLocker/Resources/QRcode_bg.png);");
	ui.mainWidget->resize(667, 368);
	ui.mainWidget->move((this->width() - 667) / 2, (this->height() - 368) / 2 - 60);

	ui.lineEdit->resize(250, 40);
	ui.lineEdit->move((this->width() - 667) / 2 + 130, ui.mainWidget->y() + ui.mainWidget->height() + 30);
	ui.lineEdit->setPlaceholderText(QStringLiteral("请输入幽游订座验证码"));
	
	ui.btn_confirm->resize(100, 40);
	ui.btn_confirm->move(ui.lineEdit->x() + ui.lineEdit->width() + 16, ui.lineEdit->y());
	ui.btn_confirm->setFocus();

	ui.lbl_hint->resize(667, 40);
	ui.lbl_hint->move(ui.mainWidget->x(), ui.lineEdit->y() + ui.lineEdit->height() + 20);

	ui.lbl_logo->resize(90, 56);
	ui.lbl_logo->move(31, 28);

	ui.lbl_pcno->resize(275, 30);
	ui.lbl_pcno->move(190, 28);

	ui.lbl_tip->resize(275, 30);
	ui.lbl_tip->move(190, 65);

	ui.lbl_title->resize(347, 80);
	ui.lbl_title->move(ui.mainWidget->x() + 32, ui.mainWidget->y() + 100);

	ui.lbl_scan->resize(347, 80);
	ui.lbl_scan->move(ui.mainWidget->x() + 32, ui.lbl_title->y() + ui.lbl_title->height() + 25);

	ui.qrWidget->resize(215, 215);
	ui.qrWidget->move(ui.mainWidget->x() + 379, ui.mainWidget->y() + 91);
	ui.qrWidget->generateString(c_sAppDownloadUrl);
}

void masterLocker::initConnect()
{
	connect(HttpAgent::instance(), SIGNAL(sigOnGetBookSeatStatus(int, QString, int)), this, SLOT(slotOnGetBookSeatStatus(int, QString, int)));
	connect(HttpAgent::instance(), SIGNAL(sigOnGetVerifyUnlockCode(int, QString)), this, SLOT(slotOnGetVerifyUnlockCode(int, QString)));
	connect(HttpAgent::instance(), SIGNAL(sigOnReportOnlineStatus(int, QString)), this, SLOT(slotOnReportOnlineStatus(int, QString)));
	connect(HttpAgent::instance(), SIGNAL(sigOnGetDownloadUrl(int, QString, QString)), this, SLOT(slotOnGetDownloadUrl(int, QString, QString)));
	connect(HttpAgent::instance(), SIGNAL(sigOnGetBarName(int, QString, QString)), this, SLOT(slotOnGetBarName(int, QString, QString)));
	connect(webSocketAgent::instance(), SIGNAL(sigOnWSLockScreenFunc()), this, SLOT(startLock()));
	connect(webSocketAgent::instance(), SIGNAL(sigOnWSUnLockScreenFunc()), this, SLOT(stopLock()));
}

void masterLocker::handleBookSeatStatus(int nStatus)
{
	//根据订座状态处理UI
	switch (nStatus)
	{
	case s_unBookSeat:
		stopLock();
		break;
	case s_bookSeat:
		startLock();
		break;
	default:
		break;
	}
}

void masterLocker::kill()
{
	if (process.state() & QProcess::Running)
		process.waitForFinished();
	process.start("cmd /c taskkill /im taskmgr.exe");
	if (isRuning)
		QTimer::singleShot(500, this, SLOT(kill()));
}

void masterLocker::startLock()
{
	HookHelper::instance()->setHookFunc();

	isRuning = true;
	showNormal();
	emit run();
}

void masterLocker::stopLock()
{
	HookHelper::instance()->unHookFunc();

	isRuning = false;
	hide();
}

void masterLocker::slotOnGetBookSeatStatus(int nCode, QString sMsg, int nStatus)
{
	if (nCode != e_success)
	{
		Log::Info("Request On GetBookSeatStatus failed : %s", sMsg.toLatin1().data());
		ui.lbl_hint->setText(QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	//	QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		Log::Info("Request On GetBookSeatStatus success : %d", nStatus);
		//handleBookSeatStatus(nStatus);
	}
}

void masterLocker::slotOnGetVerifyUnlockCode(int nCode, QString sMsg)
{
	if (nCode == e_success)
	{
		Log::Info("Verify Unlock success !");
		ui.lbl_hint->setText("");
		stopLock();
	}
	else if (nCode == e_connectServerError)
	{
		Log::Info("Request On GetVerifyUnlockCode failed : %s", sMsg.toStdString().c_str());
		ui.lbl_hint->setText(QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
		//QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		Log::Info("Request On GetVerifyUnlockCode failed : %s", sMsg.toStdString().c_str());
		QString msg = commonFun::getBookingErrMsg(nCode);
		ui.lbl_hint->setText(QStringLiteral("%1").arg(msg));
	}
}

void masterLocker::slotOnReportOnlineStatus(int nCode, QString sMsg)
{
	if (nCode != e_success)
	{
		Log::Info("Request On ReportOnlineStatus failed : %s", sMsg.toStdString().c_str());
		ui.lbl_hint->setText(QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	//	QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		Log::Info("Request On ReportOnlineStatus success");
		HttpAgent::instance()->RequestOnGetBookSeatStatus(m_nPCName, m_nBarId, m_nMac);
	}
}

void masterLocker::slotOnGetDownloadUrl(int nCode, QString sMsg, QString nUrl)
{
	if (nCode != e_success)
	{
		Log::Info("Request On GetAppDownloadUrl failed : %s", sMsg.toStdString().c_str());
		ui.lbl_hint->setText(QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
		//QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		Log::Info("Request On GetAppDownloadUrl success");
		ui.qrWidget->generateString(nUrl);
	}
}

void masterLocker::slotOnGetBarName(int nCode, QString sMsg, QString nBarName)
{
	if (nCode != e_success)
	{
		Log::Info("Request On GetBarName failed : %s", sMsg.toStdString().c_str());
		ui.lbl_hint->setText(QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
		//QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		Log::Info("Request On GetBarName success");		
	}
}

void masterLocker::slotOnConfirm()
{
	QString code = ui.lineEdit->text();
	if (!code.isEmpty())
	{
		if (code == "10086")
		{
			stopLock();
			return;
		}
		HttpAgent::instance()->RequestOnGetVerifyUnlockCode(m_nPCName, m_nBarId, m_nMac, code);
		return;
	}
	Log::Info("the code is null !");
}
