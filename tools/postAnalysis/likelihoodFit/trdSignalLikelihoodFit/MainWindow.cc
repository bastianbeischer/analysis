#include "MainWindow.hh"
#include "Enums.hh"

#include "Likelihood.hh"
#include "SignalHeightTrdLikelihood.hh"
#include "LikelihoodPDFPlot.hh"
#include "Setup.hh"
#include "ParameterPlot.hh"

#include <QVector>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
  : LikelihoodFitWindow(parent)
  , m_signalHeightTrdLikelihoods()
{
  for (int layer = 0; layer < 8; ++layer) {
    SignalHeightTrdLikelihood* trdLikelihood = new SignalHeightTrdLikelihood(m_particles);
    trdLikelihood->setLayer(layer);
    m_signalHeightTrdLikelihoods.append(trdLikelihood);
  }
  setupAnalysis();
}

MainWindow::~MainWindow()
{
  qDeleteAll(m_signalHeightTrdLikelihoods);
}

void MainWindow::setupAnalysis()
{
  QVector<double> momenta = QVector<double>()
    << -10.0 << -7.5 << -5.0 << -4.0 << -3.0 << -2.0 << -1.5 << -1.0
    << -0.9 << -0.8 << -0.7 << -0.6 << -0.5 << -0.4 << -0.3
    << 0.3 << 0.4 << 0.5 << 0.6 << 0.7 << 0.8 << 0.9
    << 1.0 << 1.5 << 2.0 << 3.0 << 4.0 << 5.0 << 7.5 << 10.0;

  foreach (double momentum, momenta)
    foreach (SignalHeightTrdLikelihood* trdLikelihood, m_signalHeightTrdLikelihoods)
      addPlot(new LikelihoodPDFPlot(trdLikelihood, momentum));
  foreach (SignalHeightTrdLikelihood* trdLikelihood, m_signalHeightTrdLikelihoods)
    foreach (Enums::Particle particle, Setup::instance()->proposedParticleVector())
      addPlot(new ParameterPlot(trdLikelihood, particle));
}
