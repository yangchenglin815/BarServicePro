#pragma once


#include <string>
#include <vector>
#include <map>
#include <boost/asio.hpp>
#include "APIProtocol.h"

namespace dm {
	namespace server {

        class response {
            
        public:
            response(int buffer_len);
            ~response();
            
            
            std::vector<boost::asio::const_buffer> to_buffers();
            
            
            
            
        private:
            void init_netbuf(int msg_len);
        public:
            netbuffer netbuf_;
            
        };
        
        template<class T>
        static  void  response_add_resprotocol(response &res,T *api){
            assert(res.netbuf_.buffer_len != 0);
            api->write(&res.netbuf_);
        }


	}  // namespace server
}  // namespace dem1


