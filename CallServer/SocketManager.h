#pragma once
#include <iostream>
#include <string>
#include <map>
#include <QObject>
#include <memory>


#include "APIProtocol.h"
#include "UiSingleton.h"

#include "dmconnection.hpp"

using namespace dm::server;

#define  SocketManagerIns SocketManager::Instance()

class HostWidget;

class SocketManager :public QObject, public CUiSingleton<SocketManager>
{
	Q_OBJECT
		friend class CUiSingleton<SocketManager>;
private:
	SocketManager();

public:

	void SetCommSocket(dmconnection* conn);		//…Ë÷√Õ®–≈socket
	dmconnection* GetCommSocket(){ return comm_conn_; };

	void SetMainWindow(HostWidget* main_window);

	void setIsConnected(bool is_onnected);
	bool isConnected(){ return is_connected_; };
signals:


private:

	HostWidget *mainwindow_;

	dmconnection *comm_conn_;

	bool is_connected_ = false;
};
