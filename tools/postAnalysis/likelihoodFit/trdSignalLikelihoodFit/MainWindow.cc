#include "MainWindow.hh"
#include "Enums.hh"
#include "Likelihood.hh"
#include "SignalHeightTrdLikelihood.hh"
#include "LikelihoodPDFPlot.hh"
#include "Setup.hh"
#include "ParameterPlot.hh"
#include "SignalHeightTrdFitPlot.hh"
#include "PostAnalysisCanvas.hh"

#include <TFile.h>
#include <TROOT.h>
#include <TH2.h>

#include <QVector>
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
      addCanvas(&file, "signal height pdf trd all particles canvas");
    } else {
      int n = (it.key() == Enums::Electron || it.key() == Enums::Positron) ? 8 : 1;
      for (int layer = 0; layer < n; ++layer) {
        QString title = "signal height pdf trd ";
        if (n > 1)
          title+= QString("layer %1 ").arg(layer);
        title+= it.value() + " canvas";
        PostAnalysisCanvas* canvas = addCanvas(&file, qPrintable(title));
        if (canvas) {
          SignalHeightTrdLikelihood* lh = new SignalHeightTrdLikelihood(it.key());
          if (n > 1)
            lh->setLayer(layer);
          m_likelihoods.append(lh);
          TH2D* h = canvas->histograms2D().at(0);
          for (int bin = 1; bin <= h->GetXaxis()->GetNbins(); ++bin) {
            SignalHeightTrdFitPlot* plot = new SignalHeightTrdFitPlot(lh, h, bin);
            connect(plot, SIGNAL(configFileChanged()), this, SLOT(configFileChanged()));
            m_fitPlots.append(plot);
          }
        }
      }
    }
  }

  foreach (Enums::Particle particle, Setup::instance()->proposedParticleVector()) {
    int n = (particle == Enums::Electron || particle == Enums::Positron) ? 8 : 1;
    for (int layer = 0; layer < n; ++layer) {
      SignalHeightTrdLikelihood* trdLikelihood = new SignalHeightTrdLikelihood(particle);
      if (n > 1)
        trdLikelihood->setLayer(layer);
      m_otherPlots.append(new ParameterPlot(trdLikelihood, particle));
    }
  }

  plotSelectionChanged();
}
