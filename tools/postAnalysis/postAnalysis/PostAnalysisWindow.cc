#include "PostAnalysisWindow.hh"
#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "RootStyle.hh"

#include "ui_PostAnalysisWindow.h"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TF2.h>
#include <TAxis.h>

#include <RootQtWidget.hh>

#include <QDebug>
#include <QFileDialog>
#include <QPushButton>

PostAnalysisWindow::PostAnalysisWindow(QWidget* parent)
  : QMainWindow(parent)
  , m_canvases()
  , m_plots()
  , m_dialogOptions(QFileDialog::DontUseNativeDialog)
  , m_ui(new Ui_postAnalysisWindow)
  , m_selectedCanvas(0)
  , m_selectedPlot(0)
{
  m_ui->setupUi(this);
  m_ui->aspectRatioComboBox->addItem("auto", -1.);
  m_ui->aspectRatioComboBox->addItem("3:4", 3./4.);
  m_ui->aspectRatioComboBox->addItem("1:1", 1./1.);
  m_ui->aspectRatioComboBox->addItem("5:4", 5./4.);
  m_ui->aspectRatioComboBox->addItem("4:3", 4./3.);
  m_ui->aspectRatioComboBox->addItem("16:10", 16./10.);
  m_ui->aspectRatioComboBox->addItem("16:9", 16./9.);
  connect(m_ui->aspectRatioComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(aspectRatioChanged(int)));
  m_ui->verticalSplitter->setStretchFactor(0, 1);
  m_ui->verticalSplitter->setStretchFactor(1, 1);
  m_ui->verticalSplitter->setStretchFactor(2, 0);
  m_ui->userControlWidget->hide();
  m_ui->userControlWidget->layout()->setAlignment(Qt::AlignLeft | Qt::AlignHCenter);
  connect(m_ui->plotOptionComboBox, SIGNAL(currentIndexChanged(const QString&)),
    this, SLOT(plotOptionComboBoxCurrentIndexChanged(const QString&)));
  connect(m_ui->saveButton, SIGNAL(clicked()), this, SLOT(saveButtonClicked()));
  connect(m_ui->saveAllButton, SIGNAL(clicked()), this, SLOT(saveAllButtonClicked()));
  connect(m_ui->logXCheckBox, SIGNAL(stateChanged(int)), m_ui->qtWidget, SLOT(setLogX(int)));
  connect(m_ui->logYCheckBox, SIGNAL(stateChanged(int)), m_ui->qtWidget, SLOT(setLogY(int)));
  connect(m_ui->logZCheckBox, SIGNAL(stateChanged(int)), m_ui->qtWidget, SLOT(setLogZ(int)));
  connect(m_ui->qtWidget, SIGNAL(positionChanged(double, double)), this, SLOT(canvasPositionChanged(double, double)));
  connect(m_ui->qtWidget, SIGNAL(unzoomButtonPressed()), this, SLOT(unzoom()));
  connect(m_ui->canvasFilterComboBox, SIGNAL(editTextChanged(const QString&)), this, SLOT(canvasFilterChanged(const QString&)));
  connect(m_ui->plotFilterComboBox, SIGNAL(editTextChanged(const QString&)), this, SLOT(plotFilterChanged(const QString&)));
  m_ui->canvasWidget->hide();
  m_ui->plotWidget->hide();
}

PostAnalysisWindow::~PostAnalysisWindow()
{
  delete m_ui;
}

void PostAnalysisWindow::canvasFilterChanged(const QString& text)
{
  m_ui->canvasListWidget->disconnect();
  m_ui->canvasListWidget->clear();
  foreach(PostAnalysisCanvas* canvas, m_canvases) {
    if (canvas->name().contains(QRegExp(text))) {
      QListWidgetItem* item = new QListWidgetItem(canvas->name());
      item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
      m_ui->canvasListWidget->addItem(item);
    }
  }
  connect(m_ui->canvasListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectCanvas(QListWidgetItem*)));
  connect(m_ui->canvasListWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
    this, SLOT(selectCanvas(QListWidgetItem*, QListWidgetItem*)));
}

void PostAnalysisWindow::plotFilterChanged(const QString& text)
{
  m_ui->plotListWidget->disconnect();
  m_ui->plotListWidget->clear();
  foreach(PostAnalysisPlot* plot, m_plots) {
    if (plot->title().contains(QRegExp(text))) {
      QListWidgetItem* item = new QListWidgetItem(plot->title());
      item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
      m_ui->plotListWidget->addItem(item);
    }
  }
  connect(m_ui->plotListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPlot(QListWidgetItem*)));
  connect(m_ui->plotListWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
    this, SLOT(selectPlot(QListWidgetItem*, QListWidgetItem*)));
}

void PostAnalysisWindow::addCanvasFilter(const QString& string)
{
  m_ui->canvasFilterComboBox->addItem(string);
}

void PostAnalysisWindow::addPlotFilter(const QString& string)
{
  m_ui->plotFilterComboBox->addItem(string);
}

void PostAnalysisWindow::selectCanvas(QListWidgetItem* item, QListWidgetItem*)
{
  selectCanvas(item);
}

void PostAnalysisWindow::selectCanvas(QListWidgetItem* item)
{
  qDebug() << item->text();
  gPad->Clear();
  m_ui->qtWidget->GetCanvas()->cd();
  m_ui->qtWidget->GetCanvas()->Clear("D");
  if (!item)
    return;
  m_ui->plotListWidget->setCurrentItem(0);
  m_selectedPlot = 0;
  RootStyle::setPalette(RootStyle::DefaultPalette);
  foreach (PostAnalysisCanvas* canvas, m_canvases) {
    if (item->text() == canvas->name()) {
      canvas->draw(m_ui->qtWidget->GetCanvas());
      gPad->SetLogx(m_ui->logXCheckBox->isChecked());
      gPad->SetLogy(m_ui->logYCheckBox->isChecked());
      gPad->SetLogz(m_ui->logZCheckBox->isChecked());
      m_selectedCanvas = canvas;
      break;
    }
  }
  plotOptionComboBoxCurrentIndexChanged(m_ui->plotOptionComboBox->currentText());
  m_ui->titleLabel->setText(item->text());
  if (m_ui->verticalLayoutLeft->count() > 3) {
    m_ui->verticalLayoutLeft->itemAt(1)->widget()->close();
  }
}

void PostAnalysisWindow::selectPlot(QListWidgetItem* item, QListWidgetItem*)
{
  selectPlot(item);
}

void PostAnalysisWindow::selectPlot(QListWidgetItem* item)
{
  gPad->Clear();
  if (!item)
    return;
  m_ui->canvasListWidget->setCurrentItem(0);
  m_selectedCanvas = 0;
  foreach (PostAnalysisPlot* plot, m_plots) {
    if (item->text() == plot->title()) {
      plot->draw(m_ui->qtWidget->GetCanvas());
      gPad->SetLogx(m_ui->logXCheckBox->isChecked());
      gPad->SetLogy(m_ui->logYCheckBox->isChecked());
      gPad->SetLogz(m_ui->logZCheckBox->isChecked());
      m_selectedPlot = plot;
      break;
    }
  }
  plotOptionComboBoxCurrentIndexChanged(m_ui->plotOptionComboBox->currentText());

  m_ui->titleLabel->setText(item->text());
  QWidget* secondaryWidget = m_selectedPlot->secondaryWidget();
  if (m_ui->verticalLayoutLeft->count() > 3) {
    m_ui->verticalLayoutLeft->itemAt(1)->widget()->close();
  }
  if (secondaryWidget) {
    m_ui->verticalLayoutLeft->insertWidget(1, secondaryWidget);
    secondaryWidget->show();
  }
}

void PostAnalysisWindow::plotOptionComboBoxCurrentIndexChanged(const QString& option)
{
  TCanvas* canvas = m_ui->qtWidget->GetCanvas();
  for (int i = 0; i < canvas->GetListOfPrimitives()->GetSize(); ++i) {
    if (!strcmp(canvas->GetListOfPrimitives()->At(i)->ClassName(), "TH2D"))
      (static_cast<TH2*>(canvas->GetListOfPrimitives()->At(i)))->SetDrawOption(qPrintable(option));
    if (!strcmp(canvas->GetListOfPrimitives()->At(i)->ClassName(), "TF2"))
      (static_cast<TF2*>(canvas->GetListOfPrimitives()->At(i)))->SetDrawOption(qPrintable(option));
  }
  gPad->Modified();
  gPad->Update();
}

void PostAnalysisWindow::addAnalysisFiles(const QStringList& files)
{
  m_analysisFiles = files;
  setupAnalysis();
}

void PostAnalysisWindow::addCanvas(PostAnalysisCanvas* canvas)
{
  Q_ASSERT(canvas);
   m_ui->canvasListWidget->disconnect();
  m_canvases.append(canvas);
  QListWidgetItem* item = new QListWidgetItem(canvas->name());
  item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
  m_ui->canvasListWidget->addItem(item);
  m_ui->canvasWidget->show();
  connect(m_ui->canvasListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectCanvas(QListWidgetItem*)));
  connect(m_ui->canvasListWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
    this, SLOT(selectCanvas(QListWidgetItem*, QListWidgetItem*)));
}

PostAnalysisCanvas* PostAnalysisWindow::addCanvas(TFile* file, const QString& name)
{
  PostAnalysisCanvas* canvas = PostAnalysisCanvas::fromFile(file, name);
  if (!canvas)
    return 0;
  addCanvas(canvas);
  return canvas;
}

void PostAnalysisWindow::addPlot(PostAnalysisPlot* plot)
{
  m_ui->plotListWidget->disconnect();
  m_plots.append(plot);
  QListWidgetItem* item = new QListWidgetItem(plot->title());
  item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
  m_ui->plotListWidget->addItem(item);
  m_ui->plotWidget->show();
  connect(m_ui->plotListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPlot(QListWidgetItem*)));
  connect(m_ui->plotListWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
    this, SLOT(selectPlot(QListWidgetItem*, QListWidgetItem*)));
}

void PostAnalysisWindow::clearCanvases()
{
  m_ui->canvasListWidget->disconnect();
  m_selectedCanvas = 0;
  m_ui->canvasListWidget->clear();
  m_canvases.clear();
  connect(m_ui->canvasListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPlot(QListWidgetItem*)));
  connect(m_ui->canvasListWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
    this, SLOT(selectPlot(QListWidgetItem*, QListWidgetItem*)));
}

void PostAnalysisWindow::clearPlots()
{
  m_ui->plotListWidget->disconnect();
  m_selectedPlot = 0;
  m_ui->plotListWidget->clear();
  m_plots.clear();
  connect(m_ui->plotListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPlot(QListWidgetItem*)));
  connect(m_ui->plotListWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
    this, SLOT(selectPlot(QListWidgetItem*, QListWidgetItem*)));
}

void PostAnalysisWindow::saveButtonClicked()
{
  addWidget(new QPushButton());
  QStringList fileFormatEndings;
  fileFormatEndings << "svg" << "pdf" << "eps" << "root" << "png";
  QStringList filters;
  foreach(QString ending, fileFormatEndings) {
    QString description = ending.toUpper();
    filters.append( description + "(*." + ending + ")" );
  }
  QString selectedFilter;
  QString fileName = QFileDialog::getSaveFileName(this, "save current canvas", ".", "All Files(*.*);;" + filters.join(";;"), &selectedFilter, m_dialogOptions);

  if (fileName.isEmpty())
    return;

  // if file name contains an ending, use that. Otherwise use selected filter
  QString fileEnding;
  if (fileName.contains('.')) {
    fileEnding = fileName.split('.').last().toLower();
  }
  else {
    fileEnding = selectedFilter.split("(").first().toLower();
  }

  // if filter == all, save all endings, otherwise use previously determined ending
  if(fileEnding == "all files"){
    foreach(QString fileFormatEnding, fileFormatEndings)
      m_ui->qtWidget->GetCanvas()->SaveAs(qPrintable(fileName + "." + fileFormatEnding));
  }
  else{
    if (!fileEnding.startsWith('.'))
      fileEnding.prepend('.');
    if (!fileName.endsWith(fileEnding))
      fileName.append(fileEnding);
    m_ui->qtWidget->GetCanvas()->SaveAs(qPrintable(fileName));
  }
}

void PostAnalysisWindow::saveAllButtonClicked()
{
  QStringList fileFormatEndings;
  fileFormatEndings << "svg" << "pdf" << "eps" << "root" << "png";
  QFileDialog dialog(this, "save all canvases displayed", ".");
  dialog.setOptions(m_dialogOptions);
  dialog.setFileMode(QFileDialog::DirectoryOnly);
  if (dialog.exec()) {
    for (int i = 0; i < m_ui->canvasListWidget->count(); ++i) {
      m_ui->canvasListWidget->setCurrentRow(i);
      QString directoryName = dialog.selectedFiles().first();
      foreach (QString fileFormatEnding, fileFormatEndings)
        m_ui->qtWidget->GetCanvas()->SaveAs(qPrintable(directoryName + '/' + m_ui->titleLabel->text() + "." + fileFormatEnding));
    }
    for (int i = 0; i < m_ui->plotListWidget->count(); ++i) {
      m_ui->plotListWidget->setCurrentRow(i);
      QString directoryName = dialog.selectedFiles().first();
      foreach (QString fileFormatEnding, fileFormatEndings)
        m_ui->qtWidget->GetCanvas()->SaveAs(qPrintable(directoryName + '/' + m_ui->titleLabel->text() + "." + fileFormatEnding));
    }
  }
}

void PostAnalysisWindow::addWidget(QWidget* widget)
{
  m_ui->userControlWidget->show();
  static_cast<QBoxLayout*>(m_ui->userControlWidget->layout())->insertWidget(0, widget);
}

void PostAnalysisWindow::aspectRatioChanged(int i)
{
  double aspectRatio = m_ui->aspectRatioComboBox->itemData(i).toDouble();
  m_ui->qtWidget->setAspectRatio(aspectRatio);
}

void PostAnalysisWindow::canvasPositionChanged(double x, double y)
{
  if (m_selectedPlot)
    m_selectedPlot->positionChanged(x, y);
  if (m_selectedCanvas) {
    m_ui->positionLabel->setText("");
    return;
  }
  m_ui->positionLabel->setText(QString("%1%2  %3%4")
      .arg(x < 0 ? '-' : '+').arg(qAbs(x), 7, 'f', 3, '0')
      .arg(y < 0 ? '-' : '+').arg(qAbs(y), 7, 'f', 3, '0'));
}

void PostAnalysisWindow::unzoom()
{
  TCanvas* canvas = m_ui->qtWidget->GetCanvas();
  for (int i = 0; i < canvas->GetListOfPrimitives()->GetSize(); ++i) {
    if (!strcmp(canvas->GetListOfPrimitives()->At(i)->ClassName(), "TGraph")) {
      (static_cast<TGraph*>(canvas->GetListOfPrimitives()->At(i)))->GetXaxis()->UnZoom();
      (static_cast<TGraph*>(canvas->GetListOfPrimitives()->At(i)))->GetYaxis()->UnZoom();
    }
    if (!strcmp(canvas->GetListOfPrimitives()->At(i)->ClassName(), "TGraphErrors")) {
      (static_cast<TGraphErrors*>(canvas->GetListOfPrimitives()->At(i)))->GetXaxis()->UnZoom();
      (static_cast<TGraphErrors*>(canvas->GetListOfPrimitives()->At(i)))->GetYaxis()->UnZoom();
    }
    if (!strcmp(canvas->GetListOfPrimitives()->At(i)->ClassName(), "TH1D")) {
      (static_cast<TH1D*>(canvas->GetListOfPrimitives()->At(i)))->GetXaxis()->UnZoom();
      (static_cast<TH1D*>(canvas->GetListOfPrimitives()->At(i)))->GetYaxis()->UnZoom();
    }
    if (!strcmp(canvas->GetListOfPrimitives()->At(i)->ClassName(), "TH2D")) {
      (static_cast<TH2D*>(canvas->GetListOfPrimitives()->At(i)))->GetXaxis()->UnZoom();
      (static_cast<TH2D*>(canvas->GetListOfPrimitives()->At(i)))->GetYaxis()->UnZoom();
      (static_cast<TH2D*>(canvas->GetListOfPrimitives()->At(i)))->GetZaxis()->UnZoom();
    }
  }
  gPad->Modified();
  gPad->Update();
}

void PostAnalysisWindow::updateCanvasFilter()
{
  canvasFilterChanged(m_ui->canvasFilterComboBox->currentText());
}

void PostAnalysisWindow::updatePlotFilter()
{
  plotFilterChanged(m_ui->plotFilterComboBox->currentText());
}
