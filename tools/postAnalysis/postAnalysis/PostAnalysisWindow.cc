#include "PostAnalysisWindow.hh"
#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "RootStyle.hh"

#include "ui_PostAnalysisWindow.h"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>
#include <TH2.h>
#include <TF2.h>
#include <RootQtWidget.hh>

#include <QDebug>
#include <QFileDialog>
#include <QPushButton>

PostAnalysisWindow::PostAnalysisWindow(QWidget* parent)
  : QMainWindow(parent)
  , m_canvases()
  , m_plots()
  , m_ui(new Ui_postAnalysisWindow)
{
  m_ui->setupUi(this);
  m_ui->userControlWidget->hide();
  m_ui->userControlWidget->layout()->setAlignment(Qt::AlignLeft | Qt::AlignHCenter);
  connect(m_ui->canvasListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectCanvas(QListWidgetItem*)));
  connect(m_ui->canvasListWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
    this, SLOT(selectCanvas(QListWidgetItem*, QListWidgetItem*)));
  connect(m_ui->plotListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPlot(QListWidgetItem*)));
  connect(m_ui->plotListWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
    this, SLOT(selectPlot(QListWidgetItem*, QListWidgetItem*)));
  connect(m_ui->plotOptionComboBox, SIGNAL(currentIndexChanged(const QString&)),
    this, SLOT(plotOptionComboBoxCurrentIndexChanged(const QString&)));
  connect(m_ui->saveButton, SIGNAL(clicked()), this, SLOT(saveButtonClicked()));
  connect(m_ui->saveAllButton, SIGNAL(clicked()), this, SLOT(saveAllButtonClicked()));
}

PostAnalysisWindow::~PostAnalysisWindow()
{
  delete m_ui;
}

void PostAnalysisWindow::selectCanvas(QListWidgetItem* item, QListWidgetItem*)
{
  selectCanvas(item);
}

void PostAnalysisWindow::selectCanvas(QListWidgetItem* item)
{
  RootStyle::setPalette(RootStyle::DefaultPalette);
  m_ui->qtWidget->GetCanvas()->cd();
  m_ui->qtWidget->GetCanvas()->Clear();
  m_canvases[m_ui->canvasListWidget->row(item)]->draw(m_ui->qtWidget->GetCanvas());
  plotOptionComboBoxCurrentIndexChanged(m_ui->plotOptionComboBox->currentText());
  m_ui->titleLabel->setText(item->text());
  if (m_ui->leftVerticalLayout->count() > 2) {
    m_ui->leftVerticalLayout->itemAt(1)->widget()->close();
  }
}

void PostAnalysisWindow::selectPlot(QListWidgetItem* item, QListWidgetItem*)
{
  selectPlot(item);
}

void PostAnalysisWindow::selectPlot(QListWidgetItem* item)
{
  gPad->Clear();
  m_plots[m_ui->plotListWidget->row(item)]->draw(m_ui->qtWidget->GetCanvas());
  plotOptionComboBoxCurrentIndexChanged(m_ui->plotOptionComboBox->currentText());
  m_ui->titleLabel->setText(item->text());
  QWidget* secondaryWidget = m_plots[m_ui->plotListWidget->row(item)]->secondaryWidget();
  if (m_ui->leftVerticalLayout->count() > 2) {
    m_ui->leftVerticalLayout->itemAt(1)->widget()->close();
  }
  if (secondaryWidget) {
    m_ui->leftVerticalLayout->insertWidget(1, secondaryWidget);
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

void PostAnalysisWindow::setAnalysisFile(const QString& file)
{
  m_analysisFile = file;
  setupAnalysis();
}

PostAnalysisCanvas* PostAnalysisWindow::addCanvas(TFile* file, const QString& name)
{
  PostAnalysisCanvas* canvas = new PostAnalysisCanvas(file, name);
  m_canvases.append(canvas);
  QListWidgetItem* item = new QListWidgetItem(canvas->name());
  item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
  m_ui->canvasListWidget->addItem(item);
  return canvas;
}

void PostAnalysisWindow::addPlot(PostAnalysisPlot* plot)
{
  m_plots.append(plot);
  QListWidgetItem* item = new QListWidgetItem(plot->title());
  item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
  m_ui->plotListWidget->addItem(item);
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
  QString fileName = QFileDialog::getSaveFileName(this, "save current canvas", ".", "All Files(*.*);;" + filters.join(";;"), &selectedFilter);

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
