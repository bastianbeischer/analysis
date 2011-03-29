#include "MainWindow.hh"

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "TrackerTemperaturePlot.hh"
#include "TriggerRateHeightCorrelationPlot.hh"
#include "TriggerRateTimePlot.hh"

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
  PostAnalysisCanvas* triggerRateCanvas = addCanvas(&file, "TRIGGER_RATE canvas");
  addPlot(new TriggerRateTimePlot(triggerRateCanvas));
  //addPlot(new TriggerRateTimePlot(canvas));
  file.Close();
}
