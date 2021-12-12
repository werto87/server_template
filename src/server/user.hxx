#ifndef F85705C8_6F01_4F50_98CA_5636F5F5E1C1
#define F85705C8_6F01_4F50_98CA_5636F5F5E1C1

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/optional.hpp>
#include <deque>
#include <set>
#include <string>

typedef boost::asio::use_awaitable_t<>::as_default_on_t<boost::asio::basic_waitable_timer<boost::asio::chrono::system_clock> > CoroTimer;
typedef boost::beast::websocket::stream<boost::beast::tcp_stream> Websocket;

struct User
{
  boost::asio::awaitable<void> writeToClient (std::weak_ptr<Websocket> &connection);
  std::deque<std::string> msgQueue{};
  std::shared_ptr<CoroTimer> timer{};
};

#endif /* F85705C8_6F01_4F50_98CA_5636F5F5E1C1 */
