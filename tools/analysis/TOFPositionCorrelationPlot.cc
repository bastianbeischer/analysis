#include "TOFPositionCorrelationPlot.hh"
#include "BrokenLine.hh"

#include "Hit.hh"
#include "TOFCluster.hh"

#include <TH2.h>
#include <TAxis.h>
#include <TGraphErrors.h>
#include <TF1.h>

#include <QDebug>

TOFPositionCorrelationPlot::TOFPositionCorrelationPlot(unsigned short id)
  : H2DPlot(AnalysisPlot::MiscellaneousTOF)
  , m_id(id)
  , m_correlationGraph(new TGraphErrors)
  , m_correlationFunction(new TF1(qPrintable(QString("tof correlation Function %1").arg(id)), "pol1"))
{
  setTitle(QString("tof position correlation 0x%1").arg(id, 0, 16));
  TH2D* histogram = new TH2D(qPrintable(title()), "", 45, -450, 450, 90, -450, 450);
  histogram->GetXaxis()->SetTitle("y_{tracker} / mm");
  histogram->GetYaxis()->SetTitle("y_{TOF} / mm");
  setHistogram(histogram);
  m_correlationFunction->SetRange(-450, 450);
  m_correlationFunction->SetParameters(0, 1);
}

TOFPositionCorrelationPlot::~TOFPositionCorrelationPlot()
{
  delete m_correlationGraph;
  delete m_correlationFunction;
}

void TOFPositionCorrelationPlot::processEvent(const QVector<Hit*>& clusters, Track* track, SimpleEvent*)
{
  if (track) {
    int nTrackerHits = 0;
    foreach(Hit* hit, clusters)
      if (hit->type() == Hit::tracker)
        ++nTrackerHits;
    if (nTrackerHits != 8)
      return;
    foreach(Hit* hit, clusters)
      if (hit->type() == Hit::tof && (hit->detId()-hit->channel()) == m_id)
        histogram()->Fill(track->y(hit->position().z()), static_cast<TOFCluster*>(hit)->yEstimate(false));
  }
}

void TOFPositionCorrelationPlot::draw(TCanvas* canvas)
{
  H2DPlot::draw(canvas);
  if (m_correlationGraph->GetN()) {
    m_correlationGraph->Draw("SAME P");
    m_correlationFunction->Draw("SAME");
  }
}

void TOFPositionCorrelationPlot::finalize()
{
  int nBins = histogram()->GetXaxis()->GetNbins();
  for (int i = 0; i < nBins; ++i) {
    TF1 f("tofPositionCorreletionFitFunction", "gaus");
    TH1D* h = histogram()->ProjectionY("_py", i+1, i+1);
    if (h->GetEntries() > 10) {
      double x = histogram()->GetXaxis()->GetBinCenter(i+1);
      double sigma = 100;
      f.SetParameters(h->GetMaximum(), x, sigma);
      qDebug() << h->GetMaximum();
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
  m_correlationGraph->Fit(m_correlationFunction, "QN0");
}
