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
    QList<Hit*> getAllHits() {return m_allHits;}
    QList<Cluster*> getAllClusters() {return m_allClusters;}
    QList<Hit*> getAllHitsOnTrack() {return m_allHitsOnTrack;}
    QList<Cluster*> getAllClustersOnTrack() {return m_allClustersOnTrack;}
    QList<Hit*> getAllHitsOnTrackAndPierced() {return m_allHitsOnTrackAndPierced;}
    QList<Cluster*> getAllClustersOnTracAndPierced() {return m_allClustersOnTrackAndPierced;}
    QList<Hit*> getAllHitsForLayer(int layer) {return m_layerAllHits.values(layer);}
    QList<Cluster*> getAllClustersForLayer(int layer) {return m_layerAllClusters.values(layer);}
    QList<Hit*> getAllHitsOnTrackForLayer(int layer) {return m_layerAllHitsOnTrack.values(layer);}
    QList<Cluster*> getAllClustersOnTrackForLayer(int layer) {return m_layerAllClustersOnTrack.values(layer);}
    QList<Hit*> getAllHitsOnTrackAndPiercedForLayer(int layer) {return m_layerAllHitsOnTrackAndPierced.values(layer);}
    QList<Cluster*> getAllClustersOnTracAndPiercedkForLayer(int layer) {return m_layerAllClustersOnTrackAndPierced.values(layer);}
    double getEnergyDepositionForLayer(int layer) {return m_layerEnergyDeposition.at(layer);}
    double getEnergyDepositionOnTrackForLayer(int layer) {return m_layerEnergyDepositionOnTrack.at(layer);}
    double getEnergyDepositionOnTrackAndPiercedForLayer(int layer) {return m_layerEnergyDepositionOnTrackAndPierced.at(layer);}
    double getLengthThroughTubeForLayer(int layer) {return m_layerLengthThroughTube.at(layer);}
    double getEnergyDepositionOnTrackPerLengthForLayer(int layer) {return m_layerEnergyDepositionOnTrackPerLength.at(layer);}

    int getNoOfHits() {return m_allHits.size();}
    int getNoOfClusters() {return m_allClusters.size();}
    int getNoOfHitsOnTrack() {return m_allHitsOnTrack.size();}
    int getNoOfClustersOnTrack() {return m_allClustersOnTrack.size();}
    int getNoOfHitsOnTrackAndPierced() {return m_allHitsOnTrackAndPierced.size();}
    int getNoOfClustersOnTrackAndPierced() {return m_allClustersOnTrackAndPierced.size();}

private:
    QList<Hit*> m_allHits;
    QList<Cluster*> m_allClusters;
    QList<Hit*> m_allHitsOnTrack;
    QList<Cluster*> m_allClustersOnTrack;
    QList<Hit*> m_allHitsOnTrackAndPierced;
    QList<Cluster*> m_allClustersOnTrackAndPierced;
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
