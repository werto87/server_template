#include "src/database/database.hxx"
#include "src/database/constant.hxx"
#include <filesystem>

void
createEmptyDatabase ()
{
  std::filesystem::create_directory (std::filesystem::path{ pathToTestDatabase }.parent_path ());
  std::filesystem::copy_file (pathToTemplateDatabase, pathToTestDatabase, std::filesystem::copy_options::overwrite_existing);
}

void
createTables ()
{
  auto &sql = connectToDatabase ();
  confu_soci::createTableForStruct<Position> (sql);
  confu_soci::createTableForStruct<Character> (sql, { { "positionId", "Position", "id" }, { "accountId", "Account", "id" } });
  confu_soci::createTableForStruct<Account> (sql);
  confu_soci::createTableForStruct<BoardElement> (sql, { { "boardId", "Board", "id" } });
  confu_soci::createTableForStruct<Board> (sql, { { "gameId", "Game", "id" } });
  confu_soci::createTableForStruct<Game> (sql);
  confu_soci::insertStruct (sql, Account{ "account1" });
  confu_soci::insertStruct (sql, Position{ "position1", {}, {} });
  confu_soci::insertStruct (sql, Character{ "character1", "position1", "account1" });
}

soci::session &
connectToDatabase ()
{
  static soci::session sql (soci::sqlite3, pathToTestDatabase);
  sql << "PRAGMA foreign_keys = ON;";
  return sql;
}
