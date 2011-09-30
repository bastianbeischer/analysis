#include "AnalysisTopic.hh"

const AnalysisTopic AnalysisTopic::SignalHeightTracker(AnalysisTopic::EnumSignalHeightTracker);
const AnalysisTopic AnalysisTopic::SignalHeightTRD(AnalysisTopic::EnumSignalHeightTRD);
const AnalysisTopic AnalysisTopic::ClusterShapeTracker(AnalysisTopic::EnumClusterShapeTracker);
const AnalysisTopic AnalysisTopic::ClusterShapeTRD(AnalysisTopic::EnumClusterShapeTRD);
const AnalysisTopic AnalysisTopic::TimeOverThreshold(AnalysisTopic::EnumTimeOverThreshold);
const AnalysisTopic AnalysisTopic::Tracking(AnalysisTopic::EnumTracking);
const AnalysisTopic AnalysisTopic::Occupancy(AnalysisTopic::EnumOccupancy);
const AnalysisTopic AnalysisTopic::ResidualsTracker(AnalysisTopic::EnumResidualsTracker);
const AnalysisTopic AnalysisTopic::ResidualsTRD(AnalysisTopic::EnumResidualsTRD);
const AnalysisTopic AnalysisTopic::MomentumReconstruction(AnalysisTopic::EnumMomentumReconstruction);
const AnalysisTopic AnalysisTopic::EfficiencyTOF(AnalysisTopic::EnumEfficiencyTOF);
const AnalysisTopic AnalysisTopic::ResolutionTOF(AnalysisTopic::EnumResolutionTOF);
const AnalysisTopic AnalysisTopic::CalibrationTOF(AnalysisTopic::EnumCalibrationTOF);
const AnalysisTopic AnalysisTopic::MiscellaneousTracker(AnalysisTopic::EnumMiscellaneousTracker);
const AnalysisTopic AnalysisTopic::MiscellaneousTRD(AnalysisTopic::EnumMiscellaneousTRD);
const AnalysisTopic AnalysisTopic::MiscellaneousTOF(AnalysisTopic::EnumMiscellaneousTOF);
const AnalysisTopic AnalysisTopic::SlowControl(AnalysisTopic::EnumSlowControl);
const AnalysisTopic AnalysisTopic::MonteCarloTracker(AnalysisTopic::EnumMonteCarloTracker);
const AnalysisTopic AnalysisTopic::MonteCarloTRD(AnalysisTopic::EnumMonteCarloTRD);
const AnalysisTopic AnalysisTopic::MonteCarloTOF(AnalysisTopic::EnumMonteCarloTOF);
const AnalysisTopic AnalysisTopic::MonteCarlo(AnalysisTopic::EnumMonteCarlo);
const AnalysisTopic AnalysisTopic::Testbeam(AnalysisTopic::EnumTestbeam);

const QVector<AnalysisTopic> AnalysisTopic::s_topics = QVector<AnalysisTopic>()
  << AnalysisTopic::SignalHeightTracker << AnalysisTopic::SignalHeightTRD << AnalysisTopic::ClusterShapeTracker
  << AnalysisTopic::ClusterShapeTRD << AnalysisTopic::TimeOverThreshold << AnalysisTopic::Tracking
  << AnalysisTopic::Occupancy << AnalysisTopic::ResidualsTracker << AnalysisTopic::ResidualsTRD
  << AnalysisTopic::MomentumReconstruction << AnalysisTopic::EfficiencyTOF << AnalysisTopic::ResolutionTOF
  << AnalysisTopic::CalibrationTOF << AnalysisTopic::MiscellaneousTracker << AnalysisTopic::MiscellaneousTRD
  << AnalysisTopic::MiscellaneousTOF << AnalysisTopic::SlowControl << AnalysisTopic::MonteCarloTracker
  << AnalysisTopic::MonteCarloTRD << AnalysisTopic::MonteCarloTOF << AnalysisTopic::MonteCarlo
  << AnalysisTopic::Testbeam;
  
AnalysisTopic::AnalysisTopic()
  : m_topic(EnumUndefined)
{
}

AnalysisTopic::AnalysisTopic(Topic topic)
  : m_topic(topic)
{
}

const char* AnalysisTopic::label() const
{
  switch (m_topic) {
    case EnumUndefined: return 0;
    case EnumSignalHeightTracker: return "SignalHeightTracker";
    case EnumSignalHeightTRD: return "SignalHeightTRD";
    case EnumClusterShapeTracker: return "ClusterShapeTracker";
    case EnumClusterShapeTRD: return "ClusterShapeTRD";
    case EnumTimeOverThreshold: return "TimeOverThreshold";
    case EnumTracking: return "Tracking";
    case EnumOccupancy: return "Occupancy";
    case EnumResidualsTracker: return "ResidualsTracker";
    case EnumResidualsTRD: return "ResidualsTRD";
    case EnumMomentumReconstruction: return "MomentumReconstruction";
    case EnumEfficiencyTOF: return "EfficiencyTOF";
    case EnumResolutionTOF: return "ResolutionTOF";
    case EnumCalibrationTOF: return "CalibrationTOF";
    case EnumMiscellaneousTracker: return "MiscellaneousTracker";
    case EnumMiscellaneousTRD: return "MiscellaneousTRD";
    case EnumMiscellaneousTOF: return "MiscellaneousTOF";
    case EnumSlowControl: return "SlowControl";
    case EnumMonteCarloTracker: return "MonteCarloTracker";
    case EnumMonteCarloTRD: return "MonteCarloTRD";
    case EnumMonteCarloTOF: return "MonteCarloTOF";
    case EnumMonteCarlo: return "MonteCarlo";
    case EnumTestbeam: return "Testbeam";
  }
  qFatal("Every AnalysisTopic should have a label.");
  return 0;
}

const QVector<AnalysisTopic>& AnalysisTopic::topics()
{
  return s_topics;
}
