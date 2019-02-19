#include "connection.hpp"
#include <utility>
#include <vector>
#include "connection_manager.hpp"
#include "request_handler.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/bind.hpp>

#include "logging.hpp"
#include "appconfig.h"
#include "request_handler.hpp"

namespace dem1 {
	namespace server {

		connection::connection(boost::asio::ip::tcp::socket socket,
			connection_manager& manager)
			: socket_(std::move(socket)),
			connection_manager_(manager),
			incoming_buf_size_(64*1024),
			is_close_flag_(false),
			session_id_(boost::uuids::to_string(boost::uuids::random_generator()())) {

			incoming_buf_.buffer = new char[incoming_buf_size_];
			incoming_buf_.buffer_len = incoming_buf_size_;	

			
			socket_.non_blocking(true);
			
		}

		connection::~connection()
		{
			AppConfigHandler.Info("connection::~connection()");

			if (incoming_buf_.buffer) {
				delete[] incoming_buf_.buffer;
			}

		}


		void connection::start() 
        {
			is_close_flag_ = false;
            auto self(shared_from_this());
			AppConfigHandler.Info("add connection " + session_id_);
            int ret = request_handler_.start(self);
			if (ret == -1)
            {
				AppConfigHandler.Info("request_handler_ start Failed " + session_id_);
				this->stop();
				return;
			}
			do_read();
		}

		void connection::stop()
        {
			if (is_close_flag_ == false)
			{
				is_close_flag_ = true;
				auto self(shared_from_this());
				AppConfigHandler.Info("stop connection " + session_id_);
				request_handler_.stop();
				socket_.shutdown(boost::asio::socket_base::shutdown_type::shutdown_both);
				socket_.close();
			}
            
		}


		std::string  connection::get_sessionid(){
			return session_id_;
		}

		void connection::do_read() 
        {
			auto self(shared_from_this());

			incoming_buf_.buffer_offset = 0;
			memset(incoming_buf_.buffer, 0, incoming_buf_size_);

			this->set_timeout(SOCKET_TIMEOUT);

// 			int header_len = sizeof(protocol_head_t);
// 			boost::asio::async_read(socket_,
// 				boost::asio::buffer(incoming_buf_.buffer, header_len),
// 				boost::bind(&connection::handle_msg_length,
// 				shared_from_this(),
// 				boost::asio::placeholders::error,
// 				boost::asio::placeholders::bytes_transferred)
// 				);

			boost::asio::async_read(socket_,
				boost::asio::buffer(incoming_buf_.buffer, 256),
				boost::bind(&connection::handle_msg_json,
				shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)
				);
		}
		//
		// message frame
		//
		// +--------+------------+--------------+
		// | length | message body |
		// +--------+------------+--------------+
		//

		void connection::handle_msg_length(const boost::system::error_code& error,
			size_t bytes_transferred)
		{
			this->cancel_timeout();
			auto self(shared_from_this());
			if (!error) 
            {
				incoming_buf_.buffer_offset = 0;
				incoming_buf_.buffer_len = sizeof(protocol_head_t);
				prot_head_.read(&incoming_buf_);

				AppConfigHandler.Info("handle_msg_length::prot_head_.CMD:   " + to_string(prot_head_.nCmd));

                if (prot_head_.nStartFlags == 0xAF && prot_head_.nEndFlags == 0xFA)
				{
                    ensure_incoming_buf_capa((prot_head_.nDataLen));
                    memset(incoming_buf_.buffer, 0, incoming_buf_size_);


					this->set_timeout(SOCKET_TIMEOUT);
                    boost::asio::async_read(socket_,
                        boost::asio::buffer(incoming_buf_.buffer, (prot_head_.nDataLen)),
                        boost::bind(&connection::handle_msg_body,
                        shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred)
                        );               
				}
				else
                {
					do_read(); // handle other messages..
				}
			}
			else if (error != boost::asio::error::operation_aborted)
            {
				AppConfigHandler.Info("handle_msg_length::error:    error_code  code " + to_string(error.value()) + " msg " + error.message());
				connection_manager_.stop(shared_from_this());
			}
		}

		void connection::handle_msg_body(const boost::system::error_code& error,
			size_t bytes_transferred)
		{
			this->cancel_timeout();
			auto self(shared_from_this());
			if (!error) 
            {
				//³õÊ¼»¯
				incoming_buf_.buffer_offset = 0;
				incoming_buf_.buffer_len = prot_head_.nDataLen;
      
				APIProtocal_Handler(&prot_head_, &incoming_buf_, &request_handler_);

                this->do_read();

			}
			else if (error != boost::asio::error::operation_aborted) 
            {
   
				//Log::Info("handle_msg_body::error:    error_code  code %d msg %s", error.value(), error.message().c_str());
				AppConfigHandler.Info("handle_msg_body::error:    error_code  code " + to_string(error.value()) + " msg " + error.message());


				connection_manager_.stop(shared_from_this());
			}
		}

		void  connection::ensure_incoming_buf_capa(size_t required_size)
		{
			size_t orig_size = incoming_buf_size_;
			while (incoming_buf_size_ < required_size) {
				incoming_buf_size_ *= 2;
			}

			if (orig_size != incoming_buf_size_) {
				delete[] incoming_buf_.buffer;
				incoming_buf_.buffer = new char[incoming_buf_size_];
				incoming_buf_.buffer_len = incoming_buf_size_;
			}
		}

		void connection::do_write(response& res) 
        {
            if (!this )
            {
                return;
            }

			this->set_timeout(SOCKET_TIMEOUT);


			std::vector<boost::asio::const_buffer> buffer = res.to_buffers();
            std::shared_ptr<connection> self = shared_from_this();
            boost::asio::async_write(socket_, buffer,
                [this, self](boost::system::error_code ec, std::size_t n)
            {
				this->cancel_timeout();

                if (!ec) 
                {
					//Log::Debug("boost::asio::async_write Send Success   size %d \n\n", n);
					AppConfigHandler.Info("boost::asio::async_write Send Success   size " + to_string(n));
                }
                else if (ec != boost::asio::error::operation_aborted) 
                {
                    //Log::Error("boost::asio::async_write start  size_t %d",n);
					//Log::Error("boost::asio::async_write start  error_code  code %d msg %s", ec.value(), ec.message().c_str());
					AppConfigHandler.Info("boost::asio::async_write start  size_t " + to_string(n));
					AppConfigHandler.Info("boost::asio::async_write start  error_code  code " + to_string(ec.value()) + " msg " + ec.message());
					 if (self)
                    {
                        connection_manager_.stop(self);
                    }
					//Log::Error("boost::asio::async_write end size_t %d", n);
					AppConfigHandler.Info("boost::asio::async_write end size_t " + to_string(n));

                }
            });
         
		}


		void  connection::set_timeout(long seconds) {
			if (seconds == 0) {
				timer_ = nullptr;
				return;
			}
			
			std::shared_ptr<connection> self = shared_from_this();
			boost::asio::io_service &socket_ser = socket_.get_io_service();
			std::shared_ptr<boost::asio::deadline_timer> timer_;


			timer_.reset(new boost::asio::deadline_timer(socket_ser));
			timer_->expires_from_now(boost::posix_time::seconds(seconds));
			timer_->async_wait([this,self](const boost::system::error_code &ec) {
				if (!ec){
					//Log::Debug("timer_->async_wait Stop");
					AppConfigHandler.Info("timer_->async_wait Stop");
					if (self)
					{
						connection_manager_.stop(self);
					}
				}
				
			});
		}

		void  connection::cancel_timeout() {
			if (timer_) {
				try
				{
					timer_->cancel();
				}
				catch (boost::system::system_error e)
				{
					
				}
			
				
			}
		}

		void connection::handle_msg_json(const boost::system::error_code& error, size_t bytes_transferred)
		{
			incoming_buf_.buffer_len = bytes_transferred;
			int recv_length = incoming_buf_.buffer_len;

			this->cancel_timeout();
			auto self(shared_from_this());
			if (!error)
			{
				bool result = strstr(incoming_buf_.buffer, HTTP_CRLFCRLF);
				if (result)
				{
					std::string header = incoming_buf_.buffer;
					header = header.substr(0, header.find(HTTP_CRLFCRLF, 0));
					int header_length = header.length() + 4;

					std::string tmp = header.substr(header.find("Content-Length:", 0) + strlen("Content-Length:"));
					tmp = tmp.substr(0, tmp.find(HTTP_CRLF, 0));
					int content_length = ::atoi(tmp.c_str());

					std::string recv_Id = header.substr(header.find("Session-Id:", 0) + strlen("Session-Id:"));
					recv_Id = recv_Id.substr(0, recv_Id.find(HTTP_CRLF, 0));
					int recvId = ::atoi(recv_Id.c_str());

					int total_length = header_length + content_length;
					int expect_length = total_length - incoming_buf_.buffer_len;
					//Log::Info("total_length:%d, expect_length:%d.\n", total_length, expect_length);
					AppConfigHandler.Info("total_length: " + to_string(total_length) + ", expect_length: " + to_string(expect_length));

					int i = 0;
					while (expect_length > 0)
					{
						int length = expect_length > 1024 ? 1024 : expect_length;
						int ret = socket_.receive(boost::asio::buffer(incoming_buf_.buffer + recv_length, length));
						if (ret == -1)
						{
							//Log::Info("socket recv error.\n");
							AppConfigHandler.Info("socket recv error.");
							return;
						}
						recv_length += ret;
						expect_length -= ret;
					}

					std::string message = incoming_buf_.buffer;
					//Log::Info("receive message : %s", message.c_str());
					AppConfigHandler.Info("receive message : " + message);

					API_Handler(message, header_length, content_length, &request_handler_);

					this->do_read();
				}
			}
			else if (error != boost::asio::error::operation_aborted)
			{

				//Log::Info("handle_msg_body::error:    error_code  code %d msg %s", error.value(), error.message().c_str());
				AppConfigHandler.Info("handle_msg_body::error:    error_code  code " + to_string(error.value()) + " msg " 
					+ error.message());


				connection_manager_.stop(shared_from_this());
			}
		}

    }  // namespace server
}  // namespace dem1
