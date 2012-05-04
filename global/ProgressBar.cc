#include "ProgressBar.hh"
#include <QtGlobal>

ProgressBar::ProgressBar(std::ostream& stream)
  : m_stream(stream)
  , m_total(0)
  , m_current(0)
  , m_printed(0)
  , m_finished(false)
{
  restart(m_total, false);
}

ProgressBar::ProgressBar(int total, std::ostream& stream)
  : m_stream(stream)
  , m_total(total)
  , m_current(0)
  , m_printed(0)
  , m_finished(false)
{
  restart(m_total, false);
}

void ProgressBar::next()
{
  if (m_finished)
    return;
  ++m_current;
  char toBePrinted = qRound(100. * m_current / m_total);
  for (; m_printed <= toBePrinted; ++m_printed)
    m_stream << '#' << std::flush;
  if (m_current == m_total)
    finish();
}

void ProgressBar::finish()
{
  if (m_finished)
    return;
  for (; m_printed <= 100; ++m_printed)
    m_stream << ' ';
  m_stream
    << " |" << std::endl
    << "+-------------------------------------------------------------------------------------------------------+" << std::endl
    << std::flush;
  m_finished = true;
}

void ProgressBar::restart()
{
  restart(m_total, true);
}

void ProgressBar::restart(int total)
{
  restart(total, true);
}

void ProgressBar::restart(int total, bool finalize)
{
  if (finalize && m_current != m_total)
    finish();
  m_total = total;
  m_current = 0;
  m_printed = 0;
  m_finished = false;
  m_stream
    << "+-------------------------------------------------------------------------------------------------------+" << std::endl
    << "| Processing:                                                                                           |" << std::endl
    << "| 0%       10%       20%       30%       40%       50%       60%       70%       80%       90%     100% |" << std::endl
    << "| |.........|.........|.........|.........|.........|.........|.........|.........|.........|.........| |" << std::endl
    << "| " << std::flush;
}
