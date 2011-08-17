#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "RigidityMigrationPlot.hh"
#include "MeasurementTimePostAnalysisPlot.hh"
#include "RigidityFluxPlot.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>

#include <QDebug>
#include <QFileDialog>

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

  QString name = QString("Rigidity migration canvas");
  canvas = addCanvas(file, qPrintable(name));
  RigidityMigrationPlot* rigidityMigrationPlot = new RigidityMigrationPlot(canvas);
  addPlot(rigidityMigrationPlot);

  file->Close();
  delete file;
  file = 0;

  file = new TFile(qPrintable(m_analysisFiles.at(1)));
  gROOT->cd();

  name = QString("measurement time canvas");
  canvas = addCanvas(file, qPrintable(name));
  MeasurementTimePostAnalysisPlot* timePlot = new MeasurementTimePostAnalysisPlot(canvas);
  addPlot(timePlot);
  double measurementTime = timePlot->measurementTime();

  name = QString("particle spectrum - nonAlbedo canvas");
  canvas = addCanvas(file, qPrintable(name));
  addPlot(new RigidityFluxPlot(canvas, measurementTime, RigidityFluxPlot::Positive));

  name = QString("particle spectrum - nonAlbedo canvas");
  canvas = addCanvas(file, qPrintable(name));
  addPlot(new RigidityFluxPlot(canvas, measurementTime, RigidityFluxPlot::Negative));

  name = QString("particle spectrum - albedo canvas");
  canvas = addCanvas(file, qPrintable(name));
  addPlot(new RigidityFluxPlot(canvas, measurementTime, RigidityFluxPlot::Positive));

  name = QString("particle spectrum - albedo canvas");
  canvas = addCanvas(file, qPrintable(name));
  addPlot(new RigidityFluxPlot(canvas, measurementTime, RigidityFluxPlot::Negative));

  file->Close();
  delete file;
  file = 0;
}


//  AzimuthUnfolding* azimuthUnfolding= new AzimuthUnfolding(azimuthMigration->migrationHistogram(), azimuthDistribution->distribution());
//  
//  addPlot(new H2DPostAnalysisPlot(azimuthUnfolding->rohIj()));
//  addPlot(new GraphPostAnalysisPlot(azimuthUnfolding->lCurve(), azimuthUnfolding->bestlcurve()));
//  
//  TH1D* unfoldedDistribution = new TH1D(*azimuthUnfolding->unfoldedHistogram());
//  double integral = unfoldedDistribution->Integral("width");
//  unfoldedDistribution->Scale(100/integral);
//
//  
//  addPlot(new H1DPostAnalysisPlot(unfoldedDistribution, azimuthDistribution->distribution()));