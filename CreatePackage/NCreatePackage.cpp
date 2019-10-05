#include <QCryptographicHash>
#include <QDesktopWidget>
#include <QDate>
#include <QFileDialog>
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

const QString cLibrariesDirectory("Libraries");
const QString cModulesDirectory("Modules");

NCreatePackage::NCreatePackage(QWidget* parent)
         : QWidget(parent) {
  setupUi(this);
  fCreateModel();  
  fCreateValidators();
}


void NCreatePackage::fCreateModel() {
  QStringList lHeaders;
  lHeaders << tr("Directory") << tr("File Name") << tr("Size") << tr("Hash - Sha256 Base64");
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
  QFileDialog lFileDialog(this);
  lFileDialog.setFileMode(QFileDialog::Directory);
  QString lDirName = lFileDialog.getExistingDirectory(this, tr("Directory"), QString(), QFileDialog::ShowDirsOnly);
  pFileModel->removeRows(0, pFileModel->rowCount());
  pbtCreatePackage->setEnabled(fCheckUIStatus());
  if(lDirName.isEmpty())
    return;
  bool lSuccess = fProcessFiles(lDirName);
  pbtCreatePackage->setEnabled(lSuccess && fCheckUIStatus());
}

bool NCreatePackage::fProcessLibraries(const QDir& lDir) {

}

bool NCreatePackage::fProcessModules(const QDir& lDir) {

}

bool NCreatePackage::fProcessFiles(const QDir& lDir) {
  QStringList lFileEntries = lDir.entryList(QDir::Files | QDir::CaseSensitive | QDir::NoSymLinks);
  QStringList lDirEntries = lDir.entryList(QDir::Dirs | QDir::CaseSensitive | QDir::NoSymLinks | QDir::NoDotAndDotDot);
  for(const QString& lFile : lFileEntries) {
    QFile lFilePath(QString("%1/%2").arg(lDir.path()).arg(lFile));
    if(!lFilePath.open(QIODevice::ReadOnly)) {
      QString lMessage(tr("Unable to read file '%1'").arg(lFilePath.fileName()));
      emit sEventGenerated(5002003,lMessage, NMessage::ErrorMessage);
      return;
    }
    QByteArray lFileContents = lFilePath.readAll();
    QString lHash = QString(QCryptographicHash::hash(lFileContents,QCryptographicHash::Sha256).toBase64());
    QStandardItem* lDirectory = new QStandardItem(lDir.dirName());
    lDirectory->setData(lDir.path(), Qt::ToolTipRole);
    QStandardItem* lFileName = new QStandardItem(lFile);
    QStandardItem* lSize = new QStandardItem(QString::number(lFilePath.size()));
    QStandardItem* lFileHash = new QStandardItem(lHash);
    QList<QStandardItem* > lItemList({lDirectory, lFileName, lSize, lFileHash});
    pFileModel->appendRow(lItemList);
  /*     QString contentHash;
       if(file.endsWith(".pdz")) {
         QDir tmp(dir.absolutePath() + "/" + temporalDir);
         if(tmp.removeRecursively())  {
           tmp.mkpath(qApp->applicationDirPath() + "/" + temporalDir);
           QStringList extractedFiles = JlCompress::extractDir(dir.absoluteFilePath(file), qApp->applicationDirPath() + "/" + temporalDir);
           extractedFiles.sort();
           foreach(const QString& extractedFile, extractedFiles) {
             QFile fileextracted(extractedFile);
             QFileInfo finfo(extractedFile);
             if(finfo.isFile() && !finfo.isHidden() && !finfo.isSymLink()) {
               if(!fileextracted.open(QIODevice::ReadOnly))  {
                 emit eventGenerated(tr("Unable to open file '%1'").arg(extractedFile) , WCMessage::ErrorMessage);
                 return;
               }
               QByteArray fileExtContents = fileextracted.readAll();
               contentHash += QString(QCryptographicHash::hash(fileExtContents,QCryptographicHash::Sha1).toBase64());
             }
           }
           contentHash = QString(QCryptographicHash::hash(contentHash.toUtf8(),QCryptographicHash::Sha1).toBase64());
          }
          else emit eventGenerated(tr("Unable to remove temporal directory '%1'").arg(tmp.absolutePath()) , WCMessage::WarningMessage);
       }
       QString region = WCVersion::fileRegion(file);
       QString relPath = dir.absolutePath();
       relPath.remove(baseDir.absolutePath());
       if(relPath.simplified().isEmpty())
         relPath = ".";
       else
         relPath.remove(0,1); // Extract character '/'

       if(_objects.contains(object))
       {
          QString osString;
          if(!_objectMultiOS.value(object))
            osString = QString("-%1").arg(cbxOperatingSystem->itemData(cbxOperatingSystem->currentIndex(), Qt::UserRole).toString());
          QString url = QString("%1-%2%3").arg(file).arg(WCVersion::worldcoinManagerVersionCompressed(ledVersion->text())).arg(osString);

          QList<QStandardItem* > items;
          QString codRegion;
          QStandardItem* key = new QStandardItem();
          QStandardItem* master = new QStandardItem();
          QStandardItem* obj = new QStandardItem(object);
          obj->setData(_objects.value(object), Qt::UserRole);
          // Select flag
          if(region.simplified().isEmpty())
            codRegion = "1";
          else
          {
            if(_regions.contains(region))
              codRegion = _regions.value(region);
            else
              emit eventGenerated(tr("Unable to associate file '%1' to a region").arg(file) , WCMessage::WarningMessage);
          }
          QIcon flag;
          if(codRegion == "1")  // Standard
            flag.addFile(":/Resources/Images/FlagStandard.png");
          if(codRegion == "2")  // Bolivia
            flag.addFile(":/Resources/Images/FlagBolivia.png");
          //
          QStandardItem* fname = new QStandardItem(file);
          fname->setData(flag, Qt::DecorationRole);
          fname->setData(hash, Qt::ToolTipRole);
          fname->setData(WCConstants::darkBlue(), Qt::ForegroundRole);
          QStandardItem* opsys = new QStandardItem(QString::number(os));
          QStandardItem* regi = new QStandardItem(codRegion);
          QStandardItem* sizei = new QStandardItem(QString::number(filedef.size()));
          sizei->setData(WCConstants::darkGreen(), Qt::ForegroundRole);
          sizei->setData( Qt::AlignRight, Qt::TextAlignmentRole);
          QStandardItem* hashi = new QStandardItem(hash);
          hashi->setData(contentHash, Qt::UserRole + 101);
          QStandardItem* relpathi = new QStandardItem(relPath);
          QFont curFont = relpathi->font();
          curFont.setItalic(true);
          relpathi->setFont(curFont);
          relpathi->setData(url, Qt::ToolTipRole);
          relpathi->setData(WCConstants::darkBlue(), Qt::ForegroundRole);
          QStandardItem* urli = new QStandardItem(url);
          items << key << master << obj << fname << opsys << regi << sizei << hashi << relpathi << urli;
          _fileModel->appendRow(items);
       }
       else
       {
          emit eventGenerated(tr("Unable to find file '%1' in objects table").arg(file) , WCMessage::WarningMessage);
       }*/
    }
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
  if(pFileModel->rowCount() == 0)
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
