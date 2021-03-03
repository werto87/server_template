#include "src/logic/logic.hxx"
#include "src/database/database.hxx"
#include <algorithm>
#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/type_index.hpp>
#include <sstream>
#include <string>

std::vector<std::string>
handleMessage (std::string const &msg)
{
  auto result = std::vector<std::string>{};
  if (boost::algorithm::starts_with (msg, "create new account|"))
    {
      result = createAccount (msg);
    }
  if (boost::algorithm::contains (msg, "player direction|"))
    {
      result = createCharacter (msg);
    }
  return result;
}

std::vector<std::string>
createAccount (std::string const &msg)
{
  auto result = std::vector<std::string>{};
  std::vector<std::string> splitMesssage{};
  boost::algorithm::split (splitMesssage, msg, boost::is_any_of ("|"));
  if (splitMesssage.size () >= 2)
    {
      boost::algorithm::split (splitMesssage, splitMesssage.at (1), boost::is_any_of (","));
      if (splitMesssage.size () >= 2)
        {
          if (auto account = database::createAccount (splitMesssage.at (0), splitMesssage.at (1)); account.has_value ())
            {
              auto accountStringStream = std::stringstream{};
              boost::archive::text_oarchive accountArchive{ accountStringStream };
              accountArchive << account.value ();
              result.push_back (accountStringStream.str ());
            }
        }
    }
  return result;
}

std::vector<std::string>
createCharacter (std::string const &msg)
{
  auto result = std::vector<std::string>{};
  std::vector<std::string> splitMesssage{};
  boost::algorithm::split (splitMesssage, msg, boost::is_any_of ("|"));
  if (splitMesssage.size () >= 2)
    {
      if (auto character = database::createCharacter (splitMesssage.at (1)); character.has_value ())
        {
          auto characterStringStream = std::stringstream{};
          boost::archive::text_oarchive characterArchive{ characterStringStream };
          characterArchive << character.value ();
          result.push_back (characterStringStream.str ());
        }
    }
  return result;
}