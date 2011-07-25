#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "AzimuthDistribution.hh"
#include "AzimuthMigration.hh"
#include "AzimuthUnfolding.hh"
#include "H1DPostAnalysisPlot.hh"
#include "H2DPostAnalysisPlot.hh"
#include "GraphPostAnalysisPlot.hh"

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
  
  QString name = QString("Azimuth migration canvas");
  canvas = addCanvas(file, qPrintable(name));
  AzimuthMigration* azimuthMigration = new AzimuthMigration(canvas);
  addPlot(azimuthMigration);
  
  file->Close();
  delete file;
  file = 0;
  
  file = new TFile(qPrintable(m_analysisFiles.at(1)));
  gROOT->cd();
  
  name = QString("Azimuth distribution canvas");
  canvas = addCanvas(file, qPrintable(name));
  AzimuthDistribution* azimuthDistribution = new AzimuthDistribution(canvas);
  addPlot(azimuthDistribution);
  
  file->Close();
  
  AzimuthUnfolding* azimuthUnfolding= new AzimuthUnfolding(azimuthMigration->migrationHistogram(), azimuthDistribution->distribution());
  
  addPlot(new H2DPostAnalysisPlot(azimuthUnfolding->rohIj()));
  addPlot(new GraphPostAnalysisPlot(azimuthUnfolding->lCurve(), azimuthUnfolding->bestlcurve()));
  
  TH1D* unfoldedDistribution = new TH1D(*azimuthUnfolding->unfoldedHistogram());
  double integral = unfoldedDistribution->Integral("width");
  unfoldedDistribution->Scale(100/integral);

  
  addPlot(new H1DPostAnalysisPlot(unfoldedDistribution, azimuthDistribution->distribution()));

  delete file;
  file = 0;
}
