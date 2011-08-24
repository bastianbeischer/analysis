#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "RigidityMigrationPlot.hh"
#include "MeasurementTimePostAnalysisPlot.hh"
#include "SmearedRigidityComparissonPlot.hh"
#include "EfficiencyCorrectionSettings.hh"
#include "RigidityParticleSpectrum.hh"
#include "PostAnalysisGraphPlot.hh"
#include "RigidityMcSpectrum.hh"
#include "PostAnalysisH2DPlot.hh"
#include "PostAnalysisH1DPlot.hh"
#include "RigidityUnfolding.hh"
#include "RigidityFluxPlot.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TH1D.h>
#include <TROOT.h>

#include <cmath>

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
  addPlot(new PostAnalysisH2DPlot(rigidityMigrationPlot->createMigrationHistogramNormalizedToArea()));

  name = QString("Rigidity Mc Plot Generated canvas");
  PostAnalysisCanvas* genCanvas = addCanvas(file, qPrintable(name));
  name = QString("Rigidity Mc Plot Reconstructed canvas");
  PostAnalysisCanvas* recCanvas = addCanvas(file, qPrintable(name));
  RigidityMcSpectrum* rigidityMcSpectrum = new RigidityMcSpectrum(genCanvas, recCanvas);
  addPlot(new SmearedRigidityComparissonPlot(SmearedRigidityComparissonPlot::Positive, rigidityMcSpectrum->createGeneratedSpectrum(), rigidityMcSpectrum->createRecordedSpectrum()));
  addPlot(new SmearedRigidityComparissonPlot(SmearedRigidityComparissonPlot::Negative, rigidityMcSpectrum->createGeneratedSpectrum(), rigidityMcSpectrum->createRecordedSpectrum()));

  RigidityUnfolding* testUnfolding= new RigidityUnfolding(rigidityMigrationPlot->migrationHistogram(), rigidityMcSpectrum->createRecordedSpectrum());
  addPlot(new PostAnalysisH2DPlot(testUnfolding->rohIj()));
  addPlot(new PostAnalysisGraphPlot(QVector<TGraph*>() << testUnfolding->lCurve() << testUnfolding->bestlcurve()));
  addPlot(new SmearedRigidityComparissonPlot(SmearedRigidityComparissonPlot::Positive, rigidityMcSpectrum->createGeneratedSpectrum(), rigidityMcSpectrum->createRecordedSpectrum(), testUnfolding->unfoldedHistogram()));
  addPlot(new SmearedRigidityComparissonPlot(SmearedRigidityComparissonPlot::Negative, rigidityMcSpectrum->createGeneratedSpectrum(), rigidityMcSpectrum->createRecordedSpectrum(), testUnfolding->unfoldedHistogram()));

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
  
  TH1D* rawSpectrum = new TH1D(*particleSpectrum->spectrum());
  EfficiencyCorrectionSettings::BinQuantity quantity = EfficiencyCorrectionSettings::Raw;
  TH1D* multiLayerEff = EfficiencyCorrectionSettings::instance()->allTrackerLayerCutEfficiency(quantity);
  TH1D* trackFindingEff = EfficiencyCorrectionSettings::instance()->trackFindingEfficiency(quantity);
  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(rawSpectrum, multiLayerEff);
  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(rawSpectrum, trackFindingEff);
  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(rawSpectrum, 0.843684 / 0.792555);
  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(rawSpectrum, 0.999);//estimate for TOF trigger efficiency
  
  addPlot(new RigidityFluxPlot(canvas, rawSpectrum, measurementTime, RigidityFluxPlot::Positive));
  addPlot(new RigidityFluxPlot(canvas, rawSpectrum, measurementTime, RigidityFluxPlot::Negative));

  RigidityUnfolding* rigidityUnfolding= new RigidityUnfolding(rigidityMigrationPlot->migrationHistogram(), particleSpectrum->spectrum());
  addPlot(new PostAnalysisH2DPlot(rigidityUnfolding->rohIj()));
  addPlot(new PostAnalysisGraphPlot(QVector<TGraph*>() << rigidityUnfolding->lCurve() << rigidityUnfolding->bestlcurve()));
//  addPlot(new PostAnalysisH1DPlot(QVector<TH1D*>() << rigidityUnfolding->unfoldedHistogram() << particleSpectrum->spectrum()));
  
  TH1D* unfoldedSpectrum = new TH1D(*rigidityUnfolding->unfoldedHistogram());
  
  quantity = EfficiencyCorrectionSettings::Unfolded;
  delete multiLayerEff;
  delete trackFindingEff;
  multiLayerEff = EfficiencyCorrectionSettings::instance()->allTrackerLayerCutEfficiency(quantity);
  trackFindingEff = EfficiencyCorrectionSettings::instance()->trackFindingEfficiency(quantity);
  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(unfoldedSpectrum, multiLayerEff);
  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(unfoldedSpectrum, trackFindingEff);
  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(unfoldedSpectrum, 0.843684 / 0.792555);
  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(unfoldedSpectrum, 0.999);//estimate for TOF trigger efficiency

  addPlot(new RigidityFluxPlot(canvas, unfoldedSpectrum, measurementTime, RigidityFluxPlot::Positive));
  addPlot(new RigidityFluxPlot(canvas, unfoldedSpectrum, measurementTime, RigidityFluxPlot::Negative));

  name = QString("particle spectrum - albedo canvas");
  canvas = addCanvas(file, qPrintable(name));
  RigidityParticleSpectrum* particleSpectrumAlbedo = new RigidityParticleSpectrum(canvas);
  addPlot(particleSpectrumAlbedo);

  addPlot(new RigidityFluxPlot(canvas, particleSpectrumAlbedo->spectrum(), measurementTime, RigidityFluxPlot::Positive));
  addPlot(new RigidityFluxPlot(canvas, particleSpectrumAlbedo->spectrum(), measurementTime, RigidityFluxPlot::Negative));

  file->Close();
  delete file;
  file = 0;
}
