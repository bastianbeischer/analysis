#include "MainWindow.hh"
#include "RootStyle.hh"

#include <QApplication>
#include <QStringList>

int main(int argc, char* argv[])
{
  RootStyle::set();

  QApplication application(argc, argv);
  MainWindow window;
  if (application.arguments().count() == 3) {
    window.setAnalysisFile(application.arguments().at(1));
    window.setMCFile(application.arguments().at(2));
    window.setupAnalysis();
  }
  window.show();
  return application.exec();
}
