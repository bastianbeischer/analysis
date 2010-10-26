#include "Manager.hh"

#include <QString>

#include <iostream>

int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cout << "Wrong number of arguments!" << std::endl;
    return -1;
  }
  QString fileName = argv[1];

  Manager* manager = Manager::instance();
  manager->loadStrategyFromFile(fileName);
  manager->startAlignment();

  return 0;
}
