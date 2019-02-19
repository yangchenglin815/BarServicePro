#include "tcpserver.hpp"
#include <signal.h>
#include <utility>
#include "logging.hpp"

#define RESET "\033[0m"
#define BLACK "\033[30m" /* Black */
#define RED "\033[31m" /* Red */
#define GREEN "\033[32m" /* Green */
#define YELLOW "\033[33m" /* Yellow */
#define BLUE "\033[34m" /* Blue */
#define MAGENTA "\033[35m" /* Magenta */
#define CYAN "\033[36m" /* Cyan */
#define WHITE "\033[37m" /* White */
#define BOLDBLACK "\033[1m\033[30m" /* Bold Black */
#define BOLDRED "\033[1m\033[31m" /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m" /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m" /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m" /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m" /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m" /* Bold White */

namespace dem1 {
namespace server {

tcpserver::tcpserver(const std::string& address, const std::string& port) : io_service_(),
      signals_(io_service_),
      acceptor_(io_service_),
      connection_manager_(),
      socket_(io_service_) {

  signals_.add(SIGINT);
  signals_.add(SIGTERM);
  signals_.add(SIGABRT);

#if defined(SIGQUIT)
  signals_.add(SIGQUIT);
#endif  // defined(SIGQUIT)



  do_await_stop();

  // Open the acceptor with the option to reuse the address (i.e.
  // SO_REUSEADDR).

  boost::asio::ip::tcp::resolver resolver(io_service_);
  boost::asio::ip::tcp::endpoint endpoint;
  try
  {
	  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	 endpoint = *resolver.resolve({ address, port });

	 acceptor_.open(endpoint.protocol());
	 acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	 acceptor_.set_option(boost::asio::ip::tcp::acceptor::send_buffer_size(64*1024));
	 //acceptor_.set_option(boost::asio::ip::tcp::acceptor::keep_alive(true));
	 acceptor_.set_option(boost::asio::ip::tcp::no_delay(true));


	 acceptor_.bind(endpoint);
	 acceptor_.listen();


  }
  catch (boost::system::system_error e)
  {
	  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
	  //Log::Debug(" resolver.resolve %s,%s   error code %d", address.c_str(), port.c_str(), e.code().value());
	  AppConfigHandler.Info(" resolver.resolve " + address +"," + port + "  error code " + to_string(e.code().value()));
	  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
	  std::cout << "WARNING : SERVER IP OR PORT ERROR !" << std::endl;
	  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	  return;
  }

  


  do_accept();
}

void tcpserver::run() {


	//io_service_.run(); return;
  for (int i = 0; i < MAX_SERVICE_THREAD_NUM;i++)
  {
	  service_thread_[i].reset(new boost::thread(&tcpserver::runservice,this));
  }

  for (int i = 0; i < MAX_SERVICE_THREAD_NUM; i++)
  {
	  service_thread_[i]->join();
  }
}

void tcpserver::runservice() {
	// The io_service::run() call will block until all asynchronous operations
	// have finished. While the server is running, there is always at least one
	// asynchronous operation outstanding: the asynchronous accept call waiting
	// for new incoming connections.
	io_service_.run();
}

void tcpserver::do_accept() {
	
  acceptor_.async_accept(socket_, [this](boost::system::error_code ec) {
    // Check whether the server was stopped by a signal before this
    // completion handler had a chance to run.
    if (!acceptor_.is_open()) {
      return;
    }

    if (!ec)
    {
      connection_manager_.start(std::make_shared<connection>(
          std::move(socket_), connection_manager_));
    }
    do_accept();
  });
}

void tcpserver::do_await_stop() {
  signals_.async_wait([this](boost::system::error_code /*ec*/, int /*signo*/) {
    // The server is stopped by cancelling all outstanding asynchronous
    // operations. Once all operations have finished the
    // io_service::run()
    // call will exit.
    acceptor_.close();
    connection_manager_.stop_all();
  });
}

}  // namespace server
}  // namespace dem1
