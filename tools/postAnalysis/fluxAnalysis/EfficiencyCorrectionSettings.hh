#ifndef EfficiencyCorrectionSettings_hh
#define EfficiencyCorrectionSettings_hh

#include <QString>
#include <QSettings>
#include <QMap>

class TH1D;

class EfficiencyCorrectionSettings {
public:
  enum FoldingType {Undefined, Raw, Unfolded};
  static EfficiencyCorrectionSettings* instance();
  ~EfficiencyCorrectionSettings();
  void saveTrackFindingEfficiency(FoldingType, TH1D*);
  void saveAllTrackerLayerCutEfficiency(FoldingType, TH1D*);
  TH1D* trackFindingEfficiency(FoldingType);
  TH1D* allTrackerLayerCutEfficiency(FoldingType);
  QString foldingTypeName(FoldingType) const;
  QString foldingTypeName(int nBins) const;
  FoldingType foldingType(int nBins) const;
  static void efficiencyCorrection(TH1D* histogramToCorrect, double efficiency);
  static void efficiencyCorrection(TH1D* histogramToCorrect, TH1D* efficiencyHistogram);
  static int numberOfBins(FoldingType);
private:
  EfficiencyCorrectionSettings();
  TH1D* readHistogram(const QString& key);
  void save(const QString& key, TH1D*);
  static EfficiencyCorrectionSettings* s_instance;
  const static int s_nBinsUnfolded = 31;
  const static int s_nBinsRaw = 4 * s_nBinsUnfolded;
  QMap<FoldingType, TH1D*> m_trackFindingEfficiency;
  QMap<FoldingType, TH1D*> m_allTrackerLayerCutEfficiency;
  QMap<FoldingType, int> m_binQuantities;
  QSettings m_settings;
  QString m_trackFindingKey;
  QString m_allTrackerLayerCutKey;
  QString m_axisKey;
  QString m_contentKey;
  QString m_errorKey;
};

#endif
