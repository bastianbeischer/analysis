#include "TRDTimeCorrectionPlot.hh"

#include "Corrections.hh"

#include <TSpline.h>
#include <TH1D.h>
#include <TLine.h>

TRDTimeCorrectionPlot::TRDTimeCorrectionPlot(QDateTime first, QDateTime last)
  : AnalysisPlot(AnalysisPlot::MiscellaneousTRD)
  , H1DPlot()
  , m_t1(first.toTime_t())
  , m_t2(last.toTime_t())
  , m_line1(new TLine)
  , m_line2(new TLine)
  , m_corr(new Corrections)
  , m_TrdTimeFactors(m_corr->getTrdTimeFactors())
  , m_TrdTimeSpline(m_corr->getTrdTimeSpline())
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
  int nBins = 10;
  
  TH1D* histogram = new TH1D(qPrintable(title()), "", nBins, t1, t2);

  addHistogram(histogram);
}

TRDTimeCorrectionPlot::~TRDTimeCorrectionPlot()
{
  delete m_corr;
  delete m_TrdTimeSpline;
}

void TRDTimeCorrectionPlot::processEvent(const QVector<Hit*>&, Particle*, SimpleEvent*)
{
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
    m_TrdTimeSpline->Draw("SAME CP");
    yAxis()->SetRangeUser(0, 2);
  } else {
    H1DPlot::draw(canvas);
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



