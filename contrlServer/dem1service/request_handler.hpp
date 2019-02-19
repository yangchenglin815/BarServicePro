#ifndef M1_REQUEST_HANDLER_HPP
#define M1_REQUEST_HANDLER_HPP

#include <string>
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <iostream>

#include "APIProtocol.h"
#include "response.hpp"

#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace dem1 {
	namespace server {

		class connection;
		class request_handler {
		public:
			request_handler();

            int start(std::shared_ptr<connection>  dm_conn);
			void stop();

			//重启被控终端设备
			int handle_request_client_reboot(protocol_head_t *head, netbuffer *buf);
			int handle_request_client_reboot(boost::property_tree::ptree parser);
			//远程唤醒终端设备
			int handle_request_client_remote_wakeup(protocol_head_t *head, netbuffer *buf);
			int handle_request_client_remote_wakeup(boost::property_tree::ptree parser);
			//控制播报语音
			int handle_request_boardcast_msg(protocol_head_t *head, netbuffer *buf);
			int handle_request_boardcast_msg(boost::property_tree::ptree parser);
			//保存客户注册信息
			int handle_request_host_register(protocol_head_t *head, netbuffer *buf);
			int handle_request_host_register(boost::property_tree::ptree parser);
			//处理关机
			int handle_request_client_shutdown(boost::property_tree::ptree parser);
			//同步呼叫服务数据
			int handle_request_sync_data(boost::property_tree::ptree parser);
			//处理客户端请求获取呼叫音量大小
			int handle_request_call_voice_value(boost::property_tree::ptree parser);
			//处理呼叫服务控制改变呼叫音量大小
			int handle_request_control_call_voice(boost::property_tree::ptree parser);
			//取消语音呼叫
			int handle_request_cancel_boardcast(boost::property_tree::ptree parser);
			//锁屏程序注册
			int handle_request_masterLocker_register(boost::property_tree::ptree parser);

			void handle_json_header(boost::property_tree::ptree header, std::string headTitle, std::string &json_str);

		private:
			std::shared_ptr<connection> curr_conn_;//当前ＳＯＣＫＥＴ连接
		};

	}  // namespace server
}  // namespace dem1

#endif  // M1_REQUEST_HANDLER_HPP
