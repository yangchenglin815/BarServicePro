#pragma once

#pragma once
#include <memory>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <list>

#include "APIProtocol.h"
#include <QList>

using namespace std;

// 使用宏替代单例实例
#define EventHandler Event_Handler::GetInstance()

namespace dm {
	namespace server {
		class connection;
		class Event_Handler
		{
		private:
			friend class auto_ptr<Event_Handler>;				/// 使用智能指针
			/// 即使从未被调用也会被构造(析构);如果对象使用static 修饰，意思是只有一个对象会被产生出来。
			/// 构造函数属性为 private ，可以防止对象的产生。(auto_ptr need #include <memory>)
			static auto_ptr<Event_Handler> auto_ptr_instance;	/// 惟一实例

		private:
			Event_Handler();	/// 构造函数(注意:构造方法私有)
			~Event_Handler(void);							/// 析构函数
		public:
			static Event_Handler& GetInstance(void);		/// 实例(工厂方法)

			//播放音频文件
			bool playBoardcastMsg(const api_client_info_t &api_info, std::string name);
			//获取播放总时长
			DWORD MCIStatus_music_length();
			//获取当前播放时间
			DWORD MCIStatus_music_pos();
			//关闭播放
			void close_mci();
			//播放
			DWORD MCIPlay();

			//列表操作
			void addBoardCastListItem(const api_boardcast_info_t &api_info);
			void removeBoardCastListItem(const api_boardcast_info_t &api_info);
			//
			void boardcastCycleThreadFunc();

			void waitforPlayFinished(int sec = 500);
		private:
			UINT wDeviceID_;
			bool is_thread_exit_;
			QList<api_boardcast_info_t> boardCast_list_;
			QList<api_boardcast_info_t> boardCast_list_copy_;
			boost::mutex ctrl_mutex_;
			boost::shared_ptr<boost::thread> list_cycle_thread_;
		};
	}  // namespace server
} // namespace dm


