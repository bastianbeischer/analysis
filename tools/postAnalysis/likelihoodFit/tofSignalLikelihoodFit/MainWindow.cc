#include "MainWindow.hh"
#include "PostAnalysisCanvas.hh"
#include "TimeOverThresholdLikelihood.hh"
#include "TimeOverThresholdFitPlot.hh"
#include "PDFParameters.hh"
#include "Setup.hh"
#include "ParameterPlot.hh"

#include <TROOT.h>
#include <TFile.h>
#include <TH2.h>

#include <QDebug>
#include <QString>
#include <QComboBox>
#include <QRegExp>

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
  QStringList list = PostAnalysisCanvas::savedCanvases(&file, QRegExp("signal height pdf tof .* canvas"));
  gROOT->cd();
  foreach (const QString& canvasName, list) {
    QString particlesLabel = canvasName;
    particlesLabel.remove("signal height pdf tof ");
    particlesLabel.remove(" canvas");
    QVector<Enums::Particle> particles = Enums::particleVector(particlesLabel);
    PostAnalysisCanvas* canvas = addCanvas(&file, canvasName);
    TH2D* h = canvas->histograms2D().at(0);
    bool singleParticle = particles.count() == 1;
    foreach (Enums::Particle particle, particles) {
      QMap<Enums::Particle, Likelihood*>::Iterator lhIt = m_likelihoods.find(particle);
      if (lhIt == m_likelihoods.end())
        lhIt = m_likelihoods.insert(particle, new TimeOverThresholdLikelihood(particle));
      for (int bin = 1; bin <= h->GetXaxis()->GetNbins(); ++bin) {
        TimeOverThresholdFitPlot* plot = new TimeOverThresholdFitPlot(lhIt.value(), h, bin, singleParticle);
        connect(plot, SIGNAL(configFileChanged()), this, SLOT(configFileChanged()));
        if (singleParticle) {
          m_fitPlots.append(plot);
        } else {
          m_allParticleFitPlots.append(plot);
        }
      }
    }
  }
  foreach (Enums::Particle particle, m_particleVector) {
    QMap<Enums::Particle, Likelihood*>::ConstIterator lhIt = m_likelihoods.find(particle);
    if (lhIt != m_likelihoods.end())
      m_parameterPlots.append(new ParameterPlot(lhIt.value(), particle));
  }

  plotSelectionChanged();
}
