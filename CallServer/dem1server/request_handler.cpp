#include "request_handler.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "dmconnection.hpp"
#include "handleEvent.h"
#include "CallServerIni.h"

namespace dm 
{
	namespace server 
	{
		request_handler::request_handler()
		{           
        }
        
        int request_handler::start(dmconnection* dm_conn)
		{            
            this->curr_conn_ = dm_conn;
            return 0;
        }
        
        
        void request_handler::stop(){
            
        }      

		void request_handler::handle_request_boardcast_msg(protocol_head_t head, netbuffer *buf)
		{
			api_client_info_t api_reg;
			api_reg.read(buf);

			handleEvent::instance()->playBoardcastMsg(api_reg, "");
		}

		void request_handler::handle_request_boardcast_msg(boost::property_tree::ptree parser)
		{
			api_boardcast_info_t api_reg;
			api_reg.read_json(parser);

			handleEvent::instance()->addBoardCastListItem(api_reg);
		}

		void request_handler::handle_request_cancel_boardcast_msg(boost::property_tree::ptree parser)
		{
			api_boardcast_info_t api_reg;
			api_reg.read_json(parser);

			handleEvent::instance()->removeBoardCastListItem(api_reg);
		}

		void request_handler::handle_request_sync_data()
		{
			boost::property_tree::ptree root;
			root.put<int>("CmdType", CMD_API_SYNC_DATA);
			root.put<int>("ErrorCode", 0);
			root.put<int>("ServerType", 1);

			api_syncdata_info_t api_reg;
			api_reg.nCallVioceValue = CallServerIni::getCallVoiceValue();
			api_reg.write_json(root);

			std::string json_parser;
			handle_json_header(root, "CMD_API_SYNC_DATA", json_parser);

			response res(json_parser.length());
			memcpy(res.netbuf_.buffer, json_parser.c_str(), json_parser.length());

			if (curr_conn_ != NULL)
			{
				curr_conn_->do_write(res);
			}
		}

		void request_handler::handle_json_header(boost::property_tree::ptree header, std::string headTitle, std::string &json_str)
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
			//Log::Info("server send %d bytes json string", title.str().length());
			//AppConfigHandler.Info("server send " + to_string(title.str().length()) + " bytes json string");

			json_str = title.str();
		}

	}  // namespace server
} // namespace dem1
