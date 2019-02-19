#include "response.hpp"
#include <string>

namespace dm {
	namespace server {

        response::response(int buffer_len) {
            
            netbuf_.buffer_len = buffer_len;
            netbuf_.buffer = (char*)malloc(netbuf_.buffer_len);
            netbuf_.buffer_offset = 0;
            memset(netbuf_.buffer, 0, netbuf_.buffer_len * sizeof(char));
        }
        
        response::~response() {
            
            if (netbuf_.buffer != NULL)
            {
                delete[] netbuf_.buffer;
            }
        }
        
        void  response::init_netbuf(int msg_len){
            
            
            if (netbuf_.buffer_len < msg_len)
            {
                if (netbuf_.buffer != NULL)
                {
                    delete[] netbuf_.buffer;
                }
                
            }
            netbuf_.buffer_len = msg_len;
            netbuf_.buffer_offset = 0;
            
        }
        
        
        
        
        
        
        std::vector<boost::asio::const_buffer> response::to_buffers() {
            
            
            
            
            
            //boost::asio::detail::function<void()> debug_check;
            
            std::vector<boost::asio::const_buffer> buffers;
            buffers.push_back(boost::asio::const_buffer(netbuf_.buffer, netbuf_.buffer_len));
            return buffers;
            
            
        }


	}  // namespace server
}  // namespace dem1
