#ifndef E18680A5_3B06_4019_A849_6CDB82D14796
#define E18680A5_3B06_4019_A849_6CDB82D14796
#include "src/server/user.hxx"
#include <list>

void handleMessage (std::string const &msg, std::list<std::shared_ptr<User> > &users, std::shared_ptr<User> user);

#endif /* E18680A5_3B06_4019_A849_6CDB82D14796 */
