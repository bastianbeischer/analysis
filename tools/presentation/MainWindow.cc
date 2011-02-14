#include "MainWindow.hh"

#include <QDebug>
#include <QFileDialog>
#include <QProcess>

MainWindow::MainWindow(QWidget* parent)
  : QDialog(parent)
{
  m_ui.setupUi(this);

  QStringList envVariables = QProcess::systemEnvironment();
  QStringList filteredVars = envVariables.filter(QRegExp("^PERDAIXANA_PATH=*"));
  QString topLevelPath = "";
  if (filteredVars.size() != 0) {
    QString entry = filteredVars.first();
    topLevelPath = entry.split("=").at(1);
  } else {
    qFatal("ERROR: You need to set PERDAIXANA_PATH environment variable to the toplevel location!");
  }

  QFile file(topLevelPath + "/tools/presentation/" + "template.tex");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qFatal("Couldn't open template file!");
  }

  QTextStream in(&file);
  while (!in.atEnd()) {
    QString line = in.readLine();
    m_ui.texTextEdit->appendPlainText(line);
  }

  connect(m_ui.addFilesButton, SIGNAL(clicked()), this, SLOT(addFilesButtonClicked()));
  connect(m_ui.createButton, SIGNAL(clicked()), this, SLOT(createButtonClicked()));
  connect(m_ui.upButton, SIGNAL(clicked()), this, SLOT(upButtonClicked()));
  connect(m_ui.insertButton, SIGNAL(clicked()), this, SLOT(insertButtonClicked()));
  connect(m_ui.removeButton, SIGNAL(clicked()), this, SLOT(removeButtonClicked()));
  connect(m_ui.downButton, SIGNAL(clicked()), this, SLOT(downButtonClicked()));
}

MainWindow::~MainWindow()
{}

void MainWindow::createTexFile()
{
  QStringList tex = m_ui.texTextEdit->toPlainText().split('\n');
  m_ui.texTextEdit->clear();
  foreach (const QString& line, tex) {
    if (line == "\\begin{document}") {
      m_ui.texTextEdit->appendPlainText(line);
      for (int i = 0; i < m_ui.tableWidget->rowCount(); ++i) {
        QTableWidgetItem* titleItem = m_ui.tableWidget->item(i, 0);
        QTableWidgetItem* fileItem = m_ui.tableWidget->item(i, 1);
        if (titleItem && fileItem) {
          QDir dir(QDir::tempPath());
          if (!dir.exists("presentation"))
            dir.mkdir("presentation");
          QString fileName = fileItem->text();
          QString fileEnding = fileName.right(fileName.size() - fileName.lastIndexOf(".") - 1);
          QString linkName = QString("%1/presentation/slide%2.%3").arg(QDir::tempPath()).arg(i+1).arg(fileEnding);
          if (QFile::exists(linkName))
            QFile::remove(linkName);
          QFile::link(fileItem->text(), linkName);
          m_ui.texTextEdit->appendPlainText(QString("\\addSlide{%1}{%2}").arg(titleItem->text()).arg(linkName));
        }
      }
      m_ui.texTextEdit->appendPlainText("\\end{document}");
      break;
    }
    m_ui.texTextEdit->appendPlainText(line);
  }
}

void MainWindow::addFile(const QString& fileName)
{
  int cr = insertButtonClicked();
  QTableWidgetItem* item = 0;
  int index = fileName.lastIndexOf('/');
  QString title = fileName.mid(index + 1, fileName.lastIndexOf('.') - index - 1);
  item = new QTableWidgetItem(title);
  m_ui.tableWidget->setItem(cr, 0, item);
  item = new QTableWidgetItem(fileName);
  m_ui.tableWidget->setItem(cr, 1, item);
}

void MainWindow::addFilesButtonClicked()
{
  QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"),
    "", "PDF, PNG (*.pdf *.png);;PDF (*.pdf);;PNG (*.png)");
  foreach (const QString& fileName, fileNames) {
    addFile(fileName);
  }
}

void MainWindow::createButtonClicked()
{
  createTexFile();
  QProcess process(this);
  QStringList arguments;
  arguments.append("-halt-on-error");
  process.start("pdflatex", arguments);
  if (!process.waitForStarted())
    return;
  process.write(m_ui.texTextEdit->toPlainText().toAscii());
  process.closeWriteChannel();
  if (!process.waitForFinished())
    return;
  m_ui.outputTextEdit->setPlainText(process.readAll());
}

void MainWindow::upButtonClicked()
{
  int cr = m_ui.tableWidget->currentRow();
  if (cr == 0)
    return;
  QTableWidgetItem* item = 0;
  item = m_ui.tableWidget->takeItem(cr-1, 0);
  m_ui.tableWidget->setItem(cr-1, 0, m_ui.tableWidget->takeItem(cr, 0));
  m_ui.tableWidget->setItem(cr, 0, item);
  item = m_ui.tableWidget->takeItem(cr-1, 1);
  m_ui.tableWidget->setItem(cr-1, 1, m_ui.tableWidget->takeItem(cr, 1));
  m_ui.tableWidget->setItem(cr, 1, item);
  m_ui.tableWidget->setCurrentCell(cr-1, 0);
}

int MainWindow::insertButtonClicked()
{
  int cr = m_ui.tableWidget->currentRow();
  if (cr < 0) cr = 0;
  m_ui.tableWidget->insertRow(cr);
  m_ui.tableWidget->setCurrentCell(cr, 0);
  return cr;
}

void MainWindow::removeButtonClicked()
{
  int cr = m_ui.tableWidget->currentRow();
  m_ui.tableWidget->removeRow(cr);
}

void MainWindow::downButtonClicked()
{
  int cr = m_ui.tableWidget->currentRow();
  if (cr == m_ui.tableWidget->rowCount() - 1)
    return;
  QTableWidgetItem* item = 0;
  item = m_ui.tableWidget->takeItem(cr+1, 0);
  m_ui.tableWidget->setItem(cr+1, 0, m_ui.tableWidget->takeItem(cr, 0));
  m_ui.tableWidget->setItem(cr, 0, item);
  item = m_ui.tableWidget->takeItem(cr+1, 1);
  m_ui.tableWidget->setItem(cr+1, 1, m_ui.tableWidget->takeItem(cr, 1));
  m_ui.tableWidget->setItem(cr, 1, item);
  m_ui.tableWidget->setCurrentCell(cr+1, 0);
}
