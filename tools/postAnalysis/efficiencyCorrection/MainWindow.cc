#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "OneHitAllLayersEfficiencyCorrection.hh"
#include "TrackFindingEfficiencyCorrection.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>

#include <QDebug>
#include <QFileDialog>
#include <qstring.h>

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

  QString config = "kirunaFlight";
  
  QString name = QString("One hit in all layers positive canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new OneHitAllLayersEfficiencyCorrection(canvas, config));
  
  name = QString("One hit in all layers negative canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new OneHitAllLayersEfficiencyCorrection(canvas, config));
  
  name = QString("One hit in all layers all canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new OneHitAllLayersEfficiencyCorrection(canvas, config));
  
  name = QString("Track finding efficiency - positive canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new TrackFindingEfficiencyCorrection(canvas, config));
  
  name = QString("Track finding efficiency - negative canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new TrackFindingEfficiencyCorrection(canvas, config));
  
  name = QString("Track finding efficiency - all canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new TrackFindingEfficiencyCorrection(canvas, config));
  
  file.Close();
}
