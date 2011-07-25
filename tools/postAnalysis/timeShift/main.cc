#include "MainWindow.hh"
#include "RootStyle.hh"
#include "TimeShiftContainer.hh"

#include <QApplication>
#include <QStringList>

int main(int argc, char* argv[])
{
  if (argc != 2 && argc != 3) {
    qFatal("\nusage: timeShift postAnalysisFile.root [configFile.ini]\n");
  }
  RootStyle::set();

  QApplication application(argc, argv);
  if (application.arguments().count() == 3)
    TimeShiftContainer::instance()->setConfigFile(application.arguments().at(2));
  MainWindow window;
  window.addAnalysisFiles(QStringList(application.arguments().at(1)));
  window.show();
  return application.exec();
}
