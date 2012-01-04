#include "TOTLayerCollection.hh"
#include "StringSpinBox.hh"
#include "TOTProjectionPlot.hh"
#include "TOTSignalCorrelation.hh"
#include "TOTMomentumCorrelation.hh"
#include "TOTBetaCorrelation.hh"

#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QString>

TOTLayerCollection::TOTLayerCollection(Type type)
  : PlotCollection(Enums::TimeOverThreshold)
{
  QComboBox* comboBox = new QComboBox();
  comboBox->addItem("All Layers");
  comboBox->addItem("Upper Layers");
  comboBox->addItem("Lower Layers");
  QWidget* selectionWidget = new QWidget();
  QHBoxLayout* layout = new QHBoxLayout(selectionWidget);
  layout->addStretch();
  layout->addWidget(new QLabel("TOF Section:"));
  layout->addWidget(comboBox);
  layout->addStretch();
  secondaryWidget()->layout()->addWidget(selectionWidget);
  connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectPlot(int)));

  QString title = "time-over-threshold ";
  switch (type) {
    case Projection:
      title+= "projection";
      addPlot(new TOTProjectionPlot(title, TOTLayerPlot::All));
      addPlot(new TOTProjectionPlot(title, TOTLayerPlot::Upper));
      addPlot(new TOTProjectionPlot(title, TOTLayerPlot::Lower));
      break;
    case TrdSignalCorrelation:
      title+= "trd signal correlation";
      addPlot(new TOTSignalCorrelation(title, Hit::trd, TOTLayerPlot::All));
      addPlot(new TOTSignalCorrelation(title, Hit::trd, TOTLayerPlot::Upper));
      addPlot(new TOTSignalCorrelation(title, Hit::trd, TOTLayerPlot::Lower));
      break;
    case TrackerSigalCorrelation:
      title+= "tracker signal correlation";
      addPlot(new TOTSignalCorrelation(title, Hit::tracker, TOTLayerPlot::All));
      addPlot(new TOTSignalCorrelation(title, Hit::tracker, TOTLayerPlot::Upper));
      addPlot(new TOTSignalCorrelation(title, Hit::tracker, TOTLayerPlot::Lower));
      break;
    case MomentumCorrelation:
      title+= "momentum correlation";
      addPlot(new TOTMomentumCorrelation(title, TOTLayerPlot::All));
      addPlot(new TOTMomentumCorrelation(title, TOTLayerPlot::Upper));
      addPlot(new TOTMomentumCorrelation(title, TOTLayerPlot::Lower));
      break;
    case BetaCorrelation:
      title+= "beta correlation";
      addPlot(new TOTBetaCorrelation(title, TOTLayerPlot::All));
      addPlot(new TOTBetaCorrelation(title, TOTLayerPlot::Upper));
      addPlot(new TOTBetaCorrelation(title, TOTLayerPlot::Lower));
      break;
  }
  setTitle(title);
}

TOTLayerCollection::~TOTLayerCollection()
{
}
