#ifndef TrackFindingEfficiencyCorrection_hh
#define TrackFindingEfficiencyCorrection_hh

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "EfficiencyCorrectionSettings.hh"
#include "H1DPlot.hh"

#include <QObject>
#include <QString>
#include <QMap>

class TrackFindingEfficiencyCorrection : public QObject, public PostAnalysisPlot, public H1DPlot {
Q_OBJECT
public:
  enum Type {Positive, Negative};
  TrackFindingEfficiencyCorrection(Type, PostAnalysisCanvas*);
  virtual ~TrackFindingEfficiencyCorrection();
private slots:
  void save();
private:
  Type m_type;
  QMap<Type, QString> m_typeNames;
  EfficiencyCorrectionSettings::BinQuantity m_quantity;
};

#endif
