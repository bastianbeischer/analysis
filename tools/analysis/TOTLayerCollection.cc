#include "TOTLayerCollection.hh"

#include "TOTLayer.hh"

#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>

#include "StringSpinBox.hh"

TOTLayerCollection::TOTLayerCollection(TOTLayer* plot) :
  PlotCollection(AnalysisPlot::TimeOverThreshold)
{  
  QComboBox* comboBox = new QComboBox();
  
  comboBox->addItem("All Layers");
  addPlot(plot->create(TOTLayer::All));
  comboBox->addItem("Upper Layers");
  addPlot(plot->create(TOTLayer::Upper));
  comboBox->addItem("Lower Layers");
  addPlot(plot->create(TOTLayer::Lower));
  
  delete plot;

  QWidget* selectionWidget = new QWidget();
  
  QHBoxLayout* layout = new QHBoxLayout(selectionWidget);
  layout->addStretch();
  layout->addWidget(new QLabel("TOF Section:"));
  layout->addWidget(comboBox);
  layout->addStretch();
  
  secondaryWidget()->layout()->addWidget(selectionWidget);
  connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectPlot(int)));
  
  setTitle("time over threshold per layer");
}

TOTLayerCollection::~TOTLayerCollection()
{
}

