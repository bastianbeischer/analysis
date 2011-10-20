#ifndef BeamProfilePlot_hh
#define BeamProfilePlot_hh

#include "H2DProjectionPlot.hh"
#include "AnalysisPlot.hh"

class QLineEdit;

class BeamProfilePlot :
  public H2DProjectionPlot,
  public AnalysisPlot
{
  
Q_OBJECT

public:
  enum Type {Horizontal, Vertical};

public:
  BeamProfilePlot(Type);
  ~BeamProfilePlot();
  
  void processEvent(const AnalyzedEvent*);

private slots:
  void saveHistograms();

private:
  Type m_type;
  QLineEdit* m_fileNameLineEdit;

};

#endif /* BeamProfilePlot_hh */
