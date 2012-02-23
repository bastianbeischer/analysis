#include "EfficiencyCollection.hh"
#include "AllTrackerLayersFlagEfficiency.hh"
#include "TrackFindingEfficiency.hh"
#include "TrackFindingEfficiencyAzimuthCorrelation.hh"
#include "TrackFindingEfficiencyZenithCorrelation.hh"
#include "Constants.hh"

#include <cmath>
#include <vector>

#include <QLayout>
#include <QComboBox>
#include <QWidget>
#include <QVector>
#include <QDebug>

EfficiencyCollection::EfficiencyCollection()
  : PlotCollection(Enums::FluxCalculation)
{
  QWidget* widget = new QWidget;
  QHBoxLayout* layout = new QHBoxLayout(widget);
  layout->setContentsMargins(0, 0, 0, 0);
  QComboBox* comboBox = new QComboBox;
  layout->addWidget(comboBox);
  layout->addStretch();
  secondaryWidget()->layout()->addWidget(widget);
  setTitle("efficiency collection");

  addPlot(new AllTrackerLayersFlagEfficiency(false));
  comboBox->addItem("AllTrackerLayersFlagEfficiency");

  addPlot(new AllTrackerLayersFlagEfficiency(true));
  comboBox->addItem("AllTrackerLayersFlagEfficiency fine");

  addPlot(new TrackFindingEfficiency(false));
  comboBox->addItem("TrackFindingEfficiency");

  addPlot(new TrackFindingEfficiency(true));
  comboBox->addItem("TrackFindingEfficiency fine");

  addPlot(new TrackFindingEfficiencyZenithCorrelation(false));
  comboBox->addItem("TrackFindingEfficiencyZenithCorrelation");

  addPlot(new TrackFindingEfficiencyZenithCorrelation(true));
  comboBox->addItem("TrackFindingEfficiencyZenithCorrelation fine");

  addPlot(new TrackFindingEfficiencyAzimuthCorrelation(false));
  comboBox->addItem("TrackFindingEfficiencyAzimuthCorrelation");

  addPlot(new TrackFindingEfficiencyAzimuthCorrelation(true));
  comboBox->addItem("TrackFindingEfficiencyAzimuthCorrelation fine");

  connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectPlot(int)));
}

EfficiencyCollection::~EfficiencyCollection()
{
}
