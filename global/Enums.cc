#include "Enums.hh"

template <class Key, class T> class InitializableMap : public QMap<Key, T> {
public:
  inline InitializableMap<Key, T>& operator<< (const QPair<Key, T>& pair) {
    insert(pair.first, pair.second);
    return *this;
  }
};

namespace Enums {

  const QMap<ChargeSign, QString> s_chargeSignMap = InitializableMap<ChargeSign, QString>()
    << QPair<ChargeSign, QString>(Positive, "positive")
    << QPair<ChargeSign, QString>(Negative, "negative");
  const QString label(ChargeSign key) {return s_chargeSignMap.value(key);}
  ChargeSign chargeSign(const QString& value) {return s_chargeSignMap.key(value);}

  const QMap<AnalysisTopic, QString> s_analysisTopicMap = InitializableMap<AnalysisTopic, QString>()
    << QPair<AnalysisTopic, QString>(SignalHeightTracker, "signal height tracker")
    << QPair<AnalysisTopic, QString>(SignalHeightTRD, "signal height TRD")
    << QPair<AnalysisTopic, QString>(ClusterShapeTracker, "cluster shape tracker")
    << QPair<AnalysisTopic, QString>(ClusterShapeTRD, "cluster shape TRD")
    << QPair<AnalysisTopic, QString>(TimeOverThreshold, "time over\nthreshold")
    << QPair<AnalysisTopic, QString>(Tracking, "tracking")
    << QPair<AnalysisTopic, QString>(Occupancy, "occupancy")
    << QPair<AnalysisTopic, QString>(ResidualsTracker, "residuals tracker")
    << QPair<AnalysisTopic, QString>(ResidualsTRD, "residuals TRD")
    << QPair<AnalysisTopic, QString>(MomentumReconstruction, "momentum\nreconstruction")
    << QPair<AnalysisTopic, QString>(EfficiencyTOF, "efficiency TOF")
    << QPair<AnalysisTopic, QString>(ResolutionTOF, "resolution TOF")
    << QPair<AnalysisTopic, QString>(CalibrationTOF, "calibration TOF")
    << QPair<AnalysisTopic, QString>(MiscellaneousTracker, "miscellaneous tracker")
    << QPair<AnalysisTopic, QString>(MiscellaneousTRD, "miscellaneous TRD")
    << QPair<AnalysisTopic, QString>(MiscellaneousTOF, "miscellaneous TOF")
    << QPair<AnalysisTopic, QString>(SlowControl, "slow control")
    << QPair<AnalysisTopic, QString>(MonteCarloTracker, "monte carlo tracker")
    << QPair<AnalysisTopic, QString>(MonteCarloTRD, "monte carlo TRD")
    << QPair<AnalysisTopic, QString>(MonteCarloTOF, "monte carlo TOF")
    << QPair<AnalysisTopic, QString>(MonteCarlo, "monte carlo")
    << QPair<AnalysisTopic, QString>(Testbeam, "testbeam");
  const QString label(AnalysisTopic key) {return s_analysisTopicMap.value(key);}
  AnalysisTopic analysisTopic(const QString& value) {return s_analysisTopicMap.key(value);}

}
