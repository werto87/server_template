#include "src/database/database.hxx"
#include "src/database/constant.hxx"
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <filesystem>

namespace database
{

void
createEmptyDatabase ()
{
  std::filesystem::create_directory (std::filesystem::path{ pathToTestDatabase }.parent_path ());
  std::filesystem::copy_file (pathToTemplateDatabase, pathToTestDatabase, std::filesystem::copy_options::overwrite_existing);
}

void
createTables ()
{
  soci::session sql (soci::sqlite3, pathToTestDatabase);
  confu_soci::createTableForStruct<Position> (sql);
  confu_soci::createTableForStruct<Character> (sql, { { "positionId", "Position", "id" }, { "accountId", "Account", "id" } });
  confu_soci::createTableForStruct<Account> (sql);
  confu_soci::createTableForStruct<BoardElement> (sql, { { "boardId", "Board", "id" } });
  confu_soci::createTableForStruct<Board> (sql, { { "gameId", "Game", "id" } });
  confu_soci::createTableForStruct<Game> (sql);
}

// todo find out why this cant be deinfed in cxx
// maybe its because of boost optional?
// maybe its because of circular inclusion?
// boost::optional<Account>

static boost::uuids::random_generator generator;

void
createAccount (std::string const &firstName, std::string const &lastName)
{
  soci::session sql (soci::sqlite3, pathToTestDatabase);
  auto uuid = boost::lexical_cast<std::string> (generator ());
  confu_soci::insertStruct (sql, Account{ .uuid = uuid, .firstName = firstName, .lastName = lastName });
  //   return confu_soci::findStruct<Account> (sql, "id", uuid);
}

} // namespace database