#include <QDesktopWidget>
#include <QDate>
#include <QFont>
#include <QIcon>
#include <QList>
#include <QMessageBox>
#include <QModelIndexList>
#include <QSortFilterProxyModel>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStringList>
#include <QTextStream>
#include <QTime>
#include <NConstants.h>

#include "NCreatePackage.h"

NCreatePackage::NCreatePackage(QWidget* parent)
         : QWidget(parent) {
  setupUi(this);
  fCreateModel();  
}


void NCreatePackage::fCreateModel() {
  QStringList lHeaders;
  lHeaders << tr("Directory") << tr("File Name") << tr("Size") << tr("Hash");
  pFileModel = new QStandardItemModel(0, 4, this);
  pFileModel->setHorizontalHeaderLabels(lHeaders);

  lHeaders.clear();
  lHeaders << tr("Component") << tr("Type") << tr("Log");
  pLogModel = new QStandardItemModel(0, 3, this);
  pLogModel->setHorizontalHeaderLabels(lHeaders);

  pProxyModelFiles = new QSortFilterProxyModel(this);
  pProxyModelFiles->setSortRole(Qt::DisplayRole);
  pProxyModelFiles->setDynamicSortFilter(true);
  pProxyModelFiles->setSourceModel(pFileModel);

  pProxyModelLog = new QSortFilterProxyModel(this);
  pProxyModelLog->setSortRole(Qt::DisplayRole);
  pProxyModelLog->setDynamicSortFilter(false);
  pProxyModelLog->setSourceModel(pLogModel);

  tvwFiles->setSortingEnabled(true);
  tvwFiles->setModel(pProxyModelFiles);
  tvwFiles->sortByColumn(1, Qt::AscendingOrder);
  tvwFiles->setColumnWidth(0,180);
  tvwFiles->setColumnWidth(1,180);
  tvwFiles->setColumnWidth(1,110);

  tvwFiles->setSortingEnabled(false);
  tvwFiles->setModel(pProxyModelFiles);
  tvwFiles->sortByColumn(1, Qt::AscendingOrder);
  tvwFiles->setColumnWidth(0,180);
  tvwFiles->setColumnWidth(1,110);

  lblUpgradeCommand->hide();
  tvwUpgradeCommand->hide();
  tbtAddCommand->hide();
  tbtDeleteCommand->hide();

  connect(tbtAddLog, SIGNAL(clicked()), this, SLOT(fAddLog()));
  connect(tbtDeleteLog, SIGNAL(clicked()), this, SLOT(fDeleteLog()));
}

void NCreatePackage::fLoadFiles() {

}

void NCreatePackage::fLoadTables() {
  dedDate->setDate(QDate::currentDate());
  cbxPriority->clear();
  cbxPriority->addItem(QIcon(":/Resources/Images/CircleGreen.png"), tr("Low Priority"),QString("1"));
  cbxPriority->addItem(QIcon(":/Resources/Images/CircleYellow.png"), tr("Medium Priority"),QString("2"));
  cbxPriority->addItem(QIcon(":/Resources/Images/CircleOrange.png"), tr("High Priority"),QString("3"));
  cbxPriority->addItem(QIcon(":/Resources/Images/CircleRed.png"), tr("Critical Priority"),QString("4"));
  fLoadPlatform();
}

void NCreatePackage::fLoadPlatform() {
  QFile lPlatformFile;
  lPlatformFile.setFileName("Platform.ndd");
  if(lPlatformFile.open(QFile::ReadOnly)) {
    cbxPlatform->clear();
    QTextStream lStream(&lPlatformFile);
    while(!lStream.atEnd()) {
      QString lLine = lStream.readLine();
      QStringList lRecord = lLine.split("|");
      if(lRecord.size() == 3) {
        cbxPlatform->addItem(lRecord.at(1),lRecord.at(0));
      }
      else {
        QString lMessage(tr("Platform.ndd has incorrect number of fields."));
        emit sEventGenerated(5002001, lMessage, NMessage::ErrorMessage);
      }
    }
    lPlatformFile.close();
  }
  else {
    QString lMessage(tr("Platform.ndd file could not be opened! Impossible to create package!"));
    emit sEventGenerated(5002002,lMessage, NMessage::ErrorMessage);
  }
}

void NCreatePackage::fAddLog() {

}
void NCreatePackage::fDeleteLog() {

}
