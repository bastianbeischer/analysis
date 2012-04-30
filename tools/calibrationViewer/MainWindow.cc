#include "MainWindow.hh"
#include "Setup.hh"
#include "DataDescription.hh"
#include "CalibrationCollection.hh"
#include "Layer.hh"
#include "DetectorElement.hh"

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TList.h>
#include <TH1.h>
#include <TH2.h>
#include <TAxis.h>
#include <TPad.h>

#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , m_histogram(0)
  , m_collection(0)
{
  m_ui.setupUi(this);
  m_ui.typeComboBox->addItem("LED");
  m_ui.typeComboBox->addItem("pedestal");
  LayerIterator itEnd = Setup::instance()->lastLayer();
  for (LayerIterator it = Setup::instance()->firstLayer(); it != itEnd; ++it)
    m_ui.layerComboBox->insertItem(0, QString("layer %1mm").arg(it.key()), it.key());
  connect(m_ui.typeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));
  connect(m_ui.layerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));
}

MainWindow::~MainWindow()
{
  delete m_histogram;
  delete m_collection;
}

void MainWindow::setRunFile(const QString& fileName)
{
  Q_ASSERT(fileName.endsWith(".root"));

  TFile file(qPrintable(fileName));
  gROOT->cd();
  TTree* tree = static_cast<TTree*>(file.Get("SimpleEventTree"));
  TList* userInfo = static_cast<TList*>(tree->GetUserInfo());
  TObject* object = userInfo->At(0);
  Q_ASSERT(!strcmp(object->ClassName(), "DataDescription"));
  DataDescription* dataDescription = static_cast<DataDescription*>(userInfo->At(0));
  m_collection = new CalibrationCollection(*(dataDescription->calibrationCollection()));
  file.Close();
  update();
}

bool MainWindow::compareElementPosition(DetectorElement* e1, DetectorElement* e2)
{
  return e1->position().x() < e2->position().x();
}

void MainWindow::update()
{
  bool led = m_ui.typeComboBox->currentText() == "LED";
  double zPosition = m_ui.layerComboBox->itemData(m_ui.layerComboBox->currentIndex()).toDouble();
  LayerIterator it = Setup::instance()->firstLayer();
  LayerIterator itEnd = Setup::instance()->lastLayer();
  while (it != itEnd) {
    if (qAbs(it.key() - zPosition) < 1.0)
      break;
    ++it;
  }
  Layer* layer = it.value();
  QList<DetectorElement*> elements = layer->elements();
  qSort(elements.begin(), elements.end(), MainWindow::compareElementPosition);
  if (m_histogram)
    delete m_histogram;

  int nIdBins = 0;
  foreach (DetectorElement* element, elements)
    nIdBins+= element->nChannels();

  int max = 4095;
  m_histogram = new TH2D("calibration histogram", "", nIdBins, 0, nIdBins, max, 0, max);

  int idBin = 1;
  foreach (DetectorElement* element, elements) {
    for (int channel = 0; channel < element->nChannels(); ++channel) {
      unsigned short id = element->id() | channel;
      const TH1I* histogram = led ? m_collection->ledHistogram(id) : m_collection->pedestalHistogram(id);
      if (!histogram)
        continue;
      for (int adcBin = 1; adcBin <= max; ++adcBin)
        m_histogram->SetBinContent(idBin, adcBin, histogram->GetBinContent(adcBin));
        m_histogram->GetXaxis()->SetBinLabel(idBin, qPrintable(QString("0x%1").arg(id, 0, 16)));
      ++idBin;
    }
  }

  m_histogram->Draw("COLZ");
  gPad->SetLogz();
  gPad->Modified();
  gPad->Update();
}















