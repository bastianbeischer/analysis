#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "SignalHeightProjectionCollection.hh"
#include "SignalHeightCorrelationCollection.hh"
#include "SignalHeightResiduePlot.hh"

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
  addPlot(new SignalHeightCorrelationCollection(SignalHeightCorrelationCollection::Time2D, &file));
  addPlot(new SignalHeightCorrelationCollection(SignalHeightCorrelationCollection::Time, &file));
  addPlot(new SignalHeightCorrelationCollection(SignalHeightCorrelationCollection::Temperature, &file));
  addPlot(new SignalHeightResiduePlot(SignalHeightResiduePlot::Time, &file));
  addPlot(new SignalHeightResiduePlot(SignalHeightResiduePlot::Temperature, &file));
  file.Close();
}
