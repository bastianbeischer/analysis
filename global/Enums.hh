#ifndef Enums_hh
#define Enums_hh

#include <QMap>
#include <QPair>

class Enums {
public:
  enum ChargeSign {Positive = 1<<0, Negative = 1<<1};
  static const QString label(ChargeSign);
  static ChargeSign chargeSign(const QString&);
  Q_DECLARE_FLAGS(ChargeSigns, ChargeSign);

  enum AnalysisTopic {SignalHeightTracker, SignalHeightTRD, ClusterShapeTracker, ClusterShapeTRD,
    TimeOverThreshold, Tracking, Occupancy, ResidualsTracker, ResidualsTRD, MomentumReconstruction,
    EfficiencyTOF, ResolutionTOF, CalibrationTOF, MiscellaneousTracker, MiscellaneousTRD, MiscellaneousTOF,
    SlowControl, MonteCarloTracker, MonteCarloTRD, MonteCarloTOF, MonteCarlo, Testbeam};
  static const QString label(AnalysisTopic);
  static AnalysisTopic analysisTopic(const QString&);
  static QMap<AnalysisTopic, QString>::ConstIterator analysisTopicBegin();
  static QMap<AnalysisTopic, QString>::ConstIterator analysisTopicEnd();

  enum TrackType {None, StraightLine, BrokenLine, CenteredBrokenLine, CenteredBrokenLine2D};
  static const QString label(TrackType);
  static TrackType trackType(const QString&);
  static QMap<TrackType, QString>::ConstIterator trackTypeBegin();
  static QMap<TrackType, QString>::ConstIterator trackTypeEnd();
private:
  static const QMap<ChargeSign, QString> s_chargeSignMap;
  static const QMap<AnalysisTopic, QString> s_analysisTopicMap;
  static const QMap<TrackType, QString> s_trackTypeMap;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Enums::ChargeSigns);

#endif
