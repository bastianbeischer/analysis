#ifndef CutSelector_hh
#define CutSelector_hh

#include "AnalysisTopic.hh"
#include "Cut.hh"

#include <QWidget>

class QCheckBox;
class QLineEdit;

class CutSelector : public QWidget {
Q_OBJECT
public:
  CutSelector(Enums::Cut, QWidget* = 0);
  ~CutSelector();
  Cut cut() const;
  void setMinimum(double);
  double minimum(bool* ok = 0) const;
  void setMaximum(double);
  double maximum(bool* ok = 0) const;
  void setChecked(bool);
  bool checked() const;
private:
  Enums::Cut m_cut;
  QCheckBox* m_checkBox;
  QLineEdit* m_minimumEdit;
  QLineEdit* m_maximumEdit;
};

#endif
