#ifndef PostAnalysisPlot_hh
#define PostAnalysisPlot_hh

#include "RootPlot.hh"

#include <QMutex>

class Hit;
class Track;
class TrackSelection;
class SimpleEvent;

class PostAnalysisPlot : virtual public RootPlot {
public:
  PostAnalysisPlot();
  virtual ~PostAnalysisPlot();
private:
};

#endif
