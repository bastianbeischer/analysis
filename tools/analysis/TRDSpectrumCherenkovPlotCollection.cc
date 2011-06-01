#include "TRDSpectrumCherenkovPlotCollection.hh"

#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>

#include "StringSpinBox.hh"
#include "TRDSpectrumCherenkovPlot.hh"


TRDSpectrumCherenkovPlotCollection::TRDSpectrumCherenkovPlotCollection() :
  PlotCollection(AnalysisPlot::Testbeam)
{
  QComboBox* comboBox = new QComboBox();

  comboBox->addItem("no cherenkov cut");
  addPlot(new TRDSpectrumCherenkovPlot(0 /*doesnt matter*/, TRDSpectrumPlot::completeTRD, -100, 100, TRDSpectrumCherenkovPlot::None));
  comboBox->addItem("below both");
  addPlot(new TRDSpectrumCherenkovPlot(0 /*doesnt matter*/, TRDSpectrumPlot::completeTRD, -100, 100, TRDSpectrumCherenkovPlot::BothBelow));
  comboBox->addItem("above both");
  addPlot(new TRDSpectrumCherenkovPlot(0 /*doesnt matter*/, TRDSpectrumPlot::completeTRD, -100, 100, TRDSpectrumCherenkovPlot::BothAbove));
  comboBox->addItem("c1 above and c2 below");
  addPlot(new TRDSpectrumCherenkovPlot(0 /*doesnt matter*/, TRDSpectrumPlot::completeTRD, -100, 100, TRDSpectrumCherenkovPlot::C1AboveC2Below));
  comboBox->addItem("c1 below and c2 above");
  addPlot(new TRDSpectrumCherenkovPlot(0 /*doesnt matter*/, TRDSpectrumPlot::completeTRD, -100, 100, TRDSpectrumCherenkovPlot::C1BelowC2Above));

  QWidget* selectionWidget = new QWidget();

  QHBoxLayout* layout = new QHBoxLayout(selectionWidget);
  layout->addStretch();
  layout->addWidget(new QLabel("cherenkov cut settings"));
  layout->addWidget(comboBox);
  layout->addStretch();

  secondaryWidget()->layout()->addWidget(selectionWidget);
  connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectPlot(int)));

  setTitle("TRD spectra - cherenkov cuts");
}

TRDSpectrumCherenkovPlotCollection::~TRDSpectrumCherenkovPlotCollection()
{
}
