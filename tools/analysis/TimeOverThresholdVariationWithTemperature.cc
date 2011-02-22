#include "TimeOverThresholdVariationWithTemperature.hh"

#include "SimpleEvent.hh"
#include "Hit.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Track.hh"
#include "Constants.hh"
#include "Corrections.hh"

#include <TH2D.h>
#include <TAxis.h>

#include <QString>
#include <QDebug>
#include <QVariant>
#include <QSettings>
#include <math.h>

TimeOverThresholdVariationWithTemperature::TimeOverThresholdVariationWithTemperature(unsigned int tofChannel)
  : AnalysisPlot(TimeOverThreshold)
  , H2DPlot()
  , m_tofChannel(tofChannel)
{
  QString title = QString("time over threshold temperature variation 0x%1").arg(0x8000 | tofChannel, 0, 16);
  setTitle(title);
	const unsigned int nTemperatureBins = 100;
	const double minTemperature = -25;
	const double maxTemperature = 35;
	const unsigned int nTotBins = 100;
	const double minTot = 0;
	const double maxTot = 75;
  TH2D* histogram = new TH2D(qPrintable(title), "", nTemperatureBins, minTemperature, maxTemperature, nTotBins, minTot, maxTot);
  histogram->GetXaxis()->SetTitleOffset(1.4);
  histogram->GetXaxis()->SetTitle("temperature /  #circ C");
  histogram->GetYaxis()->SetTitle("time over threshold / ns");
  setHistogram(histogram);
	
	m_sensorAssignment = new TOFSensorTypeAssignment();
}

TimeOverThresholdVariationWithTemperature::~TimeOverThresholdVariationWithTemperature() {
	delete m_sensorAssignment;
	m_sensorAssignment = NULL;
}

SensorTypes::Type findTofSensorType(unsigned int tofChannel);

void TimeOverThresholdVariationWithTemperature::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent* event) {
  double temperature = event->sensorData(m_sensorAssignment->tofSensorType(m_tofChannel));
	foreach(Hit* hit, hits) {
    if (hit->type() == Hit::tof) {
      TOFCluster* cluster = static_cast<TOFCluster*> (hit);
      if (qAbs(track->x(cluster->position().z()) - cluster->position().x()) > Constants::tofBarWidth)
        continue;
      foreach(Hit* tofHit, cluster->hits()) {
				if (tofHit->detId() == m_tofChannel) {
					TOFSipmHit* tofSipmHit = static_cast<TOFSipmHit*>(tofHit);
					histogram()->Fill(temperature, tofSipmHit->timeOverThreshold());
				}
      }
    }
  }
}

void TimeOverThresholdVariationWithTemperature::finalize() {
	//saveCalibrationData(); //uncomment if you want to save new calibraion data
}

void TimeOverThresholdVariationWithTemperature::saveCalibrationData() {
	const double timeOverThresholdReference = 30;
	const double minTot = 20;
	
	QMap<QString, QVariant> temperatureMap;
	for (int bin = 0; bin < histogram()->GetNbinsX(); ++bin) {
		TH1* projectionHistogram = histogram()->ProjectionY("_px", bin + 1, bin + 1);
		double tot = projectionHistogram->GetMean();
		//double totError = projectionHistogram->GetRMS();
		double temperature = histogram()->GetBinCenter(bin+1);
		//double temperatureError = histogram()->GetBinWidth(bin+1)/sqrt(12);
		
		if (tot > minTot) {
			double scalingFactor;
			scalingFactor = timeOverThresholdReference / tot;
			temperatureMap.insert(QString::number(temperature), scalingFactor);
		}
	}
	Corrections* correction = new Corrections();
	correction->setTimeOverThresholdScaling(m_tofChannel, temperatureMap);
	delete correction;
	correction = NULL;
}



