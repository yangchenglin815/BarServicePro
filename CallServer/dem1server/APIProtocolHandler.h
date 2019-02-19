#pragma once
#include "netbuffer.h"
#include "APIProtocol.h"
#include "request_handler.hpp"

#define API_HANDLER(name) api=malloc(sizeof(name)); \
((name *)api)->read(buf); \
handler->handle_request((name *)api); \
free(api);

using namespace dm::server;

static void API_Handler(std::string message, int header_length, int content_length, request_handler *handler){

	void* api = NULL;
	try
	{
		std::string api_message = message.substr(header_length, content_length);

		std::stringstream ss(api_message);
		boost::property_tree::ptree parser;
		read_json(ss, parser);

		short nCmd = parser.get<short>("CmdType");
		switch (nCmd)
		{
		case CMD_API_CLIENT_HOST_REGISTER_RES:
			//AppConfigHandler.Info("APIProtocal_Handler : CMD_API_CLIENT_HOST_REGISTER_RES");
			Log::Info("APIProtocal_Handler : CMD_API_CLIENT_HOST_REGISTER_RES");
			handler->handle_request_sync_data();
			break;
		case CMD_API_CLIENT_BOARDCAST_MSG_RES:
			//AppConfigHandler.Info("APIProtocal_Handler : CMD_API_CLIENT_BOARDCAST_MSG_RES");
			Log::Info("APIProtocal_Handler : CMD_API_CLIENT_BOARDCAST_MSG_RES");
			handler->handle_request_boardcast_msg(parser);
			break;
		case CMD_API_CLIENT_CANCEL_BOARDCAST:
			//AppConfigHandler.Info("APIProtocal_Handler : CMD_API_CLIENT_CANCEL_BOARDCAST");
			Log::Info("APIProtocal_Handler : CMD_API_CLIENT_CANCEL_BOARDCAST");
			handler->handle_request_cancel_boardcast_msg(parser);
			break;
		case CMD_API_SYNC_DATA_RES:
			//AppConfigHandler.Info("APIProtocal_Handler : CMD_API_SYNC_DATA_RES");
			Log::Info("APIProtocal_Handler : CMD_API_SYNC_DATA_RES");
			break;
		case CMD_API_CLIENT_CONTROL_CALLVOICE_VALUE_RES:
			//AppConfigHandler.Info("APIProtocal_Handler : CMD_API_CLIENT_CONTROL_CALLVOICE_VALUE_RES");
			Log::Info("APIProtocal_Handler : CMD_API_CLIENT_CONTROL_CALLVOICE_VALUE_RES");
			break;
		default:
			break;
		}
	}
	catch (boost::property_tree::ptree_error const & error)
	{
		//AppConfigHandler.Info("boost read json errCode : " + string(error.what()));
		Log::Info("boost read json errCode : %s", error.what());
	}
}

static void APIProtocal_Handler(protocol_head_t head, netbuffer *buf, request_handler *handler)
{

    void* api = NULL;
    
	switch (head.nCmd)
	{
	case CMD_API_CLIENT_HOST_REGISTER_RES:
		//AppConfigHandler.Info("APIProtocal_Handler : CMD_API_CLIENT_HOST_REGISTER_RES");
		Log::Info("APIProtocal_Handler : CMD_API_CLIENT_HOST_REGISTER_RES");
		break;
	case CMD_API_CLIENT_BOARDCAST_MSG_RES:
		//AppConfigHandler.Info("APIProtocal_Handler : CMD_API_CLIENT_BOARDCAST_MSG_RES");
		Log::Info("APIProtocal_Handler : CMD_API_CLIENT_BOARDCAST_MSG_RES");
		handler->handle_request_boardcast_msg(head, buf);
		break;
	default:
		break;
	}
}
