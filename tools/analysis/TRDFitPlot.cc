#include "TRDFitPlot.hh"

#include <math.h>

#include <TH1D.h>

#include "Hit.hh"
#include "Cluster.hh"

#include "Track.hh"

TRDFitPlot::TRDFitPlot(AnalysisPlot::Topic topic, QString title) :
  AnalysisPlot(topic),
  H1DPlot()
{
  setTitle(title);
  TH1D* histogram = new TH1D(qPrintable(this->title()), "", 50, 5, 20);
  histogram->SetStats(true);
  addHistogram(histogram);
}

TRDFitPlot::~TRDFitPlot()
{
}

void TRDFitPlot::addLandauFit(TF1* landauFit)
{
  m_landauFits << landauFit;
}

void TRDFitPlot::processEvent(const QVector<Hit*>&,Track*, SimpleEvent* /*event*/)
{
  //do nothing on each event, maybe update
}

void TRDFitPlot::finalize()
{
  updateHistogram();
}

void TRDFitPlot::updateHistogram()
{
  //delete  all entries
  histogram(0)->Reset();

  //gather all mpv values
  QVector<double> mpvs;
  foreach(TF1* landauFit, m_landauFits)
    mpvs << landauFit->GetParameter(1);

  //sort
  qSort(mpvs);

  //check range
  int minBin = floor(mpvs.first()) -1;
  int maxBin = floor(mpvs.last()) +1;

  histogram(0)->SetBins(20, minBin, maxBin);

  //fill
  foreach(double mpv, mpvs)
    histogram(0)->Fill(mpv);
}

void TRDFitPlot::draw(TCanvas* c)
{
  updateHistogram();
  H1DPlot::draw(c);
}
