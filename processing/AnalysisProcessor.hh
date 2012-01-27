#ifndef AnalysisProcessor_hh
#define AnalysisProcessor_hh

#include "EventProcessor.hh"

#include "Track.hh"
#include "Corrections.hh"
#include "ParticleFilter.hh"
#include "MCFilter.hh"
#include "CutFilter.hh"
#include "Cut.hh"
#include "AnalysisProcessorSetting.hh"

#include <QMap>
#include <QVector>

class Particle;
class TrackFinding;
class ParticleIdentifier;
class Reconstruction;

class AnalysisProcessor : public EventProcessor {
public:
  AnalysisProcessor();
  AnalysisProcessor(QVector<EventDestination*>, const AnalysisProcessorSetting&);
  ~AnalysisProcessor();

  void setAnalysisProcessorSetting(const AnalysisProcessorSetting&);
  void setTrackType(Enums::TrackType);
  void setReconstructionMethod(Enums::ReconstructionMethod);
  void setCorrectionFlags(Enums::Corrections);
  void setParticleFilter(Enums::Particles);
  void setMCFilter(Enums::Particles);
  void setCutFilter(const CutFilter& cuts);

  void process(SimpleEvent*);

  const Particle* particle() const {return m_particle;}
  Enums::Particles particles() const {return m_particles;}
  const ParticleFilter* particleFilter() const {return m_particleFilter;}
  const MCFilter* mcFilter() const {return m_mcFilter;}
  const CutFilter* cutFilter() const {return m_cutFilter;}
  const TrackFinding* trackFinding() const {return m_trackFinding;}
  const Corrections* corrections() const {return m_corrections;}
  const ParticleIdentifier* particleIdentifier() const {return m_identifier;}
  const Reconstruction* reconstruction(Enums::ReconstructionMethod) const;
private:
  void initReconstructions();

  Enums::Particles m_particles;
  Enums::LikelihoodVariables m_likelihoods;
  Particle* m_particle;
  ParticleFilter* m_particleFilter;
  CutFilter* m_cutFilter;
  MCFilter* m_mcFilter;
  TrackFinding* m_trackFinding;
  Corrections* m_corrections;
  ParticleIdentifier* m_identifier;
  QMap<Enums::ReconstructionMethod, Reconstruction*> m_reconstructions;
};

#endif
