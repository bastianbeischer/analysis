#include "Enums.hh"

#include <QStringList>

template <class Key, class T> class InitializableMap : public QMap<Key, T> {
public:
  inline InitializableMap<Key, T>& operator<< (const QPair<Key, T>& pair) {
    insert(pair.first, pair.second);
    return *this;
  }
};

const QMap<Enums::ChargeSign, QString> Enums::s_chargeSignMap = InitializableMap<Enums::ChargeSign, QString>()
  << QPair<Enums::ChargeSign, QString>(Enums::Positive, "positive")
  << QPair<Enums::ChargeSign, QString>(Enums::Negative, "negative");

const QMap<Enums::AnalysisTopic, QString> Enums::s_analysisTopicMap = InitializableMap<Enums::AnalysisTopic, QString>()
  << QPair<Enums::AnalysisTopic, QString>(Enums::SignalHeightTracker, "signal height tracker")
  << QPair<Enums::AnalysisTopic, QString>(Enums::SignalHeightTRD, "signal height TRD")
  << QPair<Enums::AnalysisTopic, QString>(Enums::ClusterShapeTracker, "cluster shape tracker")
  << QPair<Enums::AnalysisTopic, QString>(Enums::ClusterShapeTRD, "cluster shape TRD")
  << QPair<Enums::AnalysisTopic, QString>(Enums::TimeOverThreshold, "time over threshold")
  << QPair<Enums::AnalysisTopic, QString>(Enums::Tracking, "tracking")
  << QPair<Enums::AnalysisTopic, QString>(Enums::Occupancy, "occupancy")
  << QPair<Enums::AnalysisTopic, QString>(Enums::ResidualsTracker, "residuals tracker")
  << QPair<Enums::AnalysisTopic, QString>(Enums::ResidualsTRD, "residuals TRD")
  << QPair<Enums::AnalysisTopic, QString>(Enums::MomentumReconstruction, "momentum")
  << QPair<Enums::AnalysisTopic, QString>(Enums::EfficiencyTOF, "efficiency TOF")
  << QPair<Enums::AnalysisTopic, QString>(Enums::ResolutionTOF, "resolution TOF")
  << QPair<Enums::AnalysisTopic, QString>(Enums::CalibrationTOF, "calibration TOF")
  << QPair<Enums::AnalysisTopic, QString>(Enums::MiscellaneousTracker, "miscellaneous tracker")
  << QPair<Enums::AnalysisTopic, QString>(Enums::MiscellaneousTRD, "miscellaneous TRD")
  << QPair<Enums::AnalysisTopic, QString>(Enums::MiscellaneousTOF, "miscellaneous TOF")
  << QPair<Enums::AnalysisTopic, QString>(Enums::SlowControl, "slow control")
  << QPair<Enums::AnalysisTopic, QString>(Enums::MonteCarloTracker, "monte carlo tracker")
  << QPair<Enums::AnalysisTopic, QString>(Enums::MonteCarloTRD, "monte carlo TRD")
  << QPair<Enums::AnalysisTopic, QString>(Enums::MonteCarloTOF, "monte carlo TOF")
  << QPair<Enums::AnalysisTopic, QString>(Enums::MonteCarlo, "monte carlo")
  << QPair<Enums::AnalysisTopic, QString>(Enums::Testbeam, "testbeam");

const QMap<Enums::TrackType, QString> Enums::s_trackTypeMap = InitializableMap<Enums::TrackType, QString>()
  << QPair<Enums::TrackType, QString>(Enums::NoTrack, "no track")
  << QPair<Enums::TrackType, QString>(Enums::StraightLine, "straight line")
  << QPair<Enums::TrackType, QString>(Enums::BrokenLine, "broken line")
  << QPair<Enums::TrackType, QString>(Enums::CenteredBrokenLine, "centered broken line")
  << QPair<Enums::TrackType, QString>(Enums::CenteredBrokenLine2D, "centered broken line 2D");

const QMap<Enums::Correction, QString> Enums::s_correctionMap = InitializableMap<Enums::Correction, QString>()
   << QPair<Enums::Correction, QString>(Enums::NoCorrection, "no correction")
   << QPair<Enums::Correction, QString>(Enums::Alignment, "alignment")
   << QPair<Enums::Correction, QString>(Enums::TimeShifts, "time shifts")
   << QPair<Enums::Correction, QString>(Enums::TrdMopv, "TRD mop value")
   << QPair<Enums::Correction, QString>(Enums::TrdTime, "TRD time")
   //<< QPair<Enums::Correction, QString>(Enums::TrdPressure, "TRD pressure")
   //<< QPair<Enums::Correction, QString>(Enums::TrdTemperature, "TRD temperature")
   << QPair<Enums::Correction, QString>(Enums::TofTimeOverThreshold, "time over threshold")
   << QPair<Enums::Correction, QString>(Enums::MultipleScattering, "multiple scattering")
   << QPair<Enums::Correction, QString>(Enums::PhotonTravelTime, "photon travel time");

const QMap<Enums::Particle, QString> Enums::s_particleMap = InitializableMap<Enums::Particle, QString>()
   << QPair<Enums::Particle, QString>(Enums::NoParticle, "no particle")
   << QPair<Enums::Particle, QString>(Enums::Proton, "p+")
   << QPair<Enums::Particle, QString>(Enums::AntiProton, "p-")
   << QPair<Enums::Particle, QString>(Enums::Helium, "alpha")
   << QPair<Enums::Particle, QString>(Enums::Electron, "e-")
   << QPair<Enums::Particle, QString>(Enums::Positron, "e+")
   << QPair<Enums::Particle, QString>(Enums::Muon, "mu+")
   << QPair<Enums::Particle, QString>(Enums::AntiMuon, "mu-")
   << QPair<Enums::Particle, QString>(Enums::PiPlus, "pi+")
   << QPair<Enums::Particle, QString>(Enums::PiMinus, "pi-")
   << QPair<Enums::Particle, QString>(Enums::Photon, "gamma")
   << QPair<Enums::Particle, QString>(Enums::Pi0, "pi0")
   << QPair<Enums::Particle, QString>(Enums::Higgs, "H");

const QMap<Enums::Cut, QString> Enums::s_cutMap = InitializableMap<Enums::Cut, QString>()
  << QPair<Enums::Cut, QString>(Enums::RigidityCut, "rigidity cut")
  << QPair<Enums::Cut, QString>(Enums::BetaCut, "beta cut")
  << QPair<Enums::Cut, QString>(Enums::TimeOverThresholdCut, "TOT cut")
  << QPair<Enums::Cut, QString>(Enums::TrdSignalCut, "TRD signal cut")
  << QPair<Enums::Cut, QString>(Enums::CherenkovCut, "cherenkov cut");

// ChargeSign
const QString Enums::label(Enums::ChargeSign key) {return s_chargeSignMap.value(key);}
Enums::ChargeSign Enums::chargeSign(const QString& value) {return s_chargeSignMap.key(value);}

// AnalysisTopic
const QString Enums::label(Enums::AnalysisTopic key) {return s_analysisTopicMap.value(key);}
Enums::AnalysisTopic Enums::analysisTopic(const QString& value) {return s_analysisTopicMap.key(value);}
QMap<Enums::AnalysisTopic, QString>::ConstIterator Enums::analysisTopicBegin() {return s_analysisTopicMap.constBegin();}
QMap<Enums::AnalysisTopic, QString>::ConstIterator Enums::analysisTopicEnd() {return s_analysisTopicMap.constEnd();}
const QString Enums::label(Enums::AnalysisTopics keys)
{
  QString list;
  for (QMap<Enums::AnalysisTopic, QString>::ConstIterator it = analysisTopicBegin(); it != analysisTopicEnd(); ++it) {
    if (keys & it.key()) {
      if (!list.isEmpty())
        list+= " | ";
      list+= it.value();
    }
  }
  return list;
}
Enums::AnalysisTopics Enums::analysisTopics(const QString& value)
{
  Enums::AnalysisTopics analysisTopics;
  foreach (QString string, value.split(" | "))
    analysisTopics|= analysisTopic(string);
  return analysisTopics;
}

// TrackType
const QString Enums::label(Enums::TrackType key) {return s_trackTypeMap.value(key);}
Enums::TrackType Enums::trackType(const QString& value) {return s_trackTypeMap.key(value);}
QMap<Enums::TrackType, QString>::ConstIterator Enums::trackTypeBegin() {return s_trackTypeMap.constBegin();}
QMap<Enums::TrackType, QString>::ConstIterator Enums::trackTypeEnd() {return s_trackTypeMap.constEnd();}

// Correction
const QString Enums::label(Enums::Correction key) {return s_correctionMap.value(key);}
Enums::Correction Enums::correction(const QString& value) {return s_correctionMap.key(value);}
QMap<Enums::Correction, QString>::ConstIterator Enums::correctionBegin() {return s_correctionMap.constBegin();}
QMap<Enums::Correction, QString>::ConstIterator Enums::correctionEnd() {return s_correctionMap.constEnd();}
const QString Enums::label(Enums::Corrections keys)
{
  QString list;
  for (QMap<Enums::Correction, QString>::ConstIterator it = correctionBegin(); it != correctionEnd(); ++it) {
    if (keys & it.key()) {
      if (!list.isEmpty())
        list+= " | ";
      list+= it.value();
    }
  }
  return list;
}
Enums::Corrections Enums::corrections(const QString& value)
{
  Enums::Corrections corrections;
  foreach (QString string, value.split(" | "))
    corrections|= correction(string);
  return corrections;
}

// Particle
const QString Enums::label(Enums::Particle key) {return s_particleMap.value(key);}
Enums::Particle Enums::particle(const QString& value) {return s_particleMap.key(value);}
QMap<Enums::Particle, QString>::ConstIterator Enums::particleBegin() {return s_particleMap.constBegin();}
QMap<Enums::Particle, QString>::ConstIterator Enums::particleEnd() {return s_particleMap.constEnd();}
const QString Enums::label(Enums::Particles keys)
{
  QString list;
  for (QMap<Enums::Particle, QString>::ConstIterator it = particleBegin(); it != particleEnd(); ++it) {
    if (keys & it.key()) {
      if (!list.isEmpty())
        list+= " | ";
      list+= it.value();
    }
  }
  return list; 
}
Enums::Particles Enums::particles(const QString& value)
{
  Enums::Particles particles;
  foreach (QString string, value.split(" | "))
    particles|= particle(string);
  return particles;
}

// Cut
const QString Enums::label(Enums::Cut key) {return s_cutMap.value(key);}
Enums::Cut Enums::cut(const QString& value) {return s_cutMap.key(value);}
QMap<Enums::Cut, QString>::ConstIterator Enums::cutBegin() {return s_cutMap.constBegin();}
QMap<Enums::Cut, QString>::ConstIterator Enums::cutEnd() {return s_cutMap.constEnd();}

