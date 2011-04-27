#include "MainWindow.hh"

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "RootStyle.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>
#include <TKey.h>

#include <QDebug>
#include <QFileDialog>

#include <iostream>

MainWindow::MainWindow(QWidget* parent)
  : PostAnalysisWindow(parent)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::setAnalysisFile(const QString& file)
{
  m_analysisFile = file;
}

void MainWindow::setMCFile(const QString& file)
{
  m_mcFile = file;
}

void MainWindow::setupAnalysis()
{
  TFile dataFile(qPrintable(m_analysisFile));
  TFile mcFile(qPrintable(m_mcFile));
  gROOT->cd();

  //for (int i = 0; i < mcFile.GetListOfKeys()->GetSize(); ++i)
  //  std::cout << ((TKey*)mcFile.GetListOfKeys()->At(i))->ReadObj()->GetName() << std::endl;

  PostAnalysisCanvas* dataCanvas = addCanvas(&dataFile, "complete TRD spectrum (-10 GeV to 10 Gev) canvas");
  PostAnalysisCanvas* mcCanvas = addCanvas(&mcFile, "complete TRD spectrum (-10 GeV to 10 Gev) canvas");

  qDebug() << "dataCanvas " << dataCanvas->name() << " got " << dataCanvas->histograms1D().size() << " 1D histos";
  qDebug() << "mcCanvas " << mcCanvas->name() << " got " << mcCanvas->histograms1D().size() << " 1D histos";

  //dataFile.Close();
  //mcFile.Close();
}

