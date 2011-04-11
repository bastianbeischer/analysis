#ifndef AnalysisProcessor_hh
#define AnalysisProcessor_hh

#include "EventProcessor.hh"

#include "Track.hh"
#include "Corrections.hh"
#include "ParticleFilter.hh"

class Particle;
class TrackFinding;
class ParticleIdentifier;

class AnalysisProcessor : public EventProcessor {
public:
  AnalysisProcessor();
  AnalysisProcessor(QVector<EventDestination*>, Track::Type, Corrections::Flags);
  ~AnalysisProcessor();

  void setTrackType(Track::Type);
  void setCorrectionFlags(Corrections::Flags);
  void setParticleFilter(ParticleFilter::Types);

  void process(SimpleEvent*);

private:
  Particle* m_particle;
  ParticleFilter* m_filter;
  TrackFinding* m_trackFinding;
  Corrections* m_corrections;
  ParticleIdentifier* m_identifier;
};

#endif
