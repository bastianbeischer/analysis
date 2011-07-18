#ifndef TRDRECONSTRUCTION_HH
#define TRDRECONSTRUCTION_HH

class Hit;
class Cluster;
class SimpleEvent;
class Track;

#include <QVector>
#include <QMap>
#include <QMultiMap>

class TRDReconstruction
{
public:
    TRDReconstruction();
    void reset();
    void reconstructTRD(SimpleEvent* event, Track* globalTrack);
    const QList<const Hit*>& getAllHits() const {return m_allHits;}
    const QList<const Cluster*>& getAllClusters() const {return m_allClusters;}
    const QList<const Hit*>& getAllHitsOnTrack() const {return m_allHitsOnTrack;}
    const QList<const Cluster*>& getAllClustersOnTrack() const {return m_allClustersOnTrack;}
    const QList<const Hit*>& getAllHitsOnTrackAndPierced() const {return m_allHitsOnTrackAndPierced;}
    const QList<const Cluster*>& getAllClustersOnTracAndPierced() const {return m_allClustersOnTrackAndPierced;}
    const QList<const Hit*> getAllHitsForLayer(int layer) const {return m_layerAllHits.values(layer);}
    const QList<const Cluster*> getAllClustersForLayer(int layer) const {return m_layerAllClusters.values(layer);}
    const QList<const Hit*> getAllHitsOnTrackForLayer(int layer) const {return m_layerAllHitsOnTrack.values(layer);}
    const QList<const Cluster*> getAllClustersOnTrackForLayer(int layer) const {return m_layerAllClustersOnTrack.values(layer);}
    const QList<const Hit*> getAllHitsOnTrackAndPiercedForLayer(int layer) const {return m_layerAllHitsOnTrackAndPierced.values(layer);}
    const QList<const Cluster*> getAllClustersOnTracAndPiercedkForLayer(int layer) const {return m_layerAllClustersOnTrackAndPierced.values(layer);}
    double getEnergyDepositionForLayer(int layer) const {return m_layerEnergyDeposition.at(layer);}
    double getEnergyDepositionOnTrackForLayer(int layer) const {return m_layerEnergyDepositionOnTrack.at(layer);}
    double getEnergyDepositionOnTrackAndPiercedForLayer(int layer) const {return m_layerEnergyDepositionOnTrackAndPierced.at(layer);}
    double getLengthThroughTubeForLayer(int layer) const {return m_layerLengthThroughTube.at(layer);}
    double getEnergyDepositionOnTrackPerLengthForLayer(int layer) const {return m_layerEnergyDepositionOnTrackPerLength.at(layer);}
    double getEnergyDepositionOnTrackPerMinimumLengthForLayer(int layer) const {return m_layerEnergyDepositionOnTrackPerMinLength.at(layer);}

    int getNoOfHits() const {return m_allHits.size();}
    int getNoOfClusters() const {return m_allClusters.size();}
    int getNoOfHitsOnTrack() const {return m_allHitsOnTrack.size();}
    int getNoOfClustersOnTrack() const {return m_allClustersOnTrack.size();}
    int getNoOfHitsOnTrackAndPierced() const {return m_allHitsOnTrackAndPierced.size();}
    int getNoOfClustersOnTrackAndPierced() const {return m_allClustersOnTrackAndPierced.size();}
    int getNoOfLayersWithEnergyDeposition() const;
    int getNoOfLayersWithEnergyDepositionOnTrack() const;
    int getNoOfLayersWithEnergyDepositionOnTrackAndPierced() const;
    int getNoOfLayersWithEnergyDepositionOnTrackPerLength() const;
    /*not necessery*/int getNoOfLayersWithEnergyDepositionOnTrackWithMinumLength() const;

private:
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
};

#endif // TRDRECONSTRUCTION_HH
