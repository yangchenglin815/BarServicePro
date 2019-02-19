#pragma once
#include <memory>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <list>
#include <map>

#include "APIProtocol.h"

using namespace std;

// 使用宏替代单例实例
#define TransformHandler transform_handler::GetInstance()

namespace dem1 {
	namespace server {
		class connection;
class transform_handler
{
private:
	friend class auto_ptr<transform_handler>;				/// 使用智能指针
	/// 即使从未被调用也会被构造(析构);如果对象使用static 修饰，意思是只有一个对象会被产生出来。
	/// 构造函数属性为 private ，可以防止对象的产生。(auto_ptr need #include <memory>)
	static auto_ptr<transform_handler> auto_ptr_instance;	/// 惟一实例

private:
	transform_handler();	/// 构造函数(注意:构造方法私有)
	~transform_handler(void);							/// 析构函数

public:
	static transform_handler& GetInstance(void);		/// 实例(工厂方法)

	bool rebootClientHandler(const api_client_info_t &api_info);   //处理重启终端设备
	bool shutdownClientHandler(const api_client_info_t &api_info);  //处理关机请求
	bool remoteWakeUpClientHandler(const api_client_info_t &api_info);   //处理远程唤醒终端设备

	void updateHostSession(std::string sessionId);
	std::string getHostSession();

	void syncData(const api_syncdata_info_t &api_reg);
	short getSyncData();

	//容器操作
	void addMapList(api_masterLocker_register_info api_info, std::string sessionId);
	void removeSessionID(std::string sessionId);
	std::string getSessionIdByIP(std::string ip);
	void HttpHandleOffLineStatus(string pcName, string barId, string mac);
private:
	std::string host_sessionId;
	boost::mutex mutex_ctrl;

	short callVoice_value;
	boost::mutex mutex_call;

	boost::mutex mutex_map;
	std::map<std::string, api_masterLocker_register_info> mapList_;
};
	}  // namespace server
} // namespace dem1
