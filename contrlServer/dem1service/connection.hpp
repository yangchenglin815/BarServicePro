#ifndef M1_CONNECTION_HPP
#define M1_CONNECTION_HPP

#include <array>
#include <memory>
#include <boost/asio.hpp>
#include "APIProtocolHandler.h"
#include "response.hpp"
#include "request_handler.hpp"


#define  SOCKET_TIMEOUT 300
namespace dem1 {
	namespace server {

		class connection_manager;

		/// Represents a single connection from a client.
		class connection : public std::enable_shared_from_this<connection> {
		public:

			connection(boost::asio::ip::tcp::socket socket,
				connection_manager& manager);
			~connection();

			/// Start the first asynchronous operation for the connection.
			void start();

			/// Stop all asynchronous operations associated with the connection.
			void stop();

			void do_write(response& res);

			std::string get_sessionid();

		

        public:
            /// The manager for this connection.
            connection_manager& connection_manager_;


		private:
            void do_read();   

			void set_timeout(long seconds);
			void cancel_timeout();

			void handle_msg_length(const boost::system::error_code& error, size_t bytes_transferred);
			void handle_msg_body(const boost::system::error_code& error, size_t bytes_transferred);
			void handle_msg_json(const boost::system::error_code& error, size_t bytes_transferred);
            
			void  ensure_incoming_buf_capa(size_t required_size);

          
			/// Socket for the connection.
			boost::asio::ip::tcp::socket socket_;

		

			/// The handler used to process the incoming request.
			request_handler request_handler_;

			/// Buffer for incoming data.
			protocol_head_t prot_head_;
			int incoming_buf_size_;
			netbuffer_t incoming_buf_;

			std::string session_id_;
           

			std::unique_ptr<boost::asio::deadline_timer> timer_;

			bool is_close_flag_;

		};

		typedef std::shared_ptr<connection> connection_ptr;

	}  // namespace server
} // namespace dem1

#endif  // M1_CONNECTION_HPP
