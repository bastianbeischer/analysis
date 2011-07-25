#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "AzimuthDistribution.hh"
#include "AzimuthMigration.hh"
#include "AzimuthUnfolding.hh"
#include "H1Plot.hh"
#include "H2Plot.hh"
#include "GrPlot.hh"

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
  TFile file(qPrintable(m_analysisFile));
  gROOT->cd();

  QString name = QString("Azimuth distribution canvas");
  canvas = addCanvas(&file, qPrintable(name));
  AzimuthDistribution* azimuthDistribution = new AzimuthDistribution(canvas);
  addPlot(azimuthDistribution);
  
  name = QString("Azimuth migration canvas");
  canvas = addCanvas(&file, qPrintable(name));
  AzimuthMigration* azimuthMigration = new AzimuthMigration(canvas);
  addPlot(azimuthMigration);
  
  AzimuthUnfolding* azimuthUnfolding= new AzimuthUnfolding(azimuthMigration->migrationHistogram(), azimuthDistribution->distribution());
  
  addPlot(new H2Plot(azimuthUnfolding->rohIj()));
  addPlot(new GrPlot(azimuthUnfolding->lCurve(), azimuthUnfolding->bestlcurve()));
  
  TH1D* unfoldedDistribution = new TH1D(*azimuthUnfolding->unfoldedHistogram());
  double integral = unfoldedDistribution->Integral("width");
  unfoldedDistribution->Scale(100/integral);
  
  addPlot(new H1Plot(2, unfoldedDistribution, azimuthDistribution->azimuthAcceptance()));

  file.Close();
}
