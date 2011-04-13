#include <DataManager.hh>

#include <iostream>
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QDebug>

int main(int argc, char** argv)
{
  if (argc < 2 || argc > 3) {
    std::cerr << "Wrong number of arguments:" << std::endl;
    std::cerr << "Usage: \"parser list.txt [output.root]\" - where list contains all the runs you want to parse" << std::endl;
    return -1;
  }

  QString input(argv[1]);
  QString output = input;
  if (argc == 3) {
    output = argv[2];
  }
  else {
    QRegExp rx("\\..*$");
    rx.setMinimal(true);
    output = output.replace(rx, ".root");
  }

  DataManager dm;
  dm.setOutputFileName(output);

  QRegExp root(".*\\.dat$");
  if (root.exactMatch(input))
    dm.parseSingleRun(input);
  else
    dm.parseListOfRuns(input);

  return 0;
}
