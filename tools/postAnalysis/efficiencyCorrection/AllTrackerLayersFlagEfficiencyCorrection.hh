#ifndef AllTrackerLayersFlagEfficiencyCorrection_hh
#define AllTrackerLayersFlagEfficiencyCorrection_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QObject>
#include <QString>
#include <QVector>
#include <QMap>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TH1D.h>

class AllTrackerLayersFlagEfficiencyCorrection : public QObject, public PostAnalysisPlot, public H1DPlot {
Q_OBJECT
public:
  enum Type {Positive, Negative};
  AllTrackerLayersFlagEfficiencyCorrection(Type, PostAnalysisCanvas*);
  virtual ~AllTrackerLayersFlagEfficiencyCorrection();
private slots:
  void save();
private:
  Type m_type;
  QMap<Type, QString> m_typeNames;
};

#endif
