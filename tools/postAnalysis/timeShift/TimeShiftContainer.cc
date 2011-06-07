#include "TimeShiftContainer.hh"

#include <QDebug>
#include <iostream>
#include <cmath>

TimeShiftContainer* TimeShiftContainer::s_instance = 0;

unsigned int TimeShiftContainer::s_bars[8][2] = {
  {0x8000, 0x8010}, {0x8004, 0x8014}, {0x8008, 0x8018}, {0x800c, 0x801c},
  {0x8020, 0x8030}, {0x8024, 0x8034}, {0x8028, 0x8038}, {0x802c, 0x803c}
};

TimeShiftContainer::TimeShiftContainer()
{
  for (int ch = 0; ch < Constants::nTofChannels; ++ch)
    for (int barCh = 0; barCh < 2*Constants::nTofSipmsPerBar; ++barCh)
      m_data[ch][barCh] = NAN;
}

TimeShiftContainer::~TimeShiftContainer()
{
}

TimeShiftContainer* TimeShiftContainer::instance()
{
  if (!s_instance)
    s_instance = new TimeShiftContainer;
  return s_instance;
}

void TimeShiftContainer::shiftOnFirstChannel()
{
  for (int barPair = 0; barPair < 8; ++barPair) {
    int refId = s_bars[barPair][0];
    for (int refCh = 0; refCh < 2*Constants::nTofSipmsPerBar; ++refCh) {
      double value = data(refId, refCh);
      for (int bar = 0; bar < 2; ++bar) {
        for (int barCh = 0; barCh < Constants::nTofSipmsPerBar; ++barCh) {
          int id = s_bars[barPair][bar] + barCh;
          setData(id, refCh, data(id, refCh) - value);
        }
      }
    }
  }
}

void TimeShiftContainer::applyBarShift()
{

}
  
void TimeShiftContainer::dump()
{
  for (int barPair = 0; barPair < 8; ++barPair) {
    for (int bar = 0; bar < 2; ++bar) {
      for (int barCh = 0; barCh < Constants::nTofSipmsPerBar; ++barCh) {
        int id = s_bars[barPair][bar] + barCh;
        std::cout << "0x" << std::hex << id;
        for (int refCh = 0; refCh < 2*Constants::nTofSipmsPerBar; ++refCh)
          std::cout << '\t' << std::showpos << std::fixed << data(id, refCh);
        std::cout << std::endl;
      }
    }
  }
  std::cout << std::endl;
}

void TimeShiftContainer::printResults()
{
  for (int ch = 0; ch < Constants::nTofChannels; ++ch) {
    double sum = 0.;
    for (int refCh = 0; refCh < 2*Constants::nTofSipmsPerBar; ++refCh)
      sum+= m_data[ch][refCh];
    std::cout << "0x" << std::hex << (0x8000 + ch) << '=' << (sum / (2*Constants::nTofSipmsPerBar)) << std::endl;
  }
}
  
double TimeShiftContainer::data(int id, int channel)
{
  int i = id - 0x8000;
  Q_ASSERT(0 <= i && i < Constants::nTofChannels);
  Q_ASSERT(0 <= channel && channel <= 2*Constants::nTofSipmsPerBar);
  return m_data[i][channel];
}

void TimeShiftContainer::setData(int id, int channel, double value)
{
  int i = id - 0x8000;
  Q_ASSERT(0 <= i && i < Constants::nTofChannels);
  Q_ASSERT(0 <= channel && channel <= 2*Constants::nTofSipmsPerBar);
  m_data[i][channel] = value;
}
