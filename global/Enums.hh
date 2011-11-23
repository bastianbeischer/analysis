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
  static QMap<AnalysisTopic, QString>::ConstIterator analysisTopicBegin();
  static QMap<AnalysisTopic, QString>::ConstIterator analysisTopicEnd();
  Q_DECLARE_FLAGS(AnalysisTopics, AnalysisTopic);
  static const QString label(AnalysisTopics);
  static AnalysisTopics analysisTopics(const QString&);

  enum TrackType {NoTrack = 0, StraightLine, BrokenLine, CenteredBrokenLine, CenteredBrokenLine2D};
  static const QString label(TrackType);
  static TrackType trackType(const QString&);
  static QMap<TrackType, QString>::ConstIterator trackTypeBegin();
  static QMap<TrackType, QString>::ConstIterator trackTypeEnd();

  enum Correction {NoCorrection = 0, Alignment = 1<<0, TimeShifts = 1<<1, TrdMopv = 1<<2, TrdTime = 1<<3, TrdPressure = 1<<4,
    TrdTemperature = 1<<5, TofTimeOverThreshold = 1<<6, MultipleScattering = 1<<7, PhotonTravelTime = 1<<8, TrackerSignalHeight = 1<<9};
  static const QString label(Correction);
  static Correction correction(const QString&);
  static QMap<Correction, QString>::ConstIterator correctionBegin();
  static QMap<Correction, QString>::ConstIterator correctionEnd();
  Q_DECLARE_FLAGS(Corrections, Correction);
  static const QString label(Corrections);
  static Corrections corrections(const QString&);

  enum Particle {NoParticle = 0, Proton = 1<<0, AntiProton = 1<<1, Helium = 1<<2, Electron = 1<<3, Positron = 1<<4,
    Muon = 1<<5, AntiMuon = 1<<6, PiPlus = 1<<7, PiMinus = 1<<8, Photon = 1<<9, Pi0 = 1<<10, Higgs = 1<<11};
  static const QString label(Particle);
  static Particle particle(const QString&);
  static QMap<Particle, QString>::ConstIterator particleBegin();
  static QMap<Particle, QString>::ConstIterator particleEnd();
  Q_DECLARE_FLAGS(Particles, Particle);
  static const QString label(Particles);
  static Particles particles(const QString&);

  enum Cut {RigidityCut, BetaCut, TimeOverThresholdCut, TrdSignalCut, CherenkovCut};
  static const QString label(Cut);
  static Cut cut(const QString&);
  static QMap<Cut, QString>::ConstIterator cutBegin();
  static QMap<Cut, QString>::ConstIterator cutEnd();

  enum Situation {NoSituation=-1, KirunaMuons, KirunaFlight, Testbeam11};
  static const QString label(Situation);
  static Situation situation(const QString&);
  static QMap<Situation, QString>::ConstIterator situationBegin();
  static QMap<Situation, QString>::ConstIterator situationEnd();

private:
  static const QMap<ChargeSign, QString> s_chargeSignMap;
  static const QMap<AnalysisTopic, QString> s_analysisTopicMap;
  static const QMap<TrackType, QString> s_trackTypeMap;
  static const QMap<Correction, QString> s_correctionMap;
  static const QMap<Particle, QString> s_particleMap;
  static const QMap<Cut, QString> s_cutMap;
  static const QMap<Situation, QString> s_situationMap;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Enums::AnalysisTopics);
Q_DECLARE_OPERATORS_FOR_FLAGS(Enums::ChargeSigns);
Q_DECLARE_OPERATORS_FOR_FLAGS(Enums::Corrections);
Q_DECLARE_OPERATORS_FOR_FLAGS(Enums::Particles);

#endif
