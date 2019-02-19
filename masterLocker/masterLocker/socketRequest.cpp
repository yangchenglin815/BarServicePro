#include "socketRequest.h"

using namespace dm::server;
using namespace dm::client;

socketRequest* socketRequest::m_pInstance = NULL;

socketRequest* socketRequest::instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new socketRequest;
	}
	return m_pInstance;
}

void socketRequest::initTcpSocket()
{
	if (comm_socket_thread_ && comm_socket_thread_->joinable())
	{
		comm_socket_thread_->join();
	}
	comm_socket_thread_.reset(new boost::thread(&socketRequest::RunCommSocketThreadFunc, this));
}

void socketRequest::requestOnRegisterToServer()
{
	api_masterLocker_register_info api_info;
	QString ip = appConfigIni::getHostIpAddress();
	QString mac = appConfigIni::getHostMacAddress().replace(":", "-");
	QString winName = appConfigIni::getLocalmachineName();
	memcpy(api_info.nIP, ip.toStdString().c_str(), API_ARRAY_LEN);
	memcpy(api_info.nMac, mac.toStdString().c_str(), API_ARRAY_LEN);
	memcpy(api_info.nName, winName.toStdString().c_str(), API_ARRAY_LEN);

	boost::property_tree::ptree root;
	root.put<short>("CmdType", CMD_API_MASTERLOCKER_REGISTER);
	root.put<short>("ErrorCode", RESULT_SUCCESS);
	root.put<int>("ServerType", 1);
	api_info.write_json(root);

	std::string json_parser;
	handle_json_header(root, "CMD_API_MASTERLOCKER_REGISTER", json_parser);

	response res(json_parser.length());
	memcpy(res.netbuf_.buffer, json_parser.c_str(), json_parser.length());

	dmconnection* conn = SocketManagerIns.GetCommSocket();
	if (conn != NULL)
	{
		conn->do_write(res);
	}
}

void socketRequest::requestOnHeartBeat()
{
	boost::property_tree::ptree root;
	root.put<short>("CmdType", CMD_API_CLIENT_HOST_HEART);
	root.put<short>("ErrorCode", RESULT_SUCCESS);
	root.put<int>("ServerType", 1);

	std::string json_parser;
	handle_json_header(root, "CMD_API_CLIENT_HOST_HEART", json_parser);

	response res(json_parser.length());
	memcpy(res.netbuf_.buffer, json_parser.c_str(), json_parser.length());

	dmconnection* conn = SocketManagerIns.GetCommSocket();
	if (conn != NULL)
	{
		conn->do_write(res);
	}
}

socketRequest::socketRequest(QObject *parent)
: QObject(parent),
tc_comm_ptr_(NULL)
{
	Log::Info("init comm server socket!");
	sIp = appConfigIni::getServerIP();
	sPort = appConfigIni::getServerPort();
	Log::Info("GetServer ip : %s:%s", sIp.toStdString().c_str(), sPort.toStdString().c_str());
}

socketRequest::~socketRequest()
{
	if (tc_comm_ptr_ != NULL){
		tc_comm_ptr_->close();
		tc_comm_ptr_ = NULL;
	}
}

void socketRequest::RunCommSocketThreadFunc()
{
	//通信socket
	if (tc_comm_ptr_ != NULL)
	{
		tc_comm_ptr_->close();
	}
	tc_comm_ptr_.reset(new tcpclient());
	connect(tc_comm_ptr_.get(), SIGNAL(sig0nSocketConnectStatus(int)), this, SLOT(slot0nSocketConnectStatus(int)));
	tc_comm_ptr_->setconnectioninfo(sIp.toStdString(), sPort.toInt());
	tc_comm_ptr_->run();
}

void socketRequest::handle_json_header(boost::property_tree::ptree header, std::string headTitle, std::string &json_str)
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
	Log::Info("server send %d bytes json string", title.str().length());

	json_str = title.str();
}

void socketRequest::slot0nSocketConnectStatus(int status)
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
								requestOnRegisterToServer();
								SocketManagerIns.setIsConnected(true);
								Log::Info("connect to server success , IP : %1", appConfigIni::getServerIP().toStdString().c_str());
								break;
	}
	case SOCKETSTATE_DISCONNECT:
	case SOCKETSTATE_FAILED:
	{
							   SocketManagerIns.setIsConnected(false);
							   SocketManagerIns.SetCommSocket(nullptr);
							   Log::Info(QStringLiteral("服务已断开").toStdString().c_str());
							   break;
	}
	default:
		break;
	}
}
