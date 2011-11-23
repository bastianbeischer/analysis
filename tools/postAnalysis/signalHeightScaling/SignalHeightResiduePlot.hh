#ifndef SignalHeightResiduePlot_hh
#define SignalHeightResiduePlot_hh

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"
#include "Enums.hh"

#include <QObject>
#include <QMap>

class PostAnalysisCanvas;
class TFile;
class TH2D;

class SignalHeightResiduePlot : public QObject, public PostAnalysisPlot, public H1DPlot {
Q_OBJECT
public:
  enum Type {Time, Temperature};
  SignalHeightResiduePlot(Type type, TFile* file);
  virtual ~SignalHeightResiduePlot();
private:
  Type m_type;
  QMap<unsigned short, TH2D*> m_histograms;
  void updateHistogram(TH1D*, double referenceValue);
private slots:
  void updateLocation(Enums::Situation);
};

#endif
