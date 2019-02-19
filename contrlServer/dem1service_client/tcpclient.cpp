#include "tcpclient.hpp"
#include <iostream>
#include <boost/algorithm/string.hpp>

namespace dem1 {
	namespace client {

		using boost::asio::ip::tcp;

		tcpclient::tcpclient(const std::string& address, uint16_t port)
			: address_(address), port_(port), incoming_buf_size_(1024){

			incoming_buf_.buffer = new char[incoming_buf_size_];
			incoming_buf_.buffer_len = incoming_buf_size_;


			// Get a list of endpoints corresponding to the server name.
			tcp::resolver resolver(io_service_);
			tcp::resolver::query query(address_, std::to_string(port_));
			tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
			tcp::resolver::iterator end;

			// Try each endpoint until we successfully establish a connection.
			boost::system::error_code error = boost::asio::error::host_not_found;
			while (error && endpoint_iterator != end) {
				socket_.close();
				socket_.connect(*endpoint_iterator++, error);
			}
			if (error) {
				printf("err %s",error.message().c_str());
			}
		}

		void tcpclient::sendData(protocol_head_t api_head){
			printf("sendData >> \n");
			api_head.nDataLen = 100;
			int msg_len = sizeof(api_head);

			netbuffer netbuf;
			netbuf.buffer = (char*)malloc(msg_len);
			memset(netbuf.buffer, 0, msg_len * sizeof(char));
			api_head.write(&netbuf);
			
			boost::asio::detail::function<void()> debug_check;

			std::vector<boost::asio::const_buffer> buffers;
			buffers.push_back(boost::asio::const_buffer(netbuf.buffer, msg_len, debug_check));
			boost::asio::write(socket_, buffers);

			free(netbuf.buffer);

			

		}


		void tcpclient::sendData(protocol_head_t api_head, api_register_t api_reg){

            printf("sendData >> \n");//api_head.nDataLen = sizeof(api_reg);
            int msg_len = sizeof(api_head) + sizeof(api_reg);

			netbuffer netbuf;
			netbuf.buffer = (char*)malloc(msg_len);
			memset(netbuf.buffer, 0, msg_len * sizeof(char));

			api_head.write(&netbuf);
			api_reg.write(&netbuf);

			boost::asio::detail::function<void()> debug_check;

			std::vector<boost::asio::const_buffer> buffers;
			buffers.push_back(boost::asio::const_buffer(netbuf.buffer, msg_len, debug_check));

			boost::asio::write(socket_, buffers);
			
			free(netbuf.buffer);

		}
        void tcpclient::do_read() {
		
			while (true)
			{
				incoming_buf_.buffer_offset = 0;
				memset(incoming_buf_.buffer, 0, incoming_buf_.buffer_len);
				boost::asio::read(socket_, boost::asio::buffer(incoming_buf_.buffer, sizeof(protocol_head_t)));
		
				incoming_buf_.buffer_offset = 0;
				incoming_buf_.buffer_len = sizeof(protocol_head_t);
				prot_head_.read(&incoming_buf_);

				ensure_incoming_buf_capa(prot_head_.nDataLen);
				memset(incoming_buf_.buffer, 0, incoming_buf_.buffer_len);

				boost::asio::read(socket_, boost::asio::buffer(incoming_buf_.buffer, prot_head_.nDataLen));
				bool invalid_msg_id = false;
				//≥ı ºªØ
			
				api_register_res_t api_reg_res;
                
				switch (prot_head_.nCmd)
				{
				case CMD_REGISTER_RES:

					api_reg_res.read(&incoming_buf_);
                    
					printf("api_head type %d  api_reg_res data %s \r\n", prot_head_.nCmd, api_reg_res.data);

					break;
				default:
					invalid_msg_id = true;
					std::cout << "invalid message id = " << prot_head_.nCmd << std::endl;
					break;
				}
                
			}

			
			return ;
		}


		

		void  tcpclient::ensure_incoming_buf_capa(size_t required_size)
		{
			size_t orig_size = incoming_buf_size_;
			while (incoming_buf_size_ < required_size) {
				incoming_buf_size_ *= 2;
			}

			if (orig_size != incoming_buf_size_) {
				delete[] incoming_buf_.buffer;
				incoming_buf_.buffer = new char[incoming_buf_size_];
			}
		}




	}  // namespace client
}  // namespace 
