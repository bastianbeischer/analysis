#include "MainWindow.hh"
#include "PostAnalysisCanvas.hh"
#include "SignalHeightTrackerLikelihood.hh"
#include "SignalHeightTrackerFitPlot.hh"
#include "PDFParameters.hh"

#include <TROOT.h>
#include <TFile.h>
#include <TH2.h>

#include <QDebug>

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
    if (it.key() == Enums::NoParticle) {
      addCanvas(&file, "signal height pdf tracker all particles canvas");
    } else {
      QString title = "signal height pdf tracker " + it.value() + " canvas";
      PostAnalysisCanvas* canvas = addCanvas(&file, qPrintable(title));
      if (canvas) {
        SignalHeightTrackerLikelihood* lh = new SignalHeightTrackerLikelihood(it.key());
        m_likelihoods.append(lh);
        TH2D* h = canvas->histograms2D().at(0);
        for (int bin = 1; bin <= h->GetXaxis()->GetNbins(); ++bin) {
          SignalHeightTrackerFitPlot* plot = new SignalHeightTrackerFitPlot(lh, h, bin);
          connect(plot, SIGNAL(configFileChanged()), this, SLOT(configFileChanged()));
          m_fitPlots.append(plot);
        }
      }
    }
  }
  plotSelectionChanged();
}
