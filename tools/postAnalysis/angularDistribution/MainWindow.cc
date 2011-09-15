#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "AzimuthDistribution.hh"
#include "ZenithDistribution.hh"
#include "PostAnalysisH1DPlot.hh"
#include "PostAnalysisH2DPlot.hh"
#include "PostAnalysisGraphPlot.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>

#include <QDebug>
#include <QFileDialog>
#include <QString>

MainWindow::MainWindow(QWidget* parent)
  : PostAnalysisWindow(parent)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupAnalysis()
{
  PostAnalysisCanvas* canvas = 0;

  TFile* file = new TFile(qPrintable(m_analysisFiles.at(0)));
  gROOT->cd();
  QString name = QString("Azimuth distribution canvas");
  canvas = addCanvas(file, qPrintable(name));
  AzimuthDistribution* azimuthDistribution = new AzimuthDistribution(canvas);
  addPlot(azimuthDistribution);
  file->Close();
  delete file;

  file = new TFile(qPrintable(m_analysisFiles.at(0)));
  gROOT->cd();
  name = QString("Zenith distribution canvas");
  canvas = addCanvas(file, qPrintable(name));
  ZenithDistribution* zenithDistribution = new ZenithDistribution(canvas);
  addPlot(zenithDistribution);
  file->Close();
  delete file;
}
