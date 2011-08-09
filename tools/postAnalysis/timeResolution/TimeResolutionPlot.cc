#include "TimeResolutionPlot.hh"
#include "TimeOfFlightHistogram.hh"
#include "Constants.hh"

#include <TGraph.h>
#include <TMatrix.h>

#include <QMap>

#include <iostream>
#include <iomanip>

TimeResolutionPlot::Key::Key(int vi, int vj, int vk, int vl)
  : i(vi)
  , j(vj)
  , k(vk)
  , l(vl)
{
}

TimeResolutionPlot::Key::Key(const TimeOfFlightHistogram* const h)
  : i(h->i())
  , j(h->j())
  , k(h->k())
  , l(h->l())
{
}

bool TimeResolutionPlot::Key::operator<(const Key& other) const
{
  if (i < other.i) return true;
  if (i == other.i && j < other.j) return true;
  if (i == other.i && j == other.j && k < other.k) return true;
  if (i == other.i && j == other.j && k == other.k && l < other.l) return true;
  return false;
}

TimeResolutionPlot::Value::Value(double s, double se)
  : sigma(s)
  , sigmaError(se)
{
}

TimeResolutionPlot::TimeResolutionPlot(const QVector<TimeOfFlightHistogram*>& histograms, int nBins)
  : PostAnalysisPlot()
  , GraphPlot()
  , m_nBins(nBins)
{
  setTitle("time resolution plot");
  Q_ASSERT(histograms.count());
  QMap<Key, Value> map;
  foreach (TimeOfFlightHistogram* histogram, histograms)
    map.insert(Key(histogram), Value(histogram->sigma(), histogram->sigmaError()));

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
          a(ai, aj) = value.sigma * value.sigma / (value.sigmaError * value.sigmaError);
          ai = 4 * nBins + j * nBins + l;
          aj = i * nBins + k;
          a(ai, aj) = value.sigma * value.sigma / (value.sigmaError * value.sigmaError);
        }
      }
    }
  }

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
      b(4 * nBins + j * nBins + l, 0) = sumSigmaErrorIK(map, j, l);
    }
  }

  dumpMatrix(a.Invert() * b);
//  dumpMatrix(a.Invert());

  for (int bar = 0; bar < Constants::nTofBars / 2; ++bar) {
    TGraph* graph = new TGraph(nBins);
    for (int point = 0; point < nBins; ++point) {
      graph->SetPoint(point, point, point * point);
    }
    setDrawOption(ALP);
    addGraph(graph);
  }
}

TimeResolutionPlot::~TimeResolutionPlot()
{}

double TimeResolutionPlot::sumSigmaErrorJL(const QMap<Key, Value>& map, int i, int k)
{
  double sum = 0;
  for (int j = 0; j < 4; ++j) {
    for (int l = 0; l < m_nBins; ++l) {
      const Value& value = map.find(Key(i, j, k, l)).value();
      sum+= 1. / (value.sigmaError * value.sigmaError);
    }
  }
  return sum;
}

double TimeResolutionPlot::sumSigmaErrorIK(const QMap<Key, Value>& map, int j, int l)
{
  double sum = 0;
  for (int i = 0; i < 4; ++i) {
    for (int k = 0; k < m_nBins; ++k) {
      const Value& value = map.find(Key(i, j, k, l)).value();
      sum+= 1. / (value.sigmaError * value.sigmaError);
    }
  }
  return sum;
}

double TimeResolutionPlot::sumRelativeSigmaErrorJL(const QMap<Key, Value>& map, int i, int k)
{
  double sum = 0;
  for (int j = 0; j < 4; ++j) {
    for (int l = 0; l < m_nBins; ++l) {
      const Value& value = map.find(Key(i, j, k, l)).value();
      sum+= value.sigma * value.sigma / (value.sigmaError * value.sigmaError);
    }
  }
  return sum;
}

double TimeResolutionPlot::sumRelativeSigmaErrorIK(const QMap<Key, Value>& map, int j, int l) {
  double sum = 0;
  for (int i = 0; i < 4; ++i) {
    for (int k = 0; k < m_nBins; ++k) {
      const Value& value = map.find(Key(i, j, k, l)).value();
      sum+= value.sigma * value.sigma / (value.sigmaError * value.sigmaError);
    }
  }
  return sum;
}

void TimeResolutionPlot::dumpMatrix(const TMatrixT<double>& m)
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
