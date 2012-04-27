#include "SingleFile.hh"
#include "DataChain.hh"
#include "CalibrationCollection.hh"
#include "SimpleEvent.hh"
#include "DataDescription.hh"

#include <TApplication.h>
#include <TCanvas.h>
#include <TH1.h>

#include <QString>
#include <QDebug>

int main(int argc, char** argv)
{
  Q_ASSERT(argc == 2);
  DataChain chain;
  chain.addRootFile(argv[1]);
  qDebug() << chain.nEntries();
  SimpleEvent* event = chain.event(0);
  Q_ASSERT(event);
  const DataDescription* description = event->description();
  Q_ASSERT(description);
  const CalibrationCollection* calibrations = description->calibrationCollection();
  Q_ASSERT(calibrations);

  unsigned short id = 0x615b;
  TH1I* pedestalHistogram = static_cast<TH1I*>(calibrations->pedestalHistogram(id)->Clone());
  TH1I* ledHistogram = static_cast<TH1I*>(calibrations->ledHistogram(id)->Clone());

  TApplication application("calibration", &argc, argv);
  new TCanvas("pedestal", "pedestal", 1);
  pedestalHistogram->Draw();
  new TCanvas("led", "led", 1);
  ledHistogram->Draw();
  application.Run();
  return 0;
}
