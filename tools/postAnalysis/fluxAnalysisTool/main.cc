#include "MainWindow.hh"
#include "RootStyle.hh"

#include <QApplication>
#include <QStringList>

int main(int argc, char* argv[])
{
  RootStyle::set();

  QApplication application(argc, argv);
  MainWindow window;
  if (application.arguments().count() == 2) {
    QStringList filenames = application.arguments();
    filenames.removeAt(0);
    window.showMaximized();
    window.addAnalysisFiles(filenames);
  }
  window.show();
  return application.exec();
}
