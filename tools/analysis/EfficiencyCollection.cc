#include "EfficiencyCollection.hh"

#include "AllTrackerLayersFlagEfficiency.hh"
#include "TrackFindingEfficiency.hh"
#include "Constants.hh"

#include <cmath>
#include <vector>

#include <QLayout>
#include <QComboBox>
#include <QWidget>
#include <QVector>
#include <QDebug>

EfficiencyCollection::EfficiencyCollection() 
  : PlotCollection(AnalysisPlot::MiscellaneousTracker)
{
  QWidget* widget = new QWidget;
  QHBoxLayout* layout = new QHBoxLayout(widget);
  layout->setContentsMargins(0, 0, 0, 0);
  QComboBox* comboBox = new QComboBox;
  layout->addWidget(comboBox);
  layout->addStretch();
  secondaryWidget()->layout()->addWidget(widget);
  setTitle("efficiency collection");

  int nBins = 0;
  nBins = Constants::nRigidityBinsUnfolded;
  addPlot(new AllTrackerLayersFlagEfficiency(nBins));
  comboBox->addItem(QString("AllTrackerLayersFlagEfficiency %1").arg(nBins));
  nBins = Constants::nRigidityBinsRaw;
  addPlot(new AllTrackerLayersFlagEfficiency(nBins));
  comboBox->addItem(QString("AllTrackerLayersFlagEfficiency %1").arg(nBins));
  nBins = Constants::nRigidityBinsUnfolded;
  addPlot(new TrackFindingEfficiency(nBins));
  comboBox->addItem(QString("TrackFindingEfficiency %1").arg(nBins));
  nBins = Constants::nRigidityBinsRaw;
  addPlot(new TrackFindingEfficiency(nBins));
  comboBox->addItem(QString("TrackFindingEfficiency %1").arg(nBins));

  connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectPlot(int)));
}

EfficiencyCollection::~EfficiencyCollection()
{
}
