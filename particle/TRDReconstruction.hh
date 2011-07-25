#ifndef TRDRECONSTRUCTION_HH
#define TRDRECONSTRUCTION_HH

class Hit;
class Cluster;
class SimpleEvent;
class Track;
class QVector2D;
class Particle;

#include <QVector>
#include <QMap>
#include <QMultiMap>
#include <QString>

class TRDReconstruction
{
public:
  enum Flag {None=0x0, GoodTRDEvent=0x1<<0};
  Q_DECLARE_FLAGS(Flags, Flag);

  TRDReconstruction();

public:
  static double distanceOnTrackThroughTRDTube(const Hit* hit, const Track* track);
  static double distanceTrackToWire(const Hit* hit, const Track* track);
  static double distanceTrackToWire(const QVector2D& trdChanPos2D, const Track* track);

  static int TRDLayerNo(const Hit* hit);
  static int TRDLayerNo(const unsigned int detID);

  static double spectrumUpperLimit() {return TRDReconstruction::s_calculateLengthInTube ? 15 : 50 ;}
  static QString xAxisTitle() {return TRDReconstruction::s_calculateLengthInTube ? "energy deposition length in tube / (keV/mm)" : "energy deposition / keV" ;}

public:
  void reset();
  void reconstructTRD(SimpleEvent* event, Track* globalTrack);

  const Flags& flags() const {return m_flags;}

  const QList<const Hit*>& allHits() const {return m_allHits;}
  const QList<const Cluster*>& allClusters() const {return m_allClusters;}
  const QList<const Hit*>& allHitsOnTrack() const {return m_allHitsOnTrack;}
  const QList<const Cluster*>& allClustersOnTrack() const {return m_allClustersOnTrack;}
  const QList<const Hit*>& allHitsOnTrackAndPierced() const {return m_allHitsOnTrackAndPierced;}
  const QList<const Cluster*>& allClustersOnTrackAndPierced() const {return m_allClustersOnTrackAndPierced;}
  const QList<const Hit*> allHitsForLayer(int layer) const {return m_layerAllHits.values(layer);}
  const QList<const Cluster*> allClustersForLayer(int layer) const {return m_layerAllClusters.values(layer);}
  const QList<const Hit*> allHitsOnTrackForLayer(int layer) const {return m_layerAllHitsOnTrack.values(layer);}
  const QList<const Cluster*> allClustersOnTrackForLayer(int layer) const {return m_layerAllClustersOnTrack.values(layer);}
  const QList<const Hit*> allHitsOnTrackAndPiercedForLayer(int layer) const {return m_layerAllHitsOnTrackAndPierced.values(layer);}
  const QList<const Cluster*> allClustersOnTrackAndPiercedForLayer(int layer) const {return m_layerAllClustersOnTrackAndPierced.values(layer);}

  const QVector<double>& energyDepositionForLayers() const {return m_layerEnergyDeposition;}
  const QVector<double>& energyDepositionOnTrackForLayers() const {return m_layerEnergyDepositionOnTrack;}
  const QVector<double>& energyDepositionOnTrackAndPiercedForLayers() const {return m_layerEnergyDepositionOnTrackAndPierced;}
  const QVector<double>& lengthThroughTubeForLayers() const {return m_layerLengthThroughTube;}
  const QVector<double>& energyDepositionOnTrackPerLengthForLayers() const {return m_layerEnergyDepositionOnTrackPerLength;}
  const QVector<double>& energyDepositionOnTrackPerMinLengthForLayer() const {return m_layerEnergyDepositionOnTrackPerMinLength;}

  double energyDepositionForLayer(int layer) const {return m_layerEnergyDeposition.at(layer);}
  double energyDepositionOnTrackForLayer(int layer) const {return m_layerEnergyDepositionOnTrack.at(layer);}
  double energyDepositionOnTrackAndPiercedForLayer(int layer) const {return m_layerEnergyDepositionOnTrackAndPierced.at(layer);}
  double lengthThroughTubeForLayer(int layer) const {return m_layerLengthThroughTube.at(layer);}
  double energyDepositionOnTrackPerLengthForLayer(int layer) const {return m_layerEnergyDepositionOnTrackPerLength.at(layer);}
  double energyDepositionOnTrackPerMinLengthForLayer(int layer) const {return m_layerEnergyDepositionOnTrackPerMinLength.at(layer);}

  double energyDepositionForChannel(unsigned short channelID) const {return m_channelEnergyDeposition.value(channelID);}
  double energyDepositionOnTrackForChannel(unsigned short channelID) const {return m_channelEnergyDepositionOnTrack.value(channelID);}
  double energyDepositionOnTrackPerLengthForChannel(unsigned short channelID) const {return m_channelEnergyDepositionOnTrackPerLength.value(channelID);}
  double energyDepositionOnTrackPerMinLengthForChannel(unsigned short channelID) const {return m_channelEnergyDepositionOnTrackPerMinLength.value(channelID);}
  double energyDepositionForModule(unsigned short channelID) const {return m_moduleEnergyDeposition.value(channelID);}
  double energyDepositionOnTrackForModule(unsigned short channelID) const {return m_moduleEnergyDepositionOnTrack.value(channelID);}
  double energyDepositionOnTrackPerLengthForModule(unsigned short channelID) const {return m_moduleEnergyDepositionOnTrackPerLength.value(channelID);}
  double energyDepositionOnTrackPerMinLengthForModule(unsigned short channelID) const {return m_moduleEnergyDepositionOnTrackPerMinLength.value(channelID);}

  int noOfHits() const {return m_allHits.size();}
  int noOfClusters() const {return m_allClusters.size();}
  int noOfHitsOnTrack() const {return m_allHitsOnTrack.size();}
  int noOfClustersOnTrack() const {return m_allClustersOnTrack.size();}
  int noOfHitsOnTrackAndPierced() const {return m_allHitsOnTrackAndPierced.size();}
  int noOfClustersOnTrackAndPierced() const {return m_allClustersOnTrackAndPierced.size();}
  int noOfLayersWithEnergyDeposition() const;
  int noOfLayersWithEnergyDepositionOnTrack() const;
  int noOfLayersWithEnergyDepositionOnTrackAndPierced() const;
  int noOfLayersWithEnergyDepositionOnTrackPerLength() const;
  int noOfLayersWithEnergyDepositionOnTrackPerMinLength() const; // not necessary

private:
  void checkGoodTRDEvent(const Track*);

public:
  static const int s_spectrumDefaultBins;
  static const bool s_calculateLengthInTube;
  static const int s_minLayerCut;
  static const int s_maxOffTrackCluster;

private:
  Flags m_flags;

  QList<const Hit*> m_allHits;
  QList<const Cluster*> m_allClusters;
  QList<const Hit*> m_allHitsOnTrack;
  QList<const Cluster*> m_allClustersOnTrack;
  QList<const Hit*> m_allHitsOnTrackAndPierced;
  QList<const Cluster*> m_allClustersOnTrackAndPierced;
  QMultiMap<int, const Hit*> m_layerAllHits;
  QMultiMap<int, const Cluster*> m_layerAllClusters;
  QMultiMap<int, const Hit*> m_layerAllHitsOnTrack;
  QMultiMap<int, const Hit*> m_layerAllHitsOnTrackAndPierced;
  QMultiMap<int, const Cluster*> m_layerAllClustersOnTrack;
  QMultiMap<int, const Cluster*> m_layerAllClustersOnTrackAndPierced;
  QVector<double> m_layerEnergyDeposition;
  QVector<double> m_layerEnergyDepositionOnTrack;
  QVector<double> m_layerEnergyDepositionOnTrackAndPierced;
  QVector<double> m_layerLengthThroughTube;
  QVector<double> m_layerEnergyDepositionOnTrackPerLength;
  QVector<double> m_layerEnergyDepositionOnTrackPerMinLength;
  QMap<unsigned short, double> m_channelEnergyDeposition;
  QMap<unsigned short, double> m_channelEnergyDepositionOnTrack;
  QMap<unsigned short, double> m_channelEnergyDepositionOnTrackPerLength;
  QMap<unsigned short, double> m_channelEnergyDepositionOnTrackPerMinLength;
  QMap<unsigned short, double> m_moduleEnergyDeposition;
  QMap<unsigned short, double> m_moduleEnergyDepositionOnTrack;
  QMap<unsigned short, double> m_moduleEnergyDepositionOnTrackPerLength;
  QMap<unsigned short, double> m_moduleEnergyDepositionOnTrackPerMinLength;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(TRDReconstruction::Flags);

#endif // TRDRECONSTRUCTION_HH
