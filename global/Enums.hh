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

  enum AnalysisTopic {SignalHeightTracker, SignalHeightTRD, ClusterShapeTracker, ClusterShapeTRD,
    TimeOverThreshold, Tracking, Occupancy, ResidualsTracker, ResidualsTRD, MomentumReconstruction,
    EfficiencyTOF, ResolutionTOF, CalibrationTOF, MiscellaneousTracker, MiscellaneousTRD, MiscellaneousTOF,
    SlowControl, MonteCarloTracker, MonteCarloTRD, MonteCarloTOF, MonteCarlo, Testbeam};
  static const QString label(AnalysisTopic);
  static AnalysisTopic analysisTopic(const QString&);
  static QMap<AnalysisTopic, QString>::ConstIterator analysisTopicBegin();
  static QMap<AnalysisTopic, QString>::ConstIterator analysisTopicEnd();

  enum TrackType {NoTrack, StraightLine, BrokenLine, CenteredBrokenLine, CenteredBrokenLine2D};
  static const QString label(TrackType);
  static TrackType trackType(const QString&);
  static QMap<TrackType, QString>::ConstIterator trackTypeBegin();
  static QMap<TrackType, QString>::ConstIterator trackTypeEnd();

  enum Correction {NoCorrection = 0x0, Alignment = 1<<0, TimeShifts = 1<<1, TrdMopv = 1<<2, TrdTime = 1<<3, TrdPressure = 1<<4,
    TrdTemperature = 1<<5, TofTimeOverThreshold = 1<<6, MultipleScattering = 1<<7, PhotonTravelTime = 1<<8};
  static const QString label(Correction);
  static Correction correction(const QString&);
  static QMap<Correction, QString>::ConstIterator correctionBegin();
  static QMap<Correction, QString>::ConstIterator correctionEnd();
  Q_DECLARE_FLAGS(Corrections, Correction);
  static const QString label(Corrections);
  static Corrections corrections(const QString&);

private:
  static const QMap<ChargeSign, QString> s_chargeSignMap;
  static const QMap<AnalysisTopic, QString> s_analysisTopicMap;
  static const QMap<TrackType, QString> s_trackTypeMap;
  static const QMap<Correction, QString> s_correctionMap;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Enums::ChargeSigns);

#endif
