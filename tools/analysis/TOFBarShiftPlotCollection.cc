#include "TOFBarShiftPlotCollection.hh"
#include "TOFBarShiftPlot.hh"
#include "TOFChannelSelectionWidget.hh"

#include <QLayout>

TOFBarShiftPlotCollection::TOFBarShiftPlotCollection() :
  PlotCollection(Enums::CalibrationTOF)
{
  addRequiredEventFlags(Enums::TrackGood | Enums::Chi2Good);

  unsigned int upperBars[][2] = {{0x8000, 0x8010}, {0x8004, 0x8014}, {0x8008, 0x8018}, {0x800c, 0x801c}};
  unsigned int lowerBars[][2] = {{0x8020, 0x8030}, {0x8024, 0x8034}, {0x8028, 0x8038}, {0x802c, 0x803c}};
  QStringList moduleIDs;
  for (int upperBar = 0; upperBar < 4; ++upperBar) {
    for (int lowerBar = 0; lowerBar < 4; ++lowerBar) {
      unsigned int id1 = upperBars[upperBar][0];
      unsigned int id2 = upperBars[upperBar][1];
      unsigned int id3 = lowerBars[lowerBar][0];
      unsigned int id4 = lowerBars[lowerBar][1];
      addPlot(new TOFBarShiftPlot(id1, id2, id3, id4));
      moduleIDs.append(QString("0x%1 0x%2 0x%3 0x%4").arg(id1, 0, 16).arg(id2, 0, 16).arg(id3, 0, 16).arg(id4, 0, 16));
    }
  }
  TOFChannelSelectionWidget* widget = new TOFChannelSelectionWidget(moduleIDs);
  connect(widget, SIGNAL(channelChanged(int)), this, SLOT(selectPlot(int)));
  secondaryWidget()->layout()->addWidget(widget);
  setTitle("bar shift collection");
}

TOFBarShiftPlotCollection::~TOFBarShiftPlotCollection()
{
}

