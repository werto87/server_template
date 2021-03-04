#ifndef B86FE02F_B7D0_4435_9031_A334C305B294
#define B86FE02F_B7D0_4435_9031_A334C305B294

#include "confu_boost/confuBoost.hxx"
#include "confu_soci/convenienceFunctionForSoci.hxx"
#include "src/database/constant.hxx"
#include <boost/optional.hpp>
#include <filesystem>

BOOST_FUSION_DEFINE_STRUCT ((database), Character, (std::string, id) (long, positionX) (long, positionY) (long, positionZ) (std::string, accountId))
BOOST_FUSION_DEFINE_STRUCT ((database), Account, (std::string, id) (std::string, firstName) (std::string, lastName))
BOOST_FUSION_DEFINE_STRUCT ((database), BoardElement, (std::string, id) (std::string, boardId))
BOOST_FUSION_DEFINE_STRUCT ((database), Board, (std::string, id) (std::string, gameId))
BOOST_FUSION_DEFINE_STRUCT ((database), Game, (std::string, id))

BOOST_SERIALIZATION_BOILER_PLATE (database::Character)
BOOST_SERIALIZATION_BOILER_PLATE (database::Account)
BOOST_SERIALIZATION_BOILER_PLATE (database::BoardElement)
BOOST_SERIALIZATION_BOILER_PLATE (database::Board)
BOOST_SERIALIZATION_BOILER_PLATE (database::Game)

namespace database
{
void createEmptyDatabase ();
void createTables ();

boost::optional<database::Account> createAccount (std::string const &firstName, std::string const &lastName);
boost::optional<database::Character> createCharacter (std::string const &accoundId);
}

#endif /* B86FE02F_B7D0_4435_9031_A334C305B294 */
