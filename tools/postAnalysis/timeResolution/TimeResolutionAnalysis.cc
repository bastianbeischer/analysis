#include "TimeResolutionAnalysis.hh"
#include "TimeOfFlightHistogram.hh"
#include "Constants.hh"

#include <TGraph.h>
#include <TMatrix.h>
#include <TAxis.h>

#include <QMap>
#include <QDebug>

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>

TimeResolutionAnalysis::Key::Key(int vi, int vj, int vk, int vl)
  : i(vi)
  , j(vj)
  , k(vk)
  , l(vl)
{
}

TimeResolutionAnalysis::Key::Key(const TimeOfFlightHistogram* const h)
  : i(h->i())
  , j(h->j())
  , k(h->k())
  , l(h->l())
{
}

bool TimeResolutionAnalysis::Key::operator<(const Key& other) const
{
  if (i < other.i) return true;
  if (i == other.i && j < other.j) return true;
  if (i == other.i && j == other.j && k < other.k) return true;
  if (i == other.i && j == other.j && k == other.k && l < other.l) return true;
  return false;
}

TimeResolutionAnalysis::Value::Value(double variance, double varianceError, double yUp, double yLow)
  : v(variance)
  , vError(varianceError)
  , upperY(yUp)
  , lowerY(yLow)
{
}

TimeResolutionAnalysis::TimeResolutionAnalysis(const QVector<TimeOfFlightHistogram*>& histograms, int nBins)
  : m_nBins(nBins)
  , m_y()
  , m_variance()
{
  Q_ASSERT(histograms.count());
  QMap<Key, Value> map;
  foreach (TimeOfFlightHistogram* histogram, histograms) {
    Value value = Value(histogram->v(), histogram->vError(),
      histogram->upperY(), histogram->lowerY());
    map.insert(Key(histogram), value);
    m_y.insert(histogram->k(), histogram->upperY());
  }

  TMatrixT<double> a(8*nBins, 8*nBins);

  for (int row = 0; row < 8*nBins; ++row) {
    for (int column = 0; column < 8*nBins; ++column) {
      a(row, column) = 0;
    }
  }

  for (int i = 0; i < 4; ++i) {
    for (int k = 0; k < nBins; ++k) {
      int d = i * nBins + k;
      a(d, d) = sumSigmaErrorJL(map, i, k);
    }
  }

  for (int j = 0; j < 4; ++j) {
    for (int l = 0; l < nBins; ++l) {
      int d = 4 * nBins + j * nBins + l;
      a(d, d) = sumSigmaErrorIK(map, j, l);
    }
  }

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      for (int k = 0; k < nBins; ++k) {
        for (int l = 0; l < nBins; ++l) {
          const Value& value = map.find(Key(i, j, k, l)).value();
          int ai = 0;
          int aj = 0;
          ai = i * nBins + k;
          aj = 4 * nBins + j * nBins + l;
          a(ai, aj) = 1. / (value.vError * value.vError);
          ai = 4 * nBins + j * nBins + l;
          aj = i * nBins + k;
          a(ai, aj) = 1. / (value.vError * value.vError);
        }
      }
    }
  }
  TMatrixT<double> aPrime = a.GetSub(0, 8*nBins-2, 0, 8*nBins-2);
  TMatrixT<double> aPrimeInverted = a.GetSub(0, 8*nBins-2, 0, 8*nBins-2).Invert();

  TMatrixT<double> b(8*nBins, 1);
  for (int ai = 0; ai < 8*nBins; ++ai)
    b(ai, 0) = 0;
  for (int i = 0; i < 4; ++i) {
    for (int k = 0; k < nBins; ++k) {
      b(i * nBins + k, 0) = sumRelativeSigmaErrorJL(map, i, k);
    }
  }
  for (int j = 0; j < 4; ++j) {
    for (int l = 0; l < nBins; ++l) {
      b(4 * nBins + j * nBins + l, 0) = sumRelativeSigmaErrorIK(map, j, l);
    }
  }
  TMatrixT<double> bPrime(8*nBins-1, 1);
  double r = pow(0.467, 2.);
  for (int ai = 0; ai < 8*nBins-1; ++ai)
    bPrime(ai, 0) = b(ai, 0) - r * a(ai, 8*nBins-1);

  TMatrixT<double> v = aPrimeInverted * bPrime;
  for (int ai = 0; ai < 8*nBins-1; ++ai)
    m_variance.append(v(ai, 0));
  m_variance.append(r);
}

TimeResolutionAnalysis::~TimeResolutionAnalysis()
{}

double TimeResolutionAnalysis::sumSigmaErrorJL(const QMap<Key, Value>& map, int i, int k)
{
  double sum = 0;
  for (int j = 0; j < 4; ++j) {
    for (int l = 0; l < m_nBins; ++l) {
      const Value& value = map.find(Key(i, j, k, l)).value();
      sum+= 1. / (value.vError * value.vError);
    }
  }
  return sum;
}

double TimeResolutionAnalysis::sumSigmaErrorIK(const QMap<Key, Value>& map, int j, int l)
{
  double sum = 0;
  for (int i = 0; i < 4; ++i) {
    for (int k = 0; k < m_nBins; ++k) {
      const Value& value = map.find(Key(i, j, k, l)).value();
      sum+= 1. / (value.vError * value.vError);
    }
  }
  return sum;
}

double TimeResolutionAnalysis::sumRelativeSigmaErrorJL(const QMap<Key, Value>& map, int i, int k)
{
  double sum = 0;
  for (int j = 0; j < 4; ++j) {
    for (int l = 0; l < m_nBins; ++l) {
      const Value& value = map.find(Key(i, j, k, l)).value();
      sum+= value.v / (value.vError * value.vError);
    }
  }
  return sum;
}

double TimeResolutionAnalysis::sumRelativeSigmaErrorIK(const QMap<Key, Value>& map, int j, int l) {
  double sum = 0;
  for (int i = 0; i < 4; ++i) {
    for (int k = 0; k < m_nBins; ++k) {
      const Value& value = map.find(Key(i, j, k, l)).value();
      sum+= value.v / (value.vError * value.vError);
    }
  }
  return sum;
}

void TimeResolutionAnalysis::dumpMatrix(const TMatrixT<double>& m)
{
  int precision = 2;
  int space = 1;
  std::cout << " _ ";
  for (int i = 0; i < (precision + 6 + space) * m.GetNcols() - space + 1; ++i) std::cout << ' ';
  std::cout << "_ " << std::endl;
  for (int r = 0; r < m.GetNrows(); ++r) {
    for (int c = 0; c < m.GetNcols(); ++c) {
      if (c) {
        for (int i = 0; i < space; ++i) std::cout << ' ';
      } else {
        std::cout << (r == m.GetNrows() - 1 ? "|_ " : "|  ");
      }
      std::cout << std::setfill('x') << std::scientific <<std::setprecision(precision) << m[r][c];
    }
    std::cout << (r == m.GetNrows() - 1 ? " _|" : "  |") << std::endl;
  }
}
  
int TimeResolutionAnalysis::nBins() const
{
  return m_nBins;
}

double TimeResolutionAnalysis::y(int kl) const
{
  return m_y[kl];
}

double TimeResolutionAnalysis::vIK(int i, int k) const
{
  return m_variance[i * m_nBins + k];
}

double TimeResolutionAnalysis::vJL(int j, int l) const
{
  return m_variance[(j + 4) * m_nBins + l];
}

double TimeResolutionAnalysis::sigmaIK(int i, int k) const
{
  return sqrt(vIK(i, k));
}

double TimeResolutionAnalysis::sigmaJL(int j, int l) const
{
  return sqrt(vJL(j, l));
}
