#ifndef MainWindow_hh
#define MainWindow_hh

#include "ui_MainWindow.h"

class TH2S;
class CalibrationCollection;
class DetectorElement;
class TH2S;
class TH1D;
class TLine;

class MainWindow : public QMainWindow {
Q_OBJECT
public:
  enum CalibrationType {NoCalibrationType, Pedestal, Led, LedShifted};
  enum SelectionType {NoSelectionType, ByLayer, ByID};
  MainWindow(QWidget* parent = 0);
  ~MainWindow();
  void setRunFile(const QString&);
private slots:
  void update();
  void updateProjection();
  void saveButtonClicked();
  void fillComboBox();
private:
  CalibrationType calibrationType() const;
  SelectionType selectionType() const;
  static bool compareElementPosition(DetectorElement*, DetectorElement*);
  Ui_mainWindow m_ui;
  QString m_fileName;
  TH2S* m_histogram;
  TLine* m_projectionLine;
  TH1D* m_projectionHistogram;
  CalibrationCollection* m_collection;
};

#endif