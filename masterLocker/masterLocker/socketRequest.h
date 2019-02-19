#ifndef SOCKETREQUEST_H
#define SOCKETREQUEST_H

#include <QObject>
#include "APIProtocol.h"
#include "dmconnection.hpp"
#include "SocketManager.h"
#include "appConfigIni.h"
#include "tcpclient.hpp"

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

using namespace dm::server;
using namespace dm::client;

class socketRequest : public QObject
{
	Q_OBJECT

public:
	static socketRequest* instance();

	void initTcpSocket();
	void requestOnRegisterToServer();
	void requestOnHeartBeat();
private:
	explicit socketRequest(QObject *parent = 0);
	~socketRequest();

	//异步TcpReader
	void RunCommSocketThreadFunc();
	void handle_json_header(boost::property_tree::ptree header, std::string headTitle, std::string &json_str);
private slots:
	//SocketMonitorManager出发信号槽，用以通知服务器状态
	void slot0nSocketConnectStatus(int status);
private:
	static socketRequest* m_pInstance;
	//通信socket
	boost::shared_ptr<dm::client::tcpclient> tc_comm_ptr_;
	boost::shared_ptr<boost::thread> comm_socket_thread_;
	QString sIp;
	QString sPort;
};

#endif // SOCKETREQUEST_H
