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

  void setTrackType(Track::Type);
  void setCorrectionFlags(Corrections::Flags);
  void setParticleFilter(ParticleFilter::Types);
  void setMCFilter(MCFilter::Types);
  void setCutFilter(CutFilter cuts);
  void setCherenkovCut(const Cut&);

  void process(SimpleEvent*);

  const Particle* particle() const {return m_particle;}
  const ParticleFilter* particleFilter() const {return m_filter;}
  const MCFilter* mcFilter() const {return m_mcFilter;}
  const TrackFinding* trackFinding() const {return m_trackFinding;}
  const Corrections* corrections() const {return m_corrections;}
  const ParticleIdentifier* particleIdentifier() const {return m_identifier;}
  const CutFilter* cutFilter() const {return m_cuts;}
  const Cut cherenkovCut() const {return m_CherenkovCut;}

private:
  Particle* m_particle;
  ParticleFilter* m_filter;
  MCFilter* m_mcFilter;
  TrackFinding* m_trackFinding;
  Corrections* m_corrections;
  ParticleIdentifier* m_identifier;
  CutFilter* m_cuts;
  Cut m_CherenkovCut;
};

#endif
