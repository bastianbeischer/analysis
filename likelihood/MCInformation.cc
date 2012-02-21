#include "MCInformation.hh"
#include "TimeOfFlight.hh"
#include "Likelihood.hh"
#include "KineticVariable.hh"
#include "AnalyzedEvent.hh"
#include "Particle.hh"
#include "ParticleProperties.hh"
#include "Setup.hh"
#include "Helpers.hh"
#include "SimpleEvent.hh"

#include <Math/IFunction.h>
#include <Math/BrentMinimizer1D.h>
#include <Math/Functor.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TAxis.h>

#include <cfloat>
#include <cmath>

MCInformation::MCInformation(Enums::LikelihoodVariables likelihoods, Enums::Particles particles)
  : Reconstruction(likelihoods, particles)
{
  m_externalInformation = true;
  m_method = Enums::MCInformation;
  m_minima = QVector<QPointF>(m_particles.count());
}

MCInformation::~MCInformation()
{
}

void MCInformation::identify(AnalyzedEvent* event)
{
  m_event = event;
  m_indexOfGlobalMinimum = -1;
  Q_ASSERT_X(event->simpleEvent()->MCInformation(), "MCInformation::identify", "Do not use MCInformation reconstruction method on non-MC data.");
  const MCSimpleEventParticle* mcParticle = event->simpleEvent()->MCInformation()->primary();
  QVector<Enums::Particle>::ConstIterator particleIt = m_particles.begin();
  QVector<Enums::Particle>::ConstIterator particleEnd = m_particles.end();
  for (int it = 0; particleIt != particleEnd; ++it, ++particleIt) {
    Q_ASSERT(*particleIt != Enums::NoParticle);
    ParticleProperties properties(*particleIt);
    Hypothesis* h = 0;
    if (mcParticle->pdgID == properties.pdgId()) {
      h = new Hypothesis(*particleIt, properties.charge() / mcParticle->initialMomentum.Mag());
      h->setLikelihood(m_likelihoods.count(), 1e9);
      m_indexOfGlobalMinimum = it;
    } else {
      h = new Hypothesis(*particleIt, 0);
      h->setLikelihood(m_likelihoods.count(), 1e-9);
    }
    event->particle()->addHypothesis(m_method, h);
  }
  Q_ASSERT_X(m_indexOfGlobalMinimum != -1, "MCInformation::identify",
    "Make sure to include the MC primary particle to the possible likelihood particles!");
}
