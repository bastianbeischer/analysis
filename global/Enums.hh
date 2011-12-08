#ifndef Enums_hh
#define Enums_hh

#include <QMap>
#include <QPair>

class Enums {
public:
  enum ChargeSign {Positive = 1<<0, Negative = 1<<1};
  Q_DECLARE_FLAGS(ChargeSigns, ChargeSign);
  static const QString label(ChargeSign);
  static ChargeSign chargeSign(const QString&);

  enum AnalysisTopic {NoTopic = 0, SignalHeightTracker = 1<<0, SignalHeightTRD = 1<<1, ClusterShapeTracker = 1<<2, ClusterShapeTRD = 1<<3,
    TimeOverThreshold = 1<<4, Tracking = 1<<5, Occupancy = 1<<6, ResidualsTracker = 1<<7, ResidualsTRD = 1<<8, MomentumReconstruction = 1<<9,
    EfficiencyTOF = 1<<10, ResolutionTOF = 1<<11, CalibrationTOF = 1<<12, MiscellaneousTracker = 1<<13, MiscellaneousTRD = 1<<14, MiscellaneousTOF = 1<<15,
    SlowControl = 1<<16, MonteCarloTracker = 1<<17, MonteCarloTRD = 1<<18, MonteCarloTOF = 1<<19, MonteCarlo = 1<<20, Testbeam = 1<<21};
  static const QString label(AnalysisTopic);
  static AnalysisTopic analysisTopic(const QString&);
  typedef QMap<AnalysisTopic, QString>::ConstIterator AnalysisTopicIterator;
  static AnalysisTopicIterator analysisTopicBegin();
  static AnalysisTopicIterator analysisTopicEnd();
  Q_DECLARE_FLAGS(AnalysisTopics, AnalysisTopic);
  static const QString label(AnalysisTopics);
  static AnalysisTopics analysisTopics(const QString&);

  enum TrackType {NoTrack = 0, StraightLine, BrokenLine, CenteredBrokenLine, CenteredBrokenLine2D};
  static const QString label(TrackType);
  static TrackType trackType(const QString&);
  typedef QMap<TrackType, QString>::ConstIterator TrackTypeIterator;
  static TrackTypeIterator trackTypeBegin();
  static TrackTypeIterator trackTypeEnd();

  enum Correction {NoCorrection = 0, Alignment = 1<<0, TimeShifts = 1<<1, TrdMopv = 1<<2, TrdTime = 1<<3, TrdPressure = 1<<4,
    TrdTemperature = 1<<5, TofTimeOverThreshold = 1<<6, MultipleScattering = 1<<7, PhotonTravelTime = 1<<8};
  static const QString label(Correction);
  static Correction correction(const QString&);
  typedef QMap<Correction, QString>::ConstIterator CorrectionIterator;
  static CorrectionIterator correctionBegin();
  static CorrectionIterator correctionEnd();
  Q_DECLARE_FLAGS(Corrections, Correction);
  static const QString label(Corrections);
  static Corrections corrections(const QString&);

  enum Particle {NoParticle = 0, Proton = 1<<0, AntiProton = 1<<1, Helium = 1<<2, Electron = 1<<3, Positron = 1<<4,
    Muon = 1<<5, AntiMuon = 1<<6, PiPlus = 1<<7, PiMinus = 1<<8, Photon = 1<<9, Pi0 = 1<<10, Higgs = 1<<11};
  static const QString label(Particle);
  static Particle particle(const QString&);
  typedef QMap<Particle, QString>::ConstIterator ParticleIterator;
  static ParticleIterator particleBegin();
  static ParticleIterator particleEnd();
  Q_DECLARE_FLAGS(Particles, Particle);
  static const QString label(Particles);
  static Particles particles(const QString&);

  enum Cut {RigidityCut, BetaCut, TimeOverThresholdCut, TrdSignalCut, CherenkovCut};
  static const QString label(Cut);
  static Cut cut(const QString&);
  typedef QMap<Cut, QString>::ConstIterator CutIterator;
  static CutIterator cutBegin();
  static CutIterator cutEnd();

  enum LikelihoodVariable {UndefinedLikelihood = 0, SignalHeightTrackerLikelihood = 1<<0, SignalHeightTrdLikelihood = 1<<1, TimeOverThresholdLikelihood = 1<<2,
    TimeOfFlightLikelihood = 1<<3, TrackerMomentumLikelihood = 1<<4, CherenkovLikelihood = 1<<5};
  static const QString label(LikelihoodVariable);
  static LikelihoodVariable likelihoodVariable(const QString&);
  static bool isInternalLikelihoodVariable(LikelihoodVariable);
  typedef QMap<LikelihoodVariable, QString>::ConstIterator LikelihoodVariableIterator;
  static LikelihoodVariableIterator likelihoodVariableBegin();
  static LikelihoodVariableIterator likelihoodVariableEnd();
  Q_DECLARE_FLAGS(LikelihoodVariables, LikelihoodVariable);
  static const QString label(LikelihoodVariables);
  static LikelihoodVariables likelihoodVariables(const QString&);

  enum KineticVariable {UndefinedKineticVariable, TotalEnergy, KineticEnergy, Momentum, Rigidity, AbsoluteRigidity, Curvature,
    Beta, InverseBeta, SquaredCurvature};
  static const QString label(KineticVariable);
  static KineticVariable kineticVariable(const QString&);
  typedef QMap<KineticVariable, QString>::ConstIterator KineticVariableIterator;
  static KineticVariableIterator kineticVariableBegin();
  static KineticVariableIterator kineticVariableEnd();

  enum ReconstructionMethod {UndefinedReconstructionMethod, Spectrometer, TOF, WeightedMean, Chi2, Likelihood, SpectrometerExternalInformation,
    TOFExternalInformation, WeightedMeanExternalInformation, Chi2ExternalInformation, LikelihoodExternalInformation};
  static const QString label(ReconstructionMethod);
  static ReconstructionMethod reconstructionMethod(const QString&);
  typedef QMap<ReconstructionMethod, QString>::ConstIterator ReconstructionMethodIterator;
  static ReconstructionMethodIterator reconstructionMethodBegin();
  static ReconstructionMethodIterator reconstructionMethodEnd();

private:
  static const QMap<ChargeSign, QString> s_chargeSignMap;
  static const QMap<AnalysisTopic, QString> s_analysisTopicMap;
  static const QMap<TrackType, QString> s_trackTypeMap;
  static const QMap<Correction, QString> s_correctionMap;
  static const QMap<Particle, QString> s_particleMap;
  static const QMap<Cut, QString> s_cutMap;
  static const QMap<LikelihoodVariable, QString> s_likelihoodVariableMap;
  static const QMap<KineticVariable, QString> s_kineticVariableMap;
  static const QMap<ReconstructionMethod, QString> s_reconstructionMethodMap;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Enums::AnalysisTopics);
Q_DECLARE_OPERATORS_FOR_FLAGS(Enums::ChargeSigns);
Q_DECLARE_OPERATORS_FOR_FLAGS(Enums::Corrections);
Q_DECLARE_OPERATORS_FOR_FLAGS(Enums::Particles);
Q_DECLARE_OPERATORS_FOR_FLAGS(Enums::LikelihoodVariables);

#endif
