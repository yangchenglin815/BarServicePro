#ifndef M1_SERVER_HPP
#define M1_SERVER_HPP

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>
#include <string>
#include <memory>
#include "connection.hpp"
#include "connection_manager.hpp"

#define MAX_SERVICE_THREAD_NUM 5
namespace dem1 {
namespace server {

/// The top-level class of the RTSP server.
class tcpserver {
 public:
  tcpserver(const tcpserver&) = delete;
  tcpserver& operator=(const tcpserver&) = delete;

  /// Construct the server to listen on the specified TCP address and port,
  /// and
  /// serve up files from the given directory.
  explicit tcpserver(const std::string& address, const std::string& port);

  /// Run the server's io_service loop.
  void run();

 private:
  void runservice();
  /// Perform an asynchronous accept operation.
  void do_accept();

  /// Wait for a request to stop the server.
  void do_await_stop();

  /// The io_service used to perform asynchronous operations.
  boost::asio::io_service io_service_;

  /// The signal_set is used to register for process termination
  /// notifications.
  boost::asio::signal_set signals_;

  /// Acceptor used to listen for incoming connections.
  boost::asio::ip::tcp::acceptor acceptor_;

  /// The connection manager which owns all live connections.
  connection_manager connection_manager_;

  /// The next socket to be accepted.
  boost::asio::ip::tcp::socket socket_;


  boost::scoped_ptr<boost::thread> service_thread_[MAX_SERVICE_THREAD_NUM];


};


}  // namespace server
}  // namespace dem1

#endif  // M1_SERVER_HPP
