#include "src/server/server.hxx"
#include "src/logic/logic.hxx"
#include <algorithm>
#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/type_index.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <coroutine>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>

using namespace boost::beast;
using namespace boost::asio;

Server::Server (boost::asio::io_context &io_context) : _io_context{ io_context } {}

awaitable<std::string>
Server::my_read (websocket::stream<tcp_stream> &ws_)
{
  std::cout << "read" << std::endl;
  flat_buffer buffer;
  co_await ws_.async_read (buffer, use_awaitable);
  auto msg = buffers_to_string (buffer.data ());
  std::cout << "number of letters '" << msg.size () << "' msg: '" << msg << "'" << std::endl;
  co_return msg;
}

awaitable<void>
Server::readFromClient (std::shared_ptr<websocket::stream<tcp_stream> > ws_)
{
  try
    {
      for (;;)
        {
          auto readResult = co_await my_read (*ws_);
          auto result = handleMessage (readResult);
          msgToSend.insert (msgToSend.end (), make_move_iterator (result.begin ()), make_move_iterator (result.end ()));
        }
    }
  catch (std::exception &e)
    {
      std::cout << "echo  Exception: " << e.what () << std::endl;
    }
}

awaitable<void>
Server::writeToClient (std::shared_ptr<websocket::stream<tcp_stream> > ws_)
{
  try
    {
      for (;;)
        {
          auto timer = steady_timer (co_await this_coro::executor);
          using namespace std::chrono_literals;
          timer.expires_after (1s);
          co_await timer.async_wait (use_awaitable);
          while (not msgToSend.empty ())
            {
              co_await ws_->async_write (buffer (msgToSend.back ()), use_awaitable);
              msgToSend.pop_back ();
            }
        }
    }
  catch (std::exception &e)
    {
      std::printf ("echo Exception:  %s\n", e.what ());
    }
}

awaitable<void>
Server::listener ()
{
  auto executor = co_await this_coro::executor;
  ip::tcp::acceptor acceptor (executor, { ip::tcp::v4 (), 55555 });
  for (;;)
    {
      ip::tcp::socket socket = co_await acceptor.async_accept (use_awaitable);
      auto ws_ = std::make_shared<websocket::stream<tcp_stream> > (std::move (socket));
      ws_->set_option (websocket::stream_base::timeout::suggested (role_type::server));
      ws_->set_option (websocket::stream_base::decorator ([] (websocket::response_type &res) { res.set (http::field::server, std::string (BOOST_BEAST_VERSION_STRING) + " websocket-server-async"); }));
      co_await ws_->async_accept (use_awaitable);
      co_spawn (
          executor, [this, ws_] () mutable { return readFromClient (ws_); }, detached);
      co_spawn (
          executor, [this, ws_] () mutable { return writeToClient (ws_); }, detached);
    }
}
