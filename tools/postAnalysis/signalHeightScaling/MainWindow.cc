#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "SignalHeightProjectionCollection.hh"
#include "SignalHeightTempCorCollection.hh"
#include "SignalHeightTimeCorCollection.hh"
#include "SignalHeightTimeCor2DCollection.hh"

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
  TFile file(qPrintable(m_analysisFiles.at(0)));
  gROOT->cd();
  canvas = addCanvas(&file, "signal height 2d canvas");
  addPlot(new SignalHeightProjectionCollection(canvas));
  addPlot(new SignalHeightTempCorCollection(&file));
  addPlot(new SignalHeightTimeCor2DCollection(&file));
  addPlot(new SignalHeightTimeCorCollection(&file));
  file.Close();
}
