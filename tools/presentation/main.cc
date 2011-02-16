#include "MainWindow.hh"

#include <QApplication>
#include <QStringList>

int main(int argc, char* argv[])
{
  QApplication application(argc, argv);
  MainWindow window;
  if (application.arguments().count() == 2) {
    //window.setAnalysisFile(application.arguments().at(1));
  }
  window.show();
  return application.exec();
}
