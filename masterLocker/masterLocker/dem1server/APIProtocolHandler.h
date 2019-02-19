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
		case CMD_API_CLIENT_REBOOT_RES:
			Log::Info("APIProtocal_Handler : CMD_API_CLIENT_REBOOT_RES");
			handler->handle_request_client_reboot(parser);
			break;
		case CMD_API_CLIENT_SHUTDOWN_RES:
			Log::Info("APIProtocal_Handler : CMD_API_CLIENT_SHUTDOWN_RES");
			handler->handle_request_client_shutdown(parser);
			break;
		default:
			break;
		}
	}
	catch (boost::property_tree::ptree_error const & error)
	{
		Log::Info("boost read json errCode : %s", error.what());
	}
}

static void APIProtocal_Handler(protocol_head_t head, netbuffer *buf, request_handler *handler)
{

    void* api = NULL;
    
	switch (head.nCmd)
	{
	case CMD_API_CLIENT_REBOOT_RES:
		break;
	default:
		break;
	}
}
