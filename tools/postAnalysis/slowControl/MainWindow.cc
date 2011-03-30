#include "MainWindow.hh"

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "TrackerTemperaturePlot.hh"
#include "TriggerRateHeightCorrelationPlot.hh"
#include "TriggerRateTimePlot.hh"
#include "HeightTimePlot.hh"
#include "TriggerRateHeightTimePlot.hh"

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
  TFile file(qPrintable(m_analysisFile));
  gROOT->cd();
  PostAnalysisCanvas* canvas = 0;
  canvas = addCanvas(&file, "trigger rate canvas");
  TriggerRateTimePlot* trtp = new TriggerRateTimePlot(canvas);
  addPlot(trtp);
  canvas = addCanvas(&file, "EBASS_HEIGHT canvas");
  HeightTimePlot* htp = new HeightTimePlot(canvas);
  addPlot(htp);
  addPlot(new TriggerRateHeightCorrelationPlot(trtp, htp));
  addPlot(new TriggerRateHeightTimePlot(trtp, htp));
  file.Close();
}
