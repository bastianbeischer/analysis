#include "MainWindow.hh"
#include "PostAnalysisCanvas.hh"
#include "TimeOverThresholdLikelihood.hh"
#include "TimeOverThresholdFitPlot.hh"
#include "PDFParameters.hh"

#include <TROOT.h>
#include <TFile.h>
#include <TH2.h>

#include <QDebug>
#include <QString>

MainWindow::MainWindow(QWidget* parent)
  : LikelihoodFitWindow(parent)
  , m_likelihoods()
  , m_totFitPlots()
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::currentChanged()
{
  foreach (TimeOverThresholdFitPlot* plot, m_totFitPlots)
    plot->update();
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
      TimeOverThresholdLikelihood* lh = new TimeOverThresholdLikelihood(it.key());
      TH2D* h = canvas->histograms2D().at(0);
      for (int bin = 1; bin <= h->GetXaxis()->GetNbins(); ++bin) {
        TimeOverThresholdFitPlot* plot = new TimeOverThresholdFitPlot(lh, h, bin);
        m_totFitPlots.append(plot);
        connect(plot, SIGNAL(currentChanged()), this, SLOT(currentChanged()));
        addPlot(plot);
      }
    }
  }
}
