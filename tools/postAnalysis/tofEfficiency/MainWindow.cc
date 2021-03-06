#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "BarEfficiencyPlot.hh"
#include "MeanBarEfficiencyPlot.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>
#include <TH2.h>
#include <TF2.h>

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
  TFile file(qPrintable(m_analysisFiles.at(0)));
  gROOT->cd();
  for (int i = 0; i < 64; ++i) {
    QString title = QString("tof efficiency 0x%1 canvas").arg(0x8000 | i, 0, 16);
    addCanvas(&file, qPrintable(title));
  }
  file.Close();
  QVector<BarEfficiencyPlot*> barEfficiencies;
  for (int i = 0; i < 16; ++i) {
    BarEfficiencyPlot* barEfficiency = new BarEfficiencyPlot(
      m_canvases[4*i+0], m_canvases[4*i+1], m_canvases[4*i+2], m_canvases[4*i+3]);
    addPlot(barEfficiency);
    barEfficiencies.append(barEfficiency);
  }
  addPlot(new MeanBarEfficiencyPlot(barEfficiencies));
}

