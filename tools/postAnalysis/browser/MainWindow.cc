#include "MainWindow.hh"

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>

MainWindow::MainWindow(QWidget* parent)
  : PostAnalysisWindow(parent)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupAnalysis()
{
  TFile file(qPrintable(m_analysisFiles.at(0)));
  gROOT->cd();
  TList* listOfKeys = file.GetListOfKeys();
  for (int i = 0; i < listOfKeys->GetSize(); ++i) {
    addCanvas(&file, listOfKeys->At(i)->GetName());
  }
}
