#include "Plotter.hh"
#include "MainWindow.hh"

#include <QApplication>
#include <QStringList>

#include <TROOT.h>
#include <TStyle.h>

int main(int argc, char* argv[])
{
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1, 0);
  gStyle->SetOptFit(111);
  gStyle->SetLineScalePS(1);
  gStyle->SetPadRightMargin(0.16);
  gStyle->SetLabelFont(42, "XYZ");
  gStyle->SetLabelSize(0.03, "XYZ");
  gStyle->SetStatFont(42);
  gStyle->SetStatFontSize(0.02);
  gStyle->SetTextFont(42);
  gStyle->SetTitleFont(42, "XYZ");
  gStyle->SetTitleSize(0.03, "XYZ");

  QApplication application(argc, argv);
  MainWindow window;
  foreach(QString fileList, application.arguments()) {
    if (fileList != application.arguments().first())
      window.addFileList(fileList);
  }
  window.showMaximized();
  return application.exec();
}
