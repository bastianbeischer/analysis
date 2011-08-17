#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "RigidityMigrationPlot.hh"
#include "MeasurementTimePostAnalysisPlot.hh"
#include "RigidityParticleSpectrum.hh"
#include "PostAnalysisGraphPlot.hh"
#include "PostAnalysisH2DPlot.hh"
#include "PostAnalysisH1DPlot.hh"
#include "RigidityUnfolding.hh"
#include "RigidityFluxPlot.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>

#include <QDebug>
#include <QVector>
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
  RigidityParticleSpectrum* particleSpectrum = new RigidityParticleSpectrum(canvas);
  addPlot(particleSpectrum);
  
  RigidityUnfolding* rigidityUnfolding= new RigidityUnfolding(rigidityMigrationPlot->migrationHistogram(), particleSpectrum->spectrum());
  addPlot(new PostAnalysisH2DPlot(rigidityUnfolding->rohIj()));
  addPlot(new PostAnalysisGraphPlot(QVector<TGraph*>() << rigidityUnfolding->lCurve() << rigidityUnfolding->bestlcurve()));
  addPlot(new PostAnalysisH1DPlot(rigidityUnfolding->unfoldedHistogram()));
  
  addPlot(new RigidityFluxPlot(canvas, particleSpectrum->spectrum(), measurementTime, RigidityFluxPlot::Positive));
  addPlot(new RigidityFluxPlot(canvas, particleSpectrum->spectrum(), measurementTime, RigidityFluxPlot::Negative));
  
//  addPlot(new RigidityFluxPlot(canvas, new TH1D(*rigidityUnfolding->unfoldedHistogram()), measurementTime, RigidityFluxPlot::Positive));
//  addPlot(new RigidityFluxPlot(canvas, rigidityUnfolding->unfoldedHistogram(), measurementTime, RigidityFluxPlot::Negative));
  
//  name = QString("particle spectrum - albedo canvas");
//  canvas = addCanvas(file, qPrintable(name));
//  RigidityParticleSpectrum* particleSpectrumAlbedo = new RigidityParticleSpectrum(canvas);
//  addPlot(particleSpectrumAlbedo);
//
//  addPlot(new RigidityFluxPlot(canvas, particleSpectrumAlbedo->spectrum(), measurementTime, RigidityFluxPlot::Positive));
//  addPlot(new RigidityFluxPlot(canvas, particleSpectrumAlbedo->spectrum(), measurementTime, RigidityFluxPlot::Negative));

  file->Close();
  delete file;
  file = 0;
}
