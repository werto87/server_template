#include "src/logic/logic.hxx"
#include <iostream>
void
handleMessage (std::string const &, std::list<std::shared_ptr<User> > &, std::shared_ptr<User> user)
{
  std::cout << "please implement handle message" << std::endl;
  user->sendMessageToUser ("please implement handle message");
}
