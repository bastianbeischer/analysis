#include "MainWindow.hh"
#include "RootStyle.hh"

#include <QApplication>
#include <QStringList>

#include <TStyle.h>
#include <TH1.h>

int main(int argc, char* argv[])
{
  RootStyle::set();
  TH1::AddDirectory(false);

  QApplication application(argc, argv);
  MainWindow window;
  window.processArguments(application.arguments());
  window.showMaximized();
  return application.exec();
}
