#include "src/database/database.hxx"
#include "src/logic/logic.hxx"
#include <boost/archive/basic_text_iarchive.hpp>
#include <boost/archive/basic_text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <catch2/catch.hpp>
#include <confu_soci/convenienceFunctionForSoci.hxx>
namespace test
{
// TODO write some tests

TEST_CASE ("handleMessage")
{
  database::createEmptyDatabase ();
  handleMessage ("");
}
TEST_CASE ("createAccount the test")
{
  database::createEmptyDatabase ();
  createAccount ("");
}
TEST_CASE ("createCharacter")
{
  database::createEmptyDatabase ();
  createCharacter ("");
}

SCENARIO ("create an account with createAccount", "[createAccount]")
{
  GIVEN ("empty database")
  {
    database::createEmptyDatabase ();
    database::createTables ();
    WHEN ("the account gets created")
    {
      auto accountAsString = createAccount ("|joe,doe").at (0);
      THEN ("account is in table and result from create account can be serialized into account object")
      {
        auto accountStringStream = std::stringstream{};
        accountStringStream << accountAsString;
        boost::archive::text_iarchive ia (accountStringStream);
        auto account = database::Account{};
        ia >> account;
        REQUIRE (account.firstName == "joe");
        soci::session sql (soci::sqlite3, pathToTestDatabase);
        REQUIRE (confu_soci::findStruct<database::Account> (sql, "firstName", "joe").has_value ());
      }
    }
  }
}

SCENARIO ("create an character with createCharacter", "[createCharacter]")
{
  GIVEN ("empty database")
  {
    database::createEmptyDatabase ();
    database::createTables ();
    soci::session sql (soci::sqlite3, pathToTestDatabase);
    confu_soci::insertStruct (sql, database::Account{ .id = "accountId", .firstName = "firstName", .lastName = "lastName" });
    WHEN ("the account gets created")
    {
      auto accountAsString = createCharacter ("|accountId").at (0);
      THEN ("Character is in table and result from create Character can be serialized into Character object")
      {
        auto characterStringStream = std::stringstream{};
        characterStringStream << accountAsString;
        boost::archive::text_iarchive ia (characterStringStream);
        auto character = database::Character{};
        ia >> character;
        REQUIRE (character.accountId == "accountId");
        REQUIRE (confu_soci::findStruct<database::Character> (sql, "accountId", "accountId").has_value ());
      }
    }
  }
}

}
