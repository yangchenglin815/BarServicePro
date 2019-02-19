#pragma once

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


/*#include "multi_rtsp.hpp"*/


namespace dm 
{
	namespace server 
	{
        class dmconnection;
		class request_handler 
		{
			public:
				request_handler();

				int start(dmconnection* dm_conn);
				void stop();
				void handle_request_boardcast_msg(protocol_head_t head, netbuffer *buf);
				void handle_request_boardcast_msg(boost::property_tree::ptree parser);
				void handle_request_cancel_boardcast_msg(boost::property_tree::ptree parser);
				void handle_request_sync_data();
				void handle_json_header(boost::property_tree::ptree header, std::string headTitle, std::string &json_str);
			private:
				dmconnection* curr_conn_;//当前ＳＯＣＫＥＴ连接
           
		};

	}  // namespace server
}  // namespace dem1


