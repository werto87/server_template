#ifndef B86FE02F_B7D0_4435_9031_A334C305B294
#define B86FE02F_B7D0_4435_9031_A334C305B294

#include "confu_boost/confuBoost.hxx"
#include "confu_soci/convenienceFunctionForSoci.hxx"
#include "src/database/constant.hxx"
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <filesystem>

BOOST_FUSION_DEFINE_STRUCT ((database), Character, (std::string, id) (std::string, positionId) (std::string, accountId))
BOOST_FUSION_DEFINE_STRUCT ((database), Account, (std::string, id) (std::string, firstName) (std::string, lastName))
BOOST_FUSION_DEFINE_STRUCT ((database), BoardElement, (std::string, id) (std::string, boardId))
BOOST_FUSION_DEFINE_STRUCT ((database), Board, (std::string, id) (std::string, gameId))
BOOST_FUSION_DEFINE_STRUCT ((database), Game, (std::string, id))
BOOST_FUSION_DEFINE_STRUCT ((database), Position, (std::string, id) (unsigned long, x) (unsigned long, y)) // x and y should be made in a class

BOOST_SERIALIZATION_BOILER_PLATE (database::Character)
BOOST_SERIALIZATION_BOILER_PLATE (database::Account)
BOOST_SERIALIZATION_BOILER_PLATE (database::BoardElement)
BOOST_SERIALIZATION_BOILER_PLATE (database::Board)
BOOST_SERIALIZATION_BOILER_PLATE (database::Game)
BOOST_SERIALIZATION_BOILER_PLATE (database::Position)

namespace database
{
void createEmptyDatabase ();
void createTables ();

boost::optional<database::Account> createAccount (std::string const &firstName, std::string const &lastName);
boost::optional<database::Character> createCharacter (std::string const &accoundId);
}

#endif /* B86FE02F_B7D0_4435_9031_A334C305B294 */
