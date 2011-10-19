#include "MainWindow.hh"

#include "Likelihood.hh"
#include "TrackerMomentumLikelihood.hh"
#include "TimeOfFlightLikelihood.hh"
#include "LikelihoodPDFPlot.hh"

#include <QVector>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
  : PostAnalysisWindow(parent)
  , m_trackerLikelihood(new TrackerMomentumLikelihood)
  , m_tofLikelihood(new TimeOfFlightLikelihood)
{
  setupAnalysis();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupAnalysis()
{
  QVector<double> momenta = QVector<double>()
    << -100.0 << -10.0 << -7.5 << -5.0 << -4.0 << -3.0 << -2.0 << -1.5 << -1.0
    << 0.3 << 0.4 << 0.5 << 0.6 << 0.7 << 0.8 << 0.9
    << 1.0 << 1.5 << 2.0 << 3.0 << 4.0 << 5.0 << 7.5 << 10.0 << 100.0;
  foreach (double momentum, momenta)
    addPlot(new LikelihoodPDFPlot(m_trackerLikelihood, momentum));
  foreach (double momentum, momenta)
    addPlot(new LikelihoodPDFPlot(m_tofLikelihood, momentum));
}
