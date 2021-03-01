#include "src/database/database.hxx"
#include "src/database/constant.hxx"
#include <boost/lexical_cast.hpp>
#include <boost/optional/optional.hpp>
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

static boost::uuids::random_generator generator;

boost::optional<Account>
// void
createAccount (std::string const &firstName, std::string const &lastName)
{
  auto test = boost::optional<Account>{};
  soci::session sql (soci::sqlite3, pathToTestDatabase);
  auto uuid = boost::lexical_cast<std::string> (generator ());
  confu_soci::insertStruct (sql, Account{ .uuid = uuid, .firstName = firstName, .lastName = lastName });
  confu_soci::findStruct<Account> (sql, "id", uuid);
  return test;
}

} // namespace database