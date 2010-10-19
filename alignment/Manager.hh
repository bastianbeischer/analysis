#ifndef Manager_hh
#define Manager_hh

#include <vector>
#include <QString>

class Matrix;
class Strategy;
class Parameters;
class SimulationInterface;
class TestbeamInterface;
class Track;
class Track;

class Manager
{

public:
  ~Manager();

  static Manager* GetInstance();

  Strategy*   GetStrategy() {return m_strategy;}
  Parameters* GetParameters() {return m_parameters;}
  SimulationInterface* GetSimulationInterface() {return m_simulationInterface;}
  TestbeamInterface*   GetTestbeamInterface() {return m_testbeamInterface;}

  bool LoadStrategyFromFile(QString fileName);
  void StartAlignment();
  bool WriteResultsToFile(QString fileName = "output.txt");

  void ChangeMethodTo(QString method);

  void AddTrack(Track* track) {m_tracks.push_back(track);}

private:
  Manager();

private:
  static Manager* m_instance;

  Strategy*       m_strategy;
  Matrix*         m_matrix;
  Parameters*     m_parameters;
  SimulationInterface*     m_simulationInterface;
  TestbeamInterface*       m_testbeamInterface;

  std::vector<Track*> m_tracks;

};

#endif /* Manager_hh */
