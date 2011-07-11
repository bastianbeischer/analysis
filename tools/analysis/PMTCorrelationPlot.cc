#include "PMTCorrelationPlot.hh"

#include <TH2D.h>
#include <TLatex.h>
#include <TLine.h>

#include "SimpleEvent.hh"
#include "SensorTypes.hh"
#include "ProjectionControlWidget.hh"

#include <QSpinBox>


PMTCorrelationPlot::PMTCorrelationPlot() :
  AnalysisPlot(AnalysisPlot::Testbeam),
  H2DProjectionPlot(),
  m_c1Threshold(200),
  m_c2Threshold(200)
{
  controlWidget()->spinBox()->setMaximum(1024);
  controlWidget()->spinBox()->setValue(10);

  setTitle("cherenkov correlation");
  TH2D* histogram = new TH2D(qPrintable(title()), "", 150, -500, 3000, 150, -500, 3000);
  addHistogram(histogram);

  setAxisTitle("beam cherenkov 1 / ADC counts", "beam cherenkov 2 / ADC counts", "counts");

  m_c1FirstBinAbove = histogram->GetXaxis()->FindBin(m_c1Threshold);
  m_c2FirstBinAbove = histogram->GetYaxis()->FindBin(m_c1Threshold);
  m_c1LowerEdgeFirstBinAbove = histogram->GetXaxis()->GetBinLowEdge(m_c1FirstBinAbove);
  m_c2LowerEdgeFirstBinAbove = histogram->GetYaxis()->GetBinLowEdge(m_c2FirstBinAbove);

  m_line1 = new TLine;
  m_line1->SetLineWidth(2);
  m_line1->SetLineColor(kGreen);
  m_line1->SetLineStyle(2);
  m_line2 = new TLine;
  m_line2->SetLineWidth(2);
  m_line2->SetLineColor(kGreen);
  m_line2->SetLineStyle(2);

  double xMin = histogram->GetXaxis()->GetXmin();
  double xMax = histogram->GetXaxis()->GetXmax();
  double yMin = histogram->GetYaxis()->GetXmin();
  double yMax = histogram->GetYaxis()->GetXmax();

  addLatex(RootPlot::newLatex(xMin + (xMax-xMin)*.40, yMin + (yMax-yMin)*.98));

  addLatex(RootPlot::newLatex(xMin + (xMax-xMin)*.80, yMin + (yMax-yMin)*.95));
  addLatex(RootPlot::newLatex(xMin + (xMax-xMin)*.02, yMin + (yMax-yMin)*.08));

  addLatex(RootPlot::newLatex(m_c1LowerEdgeFirstBinAbove+(xMax-xMin)*.01, yMin + (yMax-yMin)*.10));
  addLatex(RootPlot::newLatex(xMin + (xMax-xMin)*.03, m_c2LowerEdgeFirstBinAbove + (yMax-yMin)*.05));

  addLatex(RootPlot::newLatex(xMin + (xMax-xMin)*.02, yMin + (yMax-yMin)*.95));
  addLatex(RootPlot::newLatex(xMin + (xMax-xMin)*.80, yMin + (yMax-yMin)*.08));

  for (int i = 0; i < 7; ++i) {
    latex(i)->SetNDC(kFALSE);
  }

}

PMTCorrelationPlot::~PMTCorrelationPlot()
{
  delete m_line1;
  delete m_line2;
}

void PMTCorrelationPlot::draw(TCanvas* canvas)
{
  H2DPlot::draw(canvas);
  m_line1->Draw();
  m_line2->Draw();
}

void PMTCorrelationPlot::processEvent(const QVector<Hit*>&, Particle*, SimpleEvent* event)
{
  histogram()->Fill(event->sensorData(SensorTypes::BEAM_CHERENKOV1), event->sensorData(SensorTypes::BEAM_CHERENKOV2));
}

void PMTCorrelationPlot::update()
{
  m_line1->SetX1(m_c1LowerEdgeFirstBinAbove);
  m_line1->SetX2(m_c1LowerEdgeFirstBinAbove);
  m_line1->SetY1(histogram()->GetYaxis()->GetBinLowEdge(histogram()->GetYaxis()->GetFirst()));
  m_line1->SetY2(histogram()->GetYaxis()->GetBinUpEdge(histogram()->GetYaxis()->GetLast()));
  m_line2->SetX1(histogram()->GetXaxis()->GetBinLowEdge(histogram()->GetXaxis()->GetFirst()));
  m_line2->SetX2(histogram()->GetXaxis()->GetBinUpEdge(histogram()->GetXaxis()->GetLast()));
  m_line2->SetY1(m_c2LowerEdgeFirstBinAbove);
  m_line2->SetY2(m_c2LowerEdgeFirstBinAbove);

  double allEvents = histogram()->GetEntries();
  latex(0)->SetTitle(qPrintable(QString("%1 entries").arg(allEvents)));

  double aboveBothEntries = histogram()->Integral(m_c1FirstBinAbove, histogram()->GetXaxis()->GetNbins()
                                                  , m_c2FirstBinAbove, histogram()->GetYaxis()->GetNbins());
  latex(1)->SetTitle(qPrintable(QString("%1\%").arg(100. * aboveBothEntries / allEvents, 5, 'f', 2)));

  double belowBothEntries = histogram()->Integral(0, m_c1FirstBinAbove-1
                                                  , 0, m_c2FirstBinAbove-1);
  latex(2)->SetTitle(qPrintable(QString("%1\%").arg(100. * belowBothEntries / allEvents, 5, 'f', 2)));

  double aboveC1Entries = histogram()->Integral(m_c1FirstBinAbove, histogram()->GetXaxis()->GetNbins()
                                                  , 0, histogram()->GetXaxis()->GetNbins());
  latex(3)->SetTitle(qPrintable(QString("%1\%").arg(100. * aboveC1Entries / allEvents, 5, 'f', 2)));

  double aboveC2Entries = histogram()->Integral(0, histogram()->GetXaxis()->GetNbins()
                                                  , m_c2FirstBinAbove, histogram()->GetYaxis()->GetNbins());
  latex(4)->SetTitle(qPrintable(QString("%1\%").arg(100. * aboveC2Entries / allEvents, 5, 'f', 2)));

  double belowC1aboveC2Entries = histogram()->Integral(0, m_c1FirstBinAbove-1
                                                  , m_c2FirstBinAbove, histogram()->GetYaxis()->GetNbins());
  latex(5)->SetTitle(qPrintable(QString("%1\%").arg(100. * belowC1aboveC2Entries / allEvents, 5, 'f', 2)));

  double aboveC1belowC2Entries = histogram()->Integral(m_c1FirstBinAbove, histogram()->GetXaxis()->GetNbins()
                                                  , 0, m_c2FirstBinAbove-1);
  latex(6)->SetTitle(qPrintable(QString("%1\%").arg(100. * aboveC1belowC2Entries / allEvents, 5, 'f', 2)));
}
