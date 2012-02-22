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
    for (Enums::ReconstructionMethodIterator xIt = Enums::reconstructionMethodBegin(); xIt != reconstructionEnd; ++xIt)
      if (xIt.key() & methods)
        for (Enums::ReconstructionMethodIterator yIt = Enums::reconstructionMethodBegin(); yIt != reconstructionEnd; ++yIt)
          if (yIt.key() & methods)
            addPlot(new ReconstructionMethodCorrelationPlot(xIt.key(), yIt.key(), pIt.key()));
  }
}

ReconstructionMethodCorrelationPlotCollection::~ReconstructionMethodCorrelationPlotCollection()
{
}

int ReconstructionMethodCorrelationPlotCollection::methodsToIndex(Enums::ReconstructionMethod x, Enums::ReconstructionMethod y, Enums::Particle p)
{
  int index = 0;
  Enums::ParticleIterator particleEnd = Enums::particleEnd();
  Enums::ReconstructionMethodIterator reconstructionEnd = Enums::reconstructionMethodEnd();
  for (Enums::ParticleIterator pIt = Enums::particleBegin(); pIt != particleEnd; ++pIt) {
    if ((pIt.key() & m_particles) != pIt.key())
      continue;
    for (Enums::ReconstructionMethodIterator xIt = Enums::reconstructionMethodBegin(); xIt != reconstructionEnd; ++xIt) {
      for (Enums::ReconstructionMethodIterator yIt = Enums::reconstructionMethodBegin(); yIt != reconstructionEnd; ++yIt) {
        if (xIt.key() == x && yIt.key() == y && pIt.key() == p)
          return index;
        ++index;
      }
    }
  }
  Q_ASSERT(false);
  return -1;
}

void ReconstructionMethodCorrelationPlotCollection::update()
{
  Enums::Particle p = Enums::particle(m_particleComboBox->currentText());
  Enums::ReconstructionMethod x = Enums::reconstructionMethod(m_reconstructionMethodXComboBox->currentText());
  Enums::ReconstructionMethod y = Enums::reconstructionMethod(m_reconstructionMethodYComboBox->currentText());
  selectPlot(methodsToIndex(x, y, p));
  PlotCollection::update();
}
