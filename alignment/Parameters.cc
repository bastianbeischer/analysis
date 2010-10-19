#include "Parameters.hh"

#include "millepede.h"

#include <cmath>
#include <cassert>
#include <iostream>

//#include <QFile>

Parameters::Parameters() :
  m_nPar(0),
  m_parameters(0),
  m_parameterSigma(0)
{
}

Parameters::Parameters(unsigned int nPar, unsigned int nAngles)
{
  ReInitParameterArrays(nPar, nAngles);
}

Parameters::~Parameters()
{
  delete[] m_parameters;
  delete[] m_parameterSigma;
}

void Parameters::SetMillepedeParameters()
{
  PARGLO(m_parameters);
  for (unsigned int i = 0; i < m_nPar; i++) {
    if (m_parameterSigma[i] >= 0.) {
      unsigned int iPar = i+1;
      PARSIG(iPar, m_parameterSigma[i]);
    }
  }
}

void Parameters::SetAngle(unsigned int i, float angle)
{ 
  assert(i < m_nAngles);

  if (angle == 0. || angle == 90) {
    angle *= M_PI/180.;
    m_angles[i] = angle;
    m_parameters[m_nAngles + i] = 0.;
  }
  else {
    angle *= M_PI/180.;
    m_angles[i] = angle;
    if (fabs(angle) < M_PI/4.)
      m_parameters[m_nPar - m_nAngles + i] = sin(angle)/cos(angle);
    else
      m_parameters[m_nPar - m_nAngles + i] = cos(angle)/sin(angle);
  }

}

void Parameters::ReInitParameterArrays(unsigned int nPar, unsigned int nAngles)
{
  m_nPar           = nPar;
  m_nAngles        = nAngles;
  m_parameters     = new float[nPar];
  m_parameterSigma = new float[nPar];
  m_angles         = new float[nAngles];
  
  for (unsigned int i = 0; i < nPar; i++)
    m_parameters[i] = 0.;

  // -1 means that this sigma is not used.
  for (unsigned int i = 0; i < nPar; i++)
    m_parameterSigma[i] = -1.;

  for (unsigned int i = 0; i < nAngles; i++)
    m_angles[i] = 0.;

  ConstructIndexMap();
}

// bool Parameters::WriteToFile(QFile* file)
// {
//   // for (int iHybrid = 0; iHybrid <= 1; iHybrid++) {
//   //   QString values[2];
//   //   for (int iSipm = 0; iSipm <= 1; iSipm++) {
//   //     values[iSipm] = QString("%1").arg(m_parameters[m_indexMap[ArrayPair(iHybrid,iSipm)]]);
//   //   }
//   //   QString hybrid = QString("%1").arg(iHybrid);
//   //   QString line = QString("%1\t%2\t%3\n").arg(hybrid, values[0], values[1]);
//   //   file->write(qPrintable(line));
//   // }
//   for (int iHybrid = 30; iHybrid <= 37; iHybrid++) {
//     QString values[4];
//     for (int iSipm = 0; iSipm <= 3; iSipm++) {
//       values[iSipm] = QString("%1").arg(m_parameters[m_indexMap[ArrayPair(iHybrid,iSipm)]], 15);
//     }
//     QString hybrid = QString("%1").arg(iHybrid);
//     QString line = QString("%1\t%2\t%3\t%4\t%5\n").arg(hybrid, values[0], values[1], values[2], values[3]);
//     file->write(qPrintable(line));
//   }
//   for (int iHybrid = 20; iHybrid <= 23; iHybrid++) {
//     QString values[4];
//     for (int iSipm = 0; iSipm <= 3; iSipm++) {
//       values[iSipm] = QString("%1").arg(m_parameters[m_indexMap[ArrayPair(iHybrid,iSipm)]], 15);
//     }
//     QString hybrid = QString("%1").arg(iHybrid);
//     QString line = QString("%1\t%2\t%3\t%4\t%5\n").arg(hybrid, values[0], values[1], values[2], values[3]);
//     file->write(qPrintable(line));
//   }

//   // for (unsigned int i = 0; i < m_nPar; i++) {
//   //   int index = i;
//   //   QMap<ArrayPair,int>::iterator it = qFind(m_indexMap.begin(), m_indexMap.end(), index);
//   //   int hybrid = 0, sipm = 0;
//   //   if (it != m_indexMap.end()) {
//   //     hybrid = it.key().first;
//   //     sipm = it.key().second;
//   //   }
//   //   //    std::cout << "hybrid: " << hybrid << "  sipm: " << sipm << " index: " << i << " parameter: " << m_parameters[i] << std::endl;
//   //   // QString hybridString = QString("%1").arg(hybrid);
//   //   // QString sipmString = QString("%1").arg(sipm);
//   //   //    QString parameterString = QString("%1").arg(m_parameters[i]);
    
//   //   QString line = QString("%1\t%2\t%3\n").arg(hybridString,sipmString,parameterString);
//   //   file->write(qPrintable(line));
//   // }
//   return true;
// }

void Parameters::ConstructIndexMap()
{
  // first ladder S and K side
  m_indexMap[ArrayPair(0,0)]  = 0;
  m_indexMap[ArrayPair(0,1)]  = 0;
  // second ladder S and K side
  m_indexMap[ArrayPair(1,0)]  = 49;
  m_indexMap[ArrayPair(1,1)]  = 49;
  // wide VA module
  m_indexMap[ArrayPair(30,0)] = 1;
  m_indexMap[ArrayPair(30,1)] = 2;
  m_indexMap[ArrayPair(30,2)] = 3;
  m_indexMap[ArrayPair(30,3)] = 4;
  m_indexMap[ArrayPair(31,0)] = 5;
  m_indexMap[ArrayPair(31,1)] = 6;
  m_indexMap[ArrayPair(31,2)] = 7;
  m_indexMap[ArrayPair(31,3)] = 8;
  m_indexMap[ArrayPair(32,0)] = 9;
  m_indexMap[ArrayPair(32,1)] = 10;
  m_indexMap[ArrayPair(32,2)] = 11;
  m_indexMap[ArrayPair(32,3)] = 12;
  m_indexMap[ArrayPair(33,0)] = 13;
  m_indexMap[ArrayPair(33,1)] = 14;
  m_indexMap[ArrayPair(33,2)] = 15;
  m_indexMap[ArrayPair(33,3)] = 16;
  // first thin VA module
  m_indexMap[ArrayPair(34,0)] = 17;
  m_indexMap[ArrayPair(34,1)] = 18;
  m_indexMap[ArrayPair(34,2)] = 19;
  m_indexMap[ArrayPair(34,3)] = 20;
  m_indexMap[ArrayPair(35,0)] = 21;
  m_indexMap[ArrayPair(35,1)] = 22;
  m_indexMap[ArrayPair(35,2)] = 23;
  m_indexMap[ArrayPair(35,3)] = 24;
  // second thin VA module
  m_indexMap[ArrayPair(36,0)] = 25;
  m_indexMap[ArrayPair(36,1)] = 26;
  m_indexMap[ArrayPair(36,2)] = 27;
  m_indexMap[ArrayPair(36,3)] = 28;
  m_indexMap[ArrayPair(37,0)] = 29;
  m_indexMap[ArrayPair(37,1)] = 30;
  m_indexMap[ArrayPair(37,2)] = 31;
  m_indexMap[ArrayPair(37,3)] = 32;
  // wide Spiroc module
  m_indexMap[ArrayPair(20,0)] = 33;
  m_indexMap[ArrayPair(20,1)] = 34;
  m_indexMap[ArrayPair(20,2)] = 35;
  m_indexMap[ArrayPair(20,3)] = 36;
  m_indexMap[ArrayPair(21,0)] = 37;
  m_indexMap[ArrayPair(21,1)] = 38;
  m_indexMap[ArrayPair(21,2)] = 39;
  m_indexMap[ArrayPair(21,3)] = 40;
  m_indexMap[ArrayPair(22,0)] = 41;
  m_indexMap[ArrayPair(22,1)] = 42;
  m_indexMap[ArrayPair(22,2)] = 43;
  m_indexMap[ArrayPair(22,3)] = 44;
  m_indexMap[ArrayPair(23,0)] = 45;
  m_indexMap[ArrayPair(23,1)] = 46;
  m_indexMap[ArrayPair(23,2)] = 47;
  m_indexMap[ArrayPair(23,3)] = 48;
}

unsigned int Parameters::ShiftParameterIndexForItem(unsigned int hybrid, unsigned int sipm)
{
  return m_indexMap[ArrayPair(hybrid,sipm)];
}

unsigned int Parameters::RotationParameterIndexForItem(unsigned int uniqueLayer, unsigned int /*hybrid*/, unsigned int /*sipm*/)
{
  int firstIndex = m_nPar - m_nAngles;
  return firstIndex + uniqueLayer;
  //  return firstIndex + ShiftParameterIndexForItem(uniqueLayer, hybrid);
}
