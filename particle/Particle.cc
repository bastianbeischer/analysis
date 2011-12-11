#include "Particle.hh"

#include "ParticleDB.hh"
#include "ParticleProperties.hh"
#include "ParticleInformation.hh"
#include "TimeOfFlight.hh"
#include "TRDReconstruction.hh"
#include "Constants.hh"

#include "CenteredBrokenLine.hh"
#include "CenteredBrokenLine2D.hh"
#include "BrokenLine.hh"
#include "StraightLine.hh"
#include "TrackFinding.hh"
#include "Hypothesis.hh"

#include "QDebug"

Particle::Particle()
  : m_properties(0)
  , m_track(0)
  , m_tof(0)
  , m_trd(0)
  , m_information(0)
  , m_method(Enums::UndefinedReconstructionMethod)
  , m_hypotheses()
{
  init();
}

Particle::Particle(const Enums::Particle& type)
  : m_properties(0)
  , m_track(0)
  , m_tof(0)
  , m_trd(0)
  , m_information(0)
  , m_method(Enums::UndefinedReconstructionMethod)
  , m_hypotheses()
{
  init();
  setType(type);
}

Particle::Particle(const int& pdgId)
  : m_properties(0)
  , m_track(0)
  , m_tof(0)
  , m_trd(0)
  , m_information(0)
  , m_method(Enums::UndefinedReconstructionMethod)
  , m_hypotheses()
{
  init();
  setPdgId(pdgId);
}

Particle::~Particle()
{
  delete m_track;
  delete m_tof;
  delete m_trd;
  delete m_information;
}

void Particle::init()
{
  m_properties = ParticleDB::instance()->lookupType(Enums::NoParticle);
  m_track = new CenteredBrokenLine;
  m_tof = new TimeOfFlight;
  m_trd = new TRDReconstruction;
  m_information = new ParticleInformation(this);
  m_method = Enums::UndefinedReconstructionMethod;
  Q_ASSERT(m_hypotheses.size() == 0);
}

void Particle::reset()
{
  m_properties = ParticleDB::instance()->lookupType(Enums::NoParticle);
  if (m_track)
    m_track->reset();
  m_tof->reset();
  m_trd->reset();
  m_information->reset();
  QMap<Enums::ReconstructionMethod, Hypothesis*>::Iterator end = m_hypotheses.end();
  for (QMap<Enums::ReconstructionMethod, Hypothesis*>::Iterator it = m_hypotheses.begin(); it != end; ++it)
    delete it.value();
  m_hypotheses.clear();
}

void Particle::setType(const Enums::Particle& type)
{
  m_properties = ParticleDB::instance()->lookupType(type);
}

void Particle::setPdgId(const int& pdgId)
{
  m_properties = ParticleDB::instance()->lookupPdgId(pdgId);
}

void Particle::setTrackType(const Enums::TrackType& trackType)
{
  if (m_track)
    delete m_track;

  if (trackType == Enums::NoTrack)
    m_track = 0;
  if (trackType == Enums::CenteredBrokenLine)
    m_track = new CenteredBrokenLine;
  else if (trackType == Enums::CenteredBrokenLine2D)
    m_track = new CenteredBrokenLine2D;
  else if (trackType == Enums::BrokenLine)
    m_track = new BrokenLine;
  else if (trackType == Enums::StraightLine)
    m_track = new StraightLine;
}

void Particle::setReconstructionMethod(Enums::ReconstructionMethod method)
{
  m_method = method;
}

Enums::ReconstructionMethod Particle::reconstructionMethod() const
{
  return m_method;
}

void Particle::addHypothesis(Enums::ReconstructionMethod method, Hypothesis* h)
{
  Q_ASSERT(hypothesis(h->particle(), method) == 0);
  m_hypotheses.insertMulti(method, h);
}

const QMap<Enums::ReconstructionMethod, Hypothesis*>& Particle::hypotheses() const
{
  return m_hypotheses;
}

const Hypothesis* Particle::hypothesis() const
{
  return hypothesis(m_method);
}

const Hypothesis* Particle::hypothesis(Enums::ReconstructionMethod method) const
{
  const Hypothesis* hypothesis = 0;
  QMap<Enums::ReconstructionMethod, Hypothesis*>::ConstIterator upperBound = m_hypotheses.upperBound(method);
  for (QMap<Enums::ReconstructionMethod, Hypothesis*>::ConstIterator it = m_hypotheses.lowerBound(method); it != upperBound; ++it) {
    Q_ASSERT(it.key() == method);
    if (!hypothesis) {
      hypothesis = it.value();
    } else if (it.value()->logLikelihood() < hypothesis->logLikelihood()) {
      hypothesis = it.value();
    }
  }
  return hypothesis;
}

const Hypothesis* Particle::hypothesis(Enums::Particle particle) const
{
  return hypothesis(particle, m_method);
}

const Hypothesis* Particle::hypothesis(Enums::Particle particle, Enums::ReconstructionMethod method) const
{
  QMap<Enums::ReconstructionMethod, Hypothesis*>::ConstIterator upperBound = m_hypotheses.upperBound(method);
  for (QMap<Enums::ReconstructionMethod, Hypothesis*>::ConstIterator it = m_hypotheses.lowerBound(method); it != upperBound; ++it) {
    if (it.value()->particle() == particle)
      return it.value(); 
  }
  return 0;
}

double Particle::transverseMomentum() const
{
  if (m_track)
    return m_properties->charge()*m_track->transverseRigidity();
  return 0.;
}

double Particle::momentum() const
{
  if (m_track)
    return m_properties->charge()*m_track->rigidity();
  return 0.;
}

double Particle::beta() const
{
  if (m_track)
    return m_track->trackLength() / (m_tof->timeOfFlight() * Constants::speedOfLight);
  return 0.;
}

//////////////////////////////////////////////////
// loopthrough functions for convenience

Enums::Particle Particle::type() const
{
  return m_properties->type();
}

int Particle::pdgId() const
{
  return m_properties->pdgId();
}

QString Particle::name() const
{
  return m_properties->name();
}

double Particle::mass() const
{
  return m_properties->mass();
}

int Particle::charge() const
{
  return m_properties->charge();
}

Color_t Particle::color() const
{
  return m_properties->color();
}
