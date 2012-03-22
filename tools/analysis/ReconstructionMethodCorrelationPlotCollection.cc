#include "ReconstructionMethodCorrelationPlotCollection.hh"
#include "ReconstructionMethodCorrelationPlot.hh"

#include "Constants.hh"

#include <QLayout>
#include <QHBoxLayout>
#include <QDebug>

ReconstructionMethodCorrelationPlotCollection::ReconstructionMethodCorrelationPlotCollection(Enums::ReconstructionMethods methods, Enums::Particles particles)
  : PlotCollection(Enums::MomentumReconstruction)
  , m_particles(particles)
  , m_particleComboBox(new QComboBox())
  , m_reconstructionMethodXComboBox(new QComboBox())
  , m_reconstructionMethodYComboBox(new QComboBox())
{
  addRequiredEventFlags(Enums::TrackGood | Enums::Chi2Good | Enums::AllTrackerLayers | Enums::InsideMagnet | Enums::BetaGood);

  QHBoxLayout* layout = new QHBoxLayout();
  layout->addStretch();
  layout->addWidget(m_particleComboBox);
  layout->addWidget(m_reconstructionMethodXComboBox);
  layout->addWidget(m_reconstructionMethodYComboBox);
  QWidget* widget = new QWidget();
  widget->setLayout(layout);
  secondaryWidget()->layout()->addWidget(widget);

  setTitle("reconstruction method correlation");

  Enums::ParticleIterator particleEnd = Enums::particleEnd();
  for (Enums::ParticleIterator it = Enums::particleBegin(); it != particleEnd; ++it) {
    if ((it.key() & m_particles) != it.key())
      continue;
    m_particleComboBox->addItem(it.value());
  }
  Enums::ReconstructionMethodIterator reconstructionEnd = Enums::reconstructionMethodEnd();
  for (Enums::ReconstructionMethodIterator it = Enums::reconstructionMethodBegin(); it != reconstructionEnd; ++it) {
    if (it.key() & methods) {
      m_reconstructionMethodXComboBox->addItem(it.value());
      m_reconstructionMethodYComboBox->addItem(it.value());
    }
  }
  m_reconstructionMethodXComboBox->setCurrentIndex(0);
  m_reconstructionMethodYComboBox->setCurrentIndex(0);

  connect(m_particleComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));
  connect(m_reconstructionMethodXComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));
  connect(m_reconstructionMethodYComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));

  for (Enums::ParticleIterator pIt = Enums::particleBegin(); pIt != particleEnd; ++pIt) {
    if ((pIt.key() & m_particles) != pIt.key())
      continue;
    for (Enums::ReconstructionMethodIterator xIt = Enums::reconstructionMethodBegin(); xIt != reconstructionEnd; ++xIt) {
      if (xIt.key() & methods) {
        for (Enums::ReconstructionMethodIterator yIt = Enums::reconstructionMethodBegin(); yIt != reconstructionEnd; ++yIt) {
          if (yIt.key() & methods) {
            addPlot(new ReconstructionMethodCorrelationPlot(xIt.key(), yIt.key(), pIt.key()));
          }
        }
      }
    }
  }
}

ReconstructionMethodCorrelationPlotCollection::~ReconstructionMethodCorrelationPlotCollection()
{
}

void ReconstructionMethodCorrelationPlotCollection::update()
{
  Enums::Particle p = Enums::particle(m_particleComboBox->currentText());
  Enums::ReconstructionMethod x = Enums::reconstructionMethod(m_reconstructionMethodXComboBox->currentText());
  Enums::ReconstructionMethod y = Enums::reconstructionMethod(m_reconstructionMethodYComboBox->currentText());

  for (int i = 0; i < numberOfPlots(); ++i) {
    ReconstructionMethodCorrelationPlot* methodPlot = static_cast<ReconstructionMethodCorrelationPlot*>(plot(i));
    if (methodPlot->particle() == p && methodPlot->methodX() == x && methodPlot->methodY() == y) {
      selectPlot(i);
      PlotCollection::update();
      return;
    }
  }
  Q_ASSERT(false);
}
