#include <stdexcept>
#include <exception>
#include <stdio.h>
/*#include <windows.h>*/

#include "transform_handler.h"
#include "connection.hpp"
#include "logging.hpp"
#include "setip.h"
#include "commonFun.h"
#include "constData.h"
#include "HttpHandler.h"

namespace dem1 {
	namespace server {

/// 单例声明
/// 类内 class static 除了在class 之内声明，还需要在class 之外定义.
auto_ptr<transform_handler> transform_handler::auto_ptr_instance;

transform_handler::transform_handler()
{

}


transform_handler::~transform_handler()
{

}

void transform_handler::HttpHandleOffLineStatus(string pcName, string barId, string mac)
{
	char buf[256];
	GetModuleFileName(NULL, buf, MAX_PATH);
	string path;
	path.assign(buf);
	path.append("\\..\\systemExe.exe");
	path.append(" " + pcName);
	path.append(" " + barId);
	path.append(" " + mac);

	int res = system(path.c_str());
	return;
	string url = commonFun::getRequestUrl() + c_sHttpGetOrderOnlineStatusAction;
	string param = "pcName=" + pcName + "&barId=" + barId + "&pcMac=" + mac + "&status=1";
	url += param;
	AppConfigHandler.Info(url);

	string result = post(url);
	AppConfigHandler.Info("result : " + result);
}

transform_handler& transform_handler::GetInstance(void)
{
	if (auto_ptr_instance.get() == 0) {
		auto_ptr_instance.reset(new transform_handler());
	}
	return *(auto_ptr_instance.get());
}

bool transform_handler::rebootClientHandler(const api_client_info_t &api_info)
{
	char err[200];
	memset(err, 0, sizeof(err));

	std::string user = (char*)api_info.nUser;
	std::string passwd = (char*)api_info.nPassWd;
	std::string ipAdress = (char*)api_info.nIPAdress;

	std::string cmd = "net use \\\\" + ipAdress + " \"" + passwd + "\" " + "/user:" + user;
	//Log::Info("cmd exec : %s", cmd.c_str());
	AppConfigHandler.Info("cmd exec : " + cmd);
	if (!system(cmd.c_str()))
	{
		cmd = "shutdown /r /t 0 /m \\\\" + ipAdress;
		//Log::Info("cmd exec : %s", cmd.c_str());
		AppConfigHandler.Info("reboot cmd exec : " + cmd);
		system(cmd.c_str());
		return true;
	}
	else
	{
		perror(err);
		//Log::Info("reboot cmd exc failed : %s", err);
		AppConfigHandler.Info("reboot cmd exc failed : " + string(err));
		return false;
	}
}

bool transform_handler::shutdownClientHandler(const api_client_info_t &api_info)
{
	char err[200];
	memset(err, 0, sizeof(err));

	std::string user = (char*)api_info.nUser;
	std::string passwd = (char*)api_info.nPassWd;
	std::string ipAdress = (char*)api_info.nIPAdress;

	std::string cmd = "net use \\\\" + ipAdress + " \"" + passwd + "\" " + "/user:" + user;
	//Log::Info("cmd exec : %s", cmd.c_str());
	AppConfigHandler.Info("cmd exec : " + cmd);
	if (!system(cmd.c_str()))
	{
		cmd = "shutdown /s /t 0 /m \\\\" + ipAdress;
		//Log::Info("cmd exec : %s", cmd.c_str());
		AppConfigHandler.Info("shutdown cmd exec : " + cmd);
		system(cmd.c_str());
		return true;
	}
	else
	{
		perror(err);
		//Log::Info("reboot cmd exc failed : %s", err);
		AppConfigHandler.Info("shutdown cmd exc failed : " + string(err));
		return false;
	}
}

bool transform_handler::remoteWakeUpClientHandler(const api_client_info_t &api_info)
{
	std::string mac = (char*)api_info.nMacAdress;
	std::string ipAdress = (char*)api_info.nIPAdress;
	//Log::Info("mac adress : %s", mac.c_str());
	AppConfigHandler.Info("mac adress : "  + mac);

	int dstaddr[6];
	int i = sscanf(mac.c_str(), "%2x-%2x-%2x-%2x-%2x-%2x", &dstaddr[0], &dstaddr[1], &dstaddr[2], &dstaddr[3], &dstaddr[4], &dstaddr[5]);
	if (i != 6) {
		//Log::Info("Invalid MAC Adresse!");
		AppConfigHandler.Info("Invalid MAC Adresse!");
		return false;
	}

	unsigned char ether_addr[6];
	for (i = 0; i < 6; ++i)
		ether_addr[i] = dstaddr[i];

	//构造Magic Packet
	u_char magicpacket[200];
	memset(magicpacket, 0xff, 6);
	int packetsize = 6;
	for (i = 0; i < 16; ++i)
	{
		memcpy(magicpacket + packetsize, ether_addr, 6);
		packetsize += 6;
	}

	//启动WSA 
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
		//Log::Info("WSAStartup failed: %d\n", GetLastError());
		AppConfigHandler.Info("WSAStartup failed: " + to_string(GetLastError()));
		return false;
	}
	//创建socket 
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) {
		//Log::Info("Socket create error: %d\n", GetLastError());
		AppConfigHandler.Info("Socket create error: " + to_string(GetLastError()));
		return false;
	}
	//设置为广播发送
	 	BOOL bOptVal = TRUE;
	 	int iOptLen = sizeof(BOOL);
	 	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&bOptVal, iOptLen) == SOCKET_ERROR) {
	 		//Log::Info("setsockopt error: %d\n", WSAGetLastError());
			AppConfigHandler.Info("setsockopt error: " + to_string(WSAGetLastError()));
	 		closesocket(sock);
	 		WSACleanup();
	 	}

	sockaddr_in to;
	to.sin_family = AF_INET;
	to.sin_port = htons(7);
	//to.sin_addr.S_un.S_addr = inet_addr(ipAdress.c_str());
	to.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	//发送Magic Packet 
	if (sendto(sock, (const char *)magicpacket, packetsize, 0, (const struct sockaddr *)&to, sizeof(to)) == SOCKET_ERROR) {
		//Log::Info("Magic packet send error: %d", WSAGetLastError());
		AppConfigHandler.Info("Magic packet send error: " + to_string(WSAGetLastError()));
		return false;
	}
	else{
		//Log::Info("Magic packet send!");
		AppConfigHandler.Info("Magic packet send!");
	}
	closesocket(sock);
	WSACleanup();
	return true;
}

void transform_handler::updateHostSession(std::string sessionId)
{
	boost::lock_guard<boost::mutex> lock(mutex_ctrl);
	host_sessionId = sessionId;
}

std::string transform_handler::getHostSession()
{
	boost::lock_guard<boost::mutex> lock(mutex_ctrl);
	return host_sessionId;
}

void transform_handler::syncData(const api_syncdata_info_t &api_reg)
{
	boost::lock_guard<boost::mutex> lock(mutex_call);
	callVoice_value = api_reg.nCallVioceValue;
}

short transform_handler::getSyncData()
{
	boost::lock_guard<boost::mutex> lock(mutex_call);
	return callVoice_value;
}

void transform_handler::addMapList(api_masterLocker_register_info api_info, std::string sessionId)
{
	boost::lock_guard<boost::mutex> lock(mutex_map);
	mapList_[sessionId] = api_info;
}

void transform_handler::removeSessionID(std::string sessionId)
{
	boost::lock_guard<boost::mutex> lock(mutex_map);
	std::map<std::string, api_masterLocker_register_info>::iterator it = mapList_.begin();
	while (it != mapList_.end())
	{
		if (it->first == sessionId)
		{
			string pcName = (char*)(it->second.nName);
			string barId = ConfigFileRead("barId");
			string mac = (char*)(it->second.nMac);
			HttpHandleOffLineStatus(pcName, barId, mac);
		
			it = mapList_.erase(it);
		}
		else{
			it++;
		}
	}
}

std::string transform_handler::getSessionIdByIP(std::string ip)
{
	boost::lock_guard<boost::mutex> lock(mutex_map);
	std::map<std::string, api_masterLocker_register_info>::iterator it = mapList_.begin();
	while (it != mapList_.end())
	{
		std::string ip_ = (char*)(it->second.nIP);
		if (ip_ == ip)
		{
			return it->first;
		}
		it++;
	}
	return "";
}

	}  // namespace server
} // namespace dem1