#include "MainWindow.hh"
#include "RootStyle.hh"
#include "Analysis.hh"

#include <QApplication>
#include <QStringList>

#include <TStyle.h>
#include <TH1.h>

int main(int argc, char* argv[])
{
  RootStyle::set();
  TH1::AddDirectory(false);

  QApplication application(argc, argv);
  Analysis analysis;
  analysis.processArguments(application.arguments());
  MainWindow window(&analysis);
  window.showMaximized();
  return application.exec();
}
