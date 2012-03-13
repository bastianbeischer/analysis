#include "MeasurementTimePlot.hh"
#include "ParameterWidget.hh"
#include "PostAnalysisCanvas.hh"

#include <TH2.h>
#include <TAxis.h>
#include <TPad.h>
#include <TLatex.h>
#include <TLine.h>

#include <QDebug>
#include <QHBoxLayout>
#include <QWidget>

MeasurementTimePlot::MeasurementTimePlot(PostAnalysisCanvas* canvas)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_cutParameterWidget(new ParameterWidget)
  , m_cutLine(new TLine)
{
  TH1D* h = static_cast<TH1D*>(canvas->histograms1D().first()->Clone());
  setTitle("measurement time plot");
  setAxisTitle(h->GetXaxis()->GetTitle(), h->GetYaxis()->GetTitle());
  addHistogram(h);
  addLatex(RootPlot::newLatex(0.12, 0.88));

  QHBoxLayout* hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(0, 0, 0, 0);
  m_cutParameterWidget->setRange(h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());
  m_cutParameterWidget->setValue(10.0); // value for float
  connect(m_cutParameterWidget, SIGNAL(valueChanged(double)), this, SLOT(update()));
  hLayout->addWidget(m_cutParameterWidget);
  hLayout->addStretch();
  QWidget* widget = new QWidget;
  widget->setContentsMargins(0, 0, 0, 0);
  widget->setLayout(hLayout);
  setSecondaryWidget(widget);

  m_cutLine->SetLineColor(kGreen);
  m_cutLine->SetLineStyle(kDashed);
  m_cutLine->SetVertical();
}

MeasurementTimePlot::~MeasurementTimePlot()
{
}

void MeasurementTimePlot::draw(TCanvas* canvas)
{
  H1DPlot::draw(canvas);
  m_cutLine->Draw();
  update();
}

void MeasurementTimePlot::update()
{
  double cut = m_cutParameterWidget->value();
  m_cutLine->SetX1(cut);
  m_cutLine->SetX2(cut);
  m_cutLine->SetY1(histogram()->GetMinimum());
  m_cutLine->SetY2(1.05 * histogram()->GetMaximum());
  double time = histogram()->GetBinContent(histogram()->GetXaxis()->FindBin(cut));
  latex()->SetTitle(qPrintable(QString("T = %1").arg(time, 0, 'f', 2, ' ')));
  gPad->Modified();
  gPad->Update();
  emit measurementTimeChanged(time);
}
