#ifndef TopicSelector_hh
#define TopicSelector_hh

#include "AnalysisTopic.hh"

#include <QWidget>

class QCheckBox;
class QPushButton;

class TopicSelector : public QWidget {
Q_OBJECT
public:
  TopicSelector(AnalysisTopic, QWidget* = 0);
  ~TopicSelector();
  AnalysisTopic topic() const;
  bool isActive() const;
  void setActive(bool);
  bool checked() const;
  void setChecked(bool);
signals:
  void show(AnalysisTopic);
  void hide(AnalysisTopic);
private slots:
  void toggleButton();
private:
  AnalysisTopic m_topic;
  QCheckBox* m_checkBox;
  QPushButton* m_button;
};

#endif
