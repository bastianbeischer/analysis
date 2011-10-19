#include "MeasurementTimeDistributionPlot.hh"
#include "SimpleEvent.hh"
#include "Helpers.hh"

#include <TH1D.h>
#include <TLatex.h>

#include <QDebug>

#include <cmath>

MeasurementTimeDistributionPlot::MeasurementTimeDistributionPlot(int numberOfThreads)
  : AnalysisPlot(Enums::MomentumReconstruction)
  , H1DPlot()
  , m_active(numberOfThreads == 1)
  , m_calculation(0)
{
  setTitle("measurement time distribution plot");
  TH1D* histogram = m_calculation->measurementTimeDistribution();
  addHistogram(histogram);
  setAxisTitle(histogram->GetXaxis()->GetTitle(), histogram->GetYaxis()->GetTitle());
  if (!m_active) {
    const int prevNumberOfLatexs = numberOfLatexs();
    addLatex(RootPlot::newLatex(.2, .55));
    latex(prevNumberOfLatexs)->SetTextColor(kRed);
    latex(prevNumberOfLatexs)->SetTitle("This plot has to be filled by only one thread.");
  }
  histogram->GetYaxis()->SetTitleOffset(1.4);
}

MeasurementTimeDistributionPlot::~MeasurementTimeDistributionPlot()
{
}

void MeasurementTimeDistributionPlot::processEvent(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const event)
{
  m_calculation->update(event);
}

void MeasurementTimeDistributionPlot::update()
{
  m_calculation->measurementTime();
}
