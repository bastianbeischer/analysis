#ifndef AnalysisSetting_hh
#define AnalysisSetting_hh

#include "Enums.hh"
#include "Cut.hh"
#include "CutFilter.hh"

#include <QList>
#include <QString>
#include <QStringList>

class AnalysisSetting {
public:
  AnalysisSetting();
  void clear();
  void save(const QString&) const;
  void load(const QString&);

  int firstEvent;
  int lastEvent;
  int numberOfThreads;

  Enums::AnalysisTopics analysisTopics;
  Enums::TrackType trackType;
  Enums::ReconstructionMethod reconstructionMethod;
  Enums::Corrections corrections;
  Enums::Particles particles;
  Enums::LikelihoodVariables likelihoods;
  Enums::Particles particleFilter;
  Enums::Particles mcParticleFilter;
  CutFilter cutFilter;
};

#endif
