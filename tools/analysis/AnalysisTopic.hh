#ifndef AnalysisTopic_h
#define AnalysisTopic_h

#include <QVector>

class AnalysisTopic {
public:
  static const AnalysisTopic SignalHeightTracker, SignalHeightTRD, ClusterShapeTracker, ClusterShapeTRD,
    TimeOverThreshold, Tracking, Occupancy, ResidualsTracker, ResidualsTRD, MomentumReconstruction,
    EfficiencyTOF, ResolutionTOF, CalibrationTOF, MiscellaneousTracker, MiscellaneousTRD, MiscellaneousTOF,
    SlowControl, MonteCarloTracker, MonteCarloTRD, MonteCarloTOF, MonteCarlo, Testbeam;
  static const QVector<AnalysisTopic>& topics();

  AnalysisTopic();
  const char* label() const;
  friend bool operator==(const AnalysisTopic& t1, const AnalysisTopic& t2) {return t1.m_topic == t2.m_topic;}
  friend bool operator!=(const AnalysisTopic& t1, const AnalysisTopic& t2) {return !(t1 == t2);}
private:
  enum Topic {EnumUndefined, EnumSignalHeightTracker, EnumSignalHeightTRD, EnumClusterShapeTracker, EnumClusterShapeTRD,
    EnumTimeOverThreshold, EnumTracking, EnumOccupancy, EnumResidualsTracker, EnumResidualsTRD,
    EnumMomentumReconstruction, EnumEfficiencyTOF, EnumResolutionTOF, EnumCalibrationTOF, EnumMiscellaneousTracker,
    EnumMiscellaneousTRD, EnumMiscellaneousTOF, EnumSlowControl, EnumMonteCarloTracker, EnumMonteCarloTRD,
    EnumMonteCarloTOF, EnumMonteCarlo, EnumTestbeam};
  AnalysisTopic(Topic);
  static const QVector<AnalysisTopic> s_topics;
  
  Topic m_topic;
};

#endif
