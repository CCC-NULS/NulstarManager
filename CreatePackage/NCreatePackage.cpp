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
#include <NValidatorCreator.h>

#include "NCreatePackage.h"

NCreatePackage::NCreatePackage(QWidget* parent)
         : QWidget(parent) {
  setupUi(this);
  fCreateModel();  
  fCreateValidators();
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

  tvwLog->setSortingEnabled(false);
  tvwLog->setModel(pProxyModelLog);
  tvwLog->setColumnWidth(0,110);
  tvwLog->setColumnWidth(1,110);

  lblUpgradeCommand->hide();
  tvwUpgradeCommand->hide();
  tbtAddCommand->hide();
  tbtDeleteCommand->hide();

  dedDate->setProperty("Status", true);
  txtDescription->setProperty("Status", true);
  txtDescription->setStyleSheet(QString("background-color: rgb%1").arg(NConstants::colorToRgbText(NConstants::lightBlue())));

  connect(tbtAddLog, SIGNAL(clicked()), this, SLOT(fAddLog()));
  connect(tbtDeleteLog, SIGNAL(clicked()), this, SLOT(fDeleteLog()));
  connect(pbtLoadFiles, SIGNAL(clicked()), this, SLOT(fLoadFiles()));
}

void NCreatePackage::fCreateValidators() {
  NValidatorCreator::createValidator(ledName, NConstants::NameRegExp, cMinNameSize, cMaxNameSize);
  NValidatorCreator::createValidator(ledVersion, NConstants::VersionRegExp, 5, 8);

  fSetStatus(ledName, false);
  fSetStatus(ledVersion, false);

  connect(ledName, SIGNAL(textChanged(QString)), this, SLOT(fValidateParameter(QString)));
  connect(ledVersion, SIGNAL(textChanged(QString)), this, SLOT(fValidateParameter(QString)));
  connect(pbtCancel, SIGNAL(clicked()), this, SLOT(close()));
  connect(pbtCreatePackage, SIGNAL(clicked()), this, SLOT(fCreatePackage()));
  connect(pLogModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(fVerifyLogModel(QStandardItem*)));

}

void NCreatePackage::fSetStatus(QLineEdit* rLineEdit, bool fStatus) {
  QColor lColor;
  if(fStatus){
    lColor = NConstants::lightBlue();
    rLineEdit->setProperty("Status", true);
  }
  else {
    lColor = NConstants::lightRed();
     rLineEdit->setProperty("Status", false);
  }
  rLineEdit->setStyleSheet(QString("background-color: rgb%1").arg(NConstants::colorToRgbText(lColor)));
  pbtCreatePackage->setEnabled(fCheckUIStatus());
}

void NCreatePackage::fLoadFiles() {

}

void NCreatePackage::fVerifyLogModel(QStandardItem* rItem) {
  disconnect(pLogModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(fVerifyLogModel(QStandardItem*)));
  if(!rItem || ((rItem->column() < 0) || (rItem->column() > 2)))
      return;
  if(rItem->data(Qt::DisplayRole).toString().simplified().isEmpty()) {
    rItem->setData("0", NConstants::StatusRole);
    rItem->setData(QBrush(NConstants::lightRed()), Qt::BackgroundRole);
  }
  else {
    rItem->setData("1", NConstants::StatusRole);
    rItem->setData(QBrush(NConstants::lightBlue()), Qt::BackgroundRole);
  }
  rItem->setData(rItem->data(Qt::DisplayRole).toString(), Qt::UserRole);
  pbtCreatePackage->setEnabled(fCheckUIStatus());
  connect(pLogModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(fVerifyLogModel(QStandardItem*)));
}

bool NCreatePackage::fCheckUIStatus() const
{
  if(!fStatus(ledVersion))
    return false;
  if(!fStatus(ledName))
    return false;

  if(pLogModel->rowCount() == 0)
    return false;

  for(int i = 0; i < pLogModel->rowCount(); i++)
  {
    if(pLogModel->item(i,0)->data(NConstants::StatusRole).toString() == "0")
      return false;
    if(pLogModel->item(i,1)->data(NConstants::StatusRole).toString() == "0")
      return false;
    if(pLogModel->item(i,2)->data(NConstants::StatusRole).toString() == "0")
      return false;
  }
  return true;
}

void NCreatePackage::fCreatePackage() {

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
  disconnect(pLogModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(fVerifyLogModel(QStandardItem*)));
  QStandardItem* rComponent = new QStandardItem();
  //rComponent->setData("NULL", Qt::UserRole);
  QStandardItem* rType = new QStandardItem();
 // rType->setData(ledCode->text(), Qt::UserRole);
  QStandardItem* rLog = new QStandardItem();

  QList<QStandardItem* > lList;
  lList << rComponent << rType << rLog;
  pLogModel->appendRow(lList);
  fVerifyLogModel(rComponent);
  fVerifyLogModel(rType);
  fVerifyLogModel(rLog);
  connect(pLogModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(fVerifyLogModel(QStandardItem*)));
}

void NCreatePackage::fDeleteLog() {
  QModelIndexList lSelection = tvwLog->selectionModel()->selectedIndexes();
  if(!lSelection.count())
    return;
  pLogModel->removeRow(lSelection.at(0).row());
  pbtCreatePackage->setEnabled(fCheckUIStatus());
}

void NCreatePackage::fValidateParameter(const QString& lParameter) {
  QString lCurrentValue = lParameter;
  QLineEdit* lParam = qobject_cast<QLineEdit*> (sender());
  Q_ASSERT(lParam);

  int lPos = 0;
  QValidator::State lState = lParam->validator()->validate(lCurrentValue, lPos);
  if(lState == QValidator::Acceptable)
    fSetStatus(lParam, true);
  else
    fSetStatus(lParam, false);
}
