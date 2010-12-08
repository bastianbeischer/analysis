#ifndef Strategy_hh
#define Strategy_hh

#include <QVector>

class DataInterface;
class Parameters;
class Constraint;
class QString;
class QFile;

class Strategy
{

public:
  Strategy();
  ~Strategy();

public:
  void                 init();

  Parameters*          parameters()               const {return m_parameters;}
  DataInterface*       dataInterface()             const {return m_dataInterface;}
  unsigned int         numberOfGlobalIterations() const {return m_nGlobalIterations;}
  unsigned int         numberOfGlobalParameters() const {return m_nGlobal;}
  unsigned int         numberOfLocalParameters()  const {return m_nLocal;}
  float                fiberResolution()          const {return m_fiberResolution;}

  bool                 readFromFile(QString fileName);

private:
  void                 fixDetector(unsigned short detId);
  void                 fixLayer(unsigned short layer);

private:
  Parameters*              m_parameters;
  QVector<Constraint*>     m_constraints;
  DataInterface*           m_dataInterface;

  unsigned int             m_nGlobal;
  unsigned int             m_nLocal;
  unsigned int             m_nStdDev;
  int                      m_verbose;

  unsigned int             m_nIter;
  float                    m_cutValue;

  float                    m_fiberResolution;

  unsigned int             m_nGlobalIterations;

};

#endif /* Strategy_hh */
