#include "request_handler.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "dmconnection.hpp"
#include "EventHandler.h"

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
			std::string sessionId = curr_conn_->get_sessionid();
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
			Log::Info("server send %d bytes json string", title.str().length());

			json_str = title.str();
		}

		void request_handler::handle_request_client_reboot(boost::property_tree::ptree parser)
		{
			api_client_info_t api_reg;
			api_reg.read_json(parser);

			EventHandler::GetInstance().rebootClientHandler();
		}

		void request_handler::handle_request_client_shutdown(boost::property_tree::ptree parser)
		{
			api_client_info_t api_reg;
			api_reg.read_json(parser);

			EventHandler::GetInstance().shutdownClientHandler();
		}

	}  // namespace server
} // namespace dem1
