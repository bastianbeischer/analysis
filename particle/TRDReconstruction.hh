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

struct EnergyDeposition {
  double edep;
  double edepOnTrack;
  double lengthThroughTube;
  double edepOnTrackAndPierced;
  double edepOnTrackPerLength;
};

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

  static unsigned char TRDLayerNo(const Hit* hit);
  static unsigned char TRDLayerNo(const unsigned short detID);

  static double spectrumUpperLimit() {return TRDReconstruction::s_calculateLengthInTube ? 15 : 50 ;}
  static QString xAxisTitle() {return TRDReconstruction::s_calculateLengthInTube ? "energy deposition length in tube / (keV/mm)" : "energy deposition / keV" ;}

public:
  void reset();
  void reconstructTRD(SimpleEvent* event, Track* globalTrack);

  const Flags& flags() const {return m_flags;}

  const QVector<const Hit*>& allHits() const {return m_allHits;}
  const QVector<const Cluster*>& allClusters() const {return m_allClusters;}
  const QVector<const Hit*>& allHitsOnTrack() const {return m_allHitsOnTrack;}
  const QVector<const Cluster*>& allClustersOnTrack() const {return m_allClustersOnTrack;}
  const QVector<const Hit*>& allHitsOnTrackAndPierced() const {return m_allHitsOnTrackAndPierced;}
  const QVector<const Cluster*>& allClustersOnTrackAndPierced() const {return m_allClustersOnTrackAndPierced;}
  const QVector<const Hit*>& allHitsForLayer(int layer) const {return m_layerAllHits[layer];}
  const QVector<const Cluster*>& allClustersForLayer(int layer) const {return m_layerAllClusters[layer];}
  const QVector<const Hit*>& allHitsOnTrackForLayer(int layer) const {return m_layerAllHitsOnTrack[layer];}
  const QVector<const Cluster*>& allClustersOnTrackForLayer(int layer) const {return m_layerAllClustersOnTrack[layer];}
  const QVector<const Hit*>& allHitsOnTrackAndPiercedForLayer(int layer) const {return m_layerAllHitsOnTrackAndPierced[layer];}
  const QVector<const Cluster*>& allClustersOnTrackAndPiercedForLayer(int layer) const {return m_layerAllClustersOnTrackAndPierced[layer];}

  const EnergyDeposition& energyDepositionForLayer(int layer) const {return m_layerEdep[layer];}
  const EnergyDeposition& energyDepositionForChannel(unsigned short channelID) const {return m_channelEdep.constFind(channelID).value();}
  const EnergyDeposition& energyDepositionForModule(unsigned short moduleID) const {return m_moduleEdep.constFind(moduleID).value();}

  int noOfHits() const {return m_allHits.size();}
  int noOfClusters() const {return m_allClusters.size();}
  int noOfHitsOnTrack() const {return m_allHitsOnTrack.size();}
  int noOfClustersOnTrack() const {return m_allClustersOnTrack.size();}
  int noOfHitsOnTrackAndPierced() const {return m_allHitsOnTrackAndPierced.size();}
  int noOfClustersOnTrackAndPierced() const {return m_allClustersOnTrackAndPierced.size();}
  int noOfLayersWithEnergyDeposition() const;
  int noOfLayersWithEnergyDepositionOnTrack() const;
  int noOfLayersWithEnergyDepositionOnTrackAndPierced() const;

private:
  void checkGoodTRDEvent(const Track*);

public:
  static const int s_spectrumDefaultBins;
  static const bool s_calculateLengthInTube;
  static const int s_minLayerCut;
  static const int s_maxOffTrackCluster;

private:
  Flags m_flags;

  QVector<const Hit*> m_allHits;
  QVector<const Cluster*> m_allClusters;
  QVector<const Hit*> m_allHitsOnTrack;
  QVector<const Cluster*> m_allClustersOnTrack;
  QVector<const Hit*> m_allHitsOnTrackAndPierced;
  QVector<const Cluster*> m_allClustersOnTrackAndPierced;

  QVector<const Hit*> m_layerAllHits[8];
  QVector<const Cluster*> m_layerAllClusters[8];
  QVector<const Hit*> m_layerAllHitsOnTrack[8];
  QVector<const Hit*> m_layerAllHitsOnTrackAndPierced[8];
  QVector<const Cluster*> m_layerAllClustersOnTrack[8];
  QVector<const Cluster*> m_layerAllClustersOnTrackAndPierced[8];

  EnergyDeposition m_layerEdep[8];

  QMap<unsigned short, EnergyDeposition> m_channelEdep;
  QMap<unsigned short, EnergyDeposition> m_moduleEdep;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(TRDReconstruction::Flags);

#endif // TRDRECONSTRUCTION_HH
