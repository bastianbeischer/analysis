#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "RigidityMigrationPlot.hh"
#include "MeasurementTimePostAnalysisPlot.hh"
#include "SmearedRigidityComparissonPlot.hh"
#include "SmearedRigidityComparissonGraph.hh"
#include "EfficiencyCorrectionSettings.hh"
#include "RigidityParticleSpectrum.hh"
#include "PostAnalysisGraphPlot.hh"
#include "RigidityMcSpectrum.hh"
#include "PostAnalysisH2DPlot.hh"
#include "PostAnalysisH1DPlot.hh"
#include "RigidityUnfolding.hh"
#include "RigidityFluxPlot.hh"
#include "Enums.hh"

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
  name = QString("Rigidity Mc Plot Reconstructed100 canvas");
  PostAnalysisCanvas* recCanvas = addCanvas(file, qPrintable(name));
  name = QString("Rigidity Mc Plot Reconstructed075 canvas");
  PostAnalysisCanvas* recCanvasLow = addCanvas(file, qPrintable(name));
  name = QString("Rigidity Mc Plot Reconstructed125 canvas");
  PostAnalysisCanvas* recCanvasUp = addCanvas(file, qPrintable(name));

  RigidityMcSpectrum* rigidityMcSpectrum = new RigidityMcSpectrum(genCanvas, recCanvas, recCanvasLow, recCanvasUp);
  addPlot(new SmearedRigidityComparissonPlot(Enums::Positive, rigidityMcSpectrum->createGeneratedSpectrum(), rigidityMcSpectrum->createRecordedSpectrum()));
  addPlot(new SmearedRigidityComparissonPlot(Enums::Negative, rigidityMcSpectrum->createGeneratedSpectrum(), rigidityMcSpectrum->createRecordedSpectrum()));

  addPlot(new SmearedRigidityComparissonGraph(Enums::Positive, rigidityMcSpectrum->createGeneratedSpectrum(), rigidityMcSpectrum->createRecordedSpectrum(), rigidityMcSpectrum->createRecordedSpectrumLow(), rigidityMcSpectrum->createRecordedSpectrumUp()));
  addPlot(new SmearedRigidityComparissonGraph(Enums::Negative, rigidityMcSpectrum->createGeneratedSpectrum(), rigidityMcSpectrum->createRecordedSpectrum(), rigidityMcSpectrum->createRecordedSpectrumLow(), rigidityMcSpectrum->createRecordedSpectrumUp()));

//  RigidityUnfolding* testUnfolding= new RigidityUnfolding(rigidityMigrationPlot->migrationHistogram(), rigidityMcSpectrum->createRecordedSpectrum());
//  addPlot(new PostAnalysisH2DPlot(testUnfolding->rohIj()));
//  addPlot(new PostAnalysisGraphPlot(QVector<TGraph*>() << testUnfolding->lCurve() << testUnfolding->bestlcurve()));
//  addPlot(new SmearedRigidityComparissonPlot(Enums::Positive, rigidityMcSpectrum->createGeneratedSpectrum(), rigidityMcSpectrum->createRecordedSpectrum(), testUnfolding->unfoldedHistogram()));
//  addPlot(new SmearedRigidityComparissonPlot(Enums::Negative, rigidityMcSpectrum->createGeneratedSpectrum(), rigidityMcSpectrum->createRecordedSpectrum(), testUnfolding->unfoldedHistogram()));

  file->Close();
  delete file;
  file = 0;

  if (m_analysisFiles.size() == 1)
    return;

  file = new TFile(qPrintable(m_analysisFiles.at(1)));
  gROOT->cd();

  name = QString("measurement time distribution plot canvas");
  canvas = addCanvas(file, qPrintable(name));
  MeasurementTimePostAnalysisPlot* timePlot = new MeasurementTimePostAnalysisPlot(canvas);
  addPlot(timePlot);
  double measurementTime = timePlot->measurementTime();

  name = QString("particle spectrum - nonAlbedo canvas");
  canvas = addCanvas(file, qPrintable(name));
  RigidityParticleSpectrum* particleSpectrum = new RigidityParticleSpectrum(canvas);
  addPlot(particleSpectrum);

  TH1D* rawSpectrum = new TH1D(*particleSpectrum->spectrum());
  EfficiencyCorrectionSettings::FoldingType type = EfficiencyCorrectionSettings::Unfolded;
  TH1D* multiLayerEff = EfficiencyCorrectionSettings::instance()->allTrackerLayerCutEfficiency(type);
  TH1D* trackFindingEff = EfficiencyCorrectionSettings::instance()->trackFindingEfficiency(type);

  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(rawSpectrum, multiLayerEff);
  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(rawSpectrum, trackFindingEff);
  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(rawSpectrum, 0.999);//estimate for TOF trigger efficiency
  
  addPlot(new RigidityFluxPlot(canvas, rawSpectrum, measurementTime, Enums::Positive));
  addPlot(new RigidityFluxPlot(canvas, rawSpectrum, measurementTime, Enums::Negative));

  TH1D* unfoldBinWiseLow = rigidityMcSpectrum->createRecordedSpectrumLow();
  unfoldBinWiseLow->Divide(rigidityMcSpectrum->createGeneratedSpectrum());
  TH1D* rawSpectrumLow = new TH1D(*rawSpectrum);
  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(rawSpectrumLow, unfoldBinWiseLow);

  TH1D* unfoldBinWiseUp = rigidityMcSpectrum->createRecordedSpectrumUp();
  unfoldBinWiseUp->Divide(rigidityMcSpectrum->createGeneratedSpectrum());
  TH1D* rawSpectrumUp = new TH1D(*rawSpectrum);
  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(rawSpectrumUp, unfoldBinWiseUp);

  TH1D* unfoldBinWise = rigidityMcSpectrum->createRecordedSpectrum();
  unfoldBinWise->Divide(rigidityMcSpectrum->createGeneratedSpectrum());
  EfficiencyCorrectionSettings::instance()->efficiencyCorrection(rawSpectrum, unfoldBinWise);

  addPlot(new RigidityFluxPlot(canvas, rawSpectrum, measurementTime, Enums::Positive, rawSpectrumLow, rawSpectrumUp));
  addPlot(new RigidityFluxPlot(canvas, rawSpectrum, measurementTime, Enums::Negative, rawSpectrumLow, rawSpectrumUp));

  file->Close();
  delete file;
  file = 0;
}
