#include <DataManager.hh>

#include <iostream>
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QDebug>

int main(int argc, char** argv)
{
  if (argc < 2 || argc > 3) {
    std::cerr << "Wrong number of arguments:" << std::endl;
    std::cerr << "Usage: \"parser list.txt [output.root]\" - where list contains all the runs you want to parse" << std::endl;
    return -1;
  }

  QString temp(argv[1]);
  QString file = temp;
  QString output;
  if (argc == 3) {
    output = argv[2];
  }
  else {
    QString path = "";

    QStringList envVariables = QProcess::systemEnvironment();
    QStringList filteredVars = envVariables.filter(QRegExp("^PERDAIXANA_DATA=*"));
    if (filteredVars.size() != 0) {
      QString entry = filteredVars.first();
      path = entry.split("=").at(1);
      path += "/";
    }

    output = temp.replace(QRegExp("\\..*$"), ".root");
    output = path + (output.split(QRegExp("/"))).last();
  }

  DataManager dm;
  dm.setOutputFileName(output);

  QRegExp root(".*\\.dat$");
  if (root.exactMatch(file))
    dm.parseSingleRun(file);
  else
    dm.parseListOfRuns(file);

  return 0;
}
