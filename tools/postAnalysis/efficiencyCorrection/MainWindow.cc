#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "AllTrackerLayersFlagEfficiencyCorrection.hh"
#include "TrackFindingEfficiencyCorrection.hh"

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
  
  QString name = QString("One hit in all layers canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new AllTrackerLayersFlagEfficiencyCorrection(AllTrackerLayersFlagEfficiencyCorrection::Positive, canvas));
  
  name = QString("One hit in all layers canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new AllTrackerLayersFlagEfficiencyCorrection(AllTrackerLayersFlagEfficiencyCorrection::Negative, canvas));
  
  name = QString("Track finding efficiency canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new TrackFindingEfficiencyCorrection(TrackFindingEfficiencyCorrection::Positive, canvas));
  
  name = QString("Track finding efficiency canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new TrackFindingEfficiencyCorrection(TrackFindingEfficiencyCorrection::Negative, canvas));

  file.Close();
}
