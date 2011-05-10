#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "MoPVScaling.hh"

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
  QString name = "complete TRD vs time spectrum (-10 GeV to 10 GeV) canvas";
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new MoPVScaling(canvas));
  file.Close();
}
