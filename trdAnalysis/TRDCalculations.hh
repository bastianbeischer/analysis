#ifndef TRDCALCULATIONS_HH
#define TRDCALCULATIONS_HH

class Hit;
class Track;
class QVector2D;

class TRDCalculations
{

public:
  static double distanceOnTrackThroughTRDTube(const Hit* hit, const Track* track);
  static double distanceTrackToWire(const Hit* hit, const Track* track);
  static double distanceTrackToWire(const QVector2D& trdChanPos2D, const Track* track);

  static unsigned short TRDLayerNo(const Hit* hit);
  static unsigned short TRDLayerNo(const unsigned int detID);

private:
  TRDCalculations();

};

#endif // TRDCALCULATIONS_HH
