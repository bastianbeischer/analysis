#include "ProjectionPlot.hh"
#include "DoubleEdit.hh"

#include <TH1D.h>
#include <TH2D.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TPolyLine.h>
#include <TLine.h>
#include <TLatex.h>
#include <TPad.h>

#include <QDebug>
#include <QHBoxLayout>
#include <QWidget>

ProjectionPlot::ProjectionPlot(TH2D* histogram)
  : PostAnalysisPlot()
  , H2DPlot()
  , m_histogram(static_cast<TH2D*>(histogram->Clone()))
  , m_xProjection(histogram->ProjectionX())
  , m_yProjection(histogram->ProjectionY())
  , m_area(new TPolyLine(5))
  , m_xLines()
  , m_yLines()
  , m_statisticsLines()
  , m_xLabels()
  , m_yLabels()
  , m_statisticsLabels()
  , m_statisticsArea(new TPolyLine(5))
  , m_pads()
  , m_xMinCutEdit(new DoubleEdit())
  , m_xMaxCutEdit(new DoubleEdit())
  , m_yMinCutEdit(new DoubleEdit())
  , m_yMaxCutEdit(new DoubleEdit())
{
  setTitle(QString(histogram->GetName()) + " projection");

  m_area->SetLineWidth(2);

  m_xLines.append(new TLine);
  m_xLines.append(new TLine);
  foreach (TLine* line, m_xLines) {
    line->SetLineWidth(2);
    line->SetLineColor(kGreen);
    line->SetLineStyle(kDashed);
  }
  m_xLabels.append(RootPlot::newLatex(0.70, 0.85));
  m_xLabels.append(RootPlot::newLatex(0.70, 0.82));
  m_xLabels.append(RootPlot::newLatex(0.70, 0.79));
  m_xLabels.append(RootPlot::newLatex(0.70, 0.76));
  
  m_yLines.append(new TLine);
  m_yLines.append(new TLine);
  foreach (TLine* line, m_yLines) {
    line->SetLineWidth(2);
    line->SetLineColor(kGreen);
    line->SetLineStyle(kDashed);
  }
  m_yLabels.append(RootPlot::newLatex(0.70, 0.85));
  m_yLabels.append(RootPlot::newLatex(0.70, 0.82));
  m_yLabels.append(RootPlot::newLatex(0.70, 0.79));
  m_yLabels.append(RootPlot::newLatex(0.70, 0.76));
  
  double xMin = histogram->GetXaxis()->GetXmin();
  double xMax = histogram->GetXaxis()->GetXmax();
  double xCenter = .5 * (xMax + xMin);
  double xRange = xMax - xMin;
  m_xMinCutEdit->setRange(xMin, xCenter + 0.1 * xRange);
  m_xMinCutEdit->setValue(xCenter - 0.1 * xRange);
  m_xMaxCutEdit->setRange(xCenter - 0.1 * xRange, xMax);
  m_xMaxCutEdit->setValue(xCenter + 0.1 * xRange);

  double yMin = histogram->GetYaxis()->GetXmin();
  double yMax = histogram->GetYaxis()->GetXmax();
  double yCenter = .5 * (yMax + yMin);
  double yRange = yMax - yMin;
  m_yMinCutEdit->setRange(yMin, yCenter + 0.1 * yRange);
  m_yMinCutEdit->setValue(yCenter - 0.1 * yRange);
  m_yMaxCutEdit->setRange(yCenter - 0.1 * yRange, yMax);
  m_yMaxCutEdit->setValue(yCenter + 0.1 * yRange);

  m_statisticsLines.append(new TLine(0.1, 0.3, 0.9, 0.3));
  m_statisticsLines.append(new TLine(0.1, 0.7, 0.9, 0.7));
  m_statisticsLines.append(new TLine(0.3, 0.1, 0.3, 0.9));
  m_statisticsLines.append(new TLine(0.7, 0.1, 0.7, 0.9));
  foreach (TLine* line, m_statisticsLines) {
    line->SetLineWidth(2);
    line->SetLineColor(kGreen);
    line->SetLineStyle(kDashed);
  }

  m_statisticsLabels.append(RootPlot::newLatex(0.2, 0.2));
  m_statisticsLabels.append(RootPlot::newLatex(0.5, 0.2));
  m_statisticsLabels.append(RootPlot::newLatex(0.8, 0.2));
  m_statisticsLabels.append(RootPlot::newLatex(0.2, 0.5));
  m_statisticsLabels.append(RootPlot::newLatex(0.5, 0.65));
  m_statisticsLabels.append(RootPlot::newLatex(0.5, 0.50));
  m_statisticsLabels.append(RootPlot::newLatex(0.5, 0.45));
  m_statisticsLabels.append(RootPlot::newLatex(0.8, 0.5));
  m_statisticsLabels.append(RootPlot::newLatex(0.2, 0.8));
  m_statisticsLabels.append(RootPlot::newLatex(0.5, 0.8));
  m_statisticsLabels.append(RootPlot::newLatex(0.8, 0.8));
  foreach (TLatex* label, m_statisticsLabels) {
    label->SetTextAlign(22);
  }
  m_statisticsArea->SetPoint(0, 0.35, 0.35);
  m_statisticsArea->SetPoint(1, 0.65, 0.35);
  m_statisticsArea->SetPoint(2, 0.65, 0.60);
  m_statisticsArea->SetPoint(3, 0.35, 0.60);
  m_statisticsArea->SetPoint(4, 0.35, 0.35);

  QHBoxLayout* layout = new QHBoxLayout();
  layout->addWidget(m_xMinCutEdit);
  layout->addWidget(m_xMaxCutEdit);
  layout->addWidget(m_yMinCutEdit);
  layout->addWidget(m_yMaxCutEdit);
  layout->addStretch();
  QWidget* widget = new QWidget;
  widget->setLayout(layout);
  setSecondaryWidget(widget);

  connect(m_xMinCutEdit, SIGNAL(valueChanged(double)), this, SLOT(update()));
  connect(m_xMaxCutEdit, SIGNAL(valueChanged(double)), this, SLOT(update()));
  connect(m_yMinCutEdit, SIGNAL(valueChanged(double)), this, SLOT(update()));
  connect(m_yMaxCutEdit, SIGNAL(valueChanged(double)), this, SLOT(update()));
}

ProjectionPlot::~ProjectionPlot()
{
}

void ProjectionPlot::update()
{
  m_area->SetPoint(0, m_xMinCutEdit->value(), m_yMinCutEdit->value());
  m_area->SetPoint(1, m_xMaxCutEdit->value(), m_yMinCutEdit->value());
  m_area->SetPoint(2, m_xMaxCutEdit->value(), m_yMaxCutEdit->value());
  m_area->SetPoint(3, m_xMinCutEdit->value(), m_yMaxCutEdit->value());
  m_area->SetPoint(4, m_xMinCutEdit->value(), m_yMinCutEdit->value());

  m_xMinCutEdit->setRange(m_histogram->GetXaxis()->GetXmin(), m_xMaxCutEdit->value());
  m_xMaxCutEdit->setRange(m_xMinCutEdit->value(), m_histogram->GetXaxis()->GetXmax());
  m_yMinCutEdit->setRange(m_histogram->GetYaxis()->GetXmin(), m_yMaxCutEdit->value());
  m_yMaxCutEdit->setRange(m_yMinCutEdit->value(), m_histogram->GetYaxis()->GetXmax());

  m_xLines[0]->SetX1(m_xMinCutEdit->value());
  m_xLines[0]->SetX2(m_xMinCutEdit->value());
  m_xLines[0]->SetY1(0);
  m_xLines[0]->SetY2(1.05 * m_xProjection->GetMaximum());
 
  m_xLines[1]->SetX1(m_xMaxCutEdit->value());
  m_xLines[1]->SetX2(m_xMaxCutEdit->value());
  m_xLines[1]->SetY1(0);
  m_xLines[1]->SetY2(1.05 * m_xProjection->GetMaximum());
 
  m_yLines[0]->SetX1(0);
  m_yLines[0]->SetX2(1.05 * m_yProjection->GetMaximum());
  m_yLines[0]->SetY1(m_yMinCutEdit->value());
  m_yLines[0]->SetY2(m_yMinCutEdit->value());
 
  m_yLines[1]->SetX1(0);
  m_yLines[1]->SetX2(1.05 * m_yProjection->GetMaximum());
  m_yLines[1]->SetY1(m_yMaxCutEdit->value());
  m_yLines[1]->SetY2(m_yMaxCutEdit->value());

  double sum, innerSum;

  sum = 0;
  innerSum = 0;
  for (int bin = 0; bin <= m_xProjection->GetXaxis()->GetNbins() + 1; ++bin) {
    sum+= m_xProjection->GetBinContent(bin);
    if (m_xMinCutEdit->value() < m_xProjection->GetBinCenter(bin) && m_xProjection->GetBinCenter(bin) < m_xMaxCutEdit->value())
      innerSum+= m_xProjection->GetBinContent(bin);
  }
  m_xLabels[0]->SetTitle(qPrintable(QString("n     = %1").arg(sum)));
  m_xLabels[1]->SetTitle(qPrintable(QString("ratio = %1\%").arg(100. * innerSum / sum, 0, 'f', 2, ' ')));
  m_xLabels[2]->SetTitle(qPrintable(QString("uflow = %1").arg(m_xProjection->GetBinContent(0))));
  m_xLabels[3]->SetTitle(qPrintable(QString("oflow = %1").arg(m_xProjection->GetBinContent(m_xProjection->GetNbinsX() + 1))));

  sum = 0;
  innerSum = 0;
  for (int bin = 0; bin <= m_yProjection->GetXaxis()->GetNbins() + 1; ++bin) {
    sum+= m_yProjection->GetBinContent(bin);
    if (m_yMinCutEdit->value() < m_yProjection->GetBinCenter(bin) && m_yProjection->GetBinCenter(bin) < m_yMaxCutEdit->value())
      innerSum+= m_yProjection->GetBinContent(bin);
  }
  Q_ASSERT(qFuzzyCompare(sum, m_histogram->GetEntries()));
  m_yLabels[0]->SetTitle(qPrintable(QString("n     = %1").arg(sum)));
  m_yLabels[1]->SetTitle(qPrintable(QString("ratio = %1\%").arg(100. * innerSum / sum, 0, 'f', 2, ' ')));
  m_yLabels[2]->SetTitle(qPrintable(QString("uflow = %1").arg(m_yProjection->GetBinContent(0))));
  m_yLabels[3]->SetTitle(qPrintable(QString("oflow = %1").arg(m_yProjection->GetBinContent(m_yProjection->GetNbinsX() + 1))));
 

  //bottom left
  m_statisticsLabels[0]->SetTitle(qPrintable(QString("%1").arg(m_histogram->GetBinContent(0, 0))));
  //bottom center
  sum = 0;
  for (int bin = 1; bin <= m_histogram->GetNbinsX(); ++bin)
    sum+= m_histogram->GetBinContent(bin, 0);
  m_statisticsLabels[1]->SetTitle(qPrintable(QString("%1").arg(sum)));
  //bottom right
  m_statisticsLabels[2]->SetTitle(qPrintable(QString("%1").arg(m_histogram->GetBinContent(m_histogram->GetNbinsX() + 1, 0))));

  //center left
  sum = 0;
  for (int bin = 1; bin <= m_histogram->GetNbinsY(); ++bin)
    sum+= m_histogram->GetBinContent(0, bin);
  m_statisticsLabels[3]->SetTitle(qPrintable(QString("%1").arg(sum)));
  //center
  sum = 0;
  innerSum = 0;
  for (int binX = 1; binX <= m_histogram->GetNbinsX(); ++binX) {
    for (int binY = 1; binY <= m_histogram->GetNbinsY(); ++binY) {
      sum+= m_histogram->GetBinContent(binX, binY);
      if (m_xMinCutEdit->value() < m_xProjection->GetBinCenter(binX)
        && m_xProjection->GetBinCenter(binX) < m_xMaxCutEdit->value()
        && m_yMinCutEdit->value() < m_yProjection->GetBinCenter(binY)
        && m_yProjection->GetBinCenter(binY) < m_yMaxCutEdit->value())
        innerSum+= m_histogram->GetBinContent(binX, binY);
    }
  }
  m_statisticsLabels[4]->SetTitle(qPrintable(QString("%1").arg(sum)));
  m_statisticsLabels[5]->SetTitle(qPrintable(QString("%1\%").arg(100. * innerSum / m_histogram->GetEntries(), 0, 'f', 2, ' ')));
  m_statisticsLabels[6]->SetTitle(qPrintable(QString("%1 / %2").arg(innerSum).arg(m_histogram->GetEntries())));
  //center right
  sum = 0;
  for (int bin = 1; bin <= m_histogram->GetNbinsY(); ++bin)
    sum+= m_histogram->GetBinContent(m_histogram->GetNbinsX() + 1, bin);
  m_statisticsLabels[7]->SetTitle(qPrintable(QString("%1").arg(sum)));

  //top left
  m_statisticsLabels[8]->SetTitle(qPrintable(QString("%1").arg(m_histogram->GetBinContent(0, m_histogram->GetNbinsY() + 1))));
  //top center
  sum = 0;
  for (int bin = 1; bin <= m_histogram->GetNbinsX(); ++bin)
    sum+= m_histogram->GetBinContent(bin, m_histogram->GetNbinsY() + 1);
  m_statisticsLabels[9]->SetTitle(qPrintable(QString("%1").arg(sum)));
  //top right
  m_statisticsLabels[10]->SetTitle(qPrintable(QString("%1").arg(m_histogram->GetBinContent(m_histogram->GetNbinsX() + 1, m_histogram->GetNbinsY() + 1))));

  foreach (TVirtualPad* pad, m_pads) {
    pad->Modified();
    pad->Update();
  }
}

void ProjectionPlot::draw(TCanvas* canvas)
{
  canvas->Clear();
  m_pads.clear();
  canvas->Divide(2, 2, 1e-6, 1e-6);
  update();

  m_pads.append(canvas->cd(3));
  m_histogram->Draw("COL");
  m_area->Draw();

  m_pads.append(canvas->cd(4));
  m_yProjection->SetFillColor(2);
  m_yProjection->Draw("HBAR");
  foreach (TLine* line, m_yLines)
    line->Draw();
  foreach (TLatex* latex, m_yLabels)
    latex->Draw();
  
  m_pads.append(canvas->cd(1));
  m_xProjection->SetFillColor(2);
  m_xProjection->Draw("BAR");
  foreach (TLine* line, m_xLines)
    line->Draw();
  foreach (TLatex* latex, m_xLabels)
    latex->Draw();
  
  m_pads.append(canvas->cd(2));
  foreach (TLine* line, m_statisticsLines)
    line->Draw();
  foreach (TLatex* latex, m_statisticsLabels)
    latex->Draw();
  m_statisticsArea->Draw();
  
  m_pads.append(canvas->cd());
}
