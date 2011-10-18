#ifndef LHFIT_HH
#define LHFIT_HH

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QObject>
#include <QList>

class PostAnalysisCanvas;
class TH1D;
class TF1;
class TRDLikelihoods;
class QPushButton;

class LHFit: public QObject, public PostAnalysisPlot, public H1DPlot
{
  Q_OBJECT
public:
  LHFit(PostAnalysisCanvas* canvases, int layer = -1);

private:
  void fit();

private:
  TF1* m_fit;
  TRDLikelihoods* m_trdLHs;
  QPushButton* m_buttonSave;
  int m_layer;

private slots:
  void fitTRFunction();
  void fitNonTRFunction();
  void updateSaveButton();
  void saveFit();
};

#endif // LHFIT_HH
