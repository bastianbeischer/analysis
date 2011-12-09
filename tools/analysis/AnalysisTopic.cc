#include "AnalysisTopic.hh"

const QVector<Enums::AnalysisTopic> AnalysisTopic::s_topics = QVector<Enums::AnalysisTopic>()
  << Enums::SignalHeightTracker << Enums::SignalHeightTRD << Enums::ClusterShapeTracker
  << Enums::ClusterShapeTRD << Enums::TimeOverThreshold << Enums::Tracking
  << Enums::Occupancy << Enums::ResidualsTracker << Enums::ResidualsTRD
  << Enums::MomentumReconstruction << Enums::EfficiencyTOF << Enums::ResolutionTOF
  << Enums::CalibrationTOF << Enums::MiscellaneousTracker << Enums::MiscellaneousTRD
  << Enums::MiscellaneousTOF << Enums::SlowControl << Enums::MonteCarloTracker
  << Enums::MonteCarloTRD << Enums::MonteCarloTOF << Enums::MonteCarlo
  << Enums::Testbeam << Enums::LikelihoodTopic;

const QVector<Enums::AnalysisTopic> AnalysisTopic::s_trackerTopics = QVector<Enums::AnalysisTopic>()
  << Enums::SignalHeightTracker << Enums::ClusterShapeTracker << Enums::Tracking
  << Enums::ResidualsTracker << Enums::MomentumReconstruction << Enums::MonteCarloTracker
  << Enums::MiscellaneousTracker;

const QVector<Enums::AnalysisTopic> AnalysisTopic::s_trdTopics = QVector<Enums::AnalysisTopic>()
  << Enums::SignalHeightTRD << Enums::ClusterShapeTRD << Enums::ResidualsTRD
  << Enums::MonteCarloTRD << Enums::MiscellaneousTRD;

const QVector<Enums::AnalysisTopic> AnalysisTopic::s_tofTopics = QVector<Enums::AnalysisTopic>()
  << Enums::TimeOverThreshold << Enums::MomentumReconstruction << Enums::EfficiencyTOF
  << Enums::ResolutionTOF << Enums::CalibrationTOF << Enums::MonteCarloTOF
  << Enums::MiscellaneousTOF;

const QVector<Enums::AnalysisTopic> AnalysisTopic::s_otherTopics = QVector<Enums::AnalysisTopic>()
  << Enums::MomentumReconstruction << Enums::LikelihoodTopic << Enums::Occupancy << Enums::MonteCarlo
  << Enums::SlowControl << Enums::Testbeam;

const QVector<Enums::AnalysisTopic>& AnalysisTopic::topics()
{
  return s_topics;
}

const QVector<Enums::AnalysisTopic>& AnalysisTopic::trackerTopics()
{
  return s_trackerTopics;
}

const QVector<Enums::AnalysisTopic>& AnalysisTopic::trdTopics()
{
  return s_trdTopics;
}

const QVector<Enums::AnalysisTopic>& AnalysisTopic::tofTopics()
{
  return s_tofTopics;
}

const QVector<Enums::AnalysisTopic>& AnalysisTopic::otherTopics()
{
  return s_otherTopics;
}
