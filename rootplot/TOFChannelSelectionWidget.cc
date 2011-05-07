#include "TOFChannelSelectionWidget.hh"

#include "DetectorIDSpinBox.hh"

#include <QLabel>
#include <QHBoxLayout>

TOFChannelSelectionWidget::TOFChannelSelectionWidget(const QVector<unsigned short>& moduleIDs, QWidget* parent) :
  QWidget(parent)
{
  QHBoxLayout* layout = new QHBoxLayout(this);
  DetectorIDSpinBox* spinBox = new DetectorIDSpinBox(moduleIDs);
  layout->addWidget(new QLabel("channel: "));
  layout->addWidget(spinBox);
  
  connect(spinBox, SIGNAL(valueChanged(int)), this, SIGNAL(channelChanged(int)));
}

TOFChannelSelectionWidget::~TOFChannelSelectionWidget()
{
}

