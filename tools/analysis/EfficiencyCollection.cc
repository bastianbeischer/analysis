#include "EfficiencyCollection.hh"
#include "AllTrackerLayersFlagEfficiency.hh"
#include "TrackFindingEfficiency.hh"
#include "TrackFindingEfficiencyAzimuthCorrelation.hh"
#include "TrackFindingEfficiencyZenithCorrelation.hh"
#include "Constants.hh"
#include "EfficiencyCorrectionSettings.hh"

#include <cmath>
#include <vector>

#include <QLayout>
#include <QComboBox>
#include <QWidget>
#include <QVector>
#include <QDebug>

EfficiencyCollection::EfficiencyCollection()
  : PlotCollection(Enums::MiscellaneousTracker)
{
  QWidget* widget = new QWidget;
  QHBoxLayout* layout = new QHBoxLayout(widget);
  layout->setContentsMargins(0, 0, 0, 0);
  QComboBox* comboBox = new QComboBox;
  layout->addWidget(comboBox);
  layout->addStretch();
  secondaryWidget()->layout()->addWidget(widget);
  setTitle("efficiency collection");

  int nBinsUnfolded = EfficiencyCorrectionSettings::numberOfBins(EfficiencyCorrectionSettings::Unfolded);
  int nBinsRaw = EfficiencyCorrectionSettings::numberOfBins(EfficiencyCorrectionSettings::Raw);
  addPlot(new AllTrackerLayersFlagEfficiency(EfficiencyCorrectionSettings::Unfolded));
  comboBox->addItem(QString("AllTrackerLayersFlagEfficiency %1").arg(nBinsUnfolded));
  addPlot(new AllTrackerLayersFlagEfficiency(EfficiencyCorrectionSettings::Raw));
  comboBox->addItem(QString("AllTrackerLayersFlagEfficiency %1").arg(nBinsRaw));
  addPlot(new TrackFindingEfficiency(EfficiencyCorrectionSettings::Unfolded));
  comboBox->addItem(QString("TrackFindingEfficiency %1").arg(nBinsUnfolded));
  addPlot(new TrackFindingEfficiency(EfficiencyCorrectionSettings::Raw));
  comboBox->addItem(QString("TrackFindingEfficiency %1").arg(nBinsRaw));
  addPlot(new TrackFindingEfficiencyZenithCorrelation(EfficiencyCorrectionSettings::Unfolded));
  comboBox->addItem(QString("TrackFindingEfficiencyZenithCorrelation %1").arg(nBinsUnfolded));
  addPlot(new TrackFindingEfficiencyZenithCorrelation(EfficiencyCorrectionSettings::Raw));
  comboBox->addItem(QString("TrackFindingEfficiencyZenithCorrelation %1").arg(nBinsRaw));
  addPlot(new TrackFindingEfficiencyAzimuthCorrelation(EfficiencyCorrectionSettings::Unfolded));
  comboBox->addItem(QString("TrackFindingEfficiencyAzimuthCorrelation %1").arg(nBinsUnfolded));
  addPlot(new TrackFindingEfficiencyAzimuthCorrelation(EfficiencyCorrectionSettings::Raw));
  comboBox->addItem(QString("TrackFindingEfficiencyAzimuthCorrelation %1").arg(nBinsRaw));

  connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectPlot(int)));
}

EfficiencyCollection::~EfficiencyCollection()
{
}
