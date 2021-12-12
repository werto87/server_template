#include "src/server/user.hxx"
#include "server.hxx"
#include "src/logic/logic.hxx"
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/system_timer.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <list>
#include <memory>
#include <string>

// TODO use cmake to find out if the compiler is gcc or clang
#include <coroutine> // enable if build with gcc
// #include <experimental/coroutine> //enable if build with clang

using namespace boost::beast;
using namespace boost::asio;

boost::asio::awaitable<void>
User::writeToClient (std::weak_ptr<Websocket> &connection)
{
  try
    {
      while (not connection.expired ())
        {
          timer = std::make_shared<CoroTimer> (CoroTimer{ co_await this_coro::executor });
          timer->expires_after (std::chrono::system_clock::time_point::max () - std::chrono::system_clock::now ());
          try
            {
              co_await timer->async_wait ();
            }
          catch (boost::system::system_error &e)
            {
              using namespace boost::system::errc;
              if (operation_canceled == e.code ())
                {
                  // swallow cancel
                }
              else
                {
                  std::cout << "error in timer boost::system::errc: " << e.code () << std::endl;
                  abort ();
                }
            }
          while (not msgQueue.empty () && not connection.expired ())
            {
              auto tmpMsg = std::move (msgQueue.front ());
              std::cout << " msg: " << tmpMsg << std::endl;
              msgQueue.pop_front ();
              co_await connection.lock ()->async_write (buffer (tmpMsg), use_awaitable);
            }
        }
    }
  catch (std::exception &e)
    {
      std::cout << "write Exception: " << e.what () << std::endl;
    }
}