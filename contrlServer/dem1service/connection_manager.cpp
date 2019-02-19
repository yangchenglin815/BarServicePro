#include "connection_manager.hpp"

namespace dem1 {
    namespace server {

        connection_manager::connection_manager() {}

        void connection_manager::start(connection_ptr c) {
            connections_.insert(c);
            c->start();
        }

        void connection_manager::stop(connection_ptr c) {
            connections_.erase(c);
            c->stop();
        }

        void connection_manager::stop_all() {
            for (auto c : connections_) c->stop();
            connections_.clear();
        }

        connection_ptr connection_manager::get_connect(std::string session_id){

            for (auto c : connections_){
               if (c->get_sessionid() == session_id)
               {
                   return c;
               }
            }
            return NULL;
        }
 
        void connection_manager::close_connect(std::string session_id)
        {
            for (auto c : connections_){
                if (c->get_sessionid() == session_id)
                {
                    c->stop();
                }
            }
        }

        void connection_manager::broadcast(response& res){
 
             for (auto c : connections_){
                 c->do_write(res);
				 AppConfigHandler.Info("current session : " + c->get_sessionid());
             }
             return;
         }
 
 
       

    }  // namespace server
}  // namespace dem1
