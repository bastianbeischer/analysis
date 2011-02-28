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

unsigned int nGlobal = 63;
unsigned int nLocal = 1;
unsigned int nStandardDeviations = 3;
int verboseLevel = 1;
float sigmaSipm = 0.6f;

int main(int argc, char** argv)
{
  if (argc != 2)
    qFatal("Wrong number of arguments!");
  QFile file(argv[1]);
  if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text))
    qFatal("Could not open file!");
  QTextStream stream(&file);

  INITGL(nGlobal, nLocal, nStandardDeviations, verboseLevel);
  
  float globalParameters[nGlobal];
  for (unsigned int i = 0; i < nGlobal; ++i)
    globalParameters[i] = 0.0f;
  PARGLO(globalParameters);
  
  float localParameters[nLocal];
  for (unsigned int i = 0; i < nLocal; ++i)
    localParameters[i] = 0.0f;
  localParameters[0] = 1.0f;

  while (!stream.atEnd()) {
    QString line = stream.readLine();
    line.remove("\"");
    line.replace(QRegExp("  *"), " ");
    line.remove(QRegExp(" *$"));
    QStringList list = line.split(' ');
    int n = list.count();
    QMap<int, float> times;
    for (int i = 0; i < (n - 5) / 2; ++i) {
      int ch = list[2*i].toInt(0, 16) - 0x8000;
      float t = list[2*i+1].toFloat();
      times.insert(ch, t);
    }
    QPointF upperPoint(list[n-5].toFloat(), list[n-4].toFloat());
    QPointF lowerPoint(list[n-3].toFloat(), list[n-2].toFloat());
    float trackLength = list[n-1].toFloat();
    unsigned int nHits = times.size();
    unsigned int nRows = nHits - nLocal;

    float* measurement = new float[nRows];
    
    {
      unsigned int row = 0;
      QMap<int, float>::ConstIterator it = times.begin();
      QMap<int, float>::ConstIterator itEnd = times.end();
      for (; it != itEnd; ++it) {
        for (unsigned int row = 0; row < nRows; ++row) {

        }
      }
    }
    //qDebug() << '\n' << list << '\n' << times << upperPoint << ' ' << lowerPoint << ' ' << trackLength;
    
    for (unsigned int row = 0; row < nRows; ++row) {
      for (unsigned int column = 0; column < nGlobal; ++column)
        globalParameters[column] = (column == row) ? 1.0f : 0.0f;
      EQULOC(globalParameters, localParameters, measurement[row], sigmaSipm);
    }
    FITLOC();
    delete[] measurement;
  }
  FITGLO(globalParameters);
  for (unsigned int i = 0; i < nGlobal; ++i) {
    qDebug() << i << globalParameters[i];
  }
  return 0;
}
