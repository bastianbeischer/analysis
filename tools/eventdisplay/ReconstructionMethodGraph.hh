#ifndef ReconstructionMethodGraph_hh
#define ReconstructionMethodGraph_hh

#include "Enums.hh"

#include <QObject>
#include <QWidget>

class QComboBox;
class TQtWidget;
class TMultiGraph;
class TLegend;

class ReconstructionMethodGraph : public QWidget {
Q_OBJECT
public:
  ReconstructionMethodGraph(QWidget* parent = 0);
  ~ReconstructionMethodGraph();
  Enums::ReconstructionMethod method() const;
  void setMethod(Enums::ReconstructionMethod);
  void draw(TMultiGraph*, TLegend*);
signals:
  void methodChanged(Enums::ReconstructionMethod);
private slots:
  void methodComboBoxIndexChanged(const QString&);
private:
  TQtWidget* m_widget;
  QComboBox* m_methodComboBox;
  Enums::ReconstructionMethod m_method;
};

#endif
