#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>    
#define BOOST_DATE_TIME_SOURCE    

#include "connection.hpp"
#include "connection_manager.hpp"
#include "request_handler.hpp"
#include "transform_handler.h"
#include "setip.h"

namespace dem1 {
	namespace server {


		request_handler::request_handler()
        {

		}

        int request_handler::start(std::shared_ptr<connection>  dm_conn){

			this->curr_conn_ = dm_conn;
		
			return 0;
		}

		void request_handler::stop(){
			//MySQLPool.ReleaseConnection(pconn_);//»ØÊÕ
			std::string sessionId = curr_conn_->get_sessionid();
			TransformHandler.removeSessionID(sessionId);
		}

		int request_handler::handle_request_client_reboot(protocol_head_t *head, netbuffer *buf)
		{
			api_client_info_t api_reg;
			api_reg.read(buf);

			protocol_head_t api_head;
			api_head.nCmd = CMD_API_CLIENT_REBOOT_RES;
			api_head.nDataLen = 0;
			api_head.nResult = TransformHandler.rebootClientHandler(api_reg) ? RESULT_SUCCESS : RESULT_FAILED;

			response res(api_head.get_size());
			response_add_resprotocol<protocol_head_t>(res, &api_head);
			if (curr_conn_ != NULL)
			{
				curr_conn_->do_write(res);
			}
			return 0;
		}



		int request_handler::handle_request_client_reboot(boost::property_tree::ptree parser)
		{
			api_client_info_t api_reg;
			api_reg.read_json(parser);

			std::string ip = (char*)api_reg.nIPAdress;
			std::string sessionId = TransformHandler.getSessionIdByIP(ip);

			//int ret = TransformHandler.rebootClientHandler(api_reg) ? RESULT_SUCCESS : RESULT_FAILED;
			int type = parser.get<int>("ServerType");

			boost::property_tree::ptree root;
			root.put<int>("CmdType", CMD_API_CLIENT_REBOOT_RES);
			root.put<int>("ErrorCode", RESULT_SUCCESS);
			root.put<int>("ServerType", type);
			api_reg.write_json(root);

			std::string json_parser;
			handle_json_header(root, "CMD_API_CLIENT_REBOOT_RES", json_parser);

			response res(json_parser.length());
			memcpy(res.netbuf_.buffer, json_parser.c_str(), json_parser.length());

			std::shared_ptr<connection> conn = curr_conn_->connection_manager_.get_connect(sessionId);
			if (conn != NULL)
			{
				conn->do_write(res);
			}
			return 0;
		}

		int request_handler::handle_request_client_remote_wakeup(protocol_head_t *head, netbuffer *buf)
		{
			api_client_info_t api_reg;
			api_reg.read(buf);

			protocol_head_t api_head;
			api_head.nCmd = CMD_API_CLIENT_REMOTE_WAKEUP_RES;
			api_head.nDataLen = 0;
			api_head.nResult = TransformHandler.remoteWakeUpClientHandler(api_reg) ? RESULT_SUCCESS : RESULT_FAILED;

			response res(api_head.get_size());
			response_add_resprotocol<protocol_head_t>(res, &api_head);
			if (curr_conn_ != NULL)
			{
				curr_conn_->do_write(res);
			}
			return 0;
		}

		int request_handler::handle_request_client_remote_wakeup(boost::property_tree::ptree parser)
		{
			api_client_info_t api_reg;
			api_reg.read_json(parser);

			int ret = TransformHandler.remoteWakeUpClientHandler(api_reg) ? RESULT_SUCCESS : RESULT_FAILED;
			int type = parser.get<int>("ServerType");

			boost::property_tree::ptree root;
			root.put<int>("CmdType", CMD_API_CLIENT_REMOTE_WAKEUP_RES);
			root.put<int>("ErrorCode", ret);
			root.put<int>("ServerType", type);

			std::string json_parser;
			handle_json_header(root, "CMD_API_CLIENT_REMOTE_WAKEUP_RES", json_parser);

			response res(json_parser.length());
			memcpy(res.netbuf_.buffer, json_parser.c_str(), json_parser.length());

			if (curr_conn_ != NULL)
			{
				curr_conn_->do_write(res);
			}
			return 0;
		}

		int request_handler::handle_request_boardcast_msg(protocol_head_t *head, netbuffer *buf)
		{
			api_client_info_t api_reg;
			api_reg.read(buf);

			protocol_head_t api_head;
			api_head.nCmd = CMD_API_CLIENT_BOARDCAST_MSG_RES;
			api_head.nDataLen = api_reg.get_size();
			api_head.nResult = RESULT_SUCCESS;

			response res(api_head.get_size() + api_reg.get_size());
			response_add_resprotocol<protocol_head_t>(res, &api_head);
			response_add_resprotocol<api_client_info_t>(res, &api_reg);

			std::string session_id = TransformHandler.getHostSession();
			std::shared_ptr<connection> conn = curr_conn_->connection_manager_.get_connect(session_id);
			if (conn != NULL){
				conn->do_write(res);
			}
			return 0;
		}

		int request_handler::handle_request_boardcast_msg(boost::property_tree::ptree parser)
		{
			api_boardcast_info_t api_reg;
			api_reg.read_json(parser);
			std::string seatNo = (char*)api_reg.nSeatNo;
			//Log::Info("boardcast msg : %s  %d %d times", api_reg.nSeatNo, api_reg.nBoardCastType, api_reg.nPlayCycleNum);
			AppConfigHandler.Info("boardcast msg : " + seatNo + " " + to_string(api_reg.nBoardCastType) + " " + to_string(api_reg.nPlayCycleNum) + " times");

			boost::property_tree::ptree root;
			root.put<short>("CmdType", CMD_API_CLIENT_BOARDCAST_MSG_RES);
			root.put<short>("ErrorCode", RESULT_SUCCESS);
			root.put<int>("ServerType", CSERVER_CLIENT);

			api_reg.write_json(root);
			std::string json_parser;
			handle_json_header(root, "CMD_API_CLIENT_BOARDCAST_MSG_RES", json_parser);

			response res(json_parser.length());
			memcpy(res.netbuf_.buffer, json_parser.c_str(), json_parser.length());

			std::string session_id = TransformHandler.getHostSession();
			std::shared_ptr<connection> conn = curr_conn_->connection_manager_.get_connect(session_id);
			if (conn != NULL){
				conn->do_write(res);
			}
			return 0;
		}

		int request_handler::handle_request_host_register(protocol_head_t *head, netbuffer *buf)
		{
			TransformHandler.updateHostSession(curr_conn_->get_sessionid());
			return 0;
		}

		int request_handler::handle_request_host_register(boost::property_tree::ptree parser)
		{
			TransformHandler.updateHostSession(curr_conn_->get_sessionid());

			boost::property_tree::ptree root;
			root.put<int>("CmdType", CMD_API_CLIENT_HOST_REGISTER_RES);
			root.put<int>("ErrorCode", RESULT_SUCCESS);
			root.put<int>("ServerType", 1);

			std::string json_parser;
			handle_json_header(root, "CMD_API_CLIENT_HOST_REGISTER_RES", json_parser);

			response res(json_parser.length());
			memcpy(res.netbuf_.buffer, json_parser.c_str(), json_parser.length());

			if (curr_conn_ != NULL)
			{
				curr_conn_->do_write(res);
			}
			return 0;
		}

		int request_handler::handle_request_client_shutdown(boost::property_tree::ptree parser)
		{
			api_client_info_t api_reg;
			api_reg.read_json(parser);

			std::string ip = (char*)api_reg.nIPAdress;
			std::string sessionId = TransformHandler.getSessionIdByIP(ip);

			//int ret = TransformHandler.shutdownClientHandler(api_reg) ? RESULT_SUCCESS : RESULT_FAILED;
			int type = parser.get<int>("ServerType");

			boost::property_tree::ptree root;
			root.put<int>("CmdType", CMD_API_CLIENT_SHUTDOWN_RES);
			root.put<int>("ErrorCode", RESULT_SUCCESS);
			root.put<int>("ServerType", type);
			api_reg.write_json(root);

			std::string json_parser;
			handle_json_header(root, "CMD_API_CLIENT_SHUTDOWN_RES", json_parser);

			response res(json_parser.length());
			memcpy(res.netbuf_.buffer, json_parser.c_str(), json_parser.length());

			std::shared_ptr<connection> conn = curr_conn_->connection_manager_.get_connect(sessionId);
			if (conn != NULL)
			{
				conn->do_write(res);
			}
			return 0;
		}

		int request_handler::handle_request_sync_data(boost::property_tree::ptree parser)
		{
			api_syncdata_info_t api_reg;
			api_reg.read_json(parser);

			TransformHandler.syncData(api_reg);

			boost::property_tree::ptree root;
			root.put<int>("CmdType", CMD_API_SYNC_DATA_RES);
			root.put<int>("ErrorCode", RESULT_SUCCESS);
			root.put<int>("ServerType", CSERVER_CLIENT);

			std::string json_parser;
			handle_json_header(root, "CMD_API_SYNC_DATA_RES", json_parser);

			response res(json_parser.length());
			memcpy(res.netbuf_.buffer, json_parser.c_str(), json_parser.length());

			if (curr_conn_ != NULL)
			{
				curr_conn_->do_write(res);
			}
			return 0;
		}

		int request_handler::handle_request_call_voice_value(boost::property_tree::ptree parser)
		{
			boost::property_tree::ptree root;
			root.put<int>("CmdType", CMD_API_CLIENT_REQUEST_CALLVOICE_VALUE_RES);
			root.put<int>("ErrorCode", RESULT_SUCCESS);
			root.put<int>("ServerType", IPAD_CLIENT);

			api_syncdata_info_t api_reg;
			api_reg.nCallVioceValue = TransformHandler.getSyncData();
			api_reg.write_json(root);

			std::string json_parser;
			handle_json_header(root, "CMD_API_CLIENT_REQUEST_CALLVOICE_VALUE_RES", json_parser);

			response res(json_parser.length());
			memcpy(res.netbuf_.buffer, json_parser.c_str(), json_parser.length());

			if (curr_conn_ != NULL)
			{
				curr_conn_->do_write(res);
			}
			return 0;
		}

		int request_handler::handle_request_control_call_voice(boost::property_tree::ptree parser)
		{
			api_syncdata_info_t api_reg;
			api_reg.read_json(parser);

			boost::property_tree::ptree root;
			root.put<int>("CmdType", CMD_API_CLIENT_CONTROL_CALLVOICE_VALUE_RES);
			root.put<int>("ErrorCode", RESULT_SUCCESS);
			root.put<int>("ServerType", IPAD_CLIENT);

			TransformHandler.syncData(api_reg);
			api_reg.write_json(root);

			std::string json_parser;
			handle_json_header(root, "CMD_API_CLIENT_CONTROL_CALLVOICE_VALUE_RES", json_parser);

			response res(json_parser.length());
			memcpy(res.netbuf_.buffer, json_parser.c_str(), json_parser.length());

			if (curr_conn_ != NULL)
			{
				curr_conn_->connection_manager_.broadcast(res);
			}
			return 0;
		}

		int request_handler::handle_request_cancel_boardcast(boost::property_tree::ptree parser)
		{
			api_boardcast_info_t api_info;
			api_info.read_json(parser);

			boost::property_tree::ptree root;
			root.put<int>("CmdType", CMD_API_CLIENT_CANCEL_BOARDCAST);
			root.put<int>("ErrorCode", RESULT_SUCCESS);
			root.put<int>("ServerType", IPAD_CLIENT);

			api_info.write_json(root);

			std::string json_parser;
			handle_json_header(root, "CMD_API_CLIENT_CANCEL_BOARDCAST", json_parser);

			response res(json_parser.length());
			memcpy(res.netbuf_.buffer, json_parser.c_str(), json_parser.length());

			std::string session_id = TransformHandler.getHostSession();
			std::shared_ptr<connection> conn = curr_conn_->connection_manager_.get_connect(session_id);
			if (conn != NULL){
				conn->do_write(res);
			}
			return 0;
		}

		int request_handler::handle_request_masterLocker_register(boost::property_tree::ptree parser)
		{
			api_masterLocker_register_info api_info;
			api_info.read_json(parser);

			std::string sessionId = curr_conn_->get_sessionid();
			TransformHandler.addMapList(api_info, sessionId);
			return 0;
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
			AppConfigHandler.Info("server send " + to_string(title.str().length()) + " bytes json string");

			json_str = title.str();
		}

	}  // namespace server
} // namespace dem1
