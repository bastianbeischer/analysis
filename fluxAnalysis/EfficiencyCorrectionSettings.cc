#include "EfficiencyCorrectionSettings.hh"
#include "Helpers.hh"

#include <TH1.h>

#include <QString>
#include <QVariant>
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
  : m_trackFindingEfficiency(0)
  , m_allTrackerLayerCutEfficiency(0)
  , m_settings(Helpers::analysisPath() + "/conf/EfficiencyCorrections.conf", QSettings::IniFormat)
  , m_trackFindingKey("trackFindingEfficiency")
  , m_allTrackerLayerCutKey("oneHitAllLayersEfficiency")
  , m_axisKey("/axis")
  , m_contentKey("/content")
  , m_errorKey("/error")
{
}

EfficiencyCorrectionSettings::~EfficiencyCorrectionSettings()
{
  delete m_allTrackerLayerCutEfficiency;
  delete m_trackFindingEfficiency;
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

void EfficiencyCorrectionSettings::saveTrackFindingEfficiency(TH1D* histogram)
{
  save(m_allTrackerLayerCutKey, histogram);
}

void EfficiencyCorrectionSettings::saveAllTrackerLayerCutEfficiency(TH1D* histogram)
{
  save(m_trackFindingKey, histogram);
}

TH1D* EfficiencyCorrectionSettings::trackFindingEfficiency()
{
  if (!m_trackFindingEfficiency)
    m_trackFindingEfficiency = readHistogram(m_trackFindingKey);
  return m_trackFindingEfficiency;
}

TH1D* EfficiencyCorrectionSettings::allTrackerLayerCutEfficiency()
{
  if (!m_allTrackerLayerCutEfficiency)
    m_allTrackerLayerCutEfficiency = readHistogram(m_allTrackerLayerCutKey);
  return m_allTrackerLayerCutEfficiency;
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
