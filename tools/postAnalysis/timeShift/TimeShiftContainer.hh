#ifndef TimeShiftContainer_hh
#define TimeShiftContainer_hh

#include "Constants.hh"

#include <QMultiMap>

class TimeShiftContainer {
public:
  static unsigned int s_bars[8][2];
  static TimeShiftContainer* instance();
  double data(int id, int channel);
  void setData(int id, int channel, double);
  void shiftOnFirstChannel();
  void applyBarShift();
  void dump();
  void printResults();
private:
  TimeShiftContainer();
  ~TimeShiftContainer();
  static TimeShiftContainer* s_instance;
  double m_data[Constants::nTofChannels][2*Constants::nTofSipmsPerBar];
};

#endif
