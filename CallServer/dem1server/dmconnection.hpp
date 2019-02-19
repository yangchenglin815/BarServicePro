#pragma once

#include <array>
#include <memory>
#include <boost/asio.hpp>

#include "APIProtocol.h"

#include "response.hpp"
#include "request_handler.hpp"

#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

typedef enum 
{
	SOCKETSTATE_NONE,
	SOCKETSTATE_SUCCESS,
	SOCKETSTATE_FAILED,
	SOCKETSTATE_DISCONNECT


}SOCKETSTATE_ENUM;
typedef std::function<void(SOCKETSTATE_ENUM)>  SocketStateCallBackFun;


namespace dm {
	namespace server {

		class connection_manager;

		/// Represents a single connection from a client.
		class dmconnection : public std::enable_shared_from_this<dmconnection> {
		public:

			dmconnection(boost::asio::ip::tcp::socket socket);
			~dmconnection();




		
			/// Start the first asynchronous operation for the connection.
			void start();

			/// Stop all asynchronous operations associated with the connection.
			void stop();

			void close();

			void do_write(response& res);


			SOCKETSTATE_ENUM get_socketstate();


			std::string get_sessionid();
		

		private:

			void do_read();


			void handle_msg_length(const boost::system::error_code& error, size_t bytes_transferred);
			void handle_msg_body(const boost::system::error_code& error, size_t bytes_transferred);
			void handle_msg_json(const boost::system::error_code& error, size_t bytes_transferred);


			void  ensure_incoming_buf_capa(size_t required_size);
	

			
		private:
			/// Socket for the connection.
			boost::asio::ip::tcp::socket socket_;
			SOCKETSTATE_ENUM socket_state_;


			/// The handler used to process the incoming request.
			request_handler request_handler_;


			/// Buffer for incoming data.
			protocol_head_t prot_head_;
			int incoming_buf_size_;
			netbuffer_t incoming_buf_;

			std::string session_id_;

			bool socket_closed_ = false;

			bool is_close_flag_ = false;

		//	boost::mutex mutex_buffer_;

		};

		typedef std::shared_ptr<dmconnection> connection_ptr;

	}  // namespace server
} // namespace dem1

