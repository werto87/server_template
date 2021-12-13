#ifndef AC7BAF85_A15E_49E8_AD93_D8065253C6DF
#define AC7BAF85_A15E_49E8_AD93_D8065253C6DF

#include "user.hxx"
#include <list>

class Server
{
public:
  Server (boost::asio::ip::tcp::endpoint const &endpoint);

  boost::asio::awaitable<void> listener ();

private:
  void removeUser (std::list<std::shared_ptr<User> >::iterator user);
  boost::asio::awaitable<std::string> my_read (Websocket &ws_);

  boost::asio::awaitable<void> readFromClient (std::list<std::shared_ptr<User> >::iterator user, Websocket &connection);

  boost::asio::ip::tcp::endpoint _endpoint{};
  std::list<std::shared_ptr<User> > users{};
};

#endif /* AC7BAF85_A15E_49E8_AD93_D8065253C6DF */
