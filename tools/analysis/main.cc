#include "MainWindow.hh"
#include "RootStyle.hh"
#include "Analysis.hh"
#include "Plotter.hh"
#include "RootQtWidget.hh"

#include <QApplication>
#include <QStringList>

#include <TStyle.h>
#include <TH1.h>

int main(int argc, char* argv[])
{
  RootStyle::set();
  TH1::AddDirectory(false);
  QApplication application(argc, argv);
  QStringList arguments = application.arguments();
  arguments.removeFirst();
  bool batch = arguments.removeOne("--batch");
  Analysis analysis;
  analysis.processArguments(arguments);
  MainWindow window(&analysis, batch);
  return application.exec();
}
