#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "TrackingEfficiencyCorrection.hh"

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
  
  QString name = QString("Multi Layer Efficiency positive canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new TrackingEfficiencyCorrection(canvas));
  
  name = QString("Multi Layer Efficiency negative canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new TrackingEfficiencyCorrection(canvas));
  
  name = QString("Multi Layer Efficiency all canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new TrackingEfficiencyCorrection(canvas));
  
  file.Close();
}
