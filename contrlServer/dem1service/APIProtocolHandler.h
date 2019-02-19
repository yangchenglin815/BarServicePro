#pragma once

#include "netbuffer.h"
#include "APIProtocol.h"
#include "request_handler.hpp"
#include "logging.hpp"

#define BOOST_SPIRIT_THREADSAFE

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

// #define API_HANDLER(name) api=malloc(sizeof(name)); \
// 	((name *)api)->read(buf); \
// 	handler->handle_request((name *)api); \
// 	free(api);

using namespace dem1::server;

static void API_Handler(std::string message, int header_length, int content_length, request_handler *handler){
	void* api = NULL;
	try
	{
		std::string api_message = message.substr(header_length, content_length);

		std::stringstream ss(api_message);
		boost::property_tree::ptree parser;
		boost::property_tree::read_json(ss, parser);

		short nCmd = parser.get<short>("CmdType");
		switch (nCmd)
		{
		case CMD_API_CLIENT_REBOOT:
			AppConfigHandler.Info("APIProtocal_Handler : CMD_API_CLIENT_REBOOT");
			handler->handle_request_client_reboot(parser);
			break;
		case CMD_API_CLIENT_REMOTE_WAKEUP:
			AppConfigHandler.Info("APIProtocal_Handler : CMD_API_CLIENT_REMOTE_WAKEUP");
			handler->handle_request_client_remote_wakeup(parser);
			break;
		case CMD_API_CLIENT_BOARDCAST_MSG:
			AppConfigHandler.Info("APIProtocal_Handler : CMD_API_CLIENT_BOARDCAST_MSG");
			handler->handle_request_boardcast_msg(parser);
			break;
		case CMD_API_CLIENT_HOST_REGISTER:
			AppConfigHandler.Info("APIProtocal_Handler : CMD_API_CLIENT_HOST_REGISTER");
			handler->handle_request_host_register(parser);
			break;
		case CMD_API_CLIENT_SHUTDOWN:
			AppConfigHandler.Info("APIProtocal_Handler : CMD_API_CLIENT_SHUTDOWN");
			handler->handle_request_client_shutdown(parser);
			break;
		case CMD_API_SYNC_DATA:
			AppConfigHandler.Info("APIProtocal_Handler : CMD_API_SYNC_DATA");
			handler->handle_request_sync_data(parser);
			break;
		case CMD_API_CLIENT_REQUEST_CALLVOICE_VALUE:
			AppConfigHandler.Info("APIProtocal_Handler : CMD_API_CLIENT_REQUEST_CALLVOICE_VALUE");
			handler->handle_request_call_voice_value(parser);
			break;
		case CMD_API_CLIENT_CONTROL_CALLVOICE_VALUE:
			AppConfigHandler.Info("APIProtocal_Handler : CMD_API_CLIENT_CONTROL_CALLVOICE_VALUE");
			handler->handle_request_control_call_voice(parser);
			break;
		case CMD_API_CLIENT_CANCEL_BOARDCAST:
			AppConfigHandler.Info("APIProtocal_Handler : CMD_API_CLIENT_CANCEL_BOARDCAST");
			handler->handle_request_cancel_boardcast(parser);
			break;
		case CMD_API_MASTERLOCKER_REGISTER:
			AppConfigHandler.Info("APIProtocal_Handler : CMD_API_MASTERLOCKER_REGISTER");
			handler->handle_request_masterLocker_register(parser);
			break;
		default:
			AppConfigHandler.Info("APIProtocal_Handler : invalid message id = " + to_string(nCmd));
			break;
		}
	}
	catch (boost::property_tree::ptree_error const & error)
	{
		AppConfigHandler.Info("boost read json errCode : " + string(error.what()));
	}
}

static void APIProtocal_Handler(protocol_head_t *head, netbuffer *buf, request_handler *handler){

 	void* api = NULL;
 
    switch (head->nCmd) 
    {
	case CMD_API_CLIENT_REBOOT:
		AppConfigHandler.Info("APIProtocal_Handler : CMD_API_CLIENT_REBOOT");
		handler->handle_request_client_reboot(head, buf);
		break;
	case CMD_API_CLIENT_REMOTE_WAKEUP:
		AppConfigHandler.Info("APIProtocal_Handler : CMD_API_CLIENT_REMOTE_WAKEUP");
		handler->handle_request_client_remote_wakeup(head, buf);
		break;
	case CMD_API_CLIENT_BOARDCAST_MSG:
		AppConfigHandler.Info("APIProtocal_Handler : CMD_API_CLIENT_BOARDCAST_MSG");
		handler->handle_request_boardcast_msg(head, buf);
		break;
	case CMD_API_CLIENT_HOST_REGISTER:
		AppConfigHandler.Info("APIProtocal_Handler : CMD_API_CLIENT_HOST_REGISTER");
		handler->handle_request_host_register(head, buf);
		break;
	default:
	{
		AppConfigHandler.Info("APIProtocal_Handler : invalid message id = " + to_string(head->nCmd));
	    break;
	}
    }
}