#include "MainWindow.hh"
#include "RootStyle.hh"

#include <QApplication>
#include <QStringList>

int main(int argc, char* argv[])
{
  RootStyle::set();
  QApplication application(argc, argv);
  Q_ASSERT(application.arguments().count() == 2);
  MainWindow window;
  window.setRunFile(application.arguments().at(1));
  window.showMaximized();
  return application.exec();
}
