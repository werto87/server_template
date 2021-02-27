#ifndef B86FE02F_B7D0_4435_9031_A334C305B294
#define B86FE02F_B7D0_4435_9031_A334C305B294
#include "confu_soci/convenienceFunctionForSoci.hxx"

BOOST_FUSION_DEFINE_STRUCT ((), Character, (std::string, id) (std::string, positionId) (std::string, accountId))
BOOST_FUSION_DEFINE_STRUCT ((), Account, (std::string, id))
BOOST_FUSION_DEFINE_STRUCT ((), BoardElement, (std::string, id) (std::string, boardId))
BOOST_FUSION_DEFINE_STRUCT ((), Board, (std::string, id) (std::string, gameId))
BOOST_FUSION_DEFINE_STRUCT ((), Game, (std::string, id))
BOOST_FUSION_DEFINE_STRUCT ((), Position, (std::string, id) (unsigned long, x) (unsigned long, y)) // x and y should be made in a class

void createEmptyDatabase ();
void createTables ();

#endif /* B86FE02F_B7D0_4435_9031_A334C305B294 */
