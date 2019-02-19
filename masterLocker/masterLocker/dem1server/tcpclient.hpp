#ifndef tcpclient_hpp
#define tcpclient_hpp

#include <stdio.h>


#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <map>
#include <boost/asio.hpp>

#include <QObject>

#include "dmconnection.hpp"



using namespace dm::server;

namespace dm 
{
    namespace client 
	{
		class tcpclient :public QObject
		{
			Q_OBJECT

        public:
        
			tcpclient();
			~tcpclient();

			

			void setconnectioninfo(std::string address, uint16_t port);

	
			void run();
			void close();


			dmconnection*  get_connection();

		signals:
			void sig0nSocketConnectStatus(int status);


		private:
			void reconnect();
			void check_socket();
			

        private:
			bool is_first_connect_ = true;
			bool is_close_flag_ = false;
			boost::shared_ptr<boost::thread> check_socket_thread_;
			boost::asio::io_service io_service_;
			connection_ptr  connection_ptr_;
			boost::asio::ip::tcp::socket socket_;


			boost::asio::signal_set signals_;
			std::string address_; 
			uint16_t port_;

			
            

            
          
			SOCKETSTATE_ENUM last_socket_enum_ = SOCKETSTATE_DISCONNECT;
			SocketStateCallBackFun  state_fun_;
            

        };
        
    }  // namespace client
}


#endif /* tcpclient_hpp */
