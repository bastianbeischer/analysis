#include "Enums.hh"

#include <QStringList>
#include <QDebug>

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
  << QPair<Enums::AnalysisTopic, QString>(Enums::Testbeam, "testbeam")
  << QPair<Enums::AnalysisTopic, QString>(Enums::LikelihoodTopic, "likelihood")
  << QPair<Enums::AnalysisTopic, QString>(Enums::FluxCalculation, "flux calculation");

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
   << QPair<Enums::Correction, QString>(Enums::PhotonTravelTime, "photon travel time")
   << QPair<Enums::Correction, QString>(Enums::TrackerSignalHeight, "tracker signal height");

const QMap<Enums::Particle, QString> Enums::s_particleMap = InitializableMap<Enums::Particle, QString>()
   << QPair<Enums::Particle, QString>(Enums::NoParticle, "no particle")
   << QPair<Enums::Particle, QString>(Enums::Proton, "p+")
   << QPair<Enums::Particle, QString>(Enums::AntiProton, "p-")
   << QPair<Enums::Particle, QString>(Enums::Helium, "alpha")
   << QPair<Enums::Particle, QString>(Enums::Electron, "e-")
   << QPair<Enums::Particle, QString>(Enums::Positron, "e+")
   << QPair<Enums::Particle, QString>(Enums::Muon, "mu-")
   << QPair<Enums::Particle, QString>(Enums::AntiMuon, "mu+")
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
  << QPair<Enums::Cut, QString>(Enums::CherenkovCut, "cherenkov cut")
  << QPair<Enums::Cut, QString>(Enums::McMomentumCut, "MC momentum cut")
  << QPair<Enums::Cut, QString>(Enums::McRigidityCut, "MC rigidity cut")
  << QPair<Enums::Cut, QString>(Enums::McBetaCut, "MC beta cut");

const QMap<Enums::LikelihoodVariable, QString> Enums::s_likelihoodVariableMap = InitializableMap<Enums::LikelihoodVariable, QString>()
  << QPair<Enums::LikelihoodVariable, QString>(Enums::SignalHeightTrackerLikelihood, "signal height tracker likelihood")
  << QPair<Enums::LikelihoodVariable, QString>(Enums::SignalHeightTrdLikelihood, "signal height TRD likelihood")
  << QPair<Enums::LikelihoodVariable, QString>(Enums::TimeOverThresholdLikelihood, "time over threshold likelihood")
  << QPair<Enums::LikelihoodVariable, QString>(Enums::TimeOfFlightLikelihood, "time of flight likelihood")
  << QPair<Enums::LikelihoodVariable, QString>(Enums::TrackerMomentumLikelihood, "tracker momentum likelihood")
  << QPair<Enums::LikelihoodVariable, QString>(Enums::CherenkovLikelihood, "cherenkov likelihood");

const QMap<Enums::KineticVariable, QString> Enums::s_kineticVariableMap = InitializableMap<Enums::KineticVariable, QString>()
  << QPair<Enums::KineticVariable, QString>(Enums::TotalEnergy, "E / GeV")
  << QPair<Enums::KineticVariable, QString>(Enums::KineticEnergy, "T / GeV")
  << QPair<Enums::KineticVariable, QString>(Enums::Momentum, "p / GeV")
  << QPair<Enums::KineticVariable, QString>(Enums::Rigidity, "R / GV")
  << QPair<Enums::KineticVariable, QString>(Enums::AbsoluteRigidity, "|R| / GV")
  << QPair<Enums::KineticVariable, QString>(Enums::Curvature, "#kappa / GV^{-1}")
  << QPair<Enums::KineticVariable, QString>(Enums::Beta, "#beta")
  << QPair<Enums::KineticVariable, QString>(Enums::InverseBeta, "#beta^{-1}")
  << QPair<Enums::KineticVariable, QString>(Enums::SquaredCurvature, "#kappa^{2} / GV^{-2}");

const QMap<Enums::ReconstructionMethod, QString> Enums::s_reconstructionMethodMap = InitializableMap<Enums::ReconstructionMethod, QString>()
  << QPair<Enums::ReconstructionMethod, QString>(Enums::Spectrometer, "spectrometer")
  << QPair<Enums::ReconstructionMethod, QString>(Enums::TOF, "TOF")
  << QPair<Enums::ReconstructionMethod, QString>(Enums::WeightedMean, "weighted mean")
  << QPair<Enums::ReconstructionMethod, QString>(Enums::Chi2, "chi2")
  << QPair<Enums::ReconstructionMethod, QString>(Enums::Likelihood, "likelihood")
  << QPair<Enums::ReconstructionMethod, QString>(Enums::SpectrometerExternalInformation, "spectrometer with external information")
  << QPair<Enums::ReconstructionMethod, QString>(Enums::TOFExternalInformation, "TOF with external information")
  << QPair<Enums::ReconstructionMethod, QString>(Enums::WeightedMeanExternalInformation, "weighted mean with external information")
  << QPair<Enums::ReconstructionMethod, QString>(Enums::Chi2ExternalInformation, "chi2 with external information")
  << QPair<Enums::ReconstructionMethod, QString>(Enums::LikelihoodExternalInformation, "likelihood with external information")
  << QPair<Enums::ReconstructionMethod, QString>(Enums::MCInformation, "MC information");

const QMap<Enums::Situation, QString> Enums::s_situationMap = InitializableMap<Enums::Situation, QString>()
  << QPair<Enums::Situation, QString>(Enums::NoSituation, "unknown")
  << QPair<Enums::Situation, QString>(Enums::KirunaMuons, "kirunamuons")
  << QPair<Enums::Situation, QString>(Enums::KirunaFlight, "kirunaflight")
  << QPair<Enums::Situation, QString>(Enums::Testbeam11, "testbeam11");

const QMap<Enums::EventFlag, QString> Enums::s_eventFlagMap = InitializableMap<Enums::EventFlag, QString>()
  << QPair<Enums::EventFlag, QString>(Enums::AllTrackerLayers, "all tracker layers flag")
  << QPair<Enums::EventFlag, QString>(Enums::InsideMagnet, "inside magnet flag")
  << QPair<Enums::EventFlag, QString>(Enums::OutsideMagnet, "outside magnet flag")
  << QPair<Enums::EventFlag, QString>(Enums::HighTransverseRigidity, "high transverse rigidity flag")
  << QPair<Enums::EventFlag, QString>(Enums::MagnetCollision, "magnet collision flag")
  << QPair<Enums::EventFlag, QString>(Enums::Albedo, "albedo flag")
  << QPair<Enums::EventFlag, QString>(Enums::Chi2Good, "chi2 good flag")
  << QPair<Enums::EventFlag, QString>(Enums::BetaGood, "beta good flag")
  << QPair<Enums::EventFlag, QString>(Enums::TrackGood, "track good flag");

// ChargeSign
const QString Enums::label(Enums::ChargeSign key) {return s_chargeSignMap.value(key);}
Enums::ChargeSign Enums::chargeSign(const QString& value) {return s_chargeSignMap.key(value);}

// AnalysisTopic
const QString Enums::label(Enums::AnalysisTopic key) {return s_analysisTopicMap.value(key);}
Enums::AnalysisTopic Enums::analysisTopic(const QString& value) {return s_analysisTopicMap.key(value);}
Enums::AnalysisTopicIterator Enums::analysisTopicBegin() {return s_analysisTopicMap.constBegin();}
Enums::AnalysisTopicIterator Enums::analysisTopicEnd() {return s_analysisTopicMap.constEnd();}
const QString Enums::label(Enums::AnalysisTopics keys)
{
  QString list;
  for (Enums::AnalysisTopicIterator it = analysisTopicBegin(); it != analysisTopicEnd(); ++it) {
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
Enums::TrackTypeIterator Enums::trackTypeBegin() {return s_trackTypeMap.constBegin();}
Enums::TrackTypeIterator Enums::trackTypeEnd() {return s_trackTypeMap.constEnd();}

// Correction
const QString Enums::label(Enums::Correction key) {return s_correctionMap.value(key);}
Enums::Correction Enums::correction(const QString& value) {return s_correctionMap.key(value);}
Enums::CorrectionIterator Enums::correctionBegin() {return s_correctionMap.constBegin();}
Enums::CorrectionIterator Enums::correctionEnd() {return s_correctionMap.constEnd();}
const QString Enums::label(Enums::Corrections keys)
{
  QString list;
  for (Enums::CorrectionIterator it = correctionBegin(); it != correctionEnd(); ++it) {
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
Enums::ParticleIterator Enums::particleBegin() {return s_particleMap.constBegin();}
Enums::ParticleIterator Enums::particleEnd() {return s_particleMap.constEnd();}
const QString Enums::label(Enums::Particles keys)
{
  QString list;
  for (Enums::ParticleIterator it = particleBegin(); it != particleEnd(); ++it) {
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
const QVector<Enums::Particle> Enums::particleVector(Enums::Particles particles)
{
  QVector<Enums::Particle> vector;
  for (Enums::ParticleIterator it = particleBegin(); it != particleEnd(); ++it) {
    if (it.key() != Enums::NoParticle && (it.key() & particles)) {
      vector.append(it.key());
    }
  }
  return vector;
}
const QVector<Enums::Particle> Enums::particleVector(const QString& value)
{
  return Enums::particleVector(Enums::particles(value));
}
Enums::Particle Enums::particle(Enums::Particles particles)
{
  QVector<Enums::Particle> vector = Enums::particleVector(particles);
  Q_ASSERT(vector.count() == 1);
  return vector.first();
}

// Cut
const QString Enums::label(Enums::Cut key) {return s_cutMap.value(key);}
Enums::Cut Enums::cut(const QString& value) {return s_cutMap.key(value);}
Enums::CutIterator Enums::cutBegin() {return s_cutMap.constBegin();}
Enums::CutIterator Enums::cutEnd() {return s_cutMap.constEnd();}

// LikelihoodVariable
const QString Enums::label(Enums::LikelihoodVariable key) {return s_likelihoodVariableMap.value(key);}
Enums::LikelihoodVariable Enums::likelihoodVariable(const QString& value) {return s_likelihoodVariableMap.key(value);}
bool Enums::isInternalLikelihoodVariable(LikelihoodVariable variable)
{
  return variable != CherenkovLikelihood;
}
Enums::LikelihoodVariableIterator Enums::likelihoodVariableBegin() {return s_likelihoodVariableMap.constBegin();}
Enums::LikelihoodVariableIterator Enums::likelihoodVariableEnd() {return s_likelihoodVariableMap.constEnd();}
const QString Enums::label(Enums::LikelihoodVariables keys)
{
  QString list;
  for (Enums::LikelihoodVariableIterator it = likelihoodVariableBegin(); it != likelihoodVariableEnd(); ++it) {
    if (keys & it.key()) {
      if (!list.isEmpty())
        list+= " | ";
      list+= it.value();
    }
  }
  return list;
}
Enums::LikelihoodVariables Enums::likelihoodVariables(const QString& value)
{
  Enums::LikelihoodVariables variables;
  foreach (QString string, value.split(" | "))
    variables|= likelihoodVariable(string);
  return variables;
}
const QVector<Enums::LikelihoodVariable> Enums::likelihoodVariableVector(Enums::LikelihoodVariables likelihoods)
{
  QVector<Enums::LikelihoodVariable> vector;
  for (Enums::LikelihoodVariableIterator it = Enums::likelihoodVariableBegin(); it != Enums::likelihoodVariableEnd(); ++it) {
    if (it.key() != Enums::UndefinedLikelihood && (it.key() & likelihoods)) {
      vector.append(it.key());
    }
  }
  return vector;
}
const QVector<Enums::LikelihoodVariable> Enums::likelihoodVariableVector(const QString value)
{
  return Enums::likelihoodVariableVector(Enums::likelihoodVariables(value));
}

// KineticVariable
const QString Enums::label(Enums::KineticVariable key) {return s_kineticVariableMap.value(key);}
Enums::KineticVariable Enums::kineticVariable(const QString& value) {return s_kineticVariableMap.key(value);}
Enums::KineticVariableIterator Enums::kineticVariableBegin() {return s_kineticVariableMap.constBegin();}
Enums::KineticVariableIterator Enums::kineticVariableEnd() {return s_kineticVariableMap.constEnd();}

// ReconstructionMethod
const QString Enums::label(Enums::ReconstructionMethod key) {return s_reconstructionMethodMap.value(key);}
Enums::ReconstructionMethod Enums::reconstructionMethod(const QString& value) {return s_reconstructionMethodMap.key(value);}
bool Enums::isInternalReconstructionMethod(ReconstructionMethod method)
{
  Q_ASSERT(method != UndefinedReconstructionMethod);
  return method == Spectrometer || method == TOF || method == WeightedMean || method == Chi2 || method == Likelihood;
}
Enums::ReconstructionMethodIterator Enums::reconstructionMethodBegin() {return s_reconstructionMethodMap.constBegin();}
Enums::ReconstructionMethodIterator Enums::reconstructionMethodEnd() {return s_reconstructionMethodMap.constEnd();}
const QString Enums::label(Enums::ReconstructionMethods keys)
{
  QString list;
  for (Enums::ReconstructionMethodIterator it = reconstructionMethodBegin(); it != reconstructionMethodEnd(); ++it) {
    if (keys & it.key()) {
      if (!list.isEmpty())
        list+= " | ";
      list+= it.value();
    }
  }
  return list;
}
Enums::ReconstructionMethods Enums::reconstructionMethods(const QString& value)
{
  Enums::ReconstructionMethods variables;
  foreach (QString string, value.split(" | "))
    variables|= reconstructionMethod(string);
  return variables;
}
const QVector<Enums::ReconstructionMethod> Enums::reconstructionMethodVector(Enums::ReconstructionMethods methods)
{
  QVector<Enums::ReconstructionMethod> vector;
  for (Enums::ReconstructionMethodIterator it = Enums::reconstructionMethodBegin(); it != Enums::reconstructionMethodEnd(); ++it) {
    if (it.key() != Enums::UndefinedReconstructionMethod && (it.key() & methods)) {
      vector.append(it.key());
    }
  }
  return vector;
}
const QVector<Enums::ReconstructionMethod> Enums::reconstructionMethodVector(const QString value)
{
  return Enums::reconstructionMethodVector(Enums::reconstructionMethods(value));
}

// Situation
const QString Enums::label(Enums::Situation key) {return s_situationMap.value(key);}
Enums::Situation Enums::situation(const QString& value) {return s_situationMap.key(value);}
QMap<Enums::Situation, QString>::ConstIterator Enums::situationBegin() {return s_situationMap.constBegin();}
QMap<Enums::Situation, QString>::ConstIterator Enums::situationEnd() {return s_situationMap.constEnd();}

// EventFlag
const QString Enums::label(Enums::EventFlag key) {return s_eventFlagMap.value(key);}
Enums::EventFlag Enums::eventFlag(const QString& value) {return s_eventFlagMap.key(value);}
Enums::EventFlagIterator Enums::eventFlagBegin() {return s_eventFlagMap.constBegin();}
Enums::EventFlagIterator Enums::eventFlagEnd() {return s_eventFlagMap.constEnd();}
const QString Enums::label(Enums::EventFlags keys)
{
  QString list;
  for (Enums::EventFlagIterator it = eventFlagBegin(); it != eventFlagEnd(); ++it) {
    if (keys & it.key()) {
      if (!list.isEmpty())
        list+= " | ";
      list+= it.value();
    }
  }
  return list;
}
Enums::EventFlags Enums::eventFlags(const QString& value)
{
  Enums::EventFlags variables;
  foreach (QString string, value.split(" | "))
    variables|= eventFlag(string);
  return variables;
}
