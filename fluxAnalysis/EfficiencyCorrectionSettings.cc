#include "EfficiencyCorrectionSettings.hh"

#include "Constants.hh"
#include "Helpers.hh"

#include <TH1.h>

#include <QString>
#include <QVariant>
#include <QDebug>
#include <QList>
#include <QVector>

EfficiencyCorrectionSettings* EfficiencyCorrectionSettings::s_instance = 0;

EfficiencyCorrectionSettings* EfficiencyCorrectionSettings::instance()
{
  if (!s_instance)
    s_instance = new EfficiencyCorrectionSettings();
  return s_instance;
}

EfficiencyCorrectionSettings::EfficiencyCorrectionSettings()
  : m_settings(Helpers::analysisPath() + "/conf/EfficiencyCorrections.conf", QSettings::IniFormat)
  , m_trackFindingKey("trackFindingEfficiency")
  , m_allTrackerLayerCutKey("oneHitAllLayersEfficiency")
  , m_axisKey("/axis")
  , m_contentKey("/content")
  , m_errorKey("/error")
{
  m_trackFindingEfficiency.insert(Raw, 0);
  m_trackFindingEfficiency.insert(Unfolded, 0);
  m_allTrackerLayerCutEfficiency.insert(Raw, 0);
  m_allTrackerLayerCutEfficiency.insert(Unfolded, 0);
}

EfficiencyCorrectionSettings::~EfficiencyCorrectionSettings()
{
  qDeleteAll(m_allTrackerLayerCutEfficiency);
  qDeleteAll(m_trackFindingEfficiency);
  s_instance = 0;
}

void EfficiencyCorrectionSettings::save(const QString& key, TH1D* histogram)
{
  QList<QVariant> axis;
  for (int i = 0; i <= histogram->GetNbinsX(); ++i) {
    axis.push_back( histogram->GetBinLowEdge(i+1) );
  }

  QList<QVariant> content;
  QList<QVariant> error;
  for (int bin = 1; bin <= histogram->GetNbinsX(); ++bin) {
    double binContent = histogram->GetBinContent(bin);
    double binError = histogram->GetBinError(bin);
    content.push_back(binContent);
    error.push_back(binError);
  }

  m_settings.setValue(key + m_axisKey, axis);
  m_settings.setValue(key + m_contentKey, content);
  m_settings.setValue(key + m_errorKey, error);
  m_settings.sync();
}

void EfficiencyCorrectionSettings::saveTrackFindingEfficiency(FoldingType type, TH1D* histogram)
{
  save(m_allTrackerLayerCutKey + "_" + foldingTypeName(type), histogram);
}

void EfficiencyCorrectionSettings::saveAllTrackerLayerCutEfficiency(FoldingType type, TH1D* histogram)
{
  save(m_trackFindingKey + "_" + foldingTypeName(type), histogram);
}

TH1D* EfficiencyCorrectionSettings::trackFindingEfficiency(FoldingType type)
{
  if (!m_trackFindingEfficiency[type])
    m_trackFindingEfficiency[type] = readHistogram(m_trackFindingKey + "_" + foldingTypeName(type));
  return m_trackFindingEfficiency[type];
}

TH1D* EfficiencyCorrectionSettings::allTrackerLayerCutEfficiency(FoldingType type)
{
  if (!m_allTrackerLayerCutEfficiency[type])
    m_allTrackerLayerCutEfficiency[type] = readHistogram(m_allTrackerLayerCutKey + "_" + foldingTypeName(type));
  return m_allTrackerLayerCutEfficiency[type];
}

QString EfficiencyCorrectionSettings::foldingTypeName(FoldingType type) const
{
  return QString("%1_bins").arg(numberOfBins(type));
}

TH1D* EfficiencyCorrectionSettings::readHistogram(const QString& key)
{
  const QList<QVariant>& axisVariant = m_settings.value(key + m_axisKey).toList();
  const QList<QVariant>& contentVariant = m_settings.value(key + m_contentKey).toList();
  const QList<QVariant>& errorVariant = m_settings.value(key + m_errorKey).toList();

  QVector<double> axis;
  for (int i = 0; i < axisVariant.size(); ++i)
    axis.push_back(axisVariant[i].toDouble());

  const int nBins = axis.size() - 1;

  TH1D* histogram = new TH1D(qPrintable(key), "", nBins, axis.constData());
  histogram->Sumw2();

  for (int i = 0; i < nBins; ++i) {
    double content = contentVariant[i].toDouble();
    double error = errorVariant[i].toDouble();
    int bin = i + 1;
    histogram->SetBinContent(bin, content);
    histogram->SetBinError(bin, error);
  }
  return histogram;
}

void EfficiencyCorrectionSettings::efficiencyCorrection(TH1D* histogramToCorrect, double efficiency)
{
  if (!histogramToCorrect->GetSumw2())
    histogramToCorrect->Sumw2();
  histogramToCorrect->Scale(1. / efficiency);
}

void EfficiencyCorrectionSettings::efficiencyCorrection(TH1D* histogramToCorrect, TH1D* efficiencyHistogram)
{
  if (!histogramToCorrect->GetSumw2())
    histogramToCorrect->Sumw2();
  histogramToCorrect->Divide(efficiencyHistogram);
}

int EfficiencyCorrectionSettings::numberOfBins(FoldingType type)
{
  switch (type) {
    case Raw: return s_nBinsRaw;
    case Unfolded: return s_nBinsUnfolded;
    default : return 0;
  }
}

EfficiencyCorrectionSettings::FoldingType EfficiencyCorrectionSettings::foldingType(int nBins) const
{
  if (nBins == s_nBinsRaw) {
    return Raw;
  } else if (nBins == s_nBinsUnfolded) {
    return Unfolded;
  }
  return Undefined;
}

QString EfficiencyCorrectionSettings::foldingTypeName(int nBins) const
{
  return foldingTypeName(foldingType(nBins));
}
