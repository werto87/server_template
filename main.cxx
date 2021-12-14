#include "src/server/server.hxx"
#include <exception>
#include <iostream>
#include <stdexcept>

auto const DEFAULT_PORT = u_int16_t{ 55555 };

int
main ()
{
  try
    {
      using namespace boost::asio;
      io_context io_context (1);
      signal_set signals (io_context, SIGINT, SIGTERM);
      signals.async_wait ([&] (auto, auto) { io_context.stop (); });
      auto server = Server{ { ip::tcp::v4 (), DEFAULT_PORT } };
      co_spawn (
          io_context, [&server] { return server.listener (); }, detached);
      io_context.run ();
    }
  catch (std::exception &e)
    {
      std::printf ("Exception: %s\n", e.what ());
    }
  return 0;
}