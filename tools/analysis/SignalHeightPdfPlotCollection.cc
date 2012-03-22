#include "SignalHeightPdfPlotCollection.hh"
#include "SignalHeightPdfPlot.hh"

#include "Track.hh"
#include "Hit.hh"
#include "Particle.hh"
#include "DetectorElement.hh"
#include "Helpers.hh"
#include "TimeOverThresholdLikelihood.hh"
#include "SignalHeightTrdLikelihood.hh"
#include "SignalHeightTrackerLikelihood.hh"

#include <QSpinBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QDebug>

SignalHeightPdfPlotCollection::SignalHeightPdfPlotCollection(Hit::ModuleType type, Enums::Particles particles)
  : PlotCollection(Enums::LikelihoodTopic)
  , m_type(type)
  , m_particleComboBox(new QComboBox)
  , m_layerComboBox(new QComboBox)
  , m_signalHeightPdfPlots()
{
  addRequiredEventFlags(Enums::TrackGood | Enums::Chi2Good | Enums::AllTrackerLayers | Enums::InsideMagnet | Enums::BetaGood);

  QHBoxLayout* layout = new QHBoxLayout();
  layout->addStretch();
  layout->addWidget(m_particleComboBox);
  layout->addWidget(m_layerComboBox);
  QWidget* widget = new QWidget();
  widget->setLayout(layout);
  secondaryWidget()->layout()->addWidget(widget);

  QString typeString;
  QString axisTitle;
  QVector<double> xBins = Helpers::logBinning(20, 0.1, 10);
  QVector<double> yBins;
  Likelihood* lh = 0;
  if (type == Hit::tracker) {
    typeString = "tracker";
    lh = new SignalHeightTrackerLikelihood();
    yBins = Helpers::linearBinning(50, lh->measuredValueMin(), lh->measuredValueMax());
    axisTitle = lh->measuredValueAxisTitle();
  } else if (type == Hit::tof) {
    typeString = "tof";
    lh = new TimeOverThresholdLikelihood();
    yBins = Helpers::linearBinning(60, lh->measuredValueMin(), lh->measuredValueMax());
    axisTitle = lh->measuredValueAxisTitle();
  } else if (type == Hit::trd) {
    typeString = "trd";
    lh = new SignalHeightTrdLikelihood();
    yBins = Helpers::linearBinning(100, lh->measuredValueMin(), lh->measuredValueMax());
    axisTitle = lh->measuredValueAxisTitle();
  }
  if (lh)
    delete lh;
  Q_ASSERT(!typeString.isEmpty());
  setTitle(typeString + " signal height pdf plot collection");

  m_particleComboBox->addItem(Enums::label(particles));
  m_layerComboBox->addItem("all layers");
  m_signalHeightPdfPlots.append(new SignalHeightPdfPlot(axisTitle, type, particles, xBins, yBins));

  Enums::ParticleIterator end = Enums::particleEnd();
  for (Enums::ParticleIterator it = Enums::particleBegin(); it != end; ++it)
    if ((it.key() != Enums::NoParticle) && ((it.key() & particles) == it.key())) {
      m_particleComboBox->addItem(it.value());
      if (type != Hit::trd || (it.key() != Enums::Electron && it.key() != Enums::Positron))
        m_signalHeightPdfPlots.append(new SignalHeightPdfPlot(axisTitle, type, it.key(), xBins, yBins));
      else for (int layer = 0; layer < 8; ++layer)
        m_signalHeightPdfPlots.append(new SignalHeightPdfPlot(axisTitle, type, it.key(), xBins, yBins, layer));
    }
  foreach (SignalHeightPdfPlot* plot, m_signalHeightPdfPlots)
    addPlot(plot);
  connect(m_particleComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(particleChanged()));
  connect(m_layerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));
}

SignalHeightPdfPlotCollection::~SignalHeightPdfPlotCollection()
{
}

void SignalHeightPdfPlotCollection::particleChanged()
{
  QVector<Enums::Particle> particles = Enums::particleVector(m_particleComboBox->currentText());
  m_layerComboBox->disconnect();
  m_layerComboBox->clear();
  if (particles.count() > 1 || m_type != Hit::trd || (particles.first() != Enums::Electron && particles.first() != Enums::Positron))
    m_layerComboBox->addItem("all layers");
  else for (int layer = 0; layer < 8; ++layer)
    m_layerComboBox->addItem(QString("layer %1").arg(layer));
  connect(m_layerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));
  update();
}

void SignalHeightPdfPlotCollection::update()
{
  Enums::Particles particles = Enums::particles(m_particleComboBox->currentText());
  int layer = -1;
  QString string = m_layerComboBox->currentText();
  if (string != "all layers") {
    string.remove("layer ");
    layer = string.toInt();
  }
  for (int i = 0; i < m_signalHeightPdfPlots.count(); ++i) {
    SignalHeightPdfPlot* plot = m_signalHeightPdfPlots[i];
    if (plot->particles() == particles && plot->layer() == layer) {
      selectPlot(i);
      return;
    }
  }
  Q_ASSERT(false);
}
