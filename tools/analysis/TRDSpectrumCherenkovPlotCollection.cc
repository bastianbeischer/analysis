#include "TRDSpectrumCherenkovPlotCollection.hh"

#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>

#include "StringSpinBox.hh"
#include "TRDSpectrumPlot.hh"


TRDSpectrumCherenkovPlotCollection::TRDSpectrumCherenkovPlotCollection() :
  PlotCollection(AnalysisPlot::Testbeam)
{
  QComboBox* comboBox = new QComboBox();

  comboBox->addItem("no cherenkov cut");
  addPlot(new TRDSpectrumPlot(0 /*doesnt matter*/, TRDSpectrumPlot::completeTRD, -100, 100, TRDSpectrumPlot::none));
  comboBox->addItem("below both");
  addPlot(new TRDSpectrumPlot(0 /*doesnt matter*/, TRDSpectrumPlot::completeTRD, -100, 100, TRDSpectrumPlot::bothBelow));
  comboBox->addItem("above both");
  addPlot(new TRDSpectrumPlot(0 /*doesnt matter*/, TRDSpectrumPlot::completeTRD, -100, 100, TRDSpectrumPlot::bothAbove));
  comboBox->addItem("c1 above and c2 below");
  addPlot(new TRDSpectrumPlot(0 /*doesnt matter*/, TRDSpectrumPlot::completeTRD, -100, 100, TRDSpectrumPlot::c1AboveC2Below));
  comboBox->addItem("c1 below and c2 above");
  addPlot(new TRDSpectrumPlot(0 /*doesnt matter*/, TRDSpectrumPlot::completeTRD, -100, 100, TRDSpectrumPlot::c1BelowC2Above));

  QWidget* selectionWidget = new QWidget();

  QHBoxLayout* layout = new QHBoxLayout(selectionWidget);
  layout->addStretch();
  layout->addWidget(new QLabel("cherenkov cut settings"));
  layout->addWidget(comboBox);
  layout->addStretch();

  setSecondaryWidget(selectionWidget);
  connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectPlot(int)));

  setTitle("TRD spectra - cherenkov cuts");
}

TRDSpectrumCherenkovPlotCollection::~TRDSpectrumCherenkovPlotCollection()
{
}
