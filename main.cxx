#include "src/database/database.hxx"
#include <iostream>
int
main ()
{
  std::cout << "hello world1234!" << std::endl;
  createEmptyDatabase ();
  createTables ();
  return 0;
}