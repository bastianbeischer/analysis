#include "Constants.hh"

#include <iostream>
#include <cfloat>

#include <QDebug>
#include <QtGlobal>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QRegExp>
#include <QPointF>

typedef QMap<int, double> TimeMap;
QVector<TimeMap> times;
QVector<QPointF> upperPoint;
QVector<QPointF> lowerPoint;
QVector<double> trackLength;
double channelDt[Constants::nTofBars][Constants::nTofSipmsPerBar];
double barDt[Constants::nTofBars];

int id(int bar, int sipm)
{
  return 0x8000 | (bar * Constants::nTofSipmsPerBar + sipm);
}

void showInfo(int argc, char** argv)
{
  if (argc != 2)
    qFatal("Wrong number of arguments!");
  QFile file(argv[1]);
  if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text))
    qFatal("Could not open file!");
}

void readValues(const QString& fileName)
{
  QFile file(fileName);
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  QTextStream stream(&file);
  while (!stream.atEnd()) {
    QString line = stream.readLine();
    line.remove("\"");
    line.replace(QRegExp("  *"), " ");
    line.remove(QRegExp(" *$"));
    QStringList list = line.split(' ');
    int n = list.count();
    TimeMap t;
    for (int i = 0; i < (n - 5) / 2; ++i) {
      int ch = list[2*i].toInt(0, 16) - 0x8000;
      double tChannel = list[2*i+1].toDouble();
      t.insert(ch, tChannel);
    }
    times.append(t);
    upperPoint.append(QPointF(list[n-5].toDouble(), list[n-4].toDouble()));
    lowerPoint.append(QPointF(list[n-3].toDouble(), list[n-2].toDouble()));
    trackLength.append(list[n-1].toDouble());
  }
  file.close();
}

void alignChannels()
{
  double dt[Constants::nTofBars][Constants::nTofSipmsPerBar];
  int counter[Constants::nTofBars][Constants::nTofSipmsPerBar];
  for (int bar = 0; bar < Constants::nTofBars; ++bar) {
    for (int sipm = 0; sipm < Constants::nTofSipmsPerBar; ++sipm) {
      dt[bar][sipm] = 0;
      counter[bar][sipm] = 0;
    }
  }

  for (int event = 0; event < times.size(); ++event) {
    unsigned int nHits = times[event].size();
    if (nHits != 15 && nHits != 16)
      continue;
    int upperReferenceChannel = Constants::nTofChannels;
    double upperReferenceTime = 0;
    int lowerReferenceChannel = Constants::nTofChannels;
    double lowerReferenceTime = 0;
    QMap<int, double>::ConstIterator it = 0;
    QMap<int, double>::ConstIterator itEnd = times[event].end();
    
    for (it = times[event].begin(); it != itEnd; ++it) {
      int channel = it.key();
      int bar = channel / Constants::nTofSipmsPerBar;
      int sipm = channel % Constants::nTofSipmsPerBar;
      bool top = channel < Constants::nTofChannels / 2;
      if (top && channel < upperReferenceChannel) {
        upperReferenceChannel = channel;
        upperReferenceTime = it.value() - channelDt[bar][sipm];
      }
      if (!top && channel < lowerReferenceChannel) {
        lowerReferenceChannel = channel;
        lowerReferenceTime = it.value() - channelDt[bar][sipm];
      }
    }
  
    for (it = times[event].begin(); it != itEnd; ++it) {
      int channel = it.key();
      int bar = channel / Constants::nTofSipmsPerBar;
      int sipm = channel % Constants::nTofSipmsPerBar;
      bool top = channel < Constants::nTofChannels / 2;
      if(!upperReferenceTime || !lowerReferenceTime)
        break;
      if (top && qAbs(upperPoint[event].ry()) < 10) {
        dt[bar][sipm]+= it.value() + channelDt[bar][sipm] - upperReferenceTime;
        ++counter[bar][sipm];
      }
      if (!top && qAbs(lowerPoint[event].ry()) < 10) {
        dt[bar][sipm]+= it.value() + channelDt[bar][sipm] - lowerReferenceTime;
        ++counter[bar][sipm];
      }
    }
  }

  for (int bar = 0; bar < Constants::nTofBars; ++bar) {
    for (int sipm = 0; sipm < Constants::nTofSipmsPerBar; ++sipm) {
      if (counter[bar][sipm])
        dt[bar][sipm]/= counter[bar][sipm];
      channelDt[bar][sipm]-= dt[bar][sipm];
    }
  }
}

void alignBars()
{
}

void printResults()
{
  for (int bar = 0; bar < Constants::nTofBars; ++bar)
    for (int sipm = 0; sipm < Constants::nTofSipmsPerBar; ++sipm)
      qDebug() << qPrintable(QString("%1=%2").arg(id(bar, sipm), 0, 16).arg(-channelDt[bar][sipm]));
}

int main(int argc, char** argv)
{
  showInfo(argc, argv);

  readValues(argv[1]);
  
  for (int bar = 0; bar < Constants::nTofBars; ++bar)
    for (int sipm = 0; sipm < Constants::nTofSipmsPerBar; ++sipm)
      channelDt[bar][sipm] = 0;
  alignChannels();
  
  for (int bar = 0; bar < Constants::nTofBars; ++bar)
    barDt[bar] = 0;
  for (int iteration = 0; iteration < 10; ++iteration)
    alignBars();
  
  printResults();

  return 0;
}
