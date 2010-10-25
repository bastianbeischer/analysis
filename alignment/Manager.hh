#ifndef Manager_hh
#define Manager_hh

#include <QVector>
#include <QString>

class Matrix;
class Strategy;
class Parameters;
class Track;

class Manager
{

public:
  ~Manager();

  static Manager* instance();

  Strategy*   strategy()   {return m_strategy;}
  Parameters* parameters() {return m_parameters;}

  bool loadStrategyFromFile(QString fileName);
  void startAlignment();

  void addTrack(Track* track) {m_tracks.push_back(track);}

private:
  Manager();

private:
  static Manager* m_instance;

  Strategy*       m_strategy;
  Matrix*         m_matrix;
  Parameters*     m_parameters;

  QVector<Track*> m_tracks;

};

#endif /* Manager_hh */
