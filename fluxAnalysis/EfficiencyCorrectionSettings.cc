#include "EfficiencyCorrectionSettings.hh"

#include <TH1D.h>

#include <QString>
#include <QVariant>
#include <QList>
#include <QVector>

#include <iostream>
#include <iomanip>

QString EfficiencyCorrectionSettings::s_trackFindingEfficiencyPreKey = "oneHitAllLayersEfficiency_";
QString EfficiencyCorrectionSettings::s_allTrackerLayerCutEfficiencyPreKey = "trackFindingEfficiency_";

EfficiencyCorrectionSettings::EfficiencyCorrectionSettings()
{
  const char* env = getenv("PERDAIXANA_PATH");
  if (env == 0) {
    qFatal("ERROR: You need to set PERDAIXANA_PATH environment variable to the toplevel location!");
  }
  QString path(env);
  path += "/conf/";
  m_settings = new QSettings(path + "EfficiencyCorrections.conf", QSettings::IniFormat);

  m_axisKey = "/axis";
  m_contentKey = "/content";
  m_errorKey = "/error";
}

EfficiencyCorrectionSettings::~EfficiencyCorrectionSettings()
{
  delete m_settings;
  m_settings = 0;
}

void EfficiencyCorrectionSettings::save(const QString& key, TH1D* histogram)
{
  QList<QVariant> axis;
  for (int i = 0; i <= histogram->GetNbinsX(); ++i) {
    axis.push_back( histogram->GetBinLowEdge(i+1) );
  }

  QList<QVariant> content;
  QList<QVariant> error;
  for (int i = 0; i < histogram->GetNbinsX(); ++i) {
    double binContent = histogram->GetBinContent(i+1);
//    if (binContent == 0)
//      binContent = 1;
    double binError = histogram->GetBinError(i+1);
    content.push_back(binContent);
    error.push_back(binError);
  }

  const QString& prefix = key;
  m_settings->setValue(prefix+m_axisKey, axis);
  m_settings->setValue(prefix+m_contentKey, content);
  m_settings->setValue(prefix+m_errorKey, error);
  m_settings->sync();
}

TH1D* EfficiencyCorrectionSettings::readHistogram(const QString& key)
{
  const QString& prefix = key;
  QList<QVariant> axisVariant = m_settings->value(prefix+m_axisKey).toList();
  QList<QVariant> contentVariant = m_settings->value(prefix+m_contentKey).toList();
  QList<QVariant> errorVariant = m_settings->value(prefix+m_errorKey).toList();

  QVector<double> axis;
  for (int i = 0; i < axisVariant.size(); ++i) {
    axis.push_back(axisVariant[i].toDouble());
  }

  const int nBins = axis.size()-1;

  TH1D* histogram = new TH1D(qPrintable(key), "", nBins, axis.constData());
  histogram->Sumw2();

  for (int i = 0; i < nBins; ++i) {
    double content = contentVariant[i].toDouble();
    double error = errorVariant[i].toDouble();
    int bin = i+1;
    histogram->SetBinContent(bin, content);
    histogram->SetBinError(bin, error);
  }
  return histogram;
}
