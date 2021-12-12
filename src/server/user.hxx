#ifndef F85705C8_6F01_4F50_98CA_5636F5F5E1C1
#define F85705C8_6F01_4F50_98CA_5636F5F5E1C1

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/optional.hpp>
#include <deque>
#include <set>
#include <string>

struct User
{
  std::deque<std::string> msgQueue{};
};

#endif /* F85705C8_6F01_4F50_98CA_5636F5F5E1C1 */
