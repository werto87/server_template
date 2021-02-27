#include "src/database/database.hxx"
#include "src/server/server.hxx"
#include <boost/bind/bind.hpp>
#include <iostream>
int
main ()
{
  try
    {
      database::createEmptyDatabase ();
      database::createTables ();
      boost::asio::io_context io_context (1);
      boost::asio::signal_set signals (io_context, SIGINT, SIGTERM);
      signals.async_wait ([&] (auto, auto) { io_context.stop (); });
      auto server = Server{ io_context };
      co_spawn (io_context, boost::bind (&Server::listener, boost::ref (server)), boost::asio::detached);
      io_context.run ();
    }
  catch (std::exception &e)
    {
      std::printf ("Exception: %s\n", e.what ());
    }

  return 0;
}