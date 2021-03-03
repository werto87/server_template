#include "src/database/database.hxx"
#include "src/database/constant.hxx"
#include <boost/lexical_cast.hpp>
#include <boost/optional/optional.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <confu_soci/convenienceFunctionForSoci.hxx>
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
  confu_soci::createTableForStruct<Character> (sql, { { "accountId", "Account", "id" } });
  confu_soci::createTableForStruct<Account> (sql);
  confu_soci::createTableForStruct<BoardElement> (sql, { { "boardId", "Board", "id" } });
  confu_soci::createTableForStruct<Board> (sql, { { "gameId", "Game", "id" } });
  confu_soci::createTableForStruct<Game> (sql);
}

boost::optional<Account>
createAccount (std::string const &firstName, std::string const &lastName)
{
  soci::session sql (soci::sqlite3, pathToTestDatabase);
  return confu_soci::findStruct<Account> (sql, "id", confu_soci::insertStruct (sql, Account{ .id = {}, .firstName = firstName, .lastName = lastName }, true, true));
}

boost::optional<database::Character>
createCharacter (std::string const &accoundId)
{
  soci::session sql (soci::sqlite3, pathToTestDatabase);
  return confu_soci::findStruct<Character> (sql, "id", confu_soci::insertStruct (sql, Character{ .id = {}, .positionX = {}, .positionY = {}, .positionZ = {}, .accountId = accoundId }, true, true));
}

} // namespace database