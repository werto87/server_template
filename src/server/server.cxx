#include "server.hxx"
#include "src/logic/logic.hxx"
#include <iostream>

using namespace boost::beast;
using namespace boost::asio;
using boost::asio::ip::tcp;
using tcp_acceptor = use_awaitable_t<>::as_default_on_t<tcp::acceptor>;

Server::Server (boost::asio::ip::tcp::endpoint const &endpoint) : _endpoint{ endpoint } {}

awaitable<std::string>
Server::my_read (Websocket &ws_)
{
  std::cout << "read" << std::endl;
  flat_buffer buffer;
  co_await ws_.async_read (buffer);
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
Server::listener ()
{
  auto executor = co_await this_coro::executor;
  tcp_acceptor acceptor (executor, _endpoint);
  for (;;)
    {
      try
        {
          auto socket = co_await acceptor.async_accept ();
          auto connection = std::make_shared<Websocket> (Websocket{ std::move (socket) });
          users.emplace_back (std::make_shared<User> ());
          std::list<std::shared_ptr<User> >::iterator user = std::next (users.end (), -1);
          connection->set_option (websocket::stream_base::timeout::suggested (role_type::server));
          connection->set_option (websocket::stream_base::decorator ([] (websocket::response_type &res) { res.set (http::field::server, std::string (BOOST_BEAST_VERSION_STRING) + " websocket-server-async"); }));
          co_await connection->async_accept ();
          co_spawn (
              executor, [connection, this, &user] () mutable { return readFromClient (user, *connection); }, detached);
          co_spawn (
              executor, [connectionWeakPointer = std::weak_ptr<Websocket>{ connection }, &user] () mutable { return user->get ()->writeToClient (connectionWeakPointer); }, detached);
        }
      catch (std::exception &e)
        {
          std::cout << "Server::listener () connect  Exception : " << e.what () << std::endl;
        }
    }
}
