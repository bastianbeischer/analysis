#ifndef MOPVCONSTANT_HH
#define MOPVCONSTANT_HH

#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QObject>
#include <QList>
#include <QMap>
#include <QPair>

class PostAnalysisCanvas;
class TH1D;

class QSpinBox;
class QPushButton;

class MoPVConstant: public QObject, public PostAnalysisPlot, public H1DPlot
{
  Q_OBJECT
public:
  MoPVConstant(QList<PostAnalysisCanvas*>, bool modules);
private:
  bool fitMoPV(TH1D* hist, double& mopv, double& mopvErr);
  void saveConstants(double norm);
  QMap<int, QPair<double, double> > m_mopvs;

  QSpinBox* m_spinBox;
  QPushButton* m_pushButton;

  bool m_modules;
public slots:
  void saveConstants();
};

#endif // MOPVCONSTANT_HH
