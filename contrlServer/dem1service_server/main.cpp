#include <iostream>

#include "tcpserver.hpp"
#include "APIProtocol.h"
#include "setip.h"
#include "dump.h"

#include "appconfig.h"
#include "logging.hpp"
#include "KillExe.h"

#include <windows.h>
#include <QCoreApplication>
#include <stdio.h>
#include <QDir>
#include "HttpHandler.h"
#include "transform_handler.h"

using namespace std;

KillExe kill_exe_;

//Òþ²ØDOSºÚ´°¿Ú
//#pragma comment(linker,"/subsystem:\"windows\"  /entry:\"mainCRTStartup\"" ) 

void server_tcpserver()
{
	kill_exe_.KillProcess(_T("dem1serivce_server.exe"));
	Sleep(500);

	QDir dir;
	QString initFilePath = QDir::cleanPath(QDir::homePath() + "/AppData/Local/Contrl-Server/");
	if (!dir.exists(initFilePath + "/log"))
	{
		if (!dir.mkpath(initFilePath + "/log"))
		{
			return;
		}
	}

// 	char  buf[256];
// 	GetModuleFileName(NULL, (TCHAR*)buf, MAX_PATH);
// 	printf("filePath:%s\n", buf);
// 	string path;
// 	path.assign(buf);

	AppConfigHandler.InitialiseLog(initFilePath.toStdString() + "/log/daily.txt");
// 	std::string result = post("https://uu.zimonet.com/qynetbarsystem/api/barData/getBarData?barId=3");
// 	AppConfigHandler.Info("result:" + result);

// 	path.append("\\..\\log.txt");
// 	Log::Initialise(path);
	// 	Log::SetThreshold(Log::LOG_TYPE_DEBUG);	
	//string url = commonFun::getRequestUrl() + c_sHttpGetOrderOnlineStatusAction;
	//TransformHandler.HttpHandleOffLineStatus("ycl","6","123456");


	std::unique_ptr<dem1::server::tcpserver> s;

	string server_ip = ConfigFileRead("server");
	string port = ConfigFileRead("port");
	AppConfigHandler.Info("Server_IP: " + server_ip +" Port: " + port);

	s.reset(new dem1::server::tcpserver(server_ip, port));
	s->run();

	/*Log::Finalise();*/
	AppConfigHandler.DropSpdLog();
}
void main(){

	EnableAutoDump();
	server_tcpserver();
}