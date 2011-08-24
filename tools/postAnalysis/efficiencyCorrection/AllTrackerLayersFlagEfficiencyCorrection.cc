#include "AllTrackerLayersFlagEfficiencyCorrection.hh"

#include "Corrections.hh"
#include "Helpers.hh"

#include <TH1.h>
#include <TH2.h>
#include <TAxis.h>

#include <QDebug>
#include <QVector>
#include <QPushButton>
#include <QHBoxLayout>
#include <QWidget>

AllTrackerLayersFlagEfficiencyCorrection::AllTrackerLayersFlagEfficiencyCorrection(Type type, PostAnalysisCanvas* canvas)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_type(type)
{
  m_typeNames.insert(Positive, "positive");
  m_typeNames.insert(Negative, "negative");
  
  TH1D* histogram = 0;
  if (m_type == Negative)
    histogram = Helpers::createMirroredHistogram(canvas->histograms1D().at(0));
  else
    histogram = new TH1D(*canvas->histograms1D().at(0));
  const int nBins = histogram->GetNbinsX();
  double nBinsNew;
  if (nBins%2 == 0)
    nBinsNew = nBins / 2;
  else
    nBinsNew = (nBins - 1) / 2;
  m_quantity = EfficiencyCorrectionSettings::instance()->binQuantity(nBinsNew);

//  for (int i = 0; i < histogram->GetNbinsX(); ++i) {
//    histogram->SetBinContent(i+1, 1);
//    histogram->SetBinError(i+1, 0);
//  }

  QString title = QString(canvas->name()).replace("canvas", "histogram");
  title.append(QString(" - ") + m_typeNames[m_type] + EfficiencyCorrectionSettings::instance()->binQuantityName(m_quantity));
  setTitle(title);

  addHistogram(histogram);
  setAxisTitle(histogram->GetXaxis()->GetTitle(), "efficiency");

  QWidget* widget = new QWidget;
  QHBoxLayout* layout = new QHBoxLayout(widget);
  layout->setContentsMargins(0, 0, 0, 0);
  QPushButton* saveButton = new QPushButton("save");
  saveButton->setToolTip("save histogram in efficiency correction settings file. be sure to have switched to right settings with \"switch_to_config.sh\" to write desired file");
  layout->addWidget(saveButton);
  layout->addStretch();
  setSecondaryWidget(widget);
  connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));
}

AllTrackerLayersFlagEfficiencyCorrection::~AllTrackerLayersFlagEfficiencyCorrection()
{
}

void AllTrackerLayersFlagEfficiencyCorrection::save()
{
  if (m_type != Positive) {
    qDebug("you have to save the positive histogram");
    return;
  }
  EfficiencyCorrectionSettings::instance()->saveAllTrackerLayerCutEfficiency(m_quantity, histogram());
}
