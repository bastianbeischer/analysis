#include "TOFPositionCorrelationPlot.hh"
#include "BrokenLine.hh"

#include "Particle.hh"
#include "Track.hh"
#include "TrackInformation.hh"
#include "Hit.hh"
#include "TOFCluster.hh"

#include <TH2.h>
#include <TAxis.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TLatex.h>

#include <QDebug>

TOFPositionCorrelationPlot::TOFPositionCorrelationPlot(unsigned short id)
  : AnalysisPlot(AnalysisPlot::MiscellaneousTOF)
  , H2DPlot()
  , m_id(id)
  , m_correlationGraph(new TGraphErrors)
{
  setTitle(QString("tof position correlation 0x%1").arg(id, 0, 16));
  TH2D* histogram = new TH2D(qPrintable(title()), "", 45, -450, 450, 90, -450, 450);
  setAxisTitle("y_{tracker} / mm", "y_{TOF} / mm", "");
  addHistogram(histogram);
  TF1* function = new TF1(qPrintable(QString("tof correlation Function %1").arg(id)), "pol1");
  function->SetRange(-450, 450);
  function->SetParameters(0, 1);
  addFunction(function);
  addLatex(RootPlot::newLatex(.15, .85));
  addLatex(RootPlot::newLatex(.15, .82));
}

TOFPositionCorrelationPlot::~TOFPositionCorrelationPlot()
{
  delete m_correlationGraph;
}

void TOFPositionCorrelationPlot::processEvent(const QVector<Hit*>& clusters, Particle* particle, SimpleEvent*)
{
  const Track* track = particle->track();

  // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;

  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;

  const QVector<Hit*>::const_iterator endIt = clusters.end();
  for (QVector<Hit*>::const_iterator it = clusters.begin(); it != endIt; ++it) {
    Hit* hit = *it;
    if (hit->type() == Hit::tof && (hit->detId()-hit->channel()) == m_id)
      histogram()->Fill(track->y(hit->position().z()), static_cast<TOFCluster*>(hit)->yEstimate(false));
  }
}

void TOFPositionCorrelationPlot::draw(TCanvas* canvas)
{
  H2DPlot::draw(canvas);
  if (m_correlationGraph->GetN()) {
    m_correlationGraph->Draw("SAME P");
  }
}

void TOFPositionCorrelationPlot::finalize()
{
  int nBins = xAxis()->GetNbins();
  for (int i = 0; i < nBins; ++i) {
    TF1 f("tofPositionCorreletionFitFunction", "gaus");
    TH1D* h = histogram()->ProjectionY("_py", i+1, i+1);
    if (h->GetEntries() > 10) {
      double x = xAxis()->GetBinCenter(i+1);
      double sigma = 100;
      f.SetParameters(h->GetMaximum(), x, sigma);
      f.SetRange(x - 5 * sigma, x + 5 * sigma);
      h->Fit(&f, "QN0R");
      if (0.5 < f.GetChisquare() / f.GetNDF() && f.GetChisquare() / f.GetNDF() < 5.) {
        int nPoints = m_correlationGraph->GetN();
        m_correlationGraph->SetPoint(nPoints, x, f.GetParameter(1));
        m_correlationGraph->SetPointError(nPoints, 0, f.GetParError(1));
      }
    }
    delete h;
  }
  m_correlationGraph->Fit(function(), "QN0");
  latex(0)->SetTitle(qPrintable(QString("b = %1 mm").arg(function()->GetParameter(0), 0, 'f', 2, ' ')));
  latex(1)->SetTitle(qPrintable(QString("m = %1").arg(function()->GetParameter(1), 0, 'f', 2, ' ')));
}
