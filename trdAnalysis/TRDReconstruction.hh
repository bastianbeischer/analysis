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
    void reconstructTRD(SimpleEvent* event, Track* globalTrack);
    QVector<Hit*> allTRDHits() {return m_allHits;}
    QVector<Cluster*> allTRDClusters() {return m_allClusters;}
private:
    QVector<Hit*> m_allHits;
    QVector<Cluster*> m_allClusters;
    QVector<Hit*> m_allHitsOnTrack;
    QVector<Cluster*> m_allClustersOnTrack;
    QVector<Hit*> m_allHitsOnTrackAndPierced;
    QVector<Cluster*> m_allClustersOnTrackAndPierced;
    QMultiMap<int, Hit*> m_layerAllHits;
    QMultiMap<int, Cluster*> m_layerAllClusters;
    QMultiMap<int, Hit*> m_layerAllHitsOnTrack;
    QMultiMap<int, Hit*> m_layerAllHitsOnTrackAndPierced;
    QMultiMap<int, Cluster*> m_layerAllClustersOnTrack;
    QMultiMap<int, Cluster*> m_layerAllClustersOnTrackAndPierced;
    QVector<double> m_layerEnergyDeposition;
    QVector<double> m_layerEnergyDepositionOnTrack;
    QVector<double> m_layerEnergyDepositionOnTrackAndPierced;
    QVector<double> m_layerLengthThroughTube;
    QVector<double> m_layerEnergyDepositionOnTrackPerLength;
};

#endif // TRDRECONSTRUCTION_HH
