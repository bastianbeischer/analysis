#ifndef Manager_hh
#define Manager_hh

#include <vector>
#include <string>

class Matrix;
class Strategy;
class Parameters;
class SimulationInterface;
class TestbeamInterface;
class Track;

class Manager
{

public:
  ~Manager();

  static Manager* GetInstance();

  Strategy*   GetStrategy() {return m_strategy;}
  Parameters* GetParameters() {return m_parameters;}

  bool LoadStrategyFromFile(std::string fileName);
  void StartAlignment();
  bool WriteResultsToFile(std::string fileName = "output.txt");

  void ChangeMethodTo(std::string method);

  void AddTrack(Track* track) {m_tracks.push_back(track);}

private:
  Manager();

private:
  static Manager* m_instance;

  Strategy*       m_strategy;
  Matrix*         m_matrix;
  Parameters*     m_parameters;

  std::vector<Track*> m_tracks;

};

#endif /* Manager_hh */
