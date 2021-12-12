#include "server.hxx"
#include "src/logic/logic.hxx"
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>

#include <coroutine> // enable if build with gcc
//#include <experimental/coroutine> //enable if build with clang

using namespace boost::beast;
using namespace boost::asio;
namespace beast = boost::beast; // from <boost/beast.hpp>
Server::Server (boost::asio::ip::tcp::endpoint const &endpoint) : _endpoint{ endpoint } {}

awaitable<std::string>
Server::my_read (Websocket &ws_)
{
  std::cout << "read" << std::endl;
  flat_buffer buffer;
  co_await ws_.async_read (buffer, use_awaitable);
  auto msg = buffers_to_string (buffer.data ());
  std::cout << "number of letters '" << msg.size () << "' msg: '" << msg << "'" << std::endl;
  co_return msg;
}

awaitable<void>
Server::readFromClient (std::list<std::shared_ptr<User> >::iterator user, Websocket &connection)
{
  try
    {
      for (;;)
        {
          auto readResult = co_await my_read (connection);
          handleMessage (readResult, users, *user);
        }
    }
  catch (std::exception &e)
    {
      removeUser (user);
      std::cout << "read Exception: " << e.what () << std::endl;
    }
}

void
Server::removeUser (std::list<std::shared_ptr<User> >::iterator user)
{
  users.erase (user);
}

awaitable<void>
Server::writeToClient (std::shared_ptr<User> user, std::weak_ptr<Websocket> &connection)
{
  try
    {
      while (not connection.expired ())
        {
          // TODO this is polling because we check every 100 milli seconds.
          auto timer = steady_timer (co_await this_coro::executor);
          auto const waitForNewMessagesToSend = std::chrono::milliseconds{ 100 };
          timer.expires_after (waitForNewMessagesToSend);
          co_await timer.async_wait (use_awaitable);
          while (not user->msgQueue.empty () && not connection.expired ())
            {
              auto tmpMsg = std::move (user->msgQueue.front ());
              std::cout << " msg: " << tmpMsg << std::endl;
              user->msgQueue.pop_front ();
              co_await connection.lock ()->async_write (buffer (tmpMsg), use_awaitable);
            }
        }
    }
  catch (std::exception &e)
    {
      std::cout << "write Exception: " << e.what () << std::endl;
    }
}

awaitable<void>
Server::listener ()
{
  auto executor = co_await this_coro::executor;
  ip::tcp::acceptor acceptor (executor, _endpoint);
  for (;;)
    {
      try
        {
          auto socket = co_await acceptor.async_accept (use_awaitable);
          auto connection = std::make_shared<Websocket> (std::move (socket));
          users.emplace_back (std::make_shared<User> (User{ {} }));
          std::list<std::shared_ptr<User> >::iterator user = std::next (users.end (), -1);
          connection->set_option (websocket::stream_base::timeout::suggested (role_type::server));
          connection->set_option (websocket::stream_base::decorator ([] (websocket::response_type &res) { res.set (http::field::server, std::string (BOOST_BEAST_VERSION_STRING) + " websocket-server-async"); }));
          co_await connection->async_accept (use_awaitable);
          co_spawn (
              executor, [connection, this, &user] () mutable { return readFromClient (user, *connection); }, detached);
          co_spawn (
              executor, [connectionWeakPointer = std::weak_ptr<Websocket>{ connection }, this, &user] () mutable { return writeToClient (*user, connectionWeakPointer); }, detached);
        }
      catch (std::exception &e)
        {
          std::cout << "Server::listener () connect  Exception : " << e.what () << std::endl;
        }
    }
}
