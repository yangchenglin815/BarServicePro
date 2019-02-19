#include "mainwindow.h"
#include "SocketManager.h"
#include "CallServerIni.h"
#include "appconfig.h"
#include "handleEvent.h"
#include "FramelessHelper.h"
#include "webSocketAgent.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QDebug>
#include <QTimer>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QSlider>

#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>
#include <QMenu>
#include <QAction>

using namespace dm::server;
using namespace dm::client;

void func(){
	api_boardcast_info_t api_info;
	int num = 5;
    while (num)
    {
		api_info.nBoardCastType = DEV_OFFLINE;
		api_info.nPlayCycleNum = 1;
		memcpy(api_info.nSeatNo, QString::number(num).toStdString().c_str(), API_ARRAY_LEN);
		handleEvent::instance()->addBoardCastListItem(api_info);
		num--;
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
	, m_trayIcon(nullptr)
{
	kill_exe_.KillProcess(_T(L"CallServer.exe"));
	setAttribute(Qt::WA_TranslucentBackground, true);
	setWindowFlags(Qt::FramelessWindowHint/* | Qt::WindowStaysOnTopHint*/);
	setWindowTitle("Call-Server");
	setWindowIcon(QIcon(":/resource/icon-128.ico"));

	FramelessHelper *helper = new FramelessHelper;
	helper->activateOn(this);
	helper->setDbClickOn(false);
	helper->setWidgetResizable(false);
	CallServerIni::setIniPath(QApplication::applicationDirPath() + "/config/config.ini");

	initUI();
	initConnect();
	webSocketAgent::instance();
	initTcpSocket();
/* 音频测试 */
//  	api_client_info_t api_reg;
//  	handleEvent::instance()->playBoardcastMsg(api_reg, "数字/1.mp3");
// 	handleEvent::instance()->waitforPlayFinished();
// 	Sleep(1000);
//  	handleEvent::instance()->playBoardcastMsg(api_reg, "数字/2.mp3");
// 	handleEvent::instance()->waitforPlayFinished();
}

MainWindow::~MainWindow()
{
	if (tc_comm_ptr_ != NULL){
		tc_comm_ptr_->close();
		tc_comm_ptr_ = NULL;
	}

	if (comm_socket_thread_ && comm_socket_thread_->joinable())
	{
		comm_socket_thread_->join();
	}
}

void MainWindow::showAutoRunApp()
{
	if (CallServerIni::getAutoExecFlag())
	{
		initSystemtray();
	}
	else
	{
		showNormal();
	}
}

void MainWindow::initTcpSocket()
{
	if (comm_socket_thread_ && comm_socket_thread_->joinable())
	{
		comm_socket_thread_->join();
	}
	comm_socket_thread_.reset(new boost::thread(&MainWindow::RunCommSocketThreadFunc, this));

}

void MainWindow::initUI()
{
	btn_min = new QPushButton(this);
	btn_min->resize(36, 36);
	btn_min->setStyleSheet("QPushButton{background-image:url(:/resource/btn_minimum_normal2x.png);background-color:rgba(0,0,0,0);}QPushButton:pressed{margin:1px;}");
	btn_min->move(371, 24);

	btn_close = new QPushButton(this);
	btn_close->resize(36, 36);
	btn_close->setStyleSheet("QPushButton{background-image:url(:/resource/btn_close_normal2x.png);background-color:rgba(0,0,0,0);}QPushButton:pressed{margin:1px;}");
	btn_close->move(421, 24);

	QLabel *lbl_title = new QLabel(this);
	lbl_title->resize(410, 42);
	lbl_title->setStyleSheet(QString("background-color:#002E29;border:1px solid #00947E;font-size:18px;font-family:%1;color:#FFFFFF;").arg(QStringLiteral("微软雅黑")));
	lbl_title->move(34, 87);
	lbl_title->setText(QStringLiteral("全游电竞营销语音端"));
	lbl_title->setAlignment(Qt::AlignCenter);

	lbl_tip = new QLabel(this);
	lbl_tip->resize(410, 13);
	lbl_tip->setStyleSheet(QString("font-size:12px;font-family:%1;color:#FFFFFF;").arg(QStringLiteral("微软雅黑")));
	lbl_tip->setText(QStringLiteral("服务器未连接"));
	lbl_tip->move(34, 150);
	lbl_tip->setAlignment(Qt::AlignCenter);

	QLabel *lbl_call = new QLabel(this);
	lbl_call->resize(72, 16);
	lbl_call->setStyleSheet(QString("font-size:16px;font-family:%1;color:#FFFFFF;").arg(QStringLiteral("微软雅黑")));
	lbl_call->setText(QStringLiteral("呼叫音量 : "));
	lbl_call->move(52, 205);

	QLabel *lbl_play = new QLabel(this);
	lbl_play->resize(72, 16);
	lbl_play->setStyleSheet(QString("font-size:16px;font-family:%1;color:#FFFFFF;").arg(QStringLiteral("微软雅黑")));
	lbl_play->setText(QStringLiteral("播放音量 : "));
	lbl_play->move(52, 261);

	QLabel *lbl_tip_ = new QLabel(this);
	lbl_tip_->resize(410, 12);
	lbl_tip_->setStyleSheet(QString("font-size:12px;font-family:%1;color:#ACACAC;").arg(QStringLiteral("微软雅黑")));
	lbl_tip_->setText(QStringLiteral("提示：总音量请通过Windows系统音量进行调节"));
	lbl_tip_->move(34, 319);
	lbl_tip_->setAlignment(Qt::AlignCenter);

	btn_confirm = new QPushButton(this);
	btn_confirm->resize(78, 30);
	btn_confirm->setStyleSheet(QString("QPushButton{background-color:#2EB4AF;font-size:16px;font-family:%1;color:#151515;border:none;}QPushButton:pressed{margin:1px;}").arg(QStringLiteral("微软雅黑")));
	btn_confirm->setText(QStringLiteral("确定"));
	btn_confirm->move(201, 378);

	cbox_auto_exec = new QCheckBox(this);
	cbox_auto_exec->setText(QStringLiteral("开机启动"));
	cbox_auto_exec->setStyleSheet(QString("QCheckBox{spacing:10px;font-size:14px;font-family:%1;color:#2EB4AF;}QCheckBox::indicator{width:12px;height:12px;border:1px solid #008272;}QCheckBox::indicator:checked{image:url(:/resource/btn_check2x.png);}").arg(QStringLiteral("微软雅黑")));
	cbox_auto_exec->move(300, 386);
	cbox_auto_exec->setChecked(CallServerIni::getAutoExecFlag());

	QWidget *wgt_call = new QWidget(this);
	wgt_call->resize(254, 44);
	wgt_call->move(143, 191);
	wgt_call->setStyleSheet("background-image:url(:/resource/slider2x.png);");

	slider_call = new QSlider(this);
	slider_call->setOrientation(Qt::Horizontal);
	slider_call->setMinimum(0);  // 最小值
	slider_call->setMaximum(100);  // 最大值
	slider_call->setSingleStep(1);
	slider_call->resize(200, 34);
	slider_call->setStyleSheet("QSlider::groove:horizontal{border:1px solid #000000;height:34px;border-left:none;border-right:none;margin:-9px 0px -9px 0px}QSlider::handle:horizontal{width:35px;height:34px;border-image:url(:/resource/SliderButton2x.png);margin:0px -4px 0px -4px;}QSlider::sub-page:horizontal{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0.6 #00eaab, stop:0.35 #3fffc7);}");
	slider_call->move(169, 196);
	slider_call->setValue(CallServerIni::getCallVoiceValue());

	QWidget *wgt_play = new QWidget(this);
	wgt_play->resize(254, 44);
	wgt_play->move(143, 251);
	wgt_play->setStyleSheet("background-image:url(:/resource/slider2x.png);");

	slider_play = new QSlider(this);
	slider_play->setOrientation(Qt::Horizontal);
	slider_play->setMinimum(0);  // 最小值
	slider_play->setMaximum(100);  // 最大值
	slider_play->setSingleStep(1);
	slider_play->resize(200, 34);
	slider_play->setStyleSheet("QSlider::groove:horizontal{border:1px solid #000000;height:34px;border-left:none;border-right:none;margin:-9px 0px -9px 0px}QSlider::handle:horizontal{width:35px;height:34px;border-image:url(:/resource/SliderButton2x.png);margin:0px -4px 0px -4px;}QSlider::sub-page:horizontal{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0.6 #00eaab, stop:0.35 #3fffc7);}");
	slider_play->move(169, 257);
	slider_play->setValue(CallServerIni::getPlayVoiceValue());

	lbl_call_num = new QLabel(this);
	lbl_call_num->setStyleSheet(QString("font-size:14px;color:#ffffff;font-family:%1").arg(QStringLiteral("微软雅黑")));
	lbl_call_num->resize(25, 12);
	lbl_call_num->setNum(CallServerIni::getCallVoiceValue());
	lbl_call_num->move(411, 202);

	lbl_play_num = new QLabel(this);
	lbl_play_num->setStyleSheet(QString("font-size:14px;color:#ffffff;font-family:%1").arg(QStringLiteral("微软雅黑")));
	lbl_play_num->resize(25, 12);
	lbl_play_num->setNum(CallServerIni::getPlayVoiceValue());
	lbl_play_num->move(411, 263);
}

void MainWindow::initConnect()
{
	connect(btn_min, SIGNAL(clicked()), this, SLOT(showMinimized()));
	connect(btn_close, SIGNAL(clicked()), this, SLOT(close()));
	connect(btn_confirm, SIGNAL(clicked()), this, SLOT(slotOnShowSystemTray()));
	connect(slider_call, SIGNAL(valueChanged(int)), this, SLOT(slotOnCallVoiceChanged(int)));
	connect(slider_play, SIGNAL(valueChanged(int)), this, SLOT(slotOnPlayVoiceChanged(int)));
}

void MainWindow::RunCommSocketThreadFunc()
{
	//AppConfigHandler.Info("init comm server socket!");
	Log::Info("init comm server socket!");
	QString serverIp = CallServerIni::getServerIP();
	QString port = CallServerIni::getServerPort();
	//AppConfigHandler.Info(QString("GetServer ip:%1.%2").arg(serverIp).arg(port).toStdString());
	Log::Info("GetServer ip: %s,%s", serverIp.toLocal8Bit().data(), port.toLocal8Bit().data());

	//通信socket
	if (tc_comm_ptr_ != NULL)
	{
		tc_comm_ptr_->close();
	}
	tc_comm_ptr_.reset(new tcpclient());
	connect(tc_comm_ptr_.get(), SIGNAL(sig0nSocketConnectStatus(int)), this, SLOT(slot0nSocketConnectStatus(int)));
	tc_comm_ptr_->setconnectioninfo(serverIp.toStdString(), port.toInt());
	tc_comm_ptr_->run();
}

void MainWindow::registerToServer()
{
	boost::property_tree::ptree root;
	root.put<short>("CmdType", CMD_API_CLIENT_HOST_REGISTER);
	root.put<short>("ErrorCode", RESULT_SUCCESS);
	root.put<int>("ServerType", 1);

	std::string json_parser;
	handle_json_header(root, "CMD_API_CLIENT_HOST_REGISTER", json_parser);

	response res(json_parser.length());
	memcpy(res.netbuf_.buffer, json_parser.c_str(), json_parser.length());

	dmconnection* conn = SocketManagerIns.GetCommSocket();
	if (conn != NULL)
	{
		conn->do_write(res);
	}
}

void MainWindow::initSystemtray()
{
	this->hide();
	if (m_trayIcon == nullptr)
	{
		m_trayIcon = new QSystemTrayIcon(this);
		m_trayIcon->setIcon(QIcon(":/resource/icon-128.ico"));
		m_trayIcon->setToolTip(QString::fromLocal8Bit("Call-Server"));

		trayIconMenu = new QMenu();

		showAction = new QAction(this);
		showAction->setText(QStringLiteral("显示"));

		quitAction = new QAction(this);
		quitAction->setText(QStringLiteral("退出"));

		trayIconMenu->addAction(showAction);
		trayIconMenu->addAction(quitAction);
		m_trayIcon->setContextMenu(trayIconMenu);


		connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
			SLOT(activated(QSystemTrayIcon::ActivationReason)));

		connect(showAction, SIGNAL(triggered()), this, SLOT(slotOnShowNormal()));
		connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
	}
	m_trayIcon->show();
	m_trayIcon->showMessage(QString::fromLocal8Bit("Call Server"), QString::fromLocal8Bit("This is a Call-Server!"));
}

void MainWindow::commitCallVoiceChanged()
{
	boost::property_tree::ptree root;
	root.put<short>("CmdType", CMD_API_CLIENT_CONTROL_CALLVOICE_VALUE);
	root.put<short>("ErrorCode", RESULT_SUCCESS);
	root.put<int>("ServerType", 1);

	api_syncdata_info_t api_reg;
	api_reg.nCallVioceValue = CallServerIni::getCallVoiceValue();
	api_reg.write_json(root);

	std::string json_parser;
	handle_json_header(root, "CMD_API_CLIENT_CONTROL_CALLVOICE_VALUE", json_parser);

	response res(json_parser.length());
	memcpy(res.netbuf_.buffer, json_parser.c_str(), json_parser.length());

	dmconnection* conn = SocketManagerIns.GetCommSocket();
	if (conn != NULL)
	{
		conn->do_write(res);
	}
}

void MainWindow::handle_json_header(boost::property_tree::ptree header, std::string headTitle, std::string &json_str)
{
	std::stringstream tmp;
	write_json(tmp, header);
	std::string data = tmp.str();

	std::stringstream title;
	title << headTitle << " " << "ServerMessage/1.0" << HTTP_CRLF;

	int sessionId = -1;
	std::stringstream ss;
	ss << "Host: " << "localhost" << HTTP_CRLF
		<< "User-Agent: " << "server agent" << HTTP_CRLF
		<< "Connection: Keep-Alive" << HTTP_CRLF
		<< "Content-Length: " << std::dec << data.length() << HTTP_CRLF
		<< "Session-Id: " << std::dec << sessionId << HTTP_CRLF
		<< "Content-Type: application/json" << HTTP_CRLFCRLF;

	title << "Header-Length: " << std::dec << ss.str().length() << HTTP_CRLF;
	title << ss.str() << data;
	//AppConfigHandler.Info(QString("server send %1 bytes json string").arg(title.str().length()).toStdString());

	json_str = title.str();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	QImage image;
	image.load(":/resource/bg2x.png");
	QPainter draw(this);
	QRect rect(0,0,this->width(),this->height());
	draw.drawImage(rect,image);    
}

void MainWindow::showEvent(QShowEvent *event)
{
	this->setAttribute(Qt::WA_Mapped);
	QWidget::showEvent(event);
}

void MainWindow::slot0nSocketConnectStatus(int status)
{
	SOCKETSTATE_ENUM socket_status = (SOCKETSTATE_ENUM)status;
	switch (socket_status)
	{
	case SOCKETSTATE_SUCCESS:
	{
								if (tc_comm_ptr_ != NULL)
								{
									SocketManagerIns.SetCommSocket(tc_comm_ptr_->get_connection());
								}
								//注册
								registerToServer();
								SocketManagerIns.setIsConnected(true);
								lbl_tip->setText(QStringLiteral("连接服务器成功，IP：%1").arg(CallServerIni::getServerIP()));
								break;
	}
	case SOCKETSTATE_DISCONNECT:
	case SOCKETSTATE_FAILED:
	{
							   SocketManagerIns.setIsConnected(false);
							   SocketManagerIns.SetCommSocket(nullptr);
							   lbl_tip->setText(QStringLiteral("服务器未连接"));
							   break;
	}
	default:
		break;
	}
}

void MainWindow::slotOnShowSystemTray()
{
	initSystemtray();

	CallServerIni::setAutoExecFlag(cbox_auto_exec->isChecked());
	handleEvent::instance()->appAutoRun(cbox_auto_exec->isChecked());

	CallServerIni::setCallVoiceValue(slider_call->value());
	CallServerIni::setPlayVoiceValue(slider_play->value());

	commitCallVoiceChanged();
}

void MainWindow::activated(QSystemTrayIcon::ActivationReason reason)
{
	if (reason != QSystemTrayIcon::DoubleClick)
		return;
	slotOnShowNormal();
}

void MainWindow::slotOnShowNormal()
{
	m_trayIcon->hide();
	this->show();
}

void MainWindow::slotOnCallVoiceChanged(int value)
{
	lbl_call_num->setNum(value);
}

void MainWindow::slotOnPlayVoiceChanged(int value)
{
	lbl_play_num->setNum(value);
	//EventHandler.MCISetVolume(value * 10);
}
