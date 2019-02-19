#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMutex>
#include <QPixmap>
#include <QProcess>
#include <QSystemTrayIcon>

#include "tcpclient.hpp"

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "KillExe.h"

class QPushButton;
class QCheckBox;
class QLabel;
class QSlider;
class QMenu;
class QAction;

using namespace dm::server;
using namespace dm::client;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
   
	void showAutoRunApp();
private:
	//初始化TCP
	void initTcpSocket();
	//初始化界面
	void initUI();
	//初始化交互连接
	void initConnect();
	//异步TcpReader
	void RunCommSocketThreadFunc();
	//注册
	void registerToServer();
	//初始化托盘
	void initSystemtray();
	//提交呼叫音量变化
	void commitCallVoiceChanged();

	void handle_json_header(boost::property_tree::ptree header, std::string headTitle, std::string &json_str);
protected:
	void paintEvent(QPaintEvent *event);
	void showEvent(QShowEvent *event);
private slots:
	//SocketMonitorManager出发信号槽，用以通知服务器状态
	void slot0nSocketConnectStatus(int status);
	void slotOnShowSystemTray();
	void activated(QSystemTrayIcon::ActivationReason reason);
	void slotOnShowNormal();
	void slotOnCallVoiceChanged(int value);
	void slotOnPlayVoiceChanged(int value);
private:
	QSystemTrayIcon *m_trayIcon;
	KillExe kill_exe_;
	boost::shared_ptr<boost::thread> comm_socket_thread_;
	//通信socket
	boost::shared_ptr<dm::client::tcpclient> tc_comm_ptr_;

	QPushButton *btn_min;
	QPushButton *btn_close;
	QPushButton *btn_confirm;
	QCheckBox *cbox_auto_exec;
	QSlider *slider_call;
	QSlider *slider_play;
	QLabel *lbl_call_num;
	QLabel *lbl_play_num;
	QLabel *lbl_tip;
	QMenu *trayIconMenu;
	QAction *quitAction;
	QAction *showAction;
};

#endif // MAINWINDOW_H
