#include "MainWindow.hh"
#include "Enums.hh"

#include "Likelihood.hh"
#include "TrackerMomentumLikelihood.hh"
#include "TimeOfFlightLikelihood.hh"
#include "SignalHeightTrdLikelihood.hh"
#include "LikelihoodPDFPlot.hh"
#include "Setup.hh"
#include "ParameterPlot.hh"

#include <QVector>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
  : PostAnalysisWindow(parent)
  , m_particles(Enums::NoParticle)
  , m_trackerLikelihood(0)
  , m_tofLikelihood(0)
  , m_signalHeightTrdLikelihoods()
{
  m_particles = Setup::instance()->proposedParticles();

  qDebug() << Enums::label(m_particles);

  m_trackerLikelihood = new TrackerMomentumLikelihood(m_particles);
  m_tofLikelihood = new TimeOfFlightLikelihood(m_particles);
  for (int layer = 0; layer < 8; ++layer) {
    SignalHeightTrdLikelihood* trdLikelihood = new SignalHeightTrdLikelihood(m_particles);
    trdLikelihood->setLayer(layer);
    m_signalHeightTrdLikelihoods.append(trdLikelihood);
  }
  setupAnalysis();
}

MainWindow::~MainWindow()
{
  delete m_trackerLikelihood;
  delete m_tofLikelihood;
  qDeleteAll(m_signalHeightTrdLikelihoods);
}

void MainWindow::setupAnalysis()
{
  QVector<double> momenta = QVector<double>()
    << -10.0 << -7.5 << -5.0 << -4.0 << -3.0 << -2.0 << -1.5 << -1.0
    << -0.9 << -0.8 << -0.7 << -0.6 << -0.5 << -0.4 << -0.3
    << 0.3 << 0.4 << 0.5 << 0.6 << 0.7 << 0.8 << 0.9
    << 1.0 << 1.5 << 2.0 << 3.0 << 4.0 << 5.0 << 7.5 << 10.0;

  // TRD
  foreach (double momentum, momenta)
    foreach (SignalHeightTrdLikelihood* trdLikelihood, m_signalHeightTrdLikelihoods)
      addPlot(new LikelihoodPDFPlot(trdLikelihood, momentum));
  foreach (SignalHeightTrdLikelihood* trdLikelihood, m_signalHeightTrdLikelihoods)
    foreach (Enums::Particle particle, Setup::instance()->proposedParticleVector())
      addPlot(new ParameterPlot(trdLikelihood, particle));

  // Tracker
  foreach (double momentum, momenta)
    addPlot(new LikelihoodPDFPlot(m_trackerLikelihood, momentum));
  foreach (Enums::Particle particle, Setup::instance()->proposedParticleVector())
    addPlot(new ParameterPlot(m_trackerLikelihood, particle));

  // TOF
  foreach (double momentum, momenta)
    addPlot(new LikelihoodPDFPlot(m_tofLikelihood, momentum));
  foreach (Enums::Particle particle, Setup::instance()->proposedParticleVector())
    addPlot(new ParameterPlot(m_tofLikelihood, particle));

}
