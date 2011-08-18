#ifndef TrackFindingEfficiencyCorrection_hh
#define TrackFindingEfficiencyCorrection_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QString>
#include <QMap>

class TrackFindingEfficiencyCorrection : public PostAnalysisPlot, public H1DPlot {
public:
  enum Type {Positive, Negative};

  TrackFindingEfficiencyCorrection(Type, PostAnalysisCanvas*);
  virtual ~TrackFindingEfficiencyCorrection();
private:
  void saveAsSetting();

  Type m_type;
  QMap<Type, QString> m_typeNames;
};

#endif
