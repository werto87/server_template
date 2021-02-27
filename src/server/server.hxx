#ifndef AD140436_3FBA_4D63_8C0E_9113B92859E0
#define AD140436_3FBA_4D63_8C0E_9113B92859E0

#include "src/database/database.hxx"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <queue>
#include <string>
class Server
{
public:
  Server (boost::asio::io_context &_io_context);

  boost::asio::awaitable<std::string> my_read (boost::beast::websocket::stream<boost::beast::tcp_stream> &ws_);

  void handleMessage (std::string const &msg);

  boost::asio::awaitable<void> readFromClient (std::shared_ptr<boost::beast::websocket::stream<boost::beast::tcp_stream> > ws_);

  boost::asio::awaitable<void> writeToClient (std::shared_ptr<boost::beast::websocket::stream<boost::beast::tcp_stream> > ws_);

  boost::asio::awaitable<void> listener ();

  void createAccount (std::string const &msg);

  void createPlayer (std::string const &msg);

  boost::asio::io_context &_io_context;
  std::queue<std::string> msgToSend{};
};

#endif /* AD140436_3FBA_4D63_8C0E_9113B92859E0 */
