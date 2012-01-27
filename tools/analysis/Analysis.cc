#include "Analysis.hh"
#include "AnalysisPlot.hh"
#include "EventReader.hh"
#include "AnalysisProcessor.hh"

#include "Setup.hh"
#include "Layer.hh"
#include "Corrections.hh"
#include "ParticleFilter.hh"

#include "Track.hh"
#include "StraightLine.hh"
#include "BrokenLine.hh"
#include "CenteredBrokenLine.hh"
#include "CenteredBrokenLine2D.hh"

#include "TRDSpectrumPlotCollection.hh"
#include "TRDSpectrumCherenkovPlotCollection.hh"
#include "TRDSpectrumVsTimePlotCollection.hh"
#include "BendingPositionPlot.hh"
#include "BendingAnglePlot.hh"
#include "ResidualPlot.hh"
#include "GeometricOccupancyPlot.hh"
#include "BendingAnglePositionPlot.hh"
#include "Chi2Plot.hh"
#include "Chi2PerNdfPlot.hh"
#include "Chi2VsMomentumPlot.hh"
#include "AlbedosVsMomentumPlot.hh"
#include "TOFPositionCorrelationPlot.hh"
#include "MomentumSpectrumPlot.hh"
#include "SignalHeight2DPlot.hh"
#include "SignalHeight2DNormalizedPlot.hh"
#include "SignalHeightPlot.hh"
#include "ClusterLengthPlot.hh"
#include "ClusterShapePlot.hh"
#include "BetaPlot.hh"
#include "TOFTimeShiftPlotCollection.hh"
#include "BetaMomentumCorrelationPlot.hh"
#include "CutStatisticsPlot.hh"
#include "TrackerLayerStatisticsPlot.hh"
#include "TrackingEfficiencyVsMomentumPlot.hh"
#include "MultiLayerTrackingEfficiencyPlot.hh"
#include "SingleLayerTrackingEfficiencyPlot.hh"
#include "TRDClustersOnTrackPlot.hh"
#include "TRDDistanceWireToTrackPlot.hh"
#include "TRDDistanceInTube.hh"
#include "TRDTimeCorrectionPlot.hh"
#include "TRDEnergyDepositionOverMomentumPlot.hh"
#include "TRDSpectrumPlot.hh"
#include "TRDSpectrumVsTimePlot.hh"
#include "TRDSpectrumVsTemperaturePlot.hh"
#include "TRDSpectrumVsPressurePlot.hh"
#include "TRDOccupancyPlot.hh"
#include "TRDEfficiencyPlot.hh"
#include "TRDLikelihoodPlot.hh"
#include "TRDLikelihoodFunctionsPlot.hh"
#include "TotalEnergyDepositionPlot.hh"
#include "TotalEnergyDepositionTRDvsTrackerPlot.hh"
#include "TimeResolutionPlotCollection.hh"
#include "TOFTimeDifferencePlotCollection.hh"
#include "TotalSignalHeightPlot.hh"
#include "TOFEfficiencyPlot.hh"
#include "TOTPlot.hh"
#include "TOTLayerCollection.hh"
#include "TOTTemperatureCorrelationPlotCollection.hh"
#include "TOTTimeCorrelationPlotCollection.hh"
#include "TOTTemperatureCorrelationPlot.hh"
#include "TOTTimeCorrelationPlot.hh"
#include "TemperatureTimePlot.hh"
#include "PressureTimePlot.hh"
#include "SettingTimePlot.hh"
#include "ChannelTriggerProbabilityPlot.hh"
#include "TOFTimeShiftTriggerPlot.hh"
#include "TriggerRateTimePlot.hh"
#include "HeightTimePlot.hh"
#include "MCTotalEnergyDepositionTRDvsTrackerPlot.hh"
#include "MCTRDSpectrumPlot.hh"
#include "MCRigidityResolutionPlot.hh"
#include "MCTRDCalibrationPlot.hh"
#include "ResidualPlotMC.hh"
#include "TestbeamRigidityResolutionPlot.hh"
#include "PMTPlot.hh"
#include "PMTCorrelationPlot.hh"
#include "BeamProfilePlot.hh"
#include "ZSquareTRDPlot.hh"
#include "TOFBarShiftPlotCollection.hh"
#include "TimeReconstructionPlot.hh"
#include "EfficiencyCollection.hh"
#include "ZenithDistributionPlot.hh"
#include "ZenithAzimuthCorrelation.hh"
#include "AzimuthDistributionPlot.hh"
#include "AzimuthMigrationHistogram.hh"
#include "MeasurementTimePlot.hh"
#include "RigidityMigrationHistogram.hh"
#include "AzimuthPositionCorrelation.hh"
#include "AzimuthCutStatistics.hh"
#include "Helpers.hh"
#include "FluxCollection.hh"
#include "EventTimeDifferencePlot.hh"
#include "MeasurementTimeDistributionPlot.hh"
#include "TOFProbabilityDensityFunction.hh"
#include "LogLikelihoodPlot.hh"
#include "ReconstructionMethodCorrelationPlotCollection.hh"
#include "SignalHeightCorrelationPlotCollection.hh"
#include "SignalHeightPdfPlotCollection.hh"

#include <TPad.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TFile.h>

#include <QDebug>

Analysis::Analysis()
  : QObject()
  , m_analysisSetting()
  , m_reader(new EventReader(this))
  , m_processors()
  , m_plots()
{
  connect(m_reader, SIGNAL(started()), this, SIGNAL(started()));
  connect(m_reader, SIGNAL(numberOfEventsChanged(int)), this, SIGNAL(chainChanged(int)));
  connect(m_reader, SIGNAL(finished()), this, SLOT(finalize()));
}

Analysis::~Analysis()
{
  delete m_reader;
  qDeleteAll(m_processors);
  qDeleteAll(m_plots);
}

AnalysisSetting Analysis::analysisSetting()
{
  return m_analysisSetting;
}

void Analysis::clearFileList()
{
  m_reader->clearFileList();
}

void Analysis::addFileList(const QString& fileName)
{
  m_reader->addFileList(fileName);
}

void Analysis::start(const AnalysisSetting& analysisSetting)
{
  m_analysisSetting = analysisSetting;
  setupPlots();
  qDeleteAll(m_processors);
  m_processors.clear();
  for (int i = 0; i < m_analysisSetting.numberOfThreads(); ++i) {
    QVector<EventDestination*> destinations;
    foreach(AnalysisPlot* plot, m_plots)
      destinations.append(plot);
    AnalysisProcessor* processor = new AnalysisProcessor(destinations, m_analysisSetting);
    m_processors.append(processor);
  }
  if (m_analysisSetting.firstEvent() == 0 && m_analysisSetting.lastEvent() == 0) {
    m_reader->start(m_processors, 0, numberOfEvents() - 1);
  } else {
    m_reader->start(m_processors, m_analysisSetting.firstEvent(), m_analysisSetting.lastEvent());
  }
}

void Analysis::addPlot(AnalysisPlot* plot)
{
  m_plots.append(plot);
}

void Analysis::finalize()
{
  foreach(AnalysisPlot* plot, m_plots) {
    plot->finalize();
    plot->update(); // TODO: Remove and check all Plots which implemented finalize() in the following way: finalize() {update();}
  }
  emit finished();
}

void Analysis::stop()
{
  m_reader->stop();
}

void Analysis::clearPlots()
{
  qDeleteAll(m_plots);
  m_plots.clear();
}

unsigned int Analysis::numberOfPlots()
{
  return m_plots.size();
}

AnalysisPlot* Analysis::plot(unsigned int i)
{
  return m_plots[i];
}

QVector<AnalysisPlot*> Analysis::plots()
{
  return m_plots;
}

QVector<unsigned int> Analysis::plotIndices(Enums::AnalysisTopic topic)
{
  QVector<unsigned int> ret;
  for (int i = 0; i < m_plots.size(); ++i)
    if (m_plots[i]->topic() == topic)
      ret.append(i);
  return ret;
}

const QString& Analysis::plotTitle(unsigned int i)
{
  Q_ASSERT(i < numberOfPlots());
  return m_plots[i]->title();
}

Enums::AnalysisTopic Analysis::plotTopic(unsigned int i)
{
  Q_ASSERT(i < numberOfPlots());
  return m_plots[i]->topic();
}

double Analysis::progress() const
{
  return m_reader->progress();
}

double Analysis::buffer() const
{
  return m_reader->buffer();
}

void Analysis::save(const QString& fileName, TCanvas* canvas)
{
  canvas->cd();
  QString canvasName = canvas->GetName();
  TFile file(qPrintable(fileName), "RECREATE");
  for (unsigned int i = 0; i < numberOfPlots(); ++i) {
    AnalysisPlot* plot = m_plots.at(i);
    if (plot->isPlotCollection()) {
      static_cast<PlotCollection*>(plot)->saveForPostAnalysis(canvas);
    } else {
      m_plots[i]->draw(canvas);
      canvas->SetName(qPrintable(plotTitle(i) + " canvas"));
      canvas->Write();
    }
  }
  file.Close();
  canvas->SetName(qPrintable(canvasName));
}

void Analysis::processArguments(QStringList arguments)
{
  foreach(QString argument, arguments) {
    if (argument.endsWith(".conf", Qt::CaseInsensitive)) {
      m_analysisSetting.load(argument);
    } else if (argument.endsWith(".root", Qt::CaseInsensitive)) {
      m_reader->addRootFile(argument);
    } else if (argument.endsWith(".txt", Qt::CaseInsensitive)){
      m_reader->addFileList(argument);
    } else {
      qWarning() << argument << "has an unknown file ending.";
    }
  }
}

int Analysis::numberOfEvents() const
{
  return m_reader->numberOfEvents();
}

void Analysis::setupPlots()
{
  clearPlots();

  Setup* setup = Setup::instance();
  const ElementIterator& elementStartIt = setup->firstElement();
  const ElementIterator& elementEndIt = setup->lastElement();
  const LayerIterator& layerStartIt = setup->firstLayer();
  const LayerIterator& layerEndIt = setup->lastLayer();
  ElementIterator elementIt;
  LayerIterator layerIt;
  QDateTime first = m_reader->startTime();
  QDateTime last = m_reader->stopTime();

  if (m_analysisSetting.analysisTopics() & Enums::SignalHeightTracker) {
    addPlot(new SignalHeight2DPlot);
    addPlot(new SignalHeight2DNormalizedPlot);
    addPlot(new SignalHeightCorrelationPlotCollection(SignalHeightCorrelationPlot::Temperature));
    addPlot(new SignalHeightCorrelationPlotCollection(SignalHeightCorrelationPlot::Time, first, last));
    addPlot(new SignalHeightCorrelationPlotCollection(SignalHeightCorrelationPlot::Rigidity));
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::tracker)
        addPlot(new SignalHeightPlot(Enums::SignalHeightTracker, element->id()));
    }
  }

  if (m_analysisSetting.analysisTopics() & Enums::SignalHeightTRD) {
    addPlot(new ZSquareTRDPlot);
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::trd)
        addPlot(new SignalHeightPlot(Enums::SignalHeightTRD, element->id()));
    }

    addPlot(new TRDSpectrumPlot());
    addPlot(new TRDSpectrumPlotCollection);
    for (int i = 0; i < 8; ++i)
      addPlot(new TRDSpectrumPlot(i, TRDSpectrumPlot::layer));

    addPlot(new TRDSpectrumVsTimePlot(first,last));
    addPlot(new TRDSpectrumVsTimePlotCollection(first, last));

    addPlot(new TRDSpectrumVsTemperaturePlot());
    addPlot(new TRDSpectrumVsPressurePlot());
  }

  if (m_analysisSetting.analysisTopics() & Enums::ClusterShapeTracker) {
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::tracker) {
        addPlot(new ClusterLengthPlot(Enums::ClusterShapeTracker, element->id()));
        addPlot(new ClusterShapePlot(element->id()));
      }
    }
  }
  if (m_analysisSetting.analysisTopics() & Enums::ClusterShapeTRD) {
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::trd)
        addPlot(new ClusterLengthPlot(Enums::ClusterShapeTRD, element->id()));
    }
  }
  if (m_analysisSetting.analysisTopics() & Enums::TimeOverThreshold) {
    addPlot(new TOTPlot);
    addPlot(new TOTLayerCollection(TOTLayerCollection::Projection));
    addPlot(new TOTLayerCollection(TOTLayerCollection::TrdSignalCorrelation));
    addPlot(new TOTLayerCollection(TOTLayerCollection::TrackerSigalCorrelation));
    addPlot(new TOTLayerCollection(TOTLayerCollection::MomentumCorrelation));
    addPlot(new TOTLayerCollection(TOTLayerCollection::BetaCorrelation));
    addPlot(new TOTTemperatureCorrelationPlotCollection);
    addPlot(new TOTTimeCorrelationPlotCollection(first, last));
  }
  if (m_analysisSetting.analysisTopics() & Enums::Tracking) {
    addPlot(new BendingPositionPlot);
    addPlot(new BendingAnglePlot);
    for (double cut = .004; cut < .008; cut+=.001)
      addPlot(new BendingAnglePositionPlot(cut));
    for (unsigned short ndf = 6; ndf <= 16; ndf++)
      addPlot(new Chi2Plot(ndf));
    addPlot(new Chi2PerNdfPlot);
    addPlot(new Chi2VsMomentumPlot);
    addPlot(new ZenithAzimuthCorrelation());
    addPlot(new ZenithDistributionPlot());
    addPlot(new AzimuthDistributionPlot());
    addPlot(new AzimuthCutStatistics());
    addPlot(new AzimuthPositionCorrelation(AzimuthPositionCorrelation::X, Enums::Positive));
    addPlot(new AzimuthPositionCorrelation(AzimuthPositionCorrelation::X, Enums::Negative));
    addPlot(new AzimuthPositionCorrelation(AzimuthPositionCorrelation::X, Enums::Positive | Enums::Negative));
    addPlot(new AzimuthPositionCorrelation(AzimuthPositionCorrelation::Y, Enums::Positive));
    addPlot(new AzimuthPositionCorrelation(AzimuthPositionCorrelation::Y, Enums::Negative));
    addPlot(new AzimuthPositionCorrelation(AzimuthPositionCorrelation::Y, Enums::Positive | Enums::Negative));
  }
  if (m_analysisSetting.analysisTopics() & Enums::Occupancy) {
    for (layerIt = layerStartIt; layerIt != layerEndIt; ++layerIt) {
      Layer* layer = *layerIt;
      addPlot(new GeometricOccupancyPlot(layer->z()));
    }
  }
  if (m_analysisSetting.analysisTopics() & Enums::ResidualsTracker) {
    for (layerIt = layerStartIt; layerIt != layerEndIt; ++layerIt) {
      Layer* layer = *layerIt;
      if (-240 < layer->z() && layer->z() < 240)
        addPlot(new ResidualPlot(Enums::ResidualsTracker, layer));
    }
  }
  if (m_analysisSetting.analysisTopics() & Enums::ResidualsTRD) {
    for (layerIt = layerStartIt; layerIt != layerEndIt; ++layerIt) {
      Layer* layer = *layerIt;
      if (-520 < layer->z() && layer->z() < -240)
        addPlot(new ResidualPlot(Enums::ResidualsTRD, layer));
    }
  }
  if (m_analysisSetting.analysisTopics() & Enums::MomentumReconstruction) {
    addPlot(new BetaMomentumCorrelationPlot());
    addPlot(new MomentumSpectrumPlot(Enums::Positive));
    addPlot(new MomentumSpectrumPlot(Enums::Negative));
    addPlot(new MomentumSpectrumPlot(Enums::Positive | Enums::Negative));
    addPlot(new MomentumSpectrumPlot(Enums::Positive | Enums::Negative, true));
    addPlot(new AlbedosVsMomentumPlot());
    addPlot(new MeasurementTimePlot(first, last));
    addPlot(new FluxCollection(m_analysisSetting.numberOfThreads()));
    addPlot(new ReconstructionMethodCorrelationPlotCollection(m_analysisSetting.particles()));
  }
  if (m_analysisSetting.analysisTopics() & Enums::LikelihoodTopic) {
    Enums::ParticleIterator end = Enums::particleEnd();
    for (Enums::ParticleIterator it = Enums::particleBegin(); it != end; ++it)
      if (it.key() != Enums::NoParticle && (it.key() & m_analysisSetting.particles()))
        addPlot(new LogLikelihoodPlot(it.key(), m_analysisSetting.particles() & ~it.key()));
    addPlot(new SignalHeightPdfPlotCollection(Hit::tof, m_analysisSetting.particles()));
    addPlot(new SignalHeightPdfPlotCollection(Hit::trd, m_analysisSetting.particles()));
    addPlot(new SignalHeightPdfPlotCollection(Hit::tracker, m_analysisSetting.particles()));
  }
  if (m_analysisSetting.analysisTopics() & Enums::EfficiencyTOF) {
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::tof) {
        for (int ch = 0; ch < 4; ++ch) {
          addPlot(new TOFEfficiencyPlot(element->id() | ch));
        }
      }
    }
  }
  if (m_analysisSetting.analysisTopics() & Enums::ResolutionTOF) {
    addPlot(new TimeResolutionPlotCollection);
  }
  if (m_analysisSetting.analysisTopics() & Enums::CalibrationTOF) {
    addPlot(new ChannelTriggerProbabilityPlot);
    addPlot(new TOFTimeShiftTriggerPlot);
    addPlot(new TOFTimeShiftPlotCollection);
    addPlot(new TOFTimeDifferencePlotCollection);
    addPlot(new TOFBarShiftPlotCollection);
  }
  if (m_analysisSetting.analysisTopics() & Enums::MiscellaneousTracker) {
    addPlot(new TotalSignalHeightPlot);
    addPlot(new CutStatisticsPlot);
    addPlot(new TrackerLayerStatisticsPlot);
    addPlot(new TrackingEfficiencyVsMomentumPlot(Enums::Positive));
    addPlot(new TrackingEfficiencyVsMomentumPlot(Enums::Negative));
    addPlot(new TrackingEfficiencyVsMomentumPlot(Enums::Positive | Enums::Negative));
    addPlot(new SingleLayerTrackingEfficiencyPlot(Enums::Positive));
    addPlot(new SingleLayerTrackingEfficiencyPlot(Enums::Negative));
    addPlot(new SingleLayerTrackingEfficiencyPlot(Enums::Positive | Enums::Negative));
    addPlot(new MultiLayerTrackingEfficiencyPlot(MultiLayerTrackingEfficiencyPlot::Positive));
    addPlot(new MultiLayerTrackingEfficiencyPlot(MultiLayerTrackingEfficiencyPlot::Negative));
    addPlot(new MultiLayerTrackingEfficiencyPlot(MultiLayerTrackingEfficiencyPlot::All));
    addPlot(new EfficiencyCollection());
  }
  if (m_analysisSetting.analysisTopics() & Enums::MiscellaneousTRD) {
    addPlot(new TRDClustersOnTrackPlot(Enums::MiscellaneousTRD));
    addPlot(new TRDDistanceWireToTrackPlot(Enums::MiscellaneousTRD));
    addPlot(new TRDDistanceInTube(Enums::MiscellaneousTRD));
    addPlot(new TotalEnergyDepositionPlot());
    addPlot(new TRDEnergyDepositionOverMomentumPlot(Enums::MiscellaneousTRD));
    addPlot(new TotalEnergyDepositionTRDvsTrackerPlot());
    addPlot(new TRDEfficiencyPlot());
    addPlot(new TRDOccupancyPlot(TRDOccupancyPlot::numberOfHits));
    addPlot(new TRDOccupancyPlot(TRDOccupancyPlot::numberOfHits, true));
    addPlot(new TRDOccupancyPlot(TRDOccupancyPlot::sumOfSignalHeights, true));
    addPlot(new TRDOccupancyPlot(TRDOccupancyPlot::sumOfSignalHeightsNormalizedToHits, true));
    addPlot(new TRDTimeCorrectionPlot(first, last));
    addPlot(new TRDLikelihoodFunctionsPlot());
  }
  if (m_analysisSetting.analysisTopics() & Enums::MiscellaneousTOF) {
    addPlot(new BetaPlot);
    addPlot(new TimeReconstructionPlot(TimeReconstructionPlot::Mean));
    addPlot(new TimeReconstructionPlot(TimeReconstructionPlot::Median));
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::tof)
        addPlot(new TOFPositionCorrelationPlot(element->id()));
    }
    addPlot(new EventTimeDifferencePlot(m_analysisSetting.numberOfThreads()));
    addPlot(new MeasurementTimeDistributionPlot(m_analysisSetting.numberOfThreads()));
  }
  if (m_analysisSetting.analysisTopics() & Enums::SlowControl) {
    QVector<SensorTypes::Type> temperatureSensors = QVector<SensorTypes::Type>::fromStdVector(SensorTypes::temperatureSensors());
    foreach(SensorTypes::Type sensor, temperatureSensors)
      addPlot(new TemperatureTimePlot(sensor, first, last));
    addPlot(new PressureTimePlot(SensorTypes::TRD_PRESSURE, first, last));
    addPlot(new PressureTimePlot(SensorTypes::TRD_PRESSURE_SMOOTHED, first, last));
    addPlot(new TriggerRateTimePlot(first, last));
    addPlot(new HeightTimePlot(first, last));
  }
  if (m_analysisSetting.analysisTopics() & Enums::MonteCarlo) {
    addPlot(new MCTotalEnergyDepositionTRDvsTrackerPlot());
  }
  if (m_analysisSetting.analysisTopics() & Enums::MonteCarloTRD) {
    addPlot(new MCTRDSpectrumPlot());
    for (layerIt = layerStartIt; layerIt != layerEndIt; ++layerIt) {
      Layer* layer = *layerIt;
      if (-520 < layer->z() && layer->z() < -240)
        addPlot(new ResidualPlotMC(Enums::MonteCarloTRD, layer));
    }
    addPlot(new MCTRDCalibrationPlot());
    addPlot(new TRDLikelihoodPlot(Enums::MonteCarloTRD));
  }
  if (m_analysisSetting.analysisTopics() & Enums::MonteCarloTracker) {
    addPlot(new MCRigidityResolutionPlot(Enums::Positron));
    addPlot(new MCRigidityResolutionPlot(Enums::Electron));
    addPlot(new MCRigidityResolutionPlot(Enums::Proton));
    addPlot(new MCRigidityResolutionPlot(Enums::PiMinus));
    addPlot(new MCRigidityResolutionPlot(Enums::PiPlus));
    addPlot(new MCRigidityResolutionPlot(Enums::Helium));
    for (layerIt = layerStartIt; layerIt != layerEndIt; ++layerIt) {
      Layer* layer = *layerIt;
      if (-240 < layer->z() && layer->z() < 240)
        addPlot(new ResidualPlotMC(Enums::MonteCarloTracker, layer));
    }
    addPlot(new AzimuthMigrationHistogram());
    addPlot(new RigidityMigrationHistogram());
  }
  if (m_analysisSetting.analysisTopics() & Enums::Testbeam) {
    addPlot(new TOFProbabilityDensityFunction);
    addPlot(new SettingTimePlot(SettingTimePlot::MagnetInstalled, first, last));
    addPlot(new SettingTimePlot(SettingTimePlot::Momentum, first, last));
    addPlot(new SettingTimePlot(SettingTimePlot::Polarity, first, last));
    addPlot(new SettingTimePlot(SettingTimePlot::AbsMomentum, first, last));
    addPlot(new TRDSpectrumCherenkovPlotCollection());
    QVector<SensorTypes::Type> beamSensors = QVector<SensorTypes::Type>::fromStdVector(SensorTypes::beamSensors());
    foreach(SensorTypes::Type sensor, beamSensors)
      addPlot(new PMTPlot(sensor));
    addPlot(new PMTCorrelationPlot);
    addPlot(new TRDLikelihoodPlot(Enums::Testbeam));
    addPlot(new BeamProfilePlot(BeamProfilePlot::Horizontal));
    addPlot(new BeamProfilePlot(BeamProfilePlot::Vertical));
    addPlot(new TestbeamRigidityResolutionPlot(Enums::Positron));
    addPlot(new TestbeamRigidityResolutionPlot(Enums::Electron));
    addPlot(new TestbeamRigidityResolutionPlot(Enums::Proton));
    addPlot(new TestbeamRigidityResolutionPlot(Enums::PiMinus));
    addPlot(new TestbeamRigidityResolutionPlot(Enums::PiPlus));
  }
}
