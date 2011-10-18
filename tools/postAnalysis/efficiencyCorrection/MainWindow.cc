#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "AllTrackerLayersFlagEfficiencyCorrection.hh"
#include "EfficiencyCorrectionSettings.hh"
#include "TrackFindingEfficiencyCorrection.hh"
#include "Enums.hh"

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
  TFile file(qPrintable(m_analysisFiles.at(0)));
  gROOT->cd();

  EfficiencyCorrectionSettings::FoldingType type = EfficiencyCorrectionSettings::Raw;
  QString name = QString("All Tracker Layers Flag Efficiency - " + EfficiencyCorrectionSettings::instance()->foldingTypeName(type) + " canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new AllTrackerLayersFlagEfficiencyCorrection(Enums::Positive, canvas));
  addPlot(new AllTrackerLayersFlagEfficiencyCorrection(Enums::Negative, canvas));

  name = QString("Track finding efficiency - " + EfficiencyCorrectionSettings::instance()->foldingTypeName(type) + " canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new TrackFindingEfficiencyCorrection(Enums::Positive, canvas));
  addPlot(new TrackFindingEfficiencyCorrection(Enums::Negative, canvas));

  type = EfficiencyCorrectionSettings::Unfolded;
  name = QString("All Tracker Layers Flag Efficiency - " + EfficiencyCorrectionSettings::instance()->foldingTypeName(type) + " canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new AllTrackerLayersFlagEfficiencyCorrection(Enums::Positive, canvas));
  addPlot(new AllTrackerLayersFlagEfficiencyCorrection(Enums::Negative, canvas));

  name = QString("Track finding efficiency - " + EfficiencyCorrectionSettings::instance()->foldingTypeName(type) + " canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new TrackFindingEfficiencyCorrection(Enums::Positive, canvas));
  addPlot(new TrackFindingEfficiencyCorrection(Enums::Negative, canvas));

  file.Close();
}
