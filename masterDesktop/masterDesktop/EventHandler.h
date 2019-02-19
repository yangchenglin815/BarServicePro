#pragma once

#include <memory>
#include <list>
#include <map>

using namespace std;

namespace dm {
	namespace server {
class EventHandler
{
private:
	friend class auto_ptr<EventHandler>;				/// 使用智能指针
	/// 即使从未被调用也会被构造(析构);如果对象使用static 修饰，意思是只有一个对象会被产生出来。
	/// 构造函数属性为 private ，可以防止对象的产生。(auto_ptr need #include <memory>)
	static auto_ptr<EventHandler> auto_ptr_instance;	/// 惟一实例
private:
	EventHandler();
	~EventHandler(void);
public:
	static EventHandler& GetInstance(void);		/// 实例(工厂方法)


	void rebootClientHandler();   //处理重启终端设备
	void shutdownClientHandler();  //处理关机请求
};
	}  // namespace server
} // namespace dm
