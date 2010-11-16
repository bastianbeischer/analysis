#include "RootPlot.hh"

unsigned long RootPlot::s_rootPlotCounter = 0;

RootPlot::RootPlot()
{
  ++s_rootPlotCounter;
}

RootPlot::~RootPlot()
{
}
