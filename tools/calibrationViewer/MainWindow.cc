#include "MainWindow.hh"
#include "Setup.hh"
#include "DataDescription.hh"
#include "CalibrationCollection.hh"
#include "Layer.hh"
#include "DetectorElement.hh"
#include "Helpers.hh"

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TList.h>
#include <TH1.h>
#include <TH2.h>
#include <TAxis.h>
#include <TPad.h>

#include <QDebug>
#include <QFileDialog>
#include <QStringList>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , m_fileName()
  , m_histogram(0)
  , m_collection(0)
{
  m_ui.setupUi(this);
  m_ui.calibrationTypeComboBox->addItem("LED shifted");
  m_ui.calibrationTypeComboBox->addItem("LED");
  m_ui.calibrationTypeComboBox->addItem("pedestal");
  m_ui.selectionTypeComboBox->addItem("by layer");
  m_ui.selectionTypeComboBox->addItem("by ID");
  fillComboBox();
  connect(m_ui.saveButton, SIGNAL(clicked()), this, SLOT(saveButtonClicked()));
}

MainWindow::~MainWindow()
{
  delete m_histogram;
  delete m_collection;
}

void MainWindow::setRunFile(const QString& fileName)
{
  Q_ASSERT(fileName.endsWith(".root"));
  m_fileName = QFileInfo(fileName).baseName();
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

MainWindow::CalibrationType MainWindow::calibrationType() const
{
  if (m_ui.calibrationTypeComboBox->currentText() == "LED shifted") {
    return LedShifted;
  } else if (m_ui.calibrationTypeComboBox->currentText() == "LED") {
    return Led;
  } else if (m_ui.calibrationTypeComboBox->currentText() == "pedestal") {
    return Pedestal;
  }
  Q_ASSERT(false);
  return NoCalibrationType;
}

MainWindow::SelectionType MainWindow::selectionType() const
{
  if (m_ui.selectionTypeComboBox->currentText() == "by layer") {
    return  ByLayer;
  } else if (m_ui.selectionTypeComboBox->currentText() == "by ID") {
    return ByID;
  }
  Q_ASSERT(false);
  return NoSelectionType;
}

void MainWindow::fillComboBox()
{
  m_ui.calibrationTypeComboBox->disconnect();
  m_ui.selectionTypeComboBox->disconnect();
  m_ui.selectionComboBox->disconnect();
  m_ui.selectionComboBox->clear();
  
  CalibrationType calType = calibrationType();
  SelectionType selType = selectionType();

  if (selType == ByID) {
    Q_ASSERT(m_collection);
    QVector<unsigned short> allPedestalIds = QVector<unsigned short>::fromStdVector(m_collection->pedestalIds());
    QVector<unsigned short> allLedIds = QVector<unsigned short>::fromStdVector(m_collection->ledIds());
    QVector<unsigned short> pedestalIds;
    QVector<unsigned short> ledIds;
    QVector<unsigned short> ids;

    foreach (unsigned short newId, allPedestalIds) {
      newId &= 0xFF00;
      bool found = false;
      foreach (unsigned short id, pedestalIds)
        if (newId == id)
          found = true;
      if (!found)
        pedestalIds.append(newId);
    }

    foreach (unsigned short newId, allLedIds) {
      newId &= 0xFF00;
      bool found = false;
      foreach (unsigned short id, ledIds)
        if (newId == id)
          found = true;
      if (!found)
        ledIds.append(newId);
    }

    if (calType == Pedestal) {
      ids = pedestalIds;
    } else if (calType == Led) {
      ids = ledIds;
    } else if (calType == LedShifted) {
      foreach(unsigned short pedestalId, pedestalIds)
        foreach(unsigned short ledId, ledIds)
          if (pedestalId == ledId)
            ids.append(pedestalId);
    }

    foreach (unsigned int id, ids)
      m_ui.selectionComboBox->addItem(QString("ID 0x%1").arg(id, 0, 16), id);
  } else if (selType == ByLayer) {
    LayerIterator itEnd = Setup::instance()->lastLayer();
    for (LayerIterator it = Setup::instance()->firstLayer(); it != itEnd; ++it)
      m_ui.selectionComboBox->insertItem(0, QString("layer %1mm").arg(it.key()), it.key());
  }

  connect(m_ui.calibrationTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(fillComboBox()));
  connect(m_ui.selectionTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(fillComboBox()));
  connect(m_ui.selectionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));
  update();
}

bool MainWindow::compareElementPosition(DetectorElement* e1, DetectorElement* e2)
{
  return e1->position().x() < e2->position().x();
}

void MainWindow::update()
{
  if (!m_collection)
    return;
  if (m_histogram)
    delete m_histogram;

  CalibrationType calType = calibrationType();
  SelectionType selType = selectionType();
  QString title;

  QVector<unsigned short> ids;
  if (selType == ByID) {
    unsigned int fistId = m_ui.selectionComboBox->itemData(m_ui.selectionComboBox->currentIndex()).toInt();

    QVector<unsigned short> allPedestalIds = QVector<unsigned short>::fromStdVector(m_collection->pedestalIds());
    QVector<unsigned short> allLedIds = QVector<unsigned short>::fromStdVector(m_collection->ledIds());
    QVector<unsigned short> allIds = (calType == Pedestal) ? allPedestalIds : allLedIds;

    foreach (unsigned int id, allIds) {
      if ((id & 0xFF00) == fistId)
        ids.append(id);
    }
    title = m_ui.calibrationTypeComboBox->currentText() + " file " + m_fileName;
  } else if (selType == ByLayer) {
    double zPosition = m_ui.selectionComboBox->itemData(m_ui.selectionComboBox->currentIndex()).toDouble();
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
    foreach (DetectorElement* element, elements)
      for (int channel = 0; channel < element->nChannels(); ++channel)
        ids.append(element->id() | channel);
    title = m_ui.calibrationTypeComboBox->currentText() + " file " + m_fileName;
  }

  int rebin = 8;
  int max = 4095;
  double typicalShift = 0;
  if (calType == LedShifted)
    typicalShift = 500.;
  m_histogram = new TH2S("calibration histogram", qPrintable(title), ids.count(), 0, ids.count(), max/rebin, -typicalShift, max - typicalShift);
  int typicalBinShift = m_histogram->GetYaxis()->FindBin(typicalShift);
  int idBin = 1;
  foreach (unsigned short id, ids) {
      const TH1S* histogram = (calType == Pedestal) ? m_collection->pedestalHistogram(id) : m_collection->ledHistogram(id);
      if (!histogram)
        continue;
      double shift = (calType == LedShifted) ? m_collection->pedestalHistogram(id)->GetMean() : 0;
      int binShift = m_histogram->GetYaxis()->FindBin(shift);

      for (int adcBin = 1; adcBin <= max/rebin; ++adcBin) {
        double content = 0;
        for (int i = 0; i < rebin; ++i)
          content += histogram->GetBinContent(rebin * adcBin + i);
        m_histogram->SetBinContent(idBin, adcBin - binShift + typicalBinShift, content);
        m_histogram->GetXaxis()->SetBinLabel(idBin, qPrintable(QString("0x%1").arg(id, 0, 16)));
      }
      ++idBin;
    }

  m_histogram->GetYaxis()->SetTitle("ADC value");
  m_histogram->Draw("COLZ");
  //gPad->SetLogz();
  gPad->Modified();
  gPad->Update();
}

void MainWindow::saveButtonClicked()
{
  QStringList fileFormatEndings;
  fileFormatEndings << "svg" << "pdf" << "eps" << "root" << "png";
  QStringList filters;
  foreach(QString ending, fileFormatEndings) {
    QString description = ending.toUpper();
    filters.append( description + "(*." + ending + ")" );
  }
  QString selectedFilter;
  QString fileName = QFileDialog::getSaveFileName(this, "save current canvas",
      Helpers::analysisPath() + "/plots/" + m_fileName + ".png",
      "All Files(*.*);;" + filters.join(";;"), &selectedFilter);

  if (fileName.isEmpty())
    return;

  // if file name contains an ending, use that. Otherwise use selected filter
  QString fileEnding;
  if (fileName.contains('.')) {
    fileEnding = fileName.split('.').last().toLower();
  } else {
    fileEnding = selectedFilter.split("(").first().toLower();
  }

  // if filter == all, save all endings, otherwise use previously determined ending
  if (fileEnding == "all files"){
    foreach(QString fileFormatEnding, fileFormatEndings)
      m_ui.qtWidget->GetCanvas()->SaveAs(qPrintable(fileName + "." + fileFormatEnding));
  } else {
    if (!fileEnding.startsWith('.'))
      fileEnding.prepend('.');
    if (!fileName.endsWith(fileEnding))
      fileName.append(fileEnding);
    m_ui.qtWidget->GetCanvas()->SaveAs(qPrintable(fileName));
  }
}
