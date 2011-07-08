#include "TimeShiftContainer.hh"
#include "BarShiftPlot.hh"

#include <QSettings>
#include <QDebug>
#include <iostream>
#include <cmath>
#include <iomanip>

TimeShiftContainer* TimeShiftContainer::s_instance = 0;

unsigned int TimeShiftContainer::s_bars[Constants::nTofBars/2][2] = {
  {0x8000, 0x8010}, {0x8004, 0x8014}, {0x8008, 0x8018}, {0x800c, 0x801c},
  {0x8020, 0x8030}, {0x8024, 0x8034}, {0x8028, 0x8038}, {0x802c, 0x803c}
};

TimeShiftContainer::TimeShiftContainer()
  : m_configFile()
{
  for (int ch = 0; ch < Constants::nTofChannels; ++ch) {
    m_channelShift[ch] = 0;
    m_result[ch] = NAN;
    m_resultWithBarShift[ch] = NAN;
    for (int barCh = 0; barCh < 2*Constants::nTofSipmsPerBar; ++barCh)
      m_data[ch][barCh] = NAN;
  }
}

TimeShiftContainer::~TimeShiftContainer()
{
  s_instance = 0;
}

TimeShiftContainer* TimeShiftContainer::instance()
{
  if (!s_instance)
    s_instance = new TimeShiftContainer;
  return s_instance;
}

void TimeShiftContainer::shiftOnFirstChannel()
{
  for (int barPair = 0; barPair < Constants::nTofBars/2; ++barPair) {
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
 
void TimeShiftContainer::dump()
{
  for (int barPair = 0; barPair < Constants::nTofBars/2; ++barPair) {
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
  std::cout << "results:" << std::endl;
  for (int ch = 0; ch < Constants::nTofChannels; ++ch)
    std::cout << "0x" << std::hex << (0x8000 + ch) << '=' << m_result[ch] << std::endl;
  std::cout << "results with bar shifts:" << std::endl;
  for (int ch = 0; ch < Constants::nTofChannels; ++ch)
    std::cout << "0x" << std::hex << (0x8000 + ch) << '=' << m_resultWithBarShift[ch] << std::endl;
  std::cout << std::endl << std::endl;
}

void TimeShiftContainer::finalizeChannelShifts()
{
  for (int ch = 0; ch < Constants::nTofChannels; ++ch) {
    QVector<double> shifts;
    for (int refCh = 0; refCh < 2*Constants::nTofSipmsPerBar; ++refCh)
      shifts.append(m_data[ch][refCh]);
    qSort(shifts);
    int n = 0;
    double sum = 0.;
    for (int i = 2; i < shifts.size() - 2; ++i) {
      ++n;
      sum+= shifts[i];
    }
    m_result[ch] = sum / n + m_channelShift[ch];
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

void TimeShiftContainer::setConfigFile(const QString& file)
{
  QSettings settings(file, QSettings::IniFormat);
  for (int ch = 0; ch < Constants::nTofChannels; ++ch) {
    int id = 0x8000 + ch;
    double shift = settings.value("tofTimeShift/0x" + QString::number(id, 16), 1).toDouble();
    m_channelShift[ch] = shift;
  }
}

void TimeShiftContainer::saveToConfigfile(const QString& file, bool withBarShift)
{
  QSettings settings(file, QSettings::IniFormat);
  for (int ch = 0; ch < Constants::nTofChannels; ++ch) {
    int id = 0x8000 + ch;
    settings.setValue("tofTimeShift/0x" + QString::number(id, 16), withBarShift ? m_resultWithBarShift[ch] : m_result[ch]);
  }
}

double TimeShiftContainer::desiredTimeDifference(int barPositionDistance)
{
  double z = (Constants::upperTofPosition - Constants::lowerTofPosition);
  double x = barPositionDistance * Constants::tofBarWidth;
  return sqrt(x*x + z*z) / Constants::speedOfLight;
}

void TimeShiftContainer::applyBarShifts(const QVector<BarShiftPlot*>& plots)
{
  QVector<int> upperBar;
  QVector<int> lowerBar;
  QVector<double> bVector;
  QVector<double> errBVector;

  foreach(BarShiftPlot* plot, plots) {
    if (true || !std::isnan(plot->dt())) {
      int upper = 0;
      int lower = 0;
      if (plot->title().contains("bar shift 0x8000 0x8010")) upper = 0;
      if (plot->title().contains("bar shift 0x8004 0x8014")) upper = 1;
      if (plot->title().contains("bar shift 0x8008 0x8018")) upper = 2;
      if (plot->title().contains("bar shift 0x800c 0x801c")) upper = 3;
      if (plot->title().contains("0x8020 0x8030")) lower = 0;
      if (plot->title().contains("0x8024 0x8034")) lower = 1;
      if (plot->title().contains("0x8028 0x8038")) lower = 2;
      if (plot->title().contains("0x802c 0x803c")) lower = 3;
      int barPositionDistance = qAbs(upper - lower);
      if (!std::isnan(plot->dt())) {
        upperBar.append(upper);
        lowerBar.append(lower);
        bVector.append(desiredTimeDifference(barPositionDistance) - plot->dt());
        errBVector.append(plot->errDt());
      }
    }
  }

  int n = bVector.count();
  TMatrixT<double> b(n, 1);
  for (int i = 0; i < n; ++i) {
    b[i][0] = bVector[i];
  }

  int m = Constants::nTofBars/2;
  TMatrixT<double> AFull(n, m);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      AFull[i][j] = 0;
    }
    int j = 0;
    j = upperBar[i];
    AFull[i][j] = -1;
    j = lowerBar[i] + 4;
    AFull[i][j] = 1;
  }
  TMatrixT<double> A;
  AFull.GetSub(0, n-1, 1, m-1, A);
  --m;
  
  TMatrixT<double> At(m, n);
  At.Transpose(A);
  TMatrixT<double> Result = (At * A).Invert() * At * b;
  dumpMatrix(A);
  dumpMatrix(b);
  dumpMatrix(Result);
  double shift[Constants::nTofBars/2];
  shift[0] = 0;
  for (int i = 0; i < Constants::nTofBars/2 - 1; ++i)
    shift[i+1] = Result[i][0];
  for (int ch = 0; ch < Constants::nTofChannels; ++ch) {
    int bar = (ch / Constants::nTofSipmsPerBar) % 4 + 4 * (ch / 32);
    //qDebug() << hex << (ch+0x8000) << dec << " -> " << bar;
    m_resultWithBarShift[ch] = m_result[ch] + shift[bar];
  }
}
 
void TimeShiftContainer::dumpMatrix(const TMatrixT<double>& m)
{
  int width = 9;
  int space = 2;
  std::cout << " _ ";
  for (int i = 0; i < (width + space) * m.GetNcols() - space + 1; ++i) std::cout << ' ';
  std::cout << "_ " << std::endl;
  for (int r = 0; r < m.GetNrows(); ++r) {
    for (int c = 0; c < m.GetNcols(); ++c) {
      if (c) {
        for (int i = 0; i < space; ++i) std::cout << ' ';
      } else {
        std::cout << (r == m.GetNrows() - 1 ? "|_ " : "|  ");
      }
      std::cout << std::setfill('x') << std::setw(width) << std::showpos << std::fixed << m[r][c];
    }
    std::cout << (r == m.GetNrows() - 1 ? " _|" : "  |") << std::endl;
  }
}
