#ifndef RTSP_CLIENT_HPP
#define RTSP_CLIENT_HPP

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <map>
#include <boost/asio.hpp>

#include "APIProtocol.h"

using namespace dem1::server;
namespace dem1 {
	namespace client {

		struct response {
			std::string status_line;
			std::map<std::string, std::string> headers;
			std::string content;
		};

		class tcpclient {
		public:
			tcpclient(const tcpclient&) = delete;             // not copiable
			tcpclient& operator=(const tcpclient&) = delete;  // not copiable
			tcpclient(const std::string& address, uint16_t port);

			void sendData(protocol_head_t api_head);
			void sendData(protocol_head_t api_head, api_register_t api_reg);
			void do_read();
		private:
			
			void ensure_incoming_buf_capa(size_t required_size);

		private:

			boost::asio::io_service io_service_;
			boost::asio::ip::tcp::socket socket_{ io_service_ };
			std::string address_;
			uint16_t port_;
			std::string session_;
			std::string url_;
	



			/// Buffer for incoming data.
			protocol_head_t prot_head_;
			int incoming_buf_size_;
			netbuffer_t incoming_buf_;
		};

	}  // namespace client
}


#endif 
