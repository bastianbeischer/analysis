#include "ReconstructionMethodCorrelationPlotCollection.hh"
#include "ReconstructionMethodCorrelationPlot.hh"

#include "Constants.hh"

#include <QLayout>
#include <QHBoxLayout>

ReconstructionMethodCorrelationPlotCollection::ReconstructionMethodCorrelationPlotCollection()
  : PlotCollection(Enums::MomentumReconstruction)
  , m_reconstructionMethodXComboBox(new QComboBox())
  , m_reconstructionMethodYComboBox(new QComboBox())
{
  QHBoxLayout* layout = new QHBoxLayout();
  layout->addStretch();
  layout->addWidget(m_reconstructionMethodXComboBox);
  layout->addWidget(m_reconstructionMethodYComboBox);
  QWidget* widget = new QWidget();
  widget->setLayout(layout);
  secondaryWidget()->layout()->addWidget(widget);
  setTitle("reconstruction method correlation");

  connect(widget, SIGNAL(channelChanged(int)), this, SLOT(selectPlot(int)));

  Enums::ReconstructionMethodIterator end = Enums::reconstructionMethodEnd();
  for (Enums::ReconstructionMethodIterator it = Enums::reconstructionMethodBegin(); it != end; ++it) {
    m_reconstructionMethodXComboBox->addItem(it.value());
    m_reconstructionMethodYComboBox->addItem(it.value());
  }
  m_reconstructionMethodXComboBox->setCurrentIndex(m_reconstructionMethodXComboBox->findText(Enums::label(Enums::Spectrometer)));
  m_reconstructionMethodYComboBox->setCurrentIndex(m_reconstructionMethodXComboBox->findText(Enums::label(Enums::Likelihood)));
  connect(m_reconstructionMethodXComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));
  connect(m_reconstructionMethodYComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));

  for (Enums::ReconstructionMethodIterator xIt = Enums::reconstructionMethodBegin(); xIt != end; ++xIt)
    for (Enums::ReconstructionMethodIterator yIt = Enums::reconstructionMethodBegin(); yIt != end; ++yIt)
      addPlot(new ReconstructionMethodCorrelationPlot(xIt.key(), yIt.key()));
}

ReconstructionMethodCorrelationPlotCollection::~ReconstructionMethodCorrelationPlotCollection()
{
}

int ReconstructionMethodCorrelationPlotCollection::methodsToIndex(Enums::ReconstructionMethod x, Enums::ReconstructionMethod y)
{
  int index = 0;
  Enums::ReconstructionMethodIterator end = Enums::reconstructionMethodEnd();
  for (Enums::ReconstructionMethodIterator xIt = Enums::reconstructionMethodBegin(); xIt != end; ++xIt) {
    for (Enums::ReconstructionMethodIterator yIt = Enums::reconstructionMethodBegin(); yIt != end; ++yIt) {
      if (xIt.key() == x && yIt.key() == y)
        return index;
      ++index;
    }
  }
  return -1;
}

void ReconstructionMethodCorrelationPlotCollection::update()
{
  Enums::ReconstructionMethod x = Enums::reconstructionMethod(m_reconstructionMethodXComboBox->currentText());
  Enums::ReconstructionMethod y = Enums::reconstructionMethod(m_reconstructionMethodYComboBox->currentText());
  selectPlot(methodsToIndex(x, y));
  PlotCollection::update();
}
