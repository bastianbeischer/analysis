#include "TRDTimeCorrectionPlot.hh"

#include "Corrections.hh"
#include "SimpleEvent.hh"

#include <TSpline.h>
#include <TH1D.h>
#include <TLine.h>

TRDTimeCorrectionPlot::TRDTimeCorrectionPlot(QDateTime first, QDateTime last)
  : AnalysisPlot(Enums::MiscellaneousTRD)
  , H1DPlot()
  , m_t1(first.toTime_t())
  , m_t2(last.toTime_t())
  , m_line1(new TLine)
  , m_line2(new TLine)
  , m_corr(new Corrections)
  , m_TrdTimeFactors(m_corr->trdTimeFactors())
  , m_TrdTimeSpline(m_corr->trdTimeSpline())
{

  m_line1->SetLineColor(kGreen);
  m_line1->SetLineStyle(2);
  m_line1->SetLineWidth(2);

  m_line2->SetLineColor(kGreen);
  m_line2->SetLineStyle(2);
  m_line2->SetLineWidth(2);

  setTitle("in corrections saved time dependent correction for TRD");
  int dur = m_t2-m_t1;
  int t1 = m_t1 - 0.1*dur;
  int t2 = m_t2 + 0.1*dur;
  int nBins = (t2 - t1) / 60 *10;
  
  TH1D* histogram = new TH1D(qPrintable(title()), "", nBins, t1, t2);

  m_normalizationHistogram = new TH1D(qPrintable(title() + "normalization"), "", nBins, t1, t2);

  addHistogram(histogram);
}

TRDTimeCorrectionPlot::~TRDTimeCorrectionPlot()
{
  delete m_corr;
  delete m_TrdTimeSpline;
}

void TRDTimeCorrectionPlot::processEvent(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const event)
{
  double time = event->time();
  double corrFactor = m_corr->trdTimeDependendFactor(time);
  histogram()->Fill(time, corrFactor);
  m_normalizationHistogram->Fill(time);

}

void TRDTimeCorrectionPlot::finalize()
{
  //histogram()->Divide(m_normalizationHistogram); //Cannot be used due to a ROOT bug leading to a rebin of the x axis.
  for (int bin = 1; bin <= xAxis()->GetNbins(); ++bin) {
    double n = m_normalizationHistogram->GetBinContent(bin);
    if (n > 0)
      histogram()->SetBinContent(bin, histogram()->GetBinContent(bin) / n);
  }
}

void TRDTimeCorrectionPlot::update()
{
  if (m_drawn) {
    m_line1->SetX1(m_t1);
    m_line1->SetX2(m_t1);
    m_line1->SetY1(0);
    m_line1->SetY2(2);

    m_line2->SetX1(m_t2);
    m_line2->SetX2(m_t2);
    m_line2->SetY1(0);
    m_line2->SetY2(2);
  }
}

void TRDTimeCorrectionPlot::draw(TCanvas* canvas)
{
  if (m_drawn) {
    H1DPlot::draw(canvas);
    if (m_TrdTimeSpline)
      m_TrdTimeSpline->Draw("SAME CP");
    yAxis()->SetRangeUser(0, 2);
  } else {
    H1DPlot::draw(canvas);
    if (m_TrdTimeSpline)
      m_TrdTimeSpline->Draw("SAME CP");
    //TODO check this, especially for older data !!!
    xAxis()->SetTimeOffset(3600, "gmt"); //dont understand this, but works at testbeam
    xAxis()->SetTimeDisplay(1);
    xAxis()->SetTimeFormat("%d-%H:%M");
    yAxis()->SetRangeUser(0, 2);

    update();
    m_line1->Draw("same");
    m_line2->Draw("same");
  }
}



