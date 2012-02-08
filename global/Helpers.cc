#include "Helpers.hh"
#include "ParticleProperties.hh"
#include "Constants.hh"

#include "Constants.hh"

#include <cmath>

#include <QVector>
#include <QString>
#include <QStringList>

#include <TH1.h>
#include <TGraphAsymmErrors.h>

namespace Helpers
{
  double addQuad(double a, double b, double c, double d, double e)
  {
    return sqrt(a*a + b*b + c*c + d*d + e*e);
  }

  QVector<double> logBinning(unsigned int nBins, double min, double max)
  {
    QVector<double> binning;
    const double delta = (log(max) / log(min) - 1) / nBins;
    for (unsigned int i = 0; i <= nBins; ++i) {
      binning.append(pow(min, 1 + delta * i));
    }
    return binning;
  }

  QVector<double> linearBinning(unsigned int nBins, double min, double max)
  {
    QVector<double> binning;
    const double step = (max - min) / nBins;
    for (unsigned int i = 0; i <= nBins; ++i) {
      binning.append(min + i * step);
    }
    return binning;
  }

  void updateMirroredHistogram(TH1D* histogramToUpdate, TH1D* dataHistogram)
  {
    const int nBins = dataHistogram->GetNbinsX();
    for (int oldBin = 0; oldBin <= nBins+1; ++oldBin) {
      int newBin = nBins + 1 - oldBin;
      histogramToUpdate->SetBinContent(newBin, dataHistogram->GetBinContent(oldBin));
      histogramToUpdate->SetBinError(newBin, dataHistogram->GetBinError(oldBin));
    }
    histogramToUpdate->SetEntries(dataHistogram->GetEntries());
  }

  TH1D* createMirroredHistogram(TH1D* histogram)
  {
    QString title = histogram->GetTitle();
    QString name = QString(histogram->GetName()) + "_mirrored";
    QVector<double> binning;
    int nBins = histogram->GetNbinsX();
    for (int i = 0; i <= nBins; ++i) {
      double lowEdge = histogram->GetBinLowEdge(i+1);
      lowEdge *= -1;
      binning.prepend(lowEdge);
    }
    TH1D* mirroredHistogram = new TH1D(qPrintable(title), qPrintable(name), nBins, binning.constData());
    mirroredHistogram->Sumw2();
    mirroredHistogram->SetMarkerColor(histogram->GetMarkerColor());
    mirroredHistogram->SetMarkerSize(histogram->GetMarkerSize());
    mirroredHistogram->SetMarkerStyle(histogram->GetMarkerStyle());
    mirroredHistogram->SetLineColor(histogram->GetLineColor());
    mirroredHistogram->SetLineWidth(histogram->GetLineWidth());
    mirroredHistogram->SetLineStyle(histogram->GetLineStyle());
    mirroredHistogram->SetXTitle(qPrintable(QString("- ")+histogram->GetXaxis()->GetTitle()));
    updateMirroredHistogram(mirroredHistogram, histogram);

    return mirroredHistogram;
  }

  QString analysisPath()
  {
    const char* env = getenv("PERDAIXANA_PATH");
    if (!env)
      qFatal("ERROR: You need to set PERDAIXANA_PATH environment variable to the toplevel location!");
    return env;
  }

  QString dataPath()
  {
    const char* env = getenv("PERDAIXDATA_PATH");
    if (!env)
      qFatal("ERROR: You need to set PERDAIXDATA_PATH environment variable to the toplevel location of the data!");
    return env;
  }

  double trackerResolution(double curvature, Enums::Particle particle)
  {
    ParticleProperties properties(particle);
    Q_ASSERT(properties.charge() != 0 && properties.mass() > 0);
    double a = 0, b = 0;
    switch (particle) {
      case Enums::Proton: case Enums::AntiProton: a = 0.077; b = 0.3175; break;
      case Enums::Electron: case Enums::Positron: a = 0.07627; b = 0.2349; break;
      case Enums::Helium: a = 0.04195; b = 0.3024; break;
      case Enums::PiPlus: case Enums::PiMinus: a = 0.0759; b = 0.2356; break;
      default: a = 0.077; b = 0.3175; break;
    }
    double inverseBeta = addQuad(1, properties.mass() * curvature);
    return addQuad(a / curvature, b * inverseBeta);
  }

  double tofResolution()
  {
    double l = Constants::upperTofPosition - Constants::lowerTofPosition;
    return Constants::tofResolution * Constants::speedOfLight / l;
  }

  void scaleToBinWidth(TH1D* histogram)
  {
    if (!histogram->GetSumw2())
      histogram->Sumw2();
    histogram->Scale(1., "width");
  }

  TGraphAsymmErrors* createBeltWithSystematicUncertainty(const TH1D* histogram,const TH1D* lowerHistogram,const TH1D* upperHistogram)
  {
    QList<double> x;
    QList<double> exl;
    QList<double> exu;
    QList<double> y;
    QList<double> eyl;
    QList<double> eyu;
    for(int i = 1; i <= histogram->GetNbinsX(); i++) {
      x.push_back(histogram->GetBinCenter(i));
      exl.push_back(histogram->GetBinCenter(i) - histogram->GetBinLowEdge(i));
      exu.push_back( (histogram->GetBinLowEdge(i) + histogram->GetBinWidth(i) ) - histogram->GetBinCenter (i));
      QVector<double> point;
      point << lowerHistogram->GetBinContent(i) << histogram->GetBinContent(i) <<upperHistogram->GetBinContent(i);
      qSort(point);
      const double min = point[0];
      const double middle = point[1];
      const double max = point[2];
      y.push_back(middle);
      eyl.push_back(middle - min);
      eyu.push_back(max - middle);
    }
    TGraphAsymmErrors* graph = new TGraphAsymmErrors(x.size(), &(x.toVector().toStdVector()[0]), &(y.toVector().toStdVector()[0]),
      &(exl.toVector().toStdVector()[0]), &(exu.toVector().toStdVector()[0]), &(eyl.toVector().toStdVector()[0]), &(eyu.toVector().toStdVector()[0]));
    QString name = QString(histogram->GetName()) + "_graph";
    graph->SetName(name.toStdString().c_str());
    return graph;
  }

  QVector<double> variantToDoubleVector(const QVariant& variant)
  {
    QStringList list = variant.toStringList();
    QStringList::ConstIterator it = list.begin();
    QStringList::ConstIterator end = list.end();
    QVector<double> vector(list.count());
    int i = 0;
    while (it != end) {
      vector[i] = it->toDouble();
      ++i;
      ++it;
    }
    return vector;
  }

  QVariant doubleVectorToVariant(const QVector<double>& vector)
  {
    QStringList list;
    foreach (double value, vector)
      list.append(QString::number(value));
    return list;
  }

  double min(const QVector<double>& vector)
  {
    Q_ASSERT(vector.count());
    double minValue = DBL_MAX;
    foreach(double value, vector)
      if (value < minValue)
        minValue = value;
    return minValue;
  }

  double max(const QVector<double>& vector)
  {
    Q_ASSERT(vector.count());
    double maxValue = -DBL_MAX;
    foreach(double value, vector)
      if (value > maxValue)
        maxValue = value;
    return maxValue;
  }

  bool sorted(const QVector<double>& vector)
  {
    if (vector.count() < 2)
      return true;
    const QVector<double>::ConstIterator end = vector.end() - 1;
    for (QVector<double>::ConstIterator it = vector.begin(); it != end; ++it)
      if (*(it+1) < *it)
        return false;
    return true;
  }

  QString greekifyLetters(QString string)
  {
    string.replace("alpha", "#alpha");
    string.replace("mu", "#mu");
    string.replace("pi", "#pi");
    return string;
  }

  QVector<double> logLikelihoodBinning()
  {
    return linearBinning(1000, 0, 20);
  }
}
