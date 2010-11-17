#include "StraightLineMatrix.hh"

#include "Hit.hh"
#include <QDebug>
#include <cmath>

StraightLineMatrix::StraightLineMatrix() :
  Matrix()
{
  m_nCol = 4;
  m_solution.ResizeTo(m_nCol);
}

StraightLineMatrix::~StraightLineMatrix()
{
}

bool StraightLineMatrix::checkInvertability(const QVector<Hit*>& hits) const
{
  bool ret = Matrix::checkInvertability(hits);
  if(!ret)
    return ret;

  typedef QMap<double,int> ZMap;
  QMap<double,ZMap> angles;
  foreach(Hit* hit, hits) {
    double angle = round(hit->angle()*180./M_PI * 10.)/10.;
    angles[angle][hit->position().z()]++;
  }

  //qDebug() << angles;
  
  if (angles.size() < 2) {
    //qDebug() << "1: return false";
    return false;
  }
  if (angles.size() == 2) {
    foreach(ZMap zMap, angles) {
      if (zMap.size() < 2) {
        return false;
        //qDebug() << "2: return false";
      }
    }
  }

  return true;
}

void StraightLineMatrix::fillMatrixFromHit(TMatrixD& A, unsigned int i, bool useTangens, float k, float xi) const
{
  if (useTangens) {
    A(i,0)            = -xi;
    A(i,1)            = 1.;
    A(i,2)            = -k*xi;
    A(i,3)            = k;
  }
  else {
    A(i,0)            = 1.;
    A(i,1)            = -xi;
    A(i,2)            = k;
    A(i,3)            = -k*xi;
  }
}
