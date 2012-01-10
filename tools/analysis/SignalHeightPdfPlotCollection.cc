#include "SignalHeightPdfPlotCollection.hh"
#include "SignalHeightPdfPlot.hh"

#include "Track.hh"
#include "Hit.hh"
#include "Particle.hh"
#include "DetectorElement.hh"
#include "Helpers.hh"
#include "TimeOverThresholdLikelihood.hh"
#include "SignalHeightTrdLikelihood.hh"

#include <QSpinBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QDebug>

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

  QString typeString;
  QVector<double> xBins = Helpers::linearBinning(50, 0, 10);
  QVector<double> yBins;
  Likelihood* lh = 0;
  if (type == Hit::tracker) {
    typeString = "tracker";
    //lh = new TrackerSignalLikelihood();
    //yBins = Helpers::linearBinning(100, lh->measuredValueMin(), lh->measuredValueMax());
    yBins = Helpers::linearBinning(100, 0, 5000);
  } else if (type == Hit::tof) {
    typeString = "tof";
    lh = new TimeOverThresholdLikelihood();
    yBins = Helpers::linearBinning(60, lh->measuredValueMin(), lh->measuredValueMax());
  } else if (type == Hit::trd) {
    typeString = "trd";
    lh = new SignalHeightTrdLikelihood();
    yBins = Helpers::linearBinning(100, lh->measuredValueMin(), lh->measuredValueMax());
  }
  if (lh)
    delete lh;
  Q_ASSERT(!typeString.isEmpty());
  setTitle(typeString + " signal height pdf plot collection");

  m_particleComboBox->addItem("all particles");
  addPlot(new SignalHeightPdfPlot(type, Enums::NoParticle, xBins, yBins));

  Enums::ParticleIterator end = Enums::particleEnd();
  for (Enums::ParticleIterator it = Enums::particleBegin(); it != end; ++it)
    if ((it.key() != Enums::NoParticle) && ((it.key() & m_particles) == it.key())) {
      m_particleComboBox->addItem(it.value());
      addPlot(new SignalHeightPdfPlot(type, it.key(), xBins, yBins));
    }
  connect(m_particleComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));
}

SignalHeightPdfPlotCollection::~SignalHeightPdfPlotCollection()
{
}

void SignalHeightPdfPlotCollection::update()
{
  if (m_particleComboBox->currentText() == "all particles") {
    selectPlot(0);
    return;
  }
  Enums::ParticleIterator end = Enums::particleEnd();
  Enums::ParticleIterator it = Enums::particleBegin();
  for (int i = 1; it != end; ++it) {
    if ((it.key() == Enums::NoParticle) || ((it.key() & m_particles) != it.key()))
      continue;
    if (it.value() == m_particleComboBox->currentText()) {
      selectPlot(i);
      return;
    }
    ++i;
  }
}
