// 
//  tcpclient.cpp
//  rtspclient
// 
//  Created by qiruisun on 17/6/15.
//  Copyright © 2017年 detu. All rights reserved.


#include "tcpclient.hpp"
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/asio/detail/socket_ops.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/detail/socket_types.hpp>
#include "appconfig.h"

namespace dm {
namespace client {

using boost::asio::ip::tcp;

tcpclient::tcpclient():
signals_(io_service_),
socket_(io_service_)
{


	// 	// Get a list of endpoints corresponding to the server name.

	signals_.add(SIGINT);
	signals_.add(SIGTERM);
#if defined(SIGQUIT)
	signals_.add(SIGQUIT);
#endif 
	//	signals_.add(SIGBREAK);
	//signals_.add(SIGABRT);
	signals_.async_wait([this]
		(
		const boost::system::error_code& error,
		int signal_number)
	{
		Log::Info("signals_.async_wait  >>>>");
		exit(0);
	}
	);


	is_close_flag_ = false;
}

tcpclient::~tcpclient(){
	this->close();
}

void tcpclient::setconnectioninfo(std::string address, uint16_t port)
{
	address_ = address;
	port_ = port;
}
void tcpclient::run()
{
	io_service_.stop();
	io_service_.reset();	

	if (check_socket_thread_ != NULL && check_socket_thread_->joinable())
	{
		check_socket_thread_->join();
	}
	check_socket_thread_.reset(new boost::thread(&tcpclient::check_socket, this));
			
	io_service_.run();

	return ;
}


void tcpclient::check_socket(){

	while (true)
	{
		if (is_close_flag_ == true)
		{
			break;
		}

		if (connection_ptr_ == NULL)
		{
			if (!is_first_connect_)
			{
				if (last_socket_enum_ != SOCKETSTATE_FAILED)
				{

					last_socket_enum_ = SOCKETSTATE_FAILED;
					emit sig0nSocketConnectStatus((int)last_socket_enum_);
				}
			}
		}
		else{
			if (last_socket_enum_ != connection_ptr_->get_socketstate())
			{
				last_socket_enum_ = connection_ptr_->get_socketstate();
				emit sig0nSocketConnectStatus((int)last_socket_enum_);
			}
		}

		if (connection_ptr_ == NULL || connection_ptr_->get_socketstate() != SOCKETSTATE_SUCCESS)
		{
			reconnect();
			is_first_connect_ = false;
		}
		
		boost::this_thread::sleep(boost::posix_time::milliseconds(500));
	}
}


void tcpclient::reconnect(){

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
		return;
	}


	boost::asio::ip::tcp::no_delay no_delay_option(true);
	boost::asio::ip::tcp::acceptor::reuse_address reuseaddr(true);
	boost::asio::ip::tcp::acceptor::send_buffer_size buffersize(64*1024);

	boost::system::error_code ec;
	socket_.set_option(no_delay_option, ec);
	socket_.set_option(reuseaddr, ec);
	socket_.set_option(buffersize, ec);

	connection_ptr_.reset(new dmconnection(std::move(socket_)));
	connection_ptr_->start();

	

}

void tcpclient::close()
{
	if (is_close_flag_ == false)
	{
		io_service_.stop();
		is_close_flag_ = true;
		if (check_socket_thread_ != NULL && check_socket_thread_->joinable())
		{
			check_socket_thread_->join();
		}


		if (connection_ptr_ != NULL)
		{
			connection_ptr_->stop();
			connection_ptr_->close();
			connection_ptr_  = NULL;
		}

		io_service_.reset();
		
	}

}


dmconnection* tcpclient::get_connection(){

	return connection_ptr_.get();

}


}  // namespace client
} // namespace