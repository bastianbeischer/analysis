#ifndef Strategy_hh
#define Strategy_hh

#include <vector>

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
  void                 DoInit();

  Parameters*          GetParameters()               const {return m_parameters;}
  unsigned int         GetNumberOfModules()          const {return m_nModules;}
  unsigned int         GetNumberOfGlobalIterations() const {return m_nGlobalIterations;}
  unsigned int         GetNumberOfGlobalParameters() const {return m_nGlobal;}
  unsigned int         GetNumberOfLocalParameters()  const {return m_nLocal;}
  float                GetLadderResolutionS()        const {return m_ladderResolutionS;}
  float                GetLadderResolutionK()        const {return m_ladderResolutionK;}
  float                GetFiberResolution()          const {return m_fiberResolution;}

  bool                 ReadFromFile(QString fileName);
  bool                 WriteToFile(QFile* file);

private:
  Parameters*              m_parameters;

  std::vector<Constraint*> m_constraints;

  unsigned int             m_nModules;
  unsigned int             m_nGlobal;
  unsigned int             m_nLocal;
  unsigned int             m_nStdDev;
  int                      m_verbose;

  unsigned int             m_nIter;
  float                    m_cutValue;

  float                    m_ladderResolutionS;
  float                    m_ladderResolutionK;
  float                    m_fiberResolution;

  unsigned int             m_nGlobalIterations;

};

#endif /* Strategy_hh */
