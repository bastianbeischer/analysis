#ifndef AnalysisProcessorSetting_hh
#define AnalysisProcessorSetting_hh

#include "Enums.hh"
#include "Cut.hh"
#include "CutFilter.hh"

#include <QList>
#include <QString>
#include <QStringList>
#include <QDebug>

class AnalysisProcessorSetting {
public:
  AnalysisProcessorSetting();
  void clear();
  void save(const QString&) const;
  void load(const QString&);
  Enums::TrackType trackType() const {return m_trackType;}
  Enums::ReconstructionMethods reconstructionMethods() const {return m_reconstructionMethods;}
  Enums::ReconstructionMethod reconstructionMethod() const {return m_reconstructionMethod;}
  Enums::Corrections corrections() const {return m_corrections;}
  Enums::Particles particles() const {return m_particles;}
  Enums::LikelihoodVariables likelihoods() const {return m_likelihoods;}
  Enums::Particles particleFilter() const {return m_particleFilter;}
  Enums::Particles mcParticleFilter() const {return m_mcParticleFilter;}
  const CutFilter& cutFilter() const {return m_cutFilter;}
  CutFilter& cutFilter() {return m_cutFilter;}
  void setTrackType(Enums::TrackType trackType) {m_trackType = trackType;}
  void setReconstructionMethods(Enums::ReconstructionMethods reconstructionMethods) {m_reconstructionMethods = reconstructionMethods;}
  void setReconstructionMethod(Enums::ReconstructionMethod reconstructionMethod) {m_reconstructionMethod = reconstructionMethod;}
  void setCorrections(Enums::Corrections corrections) {m_corrections = corrections;}
  void setParticles(Enums::Particles particles) {m_particles = particles;}
  void setLikelihoodVariables(Enums::LikelihoodVariables likelihoods) {m_likelihoods = likelihoods;}
  void setParticleFilter(Enums::Particles particleFilter) {m_particleFilter = particleFilter;}
  void setMcParticleFilter(Enums::Particles mcParticleFilter) {m_mcParticleFilter = mcParticleFilter;}
  void setCutFilter(const CutFilter& cutFilter) {m_cutFilter = cutFilter;}
  virtual QString toString() const;
protected:
  Enums::TrackType m_trackType;
  Enums::ReconstructionMethods m_reconstructionMethods;
  Enums::ReconstructionMethod m_reconstructionMethod;
  Enums::Corrections m_corrections;
  Enums::Particles m_particles;
  Enums::LikelihoodVariables m_likelihoods;
  Enums::Particles m_particleFilter;
  Enums::Particles m_mcParticleFilter;
  CutFilter m_cutFilter;
};

QDebug operator<<(QDebug, const AnalysisProcessorSetting&);

#endif
