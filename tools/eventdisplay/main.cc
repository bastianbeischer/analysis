#include "Plotter.hh"
#include "MainWindow.hh"
#include "RootStyle.hh"

#include <QApplication>
#include <QStringList>

int main(int argc, char* argv[])
{
  RootStyle::set();

  QApplication application(argc, argv);
  MainWindow window;
  foreach(QString fileList, application.arguments()) {
    if (fileList != application.arguments().first())
      window.addFileList(fileList);
  }
  window.showMaximized();
  return application.exec();
}
