#include <iostream>
#include <QString>

#include <QDebug>

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
  int nEvents = runfile.GetNumberOfCalibrationEvents() - nCalibEvents;;
  if (nEvents == 0) {
    qDebug() << "File doesn't contain a valid header. Trying to read as many events as possible.";

    int counter = 0;
    RawEvent* event = 0;
    while(counter < nCalibEvents) {
      event = (RawEvent*) runfile.ReadNextEvent();
      delete event;
      counter++;
    }

    counter = 0;
    while((event = (RawEvent*) runfile.ReadNextEvent())) {
      delete event;
      counter++;
    }
    nEvents = counter;
  }

  QString output = QString("%1 particle events   %2 calibration events").arg(nEvents, 6).arg(nCalibEvents, 6);
  std::cout << qPrintable(output) << std::endl;

  return 0;
}
