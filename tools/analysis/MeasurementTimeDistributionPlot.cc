#include "MeasurementTimeDistributionPlot.hh"
#include "SimpleEvent.hh"
#include "Helpers.hh"

#include <TH1D.h>
#include <TLatex.h>

#include <QDebug>

#include <cmath>

MeasurementTimeDistributionPlot::MeasurementTimeDistributionPlot(int numberOfThreads)
  : AnalysisPlot(Enums::FluxCalculation)
  , H1DPlot()
  , m_active(numberOfThreads == 1)
  , m_lastEventTime(-1)
  , m_positionInsideBin()
  , m_positionInsideBinCounter()
  , m_timeDifference(0)
{
  QString title = "measurement time distribution";
  setTitle(title);
  const int nBins = 60000;
  m_positionInsideBin.resize(nBins);
  m_positionInsideBinCounter.resize(nBins);
  const double min = 0.;
  const double max = 600.;
  m_timeDifference = new TH1D(qPrintable("dt " + title), "", nBins, min, max);

  TH1D* histogram = new TH1D(qPrintable(title), "", nBins, min, max);
  addHistogram(histogram);
  setAxisTitle("t_{cut} / s", "measurement time / s");

  histogram->GetYaxis()->SetTitleOffset(1.4);
}

MeasurementTimeDistributionPlot::~MeasurementTimeDistributionPlot()
{
}

void MeasurementTimeDistributionPlot::processEvent(const AnalyzedEvent* event)
{
  if (!m_active)
    return;
  double eventTime = event->simpleEvent()->time();
  if (-1 < m_lastEventTime) {
    double deltaT = eventTime - m_lastEventTime;
    if (deltaT < 0)
      qDebug("time between events is smaller than 0, check run list order!");
    m_timeDifference->Fill(deltaT);
    int bin = m_timeDifference->FindBin(deltaT);
    if (m_positionInsideBin.size() > bin - 1) {
      m_positionInsideBin[bin-1]+= deltaT;
      ++m_positionInsideBinCounter[bin-1];
    }
  }
  m_lastEventTime = eventTime;
}

void MeasurementTimeDistributionPlot::update()
{
  if (!m_active)
    return;
  double sum = 0;
  TH1D* h = histogram();
  for (int cutBin = 1; cutBin <= h->GetNbinsX(); ++cutBin) {
    double binContent = m_timeDifference->GetBinContent(cutBin);
    if (binContent > 0)
      sum+= m_timeDifference->GetBinContent(cutBin) * m_positionInsideBin[cutBin-1] / m_positionInsideBinCounter[cutBin-1];
    h->SetBinContent(cutBin, sum);
  }
}
