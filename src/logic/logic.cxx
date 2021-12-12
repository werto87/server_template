#include "src/logic/logic.hxx"
#include <iostream>
void
handleMessage (std::string const &msg, std::list<std::shared_ptr<User> > &users, std::shared_ptr<User> user)
{
  std::cout << "please implement handle message" << std::endl;
  user->msgQueue.push_back ("please implement handle message");
}
