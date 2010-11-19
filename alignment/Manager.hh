#ifndef Manager_hh
#define Manager_hh

#include <QVector>
#include <QString>

class AlignmentMatrix;
class DataInterface;
class Strategy;
class Parameters;

class Manager
{

public:
  ~Manager();

  static Manager* instance();

  Strategy*       strategy()   {return m_strategy;}
  Parameters*     parameters() {return m_parameters;}

  bool            loadStrategyFromFile(QString fileName);
  void            startAlignment();
  void            saveResults() const;

private:
  Manager();

private:
  static Manager*  m_instance;

  Strategy*        m_strategy;
  AlignmentMatrix* m_matrix;
  Parameters*      m_parameters;
  DataInterface*   m_dataInterface;

};

#endif /* Manager_hh */
