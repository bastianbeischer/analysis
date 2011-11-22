#include "MeasurementTimeDistributionPlot.hh"
#include "SimpleEvent.hh"
#include "Helpers.hh"

#include <TH1D.h>
#include <TLatex.h>

#include <QDebug>

#include <cmath>

MeasurementTimeDistributionPlot::MeasurementTimeDistributionPlot(int numberOfThreads)
  : AnalysisPlot(Enums::MiscellaneousTOF)
  , H1DPlot()
  , m_calculation(new MeasurementTimeCalculation(numberOfThreads))
{
  setTitle("measurement time distribution plot");
  TH1D* histogram = m_calculation->measurementTimeDistribution();
  addHistogram(histogram);
  setAxisTitle(histogram->GetXaxis()->GetTitle(), histogram->GetYaxis()->GetTitle());
  if (!m_calculation->isActive()) {
    const int prevNumberOfLatexs = numberOfLatexs();
    addLatex(RootPlot::newLatex(.2, .55));
    latex(prevNumberOfLatexs)->SetTextColor(kRed);
    latex(prevNumberOfLatexs)->SetTitle("This plot has to be filled by only one thread.");
  }
  histogram->GetYaxis()->SetTitleOffset(1.4);
  addLatex(RootPlot::newLatex(.2, .55));
}

MeasurementTimeDistributionPlot::~MeasurementTimeDistributionPlot()
{
  delete m_calculation;
}

void MeasurementTimeDistributionPlot::processEvent(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const event)
{
  m_calculation->update(event);
}

void MeasurementTimeDistributionPlot::update()
{
  if (m_calculation->isActive())
    latex()->SetTitle(qPrintable(QString("effective measurement time = %1 s").arg(m_calculation->measurementTime())));
}
