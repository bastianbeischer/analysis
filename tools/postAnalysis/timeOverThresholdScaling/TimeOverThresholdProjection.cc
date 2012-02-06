#include "TimeOverThresholdProjection.hh"
#include "PostAnalysisCanvas.hh"
#include "ProjectionControlWidget.hh"
#include "Corrections.hh"
#include "RootQtWidget.hh"

#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TF1.h>
#include <TLatex.h>

#include <iostream>
#include <iomanip>

#include <QDebug>
#include <QWidget>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QList>

TimeOverThresholdProjection::TimeOverThresholdProjection(PostAnalysisCanvas* canvas)
  : H2DProjectionPlot()
  , PostAnalysisPlot()
  , m_tofId(0x8000)
{
  TH2D* histogram = canvas->histograms2D().at(0);
  QString title = QString(canvas->name()).replace("canvas", "histogram");
  setTitle(title);
  setAxisTitle("channel", "time over threshold / ns", "");
  addHistogram(new TH2D(*histogram));

  TF1* function = new TF1(qPrintable(QString(histogram->GetTitle()) + "Function"), "gaus", histogram->GetYaxis()->GetXmin(), histogram->GetYaxis()->GetXmax());
  function->SetLineColor(kRed);
  addProjectionFunction(function);
  addProjectionLatex(RootPlot::newLatex(.60, .85));
  addProjectionLatex(RootPlot::newLatex(.60, .80));
  addProjectionLatex(RootPlot::newLatex(.60, .75));
  const int defaultIndex = projectionControlWidget()->comboBox()->findText("projection on y");
  projectionControlWidget()->comboBox()->setCurrentIndex(defaultIndex);

  QWidget* saveWidget = new QWidget;
  QHBoxLayout* saveLayout = new QHBoxLayout(saveWidget);
  saveLayout->setContentsMargins(0, 0, 0, 0);
  QPushButton* saveButton = new QPushButton("save channel");
  saveLayout->addWidget(saveButton);
  QPushButton* saveAllButton = new QPushButton("save all channels");
  saveLayout->addWidget(saveAllButton);
  saveLayout->addStretch();

  QWidget* widget = new QWidget;
  QVBoxLayout* layout = new QVBoxLayout(widget);
  layout->addWidget(saveWidget);
  layout->addWidget(projectionControlWidget());
  layout->addWidget(projectionWidget());
  setSecondaryWidget(widget);

  connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));
  connect(saveAllButton, SIGNAL(clicked()), this, SLOT(saveAll()));
}

TimeOverThresholdProjection::~TimeOverThresholdProjection()
{}

void TimeOverThresholdProjection::updateProjection()
{
  if (projectionType() == ProjectionOnY) {
    m_tofId = (projectionBin() - 1) | 0x8000;
    const int nEntries = projection()->GetEntries();
    projection()->Fit(projectionFunction(), "EQN0");
    const double mean = projectionFunction()->GetParameter(1);
    const double meanError = projectionFunction()->GetParError(1);
    projectionLatex(0)->SetTitle(qPrintable(QString("channel: 0x%1").arg(m_tofId , 0, 16)));
    projectionLatex(1)->SetTitle(qPrintable(QString("entries: %1").arg(nEntries)));
    projectionLatex(2)->SetTitle(qPrintable(QString("mean: %1 #pm %2").arg(mean).arg(meanError)));
  }
}

void TimeOverThresholdProjection::save(unsigned int tofId)
{
  QList<QVariant> param;
  unsigned int ch = tofId & 0xFF;
  TH1* projectionHistogram = histogram()->ProjectionY(qPrintable(title() + "projection"), ch+1, ch+1);
  projectionHistogram->Fit(projectionFunction(), "EQN0");
  param.push_back(projectionFunction()->GetParameter(1));
  param.push_back(0.);
  Corrections* correction = new Corrections();
  correction->setTotScaling(tofId, param);
  delete correction;
}

void TimeOverThresholdProjection::save()
{
  save(m_tofId);
}

void TimeOverThresholdProjection::saveAll()
{
  for (int tofId = 0x8000; tofId <= 0x803f; ++tofId) {
    save(tofId);
  }
}
