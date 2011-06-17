#ifndef TimeShiftContainer_hh
#define TimeShiftContainer_hh

#include "Constants.hh"
#include <TMatrixT.h>
#include <QString>

class BarShiftPlot;

class TimeShiftContainer {
public:
  ~TimeShiftContainer();
  static unsigned int s_bars[Constants::nTofBars/2][2];
  static TimeShiftContainer* instance();
  static double desiredTimeDifference(int barPositionDistance);
  double data(int id, int channel);
  void setData(int id, int channel, double);
  void setConfigFile(const QString&);
  void shiftOnFirstChannel();
  void applyBarShifts(const QVector<BarShiftPlot*>&);
  void finalizeChannelShifts();
  void dump();
  void saveToConfigfile(const QString&);
private:
  TimeShiftContainer();
  void dumpMatrix(const TMatrixT<double>&);

  static TimeShiftContainer* s_instance;
  QString m_configFile;
  double m_channelShift[Constants::nTofChannels];
  double m_result[Constants::nTofChannels];
  double m_data[Constants::nTofChannels][2*Constants::nTofSipmsPerBar];
};

#endif
