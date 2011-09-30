#include "Caption.hh"

#include <QFrame>
#include <QLabel>
#include <QHBoxLayout>

Caption::Caption(const QString& caption, QWidget* parent)
  : QWidget(parent)
{
  QFrame* leftLine = 0;
  leftLine = new QFrame(this);
  leftLine->setGeometry(QRect(320, 150, 118, 3));
  leftLine->setFrameShape(QFrame::HLine);
  leftLine->setFrameShadow(QFrame::Sunken);

  QLabel* label = new QLabel(caption);
  label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

  QFrame* rightLine = new QFrame(this);
  rightLine->setGeometry(QRect(320, 150, 118, 3));
  rightLine->setFrameShape(QFrame::HLine);
  rightLine->setFrameShadow(QFrame::Sunken);

  QHBoxLayout* layout = new QHBoxLayout;
  layout->addWidget(leftLine);
  layout->addWidget(label);
  layout->addWidget(rightLine);
  setLayout(layout);
}

Caption::~Caption()
{}
