#include <iostream>
#include <QString>

#include "RunFile.h"

int main(int argc, char** argv)
{
  if (argc < 2) {
    std::cerr << "Wrong number of arguments:" << std::endl;
    std::cerr << "Usage: \"parser runfile.dat\" - where runfile.dat contains a PERDaix (C) rawfile" << std::endl;
    return -1;
  }

  QString input(argv[1]);

  RunFile runfile(input, RunFile::MODE_READING);

  int nCalibEvents = runfile.GetNumberOfCalibrationEvents();
  int nEvents = runfile.GetNumberOfEvents() - runfile.GetNumberOfCalibrationEvents();

  QString output = QString("%1 particle events   %2 calibration events").arg(nEvents, 6).arg(nCalibEvents, 6);
  std::cout << qPrintable(output) << std::endl;

  return 0;
}
