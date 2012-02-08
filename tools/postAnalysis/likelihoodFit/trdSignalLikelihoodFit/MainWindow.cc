#include "MainWindow.hh"
#include "Enums.hh"
#include "Likelihood.hh"
#include "SignalHeightTrdLikelihood.hh"
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
  , m_layerSpinBox(new QSpinBox)
{
  m_layerSpinBox->setPrefix("layer: ");
  m_layerSpinBox->setRange(0, 7);
  connect(m_layerSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setupAnalysis()));
  addWidget(m_layerSpinBox);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupAnalysis()
{
  clear();
  TFile file(qPrintable(m_analysisFiles.at(0)));
  QStringList list = PostAnalysisCanvas::savedCanvases(&file, QRegExp("signal height pdf trd .* canvas"));
  gROOT->cd();
  foreach (const QString& canvasName, list) {
    QString particlesLabel = canvasName;
    particlesLabel.remove("signal height pdf trd ");
    particlesLabel.remove(QRegExp(" layer [0-9]"));
    particlesLabel.remove(" canvas");

    QString layerLabel = canvasName;
    layerLabel.remove("signal height pdf trd " + particlesLabel);
    layerLabel.remove(" layer ");
    layerLabel.remove(" canvas");

    QVector<Enums::Particle> particles = Enums::particleVector(particlesLabel);
    int layer = layerLabel.isEmpty() ? -1 : layerLabel.toInt();
    if (0 <= layer && layer != m_layerSpinBox->value())
      continue;

    PostAnalysisCanvas* canvas = addCanvas(&file, canvasName);
    TH2D* h = canvas->histograms2D().at(0);
    bool singleParticle = particles.count() == 1;
    foreach (Enums::Particle particle, particles) {
      QMap<Enums::Particle, Likelihood*>::Iterator lhIt = m_likelihoods.find(particle);
      if (lhIt == m_likelihoods.end()) {
        SignalHeightTrdLikelihood* lh = new SignalHeightTrdLikelihood(particle);
        if (particle == Enums::Electron || particle == Enums::Positron)
          lh->setLayer(m_layerSpinBox->value());
        lhIt = m_likelihoods.insert(particle, lh);
      }
      for (int bin = 1; bin <= h->GetXaxis()->GetNbins(); ++bin) {
        SignalHeightTrdFitPlot* plot = new SignalHeightTrdFitPlot(lhIt.value(), h, bin, singleParticle);
        connect(plot, SIGNAL(configFileChanged()), this, SLOT(configFileChanged()));
        if (singleParticle) {
          m_fitPlots.append(plot);
        } else {
          m_allParticleFitPlots.append(plot);
        }
      }
    }
  }

  foreach (Enums::Particle particle, Setup::instance()->proposedParticleVector()) {
    bool electron = particle == Enums::Electron || particle == Enums::Positron;
    int layer = m_layerSpinBox->value();
    QMap<Enums::Particle, Likelihood*>::Iterator lhIt = m_likelihoods.find(particle);
    if (lhIt == m_likelihoods.end())
      continue;
    SignalHeightTrdLikelihood* trdLikelihood = static_cast<SignalHeightTrdLikelihood*>(lhIt.value());
    if (electron)
      trdLikelihood->setLayer(layer);
    m_parameterPlots.append(new ParameterPlot(trdLikelihood, particle));
  }

  updateCanvasFilter();
  updatePlotFilter();

  plotSelectionChanged();
}
