#include "MainWindow.hh"
#include "PostAnalysisCanvas.hh"
#include "TimeOverThresholdLikelihood.hh"
#include "LikelihoodPdfFitPlot.hh"

#include <TROOT.h>
#include <TFile.h>
#include <TH2.h>

#include <QDebug>
#include <QString>

MainWindow::MainWindow(QWidget* parent)
  : LikelihoodFitWindow(parent)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupAnalysis()
{
  TFile file(qPrintable(m_analysisFiles.at(0)));
  gROOT->cd();
  Enums::ParticleIterator end = Enums::particleEnd();
  for (Enums::ParticleIterator it = Enums::particleBegin(); it != end; ++it) {
    QString title = QString("signal height pdf tof %1 canvas").arg(it.value());
    PostAnalysisCanvas* canvas = addCanvas(&file, qPrintable(title));
    if (canvas) {
      const TimeOverThresholdLikelihood* lh = new TimeOverThresholdLikelihood(it.key());
      TH2D* h = canvas->histograms2D().at(0);
      for (int bin = 1; bin <= h->GetXaxis()->GetNbins(); ++bin)
        addPlot(new LikelihoodPdfFitPlot(lh, h, bin));
    }
  }
}
