#ifndef TrackFindingEfficiencyCorrection_hh
#define TrackFindingEfficiencyCorrection_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QString>
#include <QVector>
#include <QMap>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TH1D.h>

class TrackFindingEfficiencyCorrection : public PostAnalysisPlot, public H1DPlot {
public:
  enum Type {
    Positive,
    Negative
  };
  TrackFindingEfficiencyCorrection(Type, PostAnalysisCanvas*);
  virtual ~TrackFindingEfficiencyCorrection();

private:
  Type m_type;
  QMap<Type, QString> m_typeNames;

  void saveAsSetting();
};

#endif
