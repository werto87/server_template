#include "src/logic/logic.hxx"
#include <iostream>
void
handleMessage (std::string const &, std::list<std::shared_ptr<User> > &, std::shared_ptr<User> user)
{
  std::cout << "please implement handle message" << std::endl;
  user->msgQueue.push_back ("please implement handle message");
  user->timer->cancel ();
}
