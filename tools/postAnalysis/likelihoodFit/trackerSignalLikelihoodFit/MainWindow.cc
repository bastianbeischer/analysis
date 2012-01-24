#include "MainWindow.hh"
#include "PostAnalysisCanvas.hh"
#include "SignalHeightTrackerLikelihood.hh"
#include "SignalHeightTrackerFitPlot.hh"
#include "PDFParameters.hh"
#include "ParameterPlot.hh"
#include "Setup.hh"

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
  QStringList list = PostAnalysisCanvas::savedCanvases(&file, QRegExp("signal height pdf tracker .* canvas"));
  gROOT->cd();
  foreach (const QString& canvasName, list) {
    QString particlesLabel = canvasName;
    particlesLabel.remove("signal height pdf tracker ");
    particlesLabel.remove(" canvas");
    QVector<Enums::Particle> particles = Enums::particleVector(particlesLabel);
    PostAnalysisCanvas* canvas = addCanvas(&file, canvasName);
    TH2D* h = canvas->histograms2D().at(0);
    foreach (Enums::Particle particle, particles) {
      QMap<Enums::Particle, Likelihood*>::Iterator lhIt = m_likelihoods.find(particle);
      if (lhIt == m_likelihoods.end())
        lhIt = m_likelihoods.insert(particle, new SignalHeightTrackerLikelihood(particle));
      for (int bin = 1; bin <= h->GetXaxis()->GetNbins(); ++bin) {
        SignalHeightTrackerFitPlot* plot = new SignalHeightTrackerFitPlot(lhIt.value(), h, bin, particles.count() == 1);
        connect(plot, SIGNAL(configFileChanged()), this, SLOT(configFileChanged()));
        m_fitPlots.append(plot);
      }
    }
  }

  foreach (Enums::Particle particle, Setup::instance()->proposedParticleVector()) {
    QMap<Enums::Particle, Likelihood*>::ConstIterator lhIt = m_likelihoods.find(particle);
    if (lhIt != m_likelihoods.end())
      m_parameterPlots.append(new ParameterPlot(lhIt.value(), particle));
  }

  plotSelectionChanged();
}
