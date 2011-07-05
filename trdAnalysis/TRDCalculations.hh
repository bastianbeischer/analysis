#ifndef TRDCALCULATIONS_HH
#define TRDCALCULATIONS_HH

class Hit;
class Track;
class Particle;
class SimpleEvent;
class QVector2D;

#include <QString>

class TRDCalculations
{

public:
  static double distanceOnTrackThroughTRDTube(const Hit* hit, const Track* track);
  static double distanceTrackToWire(const Hit* hit, const Track* track);
  static double distanceTrackToWire(const QVector2D& trdChanPos2D, const Track* track);

  static unsigned short TRDLayerNo(const Hit* hit);
  static unsigned short TRDLayerNo(const unsigned int detID);

  static bool globalTRDCuts(const QVector<Hit*>&, const Particle* particle, const SimpleEvent* event);
  static const int spectrumDefaultBins;

  static double spectrumUpperLimit() {return TRDCalculations::calculateLengthInTube ? 15 : 50 ;}
  static QString xAxisTitle() {return TRDCalculations::calculateLengthInTube ? "energy deposition length in tube / (keV/mm)" : "energy deposition / keV" ;}

public:
  static const bool calculateLengthInTube;
  static const int minTRDLayerCut;

private:
  TRDCalculations();

};

#endif // TRDCALCULATIONS_HH
