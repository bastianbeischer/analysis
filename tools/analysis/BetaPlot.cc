#include "BetaPlot.hh"
#include "BrokenLine.hh"
#include "TrackInformation.hh"
#include "Hit.hh"
#include "Constants.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "Setup.hh"
#include "TOFBar.hh"

#include <TH1.h>
#include <TVector3.h>
#include <TLatex.h>

#include <QDebug>

BetaPlot::BetaPlot()
  : AnalysisPlot(AnalysisPlot::MiscellaneousTOF)
  , H1DPlot()
{
  setTitle("1 / beta");
  TH1D* histogram = new TH1D(qPrintable(title()), "", 1200, -60, 60);
  histogram->GetXaxis()->SetTitle("1 / #beta");
  addHistogram(histogram);
  addLatex(RootPlot::newLatex(.15, .85));
  addLatex(RootPlot::newLatex(.15, .82));
  addLatex(RootPlot::newLatex(.15, .79));
  addLatex(RootPlot::newLatex(.15, .76));
  addLatex(RootPlot::newLatex(.15, .73));
}

BetaPlot::~BetaPlot()
{}

void BetaPlot::processEvent(const QVector<Hit*>&, Track* track, SimpleEvent*)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::AllTrackerLayers))
    return;
  histogram()->Fill(1./track->beta());
}

void BetaPlot::update()
{
  latex(0)->SetTitle(qPrintable(QString("entries = %1").arg(histogram()->GetEntries())));
  latex(1)->SetTitle(qPrintable(QString("mean    = %1").arg(histogram()->GetMean())));
  latex(2)->SetTitle(qPrintable(QString("RMS     = %1").arg(histogram()->GetRMS())));
  latex(3)->SetTitle(qPrintable(QString("uflow   = %1").arg(histogram()->GetBinContent(0))));
  int nBins = histogram()->GetXaxis()->GetNbins();
  latex(4)->SetTitle(qPrintable(QString("oflow   = %1").arg(histogram()->GetBinContent(nBins+1))));
}
