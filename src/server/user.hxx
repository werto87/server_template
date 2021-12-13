#ifndef F85705C8_6F01_4F50_98CA_5636F5F5E1C1
#define F85705C8_6F01_4F50_98CA_5636F5F5E1C1

#include <boost/asio.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/beast.hpp>
#include <deque>

#ifdef BOOST_ASIO_HAS_CLANG_LIBCXX
#include <experimental/coroutine>
#endif

typedef boost::asio::use_awaitable_t<>::as_default_on_t<boost::asio::basic_waitable_timer<boost::asio::chrono::system_clock> > CoroTimer;
using Websocket = boost::beast::websocket::stream<boost::asio::use_awaitable_t<>::as_default_on_t<boost::beast::tcp_stream> >;

class User
{
public:
  boost::asio::awaitable<void> writeToClient (std::weak_ptr<Websocket> &connection);
  void sendMessageToUser (std::string const &message);

private:
  std::deque<std::string> msgQueue{};
  std::shared_ptr<CoroTimer> timer{};
};

#endif /* F85705C8_6F01_4F50_98CA_5636F5F5E1C1 */
