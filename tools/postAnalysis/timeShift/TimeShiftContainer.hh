#ifndef TimeShiftContainer_hh
#define TimeShiftContainer_hh

#include "Constants.hh"

#include <QString>

class TimeShiftContainer {
public:
  static unsigned int s_bars[Constants::nTofBars/2][2];
  static TimeShiftContainer* instance();
  double data(int id, int channel);
  void setData(int id, int channel, double);
  void setConfigFile(const QString&);
  void shiftOnFirstChannel();
  void applyBarShifts(double [Constants::nTofBars/2]);
  void finalizeChannelShifts();
  void dump();
  void saveToConfigfile(const QString&);
private:
  TimeShiftContainer();
  ~TimeShiftContainer();
  static TimeShiftContainer* s_instance;
  QString m_configFile;
  double m_channelShift[Constants::nTofChannels];
  double m_result[Constants::nTofChannels];
  double m_data[Constants::nTofChannels][2*Constants::nTofSipmsPerBar];
};

#endif
