#ifndef ProgressBar_hh
#define ProgressBar_hh

#include <ostream>
#include <iostream>

class ProgressBar
{
public:
  ProgressBar(std::ostream& = std::cout);
  ProgressBar(int, std::ostream& = std::cout);
  void next();
  void finish();
  void restart();
  void restart(int);
  double progress() const;
private:
  void restart(int, bool);
  std::ostream& m_stream;
  long m_total;
  long m_current;
  char m_printed;
  bool m_finished;
};

#endif
