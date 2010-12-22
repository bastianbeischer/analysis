#ifndef TRDCALCULATIONS_HH
#define TRDCALCULATIONS_HH

class Hit;
class Track;

class TRDCalculations
{

public:
  static double distanceOnTrackThroughTRDTube(const Hit* hit, const Track* track);
  static double distanceTrackToWire(const Hit* hit, const Track* track);

private:
  TRDCalculations();

};

#endif // TRDCALCULATIONS_HH
