#include "SignalHeightPdfPlotCollection.hh"
#include "SignalHeightPdfPlot.hh"

#include "Track.hh"
#include "Hit.hh"
#include "Particle.hh"
#include "DetectorElement.hh"

#include <QSpinBox>
#include <QComboBox>
#include <QHBoxLayout>

#include <iostream>

SignalHeightPdfPlotCollection::SignalHeightPdfPlotCollection(Hit::ModuleType type, Enums::Particles particles)
  : PlotCollection(Enums::LikelihoodTopic)
  , m_particles(particles)
  , m_particleComboBox(new QComboBox)
{
  QHBoxLayout* layout = new QHBoxLayout();
  layout->addStretch();
  layout->addWidget(m_particleComboBox);
  QWidget* widget = new QWidget();
  widget->setLayout(layout);
  secondaryWidget()->layout()->addWidget(widget);
  connect(m_particleComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));

  QString typeString;
  if (type == Hit::tracker) {
    typeString = "tracker";
  } else if (type == Hit::tof) {
    typeString = "tof";
  } else if (type == Hit::trd) {
    typeString = "trd";
  }
  Q_ASSERT(!typeString.isEmpty());
  setTitle(typeString + " signal height pdf plot collection");

  QVector<double> xBins = QVector<double>() << 0 << 1 << 2 << 3;
  QVector<double> yBins = QVector<double>() << 0 << 1 << 2 << 3;
  Enums::ParticleIterator end = Enums::particleEnd();
  for (Enums::ParticleIterator it = Enums::particleBegin(); it != end; ++it)
    if ((it.key() != Enums::NoParticle) && ((it.key() & m_particles) == it.key())) {
      m_particleComboBox->addItem(it.value());
      addPlot(new SignalHeightPdfPlot(type, it.key(), xBins, yBins));
    }
}

SignalHeightPdfPlotCollection::~SignalHeightPdfPlotCollection()
{
}

void SignalHeightPdfPlotCollection::update()
{
  Enums::ParticleIterator end = Enums::particleEnd();
  Enums::ParticleIterator it = Enums::particleBegin();
  for (int i = 0; it != end; ++it) {
    if ((it.key() == Enums::NoParticle) || ((it.key() & m_particles) == it.key()))
      continue;
    if (it.key() == Enums::particle(m_particleComboBox->currentText())) {
      selectPlot(i);
      return;
    }
    ++i;
  }
}
