#include "MainWindow.hh"

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>

MainWindow::MainWindow(QWidget* parent)
  : PostAnalysisWindow(parent)
  , m_file(0)
{
}

MainWindow::~MainWindow()
{
  m_file->Close();
  delete m_file;
}

void MainWindow::setupAnalysis()
{
  m_file = new TFile(qPrintable(m_analysisFiles.at(0)));
  QStringList list = PostAnalysisCanvas::savedCanvases(m_file);
  list.sort();
  gROOT->cd();
  foreach (const QString& name, list)
    addCanvas(new PostAnalysisCanvas(m_file, name));
}
