#ifndef MainWindow_h
#define MainWindow_h

#include "PostAnalysisWindow.hh"

class MainWindow : public PostAnalysisWindow {
Q_OBJECT
public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();
  void setAnalysisFile(const QString&);
  void setMCFile(const QString&);

  void setupAnalysis();

protected:
  QString m_mcFile;
};

#endif
