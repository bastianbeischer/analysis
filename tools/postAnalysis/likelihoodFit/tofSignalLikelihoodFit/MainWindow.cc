#include "MainWindow.hh"
#include "PostAnalysisCanvas.hh"
#include "TimeOverThresholdLikelihood.hh"
#include "LikelihoodPDFFitPlot.hh"

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
    QString particleLabel = (it.key() == Enums::NoParticle) ? "all particles" : it.value();
    QString title = "signal height pdf tof " + particleLabel + " canvas";
    PostAnalysisCanvas* canvas = addCanvas(&file, qPrintable(title));
    if (canvas) {
      const TimeOverThresholdLikelihood* lh = new TimeOverThresholdLikelihood(it.key());
      TH2D* h = canvas->histograms2D().at(0);
      for (int bin = 1; bin <= h->GetXaxis()->GetNbins(); ++bin)
        addPlot(new LikelihoodPDFFitPlot(lh, h, bin));
    }
  }
}
