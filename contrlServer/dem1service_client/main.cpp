#include "tcpclient.hpp"
#include <boost/thread.hpp>
#include <vector>
#include <boost/shared_ptr.hpp>
void run(){
	dem1::client::tcpclient c("10.0.0.94", 8088);

	protocol_head_t api_head;

    api_head.nStartFlags = 0xAF;
	api_head.nVersion = 2;
	api_head.nCmd = CMD_REGISTER;
	api_head.nDataLen = 100;
	api_head.nResult = 4;
	api_head.nReserver = 3;
    api_head.nEndFlags = 0xFA;

	api_register_t api_reg;
	api_reg.szDeviceId[0] = 'a';
	api_reg.szUserName[0] = 'b';
	api_reg.szPassWord[0] = 'c';
    api_reg.nHeartbeatInterval = 100;

    //c.sendData(api_head);
	c.sendData(api_head, api_reg);
	//c.sendData(api_head, api_reg);

	c.do_read();
}

void main(){


	std::vector<boost::shared_ptr<boost::thread> > threads;
	for (std::size_t i = 0; i < 1; ++i)
	{
		boost::shared_ptr<boost::thread> t(new boost::thread(&run));
		threads.push_back(t);
	}

	// Wait for all threads in the pool to exit.
	for (std::size_t i = 0; i < threads.size(); ++i)
		threads[i]->join();
}
