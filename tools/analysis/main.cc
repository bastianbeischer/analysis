#include "Plotter.hh"
#include "MainWindow.hh"

#include <QApplication>
#include <QStringList>

#include <TROOT.h>
#include <TStyle.h>

int main(int argc, char* argv[])
{
  gStyle->SetFrameFillColor(0);
  gStyle->SetPadColor(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetTitleFillColor(0);
  gStyle->SetTitleBorderSize(1);
  gStyle->SetStatColor(0);
  gStyle->SetStatBorderSize(1);
  gStyle->SetLegendBorderSize(1);

  gStyle->SetPalette(1, 0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(111);
  gStyle->SetLineScalePS(1);
  gStyle->SetHistLineWidth(2);
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
