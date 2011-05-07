#include "TOFChannelSelectionWidget.hh"

#include "StringSpinBox.hh"

#include <QLabel>
#include <QHBoxLayout>

TOFChannelSelectionWidget::TOFChannelSelectionWidget(const QStringList& moduleIDs, QWidget* parent) :
  QWidget(parent)
{
  QHBoxLayout* layout = new QHBoxLayout(this);
  StringSpinBox* spinBox = new StringSpinBox(moduleIDs);
  layout->addWidget(new QLabel("channel: "));
  layout->addWidget(spinBox);
  
  connect(spinBox, SIGNAL(valueChanged(int)), this, SIGNAL(channelChanged(int)));
}

TOFChannelSelectionWidget::~TOFChannelSelectionWidget()
{
}

