#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "MultiLayerEfficiencyCorrection.hh"
#include "TrackFindingEfficiencyCorrection.hh"

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
  TFile file(qPrintable(m_analysisFile));
  gROOT->cd();
  qDebug("hallo");
  QString name = QString("Multi Layer Efficiency positive canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new MultiLayerEfficiencyCorrection(canvas));
  
  name = QString("Multi Layer Efficiency negative canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new MultiLayerEfficiencyCorrection(canvas));
  
  name = QString("Multi Layer Efficiency all canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new MultiLayerEfficiencyCorrection(canvas));
  
//  name = QString("Track finding efficiency - positive canvas");
//  canvas = addCanvas(&file, qPrintable(name));
//  addPlot(new TrackFindingEfficiencyCorrection(canvas));
//  
//  name = QString("Track finding efficiency - negative canvas");
//  canvas = addCanvas(&file, qPrintable(name));
//  addPlot(new TrackFindingEfficiencyCorrection(canvas));
//  
//  name = QString("Track finding efficiency - all canvas");
//  canvas = addCanvas(&file, qPrintable(name));
//  addPlot(new TrackFindingEfficiencyCorrection(canvas));
  
  file.Close();
}
