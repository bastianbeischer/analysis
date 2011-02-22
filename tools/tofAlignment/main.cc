#include "millepede.h"

#include "Constants.hh"

#include <iostream>

#include <QDebug>
#include <QtGlobal>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QRegExp>
#include <QPointF>

unsigned int nGlobal = 3;
unsigned int nLocal = 1;
unsigned int nStandardDeviations = 3;
int verboseLevel = 1;
int sigmaSipm = 0.6;

int main(int argc, char** argv)
{
  if (argc != 2)
    qFatal("Wrong number of arguments!");
  QFile file(argv[1]);
  if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text))
    qFatal("Could not open file!");
  QTextStream stream(&file);

  INITGL(nGlobal, nLocal, nStandardDeviations, verboseLevel);

  while (!stream.atEnd()) {
    QString line = stream.readLine();
    line.remove("\"");
    line.replace(QRegExp("  *"), " ");
    line.remove(QRegExp(" *$"));
    QStringList list = line.split(' ');
    int n = list.count();
    QMap<int, double> times;
    for (int i = 0; i < (n - 5) / 2; ++i) {
      int ch = list[2*i].toInt(0, 16) - 0x8000;
      double t = list[2*i+1].toDouble();
      times.insert(ch, t);
    }
    QPointF upperPoint(list[n-5].toDouble(), list[n-4].toDouble());
    QPointF lowerPoint(list[n-3].toDouble(), list[n-2].toDouble());
    double trackLength = list[n-1].toDouble();
    //qDebug() << '\n' << list << '\n' << times << upperPoint << ' ' << lowerPoint << ' ' << trackLength;
    

  }

  return 0;
}
