#include "MainWindow.hh"

#include "Likelihood.hh"
#include "TimeOfFlightLikelihood.hh"
#include "LikelihoodPDFPlot.hh"
#include "LikelihoodRatioPlot.hh"

#include <QVector>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
  : PostAnalysisWindow(parent)
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
    << 0.5 << 0.6 << 0.7 << 0.8 << 0.9 << 1.0 << 1.2 << 1.4 << 1.6 << 1.8 << 2 << 3 << 4 << 5 << 100;
  foreach (double momentum, momenta)
    addPlot(new LikelihoodPDFPlot(m_tofLikelihood, momentum));
  foreach (double momentum, momenta)
    addPlot(new LikelihoodRatioPlot(m_tofLikelihood, momentum));
}
