#ifndef AnalysisProcessor_hh
#define AnalysisProcessor_hh

#include "EventProcessor.hh"

#include "Track.hh"
#include "Corrections.hh"
#include "ParticleFilter.hh"
#include "MCFilter.hh"
#include "CutFilter.hh"
#include "Cut.hh"

class Particle;
class TrackFinding;
class ParticleIdentifier;


class AnalysisProcessor : public EventProcessor {
public:
  AnalysisProcessor();
  AnalysisProcessor(QVector<EventDestination*>, Track::Type, Corrections::Flags);
  ~AnalysisProcessor();

  void setTrackType(const Track::Type&);
  void setCorrectionFlags(const Corrections::Flags&);
  void setParticleFilter(const ParticleFilter::Types&);
  void setMCFilter(const MCFilter::Types&);
  void setCutFilter(const CutFilter& cuts);

  void process(SimpleEvent*);

  const Particle* particle() const {return m_particle;}
  const ParticleFilter* particleFilter() const {return m_particleFilter;}
  const MCFilter* mcFilter() const {return m_mcFilter;}
  const CutFilter* cutFilter() const {return m_cutFilter;}
  const TrackFinding* trackFinding() const {return m_trackFinding;}
  const Corrections* corrections() const {return m_corrections;}
  const ParticleIdentifier* particleIdentifier() const {return m_identifier;}

private:
  Particle* m_particle;
  ParticleFilter* m_particleFilter;
  CutFilter* m_cutFilter;
  MCFilter* m_mcFilter;
  TrackFinding* m_trackFinding;
  Corrections* m_corrections;
  ParticleIdentifier* m_identifier;
};

#endif
