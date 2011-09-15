#include "MeasurementTimeDistributionPlot.hh"
#include "SimpleEvent.hh"
#include "Helpers.hh"

#include <TH1D.h>
#include <TLatex.h>
#include <TF1.h>

#include <QDebug>

#include <cmath>

MeasurementTimeDistributionPlot::MeasurementTimeDistributionPlot(int numberOfThreads)
  : AnalysisPlot(AnalysisPlot::MiscellaneousTOF)
  , H1DPlot()
  , m_lastEventTime(-1)
  , m_active(numberOfThreads == 1)
{
  setTitle("Measurement Time Distribution Plot");
  const int nBins = 5000;
  const double min = 50;
  const double max = 600000;
  QVector<double> binning = Helpers::logBinning(nBins, min, max);

  TH1D* histogram = new TH1D(qPrintable(title()), "", nBins, binning.constData());
  histogram->Sumw2();
  setAxisTitle("t_{cut} / ms", "measurement time / s");
  addHistogram(histogram, H1DPlot::P);
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
  if (!m_active)
    return;
  double eventTime = event->time() * 1000;
  if (m_lastEventTime > -1) {
    double deltaT = eventTime - m_lastEventTime;
    if (deltaT < 0)
      qDebug("time between events is smaller than 0");
    for (int i = 0; i < histogram()->GetNbinsX(); ++i) {
      double tCut = histogram()->GetBinCenter(i+1);
      if (deltaT < tCut) {
        double oldContent = histogram()->GetBinContent(i+1);
        double newContent = oldContent + deltaT / 1000.;
        histogram()->SetBinContent(i+1, newContent);
      }
    }
  }
  m_lastEventTime = eventTime;
}

void MeasurementTimeDistributionPlot::update()
{
}

void MeasurementTimeDistributionPlot::finalize()
{
}

