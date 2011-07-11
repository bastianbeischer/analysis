#include "MainWindow.hh"
#include "RootStyle.hh"

#include <QApplication>
#include <QStringList>

int main(int argc, char* argv[])
{
  RootStyle::set();

  QApplication application(argc, argv);
  MainWindow window;
  if (application.arguments().size() > 1) {
    window.setupAnalysis(application.arguments());
    window.show();
    return application.exec();
  }
}
