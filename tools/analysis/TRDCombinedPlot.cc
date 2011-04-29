#include "TRDCombinedPlot.hh"

#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QCheckBox>

#include <iostream>

#include "DetectorIDSpinBox.hh"
#include "TRDSpectrumPlot.hh"
#include "Setup.hh"
#include "DetectorElement.hh"

TRDCombinedPlot::TRDCombinedPlot() :
  PlotCollection(AnalysisPlot::SignalHeightTRD),
  m_moduleSpinBox(0),
  m_channelSpinBox(new QSpinBox),
  m_individualChannelCheckBox(new QCheckBox("show individual channels"))
{
  QVector<unsigned short> moduleIDs;

  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  for (ElementIterator elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::trd){
      TRDSpectrumPlot* trdModuleSpectrumPlot = new TRDSpectrumPlot(AnalysisPlot::SignalHeightTRD, element->id(),TRDSpectrumPlot::module);
      moduleIDs.append(element->id());
      addPlot(trdModuleSpectrumPlot);
      for(unsigned short tubeNo = 0; tubeNo < 16; tubeNo++){
        TRDSpectrumPlot* trdChannelSpectrumPlot = new TRDSpectrumPlot(AnalysisPlot::SignalHeightTRD, element->id() | tubeNo,TRDSpectrumPlot::channel);
        addPlot(trdChannelSpectrumPlot);
      }
    }
  }

  m_moduleSpinBox = new DetectorIDSpinBox(moduleIDs);

  m_secondaryWidget = new QWidget;
  QHBoxLayout* layout = new QHBoxLayout;
  m_secondaryWidget->setLayout(layout);

  m_channelSpinBox->setMinimum(0);
  m_channelSpinBox->setMaximum(15);
  m_channelSpinBox->setEnabled(false);

  layout->addWidget(new QLabel("Module"));
  layout->addWidget(m_moduleSpinBox);
  layout->addWidget(m_individualChannelCheckBox);
  layout->addWidget(new QLabel("Channel"));
  layout->addWidget(m_channelSpinBox);

  connect(m_individualChannelCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setChannelSpinBoxState(int)));
  connect(m_moduleSpinBox, SIGNAL(valueChanged(int)), this, SLOT(selectedPlotChanged()));
  connect(m_channelSpinBox, SIGNAL(valueChanged(int)), this, SLOT(selectedPlotChanged()));
  connect(m_individualChannelCheckBox, SIGNAL(stateChanged(int)), this, SLOT(selectedPlotChanged()));

  setTitle("TRD spectrum composite plot");
}

TRDCombinedPlot::~TRDCombinedPlot()
{
  delete m_moduleSpinBox;
  delete m_channelSpinBox;
  delete m_individualChannelCheckBox;
}

void TRDCombinedPlot::selectedPlotChanged()
{
  int module = m_moduleSpinBox->value();
  int channel = m_channelSpinBox->value();

  int index = module*17;
  if (m_individualChannelCheckBox->isChecked())
    index += channel + 1;

  selectPlot(index);
}

void TRDCombinedPlot::setChannelSpinBoxState(int state)
{
  m_channelSpinBox->setEnabled(state);
}
