#ifndef AnalysisProcessor_hh
#define AnalysisProcessor_hh

#include "EventProcessor.hh"

#include "Track.hh"
#include "Corrections.hh"
#include "ParticleFilter.hh"
#include "CutFilter.hh"

class Particle;
class TrackFinding;
class ParticleIdentifier;
class MCFilter;

class AnalysisProcessor : public EventProcessor {
public:
  AnalysisProcessor();
  AnalysisProcessor(QVector<EventDestination*>, Track::Type, Corrections::Flags);
  ~AnalysisProcessor();

  void setTrackType(Track::Type);
  void setCorrectionFlags(Corrections::Flags);
  void setParticleFilter(ParticleFilter::Types);
  void setMCFilter(QList<int> pdgIDs);
  void setCutFilter(CutFilter cuts);

  void process(SimpleEvent*);

private:
  Particle* m_particle;
  ParticleFilter* m_filter;
  MCFilter* m_mcFilter;
  TrackFinding* m_trackFinding;
  Corrections* m_corrections;
  ParticleIdentifier* m_identifier;
  CutFilter* m_cuts;
};

#endif
