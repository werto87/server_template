#include "src/database/database.hxx"
#include "src/logic/logic.hxx"
#include <catch2/catch.hpp>
namespace test
{
// TODO write some tests

TEST_CASE ("discribe the test")
{
  database::createEmptyDatabase ();
  handleMessage ("");
}
TEST_CASE ("discribe the test")
{
  database::createEmptyDatabase ();
  createAccount ("");
}
TEST_CASE ("discribe the test")
{
  database::createEmptyDatabase ();
  createCharacter ("");
}

}
