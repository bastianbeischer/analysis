#include "PMTPlot.hh"

#include <TH1D.h>
#include <TLatex.h>
#include <TLine.h>

#include "SimpleEvent.hh"
#include "SensorTypes.hh"

const int PMTPlot::s_threshold = 200.;

PMTPlot::PMTPlot(SensorTypes::Type type) :
  AnalysisPlot(Enums::Testbeam),
  H1DPlot(),
  m_type(type)
{
  setTitle(SensorTypes::convertToString(m_type));
  TH1D* histogram = new TH1D(qPrintable(title()), "", 500, -500, 3000);
  m_thresholdBin = histogram->GetXaxis()->FindBin(s_threshold);
  addHistogram(histogram);

  setAxisTitle("signal height / ADC counts", "counts");
  m_line = new TLine;
  m_line->SetLineWidth(2);
  m_line->SetLineColor(kGreen);
  m_line->SetLineStyle(2);

  addLatex(RootPlot::newLatex(.80, .85));
}

PMTPlot::~PMTPlot()
{
}

void PMTPlot::draw(TCanvas* canvas)
{
  H1DPlot::draw(canvas);
  m_line->Draw();
}

void PMTPlot::processEvent(const AnalyzedEvent* event)
{
  histogram()->Fill(event->simpleEvent()->sensorData(m_type));
}

void PMTPlot::update()
{
  m_line->SetX1(s_threshold);
  m_line->SetX2(s_threshold);
  m_line->SetY1(0);
  m_line->SetY2(1.05*histogram()->GetMaximum());
  double sum0 = 0;
  double sum1 = 0;
  int nBins = histogram()->GetNbinsX();
  for (int bin = 1; bin <= nBins; ++bin) {
    double content = histogram()->GetBinContent(bin);
    sum0+= content;
    if (bin > m_thresholdBin)
      sum1+= content;
  }
  latex()->SetTitle(qPrintable(QString("%1\%").arg(100. * sum1 / sum0, 5, 'f', 2)));
}
