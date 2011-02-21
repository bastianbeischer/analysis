#include "DataChain.hh"
#include "SimpleEvent.hh"

#include "millepede.h"

#include <iostream>

#include <QtGlobal>

int main(int argc, char** argv)
{
  if (argc != 2)
    qFatal("Wrong number of arguments!");
  DataChain* chain = new DataChain;
  chain->addFileList(argv[1]);

  int entries = chain->nEntries();
  for (int it = 0; it < entries; ++it) {
    SimpleEvent* event = chain->event(it);
    event->eventTime();
    
    if (it % 10000 == 0)
      std::cerr << '.' << std::flush;
    //if (it == 20000) break;
  }
  std::cerr << std::endl;

  return 0;
}
