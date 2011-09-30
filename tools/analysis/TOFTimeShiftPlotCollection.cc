#include "TOFTimeShiftPlotCollection.hh"

#include "TOFTimeShiftPlot.hh"
#include "TOFChannelSelectionWidget.hh"

#include <QLayout>

TOFTimeShiftPlotCollection::TOFTimeShiftPlotCollection() :
  PlotCollection(AnalysisTopic::CalibrationTOF)
{
  unsigned int bars[][2] = {
    {0x8000, 0x8010}, {0x8004, 0x8014}, {0x8008, 0x8018}, {0x800c, 0x801c},
    {0x8020, 0x8030}, {0x8024, 0x8034}, {0x8028, 0x8038}, {0x802c, 0x803c}
  };
  QStringList moduleIDs;
  for (int bar = 0; bar < 8; ++bar) {
    for (unsigned int refCh = 0; refCh < 8; ++refCh) {
      addPlot(new TOFTimeShiftPlot(bars[bar][0], bars[bar][1], refCh));
      unsigned int id = (refCh < 4) ? (bars[bar][0] | refCh) : (bars[bar][1] | (refCh - 4));
      moduleIDs.append(QString("0x%1 0x%2 ch %3 (0x%4)").arg(bars[bar][0], 0, 16).arg(bars[bar][1], 0, 16).arg(refCh).arg(id, 0, 16));
    }
  }
  TOFChannelSelectionWidget* widget = new TOFChannelSelectionWidget(moduleIDs);
  connect(widget, SIGNAL(channelChanged(int)), this, SLOT(selectPlot(int)));
  secondaryWidget()->layout()->addWidget(widget);
  setTitle("time shift collection");
}

TOFTimeShiftPlotCollection::~TOFTimeShiftPlotCollection()
{
}

