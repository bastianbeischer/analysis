#include "Enums.hh"

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
  << QPair<Enums::AnalysisTopic, QString>(Enums::TimeOverThreshold, "time over\nthreshold")
  << QPair<Enums::AnalysisTopic, QString>(Enums::Tracking, "tracking")
  << QPair<Enums::AnalysisTopic, QString>(Enums::Occupancy, "occupancy")
  << QPair<Enums::AnalysisTopic, QString>(Enums::ResidualsTracker, "residuals tracker")
  << QPair<Enums::AnalysisTopic, QString>(Enums::ResidualsTRD, "residuals TRD")
  << QPair<Enums::AnalysisTopic, QString>(Enums::MomentumReconstruction, "momentum\nreconstruction")
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
  << QPair<Enums::TrackType, QString>(Enums::None, "none")
  << QPair<Enums::TrackType, QString>(Enums::StraightLine, "straight line")
  << QPair<Enums::TrackType, QString>(Enums::BrokenLine, "broken line")
  << QPair<Enums::TrackType, QString>(Enums::CenteredBrokenLine, "centered broken line")
  << QPair<Enums::TrackType, QString>(Enums::CenteredBrokenLine2D, "centered broken line 2D");

// ChargeSign
const QString Enums::label(Enums::ChargeSign key) {return s_chargeSignMap.value(key);}
Enums::ChargeSign Enums::chargeSign(const QString& value) {return s_chargeSignMap.key(value);}

// AnalysisTopic
const QString Enums::label(Enums::AnalysisTopic key) {return s_analysisTopicMap.value(key);}
Enums::AnalysisTopic Enums::analysisTopic(const QString& value) {return s_analysisTopicMap.key(value);}
QMap<Enums::AnalysisTopic, QString>::ConstIterator Enums::analysisTopicBegin() {return s_analysisTopicMap.constBegin();}
QMap<Enums::AnalysisTopic, QString>::ConstIterator Enums::analysisTopicEnd() {return s_analysisTopicMap.constEnd();}

// TrackType
const QString Enums::label(Enums::TrackType key) {return s_trackTypeMap.value(key);}
Enums::TrackType Enums::trackType(const QString& value) {return s_trackTypeMap.key(value);}
QMap<Enums::TrackType, QString>::ConstIterator Enums::trackTypeBegin() {return s_trackTypeMap.constBegin();}
QMap<Enums::TrackType, QString>::ConstIterator Enums::trackTypeEnd() {return s_trackTypeMap.constEnd();}

