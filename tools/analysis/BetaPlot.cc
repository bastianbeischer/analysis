#include "BetaPlot.hh"
#include "BrokenLine.hh"
#include "ParticleInformation.hh"
#include "Hit.hh"
#include "Constants.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "Setup.hh"
#include "TOFBar.hh"
#include "Particle.hh"
#include "Track.hh"
#include "Hypothesis.hh"

#include <TH1.h>
#include <TVector3.h>
#include <TLatex.h>
#include <TLine.h>
#include <TPad.h>

#include <QDebug>

BetaPlot::BetaPlot()
  : AnalysisPlot(Enums::MiscellaneousTOF)
  , H1DPlot()
  , m_cut(0)
  , m_line(new TLine)
{
  setTitle("beta");
  TH1D* histogram = new TH1D(qPrintable(title()), "", 200, -10, 10);
  setAxisTitle("1 / #beta", "");
  addHistogram(histogram);
  addLatex(RootPlot::newLatex(.15, .85));
  addLatex(RootPlot::newLatex(.15, .82));
  addLatex(RootPlot::newLatex(.15, .79));
  addLatex(RootPlot::newLatex(.15, .76));
  m_line->SetLineColor(kGreen);
  m_line->SetLineStyle(2);
  m_line->SetLineWidth(2);
}

BetaPlot::~BetaPlot()
{
  delete m_line;
}

void BetaPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->goodTrack();
  if (!track)
    return;
  if (!event->flagsSet(Enums::Chi2Good | Enums::BetaGood))
    return;
  histogram()->Fill(1./event->particle()->hypothesis()->beta());
}

void BetaPlot::update()
{
  int nBins = xAxis()->GetNbins();
  int cutBin = qMin(xAxis()->FindBin(m_cut), nBins);
  int n = histogram()->GetEntries();
  double sum = 0;
  for (int bin = 1; bin <= cutBin; ++bin)
    sum+= histogram()->GetBinContent(bin);
  latex(0)->SetTitle(qPrintable(QString("n     = %1").arg(n)));
  latex(1)->SetTitle(qPrintable(QString("ratio = %1\%").arg(100. * sum / n, 0, 'f', 2, ' ')));
  latex(2)->SetTitle(qPrintable(QString("uflow = %1").arg(histogram()->GetBinContent(0))));
  latex(3)->SetTitle(qPrintable(QString("oflow = %1").arg(histogram()->GetBinContent(nBins+1))));
  m_line->SetX1(m_cut);
  m_line->SetX2(m_cut);
  m_line->SetY1(0);
  m_line->SetY2(1.05*histogram()->GetMaximum());
}

void BetaPlot::draw(TCanvas* c)
{
  H1DPlot::draw(c);
  m_line->SetX1(m_cut);
  m_line->SetX2(m_cut);
  m_line->SetY1(0);
  m_line->SetY2(1.05*histogram()->GetMaximum());
  m_line->Draw();
}
